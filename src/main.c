///
/// @version 1.0 2017-03-22 21:09
/// @author Tavis
///

#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"


void GPIO_Config(void);
void USART2_Config(void);
void RCC_Config(void);

FlagStatus RX_status;  
FlagStatus Tx_status;  
unsigned char Rxbuf[20]; 
///////////////////////////////////

//stm32flash -w Demo.hex -v -g 0 /dev/ttyUSB0
void esp_set(unsigned char *puf){
	while(*puf!='\0')
	{
		
		USART_SendData(USART2,*puf);
		Delay(0xffff); 
		puf++;
	}
	Delay(0xffff); 
	USART_SendData(USART2,'\r');
	Delay(0xffff); 
	USART_SendData(USART2,'\n');
	Delay(0x1fffff); 
}
void esp_sent(unsigned char *puf){
	esp_set("AT+CIPSEND=0,4");
	while(*puf!='\0')
	{
		
		USART_SendData(USART2,*puf);
		Delay(0xffff); 
		puf++;
	}
	Delay(0xffff); 
	USART_SendData(USART2,'\n');
	Delay(0xfffff); 
}
void USART2_IRQHandler(void)  
  
{  int i=0;
       //确认是否接收到数据  
  
       RX_status = USART_GetFlagStatus(USART2, USART_FLAG_RXNE);  
  
       //接收到数据  
        
      if(RX_status == SET)  
      {  
       //读出数据  
	Rxbuf[i] = USART_ReceiveData(USART2);  
	  i++;  
	if(Rxbuf[i-1]=='\n')i=0;
	}
}  
//////////////////////////////////

int main(void)
{
	RCC_Config();   //时钟设置
	GPIO_Config();
	USART2_Config();
	esp_set("AT+CWMODE=2");
	esp_set("AT+RST");
	esp_set("AT+CWSAP=\"YMJ_\",\"1234567890\",11,4");
	esp_set("AT+CIPMUX=1");
	esp_set("AT+CIPSERVER=1,8080");
	while(1)
	{
		if((Rxbuf[0]=='+')&&(Rxbuf[1]=='I')&&(Rxbuf[2]=='P')){
  			if((Rxbuf[3]=='D')&&(Rxbuf[6]==',')){
				if(Rxbuf[9]=='1')LED_Blink(); 
			}
		}
	}
}

void RCC_Config(void)
{
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;	  //定义一个结构体变量
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   	GPIO_Init(GPIOA, &GPIO_InitStructure);	 //初始化库函数

	//PA2 TXD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	//PA3 RXD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
}

void USART2_Config(void)
{
	USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 	//开启接收中断
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);	//开启发送中断
	USART_Cmd(USART2, ENABLE);
}
/*
int main()
{

  LED_Blink(); 
  
  return 0;
}
*/
