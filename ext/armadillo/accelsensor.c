/*
** accelsensor.c - Acceleration Sensor class for I2C I/F
**
** Copyright(C) 2012 Kyushu Embedded Software Technology Consortium(QUEST)
** Copyright(C) 2012 FUKUOKA CSK CORP.
*/

#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/array.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "mma7455l.h"

#define ACCELCLASSMETHOD

#ifndef ACCELCLASSMETHOD
/*
 * call-seq:
 *   AccelSensor.new()  ->  AccelSensor
 */
static mrb_value
accel_init(mrb_state *mrb, mrb_value self)
{
  int fd;
  mrb_value v;

  /* 3軸加速度センサオープン */
#if defined(_WIN32) || defined(_WIN64)
  fd = 0;
//  fd = -EBADF;
#else
  fd = mma7455l_open(DEVICE_NAME, I2C_ADDR);
#endif
  if (fd < 0) {
    errno = -fd;
    mrb_raise(mrb, E_RUNTIME_ERROR, "MMA7455L cannot open. (%s)", strerror(errno));
  }

  /* ファイルディスクリプタ登録 */
  v = mrb_fixnum_value(fd);
  mrb_iv_set(mrb, self, mrb_intern(mrb, "fd"), v);

  return self;
}
#endif

/*
 * call-seq:
 *   accel.read  ->  Array
 */
static mrb_value
accel_read(mrb_state *mrb, mrb_value acc)
{
  mrb_value ary;
  int8_t x, y, z;
#ifndef ACCELCLASSMETHOD
  mrb_value v;
#endif
  int fd;

#ifdef ACCELCLASSMETHOD
  /* 3軸加速度センサオープン */
#if defined(_WIN32) || defined(_WIN64)
  fd = 0;
//  fd = -EBADF;
#else
  fd = mma7455l_open(DEVICE_NAME, I2C_ADDR);
#endif
  if (fd < 0) {
    errno = -fd;
    mrb_raise(mrb, E_RUNTIME_ERROR, "MMA7455L cannot open. (%s)", strerror(errno));
  }
#else
  /* 加速度センサファイルディスクリプタ取得 */
  v = mrb_iv_get(mrb, acc, mrb_intern(mrb, "fd"));
  fd = mrb_fixnum(v);
#endif

  /* センサ値取得 */
#if defined(_WIN32) || defined(_WIN64)
  x = 1;
  y = -2;
  z = 63;
#else
  if (mma7455l_read(fd, &x , &y , &z) < 0) {
    /* リードエラー */
    return mrb_nil_value();
  }
#endif

  ary = mrb_ary_new(mrb);
  mrb_ary_push(mrb, ary, mrb_fixnum_value(x));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(y));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(z));

#ifdef ACCELCLASSMETHOD
#if !defined(_WIN32) && !defined(_WIN64)
  /* 3軸加速度センサクローズ */
  mma7455l_close(fd);
#endif
#endif

  return ary;
}

void
mrb_init_accel(mrb_state *mrb)
{
  struct RClass *acc;

  /* クラス定義 */
  acc = mrb_define_class(mrb, "AccelSensor", mrb->object_class);

#ifdef ACCELCLASSMETHOD
//  mrb_undef_class_method(mrb, acc, "new");

  /* クラスメソッド定義 */
  mrb_define_class_method(mrb, acc, "read", accel_read, ARGS_NONE());
#else
  /* インスタンスメソッド定義 */
  mrb_define_method(mrb, acc, "initialize", accel_init, ARGS_NONE());
  mrb_define_method(mrb, acc, "read", accel_read, ARGS_NONE());
#endif
}
