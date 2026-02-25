/*
 * @Author: userName userEmail
 * @Date: 2026-02-25 10:52:48
 * @LastEditTime: 2026-02-25 19:55:02
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\Hardware\Src\Touch_XPT2046.c
 * @Description: 
 */
#include "stm32f1xx_hal.h"
#include "spi.h"
#include "Touch_XPT2046.h"

Touch_t Touch_Cache = {0};

uint8_t Rx_Data[2] = {0};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_8)
    {
        if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == GPIO_PIN_RESET)
        {
            HAL_Delay(1);
            if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == GPIO_PIN_RESET)
            {
                Touch_Cache.PRESS = true;
            }
            else
            {
                Touch_Cache.PRESS = false;
            }
        }
        else
        {
            Touch_Cache.PRESS = false;
        }
    }
}
/**
 * @description: 
 * @return {*}
 */
int8_t Touch_read_raw_xy(uint16_t* x,uint16_t* y)
{
    /******************流程图***********************/
    /*人手触摸-->
    进入EXTI中断-->
    进行必要操作,如标记触摸事件等待read_xy任务-->
    FreeRTOS调度到该任务-->
    二次判断是否误触-->
    禁用EXTI防止函数重入(手册建议)-->
    计算得到x,y*/

    uint16_t x_temp = 0,y_temp = 0;
    uint8_t tx_data;

    //判断是否触摸(PEN引脚低电平)
    if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == GPIO_PIN_SET)
    {
        return -1;
    }
    //禁用PEN中断
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    //读取数据,均值滤波
    for(int i = 0; i < 4;i++)
    {
        //选中XPT2046,即拉低CS
        HAL_GPIO_WritePin(SPICS_GPIO_Port,SPICS_Pin,GPIO_PIN_RESET);
        tx_data = 0x90;
        HAL_SPI_Transmit(&hspi1,&tx_data,sizeof(tx_data),HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1,Rx_Data,sizeof(Rx_Data),HAL_MAX_DELAY);
        HAL_GPIO_WritePin(SPICS_GPIO_Port,SPICS_Pin,GPIO_PIN_SET);
        x_temp += ((Rx_Data[0]<<8)|Rx_Data[1])>>4;

        HAL_GPIO_WritePin(SPICS_GPIO_Port,SPICS_Pin,GPIO_PIN_RESET);
        tx_data = 0xD0;
        HAL_SPI_Transmit(&hspi1,&tx_data,sizeof(tx_data),HAL_MAX_DELAY);
        HAL_SPI_Receive(&hspi1,Rx_Data,sizeof(Rx_Data),HAL_MAX_DELAY);
        HAL_GPIO_WritePin(SPICS_GPIO_Port,SPICS_Pin,GPIO_PIN_SET);
        y_temp += ((Rx_Data[0]<<8)|Rx_Data[1])>>4;
    }

    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    *x = x_temp/4;
    *y = y_temp/4;

    return 0;
}
void Touch_Calibrate(uint16_t raw_x, uint16_t raw_y, uint16_t* cal_x, uint16_t* cal_y)
{
    // 校准参数（根据实际测量修改）
    const int32_t X_MIN = 200;
    const int32_t X_MAX = 3800;
    const int32_t Y_MIN = 200;
    const int32_t Y_MAX = 3800;

    const uint16_t LCD_W_1 = 319; // 320 - 1
    const uint16_t LCD_H_1 = 239; // 240 - 1

    // 安全检查：防止除零
    if (X_MAX <= X_MIN || Y_MAX <= Y_MIN) {
        *cal_x = 0;
        *cal_y = 0;
        return;
    }

    // 计算 X 坐标：映射 raw_x ∈ [X_MIN, X_MAX] → [0, LCD_W_1]
    int32_t x_range = X_MAX - X_MIN;
    int32_t y_range = Y_MAX - Y_MIN;

    // 使用公式: result = (raw - min) * output_max / range
    // 为实现四舍五入，先乘以 2，最后除以 2（即 +0.5 效果）
    int32_t x = (int32_t)(raw_x) - X_MIN;
    int32_t y = (int32_t)(raw_y) - Y_MIN;

    // 防止负值（如 raw 超出范围）
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    // 执行缩放（注意：先乘后除，避免精度损失）
    // 四舍五入技巧：(a * b + b/2) / b ≈ a 四舍五入，但这里我们对结果做：
    // (x * LCD_W_1 * 2 + x_range) / (x_range * 2) → 等价于 +0.5
    // 更简单方式：先计算，再手动加半量
    int32_t scaled_x = (x * LCD_W_1 + (x_range >> 1)) / x_range; // +0.5 效果
    int32_t scaled_y = (y * LCD_H_1 + (y_range >> 1)) / y_range;

    // 边界钳位（虽然理论上不会超，但防异常）
    if (scaled_x > LCD_W_1) scaled_x = LCD_W_1;
    if (scaled_y > LCD_H_1) scaled_y = LCD_H_1;
    if (scaled_x < 0) scaled_x = 0;
    if (scaled_y < 0) scaled_y = 0;

    *cal_x = (uint16_t)scaled_x;
    *cal_y = (uint16_t)scaled_y;
}
/**
 * @description: 
 * @return {*}
 */
void Touch_Init()
{
    //配置EXTI的参数
}