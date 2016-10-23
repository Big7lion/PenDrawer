#include "include.h"
#define lxpos  5
#define lypos  3

static uint32_t lastpos[2]={lxpos,lypos};


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

