#include "scheme_data.h"
#include <stdio.h>
#include <stdlib.h>

static void _print_fixnum(scheme_data *);
static void _print_float(scheme_data *);
static void _print_char(scheme_data *);
static void _print_string(scheme_data *);
// static void _print_cons(scheme_data *);
static void _print_symbol(scheme_data *);

static void _free_string(scheme_data *);
static void _free_symbol(scheme_data *);

void scheme_print_data(scheme_data *data)
{
  switch (data->type) {
  case FLOAT:
    _print_float(data);
    break;
  case FIXNUM:
    _print_fixnum(data);
    break;
  case CHAR:
    _print_char(data);
    break;
  case STRING:
    _print_string(data);
    break;
  case CONS:
    break;
  case NIL:
    printf("()");
  case SYMBOL:
    _print_symbol(data);
    break;
  }
}

void _print_fixnum(scheme_data *fixnum_data)
{
  printf("%d", data->i);
}

void _print_float(scheme_data *float_data)
{
  printf("%f", data->f);
}

void _print_char(scheme_data *char_data)
{
  printf("%c", data->c);
}

void _print_string(scheme_data *str_data)
{
  int i;
  for (i = 0; i < str_data->str.length; i++) {
    printf("%c", str_data->str.arr[i]);
  }
}

void _print_symbol(scheme_data *sym_data)
{
  printf("%s", sym_data->sym);
}


void scheme_free_data(scheme_data *data)
{
  switch (data->type) {
  case FLOAT:
    break;
  case FIXNUM:
    break;
  case CHAR:
    break;
  case STRING:
    _free_string(data);
    break;
  case CONS:
    break;
  case NIL:
    break;
  case SYMBOL:
    _free_symbol(data);
    break;
  }

  free(data);
}

void _free_string(scheme_data *str_data)
{
  free(str_data->str.arr);
}

void _free_symbol(scheme_data *sym_data)
{
  free(sym_data->sym);
}


