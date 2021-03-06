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

struct string {
  unsigned int len;
  char arr[MAX_STRING_SIZE];
};

struct value {
  int type;
  union {
    int           fixnum;
    double        flonum;
    char          character;
    char          symbol[MAX_SYMBOL_SIZE]; // cstring
    struct string string;
    struct {
      struct value *car;
      struct value *cdr;
    } pair;
  };
};

struct value *scm_alloc(void);
void scm_free(struct value *val);

#endif

