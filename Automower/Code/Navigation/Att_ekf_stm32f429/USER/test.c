#include "sys.h"
#include "delay.h" 
#include "usart.h" 
#include "sdram.h"   
#include "usmart.h" 
#include "mpu9250.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "ekf.h"
#include "time_control.h"
#define PI 3.141592653589793
//#define GRAV 9.7877

float grav_acc(float lat_rad)
{
  return 9.806F - 0.0260000229F * (float)cos(2.0F * lat_rad);
}

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0)	//循环发送,直到发送完毕   
    USART1->DR=c;  
} 

//传送数据给匿名四轴地面站(V4版本)
//fun:功能字. 0X01~0X1C
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X41;	//帧头
	send_buf[1]=0X41;	//帧头
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}

//发送加速度传感器数据+陀螺仪数据(传感器帧)
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值 
void mpu9250_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,float eul_deg[3])
{
	u8 tbuf[18]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	tbuf[12]=0;//因为开启MPL后,无法直接读取磁力计数据,所以这里直接屏蔽掉.用0替代.
	tbuf[13]=0;
	tbuf[14]=0;
	tbuf[15]=0;
	tbuf[16]=0;
	tbuf[17]=0;
	usart1_niming_report(0X31,tbuf,18);//传感器帧,0X02
}	

//通过串口1上报结算后的姿态数据给电脑(状态帧)
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
//csb:超声波高度,单位:cm
//prs:气压计高度,单位:mm
void usart1_report_imu(short roll,short pitch,short yaw,short csb,int prs)
{
	u8 tbuf[12];   	
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF;
	tbuf[6]=(csb>>8)&0XFF;
	tbuf[7]=csb&0XFF;
	tbuf[8]=(prs>>24)&0XFF;
	tbuf[9]=(prs>>16)&0XFF;
	tbuf[10]=(prs>>8)&0XFF;
	tbuf[11]=prs&0XFF;
	usart1_niming_report(0X01,tbuf,12);//状态帧,0X01
}   

float g_timediff_s = 0;
short aacx,aacy,aacz;	//加速度传感器原始数据
short gyrox,gyroy,gyroz;//陀螺仪原始数据 
float acc_imu_m_s2[3] = {0};
float angular_imu_rps[3] = {0};
float ax_m_s2,az_m_s2,ay_m_s2;
float gx_rps,gy_rps,gz_rps;
float QAtt[9] = {0.00111111111111111,0,0,0,0.00111111111111111,0,0,0,0.00217777777777778};
float RAtt[9] = {0.00111111111111111,0,0,0,0.00111111111111111,0,0,0,0.00111111111111111};
static float quat[4] = {1,0,0,0};
static float PAtt[16] = {0.3,0,0,0,0,0.3,0,0,0,0,0.3,0,0,0,0,0.3};
//integrated navigation part
static float pos[3] = {0,0,0};
static float vel[3] = {0,0,0};
static float P[49] = {10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10};
float Q[9] = {0.800000000000000,0,0,0,0.800000000000000,0,0,0,0.800000000000000};
float stateVec[7] = {0};
//hampel varialbes
static float gyro_group_x[11] = {0};
static float gyro_group_y[11] = {0};
static float gyro_group_z[11] = {0};
static float acc_group_x[11] = {0};
static float acc_group_y[11] = {0};
static float acc_group_z[11] = {0};
	

/***************************************main*******************************************/
int main(void)
{     
	//global variables
//	short aacx,aacy,aacz;	//加速度传感器原始数据
//	short gyrox,gyroy,gyroz;//陀螺仪原始数据 
//	float Q[9] = {6.25000000000000e-04,0,0,0,6.25000000000000e-04,0,0,0,6.25000000000000e-04};
//	float R_acc[9] = {0.000500000000000000,0,0,0,0.000500000000000000,0,0,0,0.000500000000000000};
//	static float stateVec[4] = {1,0,0,0};
//	static float P[16] = {10,0,0,0,0,10,0,0,0,0,10,0,0,0,0,10};
//	static float P_updated[16] = {0};
//	static float stateVec_updated[4] = {0};
//	float GRAV = 9.7877;
//	float acc_imu_m_s2[3] = {0};
//	float angular_imu_rps[3] = {0};
//	float ax_m_s2,az_m_s2,ay_m_s2;
//	float gx_rps,gy_rps,gz_rps;
	
//	//hampel varialbes
//	static float gyro_group_x[11] = {0};
//	static float gyro_group_y[11] = {0};
//	static float gyro_group_z[11] = {0};
//	static float acc_group_x[11] = {0};
//	static float acc_group_y[11] = {0};
//	static float acc_group_z[11] = {0};
	float gx_rps_h,gy_rps_h,gz_rps_h;
	float ax_m_s2_h,ay_m_s2_h,az_m_s2_h;

	//intermediate variables
  int i;
	float temp;
  float norm_q;
	//output data
  float eul_rad[3] = {0};   //eul = [yaw,pitch,roll]
	float eul_deg[3] = {0};   
	float grav;               //local gravity acceleration constants
	double delta_heading_rad;
	double points[3] = {0.395684269289390,1.98808721214782,0};
	//gps pseudo data
	double gps_pos_lla[3] = {0.395655,1.988094,77.70200};
	float  gps_vel_ned[3] = {0.15,0.25,0.15};
	double pos_lla_ref[3] = {0.3956,1.9881,77};
	float gps_hAcc_m = 3;
  float gps_vAcc_m = 2.4;
	float gps_sAcc_m_s = 0.3;
	//odometer pseudo data
	float delta_odo_r = 0.13;
	float delta_odo_l = 0.15;
	float diameter = 0.32;
	float R_odom[9] = {0.0200000000000000,0,0,0,0.0200000000000000,0,0,0,0.0200000000000000};

	/***************** Initialization *************************/
	Stm32_Clock_Init(360,25,2,8);//设置时钟,180Mhz
	delay_init(180);			       //初始化延时函数 
	uart_init(90,115200);		     //初始化串口波特率为115200
	usmart_dev.init(90);		     //初始化USMART
	SDRAM_Init();				         //初始化SDRAM 
  MPU_Init();             	   //初始化MPU9250
	time_control_clock_init();
  printf("\r\n");
	grav = grav_acc(0.395672197719619F);
	//delta_heading_rad = magHeading2Geog(points);

	/****************** Processing ****************************/
  while(1){
		//gyro calibration variables
		static int flag = 0;
		static float gyro_sum[3] = {0};	
		static float gyro_bias[3] = {0};
		int gyro_cali_times = 1000;	
		
		//raw data 
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
      
		/**************attitude calculation*******************/
		//raw data to scale data
		ax_m_s2 = (float)aacx/16384*grav;   ay_m_s2 = (float)aacy/16384*grav;  az_m_s2 = (float)aacz/16384*grav;
		gx_rps  = (float)gyrox/16.4*PI/180; gy_rps = (float)gyroy/16.4*PI/180; gz_rps = (float)gyroz/16.4*PI/180;

		//gyro calibration
		if(flag<gyro_cali_times){
			gyro_sum[0] = gyro_sum[0] + gx_rps;
		  gyro_sum[1] = gyro_sum[1] + gy_rps;
		  gyro_sum[2] = gyro_sum[2] + gz_rps;
		  flag++;
		}
		else if(flag==gyro_cali_times){
			gyro_bias[0] = gyro_sum[0]/gyro_cali_times;
			gyro_bias[1] = gyro_sum[1]/gyro_cali_times;
			gyro_bias[2] = gyro_sum[2]/gyro_cali_times;
  	  flag++;
		}
		else{
			u64 t1;
			gx_rps = gx_rps - gyro_bias[0];
			gy_rps = gy_rps - gyro_bias[1];
			gz_rps = gz_rps - gyro_bias[2];

		  /************outliers removal****************/	
			for(i=0;i<10;i++) {
				gyro_group_x[i] = gyro_group_x[i+1];gyro_group_y[i] = gyro_group_y[i+1];gyro_group_z[i] = gyro_group_z[i+1];
				acc_group_x[i] = acc_group_x[i+1];  acc_group_y[i] = acc_group_y[i+1];  acc_group_z[i] = acc_group_z[i+1];
			}
		
			gyro_group_x[10] = gx_rps;   gyro_group_y[10] = gy_rps;   gyro_group_z[10] = gz_rps;
			acc_group_x[10] = ax_m_s2; acc_group_y[10] = ay_m_s2;     acc_group_z[10] = az_m_s2;

			if(gyro_group_x[0]!=0){
				
				gx_rps_h = hampel11(gyro_group_x);gy_rps_h = hampel11(gyro_group_y);gz_rps_h = hampel11(gyro_group_z);
				ax_m_s2_h = hampel11(acc_group_x);ay_m_s2_h = hampel11(acc_group_y);az_m_s2_h = hampel11(acc_group_z);
				
				/**************frame alighnment***************************/
				acc_imu_m_s2[0] = ax_m_s2_h;acc_imu_m_s2[1] = -ay_m_s2_h;acc_imu_m_s2[2] = -az_m_s2_h;
				angular_imu_rps[0] = gx_rps_h;angular_imu_rps[1] = -gy_rps_h;angular_imu_rps[2] = -gz_rps_h;
				g_timediff_s = (float)get_time_diff()*0.001*0.001;

				/************ attitude calculation:time update **********/
				tu_att_ekf(PAtt,quat,angular_imu_rps,(float)g_timediff_s, QAtt);
			
				/*********** measurement update by accelerometer*************/
				mu_att_ekf(PAtt,acc_imu_m_s2,quat,RAtt);
			
				/******************quaternion renormalization *************/
				norm_q = sqrt(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3]);
				quat[0] = quat[0]/norm_q;quat[1] = quat[1]/norm_q;quat[2] = quat[2]/norm_q;quat[3] = quat[3]/norm_q;
				quat2eulf(quat, eul_rad);
				for(i=0;i<3;i++)     eul_deg[i] = eul_rad[i]*180/PI;
//				printf("yaw:   %f            ",eul_deg[0]);
//				printf("pitch: %f            ",eul_deg[1]);
//				printf("roll:  %f\r\n",eul_deg[2]);

				/***************integrated navigation: time update*************************/
				//ekf7_tu(acc_imu_m_s2, pos, vel, quat, (float)g_timediff_s, P, Q, stateVec);
				
				/***************integrated navigation: GPS measurement update*************************/
				//ekf7_gps6_mu(pos, vel, &eul_rad[0], gps_pos_lla,  gps_vel_ned, stateVec,P, pos_lla_ref, gps_hAcc_m, gps_vAcc_m, gps_sAcc_m_s);
				
				/***************integrated navigation: Odometer measurement update********************/
				//ekf7_odom_mu( pos,  vel, &eul_rad[0], stateVec, delta_odo_r, delta_odo_l, P, R_odom, diameter);
				printf("%f,%f,%f,%f,%f,%f,%f\r\n",angular_imu_rps[0],angular_imu_rps[1],angular_imu_rps[2],acc_imu_m_s2[0],acc_imu_m_s2[1],acc_imu_m_s2[2],g_timediff_s);
	      //mpu9250_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz,eul_deg);//发送加速度+陀螺仪原始数据
			  }
		}
	}	 
}

















