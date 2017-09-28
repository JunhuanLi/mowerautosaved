//组合运动测试程序
	if(state == 0)
	{
		if(!motion.tracker.path_imu.pointReached)
			trackPoint(&motion.tracker,1.0, 0.0);
		else
		{
			motion.tracker.path_imu.pointReached = FALSE;
			state++;
			stop(&motion.tracker);
		}
	}
	else if(state == 1)
	{
		if(!motion.tracker.path_imu.rotationFinished)
			rotateAngle(&motion.tracker, 180, MOTION_TURN_COUNTERCLOCKWISE);
		else
		{
			motion.tracker.path_imu.rotationFinished = FALSE;
			state++;
			stop(&motion.tracker);
		}
	}
	else if(state == 2)
	{
		if(!motion.tracker.path_imu.pointReached)
			trackPoint(&motion.tracker,0.0, 0.0);
		else
		{
			motion.tracker.path_imu.pointReached = FALSE;
			state++;
			stop(&motion.tracker);
		}
	}
	else if(state == 3)
	{
		if(!motion.tracker.path_imu.rotationFinished)
			rotateAngle(&motion.tracker, 180, MOTION_TURN_CLOCKWISE);
		else
		{
			motion.tracker.path_imu.rotationFinished = FALSE;
			state=0;
			stop(&motion.tracker);
		}
	}