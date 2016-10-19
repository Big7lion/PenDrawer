#include "lcd.h"
void LED_GPIO_Config(void)    
	{           
	GPIO_InitTypeDef GPIO_InitStructure;                                                               
	GPIO_InitStructure.GPIO_Pin = 0x00FF;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOA,0x00FF);	
	GPIO_InitStructure.GPIO_Pin = 0xF000; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB,0xF000);	
	} 
	
	
	void WriteCMD(uchar cmd)
{
	GPIO_ResetBits(GPIOB, LCD_CS); 	           //LCD_CS=0;
	GPIO_ResetBits(GPIOB, LCD_RS);             //LCD_RS=0;
  GPIOA->ODR=(GPIOB->ODR&0XFF00)|cmd;        //LCD_IO=cmd;
	GPIO_ResetBits(GPIOB, LCD_WR);             //LCD_WR=0;
	GPIO_SetBits(GPIOB, LCD_WR);               //LCD_WR=1;
	GPIO_SetBits(GPIOB, LCD_CS);               //LCD_CS=1;
}    

void WriteDAT(uchar dat)
{
	GPIO_ResetBits(GPIOB, LCD_CS);             //	LCD_CS=0;
	GPIO_SetBits(GPIOB, LCD_RS);               //	LCD_RS=1;
	GPIOA->ODR=(GPIOB->ODR&0XFF00)|dat;        //	LCD_IO= dat;
	GPIO_ResetBits(GPIOB, LCD_WR);             //	LCD_WR=0;
	GPIO_SetBits(GPIOB, LCD_WR);               //	LCD_WR=1;
	GPIO_SetBits(GPIOB, LCD_CS);               //	LCD_CS=1;
}    

/***********************************************************
                        «Â∆¡
************************************************************/
void clear(uchar charbkcolor)  
{    
	uint  i;    uchar  j;  
	WriteCMD(0x00); 
	WriteDAT(0x00);    
	WriteCMD(0x01); 
	WriteDAT(0x00);  
	WriteCMD(0x02);   
		for(i=0;i<600;i++){      
		for(j=0;j<100;j++){        
			WriteDAT(charbkcolor);    	WriteDAT(charbkcolor);   
 			WriteDAT(charbkcolor);    	WriteDAT(charbkcolor);     
			WriteDAT(charbkcolor);   	WriteDAT(charbkcolor);     
			WriteDAT(charbkcolor);    	WriteDAT(charbkcolor);     
                           }  
                           }
} 



/***********************************************************
                       ª≠µ„
************************************************************/
void putpixel(int x,int y, uchar color) 
{    
   uchar a16;    
   ulong p;  
   p = (ulong)y*SCREEN_WIDE+(ulong)x;    
   a16 = (uchar)(p>>16);    
   a16 <<= 2;    
   WriteCMD( 0x00|a16);    
   WriteDAT((uchar)p);    
   WriteCMD( 0x01|a16);   
   WriteDAT( (uchar)(p>>8));    
   WriteCMD( 0x02|a16);
   WriteDAT(color);
 } 
/***********************************************************
                       ª≠œﬂ
************************************************************/
void DDALine(int x0,int y0,int x1,int y1,int color) 
{ 
   int x; 
   float dx, dy, y, k; 
   dx = x1-x0;dy=y1-y0; 
   k=dy/dx;
   y=y0; 
   for (x=x0;x< x1;x++){ 
      putpixel (x+400,(int)(y+0.5)+300, color); 
      y=y+k; 
   } 
}
