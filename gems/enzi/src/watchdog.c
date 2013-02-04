/*
** watchdog.c - WatchdogTimer class
**
** Copyright(c) FUKUOKA CSK CORPORATION
** Copyright(c) Manycolors Inc
*/

#include "enzi.h"

/*
 *  call-seq:
 *     WatchdogTimer.start => nil
 *
 *  start watchdog timer.
 */
mrb_value
mrb_wdt_start(mrb_state *mrb, mrb_value self)
{
  mrb_int t;

  mrb_get_args(mrb, "i", &t);

  mrb_cv_set(mrb, self, INTERN("started"), mrb_true_value());

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     WatchdogTimer.cancel => nil
 *
 *  cancel watchdog timer counter.
 */
mrb_value
mrb_wdt_cancel(mrb_state *mrb, mrb_value self)
{
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     WatchdogTimer.stop => nil
 *
 *  stop watchdog timer.
 */
mrb_value
mrb_wdt_stop(mrb_state *mrb, mrb_value self)
{
  mrb_cv_set(mrb, self, INTERN("started"), mrb_false_value());

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     WatchdogTimer.start? => true/false
 *
 *  check watchdog timer is started.
 */
mrb_value
mrb_wdt_isstart(mrb_state *mrb, mrb_value self)
{
  return mrb_cv_get(mrb, self, INTERN("started"));
}

void
mrb_init_watchdog(mrb_state *mrb, struct RClass *enzi)
{
  struct RClass *wdt;

  /* WatchdogTimer class */
  wdt = mrb_define_class_under(mrb, enzi, "WatchdogTimer", mrb->object_class);

  /* Class values */
  mrb_mod_cv_set(mrb, wdt, INTERN("started"), mrb_false_value());

  /* WatchdogTimer methods */
  mrb_define_class_method(mrb, wdt, "start", mrb_wdt_start, ARGS_REQ(1));
  mrb_define_class_method(mrb, wdt, "cancel", mrb_wdt_cancel, ARGS_NONE());
  mrb_define_class_method(mrb, wdt, "stop", mrb_wdt_stop, ARGS_NONE());
  mrb_define_class_method(mrb, wdt, "start?", mrb_wdt_isstart, ARGS_NONE());
}
