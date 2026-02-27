/*
 * @Author: userName userEmail
 * @Date: 2026-02-25 10:52:48
 * @LastEditTime: 2026-02-26 19:11:55
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\Hardware\Src\Touch_XPT2046.c
 * @Description: 
 */
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "Touch_XPT2046.h"

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     if (GPIO_Pin == GPIO_PIN_8)
//     {
//         // HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
//     }
// }
static void XPT2046_Read(uint16_t* x,uint16_t* y)
{
    uint16_t x_raw,y_raw;
    uint8_t cmd = 0xD0;
    //x
    if(HAL_GPIO_ReadPin(PEN_GPIO_Port,PEN_Pin) == GPIO_PIN_SET)
    {
        return;
    }
    MYSPI_W_CLK(0);
    MYSPI_W_MOSI(0);
    MYSPI_W_CS(0);
    for(int i = 0;i < 8;i++)
    {
        if(cmd&0x80)
        {
            MYSPI_W_MOSI(1);
        }
        else
        {
            MYSPI_W_MOSI(0);
        }
        cmd<<=1;
        MYSPI_W_CLK(0);
        Delay_us(1);
        MYSPI_W_CLK(1);
    }

    Delay_us(6);
    MYSPI_W_CLK(0);
    Delay_us(1);
    MYSPI_W_CLK(1);
    Delay_us(1);
    MYSPI_W_CLK(0);

    for(int i = 0;i < 16;i++)
    {
        x_raw <<= 1;
        MYSPI_W_CLK(0);
        Delay_us(1);
        MYSPI_W_CLK(1);
        if(MYSPI_R_MISO)
        {
            x_raw++;
        }
    }
    x_raw >>= 4;
    MYSPI_W_CS(1);

    char buf[64];
    int len = sprintf(buf, "Touch Raw: (%d, %d)\r\n", x_raw, y_raw);
    HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 30);

}
/**
 * @description: 
 * @return {*}
 */
int8_t Touch_read_raw_xy(uint16_t* x,uint16_t* y)
{
    uint16_t x_sum = 0,y_sum = 0;
    uint16_t x_temp = 0,y_temp = 0;   

    //读取数据,均值滤波
    for(int i = 0; i < 4;i++)
    {
        XPT2046_Read(&x_temp,&y_temp);     
        x_sum += x_temp;
        y_sum += y_temp;
    }


    *x = x_sum/4;
    *y = y_sum/4;

    // char buf[64];
    // int len = sprintf(buf, "Touch111: (%d, %d)\r\n", *x, *y);
    // HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, 10);

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