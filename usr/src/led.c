
/* Includes ------------------------------------------------------------------*/
#include "led.h"

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
static GPIO_InitTypeDef  GPIO_InitStruct;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int led_init(void)
{



  /* -1- Enable each GPIO Clock (to be able to program the configuration registers) */
  LED_GPIO_CLK_ENABLE();

  /* -2- Configure IOs in output push-pull mode to drive external LEDs */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = LED_PIN;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}


void led_toggle(int times){
  /* -3- Toggle IOs in an infinite loop */
  while (times-->0)
  {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    /* Insert delay 100 ms */
    HAL_Delay(100);
  }

}
 
void led_test(){
	led_init();
	led_toggle(10);
}
