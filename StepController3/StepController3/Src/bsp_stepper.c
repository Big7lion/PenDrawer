#include "include.h"
/*****************声明*********************/

/*
	the size of A4 is 297mm*210mm;
	MODE1 Subdivid=0;
	X need about 23.2 pulse to a point;(7425 pulse)      
	Y need about 21.9 pulse to a point;(5250 pulse)
*/
#define BufferSize 16

uint8_t lastpos[2]={5,3};			//位置数组

uint16_t TIM2_Fre_Buffer_DEC[BufferSize] ={1060,1060,1700,1700,2300,2300,2900,2900,3500,3500,4000,4000,4500,4500,5000,5000};

uint16_t TIM2_Fre_Buffer_INC[BufferSize] ={5000,5000,4500,4500,4000,4000,3500,3500,2900,2900,2300,2300,1700,1700,1060,1060};

uint16_t TIM3_Fre_Buffer_DEC[BufferSize] ={1500,1500,2000,2000,2500,2500,3000,3000,3500,3500,4000,4000,4500,4500,5000,5000};

uint16_t TIM3_Fre_Buffer_INC[BufferSize] ={5000,5000,4500,4500,4000,4000,3500,3500,3000,3000,2500,2500,2000,2000,1500,1500};

uint16_t TIM2_Fre_DEC[BufferSize] ={1060,1060,1700,1700,2300,2300,2900,2900,3500,3500,4000,4000,4500,4500,5000,5000};

uint16_t TIM2_Fre_INC[BufferSize] ={5000,5000,4500,4500,4000,4000,3500,3500,2900,2900,2300,2300,1700,1700,1060,1060};

uint16_t TIM3_Fre_DEC[BufferSize] ={1500,1500,2000,2000,2500,2500,3000,3000,3500,3500,4000,4000,4500,4500,5000,5000};

uint16_t TIM3_Fre_INC[BufferSize] ={5000,5000,4500,4500,4000,4000,3500,3500,3000,3000,2500,2500,2000,2000,1500,1500};

void Stepper_Save_pos(uint8_t xlen,uint8_t ylen);
uint16_t Stepper_Read_eeprom_pos(void);

uint8_t Subdivid_Contral(char pass,uint8_t num);
uint8_t CircleCounter;

void Stepper_Draw_Point(uint8_t xpos,uint8_t ypos);
void Stepper_Pen_Press(void);
//void Stepper_Point_Planner(uint16_t TargetPos,uint8_t xpos,uint8_t ypos);
//uint16_t lxpos = Stepper_Config.CurrentPosition;
Config_InitStruct  Stepper_Config;
/****************细分控制设定********************/
//输入：pass：通道'X' or 'Y'      num：细分数[0，2，4，8]
uint8_t Subdivid_Contral(char pass,uint8_t num)             //驱动器细分控制
{
		if(pass == 'X')
	{
		switch(num)
		{
			case 0:
			{	
				XSET0 = 0;
				XSET1 = 0;
				break;
			}
			case 2:
			{
				XSET0 = 1;
				XSET1 = 0;
				break;
			}
			case 4:
			{
				XSET0 = 0;
				XSET1 = 1;
				break;
			}				
			case 8:
			{
				XSET0 = 1;
				XSET1 = 1;
				break;
			}			
		}
	}
	else if(pass == 'Y')
	{
		switch(num)
		{
			case 0:
			{	
				YSET0 = 0;
				break;
			}
			case 2:
			{
				YSET0 = 1;
				break;
			}
			}				
		}

	else 
		return 0xff;
}

/********************电机数据值预设************************/
void bsp_stepper_init(void)
{
	Stepper_Config.CurrentPosition = 0;
	Stepper_Config.targetPosition = 240;
	Stepper_Config.XmaxStepPrescaller = 7200;
	Stepper_Config.XminStepPrescaller = 720;
	Stepper_Config.YmaxStepPrescaller = 7200;
	Stepper_Config.YminStepPrescaller = 720;
	Stepper_Config.Zstatus = 0;
}


/****************保存最后坐标********************/
void Stepper_Save_pos(uint8_t xpos,uint8_t ypos)
{
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE,xpos);  
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE+1,ypos);
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE+2,0x0A);     //表示当前有存储
}

/****************读取EEPROM中的坐标********************/
uint16_t Stepper_Read_eeprom_pos(void)
{
		uint8_t lastpos[2];
		if(AT24CXX_ReadOneByte(SAVE_ADDR_BASE+2)==0x0A)
		{
			lastpos[0] = AT24CXX_ReadOneByte(SAVE_ADDR_BASE);								//xpos
			lastpos[1] = AT24CXX_ReadOneByte(SAVE_ADDR_BASE+1);             //ypos
			return ((uint16_t)lastpos[1]<<8)&lastpos[0];   //返回高八位为y坐标，低八位为x坐标的16位数据
		}
		else
		{
			USART_SendData("NO Data in 24C02!");
			return 0;
		}
}

void Stepper_Get_pos(void)
{
		lastpos[1] = Stepper_Config.CurrentPosition&0xff;
		lastpos[0] = Stepper_Config.CurrentPosition>>8;
}

void Stepper_Draw_Point(uint8_t xlen,uint8_t ylen)
{
	/********两轴速度计算********/
	xlen = (uint16_t)(xlen)<<1;
	uint8_t sig;
	float Multiple;
	/******倍率******/
			if(xlen>ylen)
			{
				Multiple = xlen/(ylen+0.1);                 //sig=1，X轴距离长
				sig=1;
			}
			else
			{
				Multiple = ylen/(xlen+0.1);                 //sig=2，Y轴距离长
				sig=2;
			}
		/******控制细分******/
		if((sig==2&&Multiple<=32)||(sig==1&&Multiple<=8))
		{
			if(Multiple >= 2)
			{
				if(Multiple >= 4)
				{	
					if(Multiple >= 8)
						{if(sig == 2){Subdivid_Contral('X',8);Multiple = Multiple/8;}}
					else 
						{if(sig == 2){Subdivid_Contral('X',4);Multiple = Multiple/4;}}
				}
				else
				{
					if(sig == 1)Subdivid_Contral('Y',2);
					else if(sig == 2)Subdivid_Contral('X',2);
					Multiple = Multiple/2;
				}
				
			/**********分轴速度控制**********/
				if(sig == 1)
				{
						for(CircleCounter = 15;CircleCounter>9;CircleCounter--)
					{
						TIM3_Fre_Buffer_INC[CircleCounter]=TIM3_Fre_INC[CircleCounter]/Multiple;	
						TIM3_Fre_Buffer_DEC[15-CircleCounter]=TIM3_Fre_DEC[15-CircleCounter]/Multiple;
					}
				}
				else
				{
					for(CircleCounter = 15;CircleCounter>9;CircleCounter--)
					{
						TIM3_Fre_Buffer_INC[CircleCounter]=TIM3_Fre_INC[CircleCounter]*Multiple;	
						TIM3_Fre_Buffer_DEC[15-CircleCounter]=TIM3_Fre_DEC[15-CircleCounter]*Multiple;
					}
				}
			}
		}
		else
		{
			if(sig == 1)
			{
				
			}
			else if(sig ==2)
			{
				
			}
		}

	
	/********电机驱动开始********/
	HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)TIM2_Fre_Buffer_INC,(uint32_t)TIM2_ARR_Address,16);
	HAL_DMA_Start(&hdma_tim3_ch4_up,(uint32_t)TIM3_Fre_Buffer_INC,(uint32_t)TIM3_ARR_Address,16);
	TIM7_Rebuild(ylen-1);
}

void Stepper_Pen_Press(void)
{
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim6);
}





