T_bool ontoWire(T_motion* motion, T_side enterSide)
{
	if(motion->tracker.sense.side_l == motion->tracker.sense.side_r)
	{
		if(enterSide == LEFT_STEERING)
		{
			motion->tracker.line_vel = 0.0;
			motion->tracker.angular_vel = -0.2;
		}
		else if(enterSide = RIGHT_STEERING)
		{
			motion->tracker.line_vel = 0.0;
			motion->tracker.angular_vel = 0.2;
		}
	}
	else
	{
		motion.motion_state = MOTION_STATE_MAGLINE;
	}
}
