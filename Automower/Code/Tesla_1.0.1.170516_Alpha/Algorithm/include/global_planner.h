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


#define PI_ 3.14159265								//WHAT ACCURACY is 'float' type allowed?
#define START_POS_TOL 0.15 
#define HEADING_ANGLE_TOL 1.3963 			//1.3963arc = 80degree, 0.1745arc = 10degree
#define CELL_WCHG_UPPER_BOUND 1
#define DEAD_END_CLEARENCE 0.35
#define CHRGST_DOMAIN_RADIUS 0.3
#define CIRCLING_LAPS 1								//set to 2 will be some problem I guess
#define SHUTTLE_LENGTH 2							//
#define QUADRANT_DEAD_ZONE 0.1745 		//0.2618 = 15 degree, 0.1745 = 10 degree
#define CP_SPACING_TOL 0.7071					//0.25*2*sqrt(2)
#define CP_ON_SWPLN_TOL 0.25 					//the tolerance of critical points on the same sweepline
#define CP_ANXD_ZONE_EST_AHEAD 0.5 		//the estimation ahead of critical points annexed zone
#define CELL_ENTRY_SAFE_CL 0.5				//cell entry safe clearance

/* Structs Declaration */
// Enumerators
typedef enum			//type of trigger signal
{
	INIT_PROC,
	POS_REACHED, 		//has reached the current target postion.
	WIRE_SENSED,		//has sensed the wire.
	RIGID_TOUCHED		//has touched rigid obstacle.
} T_trigger;

typedef enum			//type of action
{
	TO_POSE,				//drive to a pose.								 		Param: pos_ned[x,y,z], vec_ned[a,b,c]
	DIR_DRIVE,			//rotate to dir. and drive.						Param: rot_side, vec_ned[a,b,c]
	U_TURN,					//make an U-turn and drive in dir.		Param: turn_side, vec_ned[a,b,c]
	ONTO_WIRE,			//drive onto and along the wire.	 		Param: entry_side, laps, vec_ned[a,b,c]
	BYPASS					//bypass an obstacle along an arc. 		Param: pass_side
} T_action;

typedef enum			//type of turning side
{
	LEFT_STEERING,
	RIGHT_STEERING
} T_side;

// Param Structures
typedef struct
{
	float pos[3];
	float vec[3];
} T_param_to_pose;

typedef struct
{
	T_side rot_side;			//rotation side
	float fin_vec[3];			//finished direction(vector)
} T_param_dir_drive;

typedef struct
{
	T_side turn_side;			//turn side
	float fin_vec[3];			//finished direction(vector)
} T_param_u_turn;

typedef struct 
{
	T_side enter_side;		//enter into an wire loop
	int laps;							//set laps
	float fin_vec[3];			//finished direction
} T_param_onto_wire;

typedef struct
{
	T_side pass_side;
} T_param_bypass;

typedef struct
{
	T_param_to_pose to_pose_;
	T_param_dir_drive dir_drive_;
	T_param_u_turn u_turn_;
	T_param_onto_wire onto_wire_;
	T_param_bypass bypass_;
} T_params_act;


/* Variables Declaration */
extern T_pose *g_intercell_path;	//the output path
extern int g_path_length;
extern int g_path_cursor;
extern int g_current_cell;

//derived triggers
extern int g_trig_init_proc; 
extern int g_trig_pos_reached; 
extern int g_trig_wire_sensed_plain; 
extern int g_trig_rigid_touched_plain; 
extern int g_trig_wire_bstr_plunge; 
extern int g_trig_wire_bstr_surge; 
extern int g_trig_rigid_bstr_sigchg; 
extern int g_trig_cell_end_reached; 
extern int g_trig_chrgst_reached; 

//status flags
extern int g_opt_remap;							//OPTION: remap or not
extern int g_planner_initialized;		//FLAG: planner initialized or not
extern int g_flg_mapping;						//FLAG: on mapping or not
extern int g_flg_mapping_source;		//FLAG: 0-wire, 1-rigid
extern int g_flg_sens_stp_unoccur;	//FLAG: first sensing stop(wire or rigid) //NOTICE: need to reset when bstr dir changed!
extern int g_flg_endline_ever_touched; //FLAG: endline ever touched

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
extern float g_rot_bstr_prog_dir;				//rotation(arc) of bstr coord refer to lawn coord frame(mapping: 0.0, planning: pi/2)


/* Functions Declaration */
int planner_init();
int get_pose_in_lawn(); 
int get_pose_bstr(); 
int record_track(); 
int update_map_nav(); 

int build_adj_graph(float bstr_prog_dir); 
int pick_cand_cps(L_p_list list, int idx); 	//pick candidate critical points
int belong_to_quadrant(L_p_list list, int orig_idx, int start_idx);
int get_center_idx(int length, int start_idx, int end_idx);
int is_even(int n);
int bubble_sort_jointly(L_p_list list, L_i_list co_list, int dim);
int bubble_sort(L_p_list list, int dim);
int get_cpsn_alg_swpln(PL_p_list bdrys, T_pose cp, L_p_list pts_on_swpln);
int get_vrt_intxn_pts(L_p_list bdry, float x, L_p_list pts_out); 
int compute_vrt_intxn_pt(T_pose pose_a, T_pose pose_b, float x, T_pose *pose_out); 
int update_active_cells(L_i_list active_cells, L_p_list pts_on_swpln, L_i_list cpsns, L_p_list cps, L_i_list spec_types, int *next_csn_ptr);
int modify_cell_entries(); 

int update_graph_status(int switch_type); 
int get_uncov_cell_entry(T_pose *entry_ptr);	
int depth_first_search(unsigned char *visited, int n);

int event_preproc(T_trigger *trigger);
int decision_assert(T_action *action_out, T_params_act *params_out);
int make_decision(T_trigger *trigger, T_action *action_out, T_params_act *params_out);

int get_global_path(T_pose start_in_lawn, T_pose goal_in_lawn, L_i_list pos_list_out, L_i_list dir_list_out);
int reconstruct_path(int ndx_curr, unsigned char *field_ptr, int ndx_start, L_i_list path_list_out, L_i_list dir_list_out);
int get_neighbor_cost(unsigned short *cost_chart, int ndx_curr, int *ndx_nbr, int *cost_nbr);

T_side compute_turn_side();
float compute_heading_dir(int heading_type);

float compute_dist(T_pose pose_a, T_pose pose_b);
float angle_add(float ang_a, float ang_b);
float angle_sub(float ang_a, float ang_b);
T_pose tf_general(T_pose pose, float new_coord_ang);
T_param_to_pose tf_lawn2ned(T_pose pose_in_lawn);


#endif
