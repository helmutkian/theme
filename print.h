#ifndef _PRINT_H_
#define _PRINT_H_

#include "scm_value.h"
#include <stdio.h>

typedef void (*Scm_Printer)(FILE *, Scm_Value *);

void Scm_print_integer(FILE *out, Scm_Value *val);
void Scm_print_real(FILE *out, Scm_Value *val);
void Scm_print_character(FILE *out, Scm_Value *val);
void Scm_print_string(FILE *out, Scm_Value *val);



#endif
