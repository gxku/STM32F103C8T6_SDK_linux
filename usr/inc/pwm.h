#ifndef __PWM_H
#define __PWM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM2
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL1        GPIOA
#define TIMx_GPIO_PORT_CHANNEL2        GPIOA
#define TIMx_GPIO_PORT_CHANNEL3        GPIOA
#define TIMx_GPIO_PORT_CHANNEL4        GPIOA
#define TIMx_GPIO_PIN_CHANNEL1         GPIO_PIN_0
#define TIMx_GPIO_PIN_CHANNEL2         GPIO_PIN_1
#define TIMx_GPIO_PIN_CHANNEL3         GPIO_PIN_2    
#define TIMx_GPIO_PIN_CHANNEL4         GPIO_PIN_3
#define TIMx_GPIO_AF_CHANNEL1          /
#define TIMx_GPIO_AF_CHANNEL2          /
#define TIMx_GPIO_AF_CHANNEL3          /
#define TIMx_GPIO_AF_CHANNEL4          /
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void  pwm_main(void);

#endif /* __MAIN_H */
