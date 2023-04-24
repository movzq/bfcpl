#ifndef _ASSEMBLY_H
#define _ASSEMBLY_H
#include "label.h"

enum bfTokenType {
    BF_INC_TYPE = '+',
    BF_DEC_TYPE = '-',
    BF_NXT_TYPE = '>',
    BF_PRV_TYPE = '<',
    BF_OUT_TYPE = '.',
    BF_UNK_TYPE = '?'
};

struct bfToken {
    unsigned times;
    enum bfTokenType type;
};

void asm_init ();
void asm_newToken (const enum bfTokenType);
void asm_gen (unsigned);

#endif
