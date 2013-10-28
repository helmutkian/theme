#ifndef SCHEME_DATA_H
#define SCHEME_DATA_H

typedef enum {
  FLOAT
  , FIXNUM
  , CHAR
  , STRING
  , CONS
  , NIL
  , SYMBOL
} scheme_type;

typedef struct {
  char *arr;
  int length;
} scheme_string;

typedef struct _scheme_data {
  scheme_type type;
  union {
    double f;
    int i;
    char c;
    char *sym;
    scheme_string str;
    struct {
      struct _scheme_data *car;
      struct _scheme_data *cdr;
    } cons;
  };
} scheme_data;

void scheme_print_data(scheme_data *data);
void scheme_free_data(scheme_data *data);

#endif

