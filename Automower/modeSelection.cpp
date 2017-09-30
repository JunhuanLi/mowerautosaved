//模式选择的测试程序
switch(g_action)
{
	case TO_POSE:
		if(!obj->path_imu.pointReached)
			trackPoint(obj, g_action_params.pos[0], g_action_params.pos[1]);
		else
		{
			obj->path_imu.pointReached = FALSE;
			g_trigger = POS_REACHED;
			make_decision(&g_trigger, &g_action, &g_action_params);
		}
		break;
	case DIR_DRIVE:
		mag_sensor_update();
		zigzag_flag++;
		
		if((mag_state.left_sensor_change == 1)&&(mag_state.right_sensor_change == 1)&&(zigzag_flag>100))
		{
			zigzag_flag = 0;
			//stop(&motion->tracker);
			g_trigger = WIRE_SENSED;
			make_decision(&g_trigger, &g_action, &g_action_params);
		}
		else
			{
				if(pre_action == U_TURN)
					trackVector(&motion->tracker,g_action_params.u_turn_.fin_vec[0], g_action_params.u_turn_.fin_vec[1] );
				else
					trackVector(&motion->tracker,g_action_params.dir_drive_.fin_vec[0], g_action_params.dir_drive_.fin_vec[1] );		
			}
			
		mag_state.right_sensor_change = 0;
		mag_state.left_sensor_change = 0;
		
		break;
	case U_TURN:
		zigzag_flag=0;
		if(!motion->tracker.path_imu.rotationFinished)
		{
			rotateVector(&motion->tracker, g_action_params.u_turn_.fin_vec[0], 
									g_action_params.u_turn_.fin_vec[1] , g_action_params.u_turn_.turn_side);
			
			motion->tracker.line_vel = 0.05;//???????????,????0.08m/s
			
		}
		else
		{
			motion->tracker.path_imu.rotationFinished = FALSE;
			pre_action = U_TURN;
			g_action = DIR_DRIVE;
			//stop(&motion->tracker);	
		}
	
		break;
	case ONTO_WIRE:
		break;
	case BY_PASS:
		break;
	default:
		break;
}
