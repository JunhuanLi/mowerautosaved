 /*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: bumper_front.c
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: bumper front   
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

/* includes *******************************************************************/
#include "bumper_front.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_frontBumper g_front_bumper = {0};

/* funcitons ******************************************************************/
static void front_bumper_gpio_configuration(void);
static void update_front_bumper(void);


/*******************************************************************************
  Function		: front_bumper_GPIO_configuration      
  Description	: front bumper GPIO configuration 
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void front_bumper_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(FRONT_BUMPER_LEFT_CLK | FRONT_BUMPER_RIGHT_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = FRONT_BUMPER_LEFT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FRONT_BUMPER_LEFT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FRONT_BUMPER_RIGHT_PIN;
	GPIO_Init(FRONT_BUMPER_RIGHT_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
  Function		: front_bumper_init      
  Description	: front bumper init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void front_bumper_init(void)
{
	front_bumper_gpio_configuration();
	reset_front_bumper();
}

/*******************************************************************************
  Function		: update_front_bumper      
  Description	: update front bumper 
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_front_bumper(void)
{
	g_front_bumper.left = (u8)(!GPIO_ReadInputDataBit(FRONT_BUMPER_LEFT_PORT, FRONT_BUMPER_LEFT_PIN));
	g_front_bumper.right = (u8)(!GPIO_ReadInputDataBit(FRONT_BUMPER_RIGHT_PORT, FRONT_BUMPER_RIGHT_PIN));

	g_front_bumper.bumper_state = g_front_bumper.left << 1 | g_front_bumper.right << 0;
}

/*******************************************************************************
  Function		: get_front_bumper_info      
  Description	: get front bumper info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_front_bumper_info(T_frontBumper *front_bumper)
{
	update_front_bumper();

	front_bumper->left = g_front_bumper.left;
	front_bumper->right = g_front_bumper.right;
	front_bumper->bumper_state = g_front_bumper.bumper_state;
}

/*******************************************************************************
  Function		: reset_front_bumper      
  Description	: reset front bumper
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_front_bumper(void)
{
	g_front_bumper.left = 0;
	g_front_bumper.right = 0;
	g_front_bumper.bumper_state = 0;
}

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
