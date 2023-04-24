#ifndef _LABEL_H
#define _LABEL_H
#include "ustring/ustring.h"
#include <stdio.h>

struct Label {
    struct ustr* temp;
    struct ustr* body;
};

struct Label label_main ();
void label_manipulateMem (struct ustr*, unsigned, char);
void label_goThrough (struct ustr*, unsigned, char);
void label_print (struct ustr*, unsigned);

#endif
