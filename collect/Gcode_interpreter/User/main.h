/**
  ******************************************************************************
  * @file    main.h
  * $Author: MQjehovah
  * @brief   主函数包含的头文件.
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
**/

#ifndef __MAIN_H
#define __MAIN_H
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x_conf.h"
#include  "usart.h"
#include  "lcd.h"
#include  "gcode.h"
/* Definition ----------------------------------------------------------------*/
typedef enum 
	{
    connect_request =0x04,
		connect_response = 0x05,
		wait= 0x02,
		ok =0x01,
		end =0x03
	} MCU_status;
/* Exported Functions --------------------------------------------------------*/
void delay_us(u16 time) ;
void clear_process_string(void);
#endif
/*********************************END OF FILE**********************************/
