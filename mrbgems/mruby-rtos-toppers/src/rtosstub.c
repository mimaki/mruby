#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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

#define MAX_MPF 16
static T_CMPF gcmpf[MAX_MPF] = {{0}};
static ID gmpfid = 0;

ER_ID RTOS_acre_mpf(const T_CMPF *pcmpf)
{
  if (gmpfid >= MAX_MPF) {
    return -1;
  }
  gcmpf[gmpfid++] = *pcmpf;
  return 0;
}

ER RTOS_get_mpf(ID id, void **pblk)
{
  if (id >= gmpfid || gcmpf[id].blksz == 0) {
    return -1;
  }
  *pblk = malloc(gcmpf[id].blksz);
  return 0;
}

ER RTOS_pget_mpf(ID id, void **pblk)
{
  return RTOS_get_mpf(id, pblk);
}

ER RTOS_tget_mpf(ID id, void **pblk, TMO tmo)
{
  return RTOS_get_mpf(id, pblk);
}

ER RTOS_rel_mpf(ID id, void *pblk)
{
  free(pblk);
  return 0;
}

#endif
