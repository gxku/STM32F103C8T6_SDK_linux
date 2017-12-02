#include "main.h"
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
int uart2_init(void);
void    uart2_print(char* str);
uint8_t uart2_receive(void);
uint8_t uart2_getch(void);
void uart2_putch(char ch);
