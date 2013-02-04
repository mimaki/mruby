/*
** digitalio.c - DigitalIO class
**
** Copyright(c) FUKUOKA CSK CORPORATION
** Copyright(c) Manycolors Inc
*/

#include "enzi.h"

void LEDon();
void LEDoff();

static mrb_value
enzi_get_diopin(mrb_state *mrb, struct RClass *c, mrb_value pin)
{
  mrb_value hash, v;

  if (mrb_fixnum_p(pin)) {
    return pin;
  }

  if (!mrb_symbol_p(pin)) {
    mrb_raise(mrb, E_TYPE_ERROR, "illegal pin");
  }

  hash = mrb_mod_cv_get(mrb, c, INTERN("DIOPIN"));
  v = mrb_hash_get(mrb, hash, pin);
  if (mrb_nil_p(v)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "illegal pin: %s", mrb_sym2name(mrb, mrb_symbol(pin)));
  }

  return v;
}

/*
 *  call-seq:
 *     DigitalIO.new(pin, mode) => DigitalIO
 *
 *  Constructs a DigitalIO.
 */
mrb_value
mrb_dio_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value pin, mode;
  struct RClass *c = mrb_object(self)->c;

  mrb_get_args(mrb, "oo", &pin, &mode);

  pin = enzi_get_diopin(mrb, c, pin);
  mode = enzi_get_iomode(mrb, c, mode);

  mrb_iv_set(mrb, self, mrb_intern(mrb, "pin"), pin);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "mode"), mode);

  return self;
}

/*
 *  call-seq:
 *     dio.read => Fixnum
 *
 *  read data from DigitalIO.
 */
mrb_value
mrb_dio_read(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(0);
}

/*
 *  call-seq:
 *     dio.write(v) => Fixnum
 *
 *  write data to DigitalIO.
 */
mrb_value
mrb_dio_write(mrb_state *mrb, mrb_value self)
{
  mrb_int v;

  mrb_get_args(mrb, "i", &v);

  // TODO: dio_write

  return mrb_fixnum_value(v);
}

/*
 *  call-seq:
 *     dio.high => HIGH
 *
 *  DigitalIO set to HIGH.
 */
mrb_value
mrb_dio_high(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(1);
}

/*
 *  call-seq:
 *     dio.high? => true/false
 *
 *  check DigitalIO is HIGH level.
 */
mrb_value
mrb_dio_ishigh(mrb_state *mrb, mrb_value self)
{
  return mrb_true_value();
}

/*
 *  call-seq:
 *     dio.low => LOW
 *
 *  DigitalIO set to LOW.
 */
mrb_value
mrb_dio_low(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(0);
}

/*
 *  call-seq:
 *     dio.low? => true/false
 *
 *  check DigitalIO is LOW level.
 */
mrb_value
mrb_dio_islow(mrb_state *mrb, mrb_value self)
{
  return mrb_false_value();
}

/*
 *  call-seq:
 *     LED.new => LED
 *
 *  Constracts a LED.
 */
mrb_value
mrb_led_initialize(mrb_state *mrb, mrb_value self)
{
  return self;
}

/*
 *  call-seq:
 *     led.on => nil
 *
 *  LED turn on.
 */
mrb_value
mrb_led_on(mrb_state *mrb, mrb_value self)
{
  LEDon();
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     led.off => nil
 *
 *  LED turn off.
 */
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

  /* DigitalIO class */
  dio = mrb_define_class_under(mrb, enzi, "DigitalIO", mrb->object_class);

  /* DigitalIO methods */
  mrb_define_method(mrb, dio, "initialize", mrb_dio_initialize, ARGS_REQ(2));
  mrb_define_method(mrb, dio, "read", mrb_dio_read, ARGS_NONE());
  mrb_define_method(mrb, dio, "write", mrb_dio_write, ARGS_REQ(1));
  mrb_define_method(mrb, dio, "high", mrb_dio_high, ARGS_NONE());
  mrb_define_method(mrb, dio, "high?", mrb_dio_ishigh, ARGS_NONE());
  mrb_define_method(mrb, dio, "low", mrb_dio_low, ARGS_NONE());
  mrb_define_method(mrb, dio, "low?", mrb_dio_islow, ARGS_NONE());

  /* LED class */
  led = mrb_define_class(mrb, "LED", dio);

  /* LED methods */
  mrb_define_method(mrb, led, "initialize", mrb_led_initialize, ARGS_NONE());
  mrb_define_method(mrb, led, "on",  mrb_led_on,  ARGS_NONE());
  mrb_define_method(mrb, led, "off", mrb_led_off, ARGS_NONE());
}
