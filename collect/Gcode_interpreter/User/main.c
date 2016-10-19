/**
  ******************************************************************************
  * @file    main.c
  * @Author: MQjehovah
  * @version V1.0.0
  * @date    2015-2-27
  * @brief   主函数文件.
  ******************************************************************************
  * @attention
  * 
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Definition ----------------------------------------------------------------*/

#define COMMAND_SIZE 128     //our command string
char cmdbuffer[COMMAND_SIZE];
int serial_count;

/* Functions --------------------------------------------------------*/
/**
  ********************************************************************
  * @brief  main
  * @param  None
  * @retval None
  * @Note   None
  ********************************************************************
**/
int main(void)
{
	bool connect_status=true;
  bool response_status=true;
	MCU_status status=connect_request;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);    
  init_USART1();
	USART_SendStr("usart ok");
  LED_GPIO_Config();
	clear(0xe0);
  USART_SendStr("lcd ok");
  //init_steppers();
	//USART_SendStr("steppers ok");
	while(1)
	{
		switch (status)
		{
		  case connect_request:
	      while(connect_status)
	      {
	       	USART_SendData(USART1,status);
		    	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
			    {if (USART_ReceiveData(USART1)==connect_response){connect_status=false;status=wait;}}
      	}
				break;
			case wait:
	      while(response_status)
	      {
		      USART_SendData(USART1,status);
		      if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
		    	{if (USART_ReceiveData(USART1)==ok)
						{response_status=false;status=ok;
	    		   USART_SendData(USART1,status);
					  }
				  }
				}
				response_status=true;
		    	while(cmdbuffer[serial_count-1]!=end)
	      		{
		        	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
			        	{
			        		cmdbuffer[serial_count]=USART_ReceiveData(USART1);
			         		serial_count++;
			         	}
		       	}
				USART_SendData(USART1,status);									
		    process_string( cmdbuffer,serial_count-2);
				clear_process_string();
				status=wait;
				break;
						
		} 
	
	
    }
	
  }



/**
  ********************************************************************
  * @brief  clear_process_string
  * @param  None
  * @retval None
  * @Note   None
  ********************************************************************
**/
void clear_process_string()
{
	uint8_t i;
	for ( i=0; i<COMMAND_SIZE; i++) cmdbuffer[i] = 0;
	serial_count = 0;
}



/**
  ********************************************************************
  * @brief  delay_us
  * @param  None
  * @retval None
  * @Note   None
  ********************************************************************
**/
void delay_us(u16 time) 
{      
   u16 i=0;   
   while(time--) 
   {  
      i=10;  
      while(i--);
		}
}
				
			
			
/*********************************END OF FILE**********************************/
