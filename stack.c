#include "stack.h"
#include <stdlib.h>

void stack_push(struct node **stack, struct value *val)
{
  struct node *top;
  
  top = malloc(sizeof(struct node));
  top->data = val;
  top->next = *stack;
  *stack = top;
}

struct value *stack_top(struct node **stack)
{
  if (!(*stack)) 
    return NULL;
  else
    return (*stack)->data;
}

struct value *stack_pop(struct node **stack)
{
  struct node *top = *stack;
  struct value *data;

  if (!top) 
    return NULL;
  
  data = top->data;
  *stack = top->next;
  free(top);
  return data;
}
  

  
