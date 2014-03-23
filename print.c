#include "print.h"


void print_fixnum(FILE *out, struct value *val)
{
  fprintf(out, "%d", val->fixnum);
}

void print_flonum(FILE *out, struct value *val)
{
  fprintf(out, "%f", val->flonum);
}

void print_character(FILE *out, struct value *val)
{
  fprintf(out, "%c", val->character);
}

void print_string(FILE *out, struct value *val)
{
  unsigned int i, len = val->string.len;

  for (i = 0; i < len; i++) {
    fputc(val->string.arr[i], out);
  }
}

void print_symbol(FILE *out, struct value *val)
{
  fprintf(out, "%s", val->symbol);
}

void print(FILE *out, struct value *val)
{
  switch (val->type) {
  case FIXNUM:
    print_fixnum(out, val);
    break;
  case FLONUM:
    print_flonum(out, val);
    break;
  case CHARACTER:
    print_character(out, val);
    break;
  case STRING:
    print_string(out, val);
    break;
  case SYMBOL:
    print_symbol(out, val);
    break;
  default:
    error("No print function defined.");
    break;
  }
}
