#include "main.h"

/* Definition for USARTy clock resources */
#define USARTy                           USART2
#define USARTy_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define DMAy_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTy_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTy_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTy_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTy_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTy Pins */
#define USARTy_TX_PIN                    GPIO_PIN_2
#define USARTy_TX_GPIO_PORT              GPIOA
#define USARTy_RX_PIN                    GPIO_PIN_3 
#define USARTy_RX_GPIO_PORT              GPIOA

/* Definition for USARTx's DMA */
#define USARTy_TX_DMA_CHANNEL             DMA1_Channel6
#define USARTy_RX_DMA_CHANNEL             DMA1_Channel7


/* Definition for USARTx's NVIC */
#define USARTy_DMA_TX_IRQn                DMA1_Channel6_IRQn
#define USARTy_DMA_RX_IRQn                DMA1_Channel7_IRQn
#define USARTy_DMA_TX_IRQHandler          DMA1_Channel6_IRQHandler
#define USARTy_DMA_RX_IRQHandler          DMA1_Channel7_IRQHandler

/* Definition for USARTy's NVIC */
#define USARTy_IRQn                      USART2_IRQn
#define USARTy_IRQHandler                USART2_IRQHandler


extern UART_HandleTypeDef UartHandle;
extern DMA_HandleTypeDef hdma_tx;
extern DMA_HandleTypeDef hdma_rx;

int uart2_init(void);
void uart2_putch(uint8_t ch);
void uart2_send(uint8_t* str,int len);
void uart2_test();

