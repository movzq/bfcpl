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
                      "\tsubq \t $12, %rsp\n"
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
                 "\tmovzbl \t (%rax), %edx\n" +
                 perform + " \t $" + std::to_string(times) + ", %edx\n"
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

        if (--times) Assembly::writePrint(body, times);
    }

    void writeInput (std::string *body, unsigned times)
    {
        *body += "\tmovl \t $0, %eax\n"
                 "\tcall \t getchar\n"
                 "\tmovl \t %eax, %edx\n"
                 "\tmovq \t -8(%rbp), %rax\n"
                 "\tmovb \t %dl, (%rax)\n";

        if (--times) Assembly::writeInput(body, times);
    }

    Label writeNewLoop (std::string* body, const bfTokenType type)
    {
        static int numloops = 0;
        const std::string labelnum = std::to_string(numloops++);
        const std::string cmpto = (type == bfTokenType::b_loopu0) ? "0" : "31";
        const std::string jmpif = (type == bfTokenType::b_loopu0) ? "jne" : "jg";

        *body += "\tcall \t LC" + std::to_string(numloops) + "\n";
        return Label {
            .sample = "LC" + std::to_string(numloops) + ":\n"
                      "\tmovq \t -8(%rbp), %rax\n"
                      "\tmovzbl \t (%rax), %r9d\n"
                      "\tcmpl \t $" + cmpto + ", %r9d\n"
                      "\t" + jmpif + " \t LB" + std::to_string(numloops) + "\n"
                      "\tret\n"
                      "LB" + std::to_string(numloops) + ":\n"
                      "%s"
                      "\tjmp \t LC" + std::to_string(numloops) + "\n"
                      "\tret\n"
        };
    }

    void writeThisLabel (FILE* outfile)
    {
        fprintf(outfile, __labels.back().sample.c_str(), __labels.back().body.c_str());
        __labels.pop_back();
    }
}

void asm_newToken (const bfTokenType type)
{
    static bfTokenType prevtype = bfTokenType::unk;
    bool canbe_repetead = type != bfTokenType::b_loopu0 && type != bfTokenType::e_loopu0 &&
                          type != bfTokenType::b_loopuc && type != bfTokenType::e_loopuc;

    if (prevtype == type && canbe_repetead) {
        __tokens.back().times++;
        return;
    }

    __tokens.push_back(bfToken {
        .times = 1,
        .type = type
    });
    prevtype = type;
}

void asm_generate (unsigned bytes)
{
    FILE* asmfile = fopen("out.s", "w");
    fprintf(asmfile, ".globl main\n");
    fprintf(asmfile, ".type main, @function\n");

    fprintf(asmfile, ".globl mems\n");
    fprintf(asmfile, ".type mems, @object\n");
    fprintf(asmfile, ".size mems, %d\n", bytes);

    fprintf(asmfile, ".bss\n");
    fprintf(asmfile, "\t.align 1\n");
    fprintf(asmfile, "\tmems:\n");
    fprintf(asmfile, "\t.zero %d\n", bytes);
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
        
        else if (type == bfTokenType::inp)
            Assembly::writeInput(currbody, token.times);

        else if ((type == bfTokenType::e_loopu0 || type == bfTokenType::e_loopuc) && __labels.size() >= 2) {
            Assembly::writeThisLabel(asmfile);
            currbody = &__labels.back().body;
        }

        else if (type == bfTokenType::b_loopu0 || type == bfTokenType::b_loopuc) {
            __labels.push_back(Assembly::writeNewLoop(currbody, type));
            currbody = &__labels.back().body;
        }
    }

    Assembly::writeThisLabel(asmfile);
    fclose(asmfile);
}

