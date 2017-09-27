/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	power.c
Author			:	klaus
Version			:	V1.0.0	
Date				:	2017/05/13
Description		:	power control  function
*******************************************************************************/

/* includes *******************************************************************/
#include "power.h"
/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_power g_power = {0};

/* funcitons ******************************************************************/

/*******************************************************************************
  Function		: power_control_init     
  Description	: power control init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void power_control_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(BLUETOOTH_POWER_CLK | VOLTAGE_GATHER_POWER_CLK | 
						   _2_4G_POWER_CLK	   | BORDER_POWER_CLK    | 
							MOTOR_POWER_CLK    | SYSTEM_POWER_CLK    | 
							PERIPHERAL_POWER_CONTROL_CLK, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = BLUETOOTH_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLUETOOTH_POWER_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = _2_4G_POWER_PIN;
	GPIO_Init(_2_4G_POWER_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BORDER_POWER_PIN;
	GPIO_Init(BORDER_POWER_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MOTOR_POWER_PIN;
	GPIO_Init(MOTOR_POWER_PORT, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = PERIPHERAL_POWER_CONTROL_PIN;
//	GPIO_Init(PERIPHERAL_POWER_CONTROL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = VOLTAGE_GATHER_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(VOLTAGE_GATHER_POWER_PORT, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = SYSTEM_POWER_PIN;
//	GPIO_Init(SYSTEM_POWER_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: enable_system_power     
  Description	: enable_system_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_system_power(void)
{
//	if(!g_power.power_system)
//	{
//		g_power.power_system = 1;
//		GPIO_WriteBit(SYSTEM_POWER_PORT, SYSTEM_POWER_PIN, Bit_SET);
//	}
}
/*******************************************************************************
  Function		: disable_system_power     
  Description	: disable_system_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_system_power(void)
{
//	if(g_power.power_system)
//	{
//		g_power.power_system = 0;
//		GPIO_WriteBit(SYSTEM_POWER_PORT, SYSTEM_POWER_PIN, Bit_RESET);
//	}
}
/*******************************************************************************
  Function		: enable_peripheral_power     
  Description	: enable_peripheral_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_peripheral_power(void)
{
//	if(!g_power.power_peripheral)
//	{
//		g_power.power_peripheral = 1;
//		GPIO_WriteBit(PERIPHERAL_POWER_CONTROL_PORT, PERIPHERAL_POWER_CONTROL_PIN, Bit_SET);
//	}
}
/*******************************************************************************
  Function		: disable_peripheral_power     
  Description	: disable_peripheral_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_peripheral_power(void)
{
//	if(g_power.power_peripheral)
//	{
//		g_power.power_peripheral = 0;
//		GPIO_WriteBit(PERIPHERAL_POWER_CONTROL_PORT, PERIPHERAL_POWER_CONTROL_PIN, Bit_RESET);
//	}
}
/*******************************************************************************
  Function		: enable_bluetooth_power     
  Description	: enable_bluetooth_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_bluetooth_power(void)
{
	if(!g_power.power_bluetooth)
	{
		g_power.power_bluetooth = 1;
		GPIO_WriteBit(BLUETOOTH_POWER_PORT, BLUETOOTH_POWER_PIN, Bit_RESET);
	}
}
/*******************************************************************************
  Function		: disable_bluetooth_power     
  Description	: disable_bluetooth_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_bluetooth_power(void)
{
	if(g_power.power_bluetooth)
	{
		g_power.power_bluetooth = 0;
		GPIO_WriteBit(BLUETOOTH_POWER_PORT, BLUETOOTH_POWER_PIN, Bit_SET);
	}
}

/*******************************************************************************
  Function		: enable_voltage_power     
  Description	: enable_voltage_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_voltage_power(void)
{
	if(!g_power.power_voltage_gather)
	{
		g_power.power_voltage_gather = 1;
		GPIO_WriteBit(VOLTAGE_GATHER_POWER_PORT, VOLTAGE_GATHER_POWER_PIN, Bit_SET);
	}
}
/*******************************************************************************
  Function		: disable_voltage_power     
  Description	: disable_voltage_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_voltage_power(void)
{
	if(g_power.power_voltage_gather)
	{
		g_power.power_voltage_gather = 0;
		GPIO_WriteBit(VOLTAGE_GATHER_POWER_PORT, VOLTAGE_GATHER_POWER_PIN, Bit_RESET);
	}
}

/*******************************************************************************
  Function		: enable_border_power     
  Description	: enable_border_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_border_power(void)
{
	if(!g_power.power_border)
	{
		g_power.power_border = 1;
		GPIO_WriteBit(BORDER_POWER_PORT, BORDER_POWER_PIN, Bit_RESET);
	}
}
/*******************************************************************************
  Function		: disable_border_power     
  Description	: disable_border_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_border_power(void)
{
	if(g_power.power_border)
	{
		g_power.power_border = 0;
		GPIO_WriteBit(BORDER_POWER_PORT, BORDER_POWER_PIN, Bit_SET);
	}
}

/*******************************************************************************
  Function		: enable_24g_power     
  Description	: enable_24g_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_24g_power(void)
{
	if(!g_power.power_24g)
	{
		g_power.power_24g = 1;
		GPIO_WriteBit(_2_4G_POWER_PORT, _2_4G_POWER_PIN, Bit_RESET);
	}
}
/*******************************************************************************
  Function		: disable_24g_power     
  Description	: disable_24g_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_24g_power(void)
{
	if(g_power.power_24g)
	{
		g_power.power_24g = 0;
		GPIO_WriteBit(_2_4G_POWER_PORT, _2_4G_POWER_PIN, Bit_SET);
	}
}

/*******************************************************************************
  Function		: enable_motor_power     
  Description	: enable_motor_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_motor_power(void)
{
	if(!g_power.power_motor)
	{
		g_power.power_motor = 1;
		GPIO_WriteBit(MOTOR_POWER_PORT, MOTOR_POWER_PIN, Bit_RESET);
	}
}
/*******************************************************************************
  Function		: disable_motor_power     
  Description	: disable_motor_power
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_motor_power(void)
{
	if(g_power.power_motor)
	{
		g_power.power_motor = 0;
		GPIO_WriteBit(MOTOR_POWER_PORT, MOTOR_POWER_PIN, Bit_SET);
	}
}
/*******************************************************************************
  Function		: reset_power_control     
  Description	: reset_power_control
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_power_control(void)
{
	disable_system_power();
	disable_peripheral_power();
	disable_bluetooth_power();
	disable_voltage_power();
	disable_border_power();
	disable_24g_power();
	disable_motor_power();
}

/*******************************************************************************
  Function		: get_power_control_infor     
  Description	: get_power_control_infor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_power_control_infor(T_power *power)
{
	power->power_system 	= g_power.power_system;
	power->power_peripheral = g_power.power_peripheral;
	power->power_24g 		= g_power.power_24g;
	power->power_bluetooth  = g_power.power_bluetooth;
	power->power_border		= g_power.power_border;
	power->power_motor      = g_power.power_motor;
	power->power_voltage_gather = g_power.power_voltage_gather;

	power->power_state = g_power.power_system << 6    | g_power.power_peripheral << 5 |
						 g_power.power_bluetooth << 4 | g_power.power_voltage_gather << 3|
						 g_power.power_border << 2    | g_power.power_24g << 1 |
						 g_power.power_motor << 0;

}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
