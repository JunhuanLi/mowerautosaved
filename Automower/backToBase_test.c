void traceWire(T_motion_tracker* obj)
{
	//·´Ïò£¬ÐèÒªÐý×ª180¶È
		if( (obj->path_mag_line.wire_dir == MOTION_MAG_LINE_DIRECT)
				&&(obj->sense.side_r == MOTION_MAG_LINE_INSIDE)
				&& (obj->sense.side_l == MOTION_MAG_LINE_OUTSIDE) )
		{
			needUTurn = TRUE;
		}
		
		if( (needUTurn)&&(!obj->path_imu.rotationFinished) )
			rotateAngle(obj, 180, MOTION_TURN_COUNTERCLOCKWISE);
		else
		{
			needUTurn = FALSE;
			obj->path_imu.rotationFinished = FALSE;
			obj->path_mag_line.state = MOTION_MAG_LINE_STATE_START;
			obj->angular_vel = PI_Run2(&obj->path_mag_line.mag_tracking_pi, -magDiff);
			//obj->angular_vel = 0;
			obj->line_vel = 0.1;
		//Motion_backToBase(obj);
		}
}
	
//#define DIST_THRESHOLD 0.1
//void Motion_backToBase(T_motion_tracker* obj)
//{
//	sonar = getSonar(obj);
//	bumper = getBumper(obj);
//	chargingFailed = getCharging(obj);
//	
//	if(sonar<DIST_THRESHOLD)
//	{
//		if(!aligned)
//		{
//			obj->angular_vel = PI_Run2(&obj->path_mag_line.mag_tracking_pi, -magRatio) * 6.98;
//			obj->line_vel = 0;
//			if(magRatio<0.1)
//				aligned = TRUE;
//		}
//		else if(aligned) 
//		{
//			obj->line_vel = obj->target_vel * sonar/DIST_THRESHOLD;
//			obj->angular_vel = 0;
//			if(bumper&& (!charging))//5cm
//				chargingFailed = TRUE;
//			else if(charging)
//				obj->path_mag_line.state = MOTION_MAG_LINE_STATE_IDLE;
//			
//			if(chargingFailed)
//			{
//				obj->line_vel = -obj->target_vel * sonar/DIST_THRESHOLD;
//				obj->angular_vel = 0;
//			}
//		}
//	}
//	else
//	{
//		chargingFailed = FALSE;
//		aligned = FALSE;
//		obj->line_vel = obj->target_vel;
//	}
//		
//}
