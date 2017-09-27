/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: motor.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: motor driver 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/


/* includes *******************************************************************/
#include "encoder_mower.h"
#include "mow_motor.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_encoder_mower g_encoder_mower = {TRUE, 0, 0, 0, 0, 0, 0};

/* funcitons ******************************************************************/
static void encoder_mower_gpio_configuration(void);
static void encoder_mower_timer_configuration(void);
static void update_encoder_mower(void);


/*******************************************************************************
  Function		: encoder_mower_gpio_configuration      
  Description	: encoder_mower gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void encoder_mower_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(ENCODER_MOWER_CLK, ENABLE);

	GPIO_PinAFConfig(ENCODER_MOWER_PORT, ENCODER_MOWER_PINSOURCE, ENCODER_MOWER_AF);

	GPIO_InitStructure.GPIO_Pin = ENCODER_MOWER_PIN;  /* or in up */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ENCODER_MOWER_PORT, &GPIO_InitStructure);
}
/*******************************************************************************
  Function		: encoder_mower_timer_configuration      
  Description	: encoder_mower timer configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void encoder_mower_timer_configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(ENCODER_MOWER_TIM_CLK, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(ENCODER_MOWER_TIMER, &TIM_TimeBaseStructure);
	
	TIM_TIxExternalClockConfig(ENCODER_MOWER_TIMER,TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_Rising,0);
	TIM_CounterModeConfig(ENCODER_MOWER_TIMER,TIM_CounterMode_Up);    
	TIM_SetCounter(ENCODER_MOWER_TIMER, 0);
    TIM_ClearITPendingBit(ENCODER_MOWER_TIMER, TIM_IT_Update);
    TIM_Cmd(ENCODER_MOWER_TIMER, ENABLE);  
}

/*******************************************************************************
  Function		: encoder_mower_timer_configuration      
  Description	: encoder_mower timer configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void encoder_mower_init(void)
{
	encoder_mower_gpio_configuration();
	encoder_mower_timer_configuration();
	reset_encoder_mower();
}

/*******************************************************************************
  Function		: update_encoder_mower      
  Description	: update_encoder_mower
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void update_encoder_mower(void)
{
	T_mow_motor motor;
	get_mow_motor_info(&motor);
	
	if(g_encoder_mower.first_measure_flag == FALSE)
	{
		g_encoder_mower.mower_encoder = ENCODER_MOWER_TIMER->CNT;
		g_encoder_mower.encoder_mower_delta = g_encoder_mower.last_mower_encoder - g_encoder_mower.mower_encoder;
		
		if(g_encoder_mower.mower_encoder < 5535)
		{
			g_encoder_mower.encoder_mower_overflow--;
			g_encoder_mower.mower_encoder = 60000 + (g_encoder_mower.mower_encoder - 5535);
			ENCODER_MOWER_TIMER->CNT = g_encoder_mower.mower_encoder;
		}
		else if(g_encoder_mower.mower_encoder > 60000)
		{
			g_encoder_mower.encoder_mower_overflow++;
			g_encoder_mower.mower_encoder = 5535 + (g_encoder_mower.mower_encoder - 60000);
			ENCODER_MOWER_TIMER->CNT = g_encoder_mower.mower_encoder;
		}
		
		g_encoder_mower.last_mower_encoder = g_encoder_mower.mower_encoder;
		if(motor.mow_motor_direction == DIRECTION_CW)
		{
			g_encoder_mower.mower_speed = g_encoder_mower.encoder_mower_delta * ENCODER_MOWER_UPDATE_FREQUENCY;
			g_encoder_mower.encoder_mower_sum += g_encoder_mower.encoder_mower_delta;
		}
		else
		{
			g_encoder_mower.mower_speed = -g_encoder_mower.encoder_mower_delta * ENCODER_MOWER_UPDATE_FREQUENCY;
			g_encoder_mower.encoder_mower_sum -= g_encoder_mower.encoder_mower_delta;
		}
	}
	else
	{
		g_encoder_mower.first_measure_flag = FALSE;
		g_encoder_mower.mower_encoder = 27000;
		g_encoder_mower.last_mower_encoder = g_encoder_mower.mower_encoder;
		g_encoder_mower.encoder_mower_delta = g_encoder_mower.last_mower_encoder - g_encoder_mower.mower_encoder;
		g_encoder_mower.encoder_mower_sum = 0;
		g_encoder_mower.encoder_mower_overflow = 0;
		g_encoder_mower.mower_speed = 0;
		ENCODER_MOWER_TIMER->CNT = g_encoder_mower.mower_encoder;
	}
}

/*******************************************************************************
  Function		: get_left_motor_speed      
  Description	: get left motor speed
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_encoder_mower_speed(void)
{
	return g_encoder_mower.mower_speed;	
}

/*******************************************************************************
  Function		: get_left_encoder_mower      
  Description	: get left encoder_mower
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s64 get_encoder_mower_encoder(void)
{
	return g_encoder_mower.encoder_mower_sum;
}


/*******************************************************************************
  Function		: reset_encoder_mower      
  Description	: reset encoder_mower
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_encoder_mower(void)
{
	g_encoder_mower.first_measure_flag = TRUE;
	update_encoder_mower();
}
/*******************************************************************************
  Function		: get_encoder_mower_info      
  Description	: get encoder_mower info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_encoder_mower_info(T_encoder_mower *encoder_mower)
{
	update_encoder_mower();

	encoder_mower->first_measure_flag = g_encoder_mower.first_measure_flag;
	encoder_mower->encoder_mower_delta = g_encoder_mower.encoder_mower_delta;
	encoder_mower->encoder_mower_overflow = g_encoder_mower.encoder_mower_overflow;
	encoder_mower->encoder_mower_sum = g_encoder_mower.encoder_mower_sum ;
	encoder_mower->last_mower_encoder = g_encoder_mower.last_mower_encoder;
	encoder_mower->mower_encoder = g_encoder_mower.mower_encoder;
	encoder_mower->mower_speed = g_encoder_mower.mower_speed;
}
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
