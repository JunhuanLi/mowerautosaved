 /*******************************************************************************
 
   Copyright (C), 2017-2027, TOPBAND Co., Ltd.
   File name	 : 2.4G.c
   Author		 : klaus	 
   Version		 : V1.0.0	   
   Date 		 : 2017/05/17
   Description	 : 2.4G driver 
   
   History: 	   
				   
	 1. Date		 :
		Author		 :
		Modification :
	 2. ...
	 
 *******************************************************************************/


/* includes *******************************************************************/
#include "2.4G_spi.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/

/* funcitons ******************************************************************/
static void _2_4G_gpio_configuration(void);
static void _2_4G_spi_configuration(void);

static void _2_4G_spi_txrx(u8 tx_data, u8 *rx_data);
static void _2_4G_spi_write_register(u8 addr, u8 value);
static void _2_4G_spi_read_register(u8 addr, u8 *ret_value);

/*******************************************************************************
  Function		: _2_4G_gpio_configuration      
  Description	: 2.4G gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void _2_4G_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(_2_4G_CS_CLK | _2_4G_CLK_CLK | _2_4G_MISO_CLK | 
						   _2_4G_MOSI_CLK,ENABLE);
	
	GPIO_PinAFConfig(_2_4G_MOSI_PORT, _2_4G_MOSI_SOURCE, _2_4G_SPI_AF);
	GPIO_PinAFConfig(_2_4G_MISO_PORT, _2_4G_MISO_SOURCE, _2_4G_SPI_AF);
	GPIO_PinAFConfig(_2_4G_CLK_PORT, _2_4G_CLK_SOURCE, _2_4G_SPI_AF);


	GPIO_InitStructure.GPIO_Pin = _2_4G_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(_2_4G_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = _2_4G_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(_2_4G_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = _2_4G_MOSI_PIN;
	GPIO_Init(_2_4G_MOSI_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = _2_4G_MISO_PIN;
	GPIO_Init(_2_4G_MISO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: _2_4G_spi_configuration      
  Description	: 2.4G spi configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void _2_4G_spi_configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	
	SPI_I2S_DeInit(_2_4G_SPI);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(_2_4G_SPI, &SPI_InitStructure);
	SPI_Cmd(_2_4G_SPI, ENABLE);
}

/*******************************************************************************
  Function		: _2_4G_spi_txrx      
  Description	: _2_4G spi txrx
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void _2_4G_spi_txrx(u8 tx_data, u8 *rx_data)
{
	while(SPI_I2S_GetFlagStatus(_2_4G_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
	SPI_I2S_SendData(_2_4G_SPI, tx_data);

	while(SPI_I2S_GetFlagStatus(_2_4G_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}
	*rx_data = SPI_I2S_ReceiveData(_2_4G_SPI);
}

/*******************************************************************************
  Function		: _2_4G_spi_write_register      
  Description	: 2.4G spi write register
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void _2_4G_spi_write_register(u8 addr, u8 value)
{
	u8 temp = 0;

	addr &= 0x7F;

	_2_4G_RESET_CS();
	
	_2_4G_spi_txrx(addr, &temp);
	_2_4G_spi_txrx(value, &temp);

	_2_4G_SET_CS();
}

/*******************************************************************************
  Function		: _2_4G_spi_read_register      
  Description	: 2.4G spi_read_register
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void _2_4G_spi_read_register(u8 addr, u8 *ret_value)
{
	u8 temp = 0;

	addr |= 0x80;

	_2_4G_RESET_CS();
	
	_2_4G_spi_txrx(addr, &temp);

	temp = 0xFF;
	
	_2_4G_spi_txrx(temp, ret_value);

	_2_4G_SET_CS();
}

/*******************************************************************************
  Function		: _2_4G_init      
  Description	: 2.4G init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool _2_4G_init(void)
{
	u8 id = 0;
	
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
  Function		: get_2_4G_info     
  Description	: get_2_4G_info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_2_4G_info(void)
{

}

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
