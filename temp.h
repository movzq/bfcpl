#ifndef _TEMP_H
#define _TEMP_H
#include "ustring/ustring.h"

struct temp {
    struct ustr* body;
    struct ustr* temp;
};

struct temp temp_setMain ();
void temp_changeValue (struct ustr*, unsigned, unsigned, char);

#endif
