#include <stdio.h>
#include <string.h>
#include "mruby.h"
// #include "mruby/value.h"
#include "mruby/variable.h"
// #include "mruby/class.h"
// #include "mruby/string.h"
// #include "mruby/hash.h"
#include "ev3if.h"
#include "rtosif.h"


static mrb_value
mrb_sensor_init(mrb_state *mrb, mrb_value self, mrb_int type)
{
  mrb_int port;
  mrb_get_args(mrb, "i", &port);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(port));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@type"), mrb_fixnum_value(type));
  return self;
}

/*
 *  call-seq:
 *     ColorSensor.new(port)  # => ColorSensor
 *
 *  Creates ColorSensor object.
 *
 *  Parameters:
 *    +port+    ColorSensor port
 *       Sensor::PORT_1
 *       Sensor::PORT_2
 *       Sensor::PORT_3
 *       Sensor::PORT_4
 *
 *  Returns ColorSensor object.
 */
static mrb_value
mrb_color_sensor_init(mrb_state *mrb, mrb_value self)
{
  return mrb_sensor_init(mrb, self, COLOR_SENSOR);
}

/*
 *  call-seq:
 *     cs.ambient  # => Fixnum
 *
 *  Get ambient light level.
 *
 *  Returns light level.
 */
static mrb_value
mrb_color_sensor_get_ambient(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_color_sensor_get_ambient(port));
}

/*
 *  call-seq:
 *     cs.color  # => Fixnum
 *
 *  Get color.
 *
 *  Returns color ID.
 *    0: NONE
 *    1: BLACK
 *    2: BLUE
 *    3: GREEN
 *    4: YELLOW
 *    5: RED
 *    6: WHITE
 *    7: BROWN
 */
static mrb_value
mrb_color_sensor_get_color(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_color_sensor_get_color(port));
}

/*
 *  call-seq:
 *     cs.reflect  # => Fixnum
 *
 *  Get reflected light level from red lED.
 *
 *  Returns light level.
 */
static mrb_value
mrb_color_sensor_get_reflect(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_color_sensor_get_reflect(port));
}

static inline mrb_value
mrb_color_sensor_judge_color(mrb_state *mrb, mrb_value self, mrb_int color)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_bool_value(EV3_color_sensor_get_color(port) == color);
}

/*
 *  call-seq:
 *     cs.black?  # => true/false
 *
 *  It is determined whether black using the color sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_color_sensor_is_black(mrb_state *mrb, mrb_value self)
{
  return mrb_color_sensor_judge_color(mrb, self, COLOR_BLACK);
}

/*
 *  call-seq:
 *     cs.blue?  # => true/false
 *
 *  It is determined whether blue using the color sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_color_sensor_is_blue(mrb_state *mrb, mrb_value self)
{
  return mrb_color_sensor_judge_color(mrb, self, COLOR_BLUE);
}

/*
 *  call-seq:
 *     cs.green?  # => true/false
 *
 *  It is determined whether green using the color sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_color_sensor_is_green(mrb_state *mrb, mrb_value self)
{
  return mrb_color_sensor_judge_color(mrb, self, COLOR_GREEN);
}

/*
 *  call-seq:
 *     cs.yellow?  # => true/false
 *
 *  It is determined whether yellow using the color sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_color_sensor_is_yellow(mrb_state *mrb, mrb_value self)
{
  return mrb_color_sensor_judge_color(mrb, self, COLOR_YELLOW);
}

/*
 *  call-seq:
 *     cs.red?  # => true/false
 *
 *  It is determined whether red using the color sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_color_sensor_is_red(mrb_state *mrb, mrb_value self)
{
  return mrb_color_sensor_judge_color(mrb, self, COLOR_RED);
}

/*
 *  call-seq:
 *     cs.white?  # => true/false
 *
 *  It is determined whether white using the color sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_color_sensor_is_white(mrb_state *mrb, mrb_value self)
{
  return mrb_color_sensor_judge_color(mrb, self, COLOR_WHITE);
}

/*
 *  call-seq:
 *     cs.brown?  # => true/false
 *
 *  It is determined whether brown using the color sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_color_sensor_is_brown(mrb_state *mrb, mrb_value self)
{
  return mrb_color_sensor_judge_color(mrb, self, COLOR_BROWN);
}

/*
 *  call-seq:
 *     GyroSensor.new(port)  # => GyroSensor
 *
 *  Creates GyroSensor object.
 *
 *  Parameters:
 *    +port+    GyroSensor port
 *       Sensor::PORT_1
 *       Sensor::PORT_2
 *       Sensor::PORT_3
 *       Sensor::PORT_4
 *
 *  Returns GyroSensor object.
 */
static mrb_value
mrb_gyro_sensor_init(mrb_state *mrb, mrb_value self)
{
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@offset"), mrb_float_value(mrb, 0.0f));
  return mrb_sensor_init(mrb, self, GYRO_SENSOR);
}

/*
 *  call-seq:
 *     gs.angle  # => Fixnum
 *
 *  Get gyro angle.
 *
 *  Returns angle.
 */
static mrb_value
mrb_gyro_sensor_get_angle(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_gyro_sensor_get_angle(port));
}

/*
 *  call-seq:
 *     gs.rate  # => Fixnum
 *
 *  Get gyro rate.
 *
 *  Returns rate.
 */
static mrb_value
mrb_gyro_sensor_get_rate(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_gyro_sensor_get_rate(port));
}

/*
 *  call-seq:
 *     gs.reset  # => nil
 *
 *  Reset gyro sensor.
 *
 *  Returns nil.
 */
static mrb_value
mrb_gyro_sensor_reset(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  EV3_gyro_sensor_reset(port);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     gs.calibrate  # => nil
 *
 *  Calibrate gyro sensor.
 *
 *  Returns nil.
 */
static mrb_value
mrb_gyro_sensor_calibrate(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  int32_t gmin = 1000, gmax = -100, gsum = 0, gyro;
  int i;
  mrb_float offset;

  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));

  for (i = 0; i < 200; i++) {
    gyro = EV3_gyro_sensor_get_rate(port);
    gsum += gyro;
    if (gyro > gmax) gmax = gyro;
    if (gyro < gmin) gmin = gyro;
    RTOS_delay(4);
  }
  if(!(gmax - gmin < 2)) {
    offset = (mrb_float)gsum / 200.0f;
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@offset"), mrb_float_value(mrb, offset));
  }
  else {
    /* TODO: calibration error */
  }

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     gs.offset  # => Float
 *
 *  Get gyro offset.
 *
 *  Returns offset.
 */
static mrb_value
mrb_gyro_sensor_get_offset(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@offset"));
}

/*
 *  call-seq:
 *     TouchSensor.new(port)  # => TouchSensor
 *
 *  Creates TouchSensor object.
 *
 *  Parameters:
 *    +port+    TouchSensor port
 *       Sensor::PORT_1
 *       Sensor::PORT_2
 *       Sensor::PORT_3
 *       Sensor::PORT_4
 *
 *  Returns TouchSensor object.
 */
static mrb_value
mrb_touch_sensor_init(mrb_state *mrb, mrb_value self)
{
  return mrb_sensor_init(mrb, self, TOUCH_SENSOR);
}

/*
 *  call-seq:
 *     ts.pressed?  # => true/false
 *
 *  Get touch sensor status.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_touch_sensor_is_pressed(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_bool_value(EV3_touch_sensor_is_pressed(port));
}

/*
 *  call-seq:
 *     UltrasonicSensor.new(port)  # => UltrasonicSensor
 *
 *  Creates UltrasonicSensor object.
 *
 *  Parameters:
 *    +port+    UltrasonicSensor port
 *       Sensor::PORT_1
 *       Sensor::PORT_2
 *       Sensor::PORT_3
 *       Sensor::PORT_4
 *
 *  Returns UltrasonicSensor object.
 */
static mrb_value
mrb_ultrasonic_sensor_init(mrb_state *mrb, mrb_value self)
{
  return mrb_sensor_init(mrb, self, ULTRASONIC_SENSOR);
}

/*
 *  call-seq:
 *     us.rate  # => Fixnum
 *
 *  Measure distance to object in front of sensor.
 *
 *  Returns distance (cm).
 */
static mrb_value
mrb_ultrasonic_sensor_get_distance(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_ultrasonic_sensor_get_distance(port));
}

/*
 *  call-seq:
 *     us.listen  # => true/false
 *
 *  Listen for the presense of other ultrasonic sensor.
 *
 *  Returns true or false.
 */
static mrb_value
mrb_ultrasonic_sensor_listen(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_bool_value(EV3_ultrasonic_sensor_listen(port));
}

void
mrb_ev3_sensor_init(mrb_state *mrb, struct RClass *ev3, struct RClass *dev)
{
  struct RClass *sen, *cls, *gys, *tos, *uss;
  mrb_value seno, clso;

  /* Sensor class */
  sen = mrb_define_class_under(mrb, ev3, "Sensor", dev);
  seno = mrb_obj_value(sen);

  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "PORT_1"),     mrb_fixnum_value(EV3_PORT_1));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "PORT_2"),     mrb_fixnum_value(EV3_PORT_2));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "PORT_3"),     mrb_fixnum_value(EV3_PORT_3));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "PORT_4"),     mrb_fixnum_value(EV3_PORT_4));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "NONE"),       mrb_fixnum_value(NONE_SENSOR));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "ULTRASONIC"), mrb_fixnum_value(ULTRASONIC_SENSOR));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "GYRO"),       mrb_fixnum_value(GYRO_SENSOR));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "TOUCH"),      mrb_fixnum_value(TOUCH_SENSOR));
  mrb_const_set(mrb, seno, mrb_intern_lit(mrb, "COLOR"),      mrb_fixnum_value(COLOR_SENSOR));

  /* Color sensor class */
  cls = mrb_define_class_under(mrb, ev3, "ColorSensor", sen);
  clso = mrb_obj_value(sen);

  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "NONE"),   mrb_fixnum_value(COLOR_NONE));
  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "BLACK"),  mrb_fixnum_value(COLOR_BLACK));
  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "BLUE"),   mrb_fixnum_value(COLOR_BLUE));
  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "GREEN"),  mrb_fixnum_value(COLOR_GREEN));
  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "YELLOW"), mrb_fixnum_value(COLOR_YELLOW));
  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "RED"),    mrb_fixnum_value(COLOR_RED));
  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "WHITE"),  mrb_fixnum_value(COLOR_WHITE));
  mrb_const_set(mrb, clso, mrb_intern_lit(mrb, "BROWN"),  mrb_fixnum_value(COLOR_BROWN));

  mrb_define_method(mrb, cls, "initialize", mrb_color_sensor_init,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, cls, "ambient",    mrb_color_sensor_get_ambient, MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "color",      mrb_color_sensor_get_color,   MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "reflect",    mrb_color_sensor_get_reflect, MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "black?",     mrb_color_sensor_is_black,    MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "blue?",      mrb_color_sensor_is_blue,     MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "green?",     mrb_color_sensor_is_green,    MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "yellow?",    mrb_color_sensor_is_yellow,   MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "red?",       mrb_color_sensor_is_red,      MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "white?",     mrb_color_sensor_is_white,    MRB_ARGS_NONE());
  mrb_define_method(mrb, cls, "brown?",     mrb_color_sensor_is_brown,    MRB_ARGS_NONE());

  /* Gyro sensor class */
  gys = mrb_define_class_under(mrb, ev3, "GyroSensor", sen);

  mrb_define_method(mrb, gys, "initialize", mrb_gyro_sensor_init,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, gys, "angle",      mrb_gyro_sensor_get_angle,    MRB_ARGS_NONE());
  mrb_define_method(mrb, gys, "rate",       mrb_gyro_sensor_get_rate,     MRB_ARGS_NONE());
  mrb_define_method(mrb, gys, "reset",      mrb_gyro_sensor_reset,        MRB_ARGS_NONE());
  mrb_define_method(mrb, gys, "calibrate",  mrb_gyro_sensor_calibrate,    MRB_ARGS_NONE());
  mrb_define_method(mrb, gys, "offset",     mrb_gyro_sensor_get_offset,   MRB_ARGS_NONE());

  /* Touch sensor class */
  tos = mrb_define_class_under(mrb, ev3, "TouchSensor", sen);

  mrb_define_method(mrb, tos, "initialize", mrb_touch_sensor_init,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, tos, "pressed?",   mrb_touch_sensor_is_pressed,  MRB_ARGS_NONE());

  /* Ultrasonic sensor class */
  uss = mrb_define_class_under(mrb, ev3, "UltrasonicSensor", sen);

  mrb_define_method(mrb, uss, "initialize", mrb_ultrasonic_sensor_init,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, uss, "distance",   mrb_ultrasonic_sensor_get_distance, MRB_ARGS_NONE());
  mrb_define_method(mrb, uss, "listen",     mrb_ultrasonic_sensor_listen,       MRB_ARGS_NONE());
}
