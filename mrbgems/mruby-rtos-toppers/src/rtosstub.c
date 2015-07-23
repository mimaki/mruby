#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rtosif.h"
#include "mruby.h"

#ifndef EV3

ER
RTOS_tslp_tsk(mrb_state *mrb, TMO tmo)
{
  return 0;
}

ER
RTOS_get_tim(mrb_state *mrb, SYSTIM *st)
{
  *st = 0;
  return 0;
}

ER
RTOS_act_tsk(mrb_state *mrb, ID id)
{
  return 0;
}

ER
RTOS_sus_tsk(mrb_state *mrb, ID id)
{
  return 0;
}

ER
RTOS_rsm_tsk(mrb_state *mrb, ID id)
{
  return 0;
}

ER
RTOS_ter_tsk(mrb_state *mrb, ID id)
{
  return 0;
}

/* Semaphore */
ER_ID RTOS_acre_sem(mrb_state *mrb, T_CSEM *pcsem)
{
  return 0;
}

ER RTOS_sig_sem(mrb_state *mrb, ID id)
{
  return E_OK;
}

ER RTOS_wai_sem(mrb_state *mrb, ID id)
{
  return E_OK;
}

ER RTOS_pol_sem(mrb_state *mrb, ID id)
{
  return E_OK;
}

/* Event flag */
ER
RTOS_acre_flg(mrb_state *mrb, T_CFLG *pk_cflg)
{
  return 0;
}

ER
RTOS_set_flg(mrb_state *mrb, ID id, FLGPTN ptn)
{
  return ptn;
}

ER
RTOS_wai_flg(mrb_state *mrb, ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
  *p_flgptn = waiptn;
  return 0;
}

ER
RTOS_pol_flg(mrb_state *mrb, ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
  *p_flgptn = waiptn;
  return 0;
}

ER RTOS_twai_flg(mrb_state *mrb, ID id, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn, TMO tmout)
{
  *p_flgptn = waiptn;
  return 0;
}

ER RTOS_clr_flg(mrb_state *mrb, ID id, FLGPTN clrptn)
{
  return 0;
}

#define MAX_MPF 16
static T_CMPF gcmpf[MAX_MPF] = {{0}};
static ID gmpfid = 0;

ER_ID RTOS_acre_mpf(mrb_state *mrb, const T_CMPF *pcmpf)
{
  ID id = gmpfid++;
  if (id >= MAX_MPF) {
    return -1;
  }
  gcmpf[id] = *pcmpf;
  return id;
}

ER RTOS_get_mpf(mrb_state *mrb, ID id, void **pblk)
{
  if (id >= gmpfid || gcmpf[id].blksz == 0) {
    return -1;
  }
  *pblk = malloc(gcmpf[id].blksz);
  return 0;
}

ER RTOS_pget_mpf(mrb_state *mrb, ID id, void **pblk)
{
  return RTOS_get_mpf(mrb, id, pblk);
}

ER RTOS_tget_mpf(mrb_state *mrb, ID id, void **pblk, TMO tmo)
{
  return RTOS_get_mpf(mrb, id, pblk);
}

ER RTOS_rel_mpf(mrb_state *mrb, ID id, void *pblk)
{
  free(pblk);
  return 0;
}

#define MAX_DTQ 16
typedef struct mrb_dtq_t {
  int wp;
  int rp;
  T_CDTQ dtq;
} mrb_dtq_t;
static mrb_dtq_t gcdtq[MAX_DTQ] = {{0}};
static ID gdtqid = 0;

ER_ID	RTOS_acre_dtq(mrb_state *mrb, const T_CDTQ *pcdtq)
{
  ID id = gdtqid++;
  if (id >= MAX_DTQ) {
    return E_ID;
  }
  gcdtq[id].wp = 0;
  gcdtq[id].rp = 0;
  gcdtq[id].dtq = *pcdtq;
  return id;
}

ER RTOS_snd_dtq(mrb_state *mrb, ID id, intptr_t data, TMO tmout/*not use*/)
{
  if (id >= MAX_DTQ) {
    return E_ID;
  }
  ((intptr_t*)gcdtq[id].dtq.dtqmb)[gcdtq[id].wp] = data;
  gcdtq[id].wp = (gcdtq[id].wp + 1) % gcdtq[id].dtq.dtqcnt;
// {
//   int i;
//   for (i=gcdtq[id].rp; i<gcdtq[id].wp; i++) {
//     printf("%016lx,", ((intptr_t*)gcdtq[id].dtq.dtqmb)[i]);
//   }
//   printf("\n");
// }
  return 0;
}

ER RTOS_psnd_dtq(mrb_state *mrb, ID id, intptr_t data, TMO tmout/*not use*/)
{
  return RTOS_snd_dtq(mrb, id, data, tmout);
}

ER RTOS_tsnd_dtq(mrb_state *mrb, ID id, intptr_t data, TMO tmout)
{
  return RTOS_snd_dtq(mrb, id, data, tmout);
}

ER RTOS_rcv_dtq(mrb_state *mrb, ID id, intptr_t *data, TMO tmout/*not use*/)
{
  if (id >= MAX_DTQ) {
    return E_ID;
  }
  *data = ((intptr_t*)gcdtq[id].dtq.dtqmb)[gcdtq[id].rp];
  gcdtq[id].rp = (gcdtq[id].rp + 1) % gcdtq[id].dtq.dtqcnt;
  return 0;
}

ER RTOS_prcv_dtq(mrb_state *mrb, ID id, intptr_t *data, TMO tmout/*not use*/)
{
  return RTOS_rcv_dtq(mrb, id, data, tmout);
}

ER RTOS_trcv_dtq(mrb_state *mrb, ID id, intptr_t *data, TMO tmout)
{
  return RTOS_rcv_dtq(mrb, id, data, tmout);
}

#endif  /* EV3 */
