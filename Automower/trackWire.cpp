//沿磁导线运动测试程序
static T_motion_mag_line_orientation_type getPoseToWire(T_motion_tracker* obj)
{		
	if(obj->sense.side_l == obj->sense.side_r)
		return ON_WIRE;
	
	else if( (obj->sense.side_l != obj->sense.side_r) 
						&& (obj->sense.side_l != MOTION_MAG_LINE_MISSING) 
						&& (obj->sense.side_r != MOTION_MAG_LINE_MISSING) )
	{
		return NOT_ON_WIRE;
	}
	
	else
		return WIRE_MISSING;
}

T_bool needUTurn = FALSE;
void trackWire(T_motion_tracker* obj)
{
	magDiff = obj->sense.value_l - obj->sense.value_r;
	
	//Òª²âÊÔ¾²Ì¬²î£¬²¢ÅÅ³ýÎóÅÐ
	//if(fabsf(magDiff<1000)
	obj->path_mag_line.turn_dir = magDiff >= 0 ? MOTION_TURN_COUNTERCLOCKWISE : MOTION_TURN_CLOCKWISE;
	//magRatio = fabsf( 1-obj->sense.value_l/ obj->sense.value_r );
	
	orientation = getPoseToWire(obj);	
		
	if(orientation == WIRE_MISSING)
	{
		obj->error = MOTION_ERROR_NO_MAG_LINE;
		//stop(obj);
		return;
	}
	
	if(obj->path_mag_line.state== MOTION_MAG_LINE_STATE_IDLE)
	{
		//stop(obj);
//		if(power>0.99)
//		{
//     ¶ÏµãÐø¸î
//			obj->path_mag_line.state = MOTION_MAG_LINE_STATE_START;
//		}
		return;
	}
	if(obj->path_mag_line.state == MOTION_MAG_LINE_STATE_START)
	{
		
		if(orientation == NOT_ON_WIRE)
		{
			//Maybe in the wrong direction which should be checked later
			obj->path_mag_line.state = MOTION_MAG_LINE_STATE_GOTOLINE; 
		}
		else if(orientation == ON_WIRE)
		{
				obj->path_mag_line.state = MOTION_MAG_LINE_STATE_TRACELINE;
		}
		
			obj->path_mag_line.pre_wheelside = obj->sense.side_l;
//		obj->path_mag_line.pre_magvalue = obj->sense.value_l;
//		stop(obj);
	}
	else if(obj->path_mag_line.state == MOTION_MAG_LINE_STATE_GOTOLINE)
	{
		//Òª²âÊÔ¾²Ì¬²î (==+-3000?)
			//if the vehicle is already perpendicular to the wire, what is the maximum difference between
		 //the value form two magnetic sensor?
		if( magDiff != 0)
		{
			if(obj->path_mag_line.turn_dir == MOTION_TURN_COUNTERCLOCKWISE)
				magDiff = -magDiff; //left or right
			
			
			oOutput = PI_Run2(&obj->path_mag_line.mag_gotoline_pi, -magDiff);
			obj->angular_vel = oOutput;
			//obj->angular_vel = 0.4;
			obj->line_vel = 0.1;
		}	
		
		if( (obj->path_mag_line.pre_wheelside != obj->sense.side_l) 
							|| ((obj->path_mag_line.pre_wheelside != obj->sense.side_r)) )
		{
			//already on the wire
			//turn according to the current direction in the wire
			
			//finished
			if( (obj->sense.side_l != obj->sense.side_r) ) 
			{
				obj->path_mag_line.state = MOTION_MAG_LINE_STATE_TRACELINE;
			}
			else
			{
										
					
					//ÐèÒª±Õ»·£¬´ý¸Ä
				float ang_vel = 0.5;
					
					
					
				if(obj->path_mag_line.wire_dir == MOTION_MAG_LINE_DIRECT)
				{
					if(obj->sense.side_l == MOTION_MAG_LINE_INSIDE)
						obj->angular_vel = ang_vel; //rotate ccw
					else
						obj->angular_vel = -ang_vel;//rotate cw
				}
				else if(obj->path_mag_line.wire_dir == MOTION_MAG_LINE_REVERSE)
				{
					if(obj->sense.side_l  == MOTION_MAG_LINE_INSIDE)
						obj->angular_vel = -ang_vel;//rotate ccw
					else
						obj->angular_vel = ang_vel; //rotate cw
				}
				obj->line_vel = 0;
			}
		}
	}
	else if(obj->path_mag_line.state == MOTION_MAG_LINE_STATE_TRACELINE)
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
		//obj->angular_vel = PI_Run2(&obj->path_mag_line.mag_tracking_pi, -magDiff) );
			obj->angular_vel = 0;
			obj->line_vel = 0.1;
		//Motion_backToBase(obj);
		}
		
	}
}