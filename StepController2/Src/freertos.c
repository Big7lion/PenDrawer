/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "include.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osMessageQId PosDataHandle;

/* USER CODE BEGIN Variables */
uint8_t Messagesign = 0;                      //消息队列标志
uint8_t sendtmp = 0x56;                    //发送成功标志
uint16_t *PenData = NULL;
uint8_t DataLength;                       //接收数组长度
static SemaphoreHandle_t  xMutex = NULL;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StepControltask(void const * argument);
void NRFTask(void const * argument);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void USART_SendData(void const * argument, ...);
void AppObjCreate (void);
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 64);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StepControltask, osPriorityIdle, 0, 160);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, NRFTask, osPriorityIdle, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of PosData */
  osMessageQDef(PosData, 16, uint16_t);
  PosDataHandle = osMessageCreate(osMessageQ(PosData), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* StepControltask function */
/* Process --- 1:Check the Message queues sign and receive
*/
void StepControltask(void const * argument)
{
  /* USER CODE BEGIN StepControltask */
  /* Infinite loop */
  for(;;)
  {
		if(Messagesign == 1)
    {
//      xQueueReceive(PosData,PenData,100);
    }
    osDelay(1);
  }
  /* USER CODE END StepControltask */
}

/* NRFTask function */
/* Process --- 1：Rx the length of data   2：malloc the mem
               3：check the first data    4：Send the receive success sign
               5: Put Message into queues     */
void NRFTask(void const * argument)
{
  /* USER CODE BEGIN NRFTask */
  /* Infinite loop */
  for(;;)
  {
		RX_Mode();
		if(NRF24L01_RxPacket(&DataLength)== 0)
		{
			AppObjCreate();              //创建互斥信号量
			PenData=(u16*)myrealloc(SRAMIN,PenData,(uint8_t)DataLength*100*sizeof(u16));
			HAL_Delay(20);
			if(NRF24L01_RxPacket(PenData) == 0)
      {
        if(PenData[0] == 0x80)
        {
//          osMessagePut(PosData,PenData,0);
          TX_Mode();
          NRF24L01_TxPacket(&sendtmp);
          Messagesign = 1;
        }
        else
        {
          USART_SendString("DATA Receive error!");
          Messagesign = 0;
        }
      }	

			xSemaphoreGive(xMutex);      //释放信号量
		}
    osDelay(10);
  }
  /* USER CODE END NRFTask */
}

/* USER CODE BEGIN Application */
void AppObjCreate (void)
{
		/* 创建互斥信号量 */
    xMutex = xSemaphoreCreateMutex();
	
	if(xMutex == NULL)
    {
      /* 没有创建成功，在这里加入创建失败的处理机制 */
			USART_SendData("xMutex Create Failed!\r\n\0");
    }
}


/* USART_SendData function */
void USART_SendData(void const * argument, ...)
{
  /* USER CODE BEGIN USART_SendData */
  /* Infinite loop */
    char  buf_str[200];
	
		sprintf(buf_str,"%s\r\n",argument);
	
		xSemaphoreTake(xMutex, portMAX_DELAY);

    HAL_UART_Transmit(&huart1,(uint8_t*)buf_str,strlen(buf_str),300);

   	xSemaphoreGive(xMutex);
  /* USER CODE END USART_SendData */
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
