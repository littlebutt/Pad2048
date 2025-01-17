#ifndef __GYRO_H
#define __GYRO_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"
#include "main.h"

#define MPU6050_ADDR    0xD0

#define	SMPLRT_DIV	    0x19
#define	CONFIG		    0x1A
#define	GYRO_CONFIG	    0x1B	
#define	ACCEL_CONFIG	0x1C	
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H	    0x41
#define	TEMP_OUT_L	    0x42
#define	GYRO_XOUT_H	    0x43
#define	GYRO_XOUT_L	    0x44	
#define	GYRO_YOUT_H	    0x45
#define	GYRO_YOUT_L	    0x46
#define	GYRO_ZOUT_H	    0x47
#define	GYRO_ZOUT_L	    0x48
#define	PWR_MGMT_1	    0x6B
#define	PWR_MGMT_2	    0x6C
#define	WHO_AM_I	    0x75

enum {
    LEFT = 1,
    UP,
    RIGHT,
    DOWN
};

void Gyro_Init(I2C_HandleTypeDef *hi2c2);

void Gyro_GetData(I2C_HandleTypeDef *hi2c2,
                int16_t *AccX, int16_t *AccY, int16_t *AccZ,
				int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

int Gyro_GetDir(I2C_HandleTypeDef *hi2c2);

#ifdef __cplusplus
}
#endif

#endif // __GYRO_H