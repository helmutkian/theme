#ifndef _READ_H_
#define _READ_H_

#include "scm_value.h"
#include <stdio.h>

#define DECL_READER(type_) Scm_ReaderResult Scm_read_##type_(FILE *, Scm_Value *)

#define BUF_LEN 256

enum { READ_FAIL, READ_SUCCESS };
typedef int Scm_ReaderResult;
typedef Scm_ReaderResult (*Scm_Reader)(FILE *, Scm_Value *);

static void ungets(char *s, FILE *f);


DECL_READER(integer);
DECL_READER(real);
DECL_READER(character);
DECL_READER(string);


#endif
