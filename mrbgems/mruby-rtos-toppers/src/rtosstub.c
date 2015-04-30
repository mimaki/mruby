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

ER
RTOS_acre_flg(T_CFLG *pk_cflg)
{
  return 0;
}

ER
RTOS_set_flg(ID id, FLGPTN ptn)
{
  return ptn;
}

ER
RTOS_wai_flg(ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
  *p_flgptn = waiptn;
  return 0;
}

ER
RTOS_pol_flg(ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
  *p_flgptn = waiptn;
  return 0;
}

ER RTOS_twai_flg(ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn, TMO tmout)
{
  *p_flgptn = waiptn;
  return 0;
}

ER RTOS_clr_flg(ID id, FLGPTN clrptn)
{
  return 0;
}

#endif
