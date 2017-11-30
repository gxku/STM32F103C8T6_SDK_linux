#include"main.h"
//////////////////////////////led
#define LED_PIN                           GPIO_PIN_13
#define LED_GPIO_PORT                     GPIOC
#define LED_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOC_CLK_DISABLE()
  
int led_init(void);
void led_toggle(int times);
void led_test(void);
