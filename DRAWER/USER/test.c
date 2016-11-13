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

//NRF24L01�����ͼ������Ϊ0x55;���ռ������Ϊ0x56; ֡������Ϊ0x80;


///////////////////////////////////////////////////////////////////////////////////*/
//========================variable==========================//
u16 ColorTab[5]={BRED,YELLOW,RED,GREEN,BLUE};//������ɫ����
u8 *PenDataX=NULL;
u8 *PenDataY=NULL;
//=====================end of variable======================//

//******************************************************************
//��������  DrawTestPage
//���ܣ�    ���Ʋ��Խ���
//���������str :�ַ���ָ��
//����ֵ��  ��
//******************************************************************
void DrawTestPage(u8 *str)
{
//���ƹ̶���up
LCD_Fill(0,0,lcddev.width,20,BLUE);
//���ƹ̶���down
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"BIG LION",16,1);//������ʾ
//���Ʋ�������
LCD_Fill(0,20,lcddev.width,lcddev.height-20,BLACK);
}




//******************************************************************
//��������  Test_FillRec
//���ܣ�    ���ο���ʾ�������ԣ�������ʾ�ۺ�ɫ����ɫ����ɫ����ɫ����ɫ���ο�
//       	��ʱ1500��������ΰ��շۺ�ɫ����ɫ����ɫ����ɫ����ɫ�����ο� 
//�����������
//����ֵ��  ��
//******************************************************************
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("����2:GUI����������");
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
//��������  Test_FillRec
//���ܣ�    Բ�ο���ʾ�������ԣ�������ʾ�ۺ�ɫ����ɫ����ɫ����ɫ����ɫԲ�ο�
//       	��ʱ1500��������ΰ��շۺ�ɫ����ɫ����ɫ����ɫ����ɫ���Բ�ο� 
//�����������
//����ֵ��  ��
//******************************************************************
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("����3:GUI��Բ������");
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
//��������  Touch_Request
//���ܣ�    ���ڴ�����д�����޸ĵı���Ŀ����
//�����������
//����ֵ��  ��
//******************************************************************
void Touch_Request(void)
{
	u8 key;
	u8 rmtmp=1;					//�ڴ������С����
	u8 tmp = 0;					//�ڴ������־λ
	u8 ltmp = 0;        //���ȡ���˲�����
	u8 rectmp;					//Ӧ������
	u8 counter = 0;     //��״̬��ʱ����
	u8 PenBit = 0;			//��״̬��־λ
	u8 m=0;
	vu8 pointx[5],pointy[5];
	u16 j=0;
	u16 colorTemp=RED;		//��ɫ�л�����
	vu16 adjx=0,adjy=0;  //���������ݴ滺��
	u32 i=1;					//��Ļ�������
	TP_Init();
	KEY_Init();
//	TP_Adjust();  		//ǿ��ִ��һ����ĻУ׼ (������û��IIC�洢�����������û�)

	DrawTestPage("Please Draw In Here");
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
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
			PenDataX = (u8*)mymalloc(SRAMIN,100*sizeof(u8));     //�ڴ�����
			PenDataX[0]=0x80;  //��λ
			PenDataY[0]=0x80;
			tmp=1;
		}
/*******************��ʾ����************************/
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
		 	if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
			{	
				if(tp_dev.x>(lcddev.width-24)&&tp_dev.y<16)
				{
					DrawTestPage("Please Draw In Here");//���
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
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

				
/********************��������******************/				
				else
				{
						TP_Draw_Big_Point(tp_dev.x-5,tp_dev.y-3,POINT_COLOR);				//���û�ͼ
						pointx[ltmp]=tp_dev.x>>1;pointy[ltmp]=tp_dev.y;
						ltmp++;
						if(ltmp==5)																							//��ֵ
						{
							if((pointx[2] - adjx >1)||(pointy[2] - adjy >1)||(adjx - pointx[2]>1)||(adjy - pointy[2]>1))
							{
								adjx = pointx[2],adjy = pointy[2],
								TP_Draw_Big_Point((adjx>>1)-5,adjy-3,WHITE);				//��ȡ��ͼ
								POINT_COLOR=colorTemp;
								ltmp=0;
								if(PenBit == 0)
								{
									PenDataX[i]=0xff;																//д״̬
									PenDataY[i]=0x00;
									i++;
									counter = 0;																			//��ʱ������λ
								}
								PenDataX[i]=(adjx-5)>>1;																	//��һ��XY����
								PenDataY[i]=adjy-3;	
								i++;
								
								/*�򵥵���������ж�*/		
								if(i/100>=rmtmp)																	
									{
										PenDataX=(u8*)myrealloc(SRAMIN,PenDataX,(rmtmp+1)*100*sizeof(u8));
										PenDataY=(u8*)myrealloc(SRAMIN,PenDataY,(rmtmp+1)*100*sizeof(u8));
										rmtmp++;
										LCD_ShowNum(80,60,my_mem_perused(SRAMIN),3,12);     //��ʾ�ڲ�SRAMʹ����
									}			
							}
						}
				}
			
			}
		}
		else
		{
			delay_ms(10);	//û�а������µ�ʱ��
			counter++;
			LCD_ShowNum(80,30,i,5,12);
			if(counter > 29 && (counter <= 300))
			{
				if(PenBit == 1)
				{
					PenDataX[i]=0x00;//��
					PenDataX[i]=0xff;
					i++;
					PenBit = 0;
				}
			}
		}
		
/*****************��������********************/		
		if(key==4)	//KEY_Wake_Up����,��ִ��У׼����
		{

			LCD_Clear(WHITE);//����
		  TP_Adjust();  //��ĻУ׼ 
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
				/*********�ָ���ֵ**********/
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




