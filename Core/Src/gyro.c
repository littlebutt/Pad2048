#include "gyro.h"

int LastStatus = 0;

void _Gyro_WriteReg(I2C_HandleTypeDef *hi2c2, uint8_t Addr, uint8_t Data);

uint8_t _Gyro_ReadReg(I2C_HandleTypeDef *hi2c2, uint8_t Addr);

void Gyro_Init(I2C_HandleTypeDef *hi2c2)
{
    _Gyro_WriteReg(hi2c2, PWR_MGMT_1, 0x01);
	_Gyro_WriteReg(hi2c2, PWR_MGMT_2, 0x00);
	_Gyro_WriteReg(hi2c2, SMPLRT_DIV, 0x09);
	_Gyro_WriteReg(hi2c2, CONFIG, 0x06);
	_Gyro_WriteReg(hi2c2, GYRO_CONFIG, 0x18);
	_Gyro_WriteReg(hi2c2, ACCEL_CONFIG, 0x18);
}

void _Gyro_WriteReg(I2C_HandleTypeDef *hi2c2, uint8_t Addr, uint8_t Data)
{
    HAL_I2C_Mem_Write(hi2c2, MPU6050_ADDR, Addr, I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
}

uint8_t _Gyro_ReadReg(I2C_HandleTypeDef *hi2c2, uint8_t Addr)
{
    uint8_t Data;
    HAL_I2C_Mem_Read(hi2c2, MPU6050_ADDR, Addr, I2C_MEMADD_SIZE_8BIT, &Data, 1, HAL_MAX_DELAY);
    return Data;
}

void Gyro_GetData(I2C_HandleTypeDef *hi2c2,
                    int16_t *AccX, int16_t *AccY, int16_t *AccZ,
					int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t DataH, DataL;
	DataH = _Gyro_ReadReg(hi2c2, ACCEL_XOUT_H);
	DataL = _Gyro_ReadReg(hi2c2, ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = _Gyro_ReadReg(hi2c2, ACCEL_YOUT_H);
	DataL = _Gyro_ReadReg(hi2c2, ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = _Gyro_ReadReg(hi2c2, ACCEL_ZOUT_H);
	DataL = _Gyro_ReadReg(hi2c2, ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = _Gyro_ReadReg(hi2c2, GYRO_XOUT_H);
	DataL = _Gyro_ReadReg(hi2c2, GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = _Gyro_ReadReg(hi2c2, GYRO_YOUT_H);
	DataL = _Gyro_ReadReg(hi2c2, GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = _Gyro_ReadReg(hi2c2, GYRO_ZOUT_H);
	DataL = _Gyro_ReadReg(hi2c2, GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}

int Gyro_GetDir(I2C_HandleTypeDef *hi2c2)
{
	int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;
	while (1)
	{
		Gyro_GetData(hi2c2, &AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);
		if (AccY > 800 && GyroY > 300)
		{
			if (LastStatus != LEFT)
			{
				LastStatus = LEFT;
				return LEFT;
			}
		}
		else if (AccY < -800 && GyroY < 300)
		{
			if (LastStatus != RIGHT)
			{
				LastStatus = RIGHT;
				return RIGHT;
			}
		}
		else if (AccX > 800 && GyroX > 300)
		{
			if (LastStatus != UP)
			{
				LastStatus = UP;
				return UP;
			}
		}
		else if (AccX < -800 && GyroX < 300)
		{
			if (LastStatus != DOWN)
			{
				LastStatus = DOWN;
				return DOWN;
			}
		}
	}
}
