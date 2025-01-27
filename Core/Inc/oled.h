#ifndef __OLED_H
#define __OLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"
#include "main.h"

#define OLED_W_SCL(x) HAL_GPIO_WritePin(GPIOB, OLED_SCL_Pin, (GPIO_PinState) x)
#define OLED_W_SDA(x) HAL_GPIO_WritePin(GPIOB, OLED_SDA_Pin, (GPIO_PinState) x)

void OLED_Init();

void OLED_SetCursor(uint8_t Y, uint8_t X);

void OLED_Clear();

void OLED_ShowChar(uint8_t Row, uint8_t Col, char Data);

void OLED_ShowString(uint8_t Row, uint8_t Col, char *Data);

void OLED_ShowNum(uint8_t Row, uint8_t Col, int32_t Data, uint8_t Size);

void OLED_ShowGameNum(uint8_t Row, uint8_t Col, int16_t Data);

#ifdef __cplusplus
}
#endif
#endif // __OLED_H
