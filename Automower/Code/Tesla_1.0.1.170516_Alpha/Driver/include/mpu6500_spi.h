/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: mpu6500_spi.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: mpu6500 spi driver 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _MPU6500_SPI_H_
#define _MPU6500_SPI_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define SMPLRT_DIV			0x19
#define	CONFIG				0x1A
#define GYRO_CONFIG			0x1B
#define ACCEL_CONFIG		0x1C
#define ACCEL_XOUT_H		0x3B
#define ACCEL_XOUT_L		0x3C
#define ACCEL_YOUT_H		0x3D
#define ACCEL_YOUT_L		0x3E
#define ACCEL_ZOUT_H		0x3F
#define ACCEL_ZOUT_L		0x40
#define TEMP_OUT_H			0x41
#define TEMP_OUT_L			0x42
#define GYRO_XOUT_H			0x43
#define GYRO_XOUT_L			0x44
#define GYRO_YOUT_H			0x45
#define GYRO_YOUT_L			0x46
#define GYRO_ZOUT_H			0x47
#define GYRO_ZOUT_L			0x48
#define PWR_MGMT_1			0x6B	
#define WHO_AM_I			0x75
#define SLAVE_ADDRESS		0xD0
#define MPU6500_SPI			SPI4
#define MPU6500_SPI_CLK		RCC_APB2Periph_SPI4


#define MPU6500_CS_PORT		GPIOE		
#define MPU6500_CS_PIN		GPIO_Pin_4
#define MPU6500_CS_SOURCE	GPIO_PinSource4
#define MPU6500_CS_CLK		RCC_AHB1Periph_GPIOE

#define MPU6500_MOSI_PORT	GPIOE		
#define MPU6500_MOSI_PIN	GPIO_Pin_6
#define MPU6500_MOSI_SOURCE	GPIO_PinSource6
#define MPU6500_MOSI_CLK	RCC_AHB1Periph_GPIOE

#define MPU6500_MISO_PORT	GPIOE		
#define MPU6500_MISO_PIN	GPIO_Pin_5
#define MPU6500_MISO_SOURCE	GPIO_PinSource5
#define MPU6500_MISO_CLK	RCC_AHB1Periph_GPIOE

#define MPU6500_CLK_PORT	GPIOE		
#define MPU6500_CLK_PIN		GPIO_Pin_2
#define MPU6500_CLK_SOURCE	GPIO_PinSource2
#define MPU6500_CLK_CLK		RCC_AHB1Periph_GPIOE

#define MPU6500_SPI_AF		GPIO_AF_SPI4

#define MPU6500_SET_CS()	GPIO_SetBits(MPU6500_CS_PORT, MPU6500_CS_PIN)		
#define MPU6500_RESET_CS()	GPIO_ResetBits(MPU6500_CS_PORT, MPU6500_CS_PIN)




/* funcitons ******************************************************************/
T_bool mpu_init(void);
void get_mpu_info(T_mpu *mpu);

#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
