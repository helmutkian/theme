#ifndef _SCM_VALUE_H_
#define _SCM_VALUE_H_

#define MAX_SYMBOL_SIZE 1000
#define MAX_STRING_SIZE 1000

enum { 
    FIXNUM
  , FLONUM
  , CHARACTER
  , STRING
  , SYMBOL
  , PAIR 
};


struct value {
  int type;
  union {
    int        integer;
    double     real;
    char       character;
    char       symbol[MAX_SYMBOL_SIZE]; // cstring
    struct {
      unsigned int len;
      char arr[MAX_STRING_SIZE];
    } string;
    struct {
      struct value *car;
      struct value *cdr;
    } pair;
  };
};

#endif

