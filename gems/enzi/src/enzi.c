/*
** enzi.c - enzi Library
**
** Copyright(c) FUKUOKA CSK CORPORATION
** Copyright(c) Manycolors Inc
*/

#include <stdio.h>
#include "enzi.h"

/* PIN assign definition */
static const int8_t _anapin[] = {0, 1, 2, 3, 4, 5};
static const int8_t _digpin[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 1, 2, 3, 4, 5};
static const int8_t _pwmpin[] = {3, 5, 6, 9, 10, 11};

void
enzi_const_set(mrb_state *mrb, struct RClass *enzi, mrb_value hash, mrb_sym sym, mrb_value v)
{
  mrb_mod_cv_set(mrb, enzi, sym, v);
  mrb_hash_set(mrb, hash, mrb_symbol_value(sym), v);
}

static void
enzi_init_constants(mrb_state *mrb, struct RClass *enzi)
{
  mrb_value hash;
  int i;
  char name[8];

  /* LEVEL */
  hash = mrb_hash_new_capa(mrb, 2);
  enzi_const_set(mrb, enzi, hash, INTERN("LOW"), mrb_fixnum_value(0));
  enzi_const_set(mrb, enzi, hash, INTERN("HIGH"), mrb_fixnum_value(1));
  mrb_mod_cv_set(mrb, enzi, INTERN("LEVEL"), hash);
//printf("LEVEL=");
//mrb_p(mrb, hash);

  /* IOMODE */
  hash = mrb_hash_new_capa(mrb, 3);
  enzi_const_set(mrb, enzi, hash, INTERN("INPUT"), mrb_fixnum_value(0));
  enzi_const_set(mrb, enzi, hash, INTERN("OUTPUT"), mrb_fixnum_value(1));
  enzi_const_set(mrb, enzi, hash, INTERN("INPUT_PULLUP"), mrb_fixnum_value(2));
  mrb_mod_cv_set(mrb, enzi, INTERN("IOMODE"), hash);
//printf("IOMODE=");
//mrb_p(mrb, hash);

  /* AIPIN */
  hash = mrb_hash_new_capa(mrb, sizeof(_anapin));
  for (i = 0; i < sizeof(_anapin); i++) {
    snprintf(name, sizeof(name), "A%d", i);
    enzi_const_set(mrb, enzi, hash, INTERN(name), mrb_fixnum_value(_anapin[i]));
  }
  mrb_mod_cv_set(mrb, enzi, INTERN("AIPIN"), hash);
//printf("AIPIN=");
//mrb_p(mrb, hash);

  /* DIOPIN */
  hash = mrb_hash_new_capa(mrb, sizeof(_digpin) + 1);
  for (i = 0; i < sizeof(_digpin); i++) {
    snprintf(name, sizeof(name), "D%d", i);
    enzi_const_set(mrb, enzi, hash, INTERN(name), mrb_fixnum_value(_digpin[i]));
  }
  enzi_const_set(mrb, enzi, hash, INTERN("DLED"), mrb_fixnum_value(14));
  mrb_mod_cv_set(mrb, enzi, INTERN("DIOPIN"), hash);
//printf("DIOPIN=");
//mrb_p(mrb, hash);

  /* PWMPIN */
  for (i = 0; i < sizeof(_pwmpin); i++) {
    snprintf(name, sizeof(name), "PWM%d", i);
    enzi_const_set(mrb, enzi, hash, INTERN(name), mrb_fixnum_value(_pwmpin[i]));
  }
  mrb_mod_cv_set(mrb, enzi, INTERN("PWMPIN"), hash);
//printf("PWMPIN=");
//mrb_p(mrb, hash);
}

mrb_value
enzi_get_iomode(mrb_state *mrb, struct RClass *c, mrb_value mode)
{
  mrb_value hash, v;

  if (mrb_fixnum_p(mode)) {
    return mode;
  }

  if (!mrb_symbol_p(mode)) {
    mrb_raise(mrb, E_TYPE_ERROR, "illegal mode");
  }

  hash = mrb_mod_cv_get(mrb, c, INTERN("IOMODE"));
  v = mrb_hash_get(mrb, hash, mode);
  if (mrb_nil_p(v)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "illegal mode: %s", mrb_sym2name(mrb, mrb_symbol(mode)));
  }

  return v;
}

static mrb_value
mrb_delay(mrb_state *mrb, mrb_value self)
{
  mrb_int ms;

  mrb_get_args(mrb, "i", &ms);

  // TODO: 
#ifdef ENZI_BOARD
#endif
  return mrb_nil_value();
}

static mrb_value
mrb_millis(mrb_state *mrb, mrb_value self)
{
  mrb_int ms = 0;

  // TODO: 
#ifdef ENZI_BOARD
#endif
  return mrb_fixnum_value(ms);
}

static mrb_value
mrb_sleep(mrb_state *mrb, mrb_value self)
{
  mrb_float sec;
  mrb_int ms;

  mrb_get_args(mrb, "f", &sec);
  ms = (mrb_int)(sec / 1000.0);

  // TODO: 
#ifdef ENZI_BOARD
#endif
  return mrb_nil_value();
}

void
mrb_enzi_gem_init(mrb_state *mrb)
{
  struct RClass *enzi;

  /* ENZI module */
  enzi = mrb_define_module(mrb, "ENZI");

  /* ENZI constants */
  enzi_init_constants(mrb, enzi);

  /* ENZI methods */
  mrb_define_method(mrb, enzi, "delay", mrb_delay, ARGS_REQ(1));
  mrb_define_method(mrb, enzi, "millis", mrb_millis, ARGS_NONE());
  mrb_define_method(mrb, enzi, "sleep", mrb_sleep, ARGS_REQ(1));

  /* initialize ENZI class libraries */
  mrb_init_digitalio(mrb, enzi);
  mrb_init_analogio(mrb, enzi);
  mrb_init_watchdog(mrb, enzi);

  mrb_include_module(mrb, mrb->object_class, enzi);
}

void
mrb_enzi_gem_final(mrb_state *mrb)
{
}

