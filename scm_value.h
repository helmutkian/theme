#ifndef _SCM_VALUE_H_
#define _SCM_VALUE_H_

enum { INTEGER, REAL, CHARACTER, STRING };

typedef int Scm_Type;

typedef struct {
  unsigned int len;
  char *chars;
} Scm_String;

typedef struct Scm_Value {
  Scm_Type type;
  union {
    int        integer;
    double     real;
    char       character;
    Scm_String string;
  };
} Scm_Value;

#endif

