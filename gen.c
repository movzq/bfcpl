#include "gen.h"

static struct bfToken* __tokens;
static size_t __numtokens;

void _evaluateTokens ();

void gen_init ()
{
    __tokens = (struct bfToken*) malloc(0);
    __numtokens = 0;
    gen_newToken(BF_TYPE_NXT);
}

void gen_newToken (const enum bfTokenType type)
{
    static enum bfTokenType prevtype = BF_NO_TOKEN;
    if (type == prevtype) {
        __tokens[__numtokens - 1].times++;
        return;
    }

    struct bfToken new = { .type = type, .times = 1 };
    __tokens = (struct bfToken*) realloc(
        __tokens,
        ++__numtokens * sizeof(struct bfToken)
    );

    __tokens[__numtokens - 1] = new;
    prevtype = type;
}

void gen_genFile (unsigned bytes)
{
    FILE* out = fopen("out.s", "w");
    fprintf(out, ".globl main\n");
    fprintf(out, ".type main, @function\n");

    fprintf(out, ".globl vect\n");
    fprintf(out, ".type vect, @object\n");
    fprintf(out, ".size vect, %d\n", bytes);

    fprintf(out, ".bss\n");
    fprintf(out, ".align 1\n");
    fprintf(out, "\tmems:\n");
    fprintf(out, "\t.zero %d\n", bytes);

    fprintf(out, ".text\n");
    fprintf(out, "main:\n");
    fprintf(out, "\tpushq \t %%rbp\n");
    fprintf(out, "\tmovq  \t %%rsp, %%rbp\n");

    _evaluateTokens();
    fclose(out);
}

// TODO: Think in a good way to do this.
void _evaluateTokens ()
{}
