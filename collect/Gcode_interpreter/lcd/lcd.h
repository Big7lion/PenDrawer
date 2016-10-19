/**
  ******************************************************************************
  * @file    lcd.h
  * @Author: MQjehovah
  * @version 1.0.0
  * @date    2015.3.23
  * @brief   
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
/* Definition ----------------------------------------------------------------*/
#define LCD_WR GPIO_Pin_12
#define LCD_RD GPIO_Pin_13 
#define LCD_CS GPIO_Pin_14
#define LCD_RS GPIO_Pin_15
#define uchar unsigned char 
#define uint  unsigned int
#define ulong unsigned long
#define SCREEN_WIDE 800
/* Exported Functions --------------------------------------------------------*/
void LED_GPIO_Config(void);
void WriteCMD(uchar cmd);
void WriteDAT(uchar dat);
void clear(uchar charbkcolor);
void putpixel(int x,int y, uchar color);
void DDALine(int x0,int y0,int x1,int y1,int color) ;
/*********************************END OF FILE**********************************/


