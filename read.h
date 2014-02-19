#ifndef _READ_H_
#define _READ_H_

#include "scm_value.h"
#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN 256

enum { READ_FAIL, READ_SUCCESS };
typedef int Scm_ReaderResult;
typedef Scm_ReaderResult (*Scm_Reader)(FILE *, Scm_Value *);

static void ungets(char *s, FILE *f);

Scm_ReaderResult Scm_read_integer(FILE *in, Scm_Value *val);
Scm_ReaderResult Scm_read_real(FILE *in, Scm_Value *val);
Scm_ReaderResult Scm_read_character(FILE *in, Scm_Value *val);


#endif
