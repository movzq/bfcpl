#include "asm.hpp"

void _read (FILE*);
unsigned _bytePosition (unsigned*, const char);

int main (int argc, char** argv) {
    if (argc == 1) {
        puts("u: This compiler needs a file to compile and the number of bytes to use (optional).");
        return 0;
    }

    FILE* bfile = fopen(argv[1], "r");
    _read(bfile);

    unsigned bytesto_use = (argc == 3) ? atoi(argv[2]) : 30000;
    asm_generate(bytesto_use);
    return 0;
}

void _read (FILE* file) {
    if (!file) {
        puts("e: File given as argument does not exist!");
        exit(1);
    }

    char inst;
    while ((inst = fgetc(file)) != EOF) {
        switch (inst) {
            case '+': case '-':
            case '[': case ']':
            case '(': case ')':
            case '<': case '>':
            case ',': case '.': {
                asm_newToken((bfTokenType) inst);
                break;
            }
        }
    }

    fclose(file);
}
