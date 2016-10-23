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












#endif
