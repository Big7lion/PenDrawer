#ifndef __BSP_STEPPER_H__
#define __BSP_STEPPER_H__
#include "include.h"

/**************ʹ������*************/
#define X_enable() X_EN = 0
#define X_disable() X_EN = 1
#define Y_enable() Y_EN = 0
#define Y_disable() Y_EN = 1

/**************�������**************/
#define Xforwarddirection   0
#define Yforwarddirection   0
#define Zforwarddirection   0
#define X_Fdir()	DIRX = Xforwarddirection
#define X_Bdir()  DIRX = !Xforwarddirection
#define Y_Fdir()	DIRY = Yforwarddirection
#define Y_Bdir()  DIRY = !Yforwarddirection
#define Z_Fdir()	DIRZ = Zforwarddirection
#define Z_Bdir()  DIRZ = !Zforwarddirection


/******************����������ýṹ��****************/
typedef struct
{
	volatile uint32_t 	XminStepPrescaller;            //ʱ����С��Ƶ�����Ť�أ�
	volatile uint32_t 	XmaxStepPrescaller;						//����Ƶ��������ٶȣ�
	volatile uint32_t 	YminStepPrescaller;            //ʱ����С��Ƶ�����Ť�أ�
	volatile uint32_t 	YmaxStepPrescaller;						//����Ƶ��������ٶȣ�
	volatile uint16_t 	CurrentPosition;							//��ǰλ��
	volatile uint16_t 	targetPosition;								//Ŀ��λ�ã��߰�λX���꣬�Ͱ�λY���꣩
	volatile uint8_t    Zstatus;											//Z��״̬��1Ϊ���£�0Ϊ����
}Config_InitStruct;


#define SAVE_ADDR_BASE 40    //24C02��������ַ
#define TIM2_ARR_Address    0x4000002C   //tim2  arr��ַ
#define TIM3_ARR_Address    0x4000042C   //tim3  arr��ַ



void Stepper_Save_pos(uint8_t xpos,uint8_t ypos);
uint16_t Stepper_Read_eeprom_pos(void);
uint8_t Subdivid_Contral(char pass,uint8_t num);
void Stepper_Draw_Point(uint8_t xlen,uint8_t ylen);
void Stepper_Pen_Press(void);

												 

#endif
