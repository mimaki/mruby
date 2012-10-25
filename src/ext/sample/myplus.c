#include "mruby.h"

static mrb_value
myplus(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  x = mrb_fixnum(self);
  mrb_get_args(mrb, "i", &y);
  return mrb_fixnum_value(x + y);
}

void
mrb_init_myplus(mrb_state *mrb)
{
  mrb_define_method(mrb, mrb->fixnum_class, "plus", myplus, ARGS_REQ(1));
}
