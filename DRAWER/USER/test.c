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
u8 *PenDataX=NULL;
u8 *PenDataY=NULL;
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
	u8 counter = 0;     //笔状态计时计数
	u8 PenBit = 0;			//笔状态标志位
	u8 m=0;
	vu8 pointx[5],pointy[5];
	u16 j=0;
	u16 colorTemp=RED;		//颜色切换计数
	vu16 adjx=0,adjy=0;  //坐标数据暂存缓冲
	u32 i=1;					//屏幕画点计数
	TP_Init();
	KEY_Init();
//	TP_Adjust();  		//强制执行一次屏幕校准 (适用于没有IIC存储触摸参数的用户)

	DrawTestPage("Please Draw In Here");
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,RED); 
	POINT_COLOR=RED;
	
	
	LCD_ShowString(10,45,12,"memory",1);
	LCD_ShowString(10,30,12,"point",1);
		while(1)
	{

	 	key=KEY_Scan();
		tp_dev.scan(0); 	
		if(tmp==0)	 
		{
			PenDataX = (u8*)mymalloc(SRAMIN,100*sizeof(u8));     //内存申请
			PenDataX[0]=0x80;  //首位
			PenDataY[0]=0x80;
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
					myfree(SRAMIN,PenDataX);
					myfree(SRAMIN,PenDataY);
					rmtmp=1;	
					ltmp=0; 
					tmp=0;
					counter = 0;
					PenBit = 0;
					i=1;
					LCD_ShowString(10,45,12,"memory",1);
					LCD_ShowString(10,30,12,"point",1);
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
						TP_Draw_Big_Point(tp_dev.x-5,tp_dev.y-3,POINT_COLOR);				//画用户图
						pointx[ltmp]=tp_dev.x>>1;pointy[ltmp]=tp_dev.y;
						ltmp++;
						if(ltmp==5)																							//滤值
						{
							if((pointx[2] - adjx >1)||(pointy[2] - adjy >1)||(adjx - pointx[2]>1)||(adjy - pointy[2]>1))
							{
								adjx = pointx[2],adjy = pointy[2],
								TP_Draw_Big_Point((adjx>>1)-5,adjy-3,WHITE);				//画取样图
								POINT_COLOR=colorTemp;
								ltmp=0;
								if(PenBit == 0)
								{
									PenDataX[i]=0xff;																//写状态
									PenDataY[i]=0x00;
									i++;
									counter = 0;																			//计时计数复位
								}
								PenDataX[i]=(adjx-5)>>1;																	//存一个XY数据
								PenDataY[i]=adjy-3;	
								i++;
								
								/*简单的溢出数据判断*/		
								if(i/100>=rmtmp)																	
									{
										PenDataX=(u8*)myrealloc(SRAMIN,PenDataX,(rmtmp+1)*100*sizeof(u8));
										PenDataY=(u8*)myrealloc(SRAMIN,PenDataY,(rmtmp+1)*100*sizeof(u8));
										rmtmp++;
										LCD_ShowNum(80,60,my_mem_perused(SRAMIN),3,12);     //显示内部SRAM使用量
									}			
							}
						}
				}
			
			}
		}
		else
		{
			delay_ms(10);	//没有按键按下的时候
			counter++;
			LCD_ShowNum(80,30,i,5,12);
			if(counter > 29 && (counter <= 300))
			{
				if(PenBit == 1)
				{
					PenDataX[i]=0x00;//提
					PenDataX[i]=0xff;
					i++;
					PenBit = 0;
				}
			}
		}
		
/*****************按键处理********************/		
		if(key==4)	//KEY_Wake_Up按下,则执行校准程序
		{

			LCD_Clear(WHITE);//清屏
		  TP_Adjust();  //屏幕校准 
			TP_Save_Adjdata();	 
			DrawTestPage("Please Draw In Here");
			myfree(SRAMIN,PenDataX);
			myfree(SRAMIN,PenDataY);
		}
		else if(key==1)
		{		
			LCD_Fill(60,100,260,150,WHITE);
			POINT_COLOR=RED;
			LCD_ShowString(85,120,12,"START WIRELESS TRANSMIT!",1);
			NRF24L01_TX_Mode();
			NRF24L01_TxPacket(&rmtmp);
			delay_ms(20);
			NRF24L01_RX_Mode();
//			while(NRF24L01_RxPacket(&rectmp));
			for(m=0;m<31;m++)
			{
				if(!NRF24L01_RxPacket(&rectmp))
					break;
				delay_ms(200);
			}
			LCD_Fill(60,100,260,150,WHITE);
			if(rectmp==0x56)
			{
				LCD_ShowString(100,120,16,"TRANSMIT SUCCESS!",1);
				/*********恢复初值**********/
				myfree(SRAMIN,PenDataX);
				myfree(SRAMIN,PenDataY);
				rmtmp=1;	
				ltmp=0; 
				tmp=0;
				counter = 0;
				PenBit = 0;
				i=1;
				delay_ms(2000);
				LCD_Clear(BLACK);
				break;
			}
			else 
			{
				LCD_ShowString(70,120,12,"NO lowerMCU ECHO! Please check!",1);
				delay_ms(500);
//				LCD_Clear(BLACK);
			}
		}
	}   
}




