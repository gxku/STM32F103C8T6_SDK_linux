
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
static uint16_t ADDR=0xd0;
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
void i2c_init(void)
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
    Error_Handler();
  }
  

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
//**************************************
 //向I2C设备写入一个字节数据
//**************************************
void single_writeI2C(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
 {
    

#if 1
    uint8_t obuf[2] = {REG_Address, REG_data};
    HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)SlaveAddress, obuf, 2, 10000);
#else
    HAL_I2C_Mem_Write(&I2cHandle,  (uint16_t)SlaveAddress, REG_Address, 1, REG_data, 1, 10000);
#endif
 }
 //**************************************
 //从I2C设备读取一个字节数据
//**************************************
uint8_t single_readI2C(unsigned char SlaveAddress, unsigned char REG_Address)
 {
    unsigned char REG_data = 0x00;
    

#if 0
    HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)SlaveAddress, &REG_Address, 1, 10000);
    HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)SlaveAddress, &REG_data, 1, 10000);
#else
    HAL_I2C_Mem_Read(&I2cHandle,  (uint16_t)SlaveAddress, REG_Address, 1, &REG_data, 1, 10000);
#endif 
    return REG_data;
}

#define     USER_CONTROL    0x6A
#define     SMPLRT_DIV      0x19    //Gyro sampling rate, typical values:  0x07(125Hz)
#define     CONFIG          0x1A    //Low pass filter frequency, typical value: 0x06(5Hz)
#define     GYRO_CONFIG     0x1B    //Gyroscope self check and measurement range, typical value:   0x18(No self check, 2000deg/s)
#define     ACCEL_CONFIG    0x1C    //Accelerometer measurement range, Self checking,  and high pass filtering frequency, typical values:  0x01(No self check, 2G锟?5Hz)
//Interrup configure register
#define     INT_BYPASS_CONFIG 0x37
#define     INT_ENABLE      0x38
#define     ACCEL_CONFIG_2  0x1D
#define     MOT_DETECT_CTRL 0X69
#define     WOM_THR         0X1F
#define     LP_ACCEL_ODR    0X1E
#define     INT_STATUS      0x3A
#define     PWR_MGMT_1      0x6B    //Power management, typical values:  0x00(?y3锟??贸?)
#define     PWR_MGMT_2      0x6C

#define     WHO_AM_I        0x75
void i2c_test(){

	uint8_t bufIn[3]={0};
	uint8_t wmi, magid;

	single_writeI2C(ADDR,PWR_MGMT_1, 0x00);
	single_writeI2C(ADDR,SMPLRT_DIV, 0x07);
	single_writeI2C(ADDR,CONFIG, 0x06);
	single_writeI2C(ADDR,USER_CONTROL,0x00);       //close Master Mode
	single_writeI2C(ADDR,GYRO_CONFIG, 0x18);
	single_writeI2C(ADDR,ACCEL_CONFIG, 0x01);

	wmi = single_readI2C(ADDR, WHO_AM_I);
	print("MPU9250 WMI = %02x\n", wmi);
	single_writeI2C(ADDR,118,0xee);
	single_writeI2C(ADDR,119,0xee);

	bufIn[0] = single_readI2C(ADDR,117);
	bufIn[1] = single_readI2C(ADDR,118);
	bufIn[2] = single_readI2C(ADDR,119);
	print(" read %0x %x %x\n",bufIn[0],bufIn[1],bufIn[2]);

}
