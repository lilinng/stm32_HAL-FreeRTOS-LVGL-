/*
 * @Author: userName userEmail
 * @Date: 2026-02-25 10:53:09
 * @LastEditTime: 2026-02-25 18:33:31
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\Hardware\Inc\Touch_XPT2046.h
 * @Description: 
 */

#ifndef __TOUCH_XPT2046_H_
#define __TOUCH_XPT2046_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdbool.h"

typedef struct
{
    uint16_t x;
    uint16_t y;
    bool PRESS;
}Touch_t;

extern Touch_t Touch_Cache;

void Touch_Calibrate(uint16_t raw_x, uint16_t raw_y, uint16_t* cal_x, uint16_t* cal_y);

int8_t Touch_read_raw_xy(uint16_t* x,uint16_t* y);

void Touch_Init();

#ifdef __cplusplus
}
#endif
#endif