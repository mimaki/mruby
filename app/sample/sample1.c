#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/dump.h"
#include <stdio.h>

int
main(int argc, char *argv[])
{
  int n;
  FILE *fp;
  mrb_state *mrb = mrb_open();

  if ((fp = fopen("./hello.mrb", "rb")) == NULL) {
    puts("file not found.");
    return -1;
  }
  n = mrb_load_irep(mrb, fp);
  fclose(fp);

  mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_top_self(mrb));
  mrb_close(mrb);

  return 0;
}

