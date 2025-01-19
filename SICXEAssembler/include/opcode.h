#ifndef OPCODE_H
#define OPCODE_H

#define MAX_OPCODE_LEN 31
#define OPCODE_TABLE_SIZE 15

typedef struct {
    char  mnemonic[MAX_OPCODE_LEN + 1];
    int   opcode;
    int   format;  // Assume format 3 for simplicity
} Opcode;

int getOpcode(const char *mnemonic, int *format);

#endif // OPCODE_H