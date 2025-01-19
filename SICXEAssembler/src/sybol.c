#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

static Symbol symbolTable[SYMBOL_TABLE_SIZE];
static int symbolCount = 0;

int lookupSymbol(const char *label) {
    for(int i = 0; i < symbolCount; i++) {
        if(strcmp(symbolTable[i].label, label) == 0) {
            return symbolTable[i].address;
        }
    }
    return -1; // Symbol not found
}

void insertSymbol(const char *label, int address) {
    if(symbolCount >= SYMBOL_TABLE_SIZE) {
        fprintf(stderr, "Error: Symbol table overflow.\n");
        exit(EXIT_FAILURE);
    }
    if(lookupSymbol(label) != -1) {
        fprintf(stderr, "Error: Duplicate symbol '%s'\n", label);
        exit(EXIT_FAILURE);
    }
    strncpy(symbolTable[symbolCount].label, label, MAX_LABEL_LEN);
    symbolTable[symbolCount].label[MAX_LABEL_LEN] = '\0';
    symbolTable[symbolCount].address = address;
    symbolCount++;
}