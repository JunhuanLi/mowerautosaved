/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: battery.h
  Author		: klaus 	
  Version		: V1.0.0	  
  Date			: 2017/05/27
  Description	: battery 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

#ifndef __BATTERY_H__
#define __BATTERY_H__

/* includes *******************************************************************/
#include "stm32f4xx.h"

/* macros *********************************************************************/
#define BATTERY_CHARGING_PORT								GPIOD
#define	BATTERY_CHARGING_PIN								GPIO_Pin_3
#define	BATTERY_CHARGING_CLK								RCC_AHB1Periph_GPIOD

#define BATTERY_CHARGING_EXTI_PORTSOURCE					EXTI_PortSourceGPIOD
#define	BATTERY_CHARGING_EXTI_PINSOURCE						EXTI_PinSource3

#define BATTERY_VOLTAGE_FACTOR								(float)((200.0 + 7.5) / 7.5)

typedef struct
{
	s32 voltage;
	s32 temperature;
	u8 	charging;
	u8	charge_done;
	u8  charge_state;
}T_battery;


/* funcitons ******************************************************************/
void battery_init(void);
void enable_battery_interrupt(void);
void disable_battery_interrupt(void);
void update_battery(void);
void reset_battery(void);
void get_battery_info(T_battery *battery);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
