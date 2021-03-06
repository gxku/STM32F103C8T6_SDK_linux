
/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#define RXBUFFERSIZE 512
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
DMA_HandleTypeDef hdma_tx;
DMA_HandleTypeDef hdma_rx;
__IO ITStatus UartReady = RESET;
/* Private function prototypes -----------------------------------------------*/
/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****UART_TwoBoards communication based on DMA****  ****UART_TwoBoards communication based on DMA****  ****UART_TwoBoards communication based on DMA**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
/* Private functions ---------------------------------------------------------*/
void uart2_test();



void Error_Handler_uart2(){

  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler_uart2();
  }
}
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int uart2_init(void)
{

  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTy;

  UartHandle.Init.BaudRate   = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }

  return 0;

}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

void uart2_putch(uint8_t ch)
{
    int ret;
  aTxBuffer[0]=ch;
  /* Place your implementation of fputc here */
    do
     {
         ret = HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)aTxBuffer, 1);//请求发送下一个数据  
     }while(ret != HAL_OK);


}

void    uart2_send(uint8_t* str,int len)
 {
    int ret;
    do
     {
         ret = HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)str, len);//请求发送下一个数据  
     }while(ret != HAL_OK);
 }



/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete */
  
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UHandle)
{
  /* Set transmission flag: transfer complete */
//  esp_parse_char(aRxBuffer[0]);
  if(HAL_UART_Receive_IT(UHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler_uart2();
  }
  
  
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    Error_Handler_uart2();
}


void uart2_test(){
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, 1) != HAL_OK)
  {
    Error_Handler_uart2();
  }
	while(1){
	uart2_send(aRxBuffer,1);
	HAL_Delay(1);
	}
}
