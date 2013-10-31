#include "scheme_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


static void 
_print_fixnum(scheme_data *);

static void 
_print_flonum(scheme_data *);

static void 
_print_char(scheme_data *);

static void 
_print_string(scheme_data *);

static void 
_print_pair(scheme_data *);

static void
_print_nil(scheme_data *);

static void
_print_symbol(scheme_data *);

static void
_print_bool(scheme_data *);

static void 
_print_ratio(scheme_data *);

static void 
_free_string(scheme_data *);

static void 
_free_symbol(scheme_data *);


scheme_data *
scheme_data_alloc(scheme_type type)
{
  scheme_data *data = malloc(sizeof(scheme_data));
  assert(data != NULL);
  data->type = type;
  return data;
}

void 
scheme_print_data(scheme_data *data)
{
  switch (data->type) {
  case SCHEME_FLONUM:
    _print_flonum(data);
    break;
  case SCHEME_FIXNUM:
    _print_fixnum(data);
    break;
  case SCHEME_CHAR:
    _print_char(data);
    break;
  case SCHEME_STRING:
    _print_string(data);
    break;
  case SCHEME_PAIR:
    break;
  case SCHEME_NIL:
    _print_nil(data);
  case SCHEME_SYMBOL:
    _print_symbol(data);
    break;
  case SCHEME_BOOL:
    _print_bool(data);
  case SCHEME_RATIO:
    _print_ratio(data);
  }
}


void 
_print_fixnum(scheme_data *fixnum_data)
{
  printf("%d", fixnum_data->i);
}

void 
_print_flonum(scheme_data *flonum_data)
{
  printf("%f", flonum_data->f);
}

void 
_print_char(scheme_data *char_data)
{
  printf("%c", char_data->c);
}

void 
_print_string(scheme_data *str_data)
{
  int i;
  for (i = 0; i < str_data->str.length; i++) {
    printf("%c", str_data->str.arr[i]);
  }
}

void 
_print_pair(scheme_data *pair_data)
{
  scheme_data *cell;

  printf("(");
  for (cell = pair_data
     ; cell->type != SCHEME_NIL
     ; cell = cell->pair.cdr) {
    scheme_print_data(cell->pair.car);
    // Handle "improper", or "dotted", lists
    if (cell->pair.cdr->type != SCHEME_PAIR) {
      scheme_print_data(cell->pair.cdr);
      break;
    }
  }
  printf(")");
}

void 
_print_nil(scheme_data *str_data)
{
  printf("()");
}

void 
_print_symbol(scheme_data *sym_data)
{
  printf("%s", sym_data->sym);
}

void 
_print_bool(scheme_data *bool_data)
{
  printf("#%c", bool_data->b ? 't' : 'f');
}

void 
_print_ratio(scheme_data *ratio_data)
{
  printf("%d/%d", ratio_data->r.num, ratio_data->r.denom);
}

void 
scheme_free_data(scheme_data *data)
{
  switch (data->type) {
  case SCHEME_FLONUM:
    break;
  case SCHEME_FIXNUM:
    break;
  case SCHEME_CHAR:
    break;
  case SCHEME_STRING:
    _free_string(data);
    break;
  case SCHEME_PAIR:
    break;
  case SCHEME_NIL:
    break;
  case SCHEME_SYMBOL:
    _free_symbol(data);
    break;
  case SCHEME_BOOL:
    break;
  case SCHEME_RATIO:
    break;
  }

  free(data);
}

void 
_free_string(scheme_data *str_data)
{
  free(str_data->str.arr);
}

void 
_free_symbol(scheme_data *sym_data)
{
  free(sym_data->sym);
}


