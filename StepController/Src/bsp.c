 #include "include.h"
 
 void NRF24L01_Connect_Check(void);
 
 void bsp_Check(void)
 {		
	 while(AT24CXX_Check())
		{
			USART_SendData("24C02 Check fail!\r\n");
			HAL_Delay(500);
		}
		USART_SendData("24C02 READY!\r\n");
		while(NRF24L01_Check())
		{
			USART_SendData("NRF24L01 Check fail!\r\n");
			HAL_Delay(500);
		}
		USART_SendData("NRF24L01 READY!\r\n");
		
		NRF24L01_Connect_Check();
 }
 
 void NRF24L01_Connect_Check(void)
 {
		uint8_t data;
		NRF24L01_RX_Mode();                               //设为接收模式
	 while(1)
	 {	 
		HAL_Delay(50);
		if(NRF24L01_RxPacket(&data)==0)
		{
			if(data==0x55)
				{
					USART_SendData("NRF24L01 Connect Sucess!\r\n");
					break;
				}
			else 
				USART_SendData("NRF24L01 Recieve Wrong Data!\r\n");
		}	
		else
		{
			USART_SendData("NRF24L01 have not recieve any data!\r\n");
		}
	 }
	 
	 NRF24L01_TX_Mode();																//设为发送模式
	 HAL_Delay(50);
	 data=0x56;
	 if(NRF24L01_TxPacket(&data)==TX_OK)								//返回0x56
	 {
			USART_SendData("NRF24L01 have send backdata!\r\n");
	 }
	 HAL_Delay(20);
 }

 