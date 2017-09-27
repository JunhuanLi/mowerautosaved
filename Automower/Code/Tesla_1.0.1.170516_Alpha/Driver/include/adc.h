/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: adc.h
  Author		: klaus 	
  Version		: V1.0.0	  
  Date			: 2017/05/26
  Description	: adc
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"


/* macros *********************************************************************/
#define ADC_CHANNEL_NUM						15

//#define MOTOR_HEIGHT_CURRENT_PORT		    GPIOA
//#define MOTOR_HEIGHT_CURRENT_PIN		    GPIO_Pin_6
//#define MOTOR_HEIGHT_CURRENT_CLK		    RCC_AHB1Periph_GPIOA
#define MOTOR_HEIGHT_CURRENT_ADC_CHANNEL    ADC_Channel_6

#define MOTOR_MOWER_CURRENT_PORT		    GPIOA
#define MOTOR_MOWER_CURRENT_PIN			    GPIO_Pin_7
#define MOTOR_MOWER_CURRENT_CLK			    RCC_AHB1Periph_GPIOA
#define MOTOR_MOWER_CURRENT_ADC_CHANNEL     ADC_Channel_7

#define MOTOR_LEFT_CURRENT_PORT			    GPIOB
#define MOTOR_LEFT_CURRENT_PIN			    GPIO_Pin_0
#define MOTOR_LEFT_CURRENT_CLK			    RCC_AHB1Periph_GPIOB
#define MOTOR_LEFT_CURREN_ADC_CHANNEL       ADC_Channel_8

#define MOTOR_RIGHT_CURRENT_PORT		    GPIOB
#define MOTOR_RIGHT_CURRENT_PIN			    GPIO_Pin_1
#define MOTOR_RIGHT_CURRENT_CLK			    RCC_AHB1Periph_GPIOB
#define MOTOR_RIGHT_CURRENT_ADC_CHANNEL     ADC_Channel_9

#define BATTERY_VOLTAGE_PORT			    GPIOC
#define BATTERY_VOLTAGE_PIN				    GPIO_Pin_0
#define BATTERY_VOLTAGE_CLK				    RCC_AHB1Periph_GPIOC
#define BATTERY_VOLTAGE_ADC_CHANNEL         ADC_Channel_10

#define RAIN_SENSOR_PORT			    	GPIOC
#define RAIN_SENSOR_PIN				   		GPIO_Pin_1
#define RAIN_SENSOR_CLK				   		RCC_AHB1Periph_GPIOC
#define RAIN_SENSOR_ADC_CHANNEL        		ADC_Channel_11

#define BORDER_SENSOR_LEFT_PORT			    GPIOC
#define BORDER_SENSOR_LEFT_PIN			    GPIO_Pin_2
#define BORDER_SENSOR_LEFT_CLK			    RCC_AHB1Periph_GPIOC
#define BORDER_SENSOR_LEFT_ADC_CHANNEL 		ADC_Channel_12

#define BORDER_SENSOR_RIGHT_PORT			GPIOC
#define BORDER_SENSOR_RIGHT_PIN			    GPIO_Pin_3
#define BORDER_SENSOR_RIGHT_CLK			    RCC_AHB1Periph_GPIOC
#define BORDER_SENSOR_RIGHT_ADC_CHANNEL 	ADC_Channel_13

#define ADC_CLK                             RCC_APB2Periph_ADC1
#define ADC_DMA_CLK                         RCC_AHB1Periph_DMA2
#define ADC_DMA_CHANNEL                     DMA_Channel_0
#define ADC_DMA_STREAM                      DMA2_Stream0

typedef enum
{
	MOTOR_HEIGHT_CURRENT = 0,	
	MOTOR_MOWER_CURRENT,		
	MOTOR_LEFT_CURRENT,			
	MOTOR_RIGHT_CURRENT,		
	BATTERY_VOLTAGE,
	RAIN_SENSOR,                                                     
	BORDER_SENSOR_LEFT,		
	BORDER_SENSOR_RIGHT			
}T_adc_channel;

/* funcitons ******************************************************************/
void adc_init(void);
u16 get_adc_value(T_adc_channel channel);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
