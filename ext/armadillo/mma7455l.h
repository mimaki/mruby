/*
** mma7455l.h - Acceleration Sensor Driver
**
** Copyright(C) 2012 Kyushu Embedded Software Technology Consortium(QUEST)
** Copyright(C) 2012 FUKUOKA CSK CORP.
*/

#ifndef MMA7455L_H
#define MMA7455L_H

#include <stdint.h>

#define DEVICE_NAME "/dev/i2c-1"     /* I2C device name */
#define I2C_ADDR    0x1D             /* MMA7455LのI2Cアドレス */

#define MMA7455L_RESOLUTION_BITS 8   /* MMA7455Lの分解能(bit) */

#define MMA7455L_CH_MIN 0 /* MMA7455Lで指定できるアナログ入力チャンネルの最小値 */
#define MMA7455L_CH_MAX 3 /* MMA7455Lで指定できるアナログ入力チャンネルの最大値 */

//struct mma7455l;

//struct mma7455l *mma7455l_open(const char *dev_path, int addr);
//int mma7455l_read(struct mma7455l *mmafd, int8_t *x, int8_t *y , int8_t *z);
//int mma7455l_close(struct mma7455l *mmafd);
//int mma7455l_auto_calibration(struct mma7455l *mmafd);
int mma7455l_open(const char *dev_path, int addr);
int mma7455l_read(int fd, int8_t *x, int8_t *y , int8_t *z);
int mma7455l_close(int fd);

#endif /* MMA7455L_H */
