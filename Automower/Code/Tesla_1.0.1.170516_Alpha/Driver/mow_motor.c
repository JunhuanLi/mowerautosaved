/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: mow_motor.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: mow motor
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/


/* includes *******************************************************************/
#include "mow_motor.h"
#include "adc.h"
#include "math_custom.h"


/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_mow_motor g_mower = {FALSE,FALSE, FALSE, DIRECTION_CW, 0, 0};


/* funcitons ******************************************************************/
static void mow_motor_gpio_configuration(void);
static void mow_motor_pwm_configuration(void);

static void pwm_ctrl_enable(void);
static void pwm_ctrl_disable(void);

static s32 get_mow_motor_current(s32 value);
static void update_mow_motor_exception(void);

/*******************************************************************************
  Function		: mow_motor_gpio_configuration      
  Description	: mow_motor gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void mow_motor_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(MOW_MOTOR_PWM1_CLK	 | MOW_MOTOR_PWM2_CLK |
						   MOW_MOTOR_ENABLE_CLK  | MOW_MOTOR_BRAKE_CLK | 
						   MOW_MOTOR_EXCEPTION_CLK , ENABLE);

	GPIO_PinAFConfig(MOW_MOTOR_PWM1_PORT, MOW_MOTOR_PWM1_PINSOURCE, GPIO_AF_TIM5);
	GPIO_PinAFConfig(MOW_MOTOR_PWM2_PORT, MOW_MOTOR_PWM2_PINSOURCE, GPIO_AF_TIM5);


	/* pwm pins*/
	GPIO_InitStructure.GPIO_Pin = MOW_MOTOR_PWM1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOW_MOTOR_PWM1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOW_MOTOR_PWM2_PIN;
	GPIO_Init(MOW_MOTOR_PWM2_PORT, &GPIO_InitStructure);

	/* enable pin*/
	GPIO_InitStructure.GPIO_Pin = MOW_MOTOR_ENABLE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOW_MOTOR_ENABLE_PORT, &GPIO_InitStructure);

	/* brake pin*/
	GPIO_InitStructure.GPIO_Pin = MOW_MOTOR_BRAKE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOW_MOTOR_BRAKE_PORT, &GPIO_InitStructure);

	/* exception pin*/
	GPIO_InitStructure.GPIO_Pin = MOW_MOTOR_EXCEPTION_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOW_MOTOR_EXCEPTION_PORT, &GPIO_InitStructure);

	GPIO_WriteBit(MOW_MOTOR_ENABLE_PORT, MOW_MOTOR_ENABLE_PIN, Bit_SET);
}

/*******************************************************************************
  Function		: mow_motor_pwm_configuration      
  Description	: mow_motor pwm configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void mow_motor_pwm_configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = MOW_MOTOR_PWM_PRESCALE - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = MOW_MOTOR_PWM_PERIOD - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(MOW_MOTOR_TIMER, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC1Init(MOW_MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(MOW_MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_OC2Init(MOW_MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(MOW_MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_OC3Init(MOW_MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(MOW_MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_OC4Init(MOW_MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(MOW_MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_Cmd(MOW_MOTOR_TIMER, ENABLE);
}


/*******************************************************************************
  Function		: set_mow_motor_pwm      
  Description	: set mow_motor pwm
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void set_mow_motor_pwm(s32 mower_pwm)
{
	g_mower.mow_motor_direction = (mower_pwm > 0)? DIRECTION_CW : DIRECTION_CCW;
	g_mower.mow_motor_pwm = get_bound_data(custom_abs(mower_pwm), MOW_MOTOR_PWM_PERIOD);

	if(g_mower.mow_motor_direction == DIRECTION_CW)
	{
		MOW_MOTOR_TIMER->CCR3 = 0;
		MOW_MOTOR_TIMER->CCR4 =(u16)(g_mower.mow_motor_pwm);
	}
	else
	{
		MOW_MOTOR_TIMER->CCR3 =(u16)(g_mower.mow_motor_pwm);
		MOW_MOTOR_TIMER->CCR4 = 0;
	}

	enable_mow_motor();
}	

/*******************************************************************************
  Function		: pwm_ctrl_enable      
  Description	: pwm ctrl enable
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void pwm_ctrl_enable(void)
{
	TIM_CtrlPWMOutputs(MOW_MOTOR_TIMER, ENABLE);
	TIM_Cmd(MOW_MOTOR_TIMER, ENABLE);
}

/*******************************************************************************
  Function		: pwm_ctrl_disable      
  Description	: pwm ctrl disable
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void pwm_ctrl_disable(void)
{
	MOW_MOTOR_TIMER->CCR3 = 0;
	MOW_MOTOR_TIMER->CCR4 = 0;
	
	TIM_CtrlPWMOutputs(MOW_MOTOR_TIMER, DISABLE);
	TIM_Cmd(MOW_MOTOR_TIMER, DISABLE);
}

/*******************************************************************************
  Function		: get_mow_motor_current      
  Description	: get mow_motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static s32 get_mow_motor_current(s32 value)
{
	return (s32)(3300 * value / 4096);   //mA
}
/*******************************************************************************
  Function		: get_mow_motor_current      
  Description	: get mow_motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void mow_motor_brake(void)
{
	if(!g_mower.mow_motor_brake_state)
	{
		g_mower.mow_motor_brake_state = TRUE;
		GPIO_WriteBit(MOW_MOTOR_BRAKE_PORT, MOW_MOTOR_BRAKE_PIN, Bit_RESET);
	}
}

/*******************************************************************************
  Function		: mow_motor_brake_release      
  Description	: get mow_motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void mow_motor_brake_release(void)
{
	if(g_mower.mow_motor_brake_state)
	{
		g_mower.mow_motor_brake_state = FALSE;
		GPIO_WriteBit(MOW_MOTOR_BRAKE_PORT, MOW_MOTOR_BRAKE_PIN, Bit_SET);
	}
}


/*******************************************************************************
  Function		: mow_motor_init      
  Description	: mow_motor init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void mow_motor_init(void)
{
	mow_motor_gpio_configuration();
	mow_motor_pwm_configuration();
	disable_mow_motor();
}

/*******************************************************************************
  Function		: enable_mow_motor      
  Description	: enable mow_motor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_mow_motor(void)
{
	mow_motor_brake_release();

	if(g_mower.mow_motor_enable != TRUE)
	{
		GPIO_WriteBit(MOW_MOTOR_ENABLE_PORT, MOW_MOTOR_ENABLE_PIN, Bit_RESET);
		g_mower.mow_motor_enable = TRUE;
		pwm_ctrl_enable();
	}
}	

/*******************************************************************************
  Function		: disable_mow_motor      
  Description	: disable mow_motor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_mow_motor(void)
{
	if(g_mower.mow_motor_enable != FALSE)
	{
		g_mower.mow_motor_pwm = 0;
		g_mower.mow_motor_enable = FALSE;
		g_mower.mow_motor_direction = DIRECTION_CW;
		pwm_ctrl_disable();
		GPIO_WriteBit(MOW_MOTOR_ENABLE_PORT, MOW_MOTOR_ENABLE_PIN, Bit_SET);
	}
	mow_motor_brake();
}	

/*******************************************************************************
  Function		: update_mow_motor_current      
  Description	: update mow_motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void update_mow_motor_current(void)
{
	g_mower.mow_motor_current = get_mow_motor_current((s32)(get_adc_value(MOTOR_MOWER_CURRENT)));
}	
/*******************************************************************************
  Function		: update_mow_motor_current      
  Description	: update mow_motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_mow_motor_exception(void)
{
	g_mower.mow_motor_exception = (T_bool)(!GPIO_ReadInputDataBit(MOW_MOTOR_EXCEPTION_PORT, MOW_MOTOR_EXCEPTION_PIN));
}

/*******************************************************************************
  Function		: reset_mow_motor      
  Description	: reset mow_motor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_mow_motor(void)
{
	set_mow_motor_pwm(0);

	g_mower.mow_motor_current = 0;
}

/*******************************************************************************
  Function		: get_mow_motor_info      
  Description	: get mow_motor info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_mow_motor_info(T_mow_motor * mow_motor)
{
	update_mow_motor_current();
	update_mow_motor_exception();
	
	mow_motor->mow_motor_brake_state = g_mower.mow_motor_brake_state;

	mow_motor->mow_motor_current = g_mower.mow_motor_current;
	mow_motor->mow_motor_enable = g_mower.mow_motor_enable;
	mow_motor->mow_motor_pwm = g_mower.mow_motor_pwm;
	mow_motor->mow_motor_direction = g_mower.mow_motor_direction;
	mow_motor->mow_motor_exception = g_mower.mow_motor_exception;
	
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
