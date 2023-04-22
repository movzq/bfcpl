#include "assembly.h"

void _read (FILE*);
unsigned _highestBytePos (unsigned*, char);

int main (int argc, char** argv) {
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        puts("u: No file given or file given as argument does not exist!");
        return 0;
    }

    _read(file);
    return 0;
}

void _read (FILE* file)
{
    char inst;
    unsigned bytepos = 1;

    while ((inst = fgetc(file)) != EOF) {
        switch (inst) {
            case '+': case '-':
            case ',': case '.':
            case '[': case ']': {
                asm_newToken(inst);
                break;
            }
            case '<': case '>': {
                _highestBytePos(&bytepos, inst);
                asm_newToken(inst);
                break;
            }
        }
    }

    fclose(file);
    asm_gen(_highestBytePos(NULL, 0));
}

unsigned _highestBytePos (unsigned* bytepos, char kaz)
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
