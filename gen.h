#ifndef _GEN_H
#define _GEN_H
#include "temp.h"
#include <stdio.h>

enum bfTokenType {
    BF_NO_TOKEN = '0',
    BF_TYPE_INC = '+',
    BF_TYPE_DEC = '-',
    BF_TYPE_PRV = '<',
    BF_TYPE_NXT = '>'
};

struct bfToken {
    unsigned times;
    enum bfTokenType type;
};

void gen_init ();
void gen_newToken (const enum bfTokenType);
void gen_genFile (unsigned);

#endif
