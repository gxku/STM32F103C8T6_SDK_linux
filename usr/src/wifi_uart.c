#include "uart.h"
	
    void wifi_uart_start(){
    uart2_init();
    int c=0;
    HAL_Delay(1000);
    uart2_print("AT+CWMODE=2\r\n");
    HAL_Delay(1000);
    uart2_print("AT+CWSAP=\"TEST\",\"12345678\",1,3\r\n");
}

