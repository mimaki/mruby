/*
** analogio.c - AnalogIO class
**
** Copyright(c) FUKUOKA CSK CORPORATION
** Copyright(c) Manycolors Inc
*/

#include "mruby.h"
#include "mruby/variable.h"

#define ENZI_AIO_DEF_REFTYP 0
#define ENZI_AIO_DEF_RESO 10

int32_t _AnalogRead(int32_t);
void _AnalogWrite(mrb_state*, int32_t, int32_t);

/*
 *  call-seq:
 *     AnalogIO.new(pin, mode, reftyp=DEFAULT, reso=10) => AnalogIO
 *
 *  Constructs a AnalogIO.
 */

mrb_value
mrb_aio_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int pin, mode, reftyp = 0, reso = 10;

  mrb_get_args(mrb, "ii|ii", &pin, &mode, &reftyp, &reso);

  mrb_iv_set(mrb, self, mrb_intern(mrb, "pin"), mrb_fixnum_value(pin));
  mrb_iv_set(mrb, self, mrb_intern(mrb, "mode"), mrb_fixnum_value(mode));
  mrb_iv_set(mrb, self, mrb_intern(mrb, "reftyp"), mrb_fixnum_value(reftyp));
  mrb_iv_set(mrb, self, mrb_intern(mrb, "reso"), mrb_fixnum_value(reso));

  return self;
}

mrb_value
mrb_aio_read(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(0);
}

extern void emscripten_run_script(const char *script);

mrb_value
mrb_aio_write(mrb_state *mrb, mrb_value self)
{
  mrb_int duty;
  mrb_value pin;

  mrb_get_args(mrb, "i", &duty);
  pin = mrb_iv_get(mrb, self, mrb_intern(mrb, "pin"));

//printf("AnalogIO#write(%d, %d) called.\n", mrb_fixnum(pin), duty);
  _AnalogWrite(mrb, (int32_t)mrb_fixnum(pin), (int32_t)duty);

  return mrb_fixnum_value(duty);
}

void
mrb_init_analogio(mrb_state *mrb)
{
  struct RClass *aio;

  aio = mrb_define_class(mrb, "AnalogIO", mrb->object_class);

  mrb_define_method(mrb, aio, "initialize", mrb_aio_initialize, ARGS_REQ(2)|ARGS_OPT(2));
  mrb_define_method(mrb, aio, "read",       mrb_aio_read,       ARGS_NONE());
  mrb_define_method(mrb, aio, "write",      mrb_aio_write,      ARGS_REQ(1));
}

#ifdef ENZI_JS
#include <stdio.h>
extern mrb_value mrb_js_funcall_argv(mrb_state*, const char*, int, mrb_value*);

void
_AnalogWrite(mrb_state *mrb, int32_t pin, int32_t duty)
{
  mrb_value argv[2];

  argv[0] = mrb_fixnum_value(pin);
  argv[1] = mrb_fixnum_value(duty);

//  printf("_AnalogWrite(%d, %d);\n", pin, duty);
  mrb_js_funcall_argv(mrb, "AnalogWrite", 2, argv);
}
#endif

