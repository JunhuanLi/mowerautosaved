/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	rain_sensor.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/23
Description		:	rain_sensor driiver
*******************************************************************************/

/* includes *******************************************************************/
#include "rain_sensor.h"
#include "adc.h"
/* macros *********************************************************************/
#define RAIN_THRESHOLD    500


/* static variables ***********************************************************/
static T_bool g_rain_sensor = FALSE;
static u16 	 g_rain_adc_value = 0;

/* funcitons ******************************************************************/
static void update_rain_sensor_adc_value(void);
static void process_rain_sensor_adc_value(void);


/*******************************************************************************
  Function		: update_broder_sensor_adc_value     
  Description	: update_broder_sensor_adc_value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_broder_sensor_adc_value(void)
{
	g_rain_adc_value =  get_adc_value(RAIN_SENSOR);
}

/*******************************************************************************
  Function		: process_rain_sensor_adc_value     
  Description	: process_rain_sensor_adc_value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void process_rain_sensor_adc_value(void)
{
	g_rain_sensor = g_rain_adc_value > RAIN_THRESHOLD ? TRUE: FALSE;
}

/*******************************************************************************
  Function		: reset_rain_sensor     
  Description	: reset rain sensor
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void reset_rain_sensor(void)
{
	g_rain_sensor = 0;
}

/*******************************************************************************
  Function		: get_rain_sensor_info     
  Description	: get rain sensor info
  Input			: None                   
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_rain_sensor_info(T_bool *rain_sensor)
{
	update_broder_sensor_adc_value();

	process_rain_sensor_adc_value();

	*rain_sensor = g_rain_sensor;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
