/*******************************************************************************

Copyright (C), 2017, TOPBAND Robot Team
File name		:	sonar.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/23
Description		:	ist8310 driiver
*******************************************************************************/

/* includes *******************************************************************/
#include "sonar.h"
#include "time_control.h"
#include "rtthread.h"

/* macros *********************************************************************/
typedef struct
{
	T_bool is_left_sonar_first_measure;
	T_bool is_right_sonar_first_measure;
	u64 left_start_time;
	u64 left_end_time;
	u64 left_diff_time;
	u64 right_start_time;
	u64 right_end_time;
	u64 right_diff_time;
}T_sonar_structure;

/* static variables ***********************************************************/
static T_sonar g_sonar = {0};
static T_sonar_structure g_sonar_structure= {TRUE, TRUE, 0, 0, 0, 0, 0, 0};

/* funcitons ******************************************************************/
static void sonar_gpio_configuration(void);
static void sonar_exti_configuration(void);

/*******************************************************************************
  Function		: sonar_gpio_configuration     
  Description	: sonar gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void sonar_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LEFT_SONAR_TX_CLK | LEFT_SONAR_RX_CLK |
						   RIGHT_SONAR_TX_CLK | RIGHT_SONAR_RX_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LEFT_SONAR_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LEFT_SONAR_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RIGHT_SONAR_TX_PIN;
	GPIO_Init(RIGHT_SONAR_TX_PORT, &GPIO_InitStructure);
						   

	GPIO_InitStructure.GPIO_Pin = LEFT_SONAR_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LEFT_SONAR_RX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RIGHT_SONAR_RX_PIN;
	GPIO_Init(RIGHT_SONAR_RX_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: sonar_exti_configuration     
  Description	: sonar exti configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void sonar_exti_configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SYSCFG_EXTILineConfig(LEFT_SONAR_RX_PORTSOURCE, LEFT_SONAR_RX_PINSOURCE);
	SYSCFG_EXTILineConfig(RIGHT_SONAR_RX_PORTSOURCE, RIGHT_SONAR_RX_PINSOURCE);

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*******************************************************************************
  Function		: sonar_init     
  Description	: sonar init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void sonar_init(void)
{
	sonar_gpio_configuration();
	sonar_exti_configuration() ;
}
/*******************************************************************************
  Function		: sonar_launch     
  Description	: sonar lauch
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void sonar_launch(void)
{
	rt_enter_critical();

	LEFT_SONAR_TX_OPEN();
	RIGHT_SONAR_TX_OPEN();

	delay_us(6);

	LEFT_SONAR_TX_CLOSE();
	RIGHT_SONAR_TX_CLOSE();
	delay_us(50);
	
	rt_exit_critical();
}
/*******************************************************************************
  Function		: process_left_sonar_rising    
  Description	: process left sonar rising
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void process_left_sonar_rising(void)
{
	g_sonar_structure.left_start_time = get_time_stamp();
	g_sonar_structure.left_end_time = 0;
}
/*******************************************************************************
  Function		: process_right_sonar_rising   
  Description	: process left sonar falling
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void process_right_sonar_rising(void)
{
	g_sonar_structure.right_start_time= get_time_stamp();
	g_sonar_structure.right_end_time = 0;
}
/*******************************************************************************
  Function		: process_left_sonar_falling     
  Description	: process left sonar falling
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void process_left_sonar_falling(void)
{
	u64 time = 0;
	g_sonar_structure.left_end_time = get_time_stamp();
	g_sonar_structure.left_diff_time = g_sonar_structure.left_end_time - g_sonar_structure.left_start_time ;
	g_sonar.left = VELOVITY_OF_SPEED / 2 * g_sonar_structure.left_diff_time / 1000;   //mm
	if(g_sonar.left < MIN_SONAR_RANGE || g_sonar.left > MAX_SONAR_RANGE || g_sonar_structure.is_left_sonar_first_measure)
	{
		g_sonar_structure.is_left_sonar_first_measure = FALSE;
		g_sonar.left = SONAR_ERROR;
	}
}
/*******************************************************************************
  Function		: process_right_sonar_falling     
  Description	: process right sonar falling
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void process_right_sonar_falling(void)
{
	g_sonar_structure.right_end_time = get_time_stamp();
	g_sonar_structure.right_diff_time = g_sonar_structure.right_end_time - g_sonar_structure.right_start_time;
	g_sonar.right = VELOVITY_OF_SPEED / 2 * g_sonar_structure.right_diff_time / 1000; // mm
	if(g_sonar.right < MIN_SONAR_RANGE || g_sonar.right > MAX_SONAR_RANGE || g_sonar_structure.is_right_sonar_first_measure )
	{
		g_sonar_structure.is_right_sonar_first_measure = FALSE;
		g_sonar.right = SONAR_ERROR;
	}
}
/*******************************************************************************
  Function		: process_left_sonar     
  Description	: process_left_sonar
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void process_left_sonar(void)
{
	if(GPIO_ReadInputDataBit(LEFT_SONAR_RX_PORT, LEFT_SONAR_RX_PIN))
	{
		process_left_sonar_rising();
	}
	else if(!GPIO_ReadInputDataBit(LEFT_SONAR_RX_PORT, LEFT_SONAR_RX_PIN))
	{
		process_left_sonar_falling();
	}
}
/*******************************************************************************
  Function		: process_right_sonar     
  Description	: process_right_sonar
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void process_right_sonar(void)
{
	if(GPIO_ReadInputDataBit(RIGHT_SONAR_RX_PORT, RIGHT_SONAR_RX_PIN))
	{
		process_right_sonar_rising();
	}
	else if(!GPIO_ReadInputDataBit(RIGHT_SONAR_RX_PORT, RIGHT_SONAR_RX_PIN))
	{
		process_right_sonar_falling();
	}
}
/*------------------------------------------------------------------------------
Function		:	get_sonar_info
Description		:	get sonar info
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void get_sonar_info(T_sonar * sonar)
{
	sonar->left = g_sonar.left;
	sonar->right = g_sonar.right;
	
	g_sonar_structure.left_start_time = 0;
	g_sonar_structure.left_end_time = 0;
	g_sonar_structure.left_diff_time = 0;
	
	g_sonar_structure.right_start_time = 0;
	g_sonar_structure.right_end_time = 0;
	g_sonar_structure.right_diff_time = 0;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
