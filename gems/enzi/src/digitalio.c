/*
** digitalio.c - DigitalIO class
**
** Copyright(c) FUKUOKA CSK CORPORATION
** Copyright(c) Manycolors Inc
*/

#include "mruby.h"
#include "mruby/variable.h"

void LEDon();
void LEDoff();

#if 0
/*
 *  call-seq:
 *     DigitalIO.new(pin, mode, reftyp=DEFAULT, reso=10) => DigitalIO
 *
 *  Constructs a DigitalIO.
 */

mrb_value
mrb_dio_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int pin, mode;

  mrb_get_args(mrb, "ii", &pin, &mode);

  mrb_iv_set(mrb, self, mrb_intern(mrb, "pin"), mrb_fixnum_value(pin));
  mrb_iv_set(mrb, self, mrb_intern(mrb, "mode"), mrb_fixnum_value(mode));

  return self;
}
#endif

mrb_value
mrb_led_on(mrb_state *mrb, mrb_value self)
{
  LEDon();
  return mrb_nil_value();
}

mrb_value
mrb_led_off(mrb_state *mrb, mrb_value self)
{
  LEDoff();
  return mrb_nil_value();
}

void
mrb_init_digitalio(mrb_state *mrb, struct RClass *enzi)
{
  struct RClass *dio, *led;

  dio = mrb_define_class_under(mrb, enzi, "DigitalIO", mrb->object_class);
#if 0
  mrb_define_method(mrb, dio, "initialize", mrb_dio_initialize, ARGS_REQ(2));
#endif

  led = mrb_define_class(mrb, "LED", dio);

  mrb_define_method(mrb, led, "on",  mrb_led_on,  ARGS_NONE());
  mrb_define_method(mrb, led, "off", mrb_led_off, ARGS_NONE());
}
