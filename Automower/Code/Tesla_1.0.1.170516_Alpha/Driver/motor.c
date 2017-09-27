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
#include "motor.h"
#include "adc.h"
#include "math_custom.h"


#include "usart_driver.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_motor g_motor = {FALSE,FALSE, FALSE, FALSE, FALSE, DIRECTION_CW, DIRECTION_CW, 0, 0, 0, 0};


/* funcitons ******************************************************************/
static void motor_gpio_configuration(void);
static void motor_pwm_configuration(void);

static void pwm_ctrl_enable(void);
static void pwm_ctrl_disable(void);

static void set_left_motor_pwm(void);
static void set_right_motor_pwm(void);
static s32 get_motor_current(s32 value);
static void update_motor_exception(void);

/*******************************************************************************
  Function		: motor_gpio_configuration      
  Description	: motor gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void motor_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(MOTOR_PWM1_LEFT_CLK 		 | MOTOR_PWM2_LEFT_CLK |
						   MOTOR_PWM1_RIGHT_CLK 	 | MOTOR_PWM2_RIGHT_CLK | 
						   MOTOR_ENABLE_CLK 		 | MOTOR_BRAKE_CLK | 
						   MOTOR_EXCEPTION_LEFT_CLK  | MOTOR_EXCEPTION_RIGHT_CLK, ENABLE);

	GPIO_PinAFConfig(MOTOR_PWM1_LEFT_PORT, MOTOR_PWM1_LEFT_PINSOURCE, GPIO_AF_TIM8);
	GPIO_PinAFConfig(MOTOR_PWM2_LEFT_PORT, MOTOR_PWM2_LEFT_PINSOURCE, GPIO_AF_TIM8);
	GPIO_PinAFConfig(MOTOR_PWM1_RIGHT_PORT, MOTOR_PWM1_RIGHT_PINSOURCE, GPIO_AF_TIM8);
	GPIO_PinAFConfig(MOTOR_PWM2_RIGHT_PORT, MOTOR_PWM2_RIGHT_PINSOURCE, GPIO_AF_TIM8);

	/* pwm pins*/
	GPIO_InitStructure.GPIO_Pin = MOTOR_PWM1_LEFT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_PWM1_LEFT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOTOR_PWM2_LEFT_PIN;
	GPIO_Init(MOTOR_PWM2_LEFT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOTOR_PWM1_RIGHT_PIN;
	GPIO_Init(MOTOR_PWM1_RIGHT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOTOR_PWM2_RIGHT_PIN;
	GPIO_Init(MOTOR_PWM2_RIGHT_PORT, &GPIO_InitStructure);


	/* enable pin*/
	GPIO_InitStructure.GPIO_Pin = MOTOR_ENABLE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_ENABLE_PORT, &GPIO_InitStructure);

	/* brake pin*/
	GPIO_InitStructure.GPIO_Pin = MOTOR_BRAKE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_BRAKE_PORT, &GPIO_InitStructure);


	/* exception pin*/
	GPIO_InitStructure.GPIO_Pin = MOTOR_EXCEPTION_LEFT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_EXCEPTION_LEFT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOTOR_EXCEPTION_RIGHT_PIN;
	GPIO_Init(MOTOR_EXCEPTION_RIGHT_PORT, &GPIO_InitStructure);


	GPIO_WriteBit(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, Bit_SET);
	GPIO_WriteBit(MOTOR_BRAKE_PORT, MOTOR_BRAKE_PIN, Bit_SET);
}
/*******************************************************************************
  Function		: motor_pwm_configuration      
  Description	: motor pwm configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void motor_pwm_configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = MOTOR_PWM_PRESCALE - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = MOTOR_PWM_PERIOD - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(MOTOR_TIMER, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	
	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC1Init(MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_OC2Init(MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_OC3Init(MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_OC4Init(MOTOR_TIMER, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(MOTOR_TIMER, TIM_OCPreload_Enable);

	TIM_Cmd(MOTOR_TIMER, ENABLE);
}


/*******************************************************************************
  Function		: set_motor_pwm      
  Description	: set motor pwm
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void set_motor_pwm(s32 left_pwm, s32 right_pwm)
{
	g_motor.motor_left_direction = (left_pwm >= 0)? DIRECTION_CW : DIRECTION_CCW;
	g_motor.motor_right_direction = (right_pwm >= 0)? DIRECTION_CCW : DIRECTION_CW;

	g_motor.motor_left_pwm = get_bound_data(custom_abs(left_pwm), MOTOR_PWM_PERIOD);
	g_motor.motor_right_pwm = get_bound_data(custom_abs(right_pwm), MOTOR_PWM_PERIOD);

	set_left_motor_pwm();
	set_right_motor_pwm();

	enable_motor();
}	
/*******************************************************************************
  Function		: set_left_motor_pwm      
  Description	: set left motor pwm
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void set_left_motor_pwm(void)
{
	if(g_motor.motor_left_direction == DIRECTION_CW)
	{
//		usart_send_byte(USART_DEBUG, 'x');
//		usart_send_byte(USART_DEBUG, '\n');
		MOTOR_TIMER->CCR1 = (u16)(g_motor.motor_left_pwm);
		MOTOR_TIMER->CCR2 = 0;
	}
	else
	{
//		usart_send_byte(USART_DEBUG, 'b');
//		usart_send_byte(USART_DEBUG, '\n');
		MOTOR_TIMER->CCR1 = (u16)(g_motor.motor_left_pwm);
		MOTOR_TIMER->CCR2 = MOTOR_PWM_PERIOD;
	}
}
/*******************************************************************************
  Function		: set_right_motor_pwm      
  Description	: set right motor pwm
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void set_right_motor_pwm(void)
{
	if(g_motor.motor_right_direction == DIRECTION_CW)
	{
		MOTOR_TIMER->CCR3 = (u16)(g_motor.motor_right_pwm);
		MOTOR_TIMER->CCR4 = 0;
	}
	else
	{
		MOTOR_TIMER->CCR3 =(u16)(g_motor.motor_right_pwm);
		MOTOR_TIMER->CCR4 = MOTOR_PWM_PERIOD;
	}
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
	TIM_CtrlPWMOutputs(MOTOR_TIMER, ENABLE);
	TIM_Cmd(MOTOR_TIMER, ENABLE);
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
	MOTOR_TIMER->CCR1 = 0;
	MOTOR_TIMER->CCR2 = 0;
	
	TIM_CtrlPWMOutputs(MOTOR_TIMER, DISABLE);
	TIM_Cmd(MOTOR_TIMER, DISABLE);
}

/*******************************************************************************
  Function		: get_motor_current      
  Description	: get motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static s32 get_motor_current(s32 value)
{
	return (s32)(3300 * value / 4096);   //mA
}
/*******************************************************************************
  Function		: get_motor_current      
  Description	: get motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void motor_brake(void)
{
	if(!g_motor.motor_brake_state)
	{
		g_motor.motor_brake_state = TRUE;
		GPIO_WriteBit(MOTOR_BRAKE_PORT, MOTOR_BRAKE_PIN, Bit_RESET);
	}
}

/*******************************************************************************
  Function		: motor_brake_release      
  Description	: get motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void motor_brake_release(void)
{
	if(g_motor.motor_brake_state)
	{
		g_motor.motor_brake_state = FALSE;
		GPIO_WriteBit(MOTOR_BRAKE_PORT, MOTOR_BRAKE_PIN, Bit_SET);
	}
}


/*******************************************************************************
  Function		: motor_init      
  Description	: motor init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void motor_init(void)
{
	motor_gpio_configuration();
	motor_pwm_configuration();
	disable_motor();
}

/*******************************************************************************
  Function		: enable_motor      
  Description	: enable motor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_motor(void)
{
	motor_brake_release();

	if(g_motor.motor_left_enable != TRUE)
	{
		
		g_motor.motor_left_enable = TRUE;
		pwm_ctrl_enable();
		GPIO_WriteBit(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, Bit_RESET);
	}

	if(g_motor.motor_right_enable != TRUE)
	{
		g_motor.motor_right_enable = TRUE;
		pwm_ctrl_enable();
		GPIO_WriteBit(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, Bit_RESET);
	}	
}	

/*******************************************************************************
  Function		: disable_motor      
  Description	: disable motor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_motor(void)
{
	if(g_motor.motor_left_enable != FALSE)
	{
		g_motor.motor_left_pwm = 0;
		g_motor.motor_left_enable = FALSE;
		g_motor.motor_left_direction = DIRECTION_CW;
		pwm_ctrl_disable();
		GPIO_WriteBit(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, Bit_SET);
	}

	if(g_motor.motor_right_enable != FALSE)
	{
		g_motor.motor_right_pwm = 0;
		g_motor.motor_right_enable = FALSE;
		g_motor.motor_right_direction = DIRECTION_CW;
		pwm_ctrl_disable();
		GPIO_WriteBit(MOTOR_ENABLE_PORT, MOTOR_ENABLE_PIN, Bit_SET);
	}	
	
	motor_brake();
}	

/*******************************************************************************
  Function		: update_motor_current      
  Description	: update motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void update_motor_current(void)
{
	g_motor.motor_left_current = get_motor_current((s32)(get_adc_value(MOTOR_LEFT_CURRENT)));
	g_motor.motor_right_current = get_motor_current((s32)(get_adc_value(MOTOR_RIGHT_CURRENT)));
}	
/*******************************************************************************
  Function		: update_motor_current      
  Description	: update motor current
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_motor_exception(void)
{
	g_motor.motor_left_exception = (T_bool)(!GPIO_ReadInputDataBit(MOTOR_EXCEPTION_LEFT_PORT, MOTOR_EXCEPTION_LEFT_PIN));
	g_motor.motor_right_exception =  (T_bool)(!GPIO_ReadInputDataBit(MOTOR_EXCEPTION_RIGHT_PORT, MOTOR_EXCEPTION_RIGHT_PIN));
}

/*******************************************************************************
  Function		: reset_motor      
  Description	: reset motor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_motor(void)
{
	set_motor_pwm(0, 0);

	g_motor.motor_left_current = 0;
	g_motor.motor_right_current = 0;
}

/*******************************************************************************
  Function		: get_motor_info      
  Description	: get motor info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_motor_info(T_motor * motor)
{
	update_motor_current();
	update_motor_exception();
	
	motor->motor_brake_state = g_motor.motor_brake_state;

	motor->motor_left_current = g_motor.motor_left_current;
	motor->motor_left_enable = g_motor.motor_left_enable;
	motor->motor_left_pwm = g_motor.motor_left_pwm;
	motor->motor_left_direction = g_motor.motor_left_direction;
	motor->motor_left_exception = g_motor.motor_left_exception;
	
	motor->motor_right_direction = g_motor.motor_right_direction;
	motor->motor_right_enable = g_motor.motor_right_enable;
	motor->motor_right_pwm = g_motor.motor_right_pwm;
	motor->motor_right_current = g_motor.motor_right_current;
	motor->motor_right_exception = g_motor.motor_right_exception;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
