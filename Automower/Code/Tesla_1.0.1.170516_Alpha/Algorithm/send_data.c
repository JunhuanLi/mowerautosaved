#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdio.h>
#include "insgps.h"
#include "imu.h"
#include "usart.h"
#include "encoder.h"
#include "debug.h"
#include "global_planner.h"
#include "conv_enc.h"

void  send_data_imu(float g_timediff_s_imu,imu_scaled_body *imu)
{
	char send_buf[100];
	char *temp;
	volatile u8 k = 0;
	send_buf[k] = 0xB5;k++;
	send_buf[k] = 0x63;k++;

	temp = (char *)(&g_timediff_s_imu);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&imu->acc_m_s2[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&imu->acc_m_s2[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&imu->acc_m_s2[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&imu->acc_m_s2_20602[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&imu->acc_m_s2_20602[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&imu->acc_m_s2_20602[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&imu->gyro_rps[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&imu->gyro_rps[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&imu->gyro_rps[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&imu->gyro_rps_20602[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&imu->gyro_rps_20602[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&imu->gyro_rps_20602[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&quat[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];	k++;
	
	temp = (char *)(&quat[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];	k++;
	
	temp = (char *)(&quat[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&quat[3]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	//send_buf[k] = is_att_valid;k++;

	
	rt_debug(send_buf,k);

}

/*
unsigned char g_map[g_map.info.width*g_map.info.height/4];
const unsigned short int g_map_len = g_map.info.width * g_map.info.height / 4;


void  send_data_globalplanner(float g_timediff_s_imu,float acc_imu_m_s2[3],float gyro_rps[3])
{
	//char send_buf[g_map_len];
	
	rt_uint32_t level;
	char send_buf[200];
	char *temp;
	volatile u8 k = 0;
	volatile u8 i = 0;
	send_buf[k] = 0xBB;k++;
	send_buf[k] = 0x65;k++;
	
	for(i = 2; i < g_map_len; i++)
	{
		send_buf[i] = g_map[i-2];
	}
	
	//disable interrupt
	level = rt_hw_interrupt_disable();
	
	rt_debug(send_buf,k);
	
	// enable interrupt
	rt_hw_interrupt_enable(level);

}
*/
void send_data(float dt,float delta_odo_l,float delta_odo_r,gpsx gps_data)
{
	char send_buf[200];
	char *temp;
	float acc_imu_m_s2[3];
	float gyro_rps[3];
	u8 k = 0;
    u8 len = 0;
	float checkbit=0;
	
	//get data ready
	memcpy(acc_imu_m_s2,g_sensor_scaled_body.acc_m_s2,sizeof(acc_imu_m_s2));
	memcpy(gyro_rps,g_sensor_scaled_body.gyro_rps,sizeof(gyro_rps));
	
	send_buf[k] = 0xB5;k++;
	send_buf[k] = 0x62;k++;
	temp = (char *)(&pos_ned_m[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&pos_ned_m[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&pos_ned_m[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&acc_imu_m_s2[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&acc_imu_m_s2[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&acc_imu_m_s2[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&delta_odo_l);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&delta_odo_r);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&quat[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];	k++;
	
	temp = (char *)(&quat[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];	k++;
	
	temp = (char *)(&quat[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&quat[3]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&dt);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

  temp = (char *)(&gps_data.latitude_rad);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&gps_data.longitude_rad);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&gps_data.hMSL_m);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&gps_data.velN_m_s);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&gps_data.velE_m_s);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&gps_data.velD_m_s);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&gps_data.hAcc_m);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&gps_data.vAcc_m);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
  temp = (char *)(&gps_data.sAcc_m_s);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

  temp = (char *)(&vel_ned_m_s[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&vel_ned_m_s[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	temp = (char *)(&vel_ned_m_s[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&gyro_rps[0]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	temp = (char *)(&gyro_rps[1]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;

	temp = (char *)(&gyro_rps[2]);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
//	
//	temp = (char *)(&g_sensor_scaled_body.mag1[0]);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	send_buf[k] = temp[2];k++;
//	send_buf[k] = temp[3];k++;

//	temp = (char *)(&g_sensor_scaled_body.mag1[1]);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	send_buf[k] = temp[2];k++;
//	send_buf[k] = temp[3];k++;

//	temp = (char *)(&g_sensor_scaled_body.mag1[2]);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	send_buf[k] = temp[2];k++;
//	send_buf[k] = temp[3];k++;

//	temp = (char *)(&g_sensor_scaled_body.mag2[0]);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	send_buf[k] = temp[2];k++;
//	send_buf[k] = temp[3];k++;

//	temp = (char *)(&g_sensor_scaled_body.mag2[1]);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	send_buf[k] = temp[2];k++;
//	send_buf[k] = temp[3];k++;

//	temp = (char *)(&g_sensor_scaled_body.mag2[2]);
//	send_buf[k] = temp[0];k++;
//	send_buf[k] = temp[1];k++;
//	send_buf[k] = temp[2];k++;
//	send_buf[k] = temp[3];k++;
//	
//	temp = (char *)(&gps_data.numSV);
//	send_buf[k] = temp[0];k++;
//	
//	temp = (char *)(&gps_data.valid);
//	send_buf[k] = temp[0];k++;
	
	for(len = 0; len<k;len++){
		checkbit += (float)send_buf[len];
	}
	
	temp = (char *)(&checkbit);
	send_buf[k] = temp[0];k++;
	send_buf[k] = temp[1];k++;
	send_buf[k] = temp[2];k++;
	send_buf[k] = temp[3];k++;
	
	//call rt data send function
	rt_debug(send_buf,k);
}


/*******²âÊÔ´®¿ÚÍ¨Ñ¶£ºÊý¾ÝÍ·0xB562, Êý¾ÝÎ»£º´Ó3-118µÄ×ÔÈ»Êý***********/
void send_data_test1(void)
{
	#define len  100
	char send_buf[len];
  u8 k = 0;
	
	send_buf[0] = 0xB5;
	send_buf[1] = 0x62;
	for(k=2;k<len;k++)
		send_buf[k] = k+1;

	//call rt data send function
	rt_debug(send_buf,len);
}

/*******²âÊÔ´®¿ÚÍ¨Ñ¶£ºÊý¾ÝÍ·0xB562, Êý¾ÝÎ»:0ºÍ1¼ä¸ôý***********/
void send_data_test2(void)
{
	char send_buf2[100]={0};
	u8 i;
	
  for(i=0;i<100;i++){
		if(i%2) send_buf2[i]=1;
	}

	//call rt data send function
	rt_debug(send_buf2,100);
}


void send_data_chc(float g_timediff_s_gps,float delta_odo_l,float delta_odo_r,gpsx gps_data)
{
	float send_buf[50];
	float acc_imu_m_s2[3];
	float gyro_rps[3];
	char head[2];
	u8 k = 0,i;
  
	head[0] = 0xB5;
	head[1] = 0x62;
	
	//get data ready
	memcpy(acc_imu_m_s2,g_sensor_scaled_body.acc_m_s2,sizeof(acc_imu_m_s2));
	memcpy(gyro_rps,g_sensor_scaled_body.gyro_rps,sizeof(gyro_rps));

	rt_debug(head,2);
//	send_buf[k] = pos_ned_m[0];k++;
//	send_buf[k] = pos_ned_m[1];k++;
//	send_buf[k] = pos_ned_m[2];k++;
	send_buf[k] = acc_imu_m_s2[0];k++;
	send_buf[k] = acc_imu_m_s2[1];k++;
	send_buf[k] = acc_imu_m_s2[2];k++;
//	send_buf[k] = delta_odo_l;k++;
//	send_buf[k] = delta_odo_r;k++;
//	send_buf[k] = quat[0];k++;
//	send_buf[k] = quat[1];k++;
//	send_buf[k] = quat[2];k++;
//	send_buf[k] = quat[3];k++;
	send_buf[k] = g_timediff_s_gps;k++;
	send_buf[k] = gyro_rps[0];k++;
	send_buf[k] = gyro_rps[1];k++;
	send_buf[k] = gyro_rps[2];k++;

//  temp = (char *)(&gps_data.latitude_rad);
//	
//	temp = (char *)(&gps_data.longitude_rad);


//	temp = (char *)(&gps_data.hMSL_m);
//	
//	temp = (char *)(&gps_data.velN_m_s);
//	
//	temp = (char *)(&gps_data.velE_m_s);
//	
//	temp = (char *)(&gps_data.velD_m_s);
//	
//	temp = (char *)(&gps_data.hAcc_m);
//	
//	temp = (char *)(&gps_data.vAcc_m);

//  temp = (char *)(&gps_data.sAcc_m_s);


//  temp = (char *)(&vel_ned_m_s[0]);

//	temp = (char *)(&vel_ned_m_s[1]);

//	temp = (char *)(&vel_ned_m_s[2]);


	for(i=0;i<k;i++){
	   //operate convutional channel encoding and serial send
		conv_enc_tx(send_buf[i]);
	}
}
