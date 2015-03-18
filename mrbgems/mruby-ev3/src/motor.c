#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "ev3if.h"

/*
 *  call-seq:
 *     Motor.new(port, type)  # => Motor
 *
 *  Creates Motor object.
 *
 *  Parameters:
 *    +port+    Motor port
 *       Motor::PORT_A
 *       Motor::PORT_B
 *       Motor::PORT_C
 *       Motor::PORT_D
 *    +type+    Motor type
 *       Motor::NONE      none
 *       Motor::MEDIUM    medium motor
 *       Motor::LARGE     large motor
 *       Motor::UNKNOWN   Unregulated motor
 *
 *  Returns Motor object.
 */
static mrb_value
mrb_motor_init(mrb_state *mrb, mrb_value self)
{
  mrb_int port, type;

  mrb_get_args(mrb, "ii", &port, &type);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(port));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@type"), mrb_fixnum_value(type));

  EV3_motor_config(port, type);

  return self;
}

/*
 *  call-seq:
 *     motor.power = pwr  # => Fixnum
 *
 *  Sets motor power.
 *
 *  Parameters:
 *    +pwr+     Motor power (-100 to 100)
 *         > 0:   Motor forwards
 *         < 0:   Motor backwards
 *
 *  Returns power value.
 */
static mrb_value
mrb_motor_set_power(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  mrb_int v;

  mrb_get_args(mrb, "i", &v);
  if      (v > EV3_MOTOR_FMAX) v = EV3_MOTOR_FMAX;
  else if (v < EV3_MOTOR_BMAX) v = EV3_MOTOR_BMAX;

  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  EV3_motor_set_power(port, v);

  return mrb_fixnum_value(v);
}

/*
 *  call-seq:
 *     motor.power  # => Fixnum
 *
 *  Gets motor current power.
 *
 *  Returns power value.
 */
static mrb_value
mrb_motor_get_power(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_motor_get_power(port));
}

/*
 *  call-seq:
 *     motor.stop(brk)  # => nil
 *
 *  Stop the motor.
 *
 *  Parameters:
 *    +brk+
 *         true:  Brake motor
 *         false: Turn off motor
 *
 *  Returns nil
 */
static mrb_value
mrb_motor_stop(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  mrb_bool brake = FALSE;

  mrb_get_args(mrb, "|b", &brake);
  EV3_motor_stop(port, brake ? TRUE : FALSE);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     motor.rotate(deg, spd, blk)  # => nil
 *
 *  Rotate the motor.
 *
 *  Parameters:
 *    +deg+   Rotates through `deg` degrees
 *      > 0:    Rotate forward
 *      < 0:    Rotate backward
 *    +spd+   Rotate speed. (0 to 100)
 *    +blk+   Blocking mode
 *      false:  No blocking (default)
 *      true:   Blocking until completion
 *
 *  Returns nil
 */
static mrb_value
mrb_motor_rotate(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  mrb_int deg, spd, blk = 1;

  mrb_get_args(mrb, "ii|i", &deg, &spd, &blk);
  spd = (spd < 0) ? 0 : (spd > 100) ? 100 : spd;
  EV3_motor_rotate(port, deg, spd, blk);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     motor.count  # => Fixnum
 *
 *  Gets motor counter.
 *
 *  Returns counter value.
 */
static mrb_value
mrb_motor_get_count(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_motor_get_count(port));
}

/*
 *  call-seq:
 *     motor.reset  # => nil
 *
 *  Resets motor counter.
 *
 *  Returns counter value.
 */
static mrb_value
mrb_motor_reset_count(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  EV3_motor_reset_count(port);
  return mrb_nil_value();
}


void
mrb_ev3_motor_init(mrb_state *mrb, struct RClass *ev3, struct RClass *dev)
{
  struct RClass *mtr;
  mrb_value mtro;

  /* Motor class */
  mtr = mrb_define_class_under(mrb, ev3, "Motor", dev);
  mtro = mrb_obj_value(mtr);

  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_A"),   mrb_fixnum_value(EV3_PORT_A));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_B"),   mrb_fixnum_value(EV3_PORT_B));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_C"),   mrb_fixnum_value(EV3_PORT_C));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_D"),   mrb_fixnum_value(EV3_PORT_D));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "NONE"),     mrb_fixnum_value(NONE_MOTOR));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "MEDIUM"),   mrb_fixnum_value(MEDIUM_MOTOR));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "LARGE"),    mrb_fixnum_value(LARGE_MOTOR));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "UNKNOWN"),  mrb_fixnum_value(UNREGULATED_MOTOR));

  mrb_define_method(mrb, mtr, "initialize",   mrb_motor_init,         MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mtr, "power=",       mrb_motor_set_power,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mtr, "power",        mrb_motor_get_power,    MRB_ARGS_NONE());
  mrb_define_method(mrb, mtr, "stop",         mrb_motor_stop,         MRB_ARGS_OPT(1));
  mrb_define_method(mrb, mtr, "rotate",       mrb_motor_rotate,       MRB_ARGS_ARG(2, 1));
  mrb_define_method(mrb, mtr, "count",        mrb_motor_get_count,    MRB_ARGS_NONE());
  mrb_define_method(mrb, mtr, "reset_count",  mrb_motor_reset_count,  MRB_ARGS_NONE());
}
