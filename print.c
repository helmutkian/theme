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
