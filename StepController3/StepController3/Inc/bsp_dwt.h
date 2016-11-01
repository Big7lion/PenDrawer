#ifndef  __BSP_DWT_H__
#define  __BSP_DWT_H__

#include "FreeRTOS.h"
#include "task.h"

void bsp_DelayMS(uint32_t _ulDelayTime);
void bsp_DelayUS(uint32_t _ulDelayTime);
void bsp_InitDWT(void);

#endif
