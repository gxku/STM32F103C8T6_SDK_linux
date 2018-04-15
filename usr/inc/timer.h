#ifndef __TIM_H
#define __TIM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
   /* Definition for TIMx clock resources */
   #define TIMy                           TIM3
   #define TIMy_CLK_ENABLE()              __HAL_RCC_TIM3_CLK_ENABLE()


   /* Definition for TIMx's NVIC */
   #define TIMy_IRQn                      TIM3_IRQn
   #define TIMy_IRQHandler                TIM3_IRQHandler

    TIM_HandleTypeDef    TimHandle;
extern    void timer_test(void);

#endif /* __MAIN_H */
