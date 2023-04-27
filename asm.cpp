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
                      "\tsubq \t $16, %rsp\n" // XXX: Can be improved.
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

        // XXX: could be improved (Gotta check).
        if (--times) Assembly::writePrint(body, times);
    }

    Label writeNewLoop (std::string* body)
    {
        static int numloops = 0;
        const std::string labelnum = std::to_string(numloops++);

        *body += "\tcall \t LC" + std::to_string(numloops) + "\n";
        return Label {
            .sample = "LC" + std::to_string(numloops) + ":\n"
                      "\tmovq \t -8(%rbp), %rax\n"
                      "\tmovzbl \t (%rax), %r9d\n"
                      "\ttest \t %r9d, %r9d\n"
                      "\tjne \t LB" + std::to_string(numloops) + "\n"
                      "\tret\n"
                      "LB" + std::to_string(numloops) + ":\n"
                      "%s"
                      "\tjmp LC" + std::to_string(numloops) + "\n"
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
    if (prevtype == type && type != bfTokenType::blo && type != bfTokenType::elo) {
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
    fprintf(asmfile, ".size mems, 30000\n"); // XXX: Can be improved.

    fprintf(asmfile, ".bss\n");
    fprintf(asmfile, "\t.align 1\n");
    fprintf(asmfile, "\tmems:\n");
    fprintf(asmfile, "\t.zero 30000\n"); // XXX: Can be improved.
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

        else if (type == bfTokenType::elo && __labels.size() >= 2) {
            Assembly::writeThisLabel(asmfile);
            currbody = &__labels.back().body;
        }

        else if (type == bfTokenType::blo) {
            __labels.push_back(Assembly::writeNewLoop(currbody));
            currbody = &__labels.back().body;
        }
    }

    Assembly::writeThisLabel(asmfile);
    fclose(asmfile);
}

