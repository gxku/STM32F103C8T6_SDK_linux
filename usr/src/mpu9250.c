
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "datasend.h"
#include "mpu9250.h"

#define __USE_MAG_AK8963

////*******************internal data********************************/
#if defined(__USE_MAG_AK8963)
  uint8_t mpu_raw_uint8[22] = {0};
#else
  uint8_t mpu_raw_uint8[14] = {0};
#endif

#pragma pack(1)
typedef struct {
  int32_t Timestamp;
  int16_t Acc[3];
  int16_t Gyr[3];
#if defined(__USE_MAG_AK8963)
  int16_t Mag[3];
#endif
#if defined(__USE_TCTEMP)
  int16_t Temp;
#endif
} MPU_Raw;
#pragma pack()

typedef struct{
  float Acc[3]; //Accel X,Y,Z
  float Gyr[3]; //Gyro X,Y,Z
  float Mag[3]; //Mag X,Y,Z 
}MPU_value;

MPU_Raw mpu_raw;
MPU_value mpu_value;        //9轴数据

uint8_t BUF[6];           //接收数据缓存区

#define mdelay(__ms)    HAL_Delay(__ms)
/////*******************internal data end********************************/

/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

/* Buffer used for transmission */
//uint8_t aTxBuffer[] = "****SPI - Two Boards communication based on Polling **** SPI Message ******** SPI Message ******** SPI Message ****";


void SPI_Init()
{
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // ???
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE); 
  
  /* SPI2 configuration */ 
  SPI_InitStructure.Direction         = SPI_DIRECTION_2LINES;
  SPI_InitStructure.Mode              = SPI_MODE_MASTER;   //主机模式
  SPI_InitStructure.DataSize          = SPI_DATASIZE_8BIT;       //8位数据
  SPI_InitStructure.CLKPolarity       = SPI_POLARITY_HIGH;             //SPI_CPOL_High=模式3，时钟空闲为高 //SPI_CPOL_Low=模式0，时钟空闲为低
  SPI_InitStructure.CLKPhase          = SPI_PHASE_2EDGE;            //SPI_CPHA_2Edge;//SPI_CPHA_1Edge, SPI_CPHA_2Edge;
  SPI_InitStructure.NSS               = SPI_NSS_SOFT;   //SPI_NSS_Soft;//SPI_NSS_Hard
  SPI_InitStructure.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;//SPI_BaudRatePrescaler_2=32M;//SPI_BaudRatePrescaler_4=18MHz
  SPI_InitStructure.FirstBit          = SPI_FIRSTBIT_MSB;//数据从高位开始发送
  SPI_InitStructure.CRCPolynomial     = 7;

  SpiHandle.Instance  = SPIx;
  SpiHandle.Init = SPI_InitStructure;
  
  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* SPI block is enabled prior calling SPI transmit/receive functions, in order to get CLK signal properly pulled down.
     Otherwise, SPI CLK signal is not clean on this board and leads to errors during transfer */
  __HAL_SPI_ENABLE(&SpiHandle);

  //GPIO Config//
  GPIO_InitStructure.Pin    = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStructure.Speed  = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Mode   = GPIO_MODE_AF_PP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure PA.4(NSS)--------------------------------------------*/
  GPIO_InitStructure.Pin    = GPIO_PIN_12;
  GPIO_InitStructure.Mode   = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  
}

// spi write byte
void SPI_Write_Byte(uint8_t byte)
{    
    uint8_t length = 1;
    int rc = HAL_SPI_Transmit(&SpiHandle, &byte, length, 5000);
    if (rc != 0)
    {
        printf("HAL_SPI_Transmit rc = %d\n", rc);
    }
}

// spi write bytes
void SPI_Write_Bytes(uint8_t byte, uint8_t length)
{    
    int rc = HAL_SPI_Transmit(&SpiHandle, &byte, length, 5000);
    if (rc != 0)
    {
        printf("HAL_SPI_Transmit rc = %d\n", rc);
    }
}

// spi read byte
void SPI_Read_Byte(uint8_t *data)
{    
    uint8_t length = 1;
    int rc = HAL_SPI_Receive(&SpiHandle, data, length, 5000);
    if (rc != 0)
    {
        printf("HAL_SPI_Receive rc = %d\n", rc);
    }
}

// spi read bytes
void SPI_Read_Bytes(uint8_t *data, uint8_t length)
{    
    int rc = HAL_SPI_Receive(&SpiHandle, data, length, 5000);
    if (rc != 0)
    {
        printf("HAL_SPI_Receive rc = %d\n", rc);
    }
}

// spi write register
static void SPI_Write_Reg(uint8_t reg, uint8_t data)
{
    SPIx_NSS_LOW();         //enableMPU9250
    SPI_Write_Byte(reg);
    SPI_Write_Byte(data);
    SPIx_NSS_HIGH();        //disenable MPU9250
}

// spi write registers
static void SPI_Write_Regs(uint8_t reg, uint8_t data, uint8_t length)
{
    SPIx_NSS_LOW();         //enableMPU9250
    SPI_Write_Byte(reg);
    SPI_Write_Bytes(data, length);
    SPIx_NSS_HIGH();        //disenable MPU9250
}

// spi read register
static void SPI_Read_Reg(uint8_t reg, uint8_t *data)
{
    SPIx_NSS_LOW();
    SPI_Write_Byte(reg | 0x80); // read transaction
    SPI_Read_Byte(data);
    SPIx_NSS_HIGH();
}

// spi read registers
static void SPI_Read_Regs(uint8_t reg, uint8_t *data, uint8_t length)
{
    SPIx_NSS_LOW();
    SPI_Write_Byte(reg | 0x80); // read transaction
    SPI_Read_Bytes(data, length);
    SPIx_NSS_HIGH();
}

// I2C write
static void I2C_Write_Reg(uint8_t reg, uint8_t value)
{
  SPI_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR);//设置磁力计地址,mode: write
  mdelay(1);
  SPI_Write_Reg(I2C_SLV0_REG ,reg);//set reg addr
  mdelay(1);
  SPI_Write_Reg(I2C_SLV0_DO ,value);//send value  
  mdelay(1);
}

// I2C read
static void I2C_Read_Reg(uint8_t reg, uint8_t *data)
{
    int add = MPU9250_AK8963_ADDR;
  SPI_Write_Reg(USER_CTRL ,0x20); //Set I2C_MST_EN, I2C_IF_DIS // I2C_MST_EN 
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_ADDR*/49 ,MPU9250_AK8963_ADDR|0x80); //设置磁力计地址，mode：read
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_REG*/50 ,reg);// set reg addr
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_CTRL*/52 ,0x80);
  mdelay(150);//此处因为MPU内部I2C读取速度较慢，必须延时等待内部读取完毕
  SPI_Read_Reg(/*EXT_SENS_DATA_00*/53, data);
  mdelay(5);


  print("form %d read: %d\n",add,*data);
  SPI_Write_Reg(USER_CTRL ,0x20); //Set I2C_MST_EN, I2C_IF_DIS // I2C_MST_EN 
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_ADDR*/49 ,0x0d|0x80); //设置磁力计地址，mode：read
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_REG*/50 ,reg);// set reg addr
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_CTRL*/52 ,0x80);
  mdelay(150);//此处因为MPU内部I2C读取速度较慢，必须延时等待内部读取完毕
  SPI_Read_Reg(/*EXT_SENS_DATA_00*/53, data);
  mdelay(5);


  print("form 0x0d read: %d\n",*data);
  SPI_Write_Reg(USER_CTRL ,0x20); //Set I2C_MST_EN, I2C_IF_DIS // I2C_MST_EN 
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_ADDR*/49 ,0x0e|0x80); //设置磁力计地址，mode：read
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_REG*/50 ,reg);// set reg addr
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_CTRL*/52 ,0x80);
  mdelay(150);//此处因为MPU内部I2C读取速度较慢，必须延时等待内部读取完毕
  SPI_Read_Reg(/*EXT_SENS_DATA_00*/53, data);
  mdelay(5);


  print("form 0x0e read: %d\n",*data);
  SPI_Write_Reg(USER_CTRL ,0x20); //Set I2C_MST_EN, I2C_IF_DIS // I2C_MST_EN 
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_ADDR*/49 ,0x0f|0x80); //设置磁力计地址，mode：read
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_REG*/50 ,reg);// set reg addr
  mdelay(5);
  SPI_Write_Reg(/*I2C_SLV0_CTRL*/52 ,0x80);
  mdelay(150);//此处因为MPU内部I2C读取速度较慢，必须延时等待内部读取完毕
  SPI_Read_Reg(/*EXT_SENS_DATA_00*/53, data);
  mdelay(5);


  print("form 0x0f read: %d\n",*data);
}

//****************INIT MPU9250************************
int8_t MPU9250_Init(void)
{ 
  SPI_Write_Reg(PWR_MGMT_1, 0x80);  //解除休眠状态
  mdelay(200);
  SPI_Write_Reg(PWR_MGMT_1, 0x00);  //解除休眠状态
  mdelay(2);
  SPI_Write_Reg(CONFIG, 0x07);      //低通滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K
  mdelay(2);

/*******************Init GYRO and ACCEL******************************/  
  SPI_Write_Reg(SMPLRT_DIV, 0x07);    //陀螺仪采样率，典型值：0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
  mdelay(2);
  SPI_Write_Reg(GYRO_CONFIG, 0x18);   //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
  mdelay(2);
  SPI_Write_Reg(ACCEL_CONFIG, 0x18);  //加速计自检、测量范围，典型值：0x18(不自检，16G)
  mdelay(2);
  SPI_Write_Reg(ACCEL_CONFIG_2, 0x08);//加速计高通滤波频率 典型值 ：0x08  （1.13kHz） ???????? 
  mdelay(2);
    
/**********************Init SLV0 i2c**********************************/ 
//Use SPI-bus read slave0
  SPI_Write_Reg(INT_PIN_CFG ,0x30);// INT Pin / Bypass Enable Configuration  
  mdelay(2);
  SPI_Write_Reg(I2C_MST_CTRL,0x4d);//I2C MAster mode and Speed 400 kHz
  mdelay(2);
  SPI_Write_Reg(USER_CTRL ,0x20); //Set I2C_MST_EN, I2C_IF_DIS // I2C_MST_EN 
  mdelay(2);
  SPI_Write_Reg(I2C_MST_DELAY_CTRL ,0x01);//I2C_SLV0 _DLY_ enable   
  mdelay(2);
  SPI_Write_Reg(I2C_SLV0_CTRL ,0x81); //enable IIC  and EXT_SENS_DATA==1 Byte
  mdelay(2);

  SPI_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR|0x80); //设置磁力计地址，mode：read
  mdelay(2);
  SPI_Write_Reg(I2C_SLV0_REG ,0x02);// set reg addr 0x02
  mdelay(2);
  SPI_Write_Reg(I2C_SLV0_CTRL ,0x88); //enable IIC  and EXT_SENS_DATA==8 Byte   //when any of measurement data is read, be sure to read ST2 register at the end.
  mdelay(100);

/**********************Init MAG **********************************/
  I2C_Write_Reg(AK8963_CNTL2_REG,AK8963_CNTL2_SRST); // Reset AK8963
  mdelay(200);
  I2C_Write_Reg(AK8963_CNTL1_REG,0x12); // use i2c to set AK8963 working on Continuous measurement mode1 
                                        // & 16-bit output  
  mdelay(2);
  // check id
  int8_t status = 0;
  mdelay(2);
  status = CHECK_MPU9250_Device();
  if (status != 1)
    return 0;


  return 1;
}
/*

  SPI_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR|0x80); //设置磁力计地址，mode：read
  SPI_Write_Reg(I2C_SLV0_REG ,0x03);// set reg addr
  SPI_Write_Reg(I2C_SLV0_CTRL ,0x87); //enable IIC  and EXT_SENS_DATA==6 Byte   //when any of measurement data is read, be sure to read ST2 register at the end.

*/
//**************check id**********************/
int8_t CHECK_MPU9250_Device( void )
{
  uint8_t deviceID = 0x00;

  SPI_Read_Reg(WHO_AM_I, &deviceID);
  print("WHO_AM_I in %x: %x\n", WHO_AM_I, deviceID);
  if (deviceID != MPU6500_DeviceID) {
    return 0;
  }

#if  defined(__USE_MAG_AK8963)
  mdelay(10);
  I2C_Read_Reg(AK8963_WHOAMI_REG, &deviceID);
  print("AK8963_WHO_AM_I in %x: %x\n", AK8963_WHOAMI_REG, deviceID);
  if (deviceID != AK8963_WHOAMI_ID) {
    return 0;
  }
#endif

  return 1;
}

void READ_MPU9250_RawRawData(void)
{
#if defined(__USE_MAG_AK8963)

  SPI_Read_Regs(ACCEL_XOUT_H, mpu_raw_uint8, 14+8);    /// Read Gyr, Acc, Mag /
/*
  SPI_Read_Regs(ACCEL_XOUT_H, mpu_raw_uint8, 14);    /// Read Gyr, Acc, Mag /
  I2C_Read_Reg(AK8963_ST1_REG, &mpu_raw_uint8[14]);
  I2C_Read_Reg(MAG_XOUT_L, &mpu_raw_uint8[15]);
  I2C_Read_Reg(MAG_XOUT_H, &mpu_raw_uint8[16]);
  I2C_Read_Reg(MAG_YOUT_L, &mpu_raw_uint8[17]);
  I2C_Read_Reg(MAG_YOUT_H, &mpu_raw_uint8[18]);
  I2C_Read_Reg(MAG_ZOUT_L, &mpu_raw_uint8[19]);
  I2C_Read_Reg(MAG_ZOUT_H, &mpu_raw_uint8[20]);
  I2C_Read_Reg(AK8963_ST1_REG, &mpu_raw_uint8[21]);
*/
#else
  SPI_Read_Regs(ACCEL_XOUT_H, mpu_raw_uint8, 14);    /* Read Gyr, Acc */
#endif
}

void READ_MPU9250_RawData(void)
{
  mpu_raw.Timestamp = HAL_GetTick();
  READ_MPU9250_RawRawData();

  mpu_raw.Gyr[0] = (mpu_raw_uint8[8]  << 8) | mpu_raw_uint8[9];   /* Gyr.X */
  mpu_raw.Gyr[1] = (mpu_raw_uint8[10] << 8) | mpu_raw_uint8[11];  /* Gyr.Y */
  mpu_raw.Gyr[2] = (mpu_raw_uint8[12] << 8) | mpu_raw_uint8[13];  /* Gyr.Z */
  mpu_raw.Acc[0] = (mpu_raw_uint8[0]  << 8) | mpu_raw_uint8[1];   /* Acc.X */
  mpu_raw.Acc[1] = (mpu_raw_uint8[2]  << 8) | mpu_raw_uint8[3];   /* Acc.Y */
  mpu_raw.Acc[2] = (mpu_raw_uint8[4]  << 8) | mpu_raw_uint8[5];   /* Acc.Z */
#if defined(__USE_TCTEMP)
  mpu_raw.Temp   = (mpu_raw_uint8[6]  << 8) | mpu_raw_uint8[7];   /* ICTemp */
#endif

#if defined(__USE_MAG_AK8963)
  print("%x %x %x %x %x %x %x %x\n",
    mpu_raw_uint8[14],
    mpu_raw_uint8[15],
    mpu_raw_uint8[16],
    mpu_raw_uint8[17],
    mpu_raw_uint8[18],
    mpu_raw_uint8[19],
    mpu_raw_uint8[20],
    mpu_raw_uint8[21]);
  if (!(!(mpu_raw_uint8[14] & AK8963_ST1_DRDY) 
    || (mpu_raw_uint8[14] & AK8963_ST1_DOR) 
    || (mpu_raw_uint8[21] & AK8963_ST2_HOFL))) {
    mpu_raw.Mag[0] = (mpu_raw_uint8[16] << 8) | mpu_raw_uint8[15];  /* Mag.X */
    mpu_raw.Mag[1] = (mpu_raw_uint8[18] << 8) | mpu_raw_uint8[17];  /* Mag.Y */
    mpu_raw.Mag[2] = (mpu_raw_uint8[20] << 8) | mpu_raw_uint8[19];  /* Mag.Z */
  }
  print("mag[1]:%d\n", mpu_raw.Mag[1]);
#endif
}

#if 0
//************************ACCEL**************************/
void READ_MPU9250_ACCEL(void)//
{ 

   SPI_Read_Reg(ACCEL_XOUT_L, &BUF[0]); 
   SPI_Read_Reg(ACCEL_XOUT_H, &BUF[1]);
   mpu_raw.Acc[0]   =  (BUF[1]<<8)|BUF[0];
   mpu_value.Acc[0] =  mpu_raw.Acc[0] / 164.0;                //X
   
   SPI_Read_Reg(ACCEL_YOUT_L, &BUF[2]);
   SPI_Read_Reg(ACCEL_YOUT_H, &BUF[3]);
   mpu_raw.Acc[1]   =  (BUF[3]<<8)|BUF[2];
   mpu_value.Acc[1] =  mpu_raw.Acc[1] / 164.0;                //Y
   
   SPI_Read_Reg(ACCEL_ZOUT_L, &BUF[4]); 
   SPI_Read_Reg(ACCEL_ZOUT_H, &BUF[5]);
   mpu_raw.Acc[2]   =  (BUF[5]<<8)|BUF[4];
   mpu_value.Acc[2] =  mpu_raw.Acc[2] / 164.0;                //Z
}
/**********************GYRO*****************************/
void READ_MPU9250_GYRO(void)
{ 

   SPI_Read_Reg(GYRO_XOUT_L, &BUF[0]); 
   SPI_Read_Reg(GYRO_XOUT_H, &BUF[1]);
   mpu_raw.Gyr[0]   = (BUF[1]<<8)|BUF[0];
   mpu_value.Gyr[0] = mpu_raw.Gyr[0] / 16.4;                //X

   SPI_Read_Reg(GYRO_YOUT_L, &BUF[2]);
   SPI_Read_Reg(GYRO_YOUT_H, &BUF[3]);
   mpu_raw.Gyr[1]   = (BUF[3]<<8)|BUF[2];
   mpu_value.Gyr[1] = mpu_raw.Gyr[1] / 16.4;                //Y
   
   SPI_Read_Reg(GYRO_ZOUT_L, &BUF[4]);
   SPI_Read_Reg(GYRO_ZOUT_H, &BUF[5]);
   mpu_raw.Gyr[2]   = (BUF[5]<<8)|BUF[4];
   mpu_value.Gyr[2] = mpu_raw.Gyr[2] / 16.4;                 //Z
}
/**********************MAG***************************/
//I2C_Read_Reg(AK8963_ST2_REG) 此步读取不可省略
//数据读取结束寄存器，reading this register means data reading end
//AK8963_ST2_REG 同时具有数据非正常溢出检测功能
/**********************************************************/
#if defined(__USE_MAG_AK8963)
void READ_MPU9250_MAG(void)
{   
  uint8_t x_axis,y_axis,z_axis; 
  
  I2C_Read_Reg(AK8963_ASAX, &x_axis);// X Sensitivity
  I2C_Read_Reg(AK8963_ASAY, &y_axis);
  I2C_Read_Reg(AK8963_ASAZ, &z_axis);
  
  uint8_t tmp;
  I2C_Read_Reg(AK8963_ST1_REG, &tmp);
  if((tmp&AK8963_ST1_DOR)==0)//data ready
  {
      //读取计算X轴数据
     I2C_Read_Reg(MAG_XOUT_L, &BUF[0]); //Low data  
     I2C_Read_Reg(AK8963_ST2_REG, &tmp);
     if((tmp&AK8963_ST2_HOFL)==1)// data reading end register & check Magnetic sensor overflow occurred 
     {
       I2C_Read_Reg(MAG_XOUT_L, &BUF[0]);//reload data
     } 
     I2C_Read_Reg(MAG_XOUT_H, &BUF[1]); //High data 
     I2C_Read_Reg(AK8963_ST2_REG, &tmp);
     if((tmp&AK8963_ST2_HOFL)==1)// data reading end register
     {
       I2C_Read_Reg(MAG_XOUT_H, &BUF[1]);
     }
     mpu_raw.Mag[0]=((BUF[1]<<8)|BUF[0])*(((x_axis-128)>>8)+1);   //sensitivity correct see./RM-MPU-9250A-00 PDF/ 5.13 
     mpu_value.Mag[0] = (float)mpu_raw.Mag[0];
     
    //读取计算Y轴数据
     I2C_Read_Reg(MAG_YOUT_L, &BUF[2]); //Low data 
     I2C_Read_Reg(AK8963_ST2_REG, &tmp);
     if((tmp&AK8963_ST2_HOFL)==1)// data reading end register
     {
       I2C_Read_Reg(MAG_YOUT_L, &BUF[2]);
     }     
     I2C_Read_Reg(MAG_YOUT_H, &BUF[3]); //High data 
     I2C_Read_Reg(AK8963_ST2_REG, &tmp);
     if((tmp&AK8963_ST2_HOFL)==1)// data reading end register
     {
       I2C_Read_Reg(MAG_YOUT_H, &BUF[3]);
     }
     mpu_raw.Mag[1]=((BUF[3]<<8)|BUF[2])*(((y_axis-128)>>8)+1);  
     mpu_value.Mag[1] = (float)mpu_raw.Mag[1];
     
    //读取计算Z轴数据
     I2C_Read_Reg(MAG_ZOUT_L, &BUF[4]); //Low data  
     I2C_Read_Reg(AK8963_ST2_REG, &tmp);
     if((tmp&AK8963_ST2_HOFL)==1)// data reading end register
     {
       I2C_Read_Reg(MAG_ZOUT_L, &BUF[4]);
     }   
     I2C_Read_Reg(MAG_ZOUT_H, &BUF[5]); //High data 
     I2C_Read_Reg(AK8963_ST2_REG, &tmp);
     if((tmp&AK8963_ST2_HOFL)==1)// data reading end register
     {
       I2C_Read_Reg(MAG_ZOUT_H, &BUF[5]);
     }
     mpu_raw.Mag[2]=((BUF[5]<<8)|BUF[4])*(((z_axis-128)>>8)+1);  
     mpu_value.Mag[2] = (float)mpu_raw.Mag[2];
  }                
}
#endif
#endif

void mpu9250_main()
{
  SPI_Init();
  if (!MPU9250_Init()) 
  {
    print("error");
    return;
  }
  //print("timestamps\tGX\tGY\tGZ\tAX\tAY\tAZ\tMX\tMY\tMZ\r\n");
  
  while(1)
  {
    mdelay(1);
#if defined(__USE_MAG_AK8963)
    READ_MPU9250_RawData();
    int fd=0, type=2;
    send_blk_data(fd,type,(char*)&mpu_raw,sizeof(mpu_raw));
#else
    READ_MPU9250_RawData();
    int fd=0, type=2;
    send_blk_data(fd,type,(char*)&mpu_raw,sizeof(mpu_raw));
#endif
    //send_blk_data(0,2,"abcdef",6);
    //print("c");
  }
}
