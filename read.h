#ifndef _READ_H_
#define _READ_H_

#include "scm_value.h"
#include <stdio.h>

#define BUF_LEN 256

enum { 
    READ_SUCCESS         /* Value successfully read */
  , READ_COMMENT         /* Comment read. */
  , READ_FAIL            /* Read failed */
  , READ_MISMATCH_DELIM  /* No closing delimiter (eg double-quote or paren). */
  , READ_INVALID_CHAR    /* Invalid char read */
};
typedef int (*reader)(FILE *, struct value *);

static void ungets(char *s, FILE *f);

int read_fixnum(FILE *in, struct value *val);
int read_flonum(FILE *in, struct value *val);
int read_character(FILE *in, struct value *val);
int read_string(FILE *in, struct value *val);
int read_symbol(FILE *in, struct value *val);

#endif
