#include "include.h"
#define lxpos  5
#define lypos  3
/*****************声明*********************/
#define SAVE_ADDR_BASE 40    //24C02地区基地址

uint8_t lastpos[2]={lxpos,lypos};			//位置数组
void Stepper_Save_pos(uint8_t xpos,uint8_t ypos);
uint16_t Stepper_Read_pos(void);
uint8_t Subdivid_Contral(char pass,uint8_t num);
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
uint16_t Stepper_Read_pos(void)
{
		if(AT24CXX_ReadOneByte(SAVE_ADDR_BASE+2)==0x0A)
		{
			lastpos[0] = AT24CXX_ReadOneByte(SAVE_ADDR_BASE);
			lastpos[1] = AT24CXX_ReadOneByte(SAVE_ADDR_BASE+1);
			return ((uint16_t)lastpos[0]<<8)&lastpos[1];   //返回高八位为X坐标，低八位为Y坐标的16位数据
		}
		else
			USART_SendData("NO Data in 24C02!");
}

void Stepper_Get_pos(void)
{
		lastpos[0] = Stepper_Config.CurrentPosition&0xff;
		lastpos[1] = Stepper_Config.CurrentPosition>>8;
}




