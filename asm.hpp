#ifndef _ASM_HPP
#define _ASM_HPP
#include <iostream>
#include <vector>

enum class bfTokenType  {
    inc = '+', dec = '-',
    prv = '<', nxt = '>',
    out = '.', inp = ',',
    blo = '[', elo = ']',
    unk = '?'
};

typedef struct {
    unsigned times;
    bfTokenType type;
} bfToken;

void asm_newToken (const bfTokenType);
void asm_generate ();

#endif
