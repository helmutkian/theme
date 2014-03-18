#ifndef _PRINT_H_
#define _PRINT_H_

#include "scm_value.h"
#include <stdio.h>

typedef void (*printer)(FILE *, struct value *);

void print_fixnum(FILE *out, struct value *val);
void print_flonum(FILE *out, struct value *val);
void print_character(FILE *out, struct value *val);
void print_string(FILE *out, struct value *val);



#endif
