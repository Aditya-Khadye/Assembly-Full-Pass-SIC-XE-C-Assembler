#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"
#include "symbol.h"
#include "opcode.h"

#define MAX_OPERAND_LEN 63
#define MAX_LINE_LEN 128
#define INTERMEDIATE_SIZE 200

typedef struct {
    int   address;
    char  label[MAX_LABEL_LEN + 1];
    char  opcode[MAX_OPCODE_LEN + 1];
    char  operand[MAX_OPERAND_LEN + 1];
    char  comment[MAX_LINE_LEN + 1];
} IntermediateLine;

static IntermediateLine intermediate[INTERMEDIATE_SIZE];
static int intermediateCount = 0;

static int startAddress = 0;
static int programLength = 0;

/* ------------------------------------------------
   PASS 1: Build symbol table & intermediate info
   ------------------------------------------------ */
void pass1(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    int LOCCTR = 0;
    char line[MAX_LINE_LEN + 1];
    intermediateCount = 0;

    while(fgets(line, sizeof(line), fp)) {
        char *p = strchr(line, '\n');
        if(p) *p = '\0';

        // Skip empty or comment lines.
        if(line[0] == '\0' || line[0] == '.') {
            IntermediateLine *iLine = &intermediate[intermediateCount++];
            iLine->address = LOCCTR;
            iLine->label[0] = '\0';
            iLine->opcode[0] = '\0';
            iLine->operand[0] = '\0';
            strncpy(iLine->comment, line, MAX_LINE_LEN);
            continue;
        }

        char tmpLabel[MAX_LABEL_LEN+1] = "";
        char tmpOpcode[MAX_OPCODE_LEN+1] = "";
        char tmpOperand[MAX_OPERAND_LEN+1] = "";
        int fields = sscanf(line, "%s %s %s", tmpLabel, tmpOpcode, tmpOperand);

        char labelStr[MAX_LABEL_LEN+1] = "";
        char opcodeStr[MAX_OPCODE_LEN+1] = "";
        char operandStr[MAX_OPERAND_LEN+1] = "";

        if(fields == 3) {
            strcpy(labelStr, tmpLabel);
            strcpy(opcodeStr, tmpOpcode);
            strcpy(operandStr, tmpOperand);
        } else if(fields == 2) {
            strcpy(opcodeStr, tmpLabel);
            strcpy(operandStr, tmpOpcode);
        } else if(fields == 1) {
            strcpy(opcodeStr, tmpLabel);
        }

        if(labelStr[0] != '\0') {
            insertSymbol(labelStr, LOCCTR);
        }

        IntermediateLine *iLine = &intermediate[intermediateCount++];
        iLine->address = LOCCTR;
        strncpy(iLine->label, labelStr, MAX_LABEL_LEN);
        iLine->label[MAX_LABEL_LEN] = '\0';
        strncpy(iLine->opcode, opcodeStr, MAX_OPCODE_LEN);
        iLine->opcode[MAX_OPCODE_LEN] = '\0';
        strncpy(iLine->operand, operandStr, MAX_OPERAND_LEN);
        iLine->operand[MAX_OPERAND_LEN] = '\0';
        iLine->comment[0] = '\0';

        if(strcmp(opcodeStr, "START") == 0) {
            if(strlen(operandStr) > 0) {
                startAddress = (int)strtol(operandStr, NULL, 16);
            } else {
                startAddress = 0;
            }
            LOCCTR = startAddress;
            iLine->address = LOCCTR;
            continue;
        }

        if(strcmp(opcodeStr, "END") == 0) {
            break;
        }

        int format = 3;
        int op = getOpcode(opcodeStr, &format);
        if(op != -1) {
            LOCCTR += format;
        } else {
            if(strcmp(opcodeStr, "WORD") == 0) {
                LOCCTR += 3;
            } else if(strcmp(opcodeStr, "RESW") == 0) {
                LOCCTR += 3 * atoi(operandStr);
            } else if(strcmp(opcodeStr, "RESB") == 0) {
                LOCCTR += atoi(operandStr);
            } else if(strcmp(opcodeStr, "BYTE") == 0) {
                LOCCTR += 1;  // Simplified assumption
            } else {
                fprintf(stderr, "Warning: Unrecognized opcode '%s'\n", opcodeStr);
            }
        }
    }

    fclose(fp);
    programLength = LOCCTR - startAddress;
}

/* ------------------------------------------------
   PASS 2: Generate object code
   ------------------------------------------------ */
void pass2(const char *objFilename) {
    FILE *objFile = fopen(objFilename, "w");
    if(!objFile) {
        perror("Error opening object file");
        exit(EXIT_FAILURE);
    }

    fprintf(objFile, "H^%06X^%06X\n", startAddress, programLength);

    char textRecord[70] = "";
    int textStart = 0;
    int textLength = 0;

    for(int i = 0; i < intermediateCount; i++) {
        IntermediateLine *line = &intermediate[i];
        char generatedCode[10] = "";
        int objCode = 0;
        int format = 3;

        if(line->opcode[0] == '\0' && line->label[0] == '\0') continue;
        if(strcmp(line->opcode, "START") == 0 || strcmp(line->opcode, "END") == 0
           || strcmp(line->opcode, "RESW") == 0 || strcmp(line->opcode, "RESB") == 0) {
            continue;
        }

        int op = getOpcode(line->opcode, &format);
        if(op != -1) {
            int addressField = 0;
            if(strlen(line->operand) > 0) {
                int symAddr = lookupSymbol(line->operand);
                if(symAddr == -1) symAddr = 0;
                addressField = symAddr & 0xFFF;
            }
            int nixbpe = 0x30;  // Simplified to direct addressing bits
            int byte1 = (nixbpe & 0x3F) << 2;
            objCode = (op << 16) | (byte1 << 8) | (addressField & 0xFF);
            sprintf(generatedCode, "%06X", objCode);
        } else {
            if(strcmp(line->opcode, "WORD") == 0) {
                int val = (int)strtol(line->operand, NULL, 0);
                sprintf(generatedCode, "%06X", val & 0xFFFFFF);
            } else if(strcmp(line->opcode, "BYTE") == 0) {
                sprintf(generatedCode, "EE");  // Simplified handling
            }
        }

        if(strlen(generatedCode) > 0) {
            if(textLength == 0) {
                textStart = line->address;
            }
            if((textLength + (int)strlen(generatedCode)/2) > 30) {
                fprintf(objFile, "T^%06X^%02X^%s\n", textStart, textLength, textRecord);
                textLength = 0;
                textRecord[0] = '\0';
                textStart = line->address;
            }
            strcat(textRecord, generatedCode);
            textLength += (int)strlen(generatedCode) / 2;
        }
    }

    if(textLength > 0) {
        fprintf(objFile, "T^%06X^%02X^%s\n", textStart, textLength, textRecord);
    }

    fprintf(objFile, "E^%06X\n", startAddress);
    fclose(objFile);
}