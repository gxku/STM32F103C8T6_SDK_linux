#include "main.h"

#if 0
/* Definition for USARTy clock resources */
#define USARTy                           USART2
#define USARTy_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTy_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTy_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTy_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTy_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTy Pins */
#define USARTy_TX_PIN                    GPIO_PIN_2
#define USARTy_TX_GPIO_PORT              GPIOA
#define USARTy_RX_PIN                    GPIO_PIN_3
#define USARTy_RX_GPIO_PORT              GPIOA
int uart2_init(void);
void    uart2_print(char* str);
uint8_t uart2_receive(void);
uint8_t uart2_getch(void);
void uart2_putch(char ch);

#else
/* Definition for USARTy clock resources */
#define USARTy                           USART2
#define USARTy_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USARTy_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTy_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTy_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTy_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTy Pins */
#define USARTy_TX_PIN                    GPIO_PIN_2
#define USARTy_TX_GPIO_PORT              GPIOA
#define USARTy_RX_PIN                    GPIO_PIN_3 
#define USARTy_RX_GPIO_PORT              GPIOA

/* Definition for USARTy's NVIC */
#define USARTy_IRQn                      USART2_IRQn
#define USARTy_IRQHandler                USART2_IRQHandler


extern UART_HandleTypeDef UartHandle;

void    uart2_send(uint8_t* str,int len);

#endif
