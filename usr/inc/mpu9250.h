#ifndef __mpu9250_h__
#define __mpu9250_h__
#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_spi.h"
#include "stm32f1xx_hal_rcc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor SPIx instance used and associated
   resources */
/* Definition for SPIx clock resources */
#define SPIx                             SPI2
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_NSS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI2_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_13
#define SPIx_SCK_GPIO_PORT               GPIOB
#define SPIx_MISO_PIN                    GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MOSI_PIN                    GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT              GPIOB

#define SPIx_NSS_PIN                     GPIO_PIN_12
#define SPIx_NSS_GPIO_PORT               GPIOB

#if 1
#define SPIx_NSS_LOW()                   HAL_GPIO_WritePin(SPIx_NSS_GPIO_PORT, SPIx_NSS_PIN, 0);
#define SPIx_NSS_HIGH()                  HAL_GPIO_WritePin(SPIx_NSS_GPIO_PORT, SPIx_NSS_PIN, 1);
#else
#define SPIx_NSS_LOW()                   
#define SPIx_NSS_HIGH()                  
#endif

// define MPU9250 internal address
/*************************************************************************/
#define SMPLRT_DIV                          0x19 // GYRO sampling rate
#define	CONFIG			            		0x1A	
#define	GYRO_CONFIG		            		0x1B	
#define	ACCEL_CONFIG	                    0x1C	
#define	ACCEL_CONFIG_2                      0x1D 
#define LP_ACCEL_ODR                        0x1E
#define MOT_THR                             0x1F

#define INT_PIN_CFG                         0x37 // interrupt config
#define I2C_MST_CTRL                        0x24
#define I2C_MST_DELAY_CTRL                  0x67
//--------------------i2c slv0-------------------------------//
#define I2C_SLV0_ADDR                       0x25  
#define I2C_SLV0_REG                        0x26
#define I2C_SLV0_CTRL                       0x27 
#define I2C_SLV1_ADDR                       0x28
#define I2C_SLV1_REG                        0x29
#define I2C_SLV1_CTRL                       0x2A
#define I2C_SLV2_ADDR                       0x2B
#define I2C_SLV2_REG                        0x2C
#define I2C_SLV2_CTRL                       0x2D
#define I2C_SLV3_ADDR                       0x2E
#define I2C_SLV3_REG                        0x2F
#define I2C_SLV3_CTRL                       0x30
#define I2C_SLV4_ADDR                       0x31
#define I2C_SLV4_REG                        0x32
#define I2C_SLV4_DO                         0x33
#define I2C_SLV4_CTRL                       0x34
#define I2C_SLV4_DI                         0x35
#define I2C_MST_STATUS                      0x36
#define INT_PIN_CFG                         0x37
#define INT_ENABLE                          0x38
#define INT_STATUS                          0x3A
//--------------------9axis  reg addr-----------------------//
#define	ACCEL_XOUT_H	                    0x3B
#define	ACCEL_XOUT_L	                    0x3C
#define	ACCEL_YOUT_H	                    0x3D
#define	ACCEL_YOUT_L	                    0x3E
#define	ACCEL_ZOUT_H	                    0x3F
#define	ACCEL_ZOUT_L	                    0x40
                    
#define	TEMP_OUT_H		                    0x41   //temperture
#define	TEMP_OUT_L		                    0x42
                    
#define	GYRO_XOUT_H		                    0x43
#define	GYRO_XOUT_L		                    0x44	
#define	GYRO_YOUT_H		                    0x45
#define	GYRO_YOUT_L		                    0x46
#define	GYRO_ZOUT_H		                    0x47
#define	GYRO_ZOUT_L		                    0x48

#define EXT_SENS_DATA_00                    0x49  //MPU9250 IIC外挂器件读取返回寄存器00
#define EXT_SENS_DATA_01                    0x4a  //MPU9250 IIC外挂器件读取返回寄存器01
#define EXT_SENS_DATA_02                    0x4b  //MPU9250 IIC外挂器件读取返回寄存器02
#define EXT_SENS_DATA_03                    0x4c  //MPU9250 IIC外挂器件读取返回寄存器03
#define EXT_SENS_DATA_04                    0x4D
#define EXT_SENS_DATA_05                    0x4E
#define EXT_SENS_DATA_06                    0x4F
#define EXT_SENS_DATA_07                    0x50
#define EXT_SENS_DATA_08                    0x51
#define EXT_SENS_DATA_09                    0x52
#define EXT_SENS_DATA_10                    0x53
#define EXT_SENS_DATA_11                    0x54
#define EXT_SENS_DATA_12                    0x55
#define EXT_SENS_DATA_13                    0x56
#define EXT_SENS_DATA_14                    0x57
#define EXT_SENS_DATA_15                    0x58
#define EXT_SENS_DATA_16                    0x59
#define EXT_SENS_DATA_17                    0x5A
#define EXT_SENS_DATA_18                    0x5B
#define EXT_SENS_DATA_19                    0x5C
#define EXT_SENS_DATA_20                    0x5D
#define EXT_SENS_DATA_21                    0x5E
#define EXT_SENS_DATA_22                    0x5F
#define EXT_SENS_DATA_23                    0x60

#define I2C_SLV0_DO                         0x63 //output reg
#define I2C_SLV1_DO                         0x64
#define I2C_SLV2_DO                         0x65
#define I2C_SLV3_DO                         0x66
#define I2C_MST_DELAY_CTRL                  0x67
#define SIGNAL_PATH_RESET                   0x68
#define MOT_DETECT_CTRL                     0x69
#define USER_CTRL                           0x6A //
#define PWR_MGMT_1                          0x6B //电源管理，典型值：0x00(正常启用)
#define PWR_MGMT_2                          0x6C

#define FIFO_COUNTH                         0x72
#define FIFO_COUNTL                         0x73
#define FIFO_R_W                            0x74
#define WHO_AM_I                            0x75 /* ID = 0x71 In MPU9250 */
#define XA_OFFSET_H                         0x77
#define XA_OFFSET_L                         0x78
#define YA_OFFSET_H                         0x7A
#define YA_OFFSET_L                         0x7B
#define ZA_OFFSET_H                         0x7D
#define ZA_OFFSET_L                         0x7E


#define I2C_SLVx_EN                         0x80
//--------------------AK8963 reg addr------------------------//
#define MPU9250_AK8963_ADDR                 0x0C  //AKM addr
#define AK8963_WHOAMI_REG                   0x00  //AKM ID addr
#define AK8963_ST1_REG                      0x02  //Data Status1
#define MAG_XOUT_L		                    0x03
#define MAG_XOUT_H		                    0x04
#define MAG_YOUT_L		                    0x05
#define MAG_YOUT_H		                    0x06
#define MAG_ZOUT_L		                    0x07
#define MAG_ZOUT_H		                    0x08
#define AK8963_ST2_REG                      0x09  //Data reading end register & check Magnetic sensor overflow occurred
#define AK8963_ST1_DRDY                     0x01 //Data Ready
#define AK8963_ST1_DOR                      0x02
#define AK8963_ST2_HOFL                     0x08 // Magnetic sensor overflow 
#define AK8963_ST2_BITM                     0x10
#define AK8963_CNTL1_REG                    0x0A
#define AK8963_CNTL2_REG                    0x0B
#define AK8963_ASTC                         0x0C
#define AK8963_TS1                          0x0D
#define AK8963_TS2                          0x0E
#define AK8963_I2CDIS                       0x0F
#define AK8963_CNTL2_SRST                   0x01 //soft Reset
#define AK8963_ASAX                         0x10 //X-axis sensitivity adjustment value 
#define AK8963_ASAY                         0x11 //Y-axis sensitivity adjustment value
#define AK8963_ASAZ                         0x12 //Z-axis sensitivity adjustment value
#define XG_OFFSET_H                         0x13
#define XG_OFFSET_L                         0x14
#define YG_OFFSET_H                         0x15
#define YG_OFFSET_L                         0x16
#define ZG_OFFSET_H                         0x17
#define ZG_OFFSET_L                         0x18
//--------------------reg value-----------------------//
#define MPU6500_DeviceID                    0x73   /* In MPU9250 */
#define AK8963_WHOAMI_ID                    0x48  //ID

/************************SPI CS ********************************/
//#define MPU_9250_DISENABLE  GPIOA->BSRR = GPIO_Pin_0;//片选
//#define MPU_9250_ENABLE  GPIOA->BRR = GPIO_Pin_0;


/*************************************************************************/
void SPI_Init();
int8_t MPU9250_Init(void);
int8_t CHECK_MPU9250_Device(void);
void READ_MPU9250_RawData(void);
void READ_MPU9250_ACCEL(void);	//读取加速度
void READ_MPU9250_GYRO(void);	//读取陀螺仪
void READ_MPU9250_MAG(void);	//读取地磁计

void mpu9250_main();




#endif

