/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
//#include "stm32f1xx_nucleo.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
#define PRINT   //if use debug print
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern void print(char* fmt, ...);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
