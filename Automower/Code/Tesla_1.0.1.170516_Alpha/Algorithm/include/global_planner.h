/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: global_planner.h
  Author		: kangshaodong
  Version		: V1.0.0      
  Date			: 2017/09/12
  Description	: Head file for the "global_planner.c"
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/
#ifndef __GLOBAL_PLANNER_H__
#define __GLOBAL_PLANNER_H__

#include "map_server.h"


#define PI_ 3.14159265						//WHAT ACCURACY is 'float' type allowed?
#define START_POS_TOLERANCE 0.15 
#define ANGLE_TOLERANCE 0.1745 		//in arc
#define CELL_WCHG_UPPER_BOUND 1
#define DEAD_END_CLEARENCE 0.3
#define CHRGST_DOMAIN_RADIUS 0.3


/* Structs Declaration */
// Enumerators
typedef enum					//type of trigger signal
{
	POS_REACHED,				//has reached the current target postion.
	DIR_REACHED,				//has reached the set direction by the set times.
	WIRE_SENSED,				//has sensed the wire.
	RIGID_TOUCHED				//has touched rigid obstacle.
} T_trigger;

typedef enum					//type of action
{
	TO_POSE,						//drive to a pose.								 Param: pos[x,y,z], vec[a,b,c]
	INTO_DIR,						//drive into some direction.			 Param: vec[a,b,c]
	U_TURN,							//make a U-turn										 Param: turn_side
	ONTO_WIRE,					//drive along the wire.						 Param: entry_side, set_laps, set_dir_ned
	BYPASS							//bypass an obstacle along an arc. Param: pass_side
} T_action;

typedef enum					//type of turning side
{
	LEFT,
	RIGHT
} T_side;

// Param Structures
typedef struct
{
	float pos[3];
	float vec[3]; 	//in NED coordinate, compatible with @Fang Rui
} T_param_to_pose;

typedef struct
{
	float vec[3]; 	//in NED coordinate, compatible with @Fang Rui
} T_param_into_dir;

typedef struct
{
	T_side side;
} T_param_u_turn;

typedef struct 
{
	T_side entry_side;
	int set_laps;
	float set_dir[3];	//in NED coordinate, compatible with @Fang Rui
} T_param_onto_wire;

typedef struct
{
	T_side pass_side;
} T_param_bypass;

typedef struct
{
	T_param_to_pose to_pose_;
	T_param_into_dir into_dir_;
	T_param_u_turn u_turn_;
	T_param_onto_wire onto_wire_;
	T_param_bypass bypass_;
} T_params_act;


/* Variables Declaration */
extern T_pose *g_intercell_path;	//the output path

//derived triggers
extern int g_trig_pos_reached; 
extern int g_trig_dir_reached; 
extern int g_trig_wire_sensed_plain; 
extern int g_trig_rigid_touched_plain;
extern int g_trig_wire_bstr_surge; 
extern int g_trig_wire_bstr_plunge; 
extern int g_trig_rigid_bstr_sigchg;
extern int g_trig_cell_end_reached; 
extern int g_trig_chrgst_reached; 

//status flags
extern int g_opt_remap;						//OPTION: remap or not
extern int g_flg_map_loaded;			//FLAG: map loaded or not
extern int g_flg_mapping;					//FLAG: on mapping or not
extern int g_flg_sens_stp_unoccur;	//FLAG: first sensing stop(wire or rigid) //NOTICE: need to reset when bstr dir changed!
extern int g_flg_bstr_progress_side;		//0: left hand side, 1: right hand side

//transient data
extern T_pose g_orig_pose_in_lawn;	//the charge station pose in lawn(0,0,0)

extern T_pose g_curr_pose_enu;			//the current pose of robot in ENU coordinate
extern T_pose g_curr_pose_in_lawn;	//the current pose of robot in lawn coordinate
extern T_pose g_curr_pose_bstr; 		//the current pose of robot in boustrophedon coordinate

extern T_pose g_lst_sens_stp_pose_enu;			//the last sensing stop(wire or rigid) pose in ENU coordinate
extern T_pose g_lst_sens_stp_pose_in_lawn;	//the last sensing stop(wire or rigid) pose in lawn coordinate
extern T_pose g_lst_sens_stp_pose_bstr;			//the last sensing stop(wire or rigid) pose in boustrophedon coordinate

extern float g_curr_bstr_width;			// the current boustrophedon width
extern float g_lst_bstr_width;			// last boustrophedon width

//coordinate relations
extern float g_rot_lawn_in_enu;					//rotation(arc) of lawn coord refer to inav coord frame
extern float g_rot_bstr_in_lawn;				//rotation(arc) of bstr coord refer to lawn coord frame(mapping: 0.0, planning: pi/2)

/* Functions Declaration */
int make_decision(T_trigger *trigger, T_action *action_out, T_params_act *params_out);
int planner_init();
int event_preproc(T_trigger *trigger);
int decision_assert(T_action *action_out, T_params_act *params_out);
int cell_cross();
int get_global_path(T_pose start_in_lawn, T_pose goal_in_lawn, T_pose *path_out);
int get_local_goal(T_pose *path_in_lawn, T_pose *local_goal_out);

int record_track(); 
int get_pose_in_lawn(); 
int get_pose_bstr(); 
T_param_to_pose tf_lawn_to_ned(T_pose pose_in_lawn);
T_param_into_dir tf_dir_bstr_to_ned(float dir_in_bstr);
T_side compute_turn_side();
float compute_opposite_dir();

float angle_add(float ang_a, float ang_b);
float angle_sub(float ang_a, float ang_b);
float dist(T_pose pose1, T_pose pose2);
T_pose rotation_tf(T_pose pose_in_par_coord, float ang_of_chd_coord);

#endif
