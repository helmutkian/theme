#ifndef SCHEME_DATA_H
#define SCHEME_DATA_H

typedef enum {
  SCHEME_FLOAT
  , SCHEME_FIXNUM
  , SCHEME_CHAR
  , SCHEME_STRING
  , SCHEME_CONS
  , SCHEME_NIL
  , SCHEME_SYMBOL
  , SCHEME_BOOL
  , SCHEME_RATIO
} scheme_type;

typedef struct {
  char *arr;
  int length;
} scheme_string;

typedef struct {
  int num;
  int denom;
} scheme_ratio;

typedef struct _scheme_data {
  scheme_type type;
  union {  
    struct {
      struct _scheme_data *car;
      struct _scheme_data *cdr;
    } cons;                     // SCHEME_CONS
    double f;                   // SCHEME_FLOAT
    int i;                      // SCHEME_FIXNUM
    char c;                     // SCHEME_CHAR
    char *sym;                  // SCHEME_SYMBOL
    scheme_string str;          // SCHEME_STR
    unsigned char b;            // SCHEME_BOOL
    scheme_ratio r;             // SCHEME_RATIO    
  };
} scheme_data;

scheme_data *scheme_alloc_data(scheme_type type);
void scheme_print_data(scheme_data *data);
void scheme_free_data(scheme_data *data);

#endif

