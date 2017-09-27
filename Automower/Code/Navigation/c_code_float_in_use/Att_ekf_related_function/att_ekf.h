/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

void tu_att_ekf(float P[16], float stateVec[4], float angular_imu_rad_s[3], float dt, float Q[9], float P_updated[16],
                float stateVec_updated[4]);