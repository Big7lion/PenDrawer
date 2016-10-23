#include "include.h"
#define lxpos  5
#define lypos  3
/*****************����*********************/
#define SAVE_ADDR_BASE 40    //24C02��������ַ

static uint32_t lastpos[2]={lxpos,lypos};			//λ������
void Stepper_Save_pos(void);
void Stepper_Read_pos(void);
uint8_t Subdivid_Contral(char pass,uint8_t num);


/****************ϸ�ֿ����趨********************/
//���룺pass��ͨ��'X' or 'Y'      num��ϸ����[0��2��4��8]
uint8_t Subdivid_Contral(char pass,uint8_t num)             //������ϸ�ֿ���
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


/****************�����������********************/
void Stepper_Save_pos(void)
{
	

		AT24CXX_WriteOneByte(SAVE_ADDR_BASE,lastpos[0]);  
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE+1,lastpos[1]);
		AT24CXX_WriteOneByte(SAVE_ADDR_BASE+2,0x0A);     //��ʾ��ǰ�д洢
}

/****************��ȡEEPROM�е�����********************/
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





