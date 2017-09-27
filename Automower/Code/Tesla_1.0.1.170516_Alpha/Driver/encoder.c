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
#include "encoder.h"
#include "motor.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static T_encoder g_encoder = {TRUE, TRUE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* funcitons ******************************************************************/
static void encoder_gpio_configuration(void);
static void encoder_exti_configuration(void);

static void encoder_timer_configuration(void);

void update_left_encoder(void);
void update_right_encoder(void);

/*******************************************************************************
  Function		: encoder_gpio_configuration      
  Description	: encoder gpio configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void encoder_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(ENCODER_LEFT_A_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(ENCODER_RIGHT_A_CLK, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = ENCODER_LEFT_A_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ENCODER_LEFT_A_PORT, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = ENCODER_LEFT_A_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(ENCODER_LEFT_A_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ENCODER_RIGHT_A_PIN;
	GPIO_Init(ENCODER_RIGHT_A_PORT, &GPIO_InitStructure);
	
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
}
/*******************************************************************************
  Function		: encoder_exti_configuration      
  Description	: encoder_exti_configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void encoder_exti_configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SYSCFG_EXTILineConfig(ENCODER_LEFT_PORTSOURCE, ENCODER_LEFT_PINSOURCE);
	SYSCFG_EXTILineConfig(ENCODER_RIGHT_PORTSOURCE, ENCODER_RIGHT_PINSOURCE);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
/*******************************************************************************
  Function		: encoder_timer_configuration      
  Description	: encoder timer configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void encoder_timer_configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(ENCODER_LEFT_TIM_CLK | ENCODER_RIGHT_TIM_CLK, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(ENCODER_LEFT_TIMER, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(ENCODER_RIGHT_TIMER, &TIM_TimeBaseStructure);

	
//	TIM_TIxExternalClockConfig(ENCODER_LEFT_TIMER, TIM_TIxExternalCLK1Source_TI2, TIM_ICPolarity_Rising, 0);
//	TIM_CounterModeConfig(ENCODER_LEFT_TIMER,TIM_CounterMode_Up);
//	TIM_SetCounter(ENCODER_LEFT_TIMER,0);//
//	TIM_Cmd(ENCODER_LEFT_TIMER,ENABLE); //
//	
//	TIM_TIxExternalClockConfig(ENCODER_RIGHT_TIMER, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
//	TIM_CounterModeConfig(ENCODER_RIGHT_TIMER,TIM_CounterMode_Up);
//	TIM_SetCounter(ENCODER_RIGHT_TIMER,0);
//	TIM_Cmd(ENCODER_RIGHT_TIMER,ENABLE); 
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM_ICInit(ENCODER_LEFT_TIMER, &TIM_ICInitStructure);

	TIM_PWMIConfig(ENCODER_LEFT_TIMER, &TIM_ICInitStructure);               
	TIM_SelectInputTrigger(ENCODER_LEFT_TIMER, TIM_TS_TI2FP2);               
	TIM_SelectSlaveMode(ENCODER_LEFT_TIMER, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(ENCODER_LEFT_TIMER, TIM_MasterSlaveMode_Enable); 
	TIM_Cmd(ENCODER_LEFT_TIMER, ENABLE);          

//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	TIM_ICInit(ENCODER_RIGHT_TIMER, &TIM_ICInitStructure);
//	
//	TIM_ETRClockMode2Config(ENCODER_LEFT_TIMER, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
//	TIM_SetCounter(ENCODER_LEFT_TIMER, 0);
//	TIM_Cmd(ENCODER_LEFT_TIMER, ENABLE);


//	TIM_ETRClockMode2Config(ENCODER_RIGHT_TIMER, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
//	TIM_SetCounter(ENCODER_RIGHT_TIMER, 0);
//	TIM_Cmd(ENCODER_RIGHT_TIMER, ENABLE);
	
////	TIM_SelectInputTrigger(ENCODER_LEFT_TIMER, TIM_TS_TI2FP2);
////	TIM_SelectSlaveMode(ENCODER_LEFT_TIMER, TIM_SlaveMode_External1);
////	TIM_SelectMasterSlaveMode(ENCODER_LEFT_TIMER, TIM_MasterSlaveMode_Enable);
////	TIM_TIxExternalClockConfig(ENCODER_LEFT_TIMER,TIM_TIxExternalCLK1Source_TI1ED,TIM_ICPolarity_Rising,0);
////	TIM_CounterModeConfig(ENCODER_LEFT_TIMER,TIM_CounterMode_Up);   
//	
//	TIM_SetCounter(ENCODER_LEFT_TIMER, 0);
//    TIM_ClearITPendingBit(ENCODER_LEFT_TIMER, TIM_IT_Update);
//    TIM_Cmd(ENCODER_LEFT_TIMER, ENABLE);  //计数器使能，开始工作
//	
////	TIM_TIxExternalClockConfig(ENCODER_RIGHT_TIMER,TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising,0);
////    TIM_CounterModeConfig(ENCODER_RIGHT_TIMER,TIM_CounterMode_Up);
//	TIM_SetCounter(ENCODER_RIGHT_TIMER, 0);
//    TIM_ClearITPendingBit(ENCODER_RIGHT_TIMER, TIM_IT_Update);
//    TIM_Cmd(ENCODER_RIGHT_TIMER, ENABLE);  //计数器使能，开始工作
}

/*******************************************************************************
  Function		: encoder_timer_configuration      
  Description	: encoder timer configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void encoder_init(void)
{
	encoder_exti_configuration();
	encoder_gpio_configuration();

//	encoder_timer_configuration();
	reset_encoder();
}
/*******************************************************************************
  Function		: update_left_encoder     
  Description	: update_left_encoder
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void update_left_encoder(void)
{
	T_motor motor;
	get_motor_info(&motor);

	if(motor.motor_left_direction == DIRECTION_CCW)
	{
		g_encoder.left_encoder_sum++;
	}
	else
	{
		g_encoder.left_encoder_sum--;
	}	
}
/*******************************************************************************
  Function		: update_right_encoder     
  Description	: update_right_encoder
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void update_right_encoder(void)
{
	T_motor motor;
	get_motor_info(&motor);

	if(motor.motor_right_direction == DIRECTION_CW)
	{
		g_encoder.right_encoder_sum++;
	}
	else
	{
		g_encoder.right_encoder_sum--;
	}
}
/*******************************************************************************
  Function		: update_left_encoder      
  Description	: update left encoder
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
//void update_left_encoder(void)
//{
//	T_motor motor;
//	get_motor_info(&motor);
//	
//	if(g_encoder.left_first_measure_flag == FALSE)
//	{
//		g_encoder.left_encoder = ENCODER_LEFT_TIMER->CNT;
//		g_encoder.left_encoder_delta = g_encoder.left_encoder - g_encoder.last_left_encoder;
//		
//		if(g_encoder.left_encoder < 5535)
//		{
//			g_encoder.left_encoder_overflow--;
//			g_encoder.left_encoder = 60000 + (g_encoder.left_encoder - 5535);
//			ENCODER_LEFT_TIMER->CNT = g_encoder.left_encoder;
//		}
//		else if(g_encoder.left_encoder > 60000)
//		{
//			g_encoder.left_encoder_overflow++;
//			g_encoder.left_encoder = 5535 + (g_encoder.left_encoder - 60000);
//			ENCODER_LEFT_TIMER->CNT = g_encoder.left_encoder;
//		}
//		
//		g_encoder.last_left_encoder = g_encoder.left_encoder;
//		if(motor.motor_left_direction == DIRECTION_CW)
//		{
//			g_encoder.left_speed = g_encoder.left_encoder_delta * ENCODER_UPDATE_FREQUENCY;
//			g_encoder.left_encoder_sum += g_encoder.left_encoder_delta;
//		}
//		else
//		{
//			g_encoder.left_speed = -g_encoder.left_encoder_delta * ENCODER_UPDATE_FREQUENCY;
//			g_encoder.left_encoder_sum -= g_encoder.left_encoder_delta;
//		}
//	}
//	else
//	{
//		g_encoder.left_first_measure_flag = FALSE;
//		g_encoder.left_encoder = 27000;
//		g_encoder.last_left_encoder = g_encoder.left_encoder;
//		g_encoder.left_encoder_delta = g_encoder.left_encoder - g_encoder.last_left_encoder;
//		g_encoder.left_encoder_sum = 0;
//		g_encoder.left_encoder_overflow = 0;
//		g_encoder.left_speed = 0;
//		ENCODER_LEFT_TIMER->CNT = g_encoder.left_encoder;
//	}
//}

/*******************************************************************************
  Function		: update_right_encoder      
  Description	: update right encoder
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
//void update_right_encoder(void)
//{
//	T_motor motor;
//	get_motor_info(&motor);
//	
//	if(g_encoder.right_first_measure_flag == FALSE)
//	{
//		g_encoder.right_encoder = ENCODER_RIGHT_TIMER->CNT;
//		g_encoder.right_encoder_delta = g_encoder.right_encoder - g_encoder.last_right_encoder;
//		
//		
//		if(g_encoder.right_encoder < 5535)
//		{
//			g_encoder.right_encoder_overflow--;
//			g_encoder.right_encoder = 60000 + (g_encoder.right_encoder - 5535);
//			ENCODER_RIGHT_TIMER->CNT = g_encoder.right_encoder;
//		}
//		else if(g_encoder.right_encoder > 60000)
//		{
//				g_encoder.right_encoder_overflow++;
//				g_encoder.right_encoder = 5535 + (g_encoder.right_encoder - 60000);
//				ENCODER_RIGHT_TIMER->CNT = g_encoder.right_encoder;
//		}
//		g_encoder.last_right_encoder = g_encoder.right_encoder;
//		if(motor.motor_right_direction == DIRECTION_CW)
//		{
//			g_encoder.right_speed = g_encoder.right_encoder_delta * ENCODER_UPDATE_FREQUENCY;
//			g_encoder.right_encoder_sum+= g_encoder.right_encoder_delta;
//		}
//		else
//		{
//			g_encoder.right_speed = -g_encoder.right_encoder_delta * ENCODER_UPDATE_FREQUENCY;
//			g_encoder.right_encoder_sum -= g_encoder.right_encoder_delta;
//		}
//	}
//	else
//	{
//		g_encoder.right_first_measure_flag = FALSE;
//		g_encoder.right_encoder = 27000;
//		g_encoder.last_right_encoder = g_encoder.right_encoder;
//		g_encoder.right_encoder_delta = g_encoder.right_encoder - g_encoder.last_right_encoder;
//		g_encoder.right_encoder_sum = 0;
//		g_encoder.right_encoder_overflow = 0;
//		g_encoder.right_speed = 0;
//		ENCODER_RIGHT_TIMER->CNT = g_encoder.right_encoder;
//	}
//}

/*******************************************************************************
  Function		: get_left_motor_speed      
  Description	: get left motor speed
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_left_motor_speed(void)
{
	return g_encoder.left_speed;	
}
/*******************************************************************************
  Function		: get_right_motor_speed      
  Description	: get right motor speed
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_right_motor_speed(void)
{
	return g_encoder.right_speed;
}
/*******************************************************************************
  Function		: get_left_encoder      
  Description	: get left encoder
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
float get_left_encoder(void)
{
	return (float)(g_encoder.left_encoder_sum * 2.0 * M_PI * WHEEL_RADIUS / PLUSE_PER_CIRCLE);
}
/*******************************************************************************
  Function		: get_right_encoder      
  Description	: get right encoder
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
float get_right_encoder(void)
{
	return (float)(g_encoder.right_encoder_sum * 2.0 * M_PI * WHEEL_RADIUS / PLUSE_PER_CIRCLE);
}

/*******************************************************************************
  Function		: reset_encoder      
  Description	: reset encoder
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_encoder(void)
{
	g_encoder.left_encoder_sum = 0;
	g_encoder.right_encoder_sum = 0;
}
/*******************************************************************************
  Function		: get_encoder_info      
  Description	: get encoder info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_encoder_info(T_encoder * encoder)
{
//	update_left_encoder();
//	update_right_encoder();

	encoder->left_first_measure_flag = g_encoder.left_first_measure_flag;
	encoder->right_first_measure_flag = g_encoder.right_first_measure_flag;
	encoder->left_speed = g_encoder.left_speed;
	encoder->right_speed = g_encoder.right_speed ;
	encoder->left_encoder = g_encoder.left_encoder;
	encoder->right_encoder = g_encoder.right_encoder;
	encoder->last_left_encoder = g_encoder.last_left_encoder;
	encoder->last_right_encoder = g_encoder.last_right_encoder;
	encoder->left_encoder_overflow = g_encoder.left_encoder_overflow;
	encoder->right_encoder_overflow = g_encoder.right_encoder_overflow;
	encoder->left_encoder_delta = g_encoder.left_encoder_delta;
	encoder->right_encoder_delta = g_encoder.right_encoder_delta;
	encoder->left_encoder_sum = g_encoder.left_encoder_sum;
	encoder->right_encoder_sum = g_encoder.right_encoder_sum;
}
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
