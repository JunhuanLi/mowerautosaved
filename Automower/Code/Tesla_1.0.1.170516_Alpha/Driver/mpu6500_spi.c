 /*******************************************************************************
 
   Copyright (C), 2017-2027, TOPBAND Co., Ltd.
   File name	 : mpu6500_spi.c
   Author		 : klaus	 
   Version		 : V1.0.0	   
   Date 		 : 2017/05/17
   Description	 : mpu6500 driver 
   
   History: 	   
				   
	 1. Date		 :
		Author		 :
		Modification :
	 2. ...
	 
 *******************************************************************************/


/* includes *******************************************************************/
#include "mpu6500_spi.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_mpu g_mpu = {0};

/* funcitons ******************************************************************/
static void mpu6500_gpio_configuration(void);
static void mpu6500_spi_configuration(void);

static void mpu6500_spi_txrx(u8 tx_data, u8 *rx_data);
static void mpu6500_spi_write_register(u8 addr, u8 value);
static void mpu6500_spi_read_register(u8 addr, u8 *ret_value);

/*******************************************************************************
  Function		: mpu6500_gpio_configuration      
  Description	: mpu6500 gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void mpu6500_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(MPU6500_CS_CLK | MPU6500_CLK_CLK | MPU6500_MISO_CLK | 
						   MPU6500_MOSI_CLK,ENABLE);
	
	GPIO_PinAFConfig(MPU6500_MOSI_PORT, MPU6500_MOSI_SOURCE, MPU6500_SPI_AF);
	GPIO_PinAFConfig(MPU6500_MISO_PORT, MPU6500_MISO_SOURCE, MPU6500_SPI_AF);
	GPIO_PinAFConfig(MPU6500_CLK_PORT, MPU6500_CLK_SOURCE, MPU6500_SPI_AF);


	GPIO_InitStructure.GPIO_Pin = MPU6500_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MPU6500_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MPU6500_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MPU6500_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MPU6500_MOSI_PIN;
	GPIO_Init(MPU6500_MOSI_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MPU6500_MISO_PIN;
	GPIO_Init(MPU6500_MISO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: mpu6500_spi_configuration      
  Description	: mpu6500 spi configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void mpu6500_spi_configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	
	SPI_I2S_DeInit(MPU6500_SPI);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(MPU6500_SPI, &SPI_InitStructure);
	SPI_Cmd(MPU6500_SPI, ENABLE);
}

/*******************************************************************************
  Function		: mpu6500_spi_txrx      
  Description	: mpu6500 spi txrx
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void mpu6500_spi_txrx(u8 tx_data, u8 *rx_data)
{
	while(SPI_I2S_GetFlagStatus(MPU6500_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
	SPI_I2S_SendData(MPU6500_SPI, tx_data);

	while(SPI_I2S_GetFlagStatus(MPU6500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}
	*rx_data = SPI_I2S_ReceiveData(MPU6500_SPI);
}

/*******************************************************************************
  Function		: mpu6500_spi_write_register      
  Description	: mpu6500 spi write register
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void mpu6500_spi_write_register(u8 addr, u8 value)
{
	u8 temp = 0;

	addr &= 0x7F;

	MPU6500_RESET_CS();
	
	mpu6500_spi_txrx(addr, &temp);
	mpu6500_spi_txrx(value, &temp);

	MPU6500_SET_CS();
}

/*******************************************************************************
  Function		: mpu6500_spi_read_register      
  Description	: mpu6500 spi read register
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void mpu6500_spi_read_register(u8 addr, u8 *ret_value)
{
	u8 temp = 0;

	addr |= 0x80;

	MPU6500_RESET_CS();
	
	mpu6500_spi_txrx(addr, &temp);

	temp = 0xFF;
	
	mpu6500_spi_txrx(temp, ret_value);

	MPU6500_SET_CS();
}

/*******************************************************************************
  Function		: get_acc      
  Description	: get acc
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_acc(void)
{
	u8 data[2] = {0};

	mpu6500_spi_read_register(ACCEL_XOUT_L, &data[0]);
	mpu6500_spi_read_register(ACCEL_XOUT_H, &data[1]);
	g_mpu.ax = (s16)(data[1] << 8 | data[0]);

	mpu6500_spi_read_register(ACCEL_YOUT_L, &data[0]);
	mpu6500_spi_read_register(ACCEL_YOUT_H, &data[1]);
	g_mpu.ay = (s16)(data[1] << 8 | data[0]);


	mpu6500_spi_read_register(ACCEL_ZOUT_L, &data[0]);
	mpu6500_spi_read_register(ACCEL_ZOUT_H, &data[1]);
	g_mpu.az = (s16)(data[1] << 8 | data[0]);
}
/*******************************************************************************
  Function		: get_gyro      
  Description	: get gyro
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_gyro(void)
{
	u8 data[2] = {0};
	
	mpu6500_spi_read_register(GYRO_XOUT_L, &data[0]);
	mpu6500_spi_read_register(GYRO_XOUT_H, &data[1]);
	g_mpu.gx = (s16)(data[1] << 8 | data[0]);
	
	mpu6500_spi_read_register(GYRO_YOUT_L, &data[0]);
	mpu6500_spi_read_register(GYRO_YOUT_H, &data[1]);
	g_mpu.gy = (s16)(data[1] << 8 | data[0]);
	
	mpu6500_spi_read_register(GYRO_ZOUT_L, &data[0]);
	mpu6500_spi_read_register(GYRO_ZOUT_H, &data[1]);
	g_mpu.gz = (s16)(data[1] << 8 | data[0]);
}

/*******************************************************************************
  Function		: get_temp      
  Description	: get mpu6500 temperature
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_temp(void)
{
	u8 data[2] = {0};

	mpu6500_spi_read_register(TEMP_OUT_L, &data[0]);
	mpu6500_spi_read_register(TEMP_OUT_H, &data[1]);
	g_mpu.temp = (s16)(data[1] << 8 | data[0]);

}

/*******************************************************************************
  Function		: mpu_init      
  Description	: mpu init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool mpu_init(void)
{
	u8 id = 0;
	
	mpu6500_gpio_configuration();
	mpu6500_spi_configuration();

	mpu6500_spi_write_register(PWR_MGMT_1, 0x00);
	mpu6500_spi_write_register(SMPLRT_DIV, 0x07);
	mpu6500_spi_write_register(CONFIG, 0x06);
	mpu6500_spi_write_register(ACCEL_CONFIG, 0x00);
	mpu6500_spi_write_register(GYRO_CONFIG, 0x00);  	// 0x01    GYRO_FACTOR  +- 500dps
	mpu6500_spi_read_register(WHO_AM_I, &id);

	if(id != 0x70)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
/*******************************************************************************
  Function		: get_mpu_info     
  Description	: get mpu infot
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_mpu_info(T_mpu *mpu)
{
	get_gyro();
	get_acc();
    get_temp();
	
	mpu->ax = g_mpu.ax;
	mpu->ay = g_mpu.ay;
	mpu->az = g_mpu.az;
	
	mpu->gx = g_mpu.gx;
	mpu->gy = g_mpu.gy;
	mpu->gz = g_mpu.gz;
	
	mpu->temp = g_mpu.temp;
	
}

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
