/*
** sleep.c - Kernel.sleep
**
** Copyright(C) 2012 Kyushu Embedded Software Technology Consortium(QUEST)
** Copyright(C) 2012 FUKUOKA CSK CORP.
*/

#include "mruby.h"
#include <unistd.h>

/*
 * call-seq:
 *   sleep(timeout)  ->  nil
 */
mrb_value
mrb_sleep(mrb_state *mrb, mrb_value self)
{
  mrb_value t;
  long to;

  mrb_get_args(mrb, "f", &t);
  to = (long)(mrb_float(t) * 1000000.0 + 0.5);
  usleep(to);

  return mrb_nil_value();
}

void
mrb_init_sleep(mrb_state *mrb)
{
  mrb_define_method(mrb, mrb->kernel_module, "sleep", mrb_sleep, ARGS_REQ(1));
}
