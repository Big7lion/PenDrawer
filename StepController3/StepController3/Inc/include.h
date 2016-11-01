#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*
		说明：步进电机红绿蓝黄对应的是A+，A-，B+，B-
*/

/*
*********************************************************************************************************
*                                       GCC标准库及OS
*********************************************************************************************************
*/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "crc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/*
*********************************************************************************************************
*                                       外部/BSP
*********************************************************************************************************
*/

#include "malloc.h"
#include "bsp_dwt.h"
#include "bsp_24C02.h"
#include "bsp_nrf.h"
#include "bsp_stepper.h"

/*
*********************************************************************************************************
*                                       位带Define
*********************************************************************************************************
*/
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(uint32_t)8<<28;}
#define SDA_OUT()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(uint32_t)3<<28;}

//IO地址映射表
#define GPIOA_ODR_Addr    0x4001080C 
#define GPIOB_ODR_Addr    0x40010C0C 
#define GPIOC_ODR_Addr    0x4001100C 
#define GPIOD_ODR_Addr    0x4001140C 
#define GPIOE_ODR_Addr    0x4001180C 
#define GPIOF_ODR_Addr    0x40011A0C    
#define GPIOG_ODR_Addr    0x40011E0C    

#define GPIOA_IDR_Addr    0x40010808 
#define GPIOB_IDR_Addr    0x40010C08 
#define GPIOC_IDR_Addr    0x40011008 
#define GPIOD_IDR_Addr    0x40011408 
#define GPIOE_IDR_Addr    0x40011808 
#define GPIOF_IDR_Addr    0x40011A08 
#define GPIOG_IDR_Addr    0x40011E08 
//#define SPI_BAUDRATEPRESCALER_8         ((uint32_t)0x00000010)

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define IIC_SCL 	BIT_ADDR(GPIOB_ODR_Addr,6)
#define IIC_SDA		BIT_ADDR(GPIOB_ODR_Addr,7)
#define READ_SDA  BIT_ADDR(GPIOB_IDR_Addr,7)

#define DIRX BIT_ADDR(GPIOA_ODR_Addr,2)
#define DIRY BIT_ADDR(GPIOC_ODR_Addr,5)
#define DIRZ BIT_ADDR(GPIOB_ODR_Addr,2)

#define XSET0 BIT_ADDR(GPIOC_ODR_Addr,1)
#define XSET1 BIT_ADDR(GPIOC_ODR_Addr,2)

#define YSET0 BIT_ADDR(GPIOA_ODR_Addr,5)
#define YSET1

#define X_EN BIT_ADDR(GPIOF_ODR_Addr,5)
#define Y_EN BIT_ADDR(GPIOA_ODR_Addr,3)

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

/*
*********************************************************************************************************
*                                       主函数调用
*********************************************************************************************************
*/
void bsp_Init(void);
void USART_SendData(void const * argument, ...);
void USART_SendString(void const * argument, ...);
void bsp_Check(void);

#endif
