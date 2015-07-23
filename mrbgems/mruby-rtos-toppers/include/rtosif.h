#ifndef _RTOSIF_H
#define _RTOSIF_H

#include "mruby.h"

// #define MRUBY_RTOS_DEBUG
#ifdef MRUBY_RTOS_DEBUG
  #define MRBROTS_PRINTF(fmt, ...) printf(fmt, __VA_ARGS__)
  #define MRBROTS_PUTS(x) puts(x)
#else
  #define MRBROTS_PRINTF(...)
  #define MRBROTS_PUTS(x)
#endif

#define UINT_C(x) (unsigned int)(x)

/* Error codes */
#define E_OK    0       /* OK */
#define E_SYS   (-5)    /* System error */
#define E_NOSPT (-9)    /* Not supported function*/
#define E_RSFN  (-10)   /* Function code is reserved */
#define E_RSATR (-11)   /* Attribute is reserved */
#define E_PAR   (-17)   /* Parameter error */
#define E_ID    (-18)   /* Invalid ID number */
#define E_CTX   (-25)   /* Context error */
#define E_MACV  (-26)   /* Memory access violation */
#define E_OACV  (-27)   /* Object access violation */
#define E_ILUSE (-28)   /* Service call was used illegally */
#define E_NOMEM (-33)   /* No memory */
#define E_NOID  (-34)   /* No ID */
#define E_NORES (-35)   /* No resouce */
#define E_OBJ   (-41)   /* Object status error */
#define E_NOEXS (-42)   /* Object not exist */
#define E_QOVR  (-43)   /* Queue overflow */
#define E_RLWAI (-49)   /* Release for wait */
#define E_TMOUT (-50)   /* Polling failure or timeout */
#define E_DLT   (-51)   /* Waiting object was deleted */
#define E_CLS   (-52)   /* Waiting object status was changed */
#define E_WBLK  (-57)   /* Non-blocking was accepted */
#define E_BOVR  (-58)   /* Buffer overflow */

/* Flags */
#define TA_TFIFO  UINT_C(0x00)
#define TA_TPRI   UINT_C(0x01)

#define TA_WSGL   UINT_C(0x00)
#define TA_WMUL   UINT_C(0x02)

#define TA_CLR    UINT_C(0x04)

#define TWF_ORW   UINT_C(0x01)
#define TWF_ANDW  UINT_C(0x02)

#define TMO_POL   0
#define TMO_FEVR	(-1)


typedef int ER;
typedef int ER_ID;
typedef int ID;
typedef int TMO;
typedef unsigned int MODE;
typedef unsigned int ATR;
typedef unsigned int FLGPTN;
typedef unsigned long SYSTIM;
typedef void *VP;

typedef struct t_csem {
  ATR sematr;
  unsigned int isemcnt;
  unsigned int maxsem;
} T_CSEM;

typedef struct t_cflg {
  ATR flgatr;
  FLGPTN iflgptn;
} T_CFLG;

typedef struct t_cmpf {
  ATR mpfatr;
  unsigned int blkcnt;
  unsigned int blksz;
  void *mpf;
  void *mpfmb;
} T_CMPF;

typedef struct t_cdtq {
  ATR dtqatr;
  unsigned int dtqcnt;
  void *dtqmb;
} T_CDTQ;
#define TSZ_DTQMB(dtqcnt)	(sizeof(intptr_t) * (dtqcnt))

/* for mruby */
TMO mrb_timeout_value(mrb_state*, mrb_value);

/* RTOS */
ER RTOS_tslp_tsk(mrb_state*, TMO);
ER RTOS_get_tim(mrb_state*, SYSTIM*);

/* Task */
ER RTOS_act_tsk(mrb_state*, ID);
ER RTOS_sus_tsk(mrb_state*, ID);
ER RTOS_rsm_tsk(mrb_state*, ID);
ER RTOS_ter_tsk(mrb_state*, ID);

/* Semaphore */
ER_ID RTOS_acre_sem(mrb_state*, T_CSEM*);
ER RTOS_sig_sem(mrb_state*, ID);
ER RTOS_wai_sem(mrb_state*, ID);
ER RTOS_pol_sem(mrb_state*, ID);
ER RTOS_twai_sem(mrb_state*, ID, TMO);

/* Event flag */
ER_ID RTOS_acre_flg(mrb_state*, T_CFLG*);
ER RTOS_set_flg(mrb_state*, ID, FLGPTN);
ER RTOS_wai_flg(mrb_state*, ID, FLGPTN, MODE, FLGPTN*);
ER RTOS_pol_flg(mrb_state*, ID, FLGPTN, MODE, FLGPTN*);
ER RTOS_twai_flg(mrb_state*, ID, FLGPTN, MODE, FLGPTN*, TMO);
ER RTOS_clr_flg(mrb_state*, ID, FLGPTN);

/* Fixed size memory pool */
ER_ID RTOS_acre_mpf(mrb_state*, const T_CMPF*);
ER RTOS_get_mpf(mrb_state*, ID, void**);
ER RTOS_pget_mpf(mrb_state*, ID, void**);
ER RTOS_tget_mpf(mrb_state*, ID, void**, TMO);
ER RTOS_rel_mpf(mrb_state*, ID, void*);

/* Data queue */
ER_ID RTOS_acre_dtq(mrb_state*, const T_CDTQ*);
ER RTOS_snd_dtq(mrb_state*, ID, intptr_t, TMO);
ER RTOS_psnd_dtq(mrb_state*, ID, intptr_t, TMO);
ER RTOS_tsnd_dtq(mrb_state*, ID, intptr_t, TMO);
ER RTOS_rcv_dtq(mrb_state*, ID, intptr_t*, TMO);
ER RTOS_prcv_dtq(mrb_state*, ID, intptr_t*, TMO);
ER RTOS_trcv_dtq(mrb_state*, ID, intptr_t*, TMO);

#endif /* _RTOSIF_H */
