#ifndef _RTOSIF_H
#define _RTOSIF_H

// #define MRUBY_RTOS_DEBUG
#ifdef MRUBY_RTOS_DEBUG
  #define MRBROTS_PRINTF(fmt, ...) printf(fmt, __VA_ARGS__)
  #define MRBROTS_PUTS(x) puts(x)
#else
  #define MRBROTS_PRINTF(...)
  #define MRBROTS_PUTS(x)
#endif

#define UINT_C(x) (unsigned int)(x)

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

typedef struct t_cflg {
	ATR		flgatr;
	FLGPTN	iflgptn;
} T_CFLG;

/* RTOS */
ER RTOS_tslp_tsk(TMO);
ER RTOS_get_tim(SYSTIM*);

/* Task */
ER RTOS_act_tsk(ID);
ER RTOS_sus_tsk(ID);
ER RTOS_rsm_tsk(ID);
ER RTOS_ter_tsk(ID);

/* Event flag */
ER_ID RTOS_acre_flg(T_CFLG*);
ER RTOS_set_flg(ID, FLGPTN);
ER RTOS_wai_flg(ID, FLGPTN, MODE, FLGPTN*);
ER RTOS_pol_flg(ID, FLGPTN, MODE, FLGPTN*);
ER RTOS_twai_flg(ID, FLGPTN, MODE, FLGPTN*, TMO);
ER RTOS_clr_flg(ID, FLGPTN);

#endif /* _RTOSIF_H */
