#ifndef _READ_H_
#define _READ_H_

#include "scm_value.h"
#include <stdio.h>

#define GET_READER(type_) Scm_read_##type_
#define DECL_READER(type_) Scm_ReaderResult GET_READER(type_)(FILE *, Scm_Value *)

#define BUF_LEN 256

enum { 
  READ_SUCCESS,         /* Value successfully read */
  READ_COMMENT          /* Comment read. */
  READ_FAIL,            /* Read failed */
  READ_MISMATCH_DELIM,  /* No closing delimiter (eg double-quote or paren). */
  READ_INVALID_CHAR     /* Invalid char in symbol */
};
typedef int (*reader)(FILE *, struct value *);

static void ungets(char *s, FILE *f);

int read_fixnum(FILE *in, struct value *val);
int read_flonum(FILE *in, struct value *val);
int read_character(FILE *in, struct value *val);
int read_string(FILE *in, struct value *val);
int read_symbol(FILE *in, struct value *val);

DECL_READER(atom);

#endif
