
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment this line to use the board as master, if not it is used as slave */
#define I2C_ADDRESS        0x30F

/* I2C SPEEDCLOCK define to max value: 400 KHz on STM32F1xx*/
//#define I2C_SPEEDCLOCK   400000
#define I2C_SPEEDCLOCK   100000
#define I2C_DUTYCYCLE    I2C_DUTYCYCLE_2



#define SETTING_NOT_POWERSAVE	0x01	/**< power-save mode not off */
#define SETTING_ENABLE   	0x02	/**< on */



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* I2C handler declaration */
I2C_HandleTypeDef I2cHandle;


/* Buffer used for reception */

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void i2c_main(void)
{

	int i=0;
	uint16_t ADDR=0xd0;
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
    Error_Handler();
  }
  

  uint8_t data[8] = {117};//, 0x03, 0x81, 128 >> 4, 0x82, 128 >> 4, 0x83, 128 >> 4}; 
uint8_t wdata[8] = {118,0xee};//, 0x03, 0x81, 128 >> 4, 0x82, 128 >> 4, 0x83, 128 >> 4}; 
uint8_t bufIn[3]={0};
if(0){   //can not use ,something wrong
	HAL_I2C_Mem_Write(&I2cHandle,  (uint16_t)ADDR, 118, 1, (uint8_t*)&wdata[1], 1, 10000);
	HAL_I2C_Mem_Read(&I2cHandle,  (uint16_t)ADDR, 117, 2, (uint8_t*)bufIn, 2, 10000);
}else{

	// write
	if (HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)ADDR, (uint8_t*)wdata, 2, 10000)!= HAL_OK)
	{
		print("write 84 error\n");
		return;
	}
	//read
	if (HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)ADDR, (uint8_t*)data, 1, 10000)!= HAL_OK)
	{
		print("write 84 error\n");
		return;
	}
	HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)ADDR, bufIn, 2, 10000);
	//end of read
} 
print("write done and read %0x %x \n",bufIn[0],bufIn[1]);
return;

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
