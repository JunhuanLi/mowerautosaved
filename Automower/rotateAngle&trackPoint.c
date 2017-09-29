void rotateAngle(T_motion_tracker* obj, float angle, T_motion_turn_type rot_dir) //angle is in degree
{
	//×ª½Ç¶È²âÊÔÏà¹Ø²ÎÊý
	float											pi_out=0.0;
	float 										angleRotated=0.0;
	float											dot_product=0.0;
	float 										cross_product = 0.0;
	float											err=0.0;
	
	if(obj->path_imu.preDirStored == FALSE)
	{
		obj->path_imu.pre_dir_x = obj->sense.dir_x;
		obj->path_imu.pre_dir_y = obj->sense.dir_y;
		obj->path_imu.preDirStored = TRUE;
	}
	
	dot_product = (obj->sense.dir_x * obj->path_imu.pre_dir_x)+(obj->sense.dir_y * obj->path_imu.pre_dir_y);
	cross_product = (obj->sense.dir_x * obj->path_imu.pre_dir_y) - (obj->sense.dir_y * obj->path_imu.pre_dir_x);
	
	if(dot_product > 1.0)
		dot_product = 1.0;
	
	angleRotated = acosf(dot_product)*57.3;
	if(cross_product<0.1)
		angleRotated = -angleRotated;
	 
	err = angle - angleRotated;
	if(fabsf(err)<=1) //in degree
	{
		obj->path_imu.preDirStored = FALSE;
		obj->path_imu.cornerAngle_pi.integral = 0;
		obj->path_imu.rotationFinished = TRUE;
	}
	else
	{
		pi_out = PI_Run2(&(obj->path_imu.cornerAngle_pi),-err);
		obj->line_vel = 0;
		if(rot_dir == MOTION_TURN_CLOCKWISE)
		{
				obj->angular_vel = pi_out;
		}	
		else
		{
				obj->angular_vel = -pi_out;
		}	
		obj->path_imu.rotationFinished = FALSE;
	}
}

void trackPoint(T_motion_tracker* obj, float target_x, float target_y)
{
	
	//×ßÖ±ÏßµãÎ»¿ØÖÆ²âÊÔ³ÌÐòÏà¹Ø²ÎÊý
	float vec_x = 0.0;
	float vec_y = 0.0;
	float dp1 = 0.0;
	float cp = 0.0;
	float error = 0.0;
	float dist_sq = 0.0;
	T_motion_turn_type 	rot_dir;
	
	vec_x = target_x- obj->sense.pos_x;
	vec_y = target_y - obj->sense.pos_y;
	
	dist_sq = vec_x*vec_x + vec_y*vec_y;
	
	if(dist_sq < 0.0025)
	{
		obj->path_imu.pointReached = TRUE;
	}
	else
	{
		Motion_Norm_2D(&vec_x,&vec_y);
		dp1 = vec_x*obj->sense.dir_x + vec_y*obj->sense.dir_y;
		cp = vec_x*obj->sense.dir_y - vec_y*obj->sense.dir_x;
		
		if(dp1 > 1.0f)
			dp1 = 1.0f;

		error = acosf(dp1)*57.4;
		rot_dir = (cp<0)?MOTION_TURN_CLOCKWISE:MOTION_TURN_COUNTERCLOCKWISE;
		
		if(rot_dir == MOTION_TURN_COUNTERCLOCKWISE)
			error = -error;
		if(error != 0.0f)
		{		
			obj->angular_vel = PI_Run2(&(obj->path_imu.lineAngle_pi),-error);
			obj->line_vel = 0.1;
		}
		obj->path_imu.pointReached = FALSE;
	}
	
}
