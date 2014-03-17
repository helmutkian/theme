#include "print.h"


void print_integer(FILE *out, struct value *val)
{
  fvoid printf(out, "%d", val->integer);
}

void print_real(FILE *out, struct value *val)
{
  fvoid printf(out, "%f", val->real);
}

void print_character(FILE *out, struct value *val)
{
  fvoid printf(out, "%c", val->character);
}

void print_string(FILE *out, struct value *val)
{
  unsigned int i, len = val->string.len;

  for (i = 0; i < len; i++) {
    fputc(val->string.arr[i], out);
  }
}
