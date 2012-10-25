/*
** led.c - Led class for Armadillo 420
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
#define LED_CLASS_PATH "c:/temp/leds"

#else
#include <linux/input.h>
#define LED_CLASS_PATH "/sys/class/leds"

#endif

#define LED_RED       "/red"
#define LED_GREEN     "/green"
#define LED_FILE      "/brightness"
#define LED_REDID     1
#define LED_GREENID   2

#define LED_OFF       "0"
#define LED_ON        "1"

static const char *ledpath[] = {
  0,
  LED_CLASS_PATH LED_RED LED_FILE,
  LED_CLASS_PATH LED_GREEN LED_FILE
};

/*
 * call-seq:
 *   Led.new(color)  ->  Led
 */
static mrb_value
led_init(mrb_state *mrb, mrb_value self)
{
  mrb_value color;
  mrb_int c;

  /* 引数（LED番号）取得 */
  mrb_get_args(mrb, "i", &color);

  /* LED色番号チェック */
  c = mrb_fixnum(color);
  if (c < LED_REDID || c > LED_GREENID) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid LED number (%d)", c);
  }

  mrb_iv_set(mrb, self, mrb_intern(mrb, "@color"), color);

  return self;
}

static int
led_write(mrb_state *mrb, const char *path, char *buf, size_t len)
{
  int fd;
  int rc = -1;

  fd = open(path, O_WRONLY);
  if (fd < 0) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "LED cannot open. (%s)", path);
  }

  rc = write(fd, buf, len);

  close(fd);

  return rc;
}

static int
led_read(mrb_state *mrb, const char *path, char *buf, size_t len)
{
  int fd;
  int rc = -1;

  fd = open(path, O_RDONLY);
  if (fd < 0) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "LED cannot open. (%s)", path);
  }

  rc = read(fd, buf, len);

  close(fd);

  return rc;
}

/*
 * call-seq:
 *   led.on  ->  nil
 */
static mrb_value
led_on(mrb_state *mrb, mrb_value led)
{
  mrb_value color;
  mrb_int c;

  color = mrb_iv_get(mrb, led, mrb_intern(mrb, "@color"));
  c = mrb_fixnum(color);
//printf("led.on: color=%d\n", c);

  led_write(mrb, ledpath[c], LED_ON, sizeof(LED_ON)-1);

  return mrb_nil_value();
}

/*
 * call-seq:
 *   led.off  ->  nil
 */
static mrb_value
led_off(mrb_state *mrb, mrb_value led)
{
  mrb_value color;
  mrb_int c;

  color = mrb_iv_get(mrb, led, mrb_intern(mrb, "@color"));
  c = mrb_fixnum(color);
//printf("led.off: color=%d\n", c);

  led_write(mrb, ledpath[c], LED_OFF, sizeof(LED_OFF)-1);

  return mrb_nil_value();
}

/*
 * call-seq:
 *   led.on?  ->  bool
 */
static mrb_value
led_ison(mrb_state *mrb, mrb_value led)
{
  mrb_value color;
  mrb_int c;
  char onoff[1];

  color = mrb_iv_get(mrb, led, mrb_intern(mrb, "@color"));
  c = mrb_fixnum(color);
//printf("led.on?: color=%d\n", c);

  led_read(mrb, ledpath[c], onoff, sizeof(onoff));

  if (onoff[0] == LED_ON[0]) {
//printf("led.on? -> true");
    return mrb_true_value();
  }
//printf("led.on? -> false");
  return mrb_false_value();
}

void
mrb_init_led(mrb_state *mrb)
{
  struct RClass *led;

  /* クラス定義 */
  led = mrb_define_class(mrb, "Led", mrb->object_class);

  /* 定数定義 */
  mrb_define_const(mrb, led, "RED", mrb_fixnum_value(LED_REDID));
  mrb_define_const(mrb, led, "GREEN", mrb_fixnum_value(LED_GREENID));

  /* インスタンスメソッド定義 */
  mrb_define_method(mrb, led, "initialize", led_init, ARGS_REQ(1));
  mrb_define_method(mrb, led, "on", led_on, ARGS_NONE());
  mrb_define_method(mrb, led, "off", led_off, ARGS_NONE());
  mrb_define_method(mrb, led, "on?", led_ison, ARGS_NONE());
}
