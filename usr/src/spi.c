
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"

/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

/* Buffer used for transmission */
//uint8_t aTxBuffer[] = "****SPI - Two Boards communication based on Polling **** SPI Message ******** SPI Message ******** SPI Message ****";

/* Buffer used for reception */
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int spi_init(void)
{

  /*##-1- Configure the SPI peripheral #######################################*/
  /* Set the SPI parameters */
    SpiHandle.Instance               = SPIx;
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial     = 7;
//    SpiHandle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;    
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  //  SpiHandle.Init.NSSPMode          = SPI_NSS_PULSE_ENABLE;
    SpiHandle.Init.Mode              = SPI_MODE_MASTER;

  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* SPI block is enabled prior calling SPI transmit/receive functions, in order to get CLK signal properly pulled down.
     Otherwise, SPI CLK signal is not clean on this board and leads to errors during transfer */
  __HAL_SPI_ENABLE(&SpiHandle);

  return 0;
}


static void delayMicroseconds(int x)
{
    volatile int vx = x * 10;
    
    while (--vx);
}

void spiWriteByte(uint8_t byte)
{    
    int rc = HAL_SPI_Transmit(&SpiHandle, &byte, 1, 5000);
    if (rc != 0)
    {
        printf("HAL_SPI_Transmit rc = %d\n", rc);
    }
}

void spiRead(uint8_t *data, uint8_t length)
{    
    int rc = HAL_SPI_Receive(&SpiHandle, data, length, 5000);
    if (rc != 0)
    {
        printf("HAL_SPI_Receive rc = %d\n", rc);
    }
}

static void mpu9250SpiWriteRegister(uint8_t reg, uint8_t data)
{
    SPIx_NSS_LOW();
    delayMicroseconds(1);
    spiWriteByte(reg);
    spiWriteByte(data);
    SPIx_NSS_HIGH();
    delayMicroseconds(1);
}

static void mpu9250SpiSlowReadRegisterBuffer(uint8_t reg, uint8_t *data, uint8_t length)
{
    SPIx_NSS_LOW();
    delayMicroseconds(1);
    spiWriteByte(reg | 0x80); // read transaction
    spiRead(data, length);
    SPIx_NSS_HIGH();
    delayMicroseconds(1);
}


void spi_test(){
    uint8_t data[3]={0};
    mpu9250SpiWriteRegister(0x6b, 0x80);
    
    HAL_Delay(2);
    mpu9250SpiWriteRegister(118,0x99);
    HAL_Delay(2);
    mpu9250SpiSlowReadRegisterBuffer(117,data,2);

    print("read %x %x \n",data[0],data[1]);

}
