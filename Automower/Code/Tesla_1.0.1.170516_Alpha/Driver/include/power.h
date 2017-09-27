/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	main.h
Author			:	klaus
Version			:	V1.0.0	
Date				:	2017/05/13
Description		:	main function
*******************************************************************************/

#ifndef _POWER_H_
#define _POWER_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"



/* macros *********************************************************************/
#define	BLUETOOTH_POWER_PORT				GPIOA
#define	BLUETOOTH_POWER_PIN					GPIO_Pin_11
#define	BLUETOOTH_POWER_CLK					RCC_AHB1Periph_GPIOA

#define VOLTAGE_GATHER_POWER_PORT			GPIOB
#define VOLTAGE_GATHER_POWER_PIN			GPIO_Pin_9
#define VOLTAGE_GATHER_POWER_CLK			RCC_AHB1Periph_GPIOB

#define BORDER_POWER_PORT					GPIOC
#define BORDER_POWER_PIN					GPIO_Pin_13
#define BORDER_POWER_CLK					RCC_AHB1Periph_GPIOC				

#define _2_4G_POWER_PORT					GPIOF
#define _2_4G_POWER_PIN						GPIO_Pin_0
#define _2_4G_POWER_CLK						RCC_AHB1Periph_GPIOF

#define MOTOR_POWER_PORT					GPIOG
#define MOTOR_POWER_PIN						GPIO_Pin_11
#define MOTOR_POWER_CLK						RCC_AHB1Periph_GPIOG

#define SYSTEM_POWER_PORT					GPIOG
#define SYSTEM_POWER_PIN					GPIO_Pin_15
#define SYSTEM_POWER_CLK					RCC_AHB1Periph_GPIOG

#define	PERIPHERAL_POWER_CONTROL_PORT		GPIOI
#define	PERIPHERAL_POWER_CONTROL_PIN		GPIO_Pin_6
#define	PERIPHERAL_POWER_CONTROL_CLK		RCC_AHB1Periph_GPIOI

typedef struct
{
	u8 power_system;
	u8 power_peripheral;
	u8 power_bluetooth;
	u8 power_voltage_gather;
	u8 power_border;
	u8 power_24g;
	u8 power_motor;
	u8 power_state;
}T_power;

/* funcitons ******************************************************************/
void power_control_init(void);

void enable_system_power(void);
void disable_system_power(void);
void enable_peripheral_power(void);
void disable_peripheral_power(void);
void enable_bluetooth_power(void);
void disable_bluetooth_power(void);
void enable_voltage_power(void);
void disable_voltage_power(void);
void enable_border_power(void);
void disable_border_power(void);
void enable_24g_power(void);
void disable_24g_power(void);
void enable_motor_power(void);
void disable_motor_power(void);

void reset_power_control(void);
void get_power_control_infor(T_power *power);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
