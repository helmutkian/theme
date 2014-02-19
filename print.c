#include "print.h"

#define DEF_PRINTER(type_) void Scm_print_##type_(FILE *out, Scm_Value *val)

DEF_PRINTER(integer)
{
  fprintf(out, "%d", val->integer);
}

DEF_PRINTER(real)
{
  fprintf(out, "%f", val->real);
}

DEF_PRINTER(character)
{
  fprintf(out, "%c", val->character);
}

DEF_PRINTER(string)
{
  unsigned int i, len = val->string.len;

  for (i = 0; i < len; i++) {
    fputc(val->string.arr[i], out);
  }
}
