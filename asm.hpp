#ifndef _ASM_HPP
#define _ASM_HPP
#include <iostream>
#include <vector>

enum class bfTokenType  {
    inc = '+', dec = '-',
    prv = '<', nxt = '>',
    out = '.', inp = ',',
    b_loopu0 = '[', e_loopu0 = ']',
    b_loopuc = '(', e_loopuc = ')',
    unk = '?'
};

typedef struct {
    unsigned times;
    bfTokenType type;
} bfToken;

void asm_newToken (const bfTokenType);
void asm_generate (unsigned);

#endif
