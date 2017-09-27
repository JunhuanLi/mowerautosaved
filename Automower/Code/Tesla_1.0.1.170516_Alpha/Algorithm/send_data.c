#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdio.h>
#include "insgps.h"
#include "imu.h"
#include "usart.h"
#include "encoder.h"
#include "debug.h"

#include "global_planner.h"

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
void send_data(float g_timediff_s_gps,float delta_odo_l,float delta_odo_r,gpsx gps_data)
{
	char send_buf[200];
	char *temp;
	float acc_imu_m_s2[3];
	float gyro_rps[3];
	volatile u8 k = 0;
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

	temp = (char *)(&g_timediff_s_gps);
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
