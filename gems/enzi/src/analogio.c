/*
** analogio.c - AnalogIO class
**
** Copyright(c) FUKUOKA CSK CORPORATION
** Copyright(c) Manycolors Inc
*/

#include "enzi.h"
#include "analogio.h"
#include <stdio.h>

int32_t AnalogRead(int32_t);
void AnalogWrite(int32_t, int32_t);

#ifdef ENZI_JS
void _AnalogWrite(mrb_state*, int32_t, int32_t);
#define AnalogWrite(pin, duty) _AnalogWrite(mrb, (pin), (duty))
#endif


static mrb_value
enzi_get_aiopin(mrb_state *mrb, struct RClass *c, mrb_value pin)
{
  mrb_value hash, v;

  if (mrb_fixnum_p(pin)) {
    return pin;
  }

  if (!mrb_symbol_p(pin)) {
    mrb_raise(mrb, E_TYPE_ERROR, "illegal pin");
  }

  hash = mrb_mod_cv_get(mrb, c, INTERN("AIPIN"));
  v = mrb_hash_get(mrb, hash, pin);
  if (!mrb_nil_p(v)) {
    return v;
  }

  hash = mrb_mod_cv_get(mrb, c, INTERN("PWMPIN"));
  v = mrb_hash_get(mrb, hash, pin);
  if (mrb_nil_p(v)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "illegal pin: %s", mrb_sym2name(mrb, mrb_symbol(pin)));
  }

  return v;
}

static mrb_value
enzi_get_aioreftyp(mrb_state *mrb, struct RClass *c, mrb_value ref)
{
  mrb_value hash, v;

  if (mrb_fixnum_p(ref)) {
    return ref;
  }

  if (!mrb_symbol_p(ref)) {
    mrb_raise(mrb, E_TYPE_ERROR, "illegal ref");
  }

  hash = mrb_mod_cv_get(mrb, c, INTERN("REFERENCE"));
  v = mrb_hash_get(mrb, hash, ref);
  if (mrb_nil_p(v)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "illegal reftyp: %s", mrb_sym2name(mrb, mrb_symbol(ref)));
  }

  return v;
}

/*
 *  call-seq:
 *     AnalogIO.new(pin, mode, reftyp=DEFAULT, reso=8) => AnalogIO
 *
 *  Constructs a AnalogIO.
 */
mrb_value
mrb_aio_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value pin, mode;
  mrb_value ref = mrb_fixnum_value(ENZI_AIO_REF_DEFAULT);
  mrb_int reso = ENZI_AIO_RES_DEFAULT;
  struct RClass *c = mrb_object(self)->c;

  mrb_get_args(mrb, "oo|oi", &pin, &mode, &ref, &reso);

  pin = enzi_get_aiopin(mrb, c, pin);
  mode = enzi_get_iomode(mrb, c, mode);
  ref = enzi_get_aioreftyp(mrb, c, ref);

  mrb_iv_set(mrb, self, mrb_intern(mrb, "pin"), pin);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "mode"), mode);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "reftyp"), ref);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "reso"), mrb_fixnum_value(reso));

  return self;
}

/*
 *  call-seq:
 *     aio#read() => Fixnum
 *
 *  Read data from AnalogIO.
 */
mrb_value
mrb_aio_read(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(0);
}

/*
 *  call-seq:
 *     aio#write(v) => Fixnum
 *
 *  Write data to AnalogIO.
 */
mrb_value
mrb_aio_write(mrb_state *mrb, mrb_value self)
{
  mrb_int duty;
  mrb_value pin;

  mrb_get_args(mrb, "i", &duty);
  pin = mrb_iv_get(mrb, self, mrb_intern(mrb, "pin"));

//printf("AnalogIO#write(%d, %d) called.\n", mrb_fixnum(pin), duty);
  AnalogWrite((int32_t)mrb_fixnum(pin), (int32_t)duty);

  return mrb_fixnum_value(duty);
}

void
mrb_init_analogio(mrb_state *mrb, struct RClass *enzi)
{
  struct RClass *aio;
  mrb_value hash;

  /* AnalogIO class */
  aio = mrb_define_class_under(mrb, enzi, "AnalogIO", mrb->object_class);

  /* constants */
  /* REFERENCE */
  hash = mrb_hash_new_capa(mrb, 3);
  enzi_const_set(mrb, aio, hash, INTERN("DEFAULT"), mrb_fixnum_value(0));
  enzi_const_set(mrb, aio, hash, INTERN("INTERNAL"), mrb_fixnum_value(1));
  enzi_const_set(mrb, aio, hash, INTERN("EXTERNAL"), mrb_fixnum_value(2));
  mrb_mod_cv_set(mrb, aio, INTERN("REFERENCE"), hash);

  /* methods */
  mrb_define_method(mrb, aio, "initialize", mrb_aio_initialize, ARGS_REQ(2)|ARGS_OPT(2));
  mrb_define_method(mrb, aio, "read",       mrb_aio_read,       ARGS_NONE());
  mrb_define_method(mrb, aio, "write",      mrb_aio_write,      ARGS_REQ(1));
}

#ifdef ENZI_JS
//#include <stdio.h>
extern mrb_value mrb_js_funcall_argv(mrb_state*, const char*, int, mrb_value*);

void
_AnalogWrite(mrb_state *mrb, int32_t pin, int32_t duty)
{
  mrb_value argv[2];

  argv[0] = mrb_fixnum_value(pin);
  argv[1] = mrb_fixnum_value(duty);

//  printf("_AnalogWrite(%d, %d);\n", pin, duty);
  mrb_js_funcall_argv(mrb, "AnalogWrite", 2, argv);
}
#endif

