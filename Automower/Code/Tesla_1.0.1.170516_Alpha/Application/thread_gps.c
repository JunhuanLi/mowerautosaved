
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			thread_gps.c
  Author:			   	rui.fang
  Date:			    	2017.5.16
  Version:        
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/


#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdio.h>
#include "insgps.h"
#include "time_control.h"
#include "usart.h"
#include "debug.h"
#include "lcd12864_io_spi.h"
#include "mower_common.h"
#include "conv_enc.h"

/*在分配堆栈空间时，必须要对其*/
ALIGN(RT_ALIGN_SIZE)
char thread_gps_stack[3072];
struct rt_thread thread_gps;
local_time ltime = {0};
float pos_ned_m[3];
float vel_ned_m_s[3];
u8 trapped_flag = 0;
float delta_odo_l,delta_odo_r;

/*170803 VARS AND FUNCTIONS*/
gpsx GPS_DATA;
rt_size_t result_size = 0;
u8 USART_GPS_RX_BUFFER[200]; 

void ubx_nav_pvt(gpsx *gpsx,u8 *buf)
{ 
	s32 temp;
	
	//u2
	temp = buf[5];
	temp = temp << 8;
	temp = temp + buf[4];
	gpsx->year = temp;
	
	//u1
	gpsx->month = buf[6];
	gpsx->day 	= buf[7];
	gpsx->hour 	= buf[8];
	gpsx->min 	= buf[9];
	gpsx->sec 	= buf[10];
	
	//x1
	gpsx->valid = buf[11];
	
	//u4
	temp = buf[15];
	temp = temp << 8;
	temp = temp + buf[14];
	temp = temp << 8;
	temp = temp + buf[13];
	temp = temp << 8;
	temp = temp + buf[12];
	gpsx->tAcc_ns = (float)temp;
	
	//i4
	temp = buf[19];
	temp = temp << 8;
	temp = temp + buf[18];
	temp = temp << 8;
	temp = temp + buf[17];
	temp = temp << 8;
	temp = temp + buf[16];
	gpsx->ns = temp;
	
	//i4
	temp = buf[27];
	temp = temp << 8;
	temp = temp + buf[26];
	temp = temp << 8;
	temp = temp + buf[25];
	temp = temp << 8;
	temp = temp + buf[24];
	gpsx->longitude_rad = (float)temp*1e-7*PI/180;
	
	temp = buf[31];
	temp = temp << 8;
	temp = temp + buf[30];
	temp = temp << 8;
	temp = temp + buf[29];
	temp = temp << 8;
	temp = temp + buf[28];
	gpsx->latitude_rad = (float)temp*1e-7*PI/180;
	
	temp = buf[35];
	temp = temp << 8;
	temp = temp + buf[34];
	temp = temp << 8;
	temp = temp + buf[33];
	temp = temp << 8;
	temp = temp + buf[32];
	gpsx->height_m = (float)temp*0.001;

	temp = buf[39];
	temp = temp << 8;
	temp = temp + buf[38];
	temp = temp << 8;
	temp = temp + buf[37];
	temp = temp << 8;
	temp = temp + buf[36];
	gpsx->hMSL_m = (float)temp*0.001;
		
	//u4
	temp = buf[43];
	temp = temp << 8;
	temp = temp + buf[42];
	temp = temp << 8;
	temp = temp + buf[41];
	temp = temp << 8;
	temp = temp + buf[40];
	gpsx->hAcc_m = (float)temp*0.001;
	
	temp = buf[47];
	temp = temp << 8;
	temp = temp + buf[46];
	temp = temp << 8;
	temp = temp + buf[45];
	temp = temp << 8;
	temp = temp + buf[44];
	gpsx->vAcc_m = (float)temp*0.001;
	
	//i4
	temp = buf[51];
	temp = temp << 8;
	temp = temp + buf[50];
	temp = temp << 8;
	temp = temp + buf[49];
	temp = temp << 8;
	temp = temp + buf[48];
	gpsx->velN_m_s = (float)temp*0.001;
	
	temp = buf[55];
	temp = temp << 8;
	temp = temp + buf[54];
	temp = temp << 8;
	temp = temp + buf[53];
	temp = temp << 8;
	temp = temp + buf[52];
	gpsx->velE_m_s = (float)temp*0.001;
	
	temp = buf[59];
	temp = temp << 8;
	temp = temp + buf[58];
	temp = temp << 8;
	temp = temp + buf[57];
	temp = temp << 8;
	temp = temp + buf[56];
	gpsx->velD_m_s = (float)temp*0.001;
	
	//u4
	temp = buf[71];
	temp = temp << 8;
	temp = temp + buf[70];
	temp = temp << 8;
	temp = temp + buf[69];
	temp = temp << 8;
	temp = temp + buf[68];	
	gpsx->sAcc_m_s = (float)temp*0.001;
	
	temp = buf[75];
	temp = temp << 8;
	temp = temp + buf[74];
	temp = temp << 8;
	temp = temp + buf[73];
	temp = temp << 8;
	temp = temp + buf[72];	
	gpsx->headAcc_deg = (float)temp*1e-5;
	
	temp = buf[67];
	temp = temp << 8;
	temp = temp + buf[66];
	temp = temp << 8;
	temp = temp + buf[65];
	temp = temp << 8;
	temp = temp + buf[64];	
	gpsx->heading_deg = (float)temp*1e-5;
	
	gpsx->numSV = buf[23];
	
	//local time 赋值
	ltime.year  = gpsx->year;
	ltime.month = gpsx->month;
	ltime.day   = gpsx->day;
	ltime.hour  = gpsx->hour+8;            //for CHINA in UTC-8
	ltime.min   = gpsx->min;
	ltime.sec   = gpsx->sec;
}

//void local_time_init(gpsx *gps)
//{
//	ltime.year  = gps->year;
//	ltime.month = gps->month;
//	ltime.day   = gps->day;
//	ltime.hour  = gps->hour+8;            //for CHINA in UTC-8
//	ltime.min   = gps->min;
//	ltime.sec   = gps->sec;
//}

void pos_init(gpsx *gps)
{
	pos_lla_init[0] = (double)gps->latitude_rad;
	pos_lla_init[1] = (double)gps->longitude_rad;
	pos_lla_init[2] = (double)gps->hMSL_m;
}

void get_local_time(local_time *time)
{
	time->year  = ltime.year;
	time->month = ltime.month;
	time->day   = ltime.day;
	time->hour  = ltime.hour;
	time->min   = ltime.min;
	time->sec   = ltime.sec;
	
}

void get_pos_ned(float pos[3])
{
	pos[0] = pos_ned_m[0];
	pos[1] = pos_ned_m[1];
	pos[2] = pos_ned_m[2];
}

void get_vel_ned(float vel[3])
{
	vel[0] = vel_ned_m_s[0];
	vel[1] = vel_ned_m_s[1];
	vel[2] = vel_ned_m_s[2];
}

static int checksum(unsigned char *buff, int len)
{
    unsigned char cka=0,ckb=0;
    int i;
	  int checkbit;
    
    for (i=2;i<len-2;i++) {
			cka+=buff[i]; 
			ckb+=cka;
    }
    checkbit = (cka==buff[len-2]&&ckb==buff[len-1]);
		return checkbit;
}

u8 get_gps_data(u8 *init_flag)
{
	s16 i;
	u8 gpsdata_index = 0;
	u8 gpsdata_flag = 0;
	static u64 sum = 0,check_sum = 0;
		
	if((USART_GPS_RX_BUFFER[0] == 0xB5) &&(USART_GPS_RX_BUFFER[1] == 0x62)){
		if((USART_GPS_RX_BUFFER[2]==0x01)&&(USART_GPS_RX_BUFFER[3]==0x07)&&(USART_GPS_RX_BUFFER[4]==0x5C)&&(USART_GPS_RX_BUFFER[5]==0x00)){
			 if (checksum(USART_GPS_RX_BUFFER,100))  {
				gpsdata_flag = 1;
				USART_Cmd(USART_GPS, DISABLE); 
				check_sum++;
			 }
			 sum++;
		}
	}
	
	if(gpsdata_flag == 1){
		ubx_nav_pvt(&GPS_DATA, (u8*)USART_GPS_RX_BUFFER+6+gpsdata_index);   //ubx协议解析，本地时间赋值
		//USART_GPS_ENABLE();
		USART_Cmd(USART_GPS, ENABLE);
		if(*init_flag == 1){
			//local_time_init(&GPS_TESTDATA);
			pos_init(&GPS_DATA);
			*init_flag = 0;
	  }
	}
	return gpsdata_flag;
}

void update_iter(float pos_prev[3],float vel_prev[3],float pos[3],float vel[3],float pos_temp[3],float vel_temp[3])
{
	memcpy(pos_prev,pos_temp,sizeof(float)*3);
	memcpy(vel_prev,vel_temp,sizeof(float)*3);
	memcpy(pos,pos_temp,sizeof(float)*3);
	memcpy(vel,vel_temp,sizeof(float)*3);
	
}

u8 trapping_detect(float pos[3])
{
	static float trapping_time;
	float max_pos[2]={0,0},min_pos[2]={0,0};
	u8 i;
	u8 trap_flag=0;
	
	trapping_time += get_time_diff_trapping()*1e-6;
	
	if(0<trapping_time && trapping_time<=TRAP_TIME_TRESH){
		for(i=0;i<2;i++){
			if(pos[i]>max_pos[i]) max_pos[i] = pos[i];
			if(pos[i]<min_pos[i]) min_pos[i] = pos[i];
		}
	}
	
	if(trapping_time>=TRAP_TIME_TRESH){
		trapping_time = 0;
		if(((max_pos[0]-min_pos[0])<=TRAP_POS_TRESH)&&((max_pos[1]-min_pos[1])<=TRAP_POS_TRESH))	trap_flag = 1;
	}
	return trap_flag;
}



/******************integrated navigation filter main function ******************************************/
void mower_gps_thread(void* parameter)
{
	static unsigned short int i = 0;
	float acc_imu_m_s2[3] ;
	static float P[49] = {100,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,100};
    float Q[9] = {0.800000000000000*2.64,0,0,0,0.800000000000000*2.64,0,0,0,0.800000000000000*2.64};
    float R_odom[9] = {0.00001*1.3624,0,0,0,0.00001*1.3624,0,0,0,0.01*1.3624};	
	float stateVec[7] = {0};
	u8 gpsdata_flag = 0;
	u8 init_flag = 1;
	float odom_valid;
	static float pos_prev[3] = {0,0,0},vel_prev[3]={0,0,0};
	float pos_temp[3],vel_temp[3];
	rt_uint32_t recved;
	u8 trap_period_flag = 0;
	rt_base_t level = 0;
	static u8 first_time = 0;
	float g_timediff_s_gps;           //time difference[s]
  s64 a = 0;
	
	while (1)
	{
		g_timediff_s_gps = (float)get_time_diff2()*1e-6;		
		if(fabs(g_sensor_scaled_body.acc_m_s2[2])>4.5f&&(is_att_valid == 2)&&(first_time==1)){
			/**************************prepare acceleration data **********************/
			memcpy(acc_imu_m_s2,g_sensor_scaled_body.acc_m_s2,sizeof(acc_imu_m_s2));
				
			/***************integrated navigation: time update*************************/
			ekf7_tu(acc_imu_m_s2, pos_prev,vel_prev,quat,g_timediff_s_gps, P, Q, pos_temp, vel_temp,stateVec);
	
			/***************integrated navigation: GPS measurement update*************************/
			gpsdata_flag = get_gps_data(&init_flag);	
			if((init_flag==0)&&(gpsdata_flag==1)&&(GPS_DATA.hAcc_m<10)&&(GPS_DATA.valid ==1)){   //初始化完成后
				ekf7_gps6_mu(pos_temp, vel_temp, &eul_rad[0],stateVec,P, pos_lla_init,&GPS_DATA);
			}
			
			/***************integrated navigation: Odometer measurement update********************/
			odom_preprocessing(&odom_valid,&delta_odo_l,&delta_odo_r);
			if(odom_valid==1){
				ekf7_odom_mu(pos_prev,vel_prev,&eul_rad[0], stateVec, delta_odo_r,delta_odo_l,P,R_odom,WHEEL_BASE,pos_temp,  vel_temp);
				is_odo_ready = 1;
			}
		
			//trapping detect
			trapped_flag = trapping_detect(pos_temp);

			//copy pos/vel to pos/vel previous for next iteration
			update_iter(pos_prev,vel_prev,pos_ned_m,vel_ned_m_s,pos_temp,vel_temp);
			
			//update eul and fusion updated quat into rotation vector   //!!!!!!!!!!!11shoule move into update functions later
			eul2quatf(eul_rad,quat);
			rot_update(quat,rot_vec);
		
			//temperory data send function
			//level = rt_hw_interrupt_disable();
			rt_enter_critical();
			//if(a%2==0){
				send_data(g_timediff_s_gps,delta_odo_l,delta_odo_r,GPS_DATA);
			//}
			//a++;
			//send_data_chc(g_timediff_s_gps,delta_odo_l,delta_odo_r,GPS_DATA);
			//send_data_test1();    //有数据头，发送自然数序列		
			//send_data_test2();    //无数据头，间隔发送0和1
			rt_exit_critical();
		}
		if(first_time==0) first_time=1;
		rt_thread_delay(50);// 
		}
}
