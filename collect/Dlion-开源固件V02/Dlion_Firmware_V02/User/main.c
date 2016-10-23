#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "lcd.h"
#include "beep.h" 
#include "switch.h"
#include "rtc.h" 
#include "adc.h"
#include "24cxx.h"
#include "flash.h" 
#include "spi.h"
#include "sram.h" 
#include "sdio_sdcard.h"
#include "touch.h"
#include "malloc.h"
#include "fontupd.h"
#include "text.h"
#include "ff.h"
#include "exfuns.h"
#include "lcdmenu.h"
#include "piclib.h"	
#include "lcdmenu.h"
#include "Dlion.h"      
//#include "setup.h"

/*******************************************************������Դ��3D��ӡѧϰʹ��************************************************
																												Dlion-3D��ӡ����
																												3D�����ƴ���---------������̳:www.3dbinmaker.com
																												�ļ�˵����main �������    �汾��V02
																												Copyright(C)�������ڿƼ����޹�˾
																												All rights reserved
***********************************************************************************************************************************/

int main(void)
 {
	SystemInit();
	delay_init();
	NVIC_Configuration();
	TIM4_Int_Init(9,7199);
	uart1_init(115200);
	BEEP_Init();
	BEEP=1;  
	delay_ms(10);
	BEEP=0;	
	LCD_Init();
	//SWITCH_Init();
	SPI2_Init();
	mem_init(SRAMIN);
	tp_dev.init();      
  

	if(SPI_Flash_ReadID()!= W25Q64)	
	{
		//LCD_ShowString(60,70,200,16,16,"W25Q64 Check Failed!");
	}


	if(SD_Init())	
	{
		//LCD_ShowString(60,90,200,16,16,"SD Card Check Failed!");
	}
	

 	exfuns_init();	 
  f_mount(0,fs[0]); 

	
	if(font_init())	
	{	
		while(SD_CD);
		delay_ms(10);
	  while(SD_Init());
		LCD_Clear(WHITE);
		if(update_font(5,0,12,0)==0 ) {}
		else
		{
			LCD_ShowString(5,80,200,200,12, "Font Updata Failed!");	
			while(1);
		}		
 	}
	


	gui_init();  
	piclib_init();
	
	start_screen();
	delay_ms(20000);
	delay_ms(20000);
	delay_ms(20000);
	delay_ms(20000);
	delay_ms(20000);
	
	setup();
	loop();
 }
