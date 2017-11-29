
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment this line to use the board as master, if not it is used as slave */
#define MASTER_BOARD
#define I2C_ADDRESS        0x55

/* I2C SPEEDCLOCK define to max value: 400 KHz on STM32F1xx*/
#define I2C_SPEEDCLOCK   400000
#define I2C_DUTYCYCLE    I2C_DUTYCYCLE_2



#define ADDR		 	0x55	/**< I2C adress of TCA62724FMG */
#define SUB_ADDR_START		0x01	/**< write everything (with auto-increment) */
#define SUB_ADDR_PWM0		0x81	/**< blue     (without auto-increment) */
#define SUB_ADDR_PWM1		0x82	/**< green    (without auto-increment) */
#define SUB_ADDR_PWM2		0x83	/**< red      (without auto-increment) */
#define SUB_ADDR_SETTINGS	0x84	/**< settings (without auto-increment)*/

#define SETTING_NOT_POWERSAVE	0x01	/**< power-save mode not off */
#define SETTING_ENABLE   	0x02	/**< on */



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* I2C handler declaration */
I2C_HandleTypeDef I2cHandle;


/* Buffer used for transmission */
//uint8_t aTxBuffer[] = " ****I2C_TwoBoards communication based on Polling****  ****I2C_TwoBoards communication based on Polling****  ****I2C_TwoBoards communication based on Polling**** ";
uint8_t aTxBuffer[] ={0x84,0x3};
uint8_t rTxBuffer[] ={0x81,250};
uint8_t gTxBuffer[] ={0x82,250};
uint8_t bTxBuffer[] ={0x83,250};

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void i2c_main(void)
{

  /*##-1- Configure the I2C peripheral ######################################*/
  I2cHandle.Instance             = I2Cx;
  I2cHandle.Init.ClockSpeed      = I2C_SPEEDCLOCK;
  I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0xFF;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;  
  
  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler("HAL_I2C_Init");
  }
  



  while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)ADDR, (uint8_t*)aTxBuffer, 2, 10000)!= HAL_OK)
  {
	  /* Error_Handler_() function is called when Timeout error occurs.
	     When Acknowledge failure occurs (Slave don't acknowledge its address)
	     Master restarts communication */
	  if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
	  {
		  Error_Handler("aTxBuffer");
	  }
  }
  while(1){
	  while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)ADDR, (uint8_t*)rTxBuffer, 2, 10000)!= HAL_OK)
	  {
		  /* Error_Handler_() function is called when Timeout error occurs.
		     When Acknowledge failure occurs (Slave don't acknowledge its address)
		     Master restarts communication */
		  if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		  {
			  Error_Handler("rTxBuffer");
		  }
	  }
	  while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)ADDR, (uint8_t*)gTxBuffer, 2, 10000)!= HAL_OK)
	  {
		  /* Error_Handler_() function is called when Timeout error occurs.
		     When Acknowledge failure occurs (Slave don't acknowledge its address)
		     Master restarts communication */
		  if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		  {
			  Error_Handler("gTxBuffer");
		  }
	  }
	  while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)ADDR, (uint8_t*)bTxBuffer, 2, 10000)!= HAL_OK)
	  {                                                                                                                                                                                          
		  /* Error_Handler_() function is called when Timeout error occurs.
		     When Acknowledge failure occurs (Slave don't acknowledge its address)
		     Master restarts communication */
		  if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
		  {
			  Error_Handler("bTxBuffer");
		  }
	  }
	HAL_Delay(1000);

  }

}
/**
 * @brief  I2C error callbacks.
 * @param  I2cHandle: I2C handle
 * @note   This example shows a simple way to report transfer error, and you can
 *         add your own implementation.
 * @retval None
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
