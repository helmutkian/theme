#include "scm_value.h"

struct value *scm_alloc(void)
{
  return malloc(sizeof(struct value));
}

void scm_free(struct value *val)
{
  free(val);
}
