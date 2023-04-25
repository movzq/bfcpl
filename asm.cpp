#include "asm.hpp"

namespace {
    typedef struct {
        std::string sample;
        std::string body;
    } Label;

    std::vector<bfToken> __tokens;
    std::vector<Label> __labels;
}

namespace Assembly {
    void writeMain () 
    {
        __labels.push_back(Label {
            .sample = "main:\n"
                      "\tpushq \t %rbp\n"
                      "\tmovq \t %rsp, %rbp\n"
                      "\tsubq \t $16, %rsp\n"
                      "\tmovq \t $mems, -8(%rbp)\n"
                      "%s"
                      "\tleave\n"
                      "\tret\n"
        });
    }

    void writeIncDec (std::string* body, unsigned times, const bfTokenType type)
    {
        const std::string perform = (type == bfTokenType::inc) ? "\taddl" : "\tsubl";
        *body += "\tmovq \t -8(%rbp), %rax\n"
                 "\tmovzbl \t (%rax), %eax\n" +
                 perform + " \t $" + std::to_string(times) + ", %eax\n"
                 "\tmovl \t %eax, %edx\n"
                 "\tmovq \t -8(%rbp), %rax\n"
                 "\tmovb \t %dl, (%rax)\n";
    }

    void writeThroughMems (std::string* body, unsigned times, const bfTokenType type)
    {
        const std::string perform = (type == bfTokenType::nxt) ? "\taddq" : "\tsubq";
        *body += perform + " \t $" + std::to_string(times) + ", -8(%rbp)\n";
    }

    void writePrint (std::string* body, unsigned times)
    {
        *body += "\tmovq \t -8(%rbp), %rax\n"
                 "\tmovzbl \t (%rax), %edi\n"
                 "\tcall \t putchar\n";

        // could be improved (Gotta check).
        if (--times) Assembly::writePrint(body, times);
    }
}

void asm_newToken (const bfTokenType type)
{
    static bfTokenType prevtype = bfTokenType::unk;
    if (prevtype == type) {
        __tokens.back().times++;
        return;
    }

    __tokens.push_back(bfToken {
        .times = 1,
        .type = type
    });
    prevtype = type;
}

void asm_generate ()
{
    FILE* asmfile = fopen("out.s", "w");
    fprintf(asmfile, ".globl main\n");
    fprintf(asmfile, ".type main, @function\n");

    fprintf(asmfile, ".globl mems\n");
    fprintf(asmfile, ".type mems, @object\n");
    fprintf(asmfile, ".size mems, 30000\n");

    fprintf(asmfile, ".bss\n");
    fprintf(asmfile, "\t.align 1\n");
    fprintf(asmfile, "\tmems:\n");
    fprintf(asmfile, "\t.zero 30000\n");
    fprintf(asmfile, ".text\n");

    Assembly::writeMain();
    std::string* currbody = &__labels.back().body;

    for (const bfToken token:__tokens) {
        const bfTokenType type = token.type;

        if (type == bfTokenType::inc || type == bfTokenType::dec)
            Assembly::writeIncDec(currbody, token.times, type);

        else if (type == bfTokenType::nxt || type == bfTokenType::prv)
            Assembly::writeThroughMems(currbody, token.times, type);

        else if (type == bfTokenType::out)
            Assembly::writePrint(currbody, token.times);
    }

    fprintf(
        asmfile,
        __labels.back().sample.c_str(),
        __labels.back().body.c_str()
    );
    fclose(asmfile);
}

