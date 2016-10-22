#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include "spi.h"
#include "24l01.h"
#include "dma.h"
#include "malloc.h"
#include "24cxx.h"

#define SEND_BUF_SIZE 8200	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.
u8 SendBuff[SEND_BUF_SIZE];	//�������ݻ�����


int main(void)
{		
	SystemInit();//��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	     //��ʱ��ʼ��
	LCD_Init();	   //Һ������ʼ��
//	SPI2_Init();
	AT24CXX_Init();
	NRF24L01_Init();
	
	MYDMA_Config(DMA1_Channel5,(u32)&SPI2->DR,(u32)SendBuff,SEND_BUF_SIZE);
	SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
/******************Ӳ�����*******************/
	LCD_Clear(WHITE);
	while(NRF24L01_Check())
			{LCD_ShowString(30,30,16,"NRF24L01 Check Failed!",1);}
	while(AT24CXX_Check())
			{LCD_ShowString(30,60,16,"AT24CXX Check Failed!",1);}
	LCD_ShowString(30,30,16,"NRF24L01 Check Success!",1);
	LCD_ShowString(30,60,16,"AT24CXX Check Success!",1);
	NRF24L01_Connect_Check();
	
	delay_ms(1000);
	
	
	while(1)
	{	

			main_test(); 		//����������
		//MYDMA_Enable(DMA1_Channel5);//��ʼһ��DMA���䣡	  
		//DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ��4������ɱ�־
		
		
		
		
/*		Test_Color();  		//��ˢ��������
		Test_FillRec();		//GUI���λ�ͼ����
		Test_Circle(); 		//GUI��Բ����
		English_Font_test();//Ӣ������ʾ������
		Chinese_Font_test();//��������ʾ������
		Pic_test();			//ͼƬ��ʾʾ������
*/
		Touch_Test();		//��������д����  */
	}				  
	
 }

