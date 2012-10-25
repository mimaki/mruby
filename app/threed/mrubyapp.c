#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/dump.h"
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
  FILE *fp;
  mrb_state *mrb;
  int n = -1;

  if (argc < 2) {
    printf("MRB file is not specified.\n");
    return EXIT_FAILURE;
  }

  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    printf("file cannot open. (%s)\n", argv[1]);
    return EXIT_FAILURE;
  }

  mrb = mrb_open();

  n = mrb_load_irep(mrb, fp);
  fclose(fp);

  if (n >= 0) {
    mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_top_self(mrb));
  }

  mrb_close(mrb);

  return EXIT_SUCCESS;
}

