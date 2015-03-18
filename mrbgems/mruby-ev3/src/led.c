#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "ev3if.h"


/*
 *  call-seq:
 *     LED.color = col  # => nil
 *
 *  Light LED.
 *
 *  Parameters:
 *    +col+     LED color
 *       LED::OFF
 *       LED::RED
 *       LED::GREEN
 *       LED::ORANGE
 *
 *  Returns nil.
 */
static mrb_value
mrb_led_color(mrb_state *mrb, mrb_value self)
{
  mrb_int col;
  mrb_get_args(mrb, "i", &col);
  EV3_led_set_color(col);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     LED.off  # => nil
 *
 *  LED turn off.
 *
 *  Returns nil.
 */
static mrb_value
mrb_led_off(mrb_state *mrb, mrb_value self)
{
  EV3_led_set_color(LED_OFF);
  return mrb_nil_value();
}


void
mrb_ev3_led_init(mrb_state *mrb, struct RClass *ev3)
{
  struct RClass *led;
  mrb_value ledo;

  /* LED class */
  led = mrb_define_class_under(mrb, ev3, "LED", mrb->object_class);
  ledo = mrb_obj_value(led);

  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "OFF"),    mrb_fixnum_value(LED_OFF));
  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "RED"),    mrb_fixnum_value(LED_RED));
  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "GREEN"),  mrb_fixnum_value(LED_GREEN));
  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "ORANGE"), mrb_fixnum_value(LED_ORANGE));

  mrb_define_class_method(mrb, led, "color=", mrb_led_color,  MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, led, "off",    mrb_led_off,    MRB_ARGS_NONE());
}
