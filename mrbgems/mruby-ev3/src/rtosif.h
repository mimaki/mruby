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

/* RTOS */
int32_t RTOS_delay(int32_t);
int32_t RTOS_millis(void);

/* Task */
int32_t RTOS_act_tsk(int32_t);
int32_t RTOS_sus_tsk(int32_t);
int32_t RTOS_rsm_tsk(int32_t);
int32_t RTOS_ter_tsk(int32_t);

#endif /* _RTOSIF_H */
