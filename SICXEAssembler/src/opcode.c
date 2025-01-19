#include <string.h>
#include "opcode.h"

static Opcode opcodeTable[OPCODE_TABLE_SIZE] = {
    {"LDA",  0x00, 3},
    {"STA",  0x0C, 3},
    {"LDCH", 0x50, 3},
    {"STCH", 0x54, 3},
    {"ADD",  0x18, 3},
    {"SUB",  0x1C, 3},
    {"MUL",  0x20, 3},
    {"DIV",  0x24, 3},
    {"COMP", 0x28, 3},
    {"TIX",  0x2C, 3},
    {"J",    0x3C, 3},
    {"JEQ",  0x30, 3},
    {"JLT",  0x38, 3},
    {"RSUB", 0x4C, 3},
    {"LDX",  0x04, 3}
};

int getOpcode(const char *mnemonic, int *format) {
    for(int i = 0; i < OPCODE_TABLE_SIZE; i++) {
        if(strcmp(opcodeTable[i].mnemonic, mnemonic) == 0) {
            if(format) *format = opcodeTable[i].format;
            return opcodeTable[i].opcode;
        }
    }
    return -1;
}