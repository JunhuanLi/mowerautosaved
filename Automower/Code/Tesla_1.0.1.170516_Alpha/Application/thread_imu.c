
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			thread_imu.c
  Author:				  rui.fang
  Date:				    2017.7.7
  Version:        

    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdio.h>
#include "imu_processing.h"
#include "imu.h"
#include "time_control.h"
#include "typedef.h"
#include "lcd12864_io_spi.h"
#include "mower_common.h"
#include "debug.h"


ALIGN(RT_ALIGN_SIZE)
char thread_imu_stack[3072];
struct rt_thread thread_imu;

/*********Variables**************/
float mag_declination_rad = -0.0421;
int is_att_valid = 0;
int is_odo_ready = 0;

/**************** main function in attitude calculation thread **********************/
void mower_imu_thread(void* parameter)
{
	float grav;
	float QAtt[9] = {0.000111111111111111*1,0,0,0,0.000111111111111111*1,0,0,0,0.000217777777777778*1};
	float RAtt[9] = {0.000111111111111111*80,0,0,0,0.000111111111111111*80,0,0,0,0.000111111111111111*80};
	static float PAtt[16] = {10,0,0,0,0,10,0,0,0,0,0.3,0,0,0,0,10};
	static float quatl[4] = {1,0,0,0};      
  static float quatl2[4] = {1,0,0,0};	
	u16 i;		
	//static u8 cnt = 0;
	imu_scaled_body imu;
	s8 init_flag = 1;
  float eul_temp[3];
  float eul_temp2[3];

	//yaw measurement update
	float P1 = 1;
	float Q_mag = 0.1;
	float R_mag = 0.01;
	static float yaw1_mag_prev=0;
	static float yaw2_mag_prev=0;
	float yaw1_after_cali_rad=0,yaw2_after_cali_rad=0;//分别是磁罗盘1和2经过校准后输出的航向角
	float trace_P;
  //float trace_P_var;
	//rt_uint32_t recved;
	float delta_yaw=0;
	grav = grav_acc((float)pos_lla_init[0]);                 //get local gravity accleration
  
	while (1)
	{

		mpu_preprocessing(grav,&imu,&yaw1_after_cali_rad,&yaw2_after_cali_rad);//22ms

		//yaw initialization  //暂时只使用磁罗盘1
		yaw_init(&init_flag,&yaw1_mag_prev,&yaw2_mag_prev,yaw1_after_cali_rad,yaw2_after_cali_rad);
			
		//outlier removal
		hampel_imu(&imu); //2ms

		//frame alignment
		frame_alignment(&imu);//us
 		memcpy(&g_sensor_scaled_body,&imu,sizeof(imu_scaled_body));
		
		//get time difference dt
		g_timediff_s_imu = (float)get_time_diff1()*1e-6;		

		//attitude calculation:time update 
		att_tu(PAtt,quatl,&imu,(float)g_timediff_s_imu, QAtt);
				
		//measurement update by accelerometer
		trace_P = att_mu_acc(PAtt,&imu,quatl,RAtt);   
		
		//filter convergence detect
		filter_converge(trace_P,&delta_yaw,eul_temp);
		
		//measurement update by magneter1
//		eul_temp[0] = att_mu_mag( &P1, yaw1_mag_prev, yaw1_after_cali_rad, R_mag, Q_mag);
//		update_yaw1_mag(eul_temp[0],&yaw1_mag_prev);

//		/*fusion mag yaw into calculated quaternion then rotation vector*/
//		eul2quatf(eul_temp,quatl);
//		quat_norm(quatl);
		
		//rotation elements update
		quat_norm(quatl);		
		quat2eulf(quatl, eul_temp);
		rot_ele_update(quatl,eul_temp,delta_yaw);

    //if(is_att_valid==2)  send_data_imu(g_timediff_s_imu,&imu);
		if(isnan(quatl[0])&&isnan(quatl[1])&&isnan(quatl[2])&&isnan(quatl[3]))
			rt_kprintf("\r\ngx = %d",g_sensor_info.mpu.gx);
		
    rt_thread_delay(20);// 20ms
	}
}
