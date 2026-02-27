/*
 * @Author: userName userEmail
 * @Date: 2026-02-25 22:24:19
 * @LastEditTime: 2026-02-25 22:57:42
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\Software_Driver\My_SPI.h
 * @Description: 
 */
#ifndef __MY_SPI_H_
#define __MY_SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

#define MYSPI_W_CS(x)  HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,(GPIO_PinState)x)

#define MYSPI_W_CLK(x)  HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,(GPIO_PinState)x)

#define MYSPI_W_MOSI(x) HAL_GPIO_WritePin(MOSI_GPIO_Port,MOSI_Pin,(GPIO_PinState)x)

#define MYSPI_R_MISO HAL_GPIO_ReadPin(MISO_GPIO_Port,MISO_Pin)

void My_SPI_Init(void);

#ifdef __cplusplus

}
#endif
#endif