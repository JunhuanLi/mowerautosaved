/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	adc.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/27
Description		:	adc
*******************************************************************************/

/* includes *******************************************************************/
#include "adc.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/
static u16 g_adc_value[ADC_CHANNEL_NUM] = {0};

/* funcitons ******************************************************************/
static void adc_gpio_configuration(void);
static void adc_configuration(void);
static void dma_configuration(void);

/*------------------------------------------------------------------------------
Function		: adc_gpio_configuration
Description		: adc gpio configuration
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void adc_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(/*MOTOR_HEIGHT_CURRENT_CLK |*/ MOTOR_MOWER_CURRENT_CLK |
						   MOTOR_LEFT_CURRENT_CLK 	| MOTOR_RIGHT_CURRENT_CLK |
						   BATTERY_VOLTAGE_CLK		| BORDER_SENSOR_LEFT_CLK 	  | 
						   BORDER_SENSOR_RIGHT_CLK,ENABLE);
						   		   
//	GPIO_InitStructure.GPIO_Pin = MOTOR_HEIGHT_CURRENT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(MOTOR_HEIGHT_CURRENT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MOTOR_MOWER_CURRENT_PIN;
	GPIO_Init(MOTOR_MOWER_CURRENT_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MOTOR_LEFT_CURRENT_PIN;
	GPIO_Init(MOTOR_LEFT_CURRENT_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = MOTOR_RIGHT_CURRENT_PIN;
	GPIO_Init(MOTOR_RIGHT_CURRENT_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BATTERY_VOLTAGE_PIN;
	GPIO_Init(BATTERY_VOLTAGE_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RAIN_SENSOR_PIN;
	GPIO_Init(RAIN_SENSOR_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BORDER_SENSOR_LEFT_PIN;
	GPIO_Init(BORDER_SENSOR_LEFT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BORDER_SENSOR_RIGHT_PIN;
	GPIO_Init(BORDER_SENSOR_RIGHT_PORT, &GPIO_InitStructure);
}


/*------------------------------------------------------------------------------
Function		: adc_configuration
Description		: adc configuration
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void adc_configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	RCC_APB2PeriphClockCmd(ADC_CLK , ENABLE);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv =  ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = ADC_CHANNEL_NUM;
    ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;  
    ADC_CommonInit(&ADC_CommonInitStructure);
    
	ADC_RegularChannelConfig(ADC1, MOTOR_HEIGHT_CURRENT_ADC_CHANNEL, 1, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, MOTOR_MOWER_CURRENT_ADC_CHANNEL, 2, ADC_SampleTime_56Cycles);

	ADC_RegularChannelConfig(ADC1, MOTOR_LEFT_CURREN_ADC_CHANNEL, 3, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, MOTOR_RIGHT_CURRENT_ADC_CHANNEL, 4, ADC_SampleTime_56Cycles);

	ADC_RegularChannelConfig(ADC1, BATTERY_VOLTAGE_ADC_CHANNEL, 5, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, RAIN_SENSOR_ADC_CHANNEL, 6, ADC_SampleTime_56Cycles);
	
	ADC_RegularChannelConfig(ADC1, BORDER_SENSOR_LEFT_ADC_CHANNEL, 7, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, BORDER_SENSOR_RIGHT_ADC_CHANNEL, 8, ADC_SampleTime_56Cycles);


	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);	
	ADC_SoftwareStartConv(ADC1);
}

/*------------------------------------------------------------------------------
Function		: dma_configuration
Description		: dma configuration
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void dma_configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(ADC_DMA_CLK, ENABLE); 

	DMA_InitStructure.DMA_Channel = ADC_DMA_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)g_adc_value;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADC_CHANNEL_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(ADC_DMA_STREAM, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_STREAM, ENABLE);
}

/*------------------------------------------------------------------------------
Function		: adc init
Description		: adc init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void adc_init(void)
{
	adc_gpio_configuration();
	adc_configuration();
	dma_configuration();
}

/*------------------------------------------------------------------------------
Function		: get_adc_value
Description		: get adc value
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
u16 get_adc_value(T_adc_channel channel)
{
	return g_adc_value[channel];
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
