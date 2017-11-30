
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include "print.h"
#include "main.h"

/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_Printf
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int print_init(void)
{

  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler("init console failed");
  }


}

uint8_t uart_getchar(void)
{
    uint8_t v;
    
    HAL_UART_Receive(&UartHandle, &v, 1, 1000000);

    return v;
}


void uart_putchar(uint8_t v)
{
    HAL_UART_Transmit(&UartHandle, &v, 1, 5000);
}

void print(char* fmt,...)
{
#ifdef PRINT
    char* pStr = 0;
    char consoleBuf[CONFIG_CONSOLE_BUF_LEN] = {0,};
    
    va_list arglist;
    va_start(arglist, fmt);
    vsprintf(consoleBuf, fmt, arglist);
    
    pStr = consoleBuf;
    
    while ('\0' != *pStr)
    {
        if ('\n' == *pStr)
        {
            uart_putchar('\r');
        }
        uart_putchar(*pStr);
        
        pStr++;
    } /* while */
    
    va_end(arglist);
#endif
}
