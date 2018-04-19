/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
//#include "stm32f1xx_nucleo.h"
#include "stm32f1xx_it.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define PRINT   //if use debug print
#define Error_Handler() (Error_Handler_Msg(__FILE__, __FUNCTION__,  __LINE__))
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void print(char* fmt, ...);
void Error_Handler_Msg(char* file,const char* function,uint32_t line);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
