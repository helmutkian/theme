#include "scheme_data.h"
#include <stdio.h>
#include <stdlib.h>

static void scheme_print_string(scheme_string_t *);
static void scheme_print_cons(scheme_cons_t *);
static void scheme_free_string(scheme_string_t *);
static void scheme_free_cons(scheme_cons_t *);

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
    scheme_print_string(&data->s);
    break;
  case SCHEME_CONS:
    scheme_print_cons(&data->cons);
    break;
  }
}

static void scheme_print_string(scheme_string_t *str_data)
{
  int i;
  for (i = 0; i < str_data->length; i++) {
    printf("%c", str_data->str[i]);
  }
}

static void scheme_print_cons(scheme_cons_t *cons_data)
{
  scheme_cons_t *cell = cons_data;

  while (cell != NULL) {
    scheme_print_data(cell->car);
    // Handle "dotted", aka "improper", lists
    if (cell->cdr->type != SCHEME_CONS) {
      scheme_print_data(cell->cdr);
      break;
    }
    cell = cell->cdr;
  }
}

void scheme_free_data(scheme_data_t *data)
{
  switch (data->type) {
  case SCHEME_STRING:
    scheme_free_string(&data->s);
    break;
  }

  free(data);
}

static void scheme_free_string(scheme_string_t *str_data)
{
  free(str_data->str);
}

static void scheme_free_cons(scheme_cons_t *cons_data)
{
  // TODO
}
