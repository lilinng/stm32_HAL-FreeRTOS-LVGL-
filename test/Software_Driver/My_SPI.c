/*
 * @Author: userName userEmail
 * @Date: 2026-02-25 22:24:03
 * @LastEditTime: 2026-02-26 15:04:54
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\Software_Driver\My_SPI.c
 * @Description: 
 */
#include "stm32f1xx_hal.h"
#include "My_SPI.h"

void My_SPI_Init(void)
{
    MYSPI_W_CS(1);
    MYSPI_W_CLK(0);
}

void MySPI_Start(void)//起始条件：SS由高电平置低电平
{
	MYSPI_W_CS(0);
}
void MySPI_Stop(void)//结束条件，SS由低电平置高电平
{
	MYSPI_W_CS(1);
}
uint8_t MySPI_WriteByte(uint8_t Byte_Send)//交换字节
{
	for(int i=0;i<8;i++)
	{
		MYSPI_W_MOSI(Byte_Send & 0x80);//mode0在SCL的上升沿移入数据，下降沿移出数据
		Byte_Send<<=1;
		MYSPI_W_CLK(1);
		if(MYSPI_R_MISO==1)
		{
			Byte_Send|=0x01;
		}
		MYSPI_W_CLK(0);
	}
	return Byte_Send;
}

