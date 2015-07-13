#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/hash.h"
#include "rtosif.h"

/* initialize functions */
extern void mrb_rtos_memory_init(mrb_state*, struct RClass*);

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
  RTOS_tslp_tsk(t);
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
  RTOS_get_tim(&st);
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


/*
 *  call-seq:
 *     EventFlag.new(id=nil, *modes)  # => EventFlag
 *
 *  Creates EventFlag object.
 *
 *  Parameters:
 *    +id+      Event flag ID
 *      nil       Assign ID automatically.
 *    +*modes+  Event flag attribute
 *      :auto       automatic-reset event (default)
 *      :manual     manual-reset event
 *      :fifo       task waiting FIFO queue (default)
 *      :priority   task Waiting priority queue
 *      :single     wait for single event (default)
 *      :multiple   wait for multiple event
 *
 *  Returns EventFlag object.
 */
static mrb_value
mrb_flag_init(mrb_state *mrb, mrb_value self)
{
  struct RClass *flg = mrb_obj_class(mrb, self);
  mrb_value amap = mrb_const_get(mrb, mrb_obj_value(flg), mrb_intern_lit(mrb, "ATTRIBUTE"));
  mrb_value ido = mrb_nil_value();
  mrb_value *pmode;
  mrb_int len;
  T_CFLG cflg = {0, 0};
  mrb_int id;
  mrb_int i;
  ATR flgatr = 0;
  mrb_value md;

  mrb_get_args(mrb, "|o*", &ido, &pmode, &len);

  for (i=0; i<len; i++) {
    md = mrb_hash_get(mrb, amap, pmode[i]);
    if (mrb_nil_p(md)) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid event flag attribute :%S", mrb_sym2str(mrb, mrb_symbol(pmode[i])));
    }
    flgatr |= mrb_fixnum(md);
  }
  cflg.flgatr = flgatr;

  if (mrb_nil_p(ido)) {
    id = RTOS_acre_flg(&cflg);
  }
  else {
    id = mrb_fixnum(ido);
    // TODO: cre_clg(id, cflg);
  }
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@id"), mrb_fixnum_value(id));

  return self;
}

/*
 *  call-seq:
 *     flg.set(ptn=1)  # => Fixnum
 *
 *  Set EventFlag signal.
 *
 *  Parameters:
 *    +ptn+     Event flag bit pattern for set
 *
 *  Returns EventFlag set value.
 */
static mrb_value
mrb_flag_set(mrb_state *mrb, mrb_value self)
{
  mrb_int id = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
  mrb_int ptn = 1;

  mrb_get_args(mrb, "|i", &ptn);

  RTOS_set_flg((ID)id, (FLGPTN)ptn);

  return mrb_fixnum_value(ptn);
}

/*
 *  call-seq:
 *     flg.wait(waiptn=1, mode=:and, tmout=:forever)  # => Fixnum
 *
 *  Wait EventFlag signal.
 *
 *  Parameters:
 *    +waiptn+  Event flag bit pattern for wait
 *    +mode+    Wait mode for wait signal.
 *      :and      Wait until all bits set
 *      :or       Wait until any bit set
 *    +tmout+
 *      :forever  Wait forever
 *      :polling  No wait
 *
 *  Returns EventFlag signaled value.
 */
static mrb_value
mrb_flag_wait(mrb_state *mrb, mrb_value self)
{
  mrb_value tmap = mrb_mod_cv_get(mrb, mrb_module_get(mrb, "RTOS"), mrb_intern_lit(mrb, "TIMEOUT"));
  mrb_int id = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
  mrb_int waiptn = 1;
  mrb_sym mds = mrb_intern_lit(mrb, "and");
  mrb_value tmo = mrb_symbol_value(mrb_intern_lit(mrb, "forever"));
  mrb_value tmov;
  FLGPTN flgptn = 0;
  mrb_int md = 0;

  mrb_get_args(mrb, "|ino", &waiptn, &mds, &tmo);

  if (mrb_fixnum_p(tmo)) {
    tmov = tmo;
  }
  else {
    tmov = mrb_hash_get(mrb, tmap, tmo);
    if (mrb_nil_p(tmov)) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid timeout value :%S", mrb_sym2str(mrb, mrb_symbol(tmo)));
    }
  }
  RTOS_twai_flg(id, waiptn, md, &flgptn, mrb_fixnum(tmov));

  return mrb_fixnum_value(flgptn);
}

/*
 *  call-seq:
 *     flg.clear(clrptn=1)  # => nil
 *
 *  Parameters:
 *    +clrptn+  Event flag bit pattern for clear
 *
 *  Clear EventFlag signal.
 *
 *  Returns nil.
 */
static mrb_value
mrb_flag_clear(mrb_state *mrb, mrb_value self)
{
  mrb_int id = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id")));
  mrb_int clrptn = 1;

  mrb_get_args(mrb, "|i", &clrptn);

  RTOS_clr_flg((ID)id, (FLGPTN)clrptn);

  return mrb_nil_value();
}


void
mrb_mruby_rtos_toppers_gem_init(mrb_state *mrb)
{
  struct RClass *rtos;
  struct RClass *tsk;
  struct RClass *flg;
  mrb_value flgo;
  mrb_value atr;
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
  // mrb_const_set(mrb, flgo, mrb_intern_lit(mrb, "WAITMODE"), wmd);
  mrb_mod_cv_set(mrb, rtos, mrb_intern_lit(mrb, "WAITMODE"), wmd);

  tmo = mrb_hash_new(mrb);
  mrb_hash_set(mrb, tmo, mrb_symbol_value(mrb_intern_lit(mrb, "polling")), mrb_fixnum_value(TMO_POL));
  mrb_hash_set(mrb, tmo, mrb_symbol_value(mrb_intern_lit(mrb, "forever")), mrb_fixnum_value(TMO_FEVR));
  // mrb_const_set(mrb, flgo, mrb_intern_lit(mrb, "TIMEOUT"), tmo);
  mrb_mod_cv_set(mrb, rtos, mrb_intern_lit(mrb, "TIMEOUT"), tmo);

  /* Task class */
  tsk = mrb_define_class_under(mrb, rtos, "Task", mrb->object_class);

  mrb_define_method(mrb, tsk, "initialize", mrb_task_init,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, tsk, "activate",   mrb_task_activate,  MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "suspend",    mrb_task_suspend,   MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "resume",     mrb_task_resume,    MRB_ARGS_NONE());
  mrb_define_method(mrb, tsk, "terminate",  mrb_task_terminate, MRB_ARGS_NONE());

  /* EventFlag class */
  flg = mrb_define_class_under(mrb, rtos, "EventFlag", mrb->object_class);
  flgo = mrb_obj_value(flg);

  atr = mrb_hash_new(mrb);
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "auto")),     mrb_fixnum_value(TA_CLR));
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "manual")),   mrb_fixnum_value(0));
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "fifo")),     mrb_fixnum_value(TA_TFIFO));
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "priority")), mrb_fixnum_value(TA_TPRI));
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "single")),   mrb_fixnum_value(TA_WSGL));
  mrb_hash_set(mrb, atr, mrb_symbol_value(mrb_intern_lit(mrb, "multiple")), mrb_fixnum_value(TA_WMUL));
  mrb_const_set(mrb, flgo, mrb_intern_lit(mrb, "ATTRIBUTE"), atr);

  mrb_define_method(mrb, flg, "initialize", mrb_flag_init,      MRB_ARGS_ANY());
  mrb_define_method(mrb, flg, "set",        mrb_flag_set,       MRB_ARGS_OPT(1));
  mrb_define_method(mrb, flg, "wait",       mrb_flag_wait,      MRB_ARGS_OPT(3));
  mrb_define_method(mrb, flg, "clear",      mrb_flag_clear,     MRB_ARGS_OPT(1));

  /* MemoryPool, MemoryBuffer */
  mrb_rtos_memory_init(mrb, rtos);
}

void
mrb_mruby_rtos_toppers_gem_final(mrb_state *mrb)
{
}
