#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*
		˵������������������ƶ�Ӧ����A+��A-��B+��B-
*/

/*
*********************************************************************************************************
*                                       GCC��׼��
*********************************************************************************************************
*/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>


/*
*********************************************************************************************************
*                                       �ⲿ/BSP
*********************************************************************************************************
*/
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bsp_dwt.h"
#include "bsp_24C02.h"
#include "bsp_NRF.h"
#include "bsp_stepper.h"

/*
*********************************************************************************************************
*                                       λ��Define
*********************************************************************************************************
*/
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(uint32_t)8<<28;}
#define SDA_OUT()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(uint32_t)3<<28;}

#define GPIOA_BASE_Addr   0x40010800
#define GPIOB_BASE_Addr   0x40010C00
#define GPIOC_BASE_Addr   0x40011000

#define GPIOA_ODR_Addr    0x4001080C 
#define GPIOA_IDR_Addr    0x40010808 
#define GPIOB_IDR_Addr    0x40010C08
#define GPIOB_ODR_Addr    0x40010C0C

#define IIC_SCL0 	HAL_GPIO_WritePin(GPIOB,SCL_Pin,GPIO_PIN_RESET)
#define IIC_SCL1 	HAL_GPIO_WritePin(GPIOB,SCL_Pin,GPIO_PIN_SET)

#define IIC_SDA0	HAL_GPIO_WritePin(GPIOB,SDA_Pin, GPIO_PIN_RESET)
#define IIC_SDA1	HAL_GPIO_WritePin(GPIOB,SDA_Pin, GPIO_PIN_RESET)


#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define READ_SDA  BIT_ADDR(GPIOB_IDR_Addr,7)
#define NRF24L01_CE  BIT_ADDR(GPIOA_ODR_Addr,4)
#define NRF24L01_CSN  BIT_ADDR(GPIOA_ODR_Addr,3)
#define NRF24L01_IRQ  BIT_ADDR(GPIOA_IDR_Addr,2)


#define DIRX BIT_ADDR(GPIOA_ODR_Addr,0)
#define DIRY BIT_ADDR(GPIOB_ODR_Addr,12)
#define DIRZ BIT_ADDR(GPIOB_ODR_Addr,13)

#define XSET0 BIT_ADDR(GPIOB_ODR_Addr,9)
#define XSET1 BIT_ADDR(GPIOB_ODR_Addr,8)

#define YSET0 BIT_ADDR(GPIOB_ODR_Addr,10)
#define YSET1 BIT_ADDR(GPIOB_ODR_Addr,11)

#define X_EN BIT_ADDR(GPIOB_ODR_Addr,5)
#define Y_EN BIT_ADDR(GPIOB_ODR_Addr,4)

/*
*********************************************************************************************************
*                                       ����������
*********************************************************************************************************
*/
void USART_SendData(void const * argument, ...);
void bsp_Check(void);

#endif
