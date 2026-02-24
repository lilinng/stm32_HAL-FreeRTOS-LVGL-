#ifndef __LCD_H
#define __LCD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f1xx_hal.h"

// 16位FSMC地址（必须×2补偿）
#define LCD_CMD_ADDR  ((__IO uint16_t *)0x60000000) // 命令地址：A16=0 → RS=0
#define LCD_DATA_ADDR ((__IO uint16_t *)0x60020000) // 数据地址：A16=1 → RS=1（核心修正）

// 复位/背光引脚定义（假设你接PB0=BL，PB1=RST）
#define LCD_RST_PORT GPIOB
#define LCD_RST_PIN  GPIO_PIN_1
#define LCD_BL_PORT  GPIOB
#define LCD_BL_PIN   GPIO_PIN_0

// 常用颜色定义 (RGB565格式)
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define BRED           0XF81F
#define GRED           0XFFE0
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define BROWN          0xBC40
#define BRRED          0xFC07
#define GRAY           0x8430

// 函数声明
void LCD_WR_Cmd(uint16_t cmd);
void LCD_WR_Data16(uint16_t data);
void LCD_WR_Data8(uint8_t data);
void LCD_SetCursor(uint16_t x, uint16_t y);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_SetWindows(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_WriteMultiData(uint16_t *color, uint32_t len);
void LCD_Clear(uint16_t color);
void LCD_Init(void);

#ifdef __cplusplus
}
#endif
#endif