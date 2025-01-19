#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <sourcefile> <objectfile>\n", argv[0]);
        return 1;
    }

    pass1(argv[1]);
    pass2(argv[2]);

    printf("Assembly completed. Object file: %s\n", argv[2]);
    return 0;
}