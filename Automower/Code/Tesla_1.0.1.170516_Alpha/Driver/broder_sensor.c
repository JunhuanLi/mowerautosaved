/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	broder_sensor.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/23
Description		:	broder_sensor driiver
*******************************************************************************/

/* includes *******************************************************************/
#include "broder_sensor.h"
#include "adc.h"
/* macros *********************************************************************/


/* static variables ***********************************************************/
static T_broder_sensor g_broder_sensor = {0};
static T_broder_value g_broder_value = {0};


/* funcitons ******************************************************************/
static void update_broder_sensor_adc_value(void);
static void process_broder_sensor_adc_value(void);
static void select_broder_sensor_00(void);
static void select_broder_sensor_01(void);
static void select_broder_sensor_10(void);
static void select_broder_sensor_11(void);
static void disable_broder_sensor_selector(void);
static void enable_broder_sensor_selector(void);


/*******************************************************************************
  Function		: update_broder_sensor_adc_value     
  Description	: update broder sensor adc value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_broder_sensor_adc_value(void)
{
	g_broder_value.left = get_adc_value(BORDER_SENSOR_LEFT);
	g_broder_value.right = get_adc_value(BORDER_SENSOR_RIGHT);
}
/*******************************************************************************
  Function		: process_broder_sensor_adc_value     
  Description	: process broder sensor adc value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void process_broder_sensor_adc_value(void)
{

}
/*******************************************************************************
  Function		: broder_sensor_Init     
  Description	: broder_sensor_Init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void broder_sensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(BRODER_SELECT_LEFT_CLK | BRODER_SELECT_RIGHT_CLK | BRODER_SELECT_ENABLE_CLK,ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = BRODER_SELECT_LEFT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BRODER_SELECT_LEFT_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BRODER_SELECT_RIGHT_PIN;
	GPIO_Init(BRODER_SELECT_RIGHT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BRODER_SELECT_ENABLE_PIN;
	GPIO_Init(BRODER_SELECT_ENABLE_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: enable_broder_sensor_selector     
  Description	: enable_broder_sensor_selector  
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void enable_broder_sensor_selector(void)
{
	GPIO_ResetBits(BRODER_SELECT_ENABLE_PORT, BRODER_SELECT_ENABLE_PIN);
}

/*******************************************************************************
  Function		: disable_broder_sensor_selector     
  Description	: disable_broder_sensor_selector  
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void disable_broder_sensor_selector(void)
{
	GPIO_SetBits(BRODER_SELECT_ENABLE_PORT, BRODER_SELECT_ENABLE_PIN);
}


/*******************************************************************************
  Function		: select_broder_sensor_00     
  Description	: select_broder_sensor_00
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void select_broder_sensor_00(void)
{
	GPIO_ResetBits(BRODER_SELECT_LEFT_PORT, BRODER_SELECT_LEFT_PIN);
	GPIO_ResetBits(BRODER_SELECT_RIGHT_PORT, BRODER_SELECT_RIGHT_PIN);
}
/*******************************************************************************
  Function		: select_broder_sensor_01     
  Description	: select_broder_sensor_01
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void select_broder_sensor_01(void)
{
	GPIO_ResetBits(BRODER_SELECT_LEFT_PORT, BRODER_SELECT_LEFT_PIN);
	GPIO_SetBits(BRODER_SELECT_RIGHT_PORT, BRODER_SELECT_RIGHT_PIN);
}
/*******************************************************************************
  Function		: select_broder_sensor_10     
  Description	: select_broder_sensor_10
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/

static void select_broder_sensor_10(void)
{
	GPIO_SetBits(BRODER_SELECT_LEFT_PORT, BRODER_SELECT_LEFT_PIN);
	GPIO_ResetBits(BRODER_SELECT_RIGHT_PORT, BRODER_SELECT_RIGHT_PIN);
}
/*******************************************************************************
  Function		: select_broder_sensor_11     
  Description	: select_broder_sensor_11
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void select_broder_sensor_11(void)
{
	GPIO_SetBits(BRODER_SELECT_LEFT_PORT, BRODER_SELECT_LEFT_PIN);
	GPIO_SetBits(BRODER_SELECT_RIGHT_PORT, BRODER_SELECT_RIGHT_PIN);
}


/*******************************************************************************
  Function		: reset_broder_sensor     
  Description	: reset broder sensor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_broder_sensor(void)
{
	g_broder_sensor.left = 0;
	g_broder_sensor.right = 0;
	g_broder_sensor.state = 0;
}
/*******************************************************************************
  Function		: get_broder_sensor_info     
  Description	: get broder sensor info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_broder_sensor_info(T_broder_sensor * broder_sensor)
{
	update_broder_sensor_adc_value();

	process_broder_sensor_adc_value();

	broder_sensor->left = g_broder_sensor.left;
	broder_sensor->right = g_broder_sensor.right;
	broder_sensor->state = g_broder_sensor.state;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
