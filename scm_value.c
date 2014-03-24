#include "scm_value.h"

Scm_Value *Scm_alloc(Scm_Type type)
{
  ScmValue *val = malloc(sizeof(Scm_Value));
  val->type = type;
  return val;
}

void Scm_free(Scm_Value *val)
{
  free(val);
}
