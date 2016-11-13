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
osThreadId Task01Handle;
osThreadId Task02Handle;

/* USER CODE BEGIN Variables */
static SemaphoreHandle_t  xMutex = NULL;
uint8_t Messagesign = 0;                  //��Ϣ���б�־
uint8_t StepProcess = 1;									//�滭���Ʊ�־
uint8_t sendtmp = 0x56;                   //���ͳɹ���־
uint8_t DataLength;                       //�������鳤��
uint8_t xPostmp;                          //8λλ������
uint8_t yPostmp;                          //8λλ������
uint16_t Postmp;                          //16λλ������

uint8_t *PenDataX = NULL;
uint8_t *PenDataY = NULL;


/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void NRFTask(void const * argument);
void StepperTask02(void const * argument);

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
  /* definition and creation of Task01 */
  osThreadDef(Task01, NRFTask, osPriorityNormal, 0, 160);
  Task01Handle = osThreadCreate(osThread(Task01), NULL);

  /* definition and creation of Task02 */
  osThreadDef(Task02, StepperTask02, osPriorityIdle, 0, 180);
  Task02Handle = osThreadCreate(osThread(Task02), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* NRFTask function */
void NRFTask(void const * argument)
{

  /* USER CODE BEGIN NRFTask */
  /* Infinite loop */
  for(;;)
  {
		RX_Mode();
		if(NRF24L01_RxPacket(&DataLength)== 0)
		{
			AppObjCreate();              //���������ź���
			PenDataX=(uint8_t *)mymalloc(SRAMIN,((uint8_t)DataLength)*100*sizeof(uint8_t));
			PenDataY=(uint8_t *)mymalloc(SRAMIN,((uint8_t)DataLength)*100*sizeof(uint8_t));
			HAL_Delay(20);
			if(NRF24L01_RxPacket(PenDataX) == 0)
      {
				RX_Mode();
				if(NRF24L01_RxPacket(PenDataY) == 0)
				{
					if(PenDataX[0] == 0x80&&PenDataY[0] == 0x80)
					{
						TX_Mode();
						NRF24L01_TxPacket(&sendtmp);
						Messagesign = 1;
					}
					else
					{
						USART_SendData("Data Received error!");
						Messagesign = 0;
					}
				}
				else USART_SendData("Data haven't received compeletly!");
      }
			xSemaphoreGive(xMutex);      //�ͷ��ź���
		}           
    osDelay(5);
  }
  /* USER CODE END NRFTask */
}

/* StepperTask02 function */
void StepperTask02(void const * argument)
{
  /* USER CODE BEGIN StepperTask02 */
  /* Infinite loop */
  for(;;)
  {
		if(Messagesign == 1)
    {
      AppObjCreate();              //���������ź���
			USART_SendData("Start Drawing!");
      Postmp = Stepper_Read_eeprom_pos();
      if(Postmp != 0)
      {
        xPostmp = Postmp&0xff;     //����ǰλ��ֵ
        yPostmp = Postmp>>8;
      }
			
			X_enable();                  //�������ʹ��
			Y_enable();
			
			volatile uint8_t xTargetPos,yTargetPos,xbit,ybit,zstatus=0,i=1;
	//ѭ��������check��		
			for(uint8_t step = 1;(step<=DataLength*100*sizeof(uint8_t));step++)
      {
				if(PenDataX[step]==0xff&&PenDataY[step]==0x00)                    //Z��״̬����
				{																							//д
					Z_Bdir();
					Stepper_Pen_Press();
					step++;
				}
				else if(PenDataX[step]==0x00&&PenDataX[step]==0xff)								//��
				{
					Z_Fdir();
					Stepper_Pen_Press();
					step++;
				}
				
				else if(PenDataX[step]!=0&&PenDataX[step]!=0)																			//�켣����
				{
						xTargetPos = PenDataX[step]; 
						yTargetPos = PenDataY[step];
						if(xTargetPos > xPostmp)
							{X_Fdir();xbit = xTargetPos - xPostmp;}
						else if(xTargetPos < xPostmp)
							{X_Bdir();xbit = xPostmp - xTargetPos;}
						if(yTargetPos > yPostmp)
							{Y_Fdir();ybit = yTargetPos - yPostmp;}
						else if(yTargetPos < yPostmp)
							{Y_Bdir();ybit = yPostmp - yTargetPos;}	
							
						Stepper_Draw_Point(xbit,ybit);     //����˶�����
						xPostmp = xTargetPos;
						yPostmp = yTargetPos;
						while(StepProcess);
						if(i/5 == 1)
						{
							Stepper_Save_pos(xPostmp,yPostmp);
							i = 0;
						}
						else	i++;
				}
      }
			X_disable();                 //���ʧ��
      Y_disable();
			myfree(SRAMIN,PenDataX);
			myfree(SRAMIN,PenDataY);
			xSemaphoreGive(xMutex);      //�ͷ��ź���
    }
    Messagesign = 0;
    osDelay(5);
  }
  /* USER CODE END StepperTask02 */
}

/* USER CODE BEGIN Application */
void AppObjCreate (void)
{
		/* ���������ź��� */
    xMutex = xSemaphoreCreateMutex();
	
	if(xMutex == NULL)
    {
      /* û�д����ɹ�����������봴��ʧ�ܵĴ������ */
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
