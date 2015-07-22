#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/hash.h"
#include "mruby/string.h"
#include "mruby/data.h"
#include "rtosif.h"

#define DTQ_MAGIC_CODE  0xBA5EC0DE

typedef ER (*SNDDTQ)(mrb_state*, ID, intptr_t, TMO);
typedef ER (*RCVDTQ)(mrb_state*, ID, intptr_t*, TMO);

static SNDDTQ
get_snd_dtq_func(TMO tmo)
{
  if (tmo == TMO_POL)   return RTOS_psnd_dtq;
  if (tmo == TMO_FEVR)  return RTOS_snd_dtq;
  return RTOS_tsnd_dtq;
}

static RCVDTQ
get_rcv_dtq_func(TMO tmo)
{
  if (tmo == TMO_POL)   return RTOS_prcv_dtq;
  if (tmo == TMO_FEVR)  return RTOS_rcv_dtq;
  return RTOS_trcv_dtq;
}

static void
mrb_dtq_free(mrb_state *mrb, void *data)
{
  if (data) {
    T_CDTQ *pcdtq = (T_CDTQ*)data;
    if (pcdtq->dtqmb) {
      mrb_free(mrb, pcdtq->dtqmb);
    }
    mrb_free(mrb, data);
  }
}
const static struct mrb_data_type mrb_dtq_type = {"DataQueue", mrb_dtq_free};

/*
 *  call-seq:
 *     DataQueue.new(id=nil, cnt=1024)  # => DataQueue
 *
 *  Create data queue.
 *  If +id+ is specified, the data queue is associated with ID.
 *
 *  Parameters:
 *    +id+      ID of 'existing' data queue
 *      !=nil     Create new DataQueue
 *    +cnt+     Capacity of data queue
 *
 *  Returns DataQueue object.
 */
static mrb_value
mrb_dtq_init(mrb_state *mrb, mrb_value self)
{
  ID id;
  mrb_value ido = mrb_nil_value();
  mrb_int dtqcnt = 1024;
  T_CDTQ *pcdtq;

  DATA_TYPE(self) = &mrb_dtq_type;
  pcdtq = (T_CDTQ*)mrb_malloc(mrb, sizeof(T_CDTQ));
  DATA_PTR(self) = pcdtq;
  memset(pcdtq, 0, sizeof(T_CDTQ));

  mrb_get_args(mrb, "|oi", &ido, &dtqcnt);

  if (mrb_nil_p(ido)) {
    pcdtq->dtqatr = TA_TFIFO;
    pcdtq->dtqcnt = dtqcnt;
    pcdtq->dtqmb = mrb_malloc(mrb, TSZ_DTQMB(dtqcnt));
    id = RTOS_acre_dtq(mrb, pcdtq);
  }
  else {
    id = mrb_fixnum(ido);
  }

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@id"), mrb_fixnum_value(id));

  return self;
}

static ER
dtq_senddata(mrb_state *mrb, mrb_int id, mrb_value msg, TMO tmo)
{
  intptr_t *ptr = (intptr_t*)RSTRING_PTR(msg);
  int len = RSTRING_LEN(msg);
  int cnt = len / sizeof(intptr_t);
  int rem = len % sizeof(intptr_t);
  SNDDTQ fn_snd_dtq = get_snd_dtq_func(tmo);

  /* send magic code */
  if (fn_snd_dtq(mrb, id, (intptr_t)DTQ_MAGIC_CODE, tmo) == E_TMOUT) {
    return E_TMOUT; /* send timeout */
  }

  /* send length (bytes) */
  if (fn_snd_dtq(mrb, id, (intptr_t)len, tmo) == E_TMOUT) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "DataQueue send timeout");  /* cannot continue */
  }

  /* send data */
  while (cnt--) {
    if (fn_snd_dtq(mrb, id, *ptr++, tmo) == E_TMOUT) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "DataQueue send timeout");  /* cannot continue */
    }
  }
  /* send remain data */
  if (rem) {
    intptr_t dt = 0;
    uint8_t *p = (uint8_t*)ptr;
    int sft = 0;
    /* assume 'little' endian */
    for (sft=0; rem--; p++,sft+=8) {
      dt |= ((intptr_t)*p << sft);
    }
    if (fn_snd_dtq(mrb, id, dt, tmo) == E_TMOUT) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "DataQueue send timeout");  /* cannot continue */
    }
  }
  return E_OK;
}

/*
 *  call-seq:
 *     DataQueue.send(msg, tmo=:forever)  # => String
 *
 *  Send message to data queue.
 *
 *  Parameters:
 *    +msg+     Message data to send
 *    +tmo+     Timeout time for sending
 *      :forever    Wait forever while sending
 *
 *  Returns sent data.
 */
mrb_value
mrb_dtq_send(mrb_state *mrb, mrb_value self)
{
  mrb_value ido, msg;
  mrb_value tmo = mrb_symbol_value(mrb_intern_lit(mrb, "forever"));
  TMO t;

  mrb_get_args(mrb, "o|o", &msg, &tmo);
  if (!mrb_string_p(msg)) {
    msg = mrb_funcall(mrb, msg, "to_s", 0);
  }
  t = mrb_timeout_value(mrb, tmo);

  ido = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id"));

  if (dtq_senddata(mrb, mrb_fixnum(ido), msg, t) != E_OK) {
    msg = mrb_nil_value();
  }

  return msg;
}

static mrb_value
dtq_receivedata(mrb_state *mrb, mrb_int id, TMO tmo)
{
  mrb_value msg;
  intptr_t dt, *ptr;
  uint8_t *p;
  int len, cnt, rem;
  RCVDTQ fn_rcv_dtq = get_rcv_dtq_func(tmo);

  /* wait magic code */
  do {
    if (fn_rcv_dtq(mrb, id, &dt, tmo) == E_TMOUT) {
      return mrb_nil_value(); /* DataQueue receive timeout */
    }
  } while (dt != DTQ_MAGIC_CODE);

  /* get length (bytes) */
  if (fn_rcv_dtq(mrb, id, &dt, tmo) == E_TMOUT) {
    return mrb_nil_value(); /* DataQueue receive timeout */
  }
  len = (int)dt;
  cnt = len / sizeof(intptr_t);
  rem = len % sizeof(intptr_t);
  msg = mrb_str_buf_new(mrb, len + 1);
  ptr = (intptr_t*)RSTRING_PTR(msg);

  /* get data */
  while (cnt--) {
    if (fn_rcv_dtq(mrb, id, ptr++, tmo) == E_TMOUT) {
      return mrb_nil_value(); /* DataQueue receive timeout */
    }
  }
  /* get remain data */
  p = (uint8_t*)ptr;
  if (rem) {
    if (fn_rcv_dtq(mrb, id, &dt, tmo) == E_TMOUT) {
      return mrb_nil_value(); /* DataQueue receive timeout */
    }
    /* assume 'little' endian */
    for ( ; rem--; p++, dt>>=8) {
      *p = (uint8_t)(dt & 0xff);
    }
  }
  *p = '\0';
  mrb_str_resize(mrb, msg, len);
// puts(RSTRING_PTR(msg));

  return msg;
}

/*
 *  call-seq:
 *     DataQueue.get(tmo=:forever)  # => String / nil
 *
 *  Get message from data queue.
 *
 *  Parameters:
 *    +tmo+     Timeout time for sending
 *      :forever    Wait forever while sending
 *
 *  Returns got message data.
 *  nil: Timeout occured.
 */
mrb_value
mrb_dtq_get(mrb_state *mrb, mrb_value self)
{
  mrb_value ido;
  mrb_value tmo = mrb_symbol_value(mrb_intern_lit(mrb, "forever"));
  TMO t;

  mrb_get_args(mrb, "|o", &tmo);
  t = mrb_timeout_value(mrb, tmo);

  ido = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@id"));

  return dtq_receivedata(mrb, mrb_fixnum(ido), t);
}

void
mrb_rtos_dataqueue_init(mrb_state *mrb, struct RClass *rtos)
{
  struct RClass *dtq;

  /* DataQueue class */
  dtq = mrb_define_class_under(mrb, rtos, "DataQueue", mrb->object_class);

  mrb_define_method(mrb, dtq, "initialize", mrb_dtq_init, MRB_ARGS_OPT(2));
  mrb_define_method(mrb, dtq, "send",       mrb_dtq_send, MRB_ARGS_ARG(1, 0));
  mrb_define_method(mrb, dtq, "get",        mrb_dtq_get,  MRB_ARGS_OPT(1));
}
