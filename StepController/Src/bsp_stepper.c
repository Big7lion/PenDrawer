#include "include.h"
#define lxpos  5
#define lypos  3
/*****************声明*********************/
#define SAVE_ADDR_BASE 40    //24C02地区基地址

uint8_t lastpos[2]={lxpos,lypos};			//位置数组
void Stepper_Save_pos(void);
void Stepper_Read_pos(void);
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
				YSET1 = 0;
				break;
			}
			case 2:
			{
				YSET0 = 1;
				YSET1 = 0;
				break;
			}
			case 4:
			{
				YSET0 = 0;
				YSET1 = 1;
				break;
			}				
			case 8:
			{
				YSET0 = 1;
				YSET1 = 1;
				break;
			}				
		}
	}
	else 
		return 0xff;
}


/****************保存最后坐标********************/
void Stepper_Save_pos(void)
{
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE,lastpos[0]);  
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE+1,lastpos[1]);
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE+2,0x0A);     //表示当前有存储
}

/****************读取EEPROM中的坐标********************/
void Stepper_Read_pos(void)
{
		if(AT24CXX_ReadOneByte(SAVE_ADDR_BASE+2)==0x0A)
		{
			lastpos[0] = AT24CXX_ReadOneByte(SAVE_ADDR_BASE);
			lastpos[1] = AT24CXX_ReadOneByte(SAVE_ADDR_BASE+1);
		}
		else
			USART_SendData("NO Data in 24C02!");
}

void Stepper_Get_pos(void)
{
		lastpos[0] = Stepper_Config.CurrentPosition&0xff;
		lastpos[1] = Stepper_Config.CurrentPosition>>8;
}




