#ifndef ENZI_H
#define ENZI_H

#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/hash.h"

#define INTERN(name) mrb_intern(mrb, name)

extern const int8_t _anapin[];

void enzi_const_set(mrb_state*, struct RClass*, mrb_value, mrb_sym, mrb_value);

mrb_value enzi_get_iomode(mrb_state*, struct RClass*, mrb_value);

#endif /* ENZI_H */
