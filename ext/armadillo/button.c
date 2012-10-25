/*
** button.c - Button class for Armadillo 420
**
** Copyright(C) 2012 Kyushu Embedded Software Technology Consortium(QUEST)
** Copyright(C) 2012 FUKUOKA CSK CORP.
*/

#include "mruby.h"
#include "mruby/variable.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#if defined(_WIN32) || defined(_WIN64)
#define BUTTON_CLASS "c:/temp/input/button"
#else
#include <linux/input.h>
#define BUTTON_CLASS "/dev/input/event0"
#endif
#define EVENTSIZE 32
#define EVENTOFFSET 12

/*
 * call-seq:
 *   btn.on?  ->  bool
 */
static mrb_value
btn_ison(mrb_state *mrb, mrb_value btn)
{
  int fd;
  int rc = -1;
  char buf[EVENTSIZE];
  mrb_value v;

  fd = open(BUTTON_CLASS, O_RDONLY);
  if (fd < 0) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Button cannot open. (" BUTTON_CLASS ")");
  }

  rc = read(fd, buf, sizeof(buf));

  close(fd);

  if (rc != sizeof(buf)) {
    v = mrb_iv_get(mrb, btn, mrb_intern(mrb, "@on"));
    return mrb_nil_p(v) ? mrb_false_value() : v;
  }

  v = (buf[EVENTOFFSET] == 0) ? mrb_false_value() : mrb_true_value();

  mrb_iv_set(mrb, btn, mrb_intern(mrb, "@on"), v);

  return v;
}

void
mrb_init_button(mrb_state *mrb)
{
  struct RClass *btn;

  /* クラス定義 */
  btn = mrb_define_class(mrb, "Button", mrb->object_class);

  /* インスタンスメソッド定義 */
  mrb_define_method(mrb, btn, "on?", btn_ison, ARGS_NONE());
}
