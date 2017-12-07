
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"

/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

/* Buffer used for transmission */
//uint8_t aTxBuffer[] = "****SPI - Two Boards communication based on Polling **** SPI Message ******** SPI Message ******** SPI Message ****";

/* Buffer used for reception */
#define BUFFERSIZE 4
uint8_t aRxBf[BUFFERSIZE]={0};
uint8_t aTxBf[BUFFERSIZE]={117,117,117,117};
#define MASTER_BOARD
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int spi_main(void)
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
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;

  SpiHandle.Init.Mode = SPI_MODE_MASTER;

  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* SPI block is enabled prior calling SPI transmit/receive functions, in order to get CLK signal properly pulled down.
     Otherwise, SPI CLK signal is not clean on this board and leads to errors during transfer */
  __HAL_SPI_ENABLE(&SpiHandle);


  /*##-2- Start the Full Duplex Communication process ########################*/  
  /* While the SPI in TransmitReceive process, user can transmit data through 
     "aTxBuffer" buffer & receive data through "aRxBuffer" */
  /* Timeout is set to 5S */
  
  switch(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)aTxBf, (uint8_t *)aRxBf, BUFFERSIZE, 5000))
  {
    case HAL_OK:
      /* Communication is completed ___________________________________________ */
      /* Compare the sent and received buffers */
	print("aRxBuffer = %x %x %x %x  \n",aRxBf[0],aRxBf[1],aRxBf[2],aRxBf[3]);
      /* Turn LED2 on: Transfer in transmission/Reception process is correct */
      break;

    case HAL_TIMEOUT:
	print("TOUT");
      /* An Error Occur ______________________________________________________ */
    case HAL_ERROR:
      /* Call Timeout Handler */
      Error_Handler();
      break;
    default:
      break;
  }

  /* Infinite loop */
  while (1)
  {
  }
}
