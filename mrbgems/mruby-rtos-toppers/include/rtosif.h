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

typedef int ER;
typedef int ID;
typedef int TMO;
typedef unsigned long SYSTIM;

/* RTOS */
ER RTOS_tslp_tsk(TMO);
ER RTOS_get_tim(SYSTIM*);

/* Task */
ER RTOS_act_tsk(ID);
ER RTOS_sus_tsk(ID);
ER RTOS_rsm_tsk(ID);
ER RTOS_ter_tsk(ID);

#endif /* _RTOSIF_H */
