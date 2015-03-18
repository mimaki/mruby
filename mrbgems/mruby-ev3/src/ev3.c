#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "ev3if.h"

extern void mrb_rtos_init(mrb_state*);
extern void mrb_ev3_lcd_init(mrb_state*, struct RClass*);
extern void mrb_ev3_led_init(mrb_state*, struct RClass*);
extern void mrb_ev3_button_init(mrb_state*, struct RClass*);
extern void mrb_ev3_sound_init(mrb_state*, struct RClass*);
extern void mrb_ev3_battery_init(mrb_state*, struct RClass*);
extern void mrb_ev3_motor_init(mrb_state*, struct RClass*, struct RClass*);
extern void mrb_ev3_sensor_init(mrb_state*, struct RClass*, struct RClass*);


void
mrb_mruby_ev3_gem_init(mrb_state *mrb)
{
  struct RClass *ev3;
  struct RClass *dev;

  /* RTOS module */
  mrb_rtos_init(mrb);

  /* EV3 module */
  ev3 = mrb_define_module(mrb, "EV3");

  /* EV3 parts */
  mrb_ev3_lcd_init(mrb, ev3);     /* LCD */
  mrb_ev3_led_init(mrb, ev3);     /* LED */
  mrb_ev3_button_init(mrb, ev3);  /* Button */
  mrb_ev3_sound_init(mrb, ev3);   /* Sound */
  mrb_ev3_battery_init(mrb, ev3); /* Battery */

  /* EV3 Devices */
  dev = mrb_define_class_under(mrb, ev3, "Device", mrb->object_class);

  mrb_ev3_motor_init(mrb, ev3, dev);  /* Motor */
  mrb_ev3_sensor_init(mrb, ev3, dev); /* Sensor, ColorSensor, GyroSensor, TouchSensor, UltrasonicSensor */

  /* Object includes EV3 */
  mrb_include_module(mrb, mrb->object_class, ev3);
}

void
mrb_mruby_ev3_gem_final(mrb_state *mrb)
{
}