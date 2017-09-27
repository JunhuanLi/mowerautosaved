/* Include Files */
#include "insgps.h"

/* Function Definitions */

/*
 * yaw clockwise
 * Arguments    : float pos[3]
 *                float vel[3]
 *                float *yaw
 *                float stateVec[7]
 *                float delta_odo_r
 *                float delta_odo_l
 *                float P[49]
 *                const float R[9]
 *                float D
 * Return Type  : void
 */
void odom_preprocessing(float *odom_valid,float *delta_odo_l,float *delta_odo_r)
{
	float odo_l = 0.0;
	float odo_r = 0.0;
	static float odo_r_prev = 0,odo_l_prev = 0;
		
	//get odometer data	
	odo_l = -get_left_encoder();
	odo_r = -get_right_encoder();
	
	*delta_odo_l = odo_l - odo_l_prev;
	*delta_odo_r = odo_r - odo_r_prev;
			
	if((odo_l==0)&&(odo_r==0))
		*odom_valid = 0;
	else
		*odom_valid = 1;
	
//	*odom_valid = 1;
	
	odo_l_prev = odo_l;
	odo_r_prev = odo_r;
	
}
