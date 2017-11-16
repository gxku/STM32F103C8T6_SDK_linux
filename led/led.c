#include "led.h"
#include "stm32f10x.h"

void LED_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  // LED2(green) --> PC.13
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_13; 
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStruct); 
}

void Delay(uint32_t count) {
	for (; count != 1; --count); 
}

void LED_Blink(void) {
  LED_Init(); 

  for (int i=0;i<1;i++) {
    GPIO_ResetBits(GPIOC, GPIO_Pin_13); 
		Delay(0xfffff); 

    GPIO_SetBits(GPIOC, GPIO_Pin_13); 
		Delay(0xfffff); 
  }
}
