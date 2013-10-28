#ifndef SCHEME_DATA_H
#define SCHEME_DATA_H

typedef enum {
  SCHEME_FLOAT
  , SCHEME_FIXNUM
  , SCHEME_CHAR
  , SCHEME_STRING
  , SCHEME_CONS
  , SCHEME_NIL
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
    struct {
      struct scheme_data *car;
      struct scheme_data *cdr;
    } cons;
    scheme_string s;
  };
} scheme_data;

void scheme_print_data(scheme_data *data);
void scheme_free_data(scheme_data *data);

#endif

