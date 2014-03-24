#ifndef _SCM_VALUE_H_
#define _SCM_VALUE_H_

#define MAX_SYMBOL_SIZE 1000
#define MAX_STRING_SIZE 1000

typedef enum {
    TYPE_FIXNUM
  , TYPE_FLONUM
  , TYPE_CHARACTER
  , TYPE_BOOLEAN
  , TYPE_STRING
  , TYPE_SYMBOL
  , TYPE_PAIR 
} Scm_Type;

typedef struct {
  unsigned int len;
  char arr[MAX_STRING_SIZE];
} Scm_String;

struct Scm_Value {
  int type;
  union {
    int           fixnum;
    double        flonum;
    char          character;
    char          symbol[MAX_SYMBOL_SIZE]; // cstring
    Scm_String    string;
    struct {
      struct Scm_Value *car;
      struct Scm_Value *cdr;
    } pair;
  };
} Scm_Value;

Scm_Value *Scm_alloc(Scm_Type type);
void Scm_free(Scm_Value *val);

#endif

