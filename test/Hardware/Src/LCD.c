/*
 * @Author: userName userEmail
 * @Date: 2024-02-24 11:59:31
 * @LastEditTime: 2026-02-25 10:21:03
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\Hardware\Src\LCD.c
 * @Description: 提供了基于FSMC实现的对ILI9341的读写操作以及LCD的清屏和初始化函数
 */

#include "fsmc.h"
#include "LCD.h"
#include "ILI9341.h"

/**
 * @description: 向ILI9341发送command
 * @param {uint16_t} cmd command
 * @return {void}
 */
void LCD_WR_Cmd(uint16_t cmd)
{
    *LCD_CMD_ADDR = cmd; // 通过16位总线发送，LCD只取低8位作为命令
}

/**
 * @description: 对ILI9341发送16位像素数据（RGB565）
 * @param {uint16_t} data 16位颜色值
 * @return {void}
 */
void LCD_WR_Data16(uint16_t data)
{
    *LCD_DATA_ADDR = data; // 直接输出16位颜色到D0~D15
}

/**
 * @description: 对ILI9341发送8位数据（用于命令参数，如坐标、寄存器值等）
 * @param {uint8_t} data 8位数据（高8位自动为0）
 * @return {void}
 */
void LCD_WR_Data8(uint8_t data)
{
    *LCD_DATA_ADDR = data; // 通过16位总线发送，高8位=0，低8位=data
}

/**
 * @description: 设置光标位置（单点窗口）
 * @param {uint16_t} x 横坐标 (0~239)
 * @param {uint16_t} y 纵坐标 (0~319)
 * @return {void}
 */
void LCD_SetCursor(uint16_t x, uint16_t y)
{
    if(x > 239 || y > 319) return;
    
    // 列地址设置（0x2A）
    LCD_WR_Cmd(0x2A);
    LCD_WR_Data8((x >> 8) & 0xFF);  // 起始X高8位
    LCD_WR_Data8(x & 0xFF);         // 起始X低8位
    LCD_WR_Data8((x >> 8) & 0xFF);  // 结束X高8位（单点：同起始）
    LCD_WR_Data8(x & 0xFF);         // 结束X低8位

    // 行地址设置（0x2B）
    LCD_WR_Cmd(0x2B);
    LCD_WR_Data8((y >> 8) & 0xFF);  // 起始Y高8位
    LCD_WR_Data8(y & 0xFF);         // 起始Y低8位
    LCD_WR_Data8((y >> 8) & 0xFF);  // 结束Y高8位（单点：同起始）
    LCD_WR_Data8(y & 0xFF);         // 结束Y低8位

    // 准备写入GRAM
    LCD_WR_Cmd(0x2C);
}

/**
 * @description: 画一个像素点
 * @param {uint16_t} x X坐标
 * @param {uint16_t} y Y坐标
 * @param {uint16_t} color 16位颜色（RGB565）
 * @return {void}
 */
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    if(x > 239 || y > 319) return;
    LCD_SetCursor(x, y);
    LCD_WR_Data16(color);  
}

/**
 * @description: 设置LCD的刷新区域（用于批量写入）
 * @param {uint16_t} x1 起始X坐标
 * @param {uint16_t} y1 起始Y坐标
 * @param {uint16_t} x2 结束X坐标
 * @param {uint16_t} y2 结束Y坐标
 * @return {void}
 */
void LCD_SetWindows(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    // 可选：添加坐标校验（推荐）
    if (x1 > 239) x1 = 239;
    if (x2 > 239) x2 = 239;
    if (y1 > 319) y1 = 319;
    if (y2 > 319) y2 = 319;
    if (x1 > x2) { uint16_t t = x1; x1 = x2; x2 = t; }
    if (y1 > y2) { uint16_t t = y1; y1 = y2; y2 = t; }

    // 列地址设置（0x2A）
    LCD_WR_Cmd(0x2A);
    LCD_WR_Data8((x1 >> 8) & 0xFF);
    LCD_WR_Data8(x1 & 0xFF);
    LCD_WR_Data8((x2 >> 8) & 0xFF);
    LCD_WR_Data8(x2 & 0xFF);

    // 行地址设置（0x2B）
    LCD_WR_Cmd(0x2B);
    LCD_WR_Data8((y1 >> 8) & 0xFF);
    LCD_WR_Data8(y1 & 0xFF);
    LCD_WR_Data8((y2 >> 8) & 0xFF);
    LCD_WR_Data8(y2 & 0xFF);

    // 准备写入GRAM
    LCD_WR_Cmd(0x2C);
}

/**
 * @description: 批量写入像素数据（优化速度）
 * @param {uint16_t} *color 像素颜色数组（RGB565）
 * @param {uint32_t} len 像素数量
 * @return {void}
 */
void LCD_WriteMultiData(uint16_t *color, uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        LCD_WR_Data16(color[i]);  // 正确：16位颜色直接发送
    }
}

/**
 * @description: 清屏（填充全屏）
 * @param {uint16_t} color 清屏颜色（RGB565）
 * @return {void}
 */
void LCD_Clear(uint16_t color)
{
    // 列地址 0x2A: X = 0 ~ 239 (0x00EF)
    LCD_WR_Cmd(0x2A);
    LCD_WR_Data8(0x00);  // SCX_H
    LCD_WR_Data8(0x00);  // SCX_L
    LCD_WR_Data8(0x00);  // ECX_H
    LCD_WR_Data8(0xEF);  // ECX_L (239)

    // 行地址 0x2B: Y = 0 ~ 319 (0x013F)
    LCD_WR_Cmd(0x2B);
    LCD_WR_Data8(0x00);  // SCY_H
    LCD_WR_Data8(0x00);  // SCY_L
    LCD_WR_Data8(0x01);  // ECY_H
    LCD_WR_Data8(0x3F);  // ECY_L (319)

    LCD_WR_Cmd(0x2C);

    // 填充 240 * 320 = 76800 个像素
    for(uint32_t i = 0; i < 76800; i++)
    {
        LCD_WR_Data16(color);
    }
}

/**
 * @description: FSMC + ILI9341 初始化
 * @return {void}
 */
void LCD_Init(void)
{
    GPIO_InitTypeDef gpio_conf = {0};

    // 1. 使能时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_FSMC_CLK_ENABLE();

    // 2. 复位/背光引脚初始化
    gpio_conf.Pin = LCD_RST_PIN | LCD_BL_PIN;
    gpio_conf.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_conf.Pull = GPIO_PULLUP;
    gpio_conf.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_conf);

    // 3. 背光开启
    HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET);

    // 4. 硬件复位 ILI9341
    HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);

    // 5. ILI9341寄存器初始化
    ILI9341_Init();

    // 6. 清屏
    LCD_Clear(WHITE);
}