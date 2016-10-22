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

#define SEND_BUF_SIZE 8200	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.
u8 SendBuff[SEND_BUF_SIZE];	//发送数据缓冲区


int main(void)
{		
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	     //延时初始化
	LCD_Init();	   //液晶屏初始化
//	SPI2_Init();
	AT24CXX_Init();
	NRF24L01_Init();
	
	MYDMA_Config(DMA1_Channel5,(u32)&SPI2->DR,(u32)SendBuff,SEND_BUF_SIZE);
	SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
/******************硬件检查*******************/
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

			main_test(); 		//测试主界面
		//MYDMA_Enable(DMA1_Channel5);//开始一次DMA传输！	  
		//DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道4传输完成标志
		
		
		
		
/*		Test_Color();  		//简单刷屏填充测试
		Test_FillRec();		//GUI矩形绘图测试
		Test_Circle(); 		//GUI画圆测试
		English_Font_test();//英文字体示例测试
		Chinese_Font_test();//中文字体示例测试
		Pic_test();			//图片显示示例测试
*/
		Touch_Test();		//触摸屏手写测试  */
	}				  
	
 }

