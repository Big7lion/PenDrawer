#ifndef __OLED_H
#define __OLED_H
#include "stm32f1xx_hal.h"

//********************************ADD TO GPIO.H********************
//**********************************BITBAND************************
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)






#define OLED_SCLK_Set() {PAout(1)=1;Delay_OLED(10);}
#define OLED_SDIN_Set() {PAout(3)=1;Delay_OLED(10);}
#define OLED_SCLK_Clr() {PAout(1)=0;Delay_OLED(10);}
#define OLED_SDIN_Clr() {PAout(3)=0;Delay_OLED(10);}
#define OLED_CMD  0
#define OLED_DATA 1
#define OPEN_OLED  Write_OLED(0x8d, OLED_CMD);Write_OLED(0x14, OLED_CMD);Write_OLED(0xAF, OLED_CMD)
#define CLOSE_OLED Write_OLED(0x8d, OLED_CMD);Write_OLED(0x10, OLED_CMD);Write_OLED(0xAe, OLED_CMD)
#define ZiKuan 6
#define ZiGao  8
#define HanZi  6
#define ShuZi  0

extern void OLED_Clear(uint8_t page1,uint8_t page2,uint8_t col1,uint8_t col2,uint8_t fs);
extern void OLED_WR_Dots(uint8_t x,uint8_t y,uint8_t state);
extern void OLED_Init(void);
extern void OLED_showWord(uint8_t row,uint8_t col,uint8_t fb,char const *p,...);
extern uint8_t videoMemory[8][128];

#endif

