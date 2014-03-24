#ifndef _STACK_H_
#define _STACK_H_

#include "scm_value.h"

struct node {
  struct value *data;
  struct node *next;
};

typedef struct node *Stack;

void stack_push(Stack *stack, struct value *val);
struct value *stack_pop(Stack *stack);
struct value *stack_top(Stack *stack);

#endif
