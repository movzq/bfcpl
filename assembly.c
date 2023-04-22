#include "assembly.h"

static struct bfToken* __tokens;
static size_t __numtokens;

void _readTokens ();

void asm_init ()
{
    __tokens = (struct bfToken*) malloc(0);
    __numtokens = 1;

    asm_newToken(BF_NXT_TYPE);
}

void asm_newToken (const enum bfTokenType type)
{
    static enum bfTokenType prev = BF_UNK_TYPE;
    if (prev == type) {
        __tokens[__numtokens - 1].times += 1;
        return;
    }


    struct bfToken tkn = { .type = type, .times = 1 };
    __tokens = (struct bfToken*) realloc(
        __tokens,
        ++__numtokens * sizeof(struct bfToken)
    );

    __tokens[__numtokens - 1] = tkn;
    prev = type;
}

void asm_gen (unsigned bytes)
{
    FILE* outfile = fopen("out.s", "w");
    fprintf(outfile, ".globl main\n");
    fprintf(outfile, ".type main, @function\n");
    fprintf(outfile, ".globl mems\n");
    fprintf(outfile, ".type mems, @object\n");
    fprintf(outfile, ".size mems, %d\n", bytes);

    fprintf(outfile, ".bss\n");
    fprintf(outfile, "\t.align 1\n");
    fprintf(outfile, "\tmems:\n");
    fprintf(outfile, "\t.zero %d\n", bytes);
    fprintf(outfile, ".text\n");

    _readTokens();
    fclose(outfile);
}

void _readTokens ()
{
    /* The first label will be for all code into the main function
     * and the second is for loops (If there's) and when that loop
     * is already used just wirte all its code and wait for the next
     * one. */
    struct Label* labels = (struct Label*) malloc(sizeof(struct Label) * 2);
    labels[0] = label_main();

    struct ustr* currbody = labels[0].body;
    for (size_t i = 0; i < __numtokens; i++) {
        const enum bfTokenType type = __tokens[i].type;

        if (type == BF_INC_TYPE || type == BF_DEC_TYPE) {
            label_manipulateMem(currbody, __tokens[i].times, type);
        }
    }


    free(labels);
}
