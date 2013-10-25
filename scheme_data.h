#ifndef SCHEME_DATA_H
#define SCHEME_DATA_H

typedef enum {
  SCHEME_FLOAT
  , SCHEME_FIXNUM
  , SCHEME_CHAR
  , SCHEME_STRING
} scheme_type_t;

typedef struct {
  int length;
  char *str;
} scheme_string_t;

typedef struct {
  scheme_type_t type;
  union {
    double f;
    int i;
    char c;
    scheme_string_t s;
  };
} scheme_data_t;

// Prints Scheme data to screen
void scheme_print_data(scheme_data_t *);

// Frees allocated Scheme data
void scheme_free_data(scheme_data_t *);

#endif

