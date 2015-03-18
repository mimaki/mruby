#include <stdio.h>
#include <stdint.h>
#include "rtosif.h"

#ifndef EV3

int32_t
RTOS_delay(int32_t tmo)
{
  return 0;
}

int32_t
RTOS_millis(void)
{
  return 0;
}

int32_t
RTOS_act_tsk(int32_t id)
{
  return 0;
}

int32_t
RTOS_sus_tsk(int32_t id)
{
  return 0;
}

int32_t
RTOS_rsm_tsk(int32_t id)
{
  return 0;
}

int32_t
RTOS_ter_tsk(int32_t id)
{
  return 0;
}
#endif
