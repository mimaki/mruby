/*
** enzi.c - enzi Library
**
** Copyright(c) FUKUOKA CSK CORPORATION
** Copyright(c) Manycolors Inc
*/

#include "mruby.h"
#include "mruby/variable.h"

void mrb_init_digitalio(mrb_state *mrb);
void mrb_init_analogio(mrb_state *mrb);

void
mrb_enzi_gem_init(mrb_state *mrb)
{
  mrb_init_digitalio(mrb);
  mrb_init_analogio(mrb);
}
