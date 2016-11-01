/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define DIRX_Pin GPIO_PIN_0
#define DIRX_GPIO_Port GPIOA
#define TIM2_CH2_STEPX_Pin GPIO_PIN_1
#define TIM2_CH2_STEPX_GPIO_Port GPIOA
#define XSET1_Pin GPIO_PIN_2
#define XSET1_GPIO_Port GPIOA
#define XSET0_Pin GPIO_PIN_3
#define XSET0_GPIO_Port GPIOA
#define X_EN_Pin GPIO_PIN_4
#define X_EN_GPIO_Port GPIOA
#define Y_EN_Pin GPIO_PIN_5
#define Y_EN_GPIO_Port GPIOA
#define YSET0_Pin GPIO_PIN_6
#define YSET0_GPIO_Port GPIOA
#define DIRY_Pin GPIO_PIN_7
#define DIRY_GPIO_Port GPIOA
#define TIM3_CH3_STEPY_Pin GPIO_PIN_0
#define TIM3_CH3_STEPY_GPIO_Port GPIOB
#define TIM3_CH4_STEPZ_Pin GPIO_PIN_1
#define TIM3_CH4_STEPZ_GPIO_Port GPIOB
#define DIRZ_Pin GPIO_PIN_10
#define DIRZ_GPIO_Port GPIOB
#define NRF_CSN_Pin GPIO_PIN_12
#define NRF_CSN_GPIO_Port GPIOB
#define NRF_SCK_Pin GPIO_PIN_13
#define NRF_SCK_GPIO_Port GPIOB
#define NRF_MOSI_Pin GPIO_PIN_14
#define NRF_MOSI_GPIO_Port GPIOB
#define NRF_MISO_Pin GPIO_PIN_15
#define NRF_MISO_GPIO_Port GPIOB
#define NRF_CE_Pin GPIO_PIN_8
#define NRF_CE_GPIO_Port GPIOA
#define NRF_IRQ_Pin GPIO_PIN_11
#define NRF_IRQ_GPIO_Port GPIOA
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/