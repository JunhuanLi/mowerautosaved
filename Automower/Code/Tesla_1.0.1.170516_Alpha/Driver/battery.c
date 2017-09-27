/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: battery.c
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


/* includes *******************************************************************/
#include "battery.h"
#include "adc.h"
#include "math.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_battery g_battery = {0};

/* funcitons ******************************************************************/
static void battery_gpio_configuration(void);
static void battery_nvic_configuration(void);
static s32 get_battery_voltage(void);
static s32 get_battery_temperature(void);

/*------------------------------------------------------------------------------
Function		: battery_gpio_configuration
Description		: battery gpio configuration
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void battery_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(BATTERY_CHARGING_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BATTERY_CHARGING_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BATTERY_CHARGING_PORT, &GPIO_InitStructure);
}

/*------------------------------------------------------------------------------
Function		: battery_nvic_configuration
Description		: battery nvic configuration
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void battery_nvic_configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*------------------------------------------------------------------------------
Function		: enable_battery_interrupt
Description		: enable battery interrupt
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void enable_battery_interrupt(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	SYSCFG_EXTILineConfig(BATTERY_CHARGING_EXTI_PORTSOURCE, BATTERY_CHARGING_EXTI_PINSOURCE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*------------------------------------------------------------------------------
Function		: disable_battery_interrupt
Description		: disable battery interrupt
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void disable_battery_interrupt(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	SYSCFG_EXTILineConfig(BATTERY_CHARGING_EXTI_PORTSOURCE, BATTERY_CHARGING_EXTI_PINSOURCE);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*------------------------------------------------------------------------------
Function		: get_battery_voltage
Description		: get battery voltage
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static s32 get_battery_voltage(void)
{
	float voltage  = 0.0;
	u16 value = get_adc_value(BATTERY_VOLTAGE);
	voltage = 3.3 * BATTERY_VOLTAGE_FACTOR * value / 4096.0;
	return (s32)voltage;
}
/*------------------------------------------------------------------------------
Function		: get_battery_temperature
Description		: get battery temperature
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static s32 get_battery_temperature(void)
{
	float voltage = 0.0, bat_r = 0.0;
	float temp = 0.0, temp_ln = 0.0, ret = 0.0;
	float value = 0.0;

	voltage = (float)(3.3f * value / 4096.0);
	bat_r = 2.0 * 10.0 *voltage / (3.3 - voltage);
	temp = bat_r / 10.0;
	temp_ln = log(temp);
	ret = 298.15 * 3435.0 / ((298.15 *temp_ln) + 3435.0) - 237.15;
	return (s32)ret;
}

/*------------------------------------------------------------------------------
Function		: batteryInit
Description		: battery Init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void battery_init(void)
{
	battery_gpio_configuration();
	battery_nvic_configuration();
	disable_battery_interrupt();
}

/*------------------------------------------------------------------------------
Function		: updateBattery
Description		: update Battery
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void update_battery(void)
{
	g_battery.charging = (u8)(!GPIO_ReadInputDataBit(BATTERY_CHARGING_PORT, BATTERY_CHARGING_PIN));
	g_battery.voltage = g_battery.charging << 0;

	g_battery.voltage = get_battery_voltage();
	g_battery.temperature = get_battery_temperature();	
}

/*------------------------------------------------------------------------------
Function		: reset_battery
Description		: reset battery
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void reset_battery(void)
{
	g_battery.voltage = 0;
	g_battery.temperature = 0;
	g_battery.charge_done = 0;
	g_battery.charging = 0;
	g_battery.charge_state = 0;
}

/*------------------------------------------------------------------------------
Function		: get_battery_info
Description		: get battery info
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void get_battery_info(T_battery *battery)
{
	update_battery();

	battery->voltage = g_battery.voltage;
	battery->temperature = g_battery.temperature;
	battery->charge_done = g_battery.charge_done;
	battery->charging = g_battery.charging;
	battery->charge_state = g_battery.charge_state;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
