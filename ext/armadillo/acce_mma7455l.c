/*
** acce_mma7455l.c - Acceleration Sensor Driver
**
** Copyright(C) 2012 Kyushu Embedded Software Technology Consortium(QUEST)
** Copyright(C) 2012 FUKUOKA CSK CORP.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "mma7455l.h"

//#define MMA7455LTEST
//#define MAIN_C

//#define BASENAME(p)   ((strrchr((p), '/') ? : ((p) - 1)) + 1)


#define MMA7455L_RETRY_MAX 3 /* read()とwrite()のリトライ回数 */
//#define DEVICE_NAME "/dev/i2c-1"
//#define I2C_ADDR        0x1D    /* MMA7455LのI2Cアドレス */

#define ADJUSTX (7)
#define ADJUSTY (21)
#define ADJUSTZ (-12)

#define DEGREE(v) (int)((v) * 90.0 / 63.0 + 0.5)

#define LIMIT(v,lim)  (int8_t)(((v) < -(lim)) ? -(lim) : ((v) > (lim)) ? (lim) : (v))

#ifdef MMA7455LTEST

//struct mma7455l {
//        int fd;
//};
void debug_print(char*str){
//  printf( "%s\n" , str);
}

int main()
{
//  struct mma7455l *mmafd;
  int fd;
  int8_t x,y,z;
  int ret;
//  int count = 10;

//  mmafd = mma7455l_open(DEVICE_NAME, I2C_ADDR);
  fd = mma7455l_open(DEVICE_NAME, I2C_ADDR);
//  if (mmafd == NULL){
  if (fd < 0) {
    debug_print("mma7455l can't open");
    return -1;
  }
  //  while (count--){
  while (1){
//    ret = mma7455l_read(mmafd, &x , &y , &z);
//    if (ret == -1){
    if (mma7455l_read(fd, &x , &y , &z) < 0) {
      debug_print("mma7455l can't read");
      return -1;
    }
    printf("x=%d,y=%d,z=%d\n", x,y,z);
  }

//  ret = mma7455l_close(mmafd);
//  if (ret == -1){
  if (mma7455l_close(fd)) {
    debug_print("mma7455l can't close");
    return -1;
  }
  return EXIT_SUCCESS;
}

#endif /* MMA7455LTEST */

static ssize_t write_uninterruptible(int fd, const void *buf, size_t count, int retry)
{
  ssize_t ret;
  int i;

  for (i = 0; i < retry; i++) {
    ret = write(fd, buf, count);
    if (ret < 0 && errno != EINTR)
      return -1;

    if ((size_t)ret == count)
      return ret;
  }

  errno = ETIMEDOUT;
  return -1;
}

static ssize_t read_uninterruptible(int fd, void *buf, size_t count, int retry)
{
  size_t read_length = 0;
  ssize_t ret;
  int i;
  
  for (i = 0; i < retry; i++) {
    ret = read(fd, buf + read_length, count - read_length);
    if (ret < 0) {
      if (errno == EINTR)
        continue;
      else
        return -1;
    }

    read_length += ret;
    if (read_length == count)
      return read_length;
  }

  errno = ETIMEDOUT;
  return -1;
}

/**
 * 指定されたデバイスファイルをオープンする
 *
 * @param dev_path  MMA7455Lが接続されたi2cdevのデバイスファイルへのパス。
 * @param addr      MMA7455Lのアドレス。
 *
 * @return 成功するとMMA7455Lのファイルディスクリプタを返す。
 *         失敗するとエラーコード（0未満）を返す。
 */
//struct mma7455l *mma7455l_open(const char *dev_path, const int addr)
int mma7455l_open(const char *dev_path, const int addr)
{
//  struct mma7455l *mmafd;
  int fd;
//  int error;
//  int ret;
//  uint8_t buf[1];
  uint8_t buf[2];
  
  if (dev_path == NULL) {
    errno = EINVAL;
    return -EINVAL;
  }

//  mmafd = calloc(1, sizeof(struct mma7455l));
//  if (mmafd == NULL)
//    return NULL;
//  mmafd->fd = open(dev_path, O_RDWR);
//  printf("mmafd->fd=%d\n",mmafd->fd);  
//  if (mmafd->fd < 0) {
//    error = errno;
//    goto err1;
//  }
  fd = open(dev_path, O_RDWR);
//  printf("fd=%d\n", fd);
  if (fd < 0) {
    return fd;
  }

//  ret = ioctl(mmafd->fd, I2C_SLAVE, addr);
//  if (ret < 0) {
//    error = errno;
//    goto err2;
//  }
  if (ioctl(fd, I2C_SLAVE, addr) < 0) {
    close(fd);
    return -1;
  }

  //アドレス0x16に0x05を書き込む
  buf[0] = 0x16;
  buf[1] = 0x05;
//  ret = write_uninterruptible(mmafd->fd, buf ,2, MMA7455L_RETRY_MAX);
//  if (ret < 0)
//    return NULL;
//  return mmafd;
  if (write_uninterruptible(fd, buf, 2, MMA7455L_RETRY_MAX) < 0) {
    close(fd);
    return -1;
  }

  return fd;

// err2:
//  close(mmafd->fd);
// err1:
//  free(mmafd);
//  errno = error;
//  return NULL;
}


/**
 * MMA7455Lからx,y,z軸の結果を読み込む。
 * 読み込んだA/D変換結果をdigitに格納する。
 *
 * @param fd オープン済みのMMA7455Lデバイスディスクリプタ。
 * @param x,y,z 加速度センサーの値が格納される。
 *
 * @return 成功すると0を返し、x,y,zに値を格納する。
 *         失敗すると-1を返す。
 */
//int mma7455l_read(struct mma7455l *mmafd, int8_t *x, int8_t *y , int8_t *z)
int mma7455l_read(int fd, int8_t *x, int8_t *y , int8_t *z)
{
//  uint8_t buf[4];
  uint8_t cmd[1];
  int8_t buf[MMA7455L_CH_MAX];
//  int ret;
  int ix, iy, iz;

//  if (mmafd == NULL || x == NULL || y == NULL || z == NULL) {
  if (fd < 0 || x == NULL || y == NULL || z == NULL) {
    errno = EINVAL;
    return -1;
  }

//  buf[0] = 0x06;
//  ret = write_uninterruptible(mmafd->fd, buf, 1, MMA7455L_RETRY_MAX);
//  if (ret < 0){
  cmd[0] = 0x06;
  if (write_uninterruptible(fd, cmd, sizeof(cmd), MMA7455L_RETRY_MAX) < 0) {
    return -1;
  }

  /* x,y,zの3バイト読み込む */
//  ret = read_uninterruptible(mmafd->fd, buf, 3, MMA7455L_RETRY_MAX);
//  if (ret < 0){
  if (read_uninterruptible(fd, buf, sizeof(buf), MMA7455L_RETRY_MAX) < 0) {
    return -1;
  }

//  *x = buf[0] + ADJUSTX;
//  *y = buf[1] + ADJUSTY;
//  *z = buf[2] + ADJUSTZ;

//  if (*x > 63){
//    *x = 63;
//  }
//  else if (*x < -63){
//    *x = -63;
//  }
//  if (*y > 63){
//    *y = 63;
//  }
//  else if (*y < -63){
//    *y = -63;
//  }
//  if (*z > 63){
//    *z = 63;
//  }
//  else if (*z < -63){
//    *z = -63;
//  }

  ix = DEGREE(buf[0] + ADJUSTX);
  iy = DEGREE(buf[1] + ADJUSTY);
  iz = DEGREE(buf[2] + ADJUSTZ);

  *x = LIMIT(ix, 90);
  *y = LIMIT(iy, 90);
  *z = LIMIT(iz, 90);

  return 0;
}

/**
 * 指定されたMMA7455Lデバイスをクローズする
 *
 * @param mmafd オープン済みのMMA7455Lデバイスディスクリプタ。
 *
 * @return 成功すると0を返す。
 *         失敗すると-1を返す。
 */
//int mma7455l_close(struct mma7455l *mmafd)
int mma7455l_close(int fd)
{
//  int fd;
//
//  if (mmafd == NULL) {
//    errno = EINVAL;
//    return -1;
//  }
//
//  fd = mmafd->fd;
//  free(mmafd);
//
  return close(fd);
}
