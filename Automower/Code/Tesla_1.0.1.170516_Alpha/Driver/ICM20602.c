#define __ICM20602_C__
#include "ICM20602.h"
#include "time_control.h"

#define SPI_20602 		SPI4
#define GPIO_CS_PORT	GPIOI
#define GPIO_CS_BIT		GPIO_Pin_5

void ICM20602_CS_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
}


unsigned char SPI_ReadWriteByte_20602(unsigned char TxData)
{
	unsigned char retry = 0;
	
	while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET)
	{
		 retry ++;
		 if(retry > 200)
		 {
			return 0;
		 }
	}
	
	SPI_I2S_SendData(SPI4, TxData);
	
	retry = 0;
	
	while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET)
	{
		 retry ++;
		 if(retry > 200)
		 {
			return 0;
		 }
	}
	
	return SPI_I2S_ReceiveData(SPI4);
}
unsigned char ICM20602_write_reg(unsigned char reg, unsigned char val)
{
	unsigned  char status;
	
	GPIO_ResetBits(GPIOI, GPIO_Pin_5);
	
	status = SPI_ReadWriteByte_20602(reg);
	SPI_ReadWriteByte_20602(val);
	
	GPIO_SetBits(GPIOI, GPIO_Pin_5);
	
	return (status);
}


unsigned char ICM20602_read_reg(unsigned char reg)
{
	unsigned char reg_val;
	
	GPIO_ResetBits(GPIOI, GPIO_Pin_5);
	
	SPI_ReadWriteByte_20602(reg + 0x80);
	reg_val = SPI_ReadWriteByte_20602(0xFF);
	
	GPIO_SetBits(GPIOI, GPIO_Pin_5);
	
	return(reg_val);
}

unsigned char ICM20602_ID_GET(void)
{
	return ICM20602_read_reg(0x75);
}

unsigned char val_test = 0;
void ICM20602_Init(void)
{

	ICM20602_CS_Config();

	delay_ms(20);
	
	val_test = ICM20602_write_reg(0x6B,0x00);//wake up on internal 8MHz
	
	val_test = ICM20602_write_reg(0x6C,0x00);//enable gyros
	
	delay_ms(10);
	
	val_test = ICM20602_write_reg(0x6B,0x01);//switch clock to gyro z aixs
	
	delay_ms(100);
	
	val_test = ICM20602_write_reg(0x6B,0x03);//wakeup
		

	val_test = ICM20602_write_reg( 0x19, 0x07);//125Hz
	
	val_test = ICM20602_write_reg(0x1A,0x02);//94Hz


	val_test = ICM20602_write_reg(0x1B,0x18);//+-2000deg/s

	val_test = ICM20602_write_reg(0x1C,0x08);//+-4G ;cutoff 5Hz


	val_test = ICM20602_write_reg(0x6A,0x00);
	
	val_test = ICM20602_write_reg(0x6B,0x03);
	
	delay_ms(100);


}


void ICM20602_getMotion6(T_mpu_20602 *mpu)
{
	ICM20602_TYPE ICM20602_GYRO_LAST;
	ICM20602_TYPE ICM20602_ACC_LAST;  
	
	ICM20602_GYRO_LAST.X=((((int16_t)ICM20602_read_reg(ICM20602_GYRO_X_H)) << 8) | ICM20602_read_reg(ICM20602_GYRO_X_L)) ;
	ICM20602_GYRO_LAST.Y=((((int16_t)ICM20602_read_reg(ICM20602_GYRO_Y_H)) << 8) | ICM20602_read_reg(ICM20602_GYRO_Y_L)) ;
	ICM20602_GYRO_LAST.Z=((((int16_t)ICM20602_read_reg(ICM20602_GYRO_Z_H)) << 8) | ICM20602_read_reg(ICM20602_GYRO_Z_L)) ;
	
	ICM20602_ACC_LAST.X=((((int16_t)ICM20602_read_reg(ICM20602_ACC_X_H)) << 8)   | ICM20602_read_reg(ICM20602_ACC_X_L)) ;
	ICM20602_ACC_LAST.Y=((((int16_t)ICM20602_read_reg(ICM20602_ACC_Y_H)) << 8)   | ICM20602_read_reg(ICM20602_ACC_Y_L)) ;
	ICM20602_ACC_LAST.Z=((((int16_t)ICM20602_read_reg(ICM20602_ACC_Z_H)) << 8)   | ICM20602_read_reg(ICM20602_ACC_Z_L)) ;

	mpu->ax = ICM20602_ACC_LAST.X;
	mpu->ay = ICM20602_ACC_LAST.Y;
	mpu->az = ICM20602_ACC_LAST.Z;
	
	mpu->gx = ICM20602_GYRO_LAST.X;
	mpu->gy = ICM20602_GYRO_LAST.Y;
	mpu->gz = ICM20602_GYRO_LAST.Z;

}

void SPI4_Slave_Init(void)
{
	SPI_InitTypeDef 	SPI_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6);
	
	
	SPI_InitStructure.SPI_Direction     = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode          = SPI_Mode_Slave;
	SPI_InitStructure.SPI_DataSize      = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL          = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA          = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS           = SPI_NSS_Hard;
	SPI_InitStructure.SPI_FirstBit      = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI4, &SPI_InitStructure);
	SPI_Cmd(SPI4, ENABLE);
}

void SPI4_Master_Init(void)
{
	SPI_InitTypeDef 	SPI_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE, GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6);
	
	//GPIO_PinAFConfig(GPIOE, GPIO_Pin_2, GPIO_AF_SPI4);
	//GPIO_PinAFConfig(GPIOE, GPIO_Pin_5, GPIO_AF_SPI4);
	//GPIO_PinAFConfig(GPIOE, GPIO_Pin_6, GPIO_AF_SPI4);	
	
	SPI_I2S_DeInit(SPI4);
	
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_Init(SPI4, &SPI_InitStructure);
	SPI_Cmd(SPI4, ENABLE);
	
	delay_ms(100);

}

void ICM20602_SPI_TX_RX(unsigned char tx_data, unsigned char *rx_data)
{
		while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
	SPI_I2S_SendData(SPI4, tx_data);

	while(SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}
	*rx_data = SPI_I2S_ReceiveData(SPI4);
}

void ICM20602_SPI_WRITE_REG(u8 addr, u8 value)
{
	u8 temp = 0;

	addr &= 0x7F;

	GPIO_ResetBits(GPIOI, GPIO_Pin_5);
	
	ICM20602_SPI_TX_RX(addr, &temp);
	ICM20602_SPI_TX_RX(value, &temp);

	GPIO_SetBits(GPIOI, GPIO_Pin_5);

}

void ICM20602_SPI_READ_REG(u8 addr, u8 *ret_value)
{
	u8 temp = 0;

	addr |= 0x80;

	GPIO_ResetBits(GPIOI, GPIO_Pin_5);
	
	ICM20602_SPI_TX_RX(addr, &temp);

	temp = 0xFF;
	
	ICM20602_SPI_TX_RX(temp, ret_value);

	GPIO_ResetBits(GPIOI, GPIO_Pin_5);
}