#include "label.h"
#include <string.h>

struct Label label_main ()
{
    struct Label lbl = {
        .temp = ustr_make(),
        .body = ustr_make()
    };

    const char* temp = "main:\n"
                       "\tpushq \t %%rbp\n"
                       "\tmovq  \t %%rsp, %%rbp\n"
                       "\tsubq  \t $16, %%rsp\n"
                       "\tmovq  \t $mems, -8(%%rbp)\n"
                       "%s"
                       "\tmovl \t $0, %%eax\n"
                       "\tleave\n"
                       "\tret\n";
    ustr_append(lbl.temp, temp);
    return lbl;
}

void label_manipulateMem (struct ustr* body, unsigned times, char kaz)
{
    const char* code = "\tmovq   \t -8(%%rbp), %%rax\n"
                       "\tmovzbl \t (%%rax), %%eax\n"
                       "\t%s \t $%d, %%eax\n"
                       "\tmovl   \t %%eax, %%edx\n"
                       "\tmovq   \t -8(%rbp), %%rax\n"
                       "\tmovb   \t %%dl, (%%rax)\n";

    size_t len = strlen(code) + 10;
    char* write = (char*) malloc(len);

    snprintf(write, len, code, (kaz == '-') ? "subl" : "addl", times);
    ustr_append(body, write);
    free(write);
}

void label_goThrough (struct ustr* body, unsigned times, char kaz)
{
    const char* code = "\t%s \t $%d, -8(%rbp)\n";
    size_t len = strlen(code) + 10;
    char* write = (char*) malloc(len);

    snprintf(write, len, code, (kaz == '<') ? "subq" : "addq", times);
    ustr_append(body, write);
    free(write);
}

void label_print (struct ustr* body, unsigned times)
{
    const char* code = "\tmovq   \t -8(%rbp), %rax\n"
                       "\tmovzbl \t (%rax), %eax\n"
                       "\tmovsbl \t %al, %eax\n"
                       "\tmovl   \t %eax, %edi\n"
                       "\tcall   \t putchar\n";
    ustr_append(body, code);
    if (--times) label_print(body, times);
}
