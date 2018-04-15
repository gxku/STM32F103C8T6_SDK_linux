/**
 ******************************************************************************
 * @file    TIM/TIM_TimeBase/Src/main.c
 * @author  MCD Application Team
 * @version V1.4.0
 * @date    29-April-2016
 * @brief   This sample code shows how to use STM32F1xx TIM HAL API to generate
 *          a time base of one second with the corresponding Interrupt request.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#include "main.h"
#include "timer.h"
#include "led.h"

/** @addtogroup STM32F1xx_HAL_Examples
 * @{
 */

/** @addtogroup TIM_TimeBase
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TIM handle declaration */

/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;


/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
void timer_test(void)
{

    /* Configure LED2 */
    led_init();
    /*##-1- Configure the TIM peripheral #######################################*/
    /* -----------------------------------------------------------------------
       In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1) x2,
       since APB1 prescaler is set to 4 (0x100).
       TIM3CLK = PCLK1*2
       PCLK1   = HCLK/2
       => TIM3CLK = PCLK1*2 = (HCLK/2)*2 = HCLK = SystemCoreClock
       To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (SystemCoreClock /10 KHz) - 1

Note:
SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f1xx.c file.
Each time the core clock (HCLK) changes, user had to update SystemCoreClock
variable value. Otherwise, any configuration based on this variable will be incorrect.
This variable is updated in three ways:
1) by calling CMSIS function SystemCoreClockUpdate()
2) by calling HAL API function HAL_RCC_GetSysClockFreq()
3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
----------------------------------------------------------------------- */

    /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
    uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

    /* Set TIMx instance */
    TimHandle.Instance = TIMy;

    /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
     */
    TimHandle.Init.Period            = 10000 - 1;
    TimHandle.Init.Prescaler         = uwPrescalerValue;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;

    if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    {
        /* Starting Error */
        Error_Handler();
    }

    while (1)
    {
    }
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    led_toggle();
}

