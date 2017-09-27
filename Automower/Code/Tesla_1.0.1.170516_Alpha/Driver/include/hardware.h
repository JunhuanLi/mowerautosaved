/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: battery.h
  Author		: klaus 	
  Version		: V1.0.0	  
  Date			: 2017/05/27
  Description	: battery 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include <rtthread.h>
#include "ist8310.h"
#include "ist83102.h"
#include "mpu6500_spi.h"
#include "time_control.h"
#include "beep.h"
#include "bumper_front.h"
#include "wheel_up.h"
#include "sonar.h"
#include "encoder.h"
#include "motor.h"
#include "usart_driver.h"
#include "sonar.h"
#include "adc.h"
#include "flash.h"
#include "power.h"
#include "typedef.h"
#include "ICM20602.h"

//extern gpsx GPS_DATA;
extern imu_scaled_body g_sensor_scaled_body;
extern T_sensor g_sensor_info;

/* funcitons ******************************************************************/
void hardware_init(void);
void update_all_sensors(void);
void gyro_bias_init(void);
T_sensor get_all_sensors(void);
ellipse get_mag_ellipse(s16 x[mag_cali_len],s16 y[mag_cali_len]);
void mag_ellipse_mapping(ellipse *ellipse_t, T_mag *mag,float mag_body[3]);
T_bool is_hardware_valid(void);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
