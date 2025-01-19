#include "oled.h"
#include "oled_font.h"

void _OLED_WriteCommand(uint8_t Command);

/**
 * @brief OLED Pin Initialization
 * @note We use the software approach to implement the 4-Pin OLED
 * communication. Note that the PB9 is for the SDA Pin on the OLED board
 * and the PB8 is for the SCL Pin on the OLED board.
 */
void OLED_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    OLED_W_SCL(1);
	OLED_W_SDA(1);

	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < 1000; j++);
	}
	
	_OLED_WriteCommand(0xAE);
	
	_OLED_WriteCommand(0xD5);
	_OLED_WriteCommand(0x80);
	
	_OLED_WriteCommand(0xA8);
	_OLED_WriteCommand(0x3F);
	
	_OLED_WriteCommand(0xD3);
	_OLED_WriteCommand(0x00);
	
	_OLED_WriteCommand(0x40);
	
	_OLED_WriteCommand(0xA1);
	
	_OLED_WriteCommand(0xC8);

	_OLED_WriteCommand(0xDA);
	_OLED_WriteCommand(0x12);
	
	_OLED_WriteCommand(0x81);
	_OLED_WriteCommand(0xCF);

	_OLED_WriteCommand(0xD9);
	_OLED_WriteCommand(0xF1);

	_OLED_WriteCommand(0xDB);
	_OLED_WriteCommand(0x30);

	_OLED_WriteCommand(0xA4);

	_OLED_WriteCommand(0xA6);

	_OLED_WriteCommand(0x8D);
	_OLED_WriteCommand(0x14);

	_OLED_WriteCommand(0xAF);
		
	OLED_Clear();
}

/**
 * @brief I2C start
 */
void _OLED_Start()
{
    OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
 * @brief I2C stop
 */
void _OLED_Stop()
{
    OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
 * @brief Send a byte to the OLED
 * @param Data The byte to be sent
 */
void _OLED_SendByte(uint8_t Data)
{
    uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(!!(Data & (0x80 >> i)));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);
	OLED_W_SCL(0);
}

/**
 * @brief Send a command to the OLED
 * @param Command The command to be sent
 */
void _OLED_WriteCommand(uint8_t Command)
{
	_OLED_Start();
	_OLED_SendByte(0x78);            // device addr
	_OLED_SendByte(0x00);            // write command
	_OLED_SendByte(Command); 
	_OLED_Stop();
}

/**
 * @brief Send data to the OLED
 * @param Data The data to be sent
 */
void _OLED_WriteData(uint8_t Data)
{
	_OLED_Start();
	_OLED_SendByte(0x78);		// device addr
	_OLED_SendByte(0x40);		// write data
	_OLED_SendByte(Data);
	_OLED_Stop();
}

/**
  * @brief  Set a cursor in OLED
  * @param  Y The Y axis from up to down, 0-7
  * @param  X The X axis from left to right, 0-127
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	_OLED_WriteCommand(0xB0 | Y);					//set Y axis
	_OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//set X axis high bits
	_OLED_WriteCommand(0x00 | (X & 0x0F));			//set X axis low bits
}

/**
  * @brief  Clear OLED
  */
void OLED_Clear()
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			_OLED_WriteData(0x00);
		}
	}
}

/**
 * @brief Print a char with a given coord
 * @param Row The row position (1-4)
 * @param Col The column position (1-16)
 * @param Data The character to be displayed
 */
void OLED_ShowChar(uint8_t Row, uint8_t Col, char Data)
{
	uint8_t i;
	OLED_SetCursor((Row - 1) * 2, (Col - 1) * 8);
	for (i = 0; i < 8; i++)
	{
		_OLED_WriteData(OLED_F8x16[Data - ' '][i]);
	}
	OLED_SetCursor((Row - 1) * 2 + 1, (Col - 1) * 8);
	for (i = 0; i < 8; i++)
	{
		_OLED_WriteData(OLED_F8x16[Data - ' '][i + 8]);
	}
}

/**
 * @brief Print a string with a given coord
 * @param Row The row position (1-4)
 * @param Col The column position (1-16)
 * @param Data The string to be displayed
 */
void OLED_ShowString(uint8_t Row, uint8_t Col, char *Data)
{
	uint8_t i;
	for (i = 0; Data[i] != '\0'; i++)
	{
		OLED_ShowChar(Row, Col + i, Data[i]);
	}
}

int8_t _OLED_GetLength(int8_t Data)
{
	int8_t len = 0;
	while (Data > 0)
	{
		len ++;
		Data /= 10;
	}
	return len;
}

uint32_t _OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void OLED_ShowNum(uint8_t Row, uint8_t Col, int32_t Data, uint8_t Size)
{
	int8_t Sign = Data >= 0 ? 1 : -1;
	uint32_t Number = Data * Sign;
	if (Sign >= 0)
	{
		for (int i = 0; i < Size; i++)							
		{
			OLED_ShowChar(Row, Col + i, Number / _OLED_Pow(10, Size - i - 1) % 10 + '0');
		}
	}
	else
	{
		OLED_ShowChar(Row, Col, '-');
		for (int i = 0; i < Size; i++)							
		{
			OLED_ShowChar(Row, Col + i + 1, Number / _OLED_Pow(10, Size - i - 1) % 10 + '0');
		}
	}
	
}
