#include "mruby.h"
#include <string.h>
#include "mruby/string.h"

static mrb_value
add_space(mrb_state *mrb, mrb_value self)
{
  char *spc, *ps, *pd;
  int spclen, times, i, j;
  mrb_int len;
  mrb_value str;

  mrb_get_args(mrb, "si", &spc, &spclen, &times);

  len = RSTRING_LEN(self) + RSTRING_LEN(self) * spclen * times;
  str = mrb_str_buf_new(mrb, len);

  ps = RSTRING_PTR(self);
  pd = RSTRING_PTR(str);

  for (i = 0; i < RSTRING_LEN(self); i++) {
    *pd++ = *ps++;
    for (j = 0; j < times; j++) {
      memcpy(pd, spc, spclen);
      pd += spclen;
    }
  }
  *pd = '\0';
  RSTRING_LEN(str) = len;

  return str;
}

void
mrb_init_mystring(mrb_state *mrb)
{
  mrb_define_method(mrb, mrb->string_class, "add_space", add_space, ARGS_REQ(2));
}
