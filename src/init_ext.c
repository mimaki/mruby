/*
** init_ext.c - initialize extend libraries
**
** See Copyright Notice in mruby.h
*/

#include "mruby.h"

#ifdef ENABLE_IO
extern void mrb_init_io(mrb_state *mrb);
extern void mrb_init_file(mrb_state *mrb);
#endif
extern void mrb_init_sleep(mrb_state *mrb);
extern void mrb_init_mystring(mrb_state *mrb);
extern void mrb_init_button(mrb_state *mrb);
extern void mrb_init_led(mrb_state *mrb);
extern void mrb_init_accel(mrb_state *mrb);

void
mrb_init_ext(mrb_state *mrb)
{
#ifdef INCLUDE_SOCKET
  extern void mrb_init_socket(mrb_state *mrb);
  mrb_init_socket(mrb);
#endif

#ifdef ENABLE_IO
  mrb_init_io(mrb);
  mrb_init_file(mrb);
#endif
  mrb_init_sleep(mrb);
  mrb_init_mystring(mrb);
  mrb_init_button(mrb);
  mrb_init_led(mrb);
  mrb_init_accel(mrb);
}
