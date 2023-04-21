#include "gen.h"

void _read (FILE*);
unsigned _bytesUsed (unsigned*, const char);

int main (int argc, char** argv) {
    FILE* bfile = fopen(argv[1], "r");
    if (!bfile) {
        puts("u: This compiler as any other one needs a file to compile!");
        return 0;
    }

    gen_init();
    _read(bfile);
    return 0;
}

void _read (FILE* bfile)
{
    char mnemonic;
    unsigned bytes = 1;

    while((mnemonic = fgetc(bfile)) != EOF) {
        switch (mnemonic) {
            case '+': case '-':
            case '[': case ']':
            case '.': case ',': {
                gen_newToken(mnemonic);
                break;
            }

            case '<': case '>': {
                gen_newToken(mnemonic);
                _bytesUsed(&bytes, mnemonic);
                break;
            }
        }
    }

    fclose(bfile);
    gen_genFile(_bytesUsed(NULL, 0));
}

unsigned _bytesUsed (unsigned* bytepos, const char kase)
{
    static unsigned highestbyte_pos = 1;
    if (!bytepos)
        return highestbyte_pos;

    if (kase == '<') {
        if ((*bytepos - 1))
            *bytepos -= 1;
        return 0;
    }

    if (*bytepos == highestbyte_pos)
        highestbyte_pos++;
    *bytepos += 1;
    return 0;
}
