/*
** init_ext.c - initialize extend libraries
**
** See Copyright Notice in mruby.h
*/

#include "mruby.h"

#if 0
/* enzi class libraries */
void mrb_init_digitalio(mrb_state *mrb);
void mrb_init_analogio(mrb_state *mrb);
#endif

void
mrb_init_ext(mrb_state *mrb)
{
#ifdef INCLUDE_SOCKET
  extern void mrb_init_socket(mrb_state *mrb);
  mrb_init_socket(mrb);
#endif

#if 0
  /* enzi class libraries */
  mrb_init_digitalio(mrb);
  mrb_init_analogio(mrb);
#endif
}
