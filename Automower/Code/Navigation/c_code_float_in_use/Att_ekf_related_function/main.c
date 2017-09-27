#include <stdio.h>

/* Function Declarations */
void tu_att_ekf(float P[16], float stateVec[4], float angular_imu_rad_s[3], float dt, float Q[9], float P_updated[16],
                float stateVec_updated[4]);
void mu_att_ekf(float P[16], float acc_imu_g[3], float stateVec[4], float R[9], float P_updated[16], float stateVec_updated[4]);
void eul2quatf(float eul[3], float q[4]);
void quat2eulf(float q[4], float eul[3]);
void hampel11(float x[11], float xfilt[11]);
//void wgslla2xyz(double lat_rad, double lon_rad, double alt_m, double p_e[3]);
//void wgsxyz2ned(double p_e[3], double ref_lat, double ref_lon, double ref_alt, double ned[3]);
void ekf7_tu(float acc_m_s2[3], float pos_prev[3], float vel_prev[3], float yaw_prev, float quat[4],float dt, 
			  float P[49], float Q[9], float pos[3], float vel[3], float *yaw, float stateVec_updated[7],float P_updated[49]);
void ekf7_gps6_mu(float pos[3], float vel[3], float *yaw, double gps_pos_lla[3], float gps_vel_ned[3], float stateVec[7],
                  float P[49], double pos_lla_ref[3], float gps_hAcc_m, float gps_vAcc_m, float gps_sAcc_m_s);
void ekf7_odom_mu(float pos[3], float vel[3], float *yaw, float stateVec[7], float delta_odo_r, float delta_odo_l, float P[49], 
	                float R[9], float D);
double magHeading2Geog(double points[3]);

void main(void)
{
	//PART1: Attitude calculation
	/*float stateVec[4] = {1,0,0,0};
	float angular_imu_rad_s[3] = {0.1,0.2,0.1};
	float dt = 0.02;
	float Q[9] = {6.25000000000000e-06,0,0,0,6.25000000000000e-06,0,0,0,6.25000000000000e-06};
	float P[16] = {10,0,0,0,0,10,0,0,0,0,10,0,0,0,0,10};
	float R[9] = {0.000500000000000000,0,0,0,0.000500000000000000,0,0,0,0.000500000000000000};
	float P_updated[16] = {0};
	float stateVec_updated[4] = {0};
	float acc_imu_g[3] = {0.1,0.1,9.79};
	float q1[4] = {0,0.7071,-0.7071,0};
	float eul1[3] ={0};
	float q2[4] = {0};
	float eul2[3] = {0.1,0.1,0.2};
	float x[11] = {1,-2,5,1,-1,0,25,6,17,2,1};
	float xfilt[11] = {0};
	int alert=0;
    double lat_rad = 0.3956550;
	double lon_rad = 1.9880949;
	double alt_m = 77.7020000;
	double gps_pos_ecef1[3] = {0};
	double gps_pos_ecef2[3] = {-2386519.5716783,5383089.8668291,2443003.8835551};
	double ref_lat_rad = 0.3956;
	double ref_lon_rad = 1.9881;
	double ref_alt_m = 77;
	double pos_ned_m[3] = {0};*/

	/* Attitude computation */
	//tu_att_ekf(P,stateVec,angular_imu_rad_s,dt,Q,P_updated,stateVec_updated);
	//mu_att_ekf(P,acc_imu_g,stateVec,R,P_updated,stateVec_updated);

	/* Euler/Quaternion conversion */
	//quat2eulf(q1,eul1);
	//if(eul1[0]==-1||eul1[1]==-1||eul1[2]==-1)
	//	alert = 1;
	
	//eul2quatf(eul2,q2);
	//if(q2[0]==-1||q2[1]==-1||q2[2]==-1||q2[3]==-1)
	//	alert = 1;

	/* Hampel function on 11 points*/
	//hampel11(x, xfilt);

	/* lla cooridinates in wgs84 frame to xyz coordinates in ecef frame */
    // Double
	//wgslla2xyz(lat_rad, lon_rad, alt_m, gps_pos_ecef1);

	/* lla coordinates in wgs84 frame to ned frame */
	// Double
	//wgsxyz2ned(gps_pos_ecef2, ref_lat_rad, ref_lon_rad, ref_alt_m, pos_ned_m);

    //PART 2: integrated navigation
	
    float P[49] = {10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,10};
	float acc_m_s2[3] = {0.1,0.1,0.1};
	float pos_prev[3] = {0,0,0};
	float vel_prev[3] = {0,0,0};
	float yaw_prev = 0.0860793218;
	float quat[4] = {1,0,0,0};
	float dt = 0.02;
	float Q[9] = {0.800000000000000,0,0,0,0.800000000000000,0,0,0,0.800000000000000};
	float pos[3] = {0};
	//float  vel[3] = {0};
	float vel[3] = {0};
	float yaw = -0.0292659216;
	float stateVec_updated[7] = {0};
	float P_updated[49] = {0};
	float stateVec[7] = {0,0,0,0,0,0,0};
	double gps_pos_lla[3] = {0.395655,1.988094,77.70200};
	float gps_vel_ned[3] = {0.15,0.25,0.15};
	double pos_lla_ref[3] = {0.3956,1.9881,77};
	float gps_hAcc_m = 3;
    float gps_vAcc_m = 2.4;
	float gps_sAcc_m_s = 0.3;

	float delta_odo_r = 0.13;
	float delta_odo_l = 0.15;
	float diameter = 0.32;
	float R_odom[9] = {0.0200000000000000,0,0,0,0.0200000000000000,0,0,0,0.0200000000000000};
	
	double delta_heading_rad;
	double points[3] = {0.395684269289390,1.98808721214782,0};
	/* integrated navigation: time update */
	//ekf7_tu(acc_m_s2, pos_prev, vel_prev, yaw_prev, quat, dt, P, Q,pos, vel,&yaw, stateVec_updated, P_updated);

	/* measurement update: gps */
    //ekf7_gps6_mu(pos,vel,&yaw,gps_pos_lla,gps_vel_ned,stateVec,P,pos_lla_ref,gps_hAcc_m,gps_vAcc_m,gps_sAcc_m_s);
	
	/* meansurement update: odometry */
	ekf7_odom_mu( pos,  vel, &yaw, stateVec, delta_odo_r, delta_odo_l, P, R_odom, diameter);
	
	/* magnetic declination angle calculation */
	//delta_heading_rad = magHeading2Geog(points);
	
}
