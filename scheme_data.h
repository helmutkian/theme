#ifndef SCHEME_DATA_H
#define SCHEME_DATA_H

typedef enum {
  SCHEME_FLOAT
  , SCHEME_FIXNUM
  , SCHEME_CHAR
  , SCHEME_STRING
  , SCHEME_CONS
  , SCHEME_NIL
} scheme_type_t;

typedef struct {
  char *arr;
  int length;
} scheme_string_t;

typedef struct _scheme_data {
  scheme_type_t type;
  union {
    double f;
    int i;
    char c;
    struct {
      struct scheme_data *car;
      struct scheme_data *cdr;
    } cons;
    scheme_string_t s;
  };
} scheme_data_t;

void scheme_print_data(scheme_data_t *data);
void scheme_free_data(scheme_data_t *data);

#endif

