#include <stdio.h>
#include <stdint.h>
#include "rtosif.h"

#ifndef EV3

ER
RTOS_tslp_tsk(TMO tmo)
{
  return 0;
}

ER
RTOS_get_tim(SYSTIM *st)
{
  *st = 0;
  return 0;
}

ER
RTOS_act_tsk(ID id)
{
  return 0;
}

ER
RTOS_sus_tsk(ID id)
{
  return 0;
}

ER
RTOS_rsm_tsk(ID id)
{
  return 0;
}

ER
RTOS_ter_tsk(ID id)
{
  return 0;
}
#endif
