/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: hardware.c
  Author		  :  klaus 	
  Version		  : V1.0.0	  
  Date			  : 2017/05/27
  Description	: battery 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

/* includes *******************************************************************/
#include "hardware.h"
#include "debug.h"
#include "motor_control.h"

#define mag_cali  0      //临时标志位，mag_cali=1需要运行磁罗盘校准程序，mag_cali=0不需要

/* static variables ***********************************************************/
T_sensor g_sensor_info;
float rot_vec[3] = {1,0,0};   //rotation vector in x/y/z order
float eul_rad[3] = {0};       //euler angles   [rad]  1=yaw 2=pitch 3=roll
float quat[4] = {0};          //quaternion in Hamilton definition
float g_timediff_s_imu;           //time difference[s]
//float g_timediff_s_gps;           //time difference[s]
float gyro_bias[3];           //gyroscope bias [rad/s]  
float acc_bias_g[3];           //accelerometer init bias [g]  
float gyro_bias_20602[3];           //gyroscope bias [rad/s]  
float acc_bias_g_20602[3];           //accelerometer init bias [g]  

double pos_lla_init[3];        //initial position in lla frame
float declination_rad;        //magnetic declination angle [rad]
imu_scaled_body g_sensor_scaled_body = {0};    
ellipse mag_cali_ellipse;
ellipse mag_cali_ellipse2;

/* funcitons ******************************************************************/
static void update_gpio_sensors(void);
static void update_imu_sensors(void);
static void update_adc_sensors(void);
static void update_other_sensors(void);
//static void gyro_bias_init(void);

/*------------------------------------------------------------------------------
Function		: update_gpio_sensors
Description		: update_gpio_sensors
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void update_gpio_sensors(void)
{

}

/*------------------------------------------------------------------------------
Function		: update_imu_sensors
Description		: update_imu_sensors
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void update_imu_sensors(void)
{
	get_mag_info1(&g_sensor_info.mag1);
	get_mag_info2(&g_sensor_info.mag2);
	get_mpu_info(&g_sensor_info.mpu); 
	get_front_bumper_info(&g_sensor_info.bumper);
	get_wheel_up_info(&g_sensor_info.wheel_up);
	
}

/*------------------------------------------------------------------------------
Function		: update_adc_sensors
Description		: update_adc_sensors
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void update_adc_sensors(void)
{

}

/*------------------------------------------------------------------------------
Function		: update_other_sensors
Description		: update_other_sensors
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
static void update_other_sensors(void)
{
	get_sonar_info(&g_sensor_info.sonar);
}

/*------------------------------------------------------------------------------
Function		: update_all_sensors
Description		: update_all_sensors
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void update_all_sensors(void)
{	
    rt_enter_critical();

	update_gpio_sensors();
	//update_imu_sensors();
	update_adc_sensors();
	update_other_sensors();

    rt_exit_critical();
}

/*------------------------------------------------------------------------------
Function		: gyroscope zero-bias init
Description	: hardware_init
Input			  : gyro bias[3]
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void mpu_bias_init(void)
{
	T_mpu       mpu;
	T_mpu_20602 mpu_20602;
	s16 i = 0;
	u8  k;
	s16 gyro_cali_times = 500;
	s64 gyro_sum[3] = {0};
	s64 acc_sum[3] = {0};
	s64 gyro_sum_20602[3] = {0};
	s64 acc_sum_20602[3] = {0};
	
	//delay_ms(1000);                  //等待mems陀螺初始化完成
	for(i=0;i<gyro_cali_times;i++){
		get_mpu_info(&mpu);
		ICM20602_getMotion6(&mpu_20602);
		gyro_sum[0] = gyro_sum[0] + mpu.gx;
		gyro_sum[1] = gyro_sum[1] + mpu.gy;
		gyro_sum[2] = gyro_sum[2] + mpu.gz;
		acc_sum[0]  = acc_sum[0] + mpu.ax;
		acc_sum[1]  = acc_sum[1] + mpu.ay;		
		acc_sum[2]  = acc_sum[2] + mpu.az;			
		gyro_sum_20602[0] = gyro_sum_20602[0] + mpu_20602.gx;
		gyro_sum_20602[1] = gyro_sum_20602[1] + mpu_20602.gy;
		gyro_sum_20602[2] = gyro_sum_20602[2] + mpu_20602.gz;
		acc_sum_20602[0]  = acc_sum_20602[0] + mpu_20602.ax;
		acc_sum_20602[1]  = acc_sum_20602[1] + mpu_20602.ay;		
		acc_sum_20602[2]  = acc_sum_20602[2] + mpu_20602.az;					
	}

	for(k=0;k<3;k++){
		gyro_bias[k]        = (float)gyro_sum[k]/gyro_cali_times/GYRO_FACTOR*PI/180.0f;
    acc_bias_g[k]       = (float)acc_sum[k]/ACC_FACTOR/gyro_cali_times;
		gyro_bias_20602[k]  = (float)gyro_sum_20602[k]/gyro_cali_times/GYRO_FACTOR_20602*PI/180.0f;
    acc_bias_g_20602[k] = (float)acc_sum_20602[k]/ACC_FACTOR_20602/gyro_cali_times;
	}
	
//	send_buf[k] = 0xB5;k++;
//	send_buf[k] = 0x62;k++;
//	
//	
//	temp = (char *)(&mag1.mag_x);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	
//  temp = (char *)(&mag1.mag_y);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	
//	temp = (char *)(&mag1.mag_z);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	
//	temp = (char *)(&mag2.mag_x);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	
//  temp = (char *)(&mag2.mag_y);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	
//	temp = (char *)(&mag2.mag_z);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	
//	rt_debug(send_buf,k);

}

void mag_cali_init(void){
	
	s16      x1[mag_cali_len]={0},y1[mag_cali_len]={0};
	s16      x2[mag_cali_len]={0},y2[mag_cali_len]={0};
    T_mag    mag1,mag2;
	s16      cnt=0;
	
	set_motor_control_type(MOTOR_CONTROL_TYPE_SPEED);
	set_motor_control_speed_s32(-800,800);
	update_motor_control();
	if(mag_cali==1){
		while(cnt<mag_cali_len){
			delay_ms(50);
			get_mag_info1(&mag1);
			get_mag_info2(&mag2);
			if(mag2.mag_x==0x00)       continue;
			x1[cnt] = mag1.mag_x; y1[cnt] = mag1.mag_y;
			x2[cnt] = mag2.mag_x; y2[cnt] = mag2.mag_y;
			//send_data_mag(mag1,mag2);
 			cnt++;
	}
	  mag_cali_ellipse = get_mag_ellipse(x1,y1);
	  mag_cali_ellipse2 = get_mag_ellipse(x2,y2);
	  rt_kprintf("\r\na = %d",(int)(mag_cali_ellipse.a*10000));
	  rt_kprintf("\r\na = %d",(int)(mag_cali_ellipse.b*10000));
	  rt_kprintf("\r\na = %d",(int)(mag_cali_ellipse.phi*10000));
      rt_kprintf("\r\na = %d",(int)(mag_cali_ellipse.X0*10000));
      rt_kprintf("\r\na = %d",(int)(mag_cali_ellipse.Y0*10000));
      rt_kprintf("\r\na = %d",(int)(mag_cali_ellipse.X0_in*10000));
      rt_kprintf("\r\na = %d",(int)(mag_cali_ellipse.Y0_in*10000));
	}
	else{//临时
		mag_cali_ellipse.a = 136.799613647;
		mag_cali_ellipse.b = 131.4135275;
		mag_cali_ellipse.phi = 0.44732256863;
		mag_cali_ellipse.X0 = 36.211554565;
		mag_cali_ellipse.Y0 = -14.87538809;
		mag_cali_ellipse.X0_in = 26.21208099;
		mag_cali_ellipse.Y0_in = -29.076323;
		mag_cali_ellipse.long_axis = mag_cali_ellipse.a*2;
		mag_cali_ellipse.short_axis = mag_cali_ellipse.b*2;
		mag_cali_ellipse.status = 1;
		
	
 		mag_cali_ellipse2.a = 86.2569613647;
		mag_cali_ellipse2.b = 73.8735275;
		mag_cali_ellipse2.phi = 0.35092256863;
		mag_cali_ellipse2.X0 = 19.46554565;
		mag_cali_ellipse2.Y0 = -15.11638809;
		mag_cali_ellipse2.X0_in = 13.03708099;
		mag_cali_ellipse2.Y0_in = -20.857323;
		mag_cali_ellipse2.long_axis = mag_cali_ellipse2.a*2;
		mag_cali_ellipse2.short_axis = mag_cali_ellipse2.b*2;
		mag_cali_ellipse2.status = 1;
	}
	if(mag_cali_ellipse.status==-1)
		rt_kprintf("mag calibration ellipse error!!!!!!!!!");

	set_motor_control_speed_s32(0,0);
	update_motor_control();
};


/*------------------------------------------------------------------------------
Function		: hardware_init
Description		: hardware_init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void hardware_init(void)
{
	time_control_clock_init();
	mpu_init();
	SPI4_Master_Init();
	ICM20602_Init();
	mag_init2();
	mag_init();
	beep_init();
	wheel_up_init();
	front_bumper_init();
	sonar_init();
	motor_init();
	encoder_init();
	usarts_init();
	adc_init();
	power_control_init();

	mag_cali_init();
 	mpu_bias_init();

	//initialization of position[lla] and time is done in thread_gps.c
	//declination_rad = magHeading2Geog(pos_lla_init);            //get local magetic declination
}

/*------------------------------------------------------------------------------
Function		: get_all_sensors
Description		: get_all_sensors
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
T_sensor get_all_sensors(void)
{
	return g_sensor_info;
}
/*------------------------------------------------------------------------------
Function		: is_hardware_valid
Description		: is_hardware_valid
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
T_bool is_hardware_valid(void)
{
	reset_encoder();
	
	return TRUE;
}

//分析UBX-NAV-PVT协议
//返回值:latitude,longitude,height,hMSL,year/month/day/hour/min/sec,fixtype,hAcc,vAcc,sAcc,velN,velE,velD
//u8 USART_TX_BUF[200];


/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
