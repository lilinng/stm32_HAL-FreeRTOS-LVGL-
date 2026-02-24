/*
 * @Author: userName userEmail
 * @Date: 2024-02-24 11:59:48
 * @LastEditTime: 2024-02-24 19:09:07
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\Hardware\Src\ILI9341.c
 * @Description: 适用于LCD驱动芯片ILI9341的初始化
 */
#include "ILI9341.h"
#include "LCD.h"

void ILI9341_Init(void)
{
  // 延长延迟以确保稳定
  HAL_Delay(100);
  
  // 重置序列
  LCD_WR_Cmd(0x01); // 软件重置
  HAL_Delay(200);
  
  LCD_WR_Cmd(0x28); // 关闭显示
  HAL_Delay(20);
  
  // 电源序列
  LCD_WR_Cmd(0xCF);
  LCD_WR_Data16(0x00);
  LCD_WR_Data16(0x83); // C1
  LCD_WR_Data16(0x30);

  LCD_WR_Cmd(0xED);
  LCD_WR_Data16(0x64);
  LCD_WR_Data16(0x03);
  LCD_WR_Data16(0x12);
  LCD_WR_Data16(0x81);

  LCD_WR_Cmd(0xE8);
  LCD_WR_Data16(0x85);
  LCD_WR_Data16(0x01); // 10 -> 01
  LCD_WR_Data16(0x79); // 7A -> 79

  LCD_WR_Cmd(0xCB);
  LCD_WR_Data16(0x39);
  LCD_WR_Data16(0x2C);
  LCD_WR_Data16(0x00);
  LCD_WR_Data16(0x34);
  LCD_WR_Data16(0x02);

  LCD_WR_Cmd(0xF7);
  LCD_WR_Data16(0x20);

  LCD_WR_Cmd(0xEA);
  LCD_WR_Data16(0x00);
  LCD_WR_Data16(0x00);

  LCD_WR_Cmd(0xC0);    // Power control
  LCD_WR_Data16(0x26);

  LCD_WR_Cmd(0xC1);    // Power control
  LCD_WR_Data16(0x11);

  LCD_WR_Cmd(0xC5);    // VCOM control
  LCD_WR_Data16(0x35);
  LCD_WR_Data16(0x3E);

  LCD_WR_Cmd(0xC7);    // VCOM control2
  LCD_WR_Data16(0xBE);

  LCD_WR_Cmd(0x36);    // Memory Access Control - 设置显示方向
  LCD_WR_Data16(0x48); // MY=1, MX=0, MV=0, ML=0, BGR=1

  LCD_WR_Cmd(0x3A);    // Pixel Format Set - 16位模式
  LCD_WR_Data16(0x55); // 16位/像素

  LCD_WR_Cmd(0xB1);    // Frame Rate Control
  LCD_WR_Data16(0x00);
  LCD_WR_Data16(0x1B);

  LCD_WR_Cmd(0xB6);    // Display Function Control
  LCD_WR_Data16(0x0A);
  LCD_WR_Data16(0xA2);
  LCD_WR_Data16(0x27);

  LCD_WR_Cmd(0xF2);    // 3Gamma function
  LCD_WR_Data16(0x00);

  LCD_WR_Cmd(0x26);    // Gamma Set
  LCD_WR_Data16(0x01);

  // Positive Gamma Correction
  LCD_WR_Cmd(0xE0);
  LCD_WR_Data16(0x0F);
  LCD_WR_Data16(0x31);
  LCD_WR_Data16(0x2B);
  LCD_WR_Data16(0x0C);
  LCD_WR_Data16(0x0E);
  LCD_WR_Data16(0x08);
  LCD_WR_Data16(0x4E);
  LCD_WR_Data16(0xF1);
  LCD_WR_Data16(0x37);
  LCD_WR_Data16(0x07);
  LCD_WR_Data16(0x10);
  LCD_WR_Data16(0x03);
  LCD_WR_Data16(0x0E);
  LCD_WR_Data16(0x09);
  LCD_WR_Data16(0x00);

  // Negative Gamma Correction
  LCD_WR_Cmd(0xE1);
  LCD_WR_Data16(0x00);
  LCD_WR_Data16(0x0E);
  LCD_WR_Data16(0x14);
  LCD_WR_Data16(0x03);
  LCD_WR_Data16(0x11);
  LCD_WR_Data16(0x07);
  LCD_WR_Data16(0x31);
  LCD_WR_Data16(0xC1);
  LCD_WR_Data16(0x48);
  LCD_WR_Data16(0x08);
  LCD_WR_Data16(0x0F);
  LCD_WR_Data16(0x0C);
  LCD_WR_Data16(0x31);
  LCD_WR_Data16(0x36);
  LCD_WR_Data16(0x0F);

  LCD_WR_Cmd(0x11); // Exit Sleep
  HAL_Delay(120);

  LCD_WR_Cmd(0x29); // Display on
  HAL_Delay(50);

  LCD_WR_Cmd(0x2C); // Memory Write
}