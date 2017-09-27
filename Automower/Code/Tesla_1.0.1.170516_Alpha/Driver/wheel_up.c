 /*******************************************************************************
 
   Copyright (C), 2017-2027, TOPBAND Co., Ltd.
   File name	 : wheel_up.c
   Author		 : klaus	 
   Version		 : V1.0.0	   
   Date 		 : 2017/05/17
   Description	 : wheel up  
   
   History: 	   
				   
	 1. Date		 :
		Author		 :
		Modification :
	 2. ...
	 
 *******************************************************************************/


/* includes *******************************************************************/
#include "wheel_up.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_wheel_up g_wheel_up = {0};

/* funcitons ******************************************************************/
static void wheel_up_gpio_configuration(void);
static void update_wheel_up(void);


/*******************************************************************************
  Function		: wheel_up_gpio_configuration      
  Description	: wheel up gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void wheel_up_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(WHEEL_UP_LEFT_CLK | WHEEL_UP_RIGHT_CLK, ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = WHEEL_UP_LEFT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WHEEL_UP_LEFT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = WHEEL_UP_RIGHT_PIN;
	GPIO_Init(WHEEL_UP_RIGHT_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
  Function		: wheel_up_init      
  Description	: wheel up init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void wheel_up_init(void)
{
	wheel_up_gpio_configuration();
	reset_wheel_up();
}

/*******************************************************************************
  Function		: update_wheel_up      
  Description	: update wheel up
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_wheel_up(void)
{
	g_wheel_up.left = (u8)(!GPIO_ReadInputDataBit(WHEEL_UP_LEFT_PORT, WHEEL_UP_LEFT_PIN));
	g_wheel_up.right = (u8)(!GPIO_ReadInputDataBit(WHEEL_UP_RIGHT_PORT, WHEEL_UP_RIGHT_PIN));

	g_wheel_up.state = g_wheel_up.left << 1 | g_wheel_up.right << 0;
}

/*******************************************************************************
  Function		: get_wheel_up_info      
  Description	: get wheel up info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_wheel_up_info(T_wheel_up * wheel_up)
{
	update_wheel_up();

	wheel_up->left = g_wheel_up.left;
	wheel_up->right = g_wheel_up.right;
	wheel_up->state = g_wheel_up.state;
}

/*******************************************************************************
  Function		: reset_wheel_up      
  Description	: reset wheel up
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_wheel_up(void)
{
	g_wheel_up.left = 0;
	g_wheel_up.right = 0;
	g_wheel_up.state = 0;
}

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
