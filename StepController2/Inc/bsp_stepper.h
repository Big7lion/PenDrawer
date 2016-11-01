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










#endif
