#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "rtosif.h"


/*
 *  call-seq:
 *     delay(ms)  # => nil
 *
 *  Delay the processing of a task for the specified number of milliseconds.
 *
 *  Returns nil.
 */
static mrb_value
mrb_rtos_delay(mrb_state *mrb, mrb_value self)
{
  mrb_int t;
  mrb_get_args(mrb, "i", &t);
  RTOS_delay(t);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     millis  # => Fixnum
 *
 *  Get system time in milliseconds.
 *
 *  Returns system time. (ms)
 */
static mrb_value
mrb_rtos_millis(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(RTOS_millis());
}

/*
 *  call-seq:
 *     Task.new(id)  # => Task
 *
 *  Creates Task object.
 *
 *  Returns Task object.
 */
static mrb_value
mrb_task_init(mrb_state *mrb, mrb_value self)
{
  mrb_int id;
  mrb_get_args(mrb, "i", &id);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "id"), mrb_fixnum_value(id));
  return self;
}

/*
 *  call-seq:
 *     tsk.activate  # => nil
 *
 *  Activates Task.
 *
 *  Returns nil.
 */
static mrb_value
mrb_task_activate(mrb_state *mrb, mrb_value self)
{
  mrb_value id = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "id"));
  RTOS_act_tsk(mrb_fixnum(id));
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     tsk.suspend  # => nil
 *
 *  Suspends Task.
 *
 *  Returns nil.
 */
static mrb_value
mrb_task_suspend(mrb_state *mrb, mrb_value self)
{
  mrb_value id = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "id"));
  RTOS_sus_tsk(mrb_fixnum(id));
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     tsk.resume  # => nil
 *
 *  Resumes Task.
 *
 *  Returns nil.
 */
static mrb_value
mrb_task_resume(mrb_state *mrb, mrb_value self)
{
  mrb_value id = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "id"));
  RTOS_rsm_tsk(mrb_fixnum(id));
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     tsk.terminate  # => nil
 *
 *  Terminates Task.
 *
 *  Returns nil.
 */
static mrb_value
mrb_task_terminate(mrb_state *mrb, mrb_value self)
{
  mrb_value id = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "id"));
  RTOS_ter_tsk(mrb_fixnum(id));
  return mrb_nil_value();
}


void
mrb_rtos_init(mrb_state *mrb)
{
  struct RClass *rtos;
  struct RClass *tsk;

  /* RTOS module */
  rtos = mrb_define_module(mrb, "RTOS");

  /* RTOS api */
  mrb_define_method(mrb, rtos, "delay",  mrb_rtos_delay,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rtos, "millis", mrb_rtos_millis,  MRB_ARGS_NONE());

  /* Task class */
  tsk = mrb_define_class_under(mrb, rtos, "Task", mrb->object_class);

  mrb_define_method(mrb, tsk, "initialize", mrb_task_init,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, tsk, "activate",   mrb_task_activate,  MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "suspend",    mrb_task_suspend,   MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "resume",     mrb_task_resume,    MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "terminate",  mrb_task_terminate, MRB_ARGS_NONE());

  /* Object includes RTOS */
  mrb_include_module(mrb, mrb->object_class, rtos);
}
