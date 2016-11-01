#ifndef __BSP_STEPPER_H__
#define __BSP_STEPPER_H__
#include "include.h"

/**************使能设置*************/
#define X_enable() X_EN = 0
#define X_disable() X_EN = 1
#define Y_enable() Y_EN = 0
#define Y_disable() Y_EN = 1

/**************方向控制**************/
#define Xforwarddirection   0
#define Yforwarddirection   0
#define Zforwarddirection   0
#define X_Fdir()	DIRX = Xforwarddirection
#define X_Bdir()  DIRX = !Xforwarddirection
#define Y_Fdir()	DIRY = Yforwarddirection
#define Y_Bdir()  DIRY = !Yforwarddirection
#define Z_Fdir()	DIRZ = Zforwarddirection
#define Z_Bdir()  DIRZ = !Zforwarddirection


/******************步进电机设置结构体****************/
typedef struct
{
	volatile uint32_t 	XminStepPrescaller;            //时钟最小分频（最大扭矩）
	volatile uint32_t 	XmaxStepPrescaller;						//最大分频数（最大速度）
	volatile uint32_t 	YminStepPrescaller;            //时钟最小分频（最大扭矩）
	volatile uint32_t 	YmaxStepPrescaller;						//最大分频数（最大速度）
	volatile uint16_t 	CurrentPosition;							//当前位置
	volatile uint16_t 	targetPosition;								//目标位置（高八位X坐标，低八位Y坐标）
	volatile uint8_t    Zstatus;											//Z轴状态（1为按下，0为提起）
}Config_InitStruct;










#endif
