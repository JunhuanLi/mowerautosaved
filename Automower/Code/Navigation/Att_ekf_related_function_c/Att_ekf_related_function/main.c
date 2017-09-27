#include <stdio.h>

/* Function Declarations */
void tu_att_ekf(double P[16], double stateVec[4], double angular_imu_rad_s[3], double dt, double Q[9], double P_updated[16],
                double stateVec_updated[4]);
void mu_att_ekf(double P[16], double acc_imu_g[3], double stateVec[4], double R[9], double P_updated[16], double stateVec_updated[4]);
void eul2quatf(double eul[3], double q[4]);
void quat2eulf(double q[4], double eul[3]);
void hampel11(double x[11], double xfilt[11]);


void main(void)
{
	double stateVec[4] = {1,0,0,0};
	double angular_imu_rad_s[3] = {0.01,0.02,0.01};
	double dt = 0.02;
	double Q[9] = {6.25000000000000e-06,0,0,0,6.25000000000000e-06,0,0,0,6.25000000000000e-06};
	double P[16] = {10,0,0,0,0,10,0,0,0,0,10,0,0,0,0,10};
	double R[9] = {0.000500000000000000,0,0,0,0.000500000000000000,0,0,0,0.000500000000000000};
	double P_updated[16] = {0};
	double stateVec_updated[4] = {0};
	double acc_imu_g[3] = {0.1,0.1,9.79};
	double q1[4] = {1,0,0,0};
	double eul1[3] ={0};
	double q2[3] = {0};
	double eul2[3] = {0.1,0.1,0.2};
	double x[11] = {1,-2,5,1,-1,0,25,6,17,2,1};
	double xfilt[11] = {0};
	/* Attitude computation */
	//tu_att_ekf(P,stateVec,angular_imu_rad_s,dt,Q,P_updated,stateVec_updated);
	//mu_att_ekf(P,acc_imu_g,stateVec,R,P_updated,stateVec_updated);

	/* Euler/Quaternion conversion */
	//quat2eulf(q1,eul1);
	//eul2quatf(eul2,q2);

	/* Hampel function on 11 points*/

	hampel11(x, xfilt);
}