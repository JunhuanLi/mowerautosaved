/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: global_planner.c
  Author		: kangshaodong
  Version		: V1.0.0      
  Date			: 2017/09/12
  Description	: providing plan result for local planner
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#include "global_planner.h"


/* Variables Definition */
T_pose *g_intercell_path; 

int g_trig_pos_reached = 0; 
int g_trig_dir_reached = 0; 
int g_trig_wire_sensed_plain = 0; 
int g_trig_rigid_touched_plain = 0;
int g_trig_wire_bstr_surge = 0; 
int g_trig_wire_bstr_plunge = 0; 
int g_trig_rigid_bstr_sigchg = 0;
int g_trig_cell_end_reached = 0; 
int g_trig_chrgst_reached = 0; 

int g_opt_remap = 1; 
int g_flg_map_loaded = 0; 
int g_flg_mapping = 1; 
int g_flg_sens_stp_unoccur = 1;
int g_flg_bstr_progress_side = 1;

T_pose g_orig_pose_in_lawn;

T_pose g_curr_pose_enu; 
T_pose g_curr_pose_in_lawn; 
T_pose g_curr_pose_bstr;	

T_pose g_lst_sens_stp_pose_enu;
T_pose g_lst_sens_stp_pose_in_lawn;
T_pose g_lst_sens_stp_pose_bstr;

float g_curr_bstr_width = 0.0;
float g_lst_bstr_width = 0.0;

float g_rot_lawn_in_enu; 
float g_rot_bstr_in_lawn = 0.0; 


/* Functions Definition */

// The general interface with Motion Control module
int make_decision(T_trigger *trigger, T_action *action_out, T_params_act *params_out)
{
	if(!g_flg_map_loaded)
		planner_init();
	
	//reset all triggers
	g_trig_pos_reached = 0; 
	g_trig_dir_reached = 0; 
	g_trig_wire_sensed_plain = 0; 
	g_trig_rigid_touched_plain = 0;
	g_trig_wire_bstr_surge = 0; 
	g_trig_wire_bstr_plunge = 0; 
	g_trig_rigid_bstr_sigchg = 0;
	g_trig_cell_end_reached = 0; 
	g_trig_chrgst_reached = 0; 
	
	event_preproc(trigger);
	decision_assert(action_out, params_out);
	
	return 0;
}


// Planner initializing
int planner_init()
{
	float pos_ned[3]; 
	float dir_ned[3]; 
	
	//remap option choosed, or loading the existed map failed
	if(g_opt_remap || (-1 == map_load())) 
	{ 
		map_init();	
		g_opt_remap = 0; //once loaded over, switch to FALSE by default.
	}
	
	g_orig_pose_in_lawn.pos.x = 0.0;
	g_orig_pose_in_lawn.pos.y = 0.0;
	g_orig_pose_in_lawn.th = 0.0;
	
	//initialize the lawn	coordinate frame
	get_pos_ned(pos_ned);
	get_rot_vector(dir_ned);
	g_curr_pose_enu.pos.x = pos_ned[1];
	g_curr_pose_enu.pos.y = pos_ned[0];
	g_curr_pose_enu.th = atan2(dir_ned[0], dir_ned[1]);
	
	if(pow(g_curr_pose_enu.pos.x, 2)+pow(g_curr_pose_enu.pos.y, 2) <= START_POS_TOLERANCE)
		g_rot_lawn_in_enu = g_curr_pose_enu.th;
	else
		printf("The lawn coordinates initialization failed!");
	
	g_flg_map_loaded = 1; 
}


// Event Preprocessing, convert the raw triggers to the derived triggers. 
// ### OUTPUT: the 7 trigger flags ###
int event_preproc(T_trigger *trigger)
{
	switch(*trigger)
	{
		case POS_REACHED:
			g_trig_pos_reached = 1;
			break;
		
		case DIR_REACHED:
			g_trig_dir_reached = 1;
			break;
		
		case WIRE_SENSED: 
			get_pose_in_lawn();
			if(1 == g_flg_sens_stp_unoccur)	//sensing stop for the first time
			{
				g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
				g_lst_bstr_width = 0.0;
				
				g_flg_sens_stp_unoccur = 0;
			}
			else 
			{
				if(0 == g_lst_bstr_width)		//sensing stop for the second time
				{
					g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
					g_lst_bstr_width = dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
				}
				else
				{
					g_curr_bstr_width = dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
					
					if(g_curr_bstr_width < DEAD_END_CLEARENCE)
						g_trig_cell_end_reached = 1;
					else if(g_curr_bstr_width - g_lst_bstr_width >= CELL_WCHG_UPPER_BOUND)
						g_trig_wire_bstr_surge = 1;
					else if(g_curr_bstr_width - g_lst_bstr_width <= -CELL_WCHG_UPPER_BOUND)
						g_trig_wire_bstr_plunge = 1;
					else
						g_trig_wire_sensed_plain = 1;
					
					g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
					g_lst_bstr_width = g_curr_bstr_width;
				}
			}
			break;
		
		case RIGID_TOUCHED:	
			get_pose_in_lawn();
			if(1 == g_flg_sens_stp_unoccur)	//sensing stop for the first time
			{
				g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
				g_lst_bstr_width = 0.0;
				
				g_flg_sens_stp_unoccur = 0;
			}
			else 
			{
				if(0 == g_lst_bstr_width)		//sensing stop for the second time
				{
					g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
					g_lst_bstr_width = dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
				}
				else
				{
					g_curr_bstr_width = dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
					
					if(dist(g_curr_pose_in_lawn, g_orig_pose_in_lawn) <= CHRGST_DOMAIN_RADIUS)
						g_trig_chrgst_reached = 1;
					else if(g_curr_bstr_width < DEAD_END_CLEARENCE)
						g_trig_cell_end_reached = 1;
					else if(fabs(g_curr_bstr_width - g_lst_bstr_width) >= CELL_WCHG_UPPER_BOUND)
						g_trig_rigid_bstr_sigchg = 1;
					else
						g_trig_rigid_touched_plain = 1;
					
					g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
					g_lst_bstr_width = g_curr_bstr_width;
				}
			}
			break;
		
		default:
			break;
	}
}


// Choose a decision according to the event
// ### OUTPUT: action_out and params_out ###
int decision_assert(T_action *action_out, T_params_act *params_out)
{	
	T_param_into_dir op_dir;
	//assert: TO_POSE
	if((1 == g_trig_cell_end_reached) && (1 == strlen("graph untraversal"))) //NOTICE: UNFINISHED!
	{
		*action_out = TO_POSE;
		// params_out = pos; //pick up the right or left start point in the obj cell
	}
	
	//assert: INTO_DIR
	//########## TO BE IMPLEMENTED ##########
	
	int t1 = g_trig_wire_sensed_plain; 
	int t2 = g_trig_rigid_touched_plain;
	int t3 = g_trig_wire_bstr_surge; 
	int t4 = g_trig_wire_bstr_plunge; 
	int t5 = g_trig_rigid_bstr_sigchg;
	if((1 == t1)||(1 == t2)||(1 == t3)||(1 == t4)||(1 == t5))
	{
		if((1 == t4) && (1 == g_flg_mapping))
		{ 
			//assert: ONTO_WIRE
			*action_out = ONTO_WIRE;
			params_out->onto_wire_.entry_side = compute_turn_side();
			op_dir = tf_dir_bstr_to_ned(compute_opposite_dir());
			params_out->onto_wire_.set_dir[0] = op_dir.vec[0];
			params_out->onto_wire_.set_dir[1] = op_dir.vec[1];
			params_out->onto_wire_.set_dir[2] = op_dir.vec[2];
			params_out->onto_wire_.set_laps = 2; 
		}
		else
		{
			//assert: U-TURN
			*action_out = U_TURN;
			params_out->u_turn_.side = compute_turn_side();
			
			if(((1 == t5)||(1 == t2)) && (1 == g_flg_mapping))
			{
				//assert: need to record
				record_track();
			}
			if(((1 == t4)||(1 == t3)||(1 == t5)) && (0 == g_flg_mapping))
			{
				//assert: enter into a new cell(adjacency graph)
				//########## TO BE IMPLEMENTED ##########
			}
		}
	}

	//assert: BYPASS
	//########## TO BE IMPLEMENTED ##########
	
}


// The global planner
int cell_cross()
{
	
}


// Implement the basic A* algorithm at first(update later)
int get_global_path(T_pose start_in_lawn, T_pose goal_in_lawn, T_pose *path_out)
{
	T_point_m start;
	T_point_m goal;
	
	start.x = (int)(start_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
	start.y = (int)(start_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
	goal.x = (int)(goal_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
	goal.y = (int)(goal_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
	
	//A star algorithm
	//########## TO BE IMPLEMENTED ##########
	
}


// Get the next pose in path
int get_local_goal(T_pose *path_in_lawn, T_pose *local_goal_out)
{
		
}


// Record the track of mower into the active map
int record_track()
{
	//record track when mapping mode is active
	if(g_flg_mapping)
	{
		get_pose_in_lawn();
		g_pose_on_map.pos.x = (int)(g_curr_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
		g_pose_on_map.pos.y = (int)(g_curr_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
		g_pose_on_map.th = g_curr_pose_in_lawn.th; 
		
		//write data into the map
		set_grid_value(sub2ind(g_map.info.width, g_map.info.height,	g_pose_on_map.pos.x, g_pose_on_map.pos.y), OCC_OBSTACLE);
	}
	
	return 0;
}


// Get pose(lawn coord) from integrated navigation(@Fang Rui)
int get_pose_in_lawn()
{
	float pos_ned[3]; 
	float dir_ned[3]; 
	
	get_pos_ned(pos_ned);
	get_rot_vector(dir_ned);
	g_curr_pose_enu.pos.x = pos_ned[1];
	g_curr_pose_enu.pos.y = pos_ned[0];
	g_curr_pose_enu.th = atan2(dir_ned[0], dir_ned[1]);
	
	g_curr_pose_in_lawn = rotation_tf(g_curr_pose_enu, g_rot_lawn_in_enu); 
}


// Get pose(bstr coord)
int get_pose_bstr()
{
	get_pose_in_lawn();
	g_curr_pose_bstr = rotation_tf(g_curr_pose_in_lawn, g_rot_bstr_in_lawn); 
}


// Pose transformation from lawn to NED coordinate
T_param_to_pose tf_lawn_to_ned(T_pose pose_in_lawn)
{
	T_pose pose_enu;
	T_param_to_pose pose_ned;
	
	pose_enu = rotation_tf(pose_in_lawn, -g_rot_lawn_in_enu); 
	pose_ned.pos[1] = pose_enu.pos.x; 
	pose_ned.pos[0] = pose_enu.pos.y; 
	pose_ned.pos[2] = 0;
	pose_ned.vec[1] = cos(pose_enu.th);
	pose_ned.vec[0] = sin(pose_enu.th);
	pose_ned.vec[2] = 0;
	
	return pose_ned;
}


// Direction transformation from boustrophedon to NED coordinate
T_param_into_dir tf_dir_bstr_to_ned(float dir_in_bstr)
{
	float dir_in_lawn;
	float dir_enu;
	T_param_into_dir dir_ned;
	
	dir_in_lawn = angle_sub(dir_in_bstr, -g_rot_bstr_in_lawn);
	dir_enu = angle_sub(dir_in_lawn, -g_rot_lawn_in_enu);
	dir_ned.vec[1] = cos(dir_enu);
	dir_ned.vec[0] = sin(dir_enu);
	dir_ned.vec[2] = 0;
	
	return dir_ned;
}


// Compute turn side
T_side compute_turn_side()
{
	T_side side;
	get_pose_bstr();
	
	if(1 == g_flg_bstr_progress_side)
	{
		if(fabs(angle_sub(g_curr_pose_bstr.th, PI_/2)) <= ANGLE_TOLERANCE)
			side = RIGHT;
		if(fabs(angle_sub(g_curr_pose_bstr.th, -PI_/2)) <= ANGLE_TOLERANCE)
			side = LEFT;
	}
  else
	{
		if(fabs(angle_sub(g_curr_pose_bstr.th, PI_/2)) <= ANGLE_TOLERANCE)
			side = LEFT;
		if(fabs(angle_sub(g_curr_pose_bstr.th, -PI_/2)) <= ANGLE_TOLERANCE)
			side = RIGHT;
	}
	
	return side;
}


// Compute opposite direction
float compute_opposite_dir()
{
	float op_dir;
	get_pose_bstr();
	
	if(fabs(angle_sub(g_curr_pose_bstr.th, PI_/2)) <= ANGLE_TOLERANCE)
		op_dir = -PI/2;
	else if(fabs(angle_sub(g_curr_pose_bstr.th, -PI_/2)) <= ANGLE_TOLERANCE)
		op_dir = PI/2;
	else
		op_dir = 0; //############## NOTICE: JUST FOR TEST!!!################
	
	return op_dir;
}


// Make sure the SUM of 2 angles is in [-pi,+pi)
float angle_add(float ang_a, float ang_b)
{
	float ang_sum = ang_a + ang_b;
	
	while(ang_sum < -PI_)
		ang_sum += 2*PI_;
	
	while(ang_sum >= PI_)
		ang_sum -= 2*PI_;
	
	return ang_sum;
}


// Make sure the DIFFERENCE of 2 angles is in [-pi,+pi)
float angle_sub(float ang_a, float ang_b)
{
	float ang_diff = ang_a - ang_b;
	
	while(ang_diff < -PI_)
		ang_diff += 2*PI_;
	
	while(ang_diff >= PI_)
		ang_diff -= 2*PI_;
	
	return ang_diff;
}


// Euler distance of two point2d
float dist(T_pose pose1, T_pose pose2)
{
	return sqrt(pow(pose1.pos.x-pose2.pos.x, 2) + pow(pose1.pos.y-pose2.pos.y, 2));
}


// Rotation transformation
T_pose rotation_tf(T_pose pose_in_par_coord, float ang_of_chd_coord)
{
	T_pose pose_in_chd_coord;
	
	pose_in_chd_coord.pos.x = pose_in_par_coord.pos.x*cos(ang_of_chd_coord) + pose_in_par_coord.pos.y*sin(ang_of_chd_coord);
	pose_in_chd_coord.pos.y = -pose_in_par_coord.pos.x*sin(ang_of_chd_coord) + pose_in_par_coord.pos.y*cos(ang_of_chd_coord);
	pose_in_chd_coord.th = angle_sub(pose_in_par_coord.th, ang_of_chd_coord);
	
	return pose_in_chd_coord;
}



