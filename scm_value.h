#ifndef _SCM_VALUE_H_
#define _SCM_VALUE_H_

#define MAX_STRING_SIZE 1000

enum { 
    INTEGER
  , REAL
  , CHARACTER
  , STRING
  , SYMBOL
  , PAIR 
};

typedef int Scm_Type;

typedef struct Scm_Value {
  Scm_Type type;
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
      struct Scm_Value *car;
      struct Scm_Value *cdr;
    } pair;
  };
} Scm_Value;

#endif

