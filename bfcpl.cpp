#include "asm.hpp"

void _read (FILE*);

int main (int argc, char** argv) {
    if (argc == 1) {
        puts("u: This compiler as any other one needs an argument.");
        return 0;
    }

    FILE* bfile = fopen(argv[1], "r");
    _read(bfile);
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
            case '<': case '>':
            case ',': case '.': {
                asm_newToken((bfTokenType) inst);
            }
        }
    }

    fclose(file);
    asm_generate();
}
