 #include "include.h"
 

 void NRF24L01_Connect_Check(void);
 void USART_SendString(void const * argument, ...);
 
 /******************板级支持包初始化*******************/
 void bsp_Init(void)   
 {
		nrf24l01_init();
		HAL_Delay(20);
 }
 
 /*******************硬件初始化检测*******************/
 void bsp_Check(void)
 {		
	 while(AT24CXX_Check())
		{
			USART_SendString("24C02 Check Fail!\r\n");
			HAL_Delay(500);
		}
			USART_SendString("24C02 Ready!\r\n");
		while(NRF24L01_Check())
		{
			USART_SendString("NRF24L01 Check Fail!\r\n");
			HAL_Delay(500);
			HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		}
		USART_SendString("NRF24L01 READY!\r\n");
		
		NRF24L01_Connect_Check();
 }
 
 void NRF24L01_Connect_Check(void)
 {
		uint8_t data;
//		NRF24L01_RX_Mode();                               //设为接收模式
	 RX_Mode();
	 while(1)
	 {	 
		HAL_Delay(50);
		if(NRF24L01_RxPacket(&data)==0)
		{
			if(data==0x55)
				{
					USART_SendString("NRF24L01 Connect Sucess!\r\n");
						for(int i=0;i<=3;i++)
						{
							HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
							HAL_Delay(100);
						}
					break;
				}
			else 
				USART_SendString("NRF24L01 Recieve Wrong Data!\r\n");
		}	
		else
		{
			USART_SendString("NRF24L01 have not recieve any data!\r\n");
		}
	 }
	 
//	 NRF24L01_TX_Mode();																//设为发送模式
	 TX_Mode();
	 HAL_Delay(50);
	 data=0x56;
	 if(NRF24L01_TxPacket(&data)==TX_OK)								//返回0x56
	 {
			USART_SendString("NRF24L01 have send backdata!\r\n");
	 }
	 HAL_Delay(20);
 }

 
/********************串口发送数据（系统外）****************/
 void USART_SendString(void const * argument, ...)
{
    char  buf_str[50];
	
		sprintf(buf_str,"%s\r\n",argument);

    HAL_UART_Transmit(&huart1,(uint8_t*)buf_str,strlen(buf_str),300);

}

