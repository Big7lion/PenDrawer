#include "lcd.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "key.h" 
#include "led.h"
#include "24l01.h"
#include "malloc.h"
#include <string.h>
///*/////////////////////////////////////////////////////////////////////////////////	 

//NRF24L01：发送检查数据为0x55;接收检查数据为0x56; 帧首数据为0x80;


///////////////////////////////////////////////////////////////////////////////////*/
//========================variable==========================//
u16 ColorTab[5]={BRED,YELLOW,RED,GREEN,BLUE};//定义颜色数组
u16 *PenData=NULL;
//=====================end of variable======================//

//******************************************************************
//函数名：  DrawTestPage
//功能：    绘制测试界面
//输入参数：str :字符串指针
//返回值：  无
//******************************************************************
void DrawTestPage(u8 *str)
{
//绘制固定栏up
LCD_Fill(0,0,lcddev.width,20,BLUE);
//绘制固定栏down
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//居中显示
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"BIG LION",16,1);//居中显示
//绘制测试区域
LCD_Fill(0,20,lcddev.width,lcddev.height-20,BLACK);
}




//******************************************************************
//函数名：  Test_FillRec
//功能：    矩形框显示和填充测试，依次显示粉红色、黄色、红色、绿色、蓝色矩形框，
//       	延时1500毫秒后，依次按照粉红色、黄色、红色、绿色、蓝色填充矩形框 
//输入参数：无
//返回值：  无
//******************************************************************
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("测试2:GUI矩形填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		LCD_DrawRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		LCD_DrawFillRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	delay_ms(1500);
}

//******************************************************************
//函数名：  Test_FillRec
//功能：    圆形框显示和填充测试，依次显示粉红色、黄色、红色、绿色、蓝色圆形框，
//       	延时1500毫秒后，依次按照粉红色、黄色、红色、绿色、蓝色填充圆形框 
//输入参数：无
//返回值：  无
//******************************************************************
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("测试3:GUI画圆填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,0);
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,1);
	delay_ms(1500);
}


//******************************************************************
//函数名：  Chinese_Font_test
//功能：    中文显示测试
//输入参数：无
//返回值：  无
//******************************************************************
void Chinese_Font_test(void)
{	
	DrawTestPage("测试5:中文显示测试");
	Show_Str(10,30,BLUE,YELLOW,"16X16:全动电子技术有限公司欢迎您",16,0);
	Show_Str(10,50,BLUE,YELLOW,"16X16:Welcome全动电子",16,1);
	Show_Str(10,70,BLUE,YELLOW,"24X24:深圳市中文测试",24,1);
	Show_Str(10,100,BLUE,YELLOW,"32X32:字体测试",32,1);
	delay_ms(1200);
}


//******************************************************************
//函数名：  Touch_Request
//功能：    基于触摸手写测试修改的本项目函数
//输入参数：无
//返回值：  无
//******************************************************************
void Touch_Request(void)
{
	u8 key;
	u8 rmtmp=1;					//内存申请大小计数
	u8 tmp = 0;					//内存申请标志位
	u8 ltmp = 0;        //软件取点滤波计数
	u8 rectmp;					//应答数据
	vu8 pointx[5],pointy[5];
	u16 j=0;
	u16 colorTemp=0;		//颜色切换计数
	vu16 adjx=0,adjy=0;
	u32 i=1;					//屏幕画点计数
	TP_Init();
	KEY_Init();
//	TP_Adjust();  		//强制执行一次屏幕校准 (适用于没有IIC存储触摸参数的用户)

	DrawTestPage("Please Draw In Here");
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,RED); 
	POINT_COLOR=RED;
		while(1)
	{
		LCD_ShowString(10,45,12,"memory",0);
		LCD_ShowString(10,30,12,"point",0);
	 	key=KEY_Scan();
		tp_dev.scan(0); 	
		if(tmp==0)	 
		{
			PenData = (u16*)mymalloc(SRAMIN,100*sizeof(u16));     //内存申请
			PenData[0]=0x80;  //首位
			tmp=1;
		}
/*******************显示界面************************/
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
			{	
				if(tp_dev.x>(lcddev.width-24)&&tp_dev.y<16)
				{
					DrawTestPage("Please Draw In Here");//清除
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
					POINT_COLOR=colorTemp;
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
//					memset(PenData,0,i+1);
					myfree(SRAMIN,PenData);
					tmp=0;
					i=1;
				}
				else if((tp_dev.x>(lcddev.width-60)&&tp_dev.x<(lcddev.width-50+20))&&tp_dev.y<20)
				{
				LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,ColorTab[j%5]); 
				POINT_COLOR=ColorTab[(j++)%5];
				colorTemp=POINT_COLOR;
				delay_ms(10);
				}

				
/********************触屏数据******************/				
				else
				{
						TP_Draw_Big_Point(tp_dev.x-5,tp_dev.y-3,POINT_COLOR);		//画图
						pointx[ltmp]=tp_dev.x;pointy[ltmp]=tp_dev.y;ltmp++;
						if(ltmp==5)
						{
							PenData[i]=((pointx[3]-5)<<8)|(pointy[3]-3);															//存一个XY数据
							ltmp=0;
							i++;
							if(i/100>=rmtmp)					/*简单的溢出数据判断*/		
								{
									PenData=(u16*)myrealloc(SRAMIN,PenData,(rmtmp+1)*100*sizeof(u16));
									rmtmp++;
									LCD_ShowNum(80,45,my_mem_perused(SRAMIN),3,12);     //显示内部SRAM使用量
								}			
						}
				}
			
			}
		}
		else
		{
			delay_ms(20);	//没有按键按下的时候 
			LCD_ShowNum(80,30,i,5,12);
			if(i>10)
			{
					myfree(SRAMIN,PenData);
					tmp=0;
					i=1;
			}
		}
		
/*****************按键处理********************/		
		if(key==4)	//KEY_Wake_Up按下,则执行校准程序
		{

			LCD_Clear(WHITE);//清屏
		    TP_Adjust();  //屏幕校准 
			TP_Save_Adjdata();	 
			DrawTestPage("Please Draw In Here");
		}
		else if(key==1)
		{
			LCD_Fill(60,100,260,150,WHITE);
			POINT_COLOR=RED;
			LCD_ShowString(85,120,12,"START WIRELESS TRANSMIT!",1);
			NRF24L01_TX_Mode();
			NRF24L01_TxPacket(&rmtmp);
			NRF24L01_TxPacket16(PenData);
			delay_ms(20);
			NRF24L01_RX_Mode();
			while(NRF24L01_RxPacket(&rectmp));
			LCD_Fill(60,100,260,150,WHITE);
			if(rectmp==0x56)
			{
				LCD_ShowString(100,120,16,"TRANSMIT SUCCESS!",1);
				delay_ms(2000);
			}
			else 
				LCD_ShowString(70,1200,12,"NO lowerMCU ECHO! Please check!",1);
		}
	}   
}




