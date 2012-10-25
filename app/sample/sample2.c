#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/dump.h"

int
main(int argc, char *argv[])
{
  int n;
  extern const char appbin[];
  mrb_state *mrb = mrb_open();

  n = mrb_read_irep(mrb, appbin);

  mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_top_self(mrb));
  mrb_close(mrb);

  return 0;
}

