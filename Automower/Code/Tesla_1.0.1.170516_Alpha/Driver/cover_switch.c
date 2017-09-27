 /*******************************************************************************
 
   Copyright (C), 2017-2027, TOPBAND Co., Ltd.
   File name	 : cover_switch.c
   Author		 : klaus	 
   Version		 : V1.0.0	   
   Date 		 : 2017/05/17
   Description	 : cover switch 
   
   History: 	   
				   
	 1. Date		 :
		Author		 :
		Modification :
	 2. ...
	 
 *******************************************************************************/


/* includes *******************************************************************/
#include "cover_switch.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_bool g_cover_switch = FALSE;

/* funcitons ******************************************************************/
static void cover_switch_gpio_configuration(void);
static void update_cover_switch(void);


/*******************************************************************************
  Function		: cover_switch_gpio_configuration      
  Description	: cover switch gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void cover_switch_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(COVER_SWITCH_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = COVER_SWITCH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COVER_SWITCH_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: cover_switch_init      
  Description	: cover switch init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void cover_switch_init(void)
{
	cover_switch_gpio_configuration();
	reset_cover_switch();
}

/*******************************************************************************
  Function		: update_cover_switch      
  Description	: update cover switch
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_cover_switch(void)
{
	g_cover_switch = (T_bool)(!GPIO_ReadInputDataBit(COVER_SWITCH_PORT, COVER_SWITCH_PIN));
}

/*******************************************************************************
  Function		: get_cover_switch_info      
  Description	: get cover switch info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_cover_switch_info(T_bool * cover_switch)
{
	update_cover_switch();

	*cover_switch = g_cover_switch;
}

/*******************************************************************************
  Function		: reset_cover_switch     
  Description	: reset cover switch 
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_cover_switch(void)
{
	g_cover_switch = FALSE;
}

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
