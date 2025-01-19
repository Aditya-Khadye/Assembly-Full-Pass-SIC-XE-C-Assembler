#ifndef SYMBOL_H
#define SYMBOL_H

#define MAX_LABEL_LEN  31
#define SYMBOL_TABLE_SIZE 100

typedef struct {
    char  label[MAX_LABEL_LEN + 1];
    int   address;
} Symbol;

int lookupSymbol(const char *label);
void insertSymbol(const char *label, int address);

#endif // SYMBOL_H