#include "asm.hpp"

void _read (FILE*);
unsigned _bytePosition (unsigned*, const char);

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
    unsigned bytepos = 1;

    while ((inst = fgetc(file)) != EOF) {
        switch (inst) {
            case '+': case '-':
            case '[': case ']':
            case '(': case ')':
            case ',': case '.': {
                asm_newToken((bfTokenType) inst);
                break;
            }

            case '<': case '>': {
                asm_newToken((bfTokenType) inst);
                _bytePosition(&bytepos, inst);
                break;
            }
        }
    }

    fclose(file);
    asm_generate(_bytePosition(NULL, 0));
}

unsigned _bytePosition (unsigned* bytepos, const char kaz)
{
    static unsigned highestpos = 1;
    if (!bytepos)
        return highestpos;

    if (kaz == '<') {
        if ((*bytepos - 1))
            *bytepos -= 1;
        return 0;
    }

    if (*bytepos == highestpos)
        highestpos++;
    *bytepos += 1;
    return 0;
}
