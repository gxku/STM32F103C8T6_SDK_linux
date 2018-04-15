#include "uart.h"
#include "print.h"
	
void wifi_uart_start(){
    uart2_init();
    HAL_Delay(1000);
    print("AT+CWMODE=2\r\n");
    HAL_Delay(1000);
    print("AT+CWSAP=\"TEST\",\"12345678\",1,3\r\n");
}

