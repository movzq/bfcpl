#include "temp.h"

struct temp temp_setMain ()
{
    struct temp tmpl = {
        .body = ustr_make(),
        .temp = ustr_make()
    };

    const char* mainbody = "%s"
                           "\tmovl \t $0, %%eax\n"
                           "\tpopq \t %%rbp\n"
                           "\tret\n";
    ustr_append(tmpl.body, mainbody);
    return tmpl;
}

void temp_changeValue (struct ustr*, unsigned times, unsigned bytepos, char kase)
{}
