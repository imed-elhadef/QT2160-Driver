/****************************************************************************/
 //   author  Imed Elhadef imed_elhadef@hotmail.fr
   
  
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _AT42QT2160_H_
#define _AT42QT2160_H_


#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#define QT2160_ADR        0x0D // The two pins I2CA1&I2CA0 are connected to GND 


#define GPIO_OUTPUT          0x1C
#define GPIO_LOW             0x00
#define GPIO1_HIGH           0x04
#define GPIO2_HIGH           0x08
#define GPIO3_HIGH           0x10
#define GPIO_HIGH            0x1C
#define CALIBRATION_VALUE    0x01  //NON ZERO VALUE
#define RESET_VALUE          0xF0  //NON ZERO VALUE
#define COMMON_CHANGE_VALUE  0x01
#define QT2160_ID            0x11 /* Chip ID (read from device-address 0) */

#define MASK_RESET          0x80 //0b10000000


#define CHIP_ID_REG           0x00
#define GENERAL_STATUS_REG    0x02
#define KEY_STATUS_ONE_REG    0x03
#define KEY_STATUS_TWO_REG    0x04
#define SLIDER_TOUCH_POS_REG  0x05
#define GPIO_READ_REG         0x06


#define CALIBRATION_REG       0x0A
#define RESET_REG             0x0B
#define SLIDER_CONTROL_REG    0x14
#define GPIO_DRIVE_REG        0x47
#define RESERVED_REG          0x48
#define GPIO_DIRECTION_REG    0x49
#define COMMON_CHANGE1_REG    0x4E
#define COMMON_CHANGE2_REG    0x4F
//Neg Threashold & Burst Length///////

#define NEGATIVE_THRESHOLD_REG       0x26
#define BURST_LENGTH_REG             0x36

struct QT2160
{
	char *dev; 	// device file i.e. /dev/i2c-N
	int addr;	// i2c address
	int fd;		// file descriptor
};

/*
 * opens the QT2160 device at [dev_fqn] (i.e. /dev/i2c-N) whose address is
 * [addr] and set the qt2160
 */
int qt2160_open(char *dev_fqn, int addr, struct QT2160*);
/*
 * closes the qt2160 device [e] 
 */
int qt2160_close(struct QT2160 *e);
/*
 * read and returns the qt2160 byte at reg address [reg_addr] 
 * Note: qt2160 must have been selected by ioctl(fd,I2C_SLAVE,address) 
 */
int qt2160_read_byte(struct QT2160* e, __u8 reg_addr);
/*
 * read the current byte
 * Note: qt2160 must have been selected by ioctl(fd,I2C_SLAVE,address) 
 */
int qt2160_read_current_byte(struct QT2160 *e);
/*
 * writes [data] at reg address [reg_addr] 
 * Note: qt2160 must have been selected by ioctl(fd,I2C_SLAVE,address) 
 */
int qt2160_write_byte(struct QT2160 *e, __u8 reg_addr, __u8 data);



#endif

