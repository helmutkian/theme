#include "scheme_data.h"
#include <stdio.h>
#include <stdlib.h>

static void scheme_print_string(scheme_data_t *);
static void scheme_free_string(scheme_data_t *);

void scheme_print_data(scheme_data_t *data)
{
  switch (data->type) {
  case SCHEME_FLOAT:
    printf("%f", data->f);
    break;
  case SCHEME_FIXNUM:
    printf("%d", data->i);
    break;
  case SCHEME_CHAR:
    printf("%c", data->c);
    break;
  case SCHEME_STRING:
    scheme_print_string(data);
    break;
  case SCHEME_CONS:
    break;
  case SCHEME_NIL:
    printf("()");
  }
}

void scheme_print_string(scheme_data_t *str_data)
{
  int i;
  for (i = 0; i < str_data->s.length; i++) {
    printf("%c", str_data->s.arr[i]);
  }
}


void scheme_free_data(scheme_data_t *data)
{
  switch (data->type) {
  case SCHEME_FLOAT:
    break;
  case SCHEME_FIXNUM:
    break;
  case SCHEME_CHAR:
    break;
  case SCHEME_STRING:
    scheme_free_string(data);
    break;
  case SCHEME_CONS:
    break;
  case SCHEME_NIL:
    break;
  }

  free(data);
}

void scheme_free_string(scheme_data_t *str_data)
{
  free(str_data->s.arr);
}


