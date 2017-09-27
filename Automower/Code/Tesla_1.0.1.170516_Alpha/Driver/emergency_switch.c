 /*******************************************************************************
 
   Copyright (C), 2017-2027, TOPBAND Co., Ltd.
   File name	 : emergency_switch.c
   Author		 : klaus	 
   Version		 : V1.0.0	   
   Date 		 : 2017/05/17
   Description	 : emergency switch
   
   History: 	   
				   
	 1. Date		 :
		Author		 :
		Modification :
	 2. ...
	 
 *******************************************************************************/


/* includes *******************************************************************/
#include "emergency_switch.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_bool g_emergency_switch = FALSE;

/* funcitons ******************************************************************/
static void emergency_switch_gpio_configuration(void);
static void update_emergency_switch(void);


/*******************************************************************************
  Function		: emergency_switch_gpio_configuration      
  Description	: emergency switch gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void emergency_switch_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(EMERGENCY_SWITCH_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = EMERGENCY_SWITCH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(EMERGENCY_SWITCH_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: emergency_switch_init      
  Description	: emergency switch init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void emergency_switch_init(void)
{
	emergency_switch_gpio_configuration();
	reset_emergency_switch();
}

/*******************************************************************************
  Function		: update_emergency_switch      
  Description	: update emergency switch
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_emergency_switch(void)
{
	g_emergency_switch = (T_bool)(!GPIO_ReadInputDataBit(EMERGENCY_SWITCH_PORT, EMERGENCY_SWITCH_PIN));
}

/*******************************************************************************
  Function		: get_cover_switch_info      
  Description	: get cover switch info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_emergency_switch_info(T_bool * emergency_switch)
{
	update_emergency_switch();

	*emergency_switch = g_emergency_switch;
}

/*******************************************************************************
  Function		: reset_emergency_switch     
  Description	: reset emergency switch 
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_emergency_switch(void)
{
	g_emergency_switch = FALSE;
}

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
