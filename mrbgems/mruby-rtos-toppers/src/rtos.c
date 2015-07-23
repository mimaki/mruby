#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/hash.h"
#include "rtosif.h"

/* initialize functions */
extern void mrb_rtos_flag_init(mrb_state*, struct RClass*);
extern void mrb_rtos_memory_init(mrb_state*, struct RClass*);
extern void mrb_rtos_dataqueue_init(mrb_state*, struct RClass*);

TMO
mrb_timeout_value(mrb_state *mrb, mrb_value tmo)
{
  if (mrb_fixnum_p(tmo)) {
    return (TMO)mrb_fixnum(tmo);
  }
  if (mrb_symbol_p(tmo)) {
    if (mrb_symbol(tmo) == mrb_intern_lit(mrb, "polling")) {
      return (TMO)TMO_POL;
    }
    if (mrb_symbol(tmo) == mrb_intern_lit(mrb, "forever")) {
      return (TMO)TMO_FEVR;
    }
  }
  mrb_raisef(mrb, E_TYPE_ERROR, "wrong timeout value (%S)", tmo);
}

/*
 *  call-seq:
 *     delay(ms)  # => nil
 *
 *  Delay the processing of a task for the specified number of milliseconds.
 *
 *  Parameters:
 *    +ms+      Delay time in milliseconds.
 *
 *  Returns nil.
 */
static mrb_value
mrb_rtos_delay(mrb_state *mrb, mrb_value self)
{
  mrb_int t;
  mrb_get_args(mrb, "i", &t);
  RTOS_tslp_tsk(mrb, t);
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
  SYSTIM st;
  RTOS_get_tim(mrb, &st);
  return mrb_fixnum_value((mrb_int)st);
}

/*
 *  call-seq:
 *     Task.new(id)  # => Task
 *
 *  Creates Task object.
 *
 *  Parameters:
 *    +id+      Task ID
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
  RTOS_act_tsk(mrb, mrb_fixnum(id));
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
  RTOS_sus_tsk(mrb, mrb_fixnum(id));
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
  RTOS_rsm_tsk(mrb, mrb_fixnum(id));
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
  RTOS_ter_tsk(mrb, mrb_fixnum(id));
  return mrb_nil_value();
}


void
mrb_mruby_rtos_toppers_gem_init(mrb_state *mrb)
{
  struct RClass *rtos;
  struct RClass *tsk;
  mrb_value wmd;
  mrb_value tmo;

  /* RTOS module */
  rtos = mrb_define_module(mrb, "RTOS");

  /* RTOS api */
  mrb_define_module_function(mrb, rtos, "delay",  mrb_rtos_delay,   MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, rtos, "millis", mrb_rtos_millis,  MRB_ARGS_NONE());

  wmd = mrb_hash_new(mrb);
  mrb_hash_set(mrb, wmd, mrb_symbol_value(mrb_intern_lit(mrb, "and")),   mrb_fixnum_value(TWF_ANDW));
  mrb_hash_set(mrb, wmd, mrb_symbol_value(mrb_intern_lit(mrb, "or")),    mrb_fixnum_value(TWF_ORW));
  mrb_mod_cv_set(mrb, rtos, mrb_intern_lit(mrb, "WAITMODE"), wmd);

  tmo = mrb_hash_new(mrb);
  mrb_hash_set(mrb, tmo, mrb_symbol_value(mrb_intern_lit(mrb, "polling")), mrb_fixnum_value(TMO_POL));
  mrb_hash_set(mrb, tmo, mrb_symbol_value(mrb_intern_lit(mrb, "forever")), mrb_fixnum_value(TMO_FEVR));
  mrb_mod_cv_set(mrb, rtos, mrb_intern_lit(mrb, "TIMEOUT"), tmo);

  /* Task class */
  tsk = mrb_define_class_under(mrb, rtos, "Task", mrb->object_class);

  mrb_define_method(mrb, tsk, "initialize", mrb_task_init,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, tsk, "activate",   mrb_task_activate,  MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "suspend",    mrb_task_suspend,   MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "resume",     mrb_task_resume,    MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "terminate",  mrb_task_terminate, MRB_ARGS_NONE());

  /* EventFlag class */
  mrb_rtos_flag_init(mrb, rtos);

  /* MemoryPool, MemoryBuffer */
  mrb_rtos_memory_init(mrb, rtos);

  /* DataQueue */
  mrb_rtos_dataqueue_init(mrb, rtos);
}

void
mrb_mruby_rtos_toppers_gem_final(mrb_state *mrb)
{
}
