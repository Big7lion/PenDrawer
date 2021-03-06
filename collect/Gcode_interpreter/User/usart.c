/**
  ******************************************************************************
  * @file    
  * @Author: 
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "stm32f10x_conf.h"
/* Definition ----------------------------------------------------------------*/

/* Functions --------------------------------------------------------*/
/**
  * @brief  None
  * @param  None
  * @retval None
  * @Note   None
**/
 void init_USART1(void)    
	 {   
		 GPIO_InitTypeDef GPIO_InitStructure;   
		 USART_InitTypeDef USART_InitStructure;     		  
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
     GPIO_Init(GPIOA, &GPIO_InitStructure);        
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
		 GPIO_Init(GPIOA, &GPIO_InitStructure);    
		 USART_InitStructure.USART_BaudRate = 9600;//115200;    
		 USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
		 USART_InitStructure.USART_StopBits = USART_StopBits_1;    
	   USART_InitStructure.USART_Parity = USART_Parity_No ;    
     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   
	   USART_Init(USART1, &USART_InitStructure);    
	   USART_Cmd(USART1, ENABLE);    
	 }

/**
  * @brief  None
  * @param  None
  * @retval None
  * @Note   None
**/
	 void USART_SendStr(char* senddata)
{
				int lenth=strlen(senddata);
        int i;
        for (i=0; i<lenth; i++)
        {
               USART_SendData(USART1,*senddata++);
					while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        }        
}  
/*********************************END OF FILE**********************************/

