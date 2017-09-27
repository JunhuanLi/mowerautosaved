
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			imu_processing.c
  Author:				rui.fang
  Date:				    2017.9.20
  Version:        

    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/

#include "imu_processing.h"

/*********Variables**************/
static float gyro_qx[FILTER_LEN] = {0};    //filter queue
static float gyro_qy[FILTER_LEN] = {0};
static float gyro_qz[FILTER_LEN] = {0};
static float gyro_qx_20602[FILTER_LEN] = {0};    //filter queue
static float gyro_qy_20602[FILTER_LEN] = {0};
static float gyro_qz_20602[FILTER_LEN] = {0};
static float acc_qx[FILTER_LEN] = {0};
static float acc_qy[FILTER_LEN] = {0};
static float acc_qz[FILTER_LEN] = {0};
static float acc_qx_20602[FILTER_LEN] = {0};
static float acc_qy_20602[FILTER_LEN] = {0};
static float acc_qz_20602[FILTER_LEN] = {0};

static float mag2_qx[FILTER_LEN] = {0};
static float mag2_qy[FILTER_LEN] = {0};
static float mag2_qz[FILTER_LEN] = {0};

/*********Functions***************/

float grav_acc(float lat_rad)
{	
  return 9.806F - 0.0260000229F * (float)cos(2.0F * lat_rad);
}


void quat_norm(float quatl[4])
{
	
	float norm_q;
	norm_q = sqrt(quatl[0]*quatl[0]+quatl[1]*quatl[1]+quatl[2]*quatl[2]+quatl[3]*quatl[3]);
	quatl[0] = quatl[0]/norm_q;
	quatl[1] = quatl[1]/norm_q;
	quatl[2] = quatl[2]/norm_q;
	quatl[3] = quatl[3]/norm_q;
	
}


void  get_rot_vector(float vector[3])               //get rotation vector
{
	vector[0] = rot_vec[0];
	vector[1] = rot_vec[1];
	vector[2] = rot_vec[2];
}


void  get_eul_rad(float angle[3])
{
	angle[0] = eul_rad[0];
	angle[1] = eul_rad[1];
	angle[2] = eul_rad[2];
}

void mpu_preprocessing(float grav,imu_scaled_body *imu,float *yaw1_after_cali,float *yaw2_after_cali)
{
	
	T_mpu       mpu;
	T_mpu_20602 mpu_20602;
	T_mag       mag1,mag2;
	
	//get mpu info
	get_mpu_info(&mpu);
	ICM20602_getMotion6(&mpu_20602);

	//get mag data
    get_mag_info1(&mag1);
    get_mag_info2(&mag2);
	
	mag_ellipse_mapping(&mag_cali_ellipse, &mag1,imu->mag1);
	mag_ellipse_mapping(&mag_cali_ellipse2, &mag2,imu->mag2);
	
	imu->imu_temp    	     = (float)mpu.temp*0.01;
	imu->acc_m_s2[0] 	     = ((float)mpu.ax/ACC_FACTOR-acc_bias_g[0])*grav;
	imu->acc_m_s2[1] 	     = ((float)mpu.ay/ACC_FACTOR-acc_bias_g[1])*grav;
	imu->acc_m_s2[2] 	     = ((float)mpu.az/ACC_FACTOR-acc_bias_g[2]+1)*grav;
	imu->acc_m_s2_20602[0] = ((float)mpu_20602.ax/ACC_FACTOR_20602-acc_bias_g_20602[0])*grav;
	imu->acc_m_s2_20602[1] = ((float)mpu_20602.ay/ACC_FACTOR_20602-acc_bias_g_20602[1])*grav;
	imu->acc_m_s2_20602[2] = ((float)mpu_20602.az/ACC_FACTOR_20602-acc_bias_g_20602[2]+1)*grav;

	imu->gyro_rps[0] 	     = (float)mpu.gx/GYRO_FACTOR*PI/180 - gyro_bias[0];
	imu->gyro_rps[1] 	     = (float)mpu.gy/GYRO_FACTOR*PI/180 - gyro_bias[1];
	imu->gyro_rps[2]       = (float)mpu.gz/GYRO_FACTOR*PI/180 - gyro_bias[2];
	imu->gyro_rps_20602[0] = (float)mpu_20602.gx/GYRO_FACTOR_20602*PI/180 - gyro_bias_20602[0];
	imu->gyro_rps_20602[1] = (float)mpu_20602.gy/GYRO_FACTOR_20602*PI/180 - gyro_bias_20602[1];
	imu->gyro_rps_20602[2] = (float)mpu_20602.gz/GYRO_FACTOR_20602*PI/180 - gyro_bias_20602[2];
	
	//???????,?????,?????,??????yaw?????,????????????????
	*yaw1_after_cali  = -(atan2f((float)imu->mag1[1],(float)imu->mag1[0]) - mag_declination_rad);	
	*yaw2_after_cali  = -(atan2f((float)imu->mag2[1],(float)imu->mag2[0]) - mag_declination_rad);	
	
}


void frame_alignment(imu_scaled_body *imu)
{
	//for current installation
	imu_scaled_body imu_mem;
	memcpy(&imu_mem,imu,sizeof(imu_scaled_body));
	//robot1
	imu->acc_m_s2[0] = (imu_mem.acc_m_s2[1]);
	imu->acc_m_s2[1] = (imu_mem.acc_m_s2[0]);
	imu->acc_m_s2[2] = -imu_mem.acc_m_s2[2];//!!!!!!!!!!!!!!!!!!!temporary
	imu->acc_m_s2_20602[0] = (imu_mem.acc_m_s2_20602[0]);
	imu->acc_m_s2_20602[1] = -(imu_mem.acc_m_s2_20602[1]);
	imu->acc_m_s2_20602[2] = -imu_mem.acc_m_s2_20602[2];//!!!!!!!!!!!!!!!!!!!temporary
	
	//robot2
//	imu->acc_m_s2[0] = (imu_mem.acc_m_s2[0]-0.3);
//	imu->acc_m_s2[1] = -(imu_mem.acc_m_s2[1]-0);
//	imu->acc_m_s2[2] = -imu_mem.acc_m_s2[2]-0.17;//!!!!!!!!!!!!!!!!!!!temporary
	
	imu->gyro_rps[0] = imu_mem.gyro_rps[1];
	imu->gyro_rps[1] = imu_mem.gyro_rps[0];
	imu->gyro_rps[2] = -imu_mem.gyro_rps[2];
	imu->gyro_rps_20602[0] = imu_mem.gyro_rps_20602[0];
	imu->gyro_rps_20602[1] = -imu_mem.gyro_rps_20602[1];
	imu->gyro_rps_20602[2] = -imu_mem.gyro_rps_20602[2];	
}


void hampel_imu(imu_scaled_body *imu)
{
	u16 i;
	
	for(i=0;i<FILTER_LEN-1;i++) {
		gyro_qx[i] = gyro_qx[i+1]; gyro_qx_20602[i] = gyro_qx_20602[i+1]; 
		gyro_qy[i] = gyro_qy[i+1]; gyro_qy_20602[i] = gyro_qy_20602[i+1]; 
		gyro_qz[i] = gyro_qz[i+1]; gyro_qz_20602[i] = gyro_qz_20602[i+1];
		acc_qx[i]  = acc_qx[i+1];  acc_qx_20602[i]  = acc_qx_20602[i+1];  
		acc_qy[i]  = acc_qy[i+1];  acc_qy_20602[i]  = acc_qy_20602[i+1];  
		acc_qz[i]  = acc_qz[i+1];  acc_qz_20602[i]  = acc_qz_20602[i+1];
		mag2_qx[i] = mag2_qx[i+1]; 
		mag2_qy[i] = mag2_qy[i+1]; 
		mag2_qz[i] = mag2_qz[i+1];
	}
			
	gyro_qx[FILTER_LEN-1] = imu->gyro_rps[0]; gyro_qx_20602[FILTER_LEN-1] = imu->gyro_rps_20602[0]; 
	gyro_qy[FILTER_LEN-1] = imu->gyro_rps[1]; gyro_qy_20602[FILTER_LEN-1] = imu->gyro_rps_20602[1]; 
	gyro_qz[FILTER_LEN-1] = imu->gyro_rps[2]; gyro_qz_20602[FILTER_LEN-1] = imu->gyro_rps_20602[2];
	acc_qx[FILTER_LEN-1]  = imu->acc_m_s2[0]; acc_qx_20602[FILTER_LEN-1]  = imu->acc_m_s2_20602[0]; 
	acc_qy[FILTER_LEN-1]  = imu->acc_m_s2[1]; acc_qy_20602[FILTER_LEN-1]  = imu->acc_m_s2_20602[1]; 
	acc_qz[FILTER_LEN-1]  = imu->acc_m_s2[2]; acc_qz_20602[FILTER_LEN-1]  = imu->acc_m_s2_20602[2];
	mag2_qx[FILTER_LEN-1] = imu->mag2[0];     
	mag2_qy[FILTER_LEN-1] = imu->mag2[1];     
	mag2_qz[FILTER_LEN-1] = imu->mag2[2];
				
//	if((gyro_qx[0]!=0)&&(acc_qx[0]!=0)){
//		imu->gyro_rps[0] = hampel(gyro_qx);   imu->gyro_rps_20602[0] = hampel(gyro_qx_20602);
//		imu->gyro_rps[1] = hampel(gyro_qy);   imu->gyro_rps_20602[1] = hampel(gyro_qy_20602);
//		imu->gyro_rps[2] = hampel(gyro_qz);   imu->gyro_rps_20602[2] = hampel(gyro_qz_20602);
//		imu->acc_m_s2[0] = hampel(acc_qx);    imu->acc_m_s2_20602[0] = hampel(acc_qx_20602);
//		imu->acc_m_s2[1] = hampel(acc_qy);    imu->acc_m_s2_20602[1] = hampel(acc_qy_20602);
//		imu->acc_m_s2[2] = hampel(acc_qz);    imu->acc_m_s2_20602[2] = hampel(acc_qz_20602);
//	}
//	
	if(mag2_qx[0]!=0){
		imu->mag2[0] = hampel(mag2_qx);
//		imu->mag2[1] = hampel(mag2_qy);
//		imu->mag2[0] = hampel(mag2_qz);
	}


}

void rot_update(float quatl[4],float rotVec[3])
{
	
  float b_quat[9];
  int i0;
  int i1;
  static const signed char b[3] = { 1, 0, 0 };
	
  b_quat[0] = quatl[0] * quatl[0] + quatl[1] * quatl[1] - quatl[2] * quatl[2] - quatl[3]* quatl[3];
  b_quat[3] = 2.0F * (quatl[1] * quatl[2] - quatl[0] * quatl[3]);
  b_quat[6] = 2.0F * (quatl[1] * quatl[3] + quatl[0] * quatl[2]);
  b_quat[1] = 2.0F * (quatl[1] * quatl[2] + quatl[0] * quatl[3]);
  b_quat[4] = quatl[0] * quatl[0] - quatl[1] * quatl[1] + quatl[2] * quatl[2] - quatl[3]* quatl[3];
  b_quat[7] = 2.0F * (quatl[2] * quatl[3] - quatl[0] * quatl[1]);
  b_quat[2] = 2.0F * (quatl[1] * quatl[3] - quatl[0] * quatl[2]);
  b_quat[5] = 2.0F * (quatl[2] * quatl[3] + quatl[0] * quatl[1]);
  b_quat[8] = quatl[0] * quatl[0] - quatl[1] * quatl[1] - quatl[2] * quatl[2] + quatl[3]* quatl[3];
  for (i0 = 0; i0 < 3; i0++) {
	  rot_vec[i0] = 0.0F;
	  for (i1 = 0; i1 < 3; i1++) {
		  rotVec[i0] += b_quat[i0 + 3 * i1] * (float)b[i1];
    }
  }
}

void rot_ele_update(float quat_in[4],float eul_in[3],volatile float delta_yaw)
{
		// quaternion renormalization
		//quat2eulf(quat_in, eul_in);

    //reset filter		
		memcpy(eul_rad,eul_in,sizeof(eul_rad));       //update global variable eul_rad
		eul_rad[0] -= delta_yaw;
	  //eul_in[0] = fmod(eul_in[0],2*PI)-PI;
	  eul2quatf(eul_rad, quat);		
	  rot_update(quat,rot_vec);                     //update global variable rot_vec
}

void update_yaw1_mag(float yaw,float *yaw1_mag)
{
	*yaw1_mag = yaw;
}


void yaw_init(s8 *flag,float *yaw1_prev,float *yaw2_prev, float yaw1,float yaw2)
{
	if(*flag==1){
		*yaw1_prev = yaw1;  
		*yaw2_prev = yaw2;  
		*flag = 0;
	}
}

void filter_converge(float trace_P,float *delta_yaw,float eul_temp[3])
{
		static u8 cnt = 0;
    float trace_P_var;
	  u8 filter_conv_len = 40;
	
    trace_P_var = movvarf(trace_P);

		if((trace_P_var!=0)&&(trace_P_var<0.001)&&(is_att_valid==0)){
			is_att_valid = 1;
		}
		
		//is_att_valid = 2?,????????
		if(is_att_valid == 1){
			if(cnt<filter_conv_len)  cnt++;
			if(cnt==filter_conv_len){
//				quatl[0]=1;
//				for(i=1;i<4;i++) quatl[i] = 0;
				*delta_yaw = eul_temp[0];
				is_att_valid = 2;
			} 
		}
}

u8 odo_validation(void)
{
	float odo_r,odo_l;
	u8 flag=0;
	
	while(flag==0)
	{
		odo_l = get_left_encoder();
		odo_r = get_right_encoder();
		if((odo_l!=0)&&(odo_r!=0))
				flag = 1;
		else flag = 0;
	}
	return flag;
}

u8 imu_validation(void)
{
	T_mpu_20602 mpu;
	volatile imu_scaled_body imu;
	u8 flag=0;

	while(flag==0)
	{
		ICM20602_getMotion6(&mpu); 
		imu.acc_m_s2[0] = (float)mpu.ax/ACC_FACTOR_20602*9.8;
		imu.acc_m_s2[1] = (float)mpu.ay/ACC_FACTOR_20602*9.8;
		imu.acc_m_s2[2] = (float)mpu.az/ACC_FACTOR_20602*9.8;
		imu.gyro_rps[0] = (float)mpu.gx/GYRO_FACTOR_20602*PI/180;
		imu.gyro_rps[1] = (float)mpu.gy/GYRO_FACTOR_20602*PI/180;
		imu.gyro_rps[2] = (float)mpu.gz/GYRO_FACTOR_20602*PI/180;
		if((fabs(imu.acc_m_s2[0])>1e-6)&&(fabs(imu.acc_m_s2[1])>1e-6)&&(fabs(imu.acc_m_s2[2])>1e-6)&&(fabs(imu.gyro_rps[0])>1e-8)&&(fabs(imu.gyro_rps[1])>1e-8))
				flag = 1;
			else flag = 0;
	}
	return flag;

}

