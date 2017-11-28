/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
//#include "stm32f1xx_nucleo.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA


/* Definition for USARTy clock resources */
#define USARTy                           USART2
#define USARTy_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTy_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTy_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTy_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTy_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTy_TX_PIN                    GPIO_PIN_2
#define USARTy_TX_GPIO_PORT              GPIOA
#define USARTy_RX_PIN                    GPIO_PIN_3
#define USARTy_RX_GPIO_PORT              GPIOA

//////////////////////////////led
#define LED_PIN                           GPIO_PIN_13
#define LED_GPIO_PORT                     GPIOC
#define LED_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOC_CLK_DISABLE()
  
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern void print(char* fmt, ...);
extern void Error_Handler(char *str);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
