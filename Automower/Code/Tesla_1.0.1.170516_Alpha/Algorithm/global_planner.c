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
int g_path_length; 
int g_path_cursor = 0; 
int g_current_cell = 0;

int g_trig_init_proc = 0; 
int g_trig_pos_reached = 0; 
int g_trig_wire_sensed_plain = 0; 
int g_trig_rigid_touched_plain = 0; 
int g_trig_wire_bstr_plunge = 0; 
int g_trig_wire_bstr_surge = 0; 
int g_trig_rigid_bstr_sigchg = 0;
int g_trig_cell_end_reached = 0; 
int g_trig_chrgst_reached = 0; 

int g_opt_remap = 1; 
int g_planner_initialized = 0; 
int g_flg_mapping = 1; 
int g_flg_mapping_source = 0; 
int g_flg_sens_stp_unoccur = 1; 
int g_flg_endline_ever_touched = 0;
int g_flg_outer_bdry_mapping = 1;

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
//float g_rot_bstr_prog_dir = PI_/2; 
float g_rot_bstr_prog_dir = 0;


/* Functions Definition */

// Planner initializing
int planner_init()
{
	float pos_ned[3]; 
	float dir_ned[3]; 
	
	//remap option choosed, or loading the existed map failed
	//if(g_opt_remap || (-1 == map_load())) 
	if(g_opt_remap) //########## NOTICE: TEMPORARY TRY, need to resume to above line!
	{ 
		map_init();	
		graph_init();
		L_p_init(&g_outer_bdry); 
		PL_p_init(&g_wire_srndd_landscape_bdrys, MAX_LANDSCAPE_NUM); 
		PL_p_init(&g_rigid_landscape_bdrys, MAX_LANDSCAPE_NUM); 
		
		g_opt_remap = 0; 		//once loaded over, switch to FALSE by default.
		g_flg_mapping = 1;  
	}
	else
	{
		graph_init(); 
		g_flg_mapping = 0;
	}
	
	//used for compute the distance between robot and charge station(##### NO NEED?)
	g_orig_pose_in_lawn.pos.x = 0.0;
	g_orig_pose_in_lawn.pos.y = 0.0;
	g_orig_pose_in_lawn.th = 0.0;
	
	//initialize the lawn	coordinate frame
	get_pos_ned(pos_ned);
	get_rot_vector(dir_ned);
	g_curr_pose_enu.pos.x = pos_ned[1];
	g_curr_pose_enu.pos.y = pos_ned[0];
	g_curr_pose_enu.th = atan2(dir_ned[0], dir_ned[1]);
	
	if(pow(g_curr_pose_enu.pos.x, 2)+pow(g_curr_pose_enu.pos.y, 2) <= START_POS_TOL)
		//g_rot_lawn_in_enu = g_curr_pose_enu.th; //the orientation of the robot before start moving, is the x axis of lawn coord in enu.
		g_rot_lawn_in_enu = angle_add(g_curr_pose_enu.th, PI_);  //##### in REAL situation, robot is BACK OUT from station #####
	else
		rt_kprintf("The lawn coordinates initialization failed!");
	
	g_planner_initialized = 1; 
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
	
	g_curr_pose_in_lawn = tf_general(g_curr_pose_enu, g_rot_lawn_in_enu); 
}


// Get pose(bstr coord)
int get_pose_bstr()
{
	get_pose_in_lawn();
	g_curr_pose_bstr = tf_general(g_curr_pose_in_lawn, g_rot_bstr_prog_dir); 
}


// Record the track of mower into the active map
int record_track()
{
	int flg_empty;
	int flg_distinct;
	
	//record track when mapping mode is active
	if(g_flg_mapping)
	{		
		int current_ndx;
		T_pose last_pose_in_lawn;
		T_pose last_pose_on_map;
		int last_ndx;
		
		get_pose_in_lawn();
		/* ##### NOTICE: NOT INEVITABLE, can be retrieved from "update_map_nav" ##### */
		//g_current_pose_on_map.pos.x = (int)round(g_curr_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
		//g_current_pose_on_map.pos.y = (int)round(g_curr_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
		g_current_pose_on_map.pos.x = (int)(g_curr_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
		g_current_pose_on_map.pos.y = (int)(g_curr_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
		g_current_pose_on_map.th = g_curr_pose_in_lawn.th; 
		/* ################################## ABOVE ################################# */
		
		//write data into the map
		current_ndx = sub2ind(g_map.info.width, g_map.info.height, g_current_pose_on_map.pos.x, g_current_pose_on_map.pos.y);
		set_2b_grid_value(g_map.data, current_ndx, OCC_OBSTACLE);
		
		//01.append the new point into the OUTER linked list
		if((g_wire_srndd_landscape_cnt == 0) && (g_flg_mapping_source == 0))
		{
			flg_empty = L_p_is_empty(g_outer_bdry);
			
			if(!flg_empty)
			{
				last_pose_in_lawn = L_p_retrieve(g_outer_bdry, 0);
				//last_pose_on_map.pos.x = (int)round(last_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
				//last_pose_on_map.pos.y = (int)round(last_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
				last_pose_on_map.pos.x = (int)(last_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
				last_pose_on_map.pos.y = (int)(last_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
				last_pose_on_map.th = last_pose_in_lawn.th; 
				//### NOTICE: here on the old huge map, will be ok, right?
				last_ndx = sub2ind(g_map.info.width, g_map.info.height, last_pose_on_map.pos.x, last_pose_on_map.pos.y);
				flg_distinct = (current_ndx != last_ndx);
			}
			
			if(flg_empty || flg_distinct)
				L_p_prepend(g_outer_bdry, g_curr_pose_in_lawn);
		}
		
		//02.append the new point into the WIRE linked list
		if((g_wire_srndd_landscape_cnt > 0) && (g_flg_mapping_source == 0))		//add a wire point
		{
			flg_empty = L_p_is_empty(g_wire_srndd_landscape_bdrys + g_wire_srndd_landscape_cnt - 1);
			
			if(!flg_empty)
			{
				last_pose_in_lawn = L_p_retrieve(g_wire_srndd_landscape_bdrys + g_wire_srndd_landscape_cnt - 1, 0);
				//last_pose_on_map.pos.x = (int)round(last_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
				//last_pose_on_map.pos.y = (int)round(last_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
				last_pose_on_map.pos.x = (int)(last_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
				last_pose_on_map.pos.y = (int)(last_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
				last_pose_on_map.th = last_pose_in_lawn.th; 
				last_ndx = sub2ind(g_map.info.width, g_map.info.height, last_pose_on_map.pos.x, last_pose_on_map.pos.y);
				flg_distinct = (current_ndx != last_ndx);
			}
			
			if(flg_empty || flg_distinct)
					L_p_prepend(g_wire_srndd_landscape_bdrys + g_wire_srndd_landscape_cnt - 1, g_curr_pose_in_lawn);
		}
		
		//03.append the new point into the RIGID linked list
		if((g_rigid_landscape_cnt > 0) && (g_flg_mapping_source == 1))				//add a rigid point
		{
			flg_empty = L_p_is_empty(g_rigid_landscape_bdrys + g_rigid_landscape_cnt - 1);
			
			if(!flg_empty)
			{
				last_pose_in_lawn = L_p_retrieve(g_rigid_landscape_bdrys + g_rigid_landscape_cnt - 1, 0);
				//last_pose_on_map.pos.x = (int)round(last_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
				//last_pose_on_map.pos.y = (int)round(last_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
				last_pose_on_map.pos.x = (int)(last_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
				last_pose_on_map.pos.y = (int)(last_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
				last_pose_on_map.th = last_pose_in_lawn.th; 
				last_ndx = sub2ind(g_map.info.width, g_map.info.height, last_pose_on_map.pos.x, last_pose_on_map.pos.y);
				flg_distinct = (current_ndx != last_ndx);
			}
			
			if(flg_empty || flg_distinct)
				L_p_prepend(g_rigid_landscape_bdrys + g_rigid_landscape_cnt - 1, g_curr_pose_in_lawn);
		}
	}
	
	return 0;
}


// Update the robot position on map(for @Gao Xiang)
// ##### NOTICE: ask @Wu Haowen add this function in motion thread!!!!!!!!%%%%%%%%%##########@@@@@@@@@@@@@
int update_map_nav()
{
	get_pose_in_lawn();
	//g_current_pose_on_map.pos.x = (int)round(g_curr_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
	//g_current_pose_on_map.pos.y = (int)round(g_curr_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
	g_current_pose_on_map.pos.x = (int)(g_curr_pose_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x;
	g_current_pose_on_map.pos.y = (int)(g_curr_pose_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y;
	g_current_pose_on_map.th = g_curr_pose_in_lawn.th; 
}


// Build up the adjacency graph
int build_adj_graph(float bstr_prog_dir)
{
	int i;
	int j;
	int k;
	int length;
	int pre_type;
	int pre_idx;
	int spec_type;
	int steady_pole;
	int nonius_pole;
	int cpsn;
	int cp_group_num;
	int next_csn;
	T_pose pose_in_lawn;
	T_pose pose_bstr;
	T_pose lhs_pose;
	T_pose rhs_pose;
	
	PL_p_list bdrys_in_bstr_dir; 
	L_p_list tmp_critical_points; 
	L_i_list tmp_cp_types; 
	L_i_list isopoint_centers; 
	L_p_list critical_points; 
	L_i_list cp_types; 
	PL_p_list critical_points_by_group;
	PL_i_list cp_types_by_group;
	L_p_list pts_on_swpln;
	L_i_list active_cells_on_swpln;
	L_i_list cpsns;
	L_i_list intragroup_spec_types;
	
	// 00.Clear the last graph
	//graph_purge();    //#################################################################
	
	// 01.Pick the critical points out, related to the boustrophedon direction
  // OUTPUT: CP points in order, and their types.
	PL_p_init(&bdrys_in_bstr_dir, MAX_LANDSCAPE_NUM); 
	L_p_init(&tmp_critical_points); 
	L_i_init(&tmp_cp_types); 
	L_i_init(&isopoint_centers); 
	L_p_init(&critical_points); 
	L_i_init(&cp_types); 
	PL_p_init(&critical_points_by_group, MAX_CP_NUM); 
	PL_i_init(&cp_types_by_group, MAX_CP_NUM); 
	L_p_init(&pts_on_swpln);
	L_i_init(&active_cells_on_swpln); 
	L_i_init(&cpsns); 
	L_i_init(&intragroup_spec_types); 
	
	// 01a.transform list points to coord of the boustrophedon direction
	// (1)outer boundary
	length = L_p_get_length(g_outer_bdry);
	for(j = 0; j < length; j++)
	{
		pose_in_lawn = L_p_retrieve(g_outer_bdry, j); 
		pose_bstr = tf_general(pose_in_lawn, g_rot_bstr_prog_dir); 
		L_p_append(bdrys_in_bstr_dir + 0, pose_bstr); 
	}
	
	// (2)wire boundaries
	for(i = 0; i < g_wire_srndd_landscape_cnt; i++)
	{
		length = L_p_get_length(g_wire_srndd_landscape_bdrys + i);
		for(j = 0; j < length; j++)
		{
			pose_in_lawn = L_p_retrieve(g_wire_srndd_landscape_bdrys + i, j); 
			pose_bstr = tf_general(pose_in_lawn, g_rot_bstr_prog_dir); 
			L_p_append(bdrys_in_bstr_dir + i + 1, pose_bstr); 
		}
	}
	
	// (3)rigid boundaries
	// (### DELAY ###)
	
	
	// 01b.traverse all the point, pick the critical points out
	steady_pole = L_p_get_length(critical_points); 
	nonius_pole = steady_pole - 1; 
	for(i = 0; i < g_total_bdrys_cnt; i++)
	{
		//pick group candidate critical points out
		length = L_p_get_length(bdrys_in_bstr_dir + i);
		for(j = 0; j < length; j++)
		{
			pre_type = pick_cand_cps(bdrys_in_bstr_dir + i, j);
			if(-1 != pre_type)
			{
				L_p_append(tmp_critical_points, L_p_retrieve(bdrys_in_bstr_dir + i, j)); 
				L_i_append(tmp_cp_types, (unsigned short)pre_type); 
			}
		}
		
		//pick the local knee points(center of the candidate cps) out
		j = 0;
		while(L_i_retrieve(tmp_cp_types, --j) == L_i_retrieve(tmp_cp_types, 0));
		++j;
		
		length = L_i_get_length(tmp_cp_types); 
		pre_idx = j; 
		pre_type = L_i_retrieve(tmp_cp_types, j); 
		for(k = j; k < j + length; k++)
		{
			if(L_i_retrieve(tmp_cp_types, k+1) != pre_type)
			{
				L_i_append(isopoint_centers, (unsigned short)get_center_idx(length, pre_idx, k));
				pre_idx = k + 1; 
				pre_type = L_i_retrieve(tmp_cp_types, k+1); 
			}
		}
		
		//dump the knee points in each bdry to a single list, and delete the knee points too near
		length = L_i_get_length(isopoint_centers);
		for(j = 0; j < length; j++)
		{
			L_p_append(critical_points, L_p_retrieve(tmp_critical_points, (int)L_i_retrieve(isopoint_centers, j)));
			L_i_append(cp_types, L_i_retrieve(tmp_cp_types, (int)L_i_retrieve(isopoint_centers, j)));
			++nonius_pole;
			
			if(nonius_pole > steady_pole)
			{
				rhs_pose = L_p_retrieve(critical_points, nonius_pole);
				lhs_pose = L_p_retrieve(critical_points, nonius_pole - 1);
				if(compute_dist(rhs_pose, lhs_pose) <= CP_SPACING_TOL)
				{
					L_p_delete_by_idx(critical_points, nonius_pole);
					L_i_delete_by_idx(cp_types, nonius_pole);
					--nonius_pole;
					L_p_delete_by_idx(critical_points, nonius_pole);
					L_i_delete_by_idx(cp_types, nonius_pole);
					--nonius_pole;
				}
			}
		}
		rhs_pose = L_p_retrieve(critical_points, nonius_pole);
		lhs_pose = L_p_retrieve(critical_points, steady_pole);
		if(compute_dist(rhs_pose, lhs_pose) <= CP_SPACING_TOL)
		{
			L_p_delete_by_idx(critical_points, nonius_pole);
			L_i_delete_by_idx(cp_types, nonius_pole);
			--nonius_pole;
			L_p_delete_by_idx(critical_points, steady_pole);
			L_i_delete_by_idx(cp_types, steady_pole);
			--nonius_pole;
		}
		steady_pole = nonius_pole + 1;
		
		//free temp lists
		L_p_delete_all_but_header(tmp_critical_points);
		L_i_delete_all_but_header(tmp_cp_types);
		L_i_delete_all_but_header(isopoint_centers);
	}
	rt_free(tmp_critical_points);
	rt_free(tmp_cp_types);
	rt_free(isopoint_centers);
	
	// 01c.bubble sort of critical points
	bubble_sort_jointly(critical_points, cp_types, 0); //sorted by x	
	
	
	// 02.Establish the adjacency graph by the CP points	
	// 02a.another sort of critical point: points near the same sweepline will be clustered in one group.
	j = 0; 		//group sn
	L_p_append(critical_points_by_group + j, L_p_retrieve(critical_points, 0));
	L_i_append(cp_types_by_group + j, L_i_retrieve(cp_types, 0));
	lhs_pose = L_p_retrieve(critical_points_by_group + j, 0);
	
	length = L_p_get_length(critical_points);
	for(i = 1; i < length; i++)
	{
		rhs_pose = L_p_retrieve(critical_points, i);
		if(rhs_pose.pos.x - lhs_pose.pos.x <= CP_ON_SWPLN_TOL)
		{
			L_p_append(critical_points_by_group + j, rhs_pose);
			L_i_append(cp_types_by_group + j, L_i_retrieve(cp_types, i));
		}
		else
		{
			L_p_append(critical_points_by_group + ++j, rhs_pose);
			L_i_append(cp_types_by_group + j, L_i_retrieve(cp_types, i));
			lhs_pose = L_p_retrieve(critical_points_by_group + j, 0);
		}
	}	
	cp_group_num = j + 1;		
	
	// 02b.update the active cell list, build the adjacency graph	
	next_csn = 0;
	for(j = 0; j < cp_group_num; j++)
	{
		bubble_sort_jointly(critical_points_by_group + j, cp_types_by_group + j, 1); //sorted by y
		length = L_p_get_length(critical_points_by_group + j);
		for(i = 0; i < length; i++)
		{
			cpsn = get_cpsn_alg_swpln(bdrys_in_bstr_dir, L_p_retrieve(critical_points_by_group + j, i), pts_on_swpln);
			pre_type = (int)L_i_retrieve(cp_types_by_group + j, i);
			
			if((pre_type == 0) && (!is_even(cpsn))) 	//CELL_SPLIT
				spec_type = CELL_SPLIT;
			if((pre_type == 0) && (is_even(cpsn))) 		//CELL_ORIGINATE
				spec_type = CELL_ORIGINATE;
			if((pre_type == 2) && (is_even(cpsn))) 		//CELL_MERGE
				spec_type = CELL_MERGE;
			if((pre_type == 2) && (!is_even(cpsn))) 	//CELL_CEASE
				spec_type = CELL_CEASE;
			
			L_i_append(intragroup_spec_types, (unsigned short)spec_type);	
			//L_i_append(active_op_idxs, (unsigned short)(cpsn / 2));
			L_i_append(cpsns, (unsigned short)cpsn);
		}
		
		update_active_cells(active_cells_on_swpln, pts_on_swpln, cpsns, critical_points_by_group + j, intragroup_spec_types, &next_csn);
		
		L_p_delete_all_but_header(pts_on_swpln);
		L_i_delete_all_but_header(intragroup_spec_types);
		L_i_delete_all_but_header(cpsns);
	}
	rt_free(pts_on_swpln);
	rt_free(intragroup_spec_types);
	rt_free(cpsns);
	
	//release resources
	PL_p_clear(bdrys_in_bstr_dir, MAX_LANDSCAPE_NUM);
	L_p_delete_all_but_header(critical_points);
	L_i_delete_all_but_header(cp_types);
	PL_p_clear(critical_points_by_group, MAX_CP_NUM);
	PL_i_clear(cp_types_by_group, MAX_CP_NUM);
	L_i_delete_all_but_header(active_cells_on_swpln);
	
	rt_free(bdrys_in_bstr_dir);
	rt_free(critical_points);
	rt_free(cp_types);
	rt_free(critical_points_by_group);
	rt_free(cp_types_by_group);
	rt_free(active_cells_on_swpln);
	
	g_adj_vexnum = next_csn; //csn is started from 0
	modify_cell_entries();
	PL_i_print(g_graph, g_adj_vexnum);
	
	return 0;
}


// Pick candidate critical points
int pick_cand_cps(L_p_list list, int idx)
{
	int i = 0;
	int pos_dir_quad = 0;
	int neg_dir_quad = 0;
	
	while(0 == pos_dir_quad)
		pos_dir_quad = belong_to_quadrant(list, idx, ++i + idx);
	
	i = 0;
	
	while(0 == neg_dir_quad)
		neg_dir_quad = belong_to_quadrant(list, idx, --i + idx);
	
	if(((pos_dir_quad == 1)&&(neg_dir_quad == 4)) || ((pos_dir_quad == 4)&&(neg_dir_quad == 1)))
		return CELL_SPLIT;
	else if(((pos_dir_quad == 2)&&(neg_dir_quad == 3)) || ((pos_dir_quad == 3)&&(neg_dir_quad == 2)))
		return CELL_MERGE;
	else
		return -1;
}


// Belong to which quadrant
int belong_to_quadrant(L_p_list list, int orig_idx, int start_idx)
{
	int i;
	T_pose pose;
	T_pose shuttle_center;
	T_pose origin;
	float angle;
	
	shuttle_center.pos.x = 0.0;
	shuttle_center.pos.y = 0.0;
	shuttle_center.th = 0.0;
	if(start_idx > orig_idx)
	{
		for(i = 0; i < SHUTTLE_LENGTH; i++)
		{
			pose = L_p_retrieve(list, start_idx + i);
			shuttle_center.pos.x += pose.pos.x/SHUTTLE_LENGTH;
			shuttle_center.pos.y += pose.pos.y/SHUTTLE_LENGTH;
			shuttle_center.th += pose.th/SHUTTLE_LENGTH;
		}
	}
	else
	{
		for(i = 0; i > -SHUTTLE_LENGTH; i--)
		{
			pose = L_p_retrieve(list, start_idx + i);
			shuttle_center.pos.x += pose.pos.x/SHUTTLE_LENGTH;
			shuttle_center.pos.y += pose.pos.y/SHUTTLE_LENGTH;
			shuttle_center.th += pose.th/SHUTTLE_LENGTH;
		}
	}
	origin = L_p_retrieve(list, orig_idx);
	angle = atan2(shuttle_center.pos.y - origin.pos.y, shuttle_center.pos.x - origin.pos.x); // (-pi/2, pi/2]
	
	if((shuttle_center.pos.x - origin.pos.x == 0) || (shuttle_center.pos.y - origin.pos.y == 0))
		return 0;
	
	/*if(shuttle_center.pos.x - origin.pos.x > 0)
	{
		if((angle >= QUADRANT_DEAD_ZONE) && (angle <= PI_/2 - QUADRANT_DEAD_ZONE))
			return 1;
		else if((angle >= -PI_/2 + QUADRANT_DEAD_ZONE) && (angle <= -QUADRANT_DEAD_ZONE))
			return 4;
		else
			return 0;
	}
	
	if(shuttle_center.pos.x - origin.pos.x < 0)
	{
		if((angle >= QUADRANT_DEAD_ZONE) && (angle <= PI_/2 - QUADRANT_DEAD_ZONE))
			return 3;
		else if((angle >= -PI_/2 + QUADRANT_DEAD_ZONE) && (angle <= -QUADRANT_DEAD_ZONE))
			return 2;
		else
			return 0;
	}*/
	
	if((angle >= QUADRANT_DEAD_ZONE) && (angle <= PI_/2 - QUADRANT_DEAD_ZONE))
		return 1;
	else if((angle >= PI_/2 + QUADRANT_DEAD_ZONE) && (angle <= PI_ - QUADRANT_DEAD_ZONE))
		return 2;
	else if((angle >= -PI_ + QUADRANT_DEAD_ZONE) && (angle <= -PI_/2 - QUADRANT_DEAD_ZONE))
		return 3;
	else if((angle >= -PI_/2 + QUADRANT_DEAD_ZONE) && (angle <= -QUADRANT_DEAD_ZONE))
		return 4;
	else
		return 0;
}


// Compute center index
int get_center_idx(int length, int start_idx, int end_idx)
{
	if(start_idx == end_idx)
		return start_idx;
	
	if(end_idx > start_idx)
		//return (int)round((start_idx + end_idx)/2); 
		return (int)((start_idx + end_idx)/2); 
	
	if(end_idx < start_idx)
		//return (int)round((start_idx + end_idx + length)/2); 
		return (int)((start_idx + end_idx + length)/2); 
}


// Is even or not
int is_even(int n)
{
	float x;
	
	x = (float)n/2;
	if(x >= 0)
	{
		if((x - (int)x < 1e-5) || (x - (int)x > 1 - 1e-5))
			return 1;
		else
			return 0;
	}
	else
	{
		if((x - (int)x > -1e-5) || (x - (int)x < -(1 - 1e-5)))
			return 1;
		else
			return 0;
	}
}


// Bubble sort of a pose list by x
int bubble_sort_jointly(L_p_list list, L_i_list co_list, int dim)
{
	int i;
	int j;
	int length;
	T_pose lhs_pose;
	T_pose rhs_pose;
	unsigned short lhs_elem;
	unsigned short rhs_elem;

	length = L_p_get_length(list);
	if(length == 0 || length == 1)
		return -1;
	else
	{
		for (j = 0; j < length - 1; j++)
		{
			for (i = 0; i < length - 1 - j; i++)
			{
				lhs_pose = L_p_retrieve(list, i);
				rhs_pose = L_p_retrieve(list, i + 1);
				lhs_elem = L_i_retrieve(co_list, i);
				rhs_elem = L_i_retrieve(co_list, i + 1);
				if(0 == dim) //sorted by x
				{
					if(lhs_pose.pos.x > rhs_pose.pos.x)
					{
						L_p_set_by_idx(list, i, rhs_pose);
						L_p_set_by_idx(list, i + 1, lhs_pose);
						L_i_set_by_idx(co_list, i, rhs_elem);
						L_i_set_by_idx(co_list, i + 1, lhs_elem);
					}
				}
				else //sorted by y
				{
					if(lhs_pose.pos.y > rhs_pose.pos.y)
					{
						L_p_set_by_idx(list, i, rhs_pose);
						L_p_set_by_idx(list, i + 1, lhs_pose);
						L_i_set_by_idx(co_list, i, rhs_elem);
						L_i_set_by_idx(co_list, i + 1, lhs_elem);
					}
				}
			}
		}
		return 0;
	}
}


// Bubble sort of a pose list by y
int bubble_sort(L_p_list list, int dim)
{
	int i;
	int j;
	int length;
	T_pose lhs_pose;
	T_pose rhs_pose;

	length = L_p_get_length(list);
	if(length == 0 || length == 1)
		return -1;
	else
	{
		for (j = 0; j < length - 1; j++)
		{
			for (i = 0; i < length - 1 - j; i++)
			{
				lhs_pose = L_p_retrieve(list, i);
				rhs_pose = L_p_retrieve(list, i + 1);
				if(0 == dim)
				{
					if(lhs_pose.pos.x > rhs_pose.pos.x)
					{
						L_p_set_by_idx(list, i, rhs_pose);
						L_p_set_by_idx(list, i + 1, lhs_pose);
					}
				}
				else
				{
					if(lhs_pose.pos.y > rhs_pose.pos.y)
					{
						L_p_set_by_idx(list, i, rhs_pose);
						L_p_set_by_idx(list, i + 1, lhs_pose);
					}
				}
			}
		}
		return 0;
	}
}


// Get critical points sequence number along sweepline
int get_cpsn_alg_swpln(PL_p_list bdrys, T_pose cp, L_p_list pts_on_swpln)
{
	int i;
	int exp;
	float x_ahead;
	int length;
	T_pose swpln_pt;
	
	i = 0;
	exp = 0;
	//x_ahead = cp.pos.x - CP_ANXD_ZONE_EST_AHEAD;
	x_ahead = cp.pos.x - pow(-0.5, exp++) * CP_ANXD_ZONE_EST_AHEAD;
	L_p_init(&pts_on_swpln);
	while(i < g_total_bdrys_cnt)
	{
		if(!is_even(get_vrt_intxn_pts(bdrys + i++, x_ahead, pts_on_swpln)))
		{
			i = 0;
			x_ahead -= pow(-0.5, exp++) * CP_ANXD_ZONE_EST_AHEAD;
			L_p_delete_all_but_header(pts_on_swpln);
		}
	}
	
	i = 0;
	length = L_p_get_length(pts_on_swpln); //length is surely even(0 included)
	if(length > 1)
	{
		bubble_sort(pts_on_swpln, 1); //sorted by y
		while(i < length)
		{
			swpln_pt = L_p_retrieve(pts_on_swpln, i);
			if(cp.pos.y < swpln_pt.pos.y)
				return i;
			++i;
		}
	}
	return length; //length of pts_on_swpln is 0, or cp.pos.y is bigger than all the pts_on_swpln
}


// Get vertical intersection point of the polygonal line of bdry and a vertical line at 'x'
int get_vrt_intxn_pts(L_p_list bdry, float x, L_p_list pts_out)
{
	int i;
	int length;
	T_pose pose;
	
	length = L_p_get_length(bdry); 
	for(i = 0; i < length; i++) //(length + 1) will be 0 in L_p_retrieve() function
	{
		if(-1 != compute_vrt_intxn_pt(L_p_retrieve(bdry, i), L_p_retrieve(bdry, i + 1), x, &pose))
			L_p_append(pts_out, pose); 
	}
	
	return L_p_get_length(pts_out);
}


// Compute vertical intersection point of a segment line
// (defined by 'pose_a' and 'pose_b') and a vertical line at 'x'
int compute_vrt_intxn_pt(T_pose pose_a, T_pose pose_b, float x, T_pose *pose_out)
{
	if(pose_a.pos.x == pose_b.pos.x)
	{
		if(pose_a.pos.x == x)
		{
			pose_out->pos.x = x;
			pose_out->pos.y = (pose_a.pos.y + pose_b.pos.y)/2;
			pose_out->th = atan2(pose_b.pos.y - pose_a.pos.y, pose_b.pos.x - pose_a.pos.x); //theta is no matter
		}
		else
			return -1; 
	}
	else
	{
		if(((x >= pose_a.pos.x) && (x < pose_b.pos.x)) || ((x > pose_b.pos.x) && (x <= pose_a.pos.x))) //pose_a inclusive, pose_b exclusive
		{
			pose_out->pos.x = x;
			pose_out->pos.y = (x - pose_a.pos.x)*(pose_b.pos.y - pose_a.pos.y)/(pose_b.pos.x - pose_a.pos.x) + pose_a.pos.y;
			pose_out->th = atan2(pose_b.pos.y - pose_a.pos.y, pose_b.pos.x - pose_a.pos.x); //theta is no matter
		}
		else
			return -1;
	}
	
	return 0;
}


// Update active cells
int update_active_cells(L_i_list active_cells, L_p_list pts_on_swpln, L_i_list cpsns, L_p_list cps, L_i_list spec_types, int *next_csn_ptr)
{
	int i;
	int cp_num;
	int active_cell_num;
	int cursor_csn;
	T_pose con_pose1;
	T_pose con_pose2;
	T_pose con_pose3;
	T_pose mid_pose;
	
	cp_num = L_i_get_length(spec_types);
	//step1: down to up, compute the new csn, update the graph
	for(i = 0; i < cp_num; i++)
	{
		switch(L_i_retrieve(spec_types, i))
		{
			case CELL_SPLIT: 
				if((0 == i) || (!is_even((int)L_i_retrieve(spec_types, i-1))))
				{
					PL_i_insert(g_graph, (int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2)), (*next_csn_ptr)++);
					PL_i_insert(g_graph, (int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2)), (*next_csn_ptr)++);
				}
				else
					PL_i_insert(g_graph, (int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2)), (*next_csn_ptr)++);
				break;
			
			case CELL_ORIGINATE: 
				PL_i_insert(g_graph, -1, (*next_csn_ptr)++);
				break;
			
			case CELL_MERGE: 
				if((0 == i) || (!is_even((int)L_i_retrieve(spec_types, i-1))))
				{
					PL_i_insert(g_graph, (int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2)), *next_csn_ptr);
					PL_i_insert(g_graph, (int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2)) - 1, (*next_csn_ptr)++);
				}
				else
					PL_i_insert(g_graph, (int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2)), *next_csn_ptr - 1);
				break;
			
			case CELL_CEASE: 
				PL_i_insert(g_graph, -1, -1);
				break;
			
			default:
				break;
		}
	}
	
	//step2: up to down, compute the cell entries, update the active cells
	cursor_csn = *next_csn_ptr - 1;
	for(i = cp_num - 1; i >= 0; i--)
	{
		switch(L_i_retrieve(spec_types, i))
		{
			case CELL_SPLIT: 
				//compute the cell entries
				con_pose1 = L_p_retrieve(pts_on_swpln, L_i_retrieve(cpsns, i));
				con_pose2 = L_p_retrieve(pts_on_swpln, L_i_retrieve(cpsns, i)-1);
				con_pose3 = L_p_retrieve(cps, i);
				if(((cp_num - 1) == i) || (!is_even((int)L_i_retrieve(spec_types, i+1))))
				{
					//(1)left cell, right entry
					mid_pose.pos.x = con_pose3.pos.x;
					mid_pose.pos.y = (con_pose1.pos.y + con_pose2.pos.y) / 2;
					if(((cp_num - 1) == i) && ((active_cell_num - 1) == (int)(L_i_retrieve(cpsns, i)/2)))
						mid_pose.th = -PI_/2;
					else
						mid_pose.th = PI_/2;
					g_cell_entries[(int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2))].right_entry = mid_pose;
					
					//(2)right up cell, left entry
					mid_pose.pos.y = (con_pose1.pos.y + con_pose3.pos.y) / 2;
					mid_pose.th = PI_/2;
					g_cell_entries[cursor_csn].left_entry = mid_pose;
					
					//(3)right down cell, left entry
					if((cp_num > 1) && (i > 0) && (is_even((int)L_i_retrieve(spec_types, i-1))))
						mid_pose.pos.y = (L_p_retrieve(cps, i-1).pos.y + con_pose3.pos.y) / 2;
					else
						mid_pose.pos.y = (con_pose2.pos.y + con_pose3.pos.y) / 2; 
					mid_pose.th = PI_/2;
					g_cell_entries[cursor_csn-1].left_entry = mid_pose;
					
					//update the active cells
					L_i_delete_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2));
					L_i_insert_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2), cursor_csn--);
					L_i_insert_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2), cursor_csn--);
				}
				else
				{
					//(3)right down cell, left entry
					if((cp_num > 1) && (i > 0) && (is_even((int)L_i_retrieve(spec_types, i-1))))
						mid_pose.pos.y = (L_p_retrieve(cps, i-1).pos.y + con_pose3.pos.y) / 2;
					else
						mid_pose.pos.y = (con_pose2.pos.y + con_pose3.pos.y) / 2; 
					mid_pose.th = PI_/2;
					g_cell_entries[cursor_csn].left_entry = mid_pose;
					
					//update the active cells
					L_i_insert_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2), cursor_csn--);
				}
				break;
			
			case CELL_ORIGINATE: 
				con_pose3 = L_p_retrieve(cps, i);
				con_pose3.th = PI_/2; 
				g_cell_entries[cursor_csn].left_entry = con_pose3;
				
				L_i_insert_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2), cursor_csn--);
				break;
			
			case CELL_MERGE: 
				//compute the cell entries
				con_pose1 = L_p_retrieve(pts_on_swpln, L_i_retrieve(cpsns, i)+1);
				con_pose2 = L_p_retrieve(pts_on_swpln, L_i_retrieve(cpsns, i)-2);
				con_pose3 = L_p_retrieve(cps, i);
				if(((cp_num - 1) == i) || (!is_even((int)L_i_retrieve(spec_types, i+1))))
				{
					//(1)left up cell, right entry
					mid_pose.pos.x = con_pose3.pos.x;
					mid_pose.pos.y = (con_pose1.pos.y + con_pose3.pos.y) / 2;
					if(((cp_num - 1) == i) && ((active_cell_num - 1) == (int)(L_i_retrieve(cpsns, i)/2)))
						mid_pose.th = -PI_/2;
					else
						mid_pose.th = PI_/2;
					g_cell_entries[(int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2))].right_entry = mid_pose;
					
					//(2)left down cell, right entry
					if((cp_num > 1) && (i > 0) && (is_even((int)L_i_retrieve(spec_types, i-1))))
						mid_pose.pos.y = (L_p_retrieve(cps, i-1).pos.y + con_pose3.pos.y) / 2;
					else
						mid_pose.pos.y = (con_pose2.pos.y + con_pose3.pos.y) / 2; 
					mid_pose.th = PI_/2;
					g_cell_entries[(int)L_i_retrieve(active_cells, ((int)(L_i_retrieve(cpsns, i)/2))-1)].right_entry = mid_pose;
					
					//(3)right cell, left entry
					if((cp_num > 1) && (i > 0) && (is_even((int)L_i_retrieve(spec_types, i-1))))
						mid_pose.pos.y = (L_p_retrieve(cps, i-1).pos.y + con_pose1.pos.y) / 2;
					else
						mid_pose.pos.y = (con_pose2.pos.y + con_pose1.pos.y) / 2; 
					mid_pose.th = -PI_/2;
					g_cell_entries[cursor_csn].left_entry = mid_pose;
					
					//update the active cells
					L_i_delete_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2));
					L_i_insert_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2), cursor_csn--);
					L_i_delete_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2) - 1);
				}
				else
				{
					//(2)left down cell, right entry
					if((cp_num > 1) && (i > 0) && (is_even((int)L_i_retrieve(spec_types, i-1))))
						mid_pose.pos.y = (L_p_retrieve(cps, i-1).pos.y + con_pose3.pos.y) / 2;
					else
						mid_pose.pos.y = (con_pose2.pos.y + con_pose3.pos.y) / 2; 
					mid_pose.th = PI_/2;
					g_cell_entries[(int)L_i_retrieve(active_cells, ((int)(L_i_retrieve(cpsns, i)/2))-1)].right_entry = mid_pose;
					
					//update the active cells
					L_i_delete_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2) - 1);
				}
				break;
			
			case CELL_CEASE: 
				con_pose3 = L_p_retrieve(cps, i);
				con_pose3.th = PI_/2; 
				g_cell_entries[(int)L_i_retrieve(active_cells, (int)(L_i_retrieve(cpsns, i)/2))].right_entry = con_pose3;			
				
				L_i_delete_by_idx(active_cells, (int)(L_i_retrieve(cpsns, i)/2));
				break;
			
			default:
				break;
		}
	}
}


// Modify cell entries
int modify_cell_entries()
{
	int i;
	float x1;
	float y1;
	float x2;
	float y2;
	float x1new;
	float y1new;
	float x2new;
	float y2new;
	
	for(i = 0; i < g_adj_vexnum; i++)
	{
		x1 = g_cell_entries[i].left_entry.pos.x;
		y1 = g_cell_entries[i].left_entry.pos.y;
		x2 = g_cell_entries[i].right_entry.pos.x;
		y2 = g_cell_entries[i].right_entry.pos.y;
		
		if(x2 != x1)
		{
			x1new = x1 + CELL_ENTRY_SAFE_CL;
			y1new = CELL_ENTRY_SAFE_CL * (y2-y1) / (x2-x1) + y1; //still in bstr dir, x2 cannot be equal x1
			x2new = x2 - CELL_ENTRY_SAFE_CL;
			y2new = -CELL_ENTRY_SAFE_CL * (y2-y1) / (x2-x1) + y2;
		}
		
		if((x2 == x1) || (x1new > x2new))
		{
			x1new = (x1+x2) / 2;
			y1new = (y1+y2) / 2;
			x2new = x1new;
			y2new = y1new;
		}
		
		g_cell_entries[i].left_entry.pos.x = x1new;
		g_cell_entries[i].left_entry.pos.y = y1new;
		g_cell_entries[i].right_entry.pos.x = x2new;
		g_cell_entries[i].right_entry.pos.y = y2new;
		
		//transform to the lawn coordinate
		g_cell_entries[i].left_entry = tf_general(g_cell_entries[i].left_entry, -g_rot_bstr_prog_dir); 
		g_cell_entries[i].right_entry = tf_general(g_cell_entries[i].right_entry, -g_rot_bstr_prog_dir); 
	}
}


// Update graph status(g_current_cell, g_vertices_marked[])
int update_graph_status(int switch_type)
{
	int i;
	int length;
	int new_cell;
	T_pose entry;
	float bstr_head_dir;
	
	if(!g_flg_mapping)
	{
		if(0 == switch_type) 			//cell end
			g_vertices_marked[g_current_cell] = (unsigned char)1;
		else if(1 == switch_type) //new entry arrived, after global path
		{
			new_cell = get_uncov_cell_entry(&entry);
			if(new_cell != -1)
				g_current_cell = new_cell;
		}
		else if(2 == switch_type) //boustrophedon, switch cell
		{
			g_vertices_marked[g_current_cell] = (unsigned char)1;
	
			bstr_head_dir = compute_heading_dir(0);
			if(PI_/2 == bstr_head_dir)
			{
				length = L_i_get_length(g_graph + g_current_cell);
				for(i = 0; i < length; i++)
				{
					new_cell = (int)L_i_retrieve(g_graph + g_current_cell, i);
					if(new_cell > g_current_cell)
					{
						g_current_cell = new_cell;
						return 0;
					}
				}
				return -1;
			}
			if(-PI_/2 == bstr_head_dir)
			{
				length = L_i_get_length(g_graph + g_current_cell);
				new_cell = (int)L_i_retrieve(g_graph + g_current_cell, length - 1);
				if(new_cell > g_current_cell)
				{
					g_current_cell = new_cell;
					return 0;
				}
				else
					return -1;
			}
		}
	}
}


// Get entry of the next covered cell
int get_uncov_cell_entry(T_pose *entry_ptr)
{
	int i;
	unsigned char *vex_visited;
	int cell_sn;
	
	vex_visited = rt_calloc(g_adj_vexnum, sizeof(unsigned char));
	if(vex_visited == RT_NULL)
		rt_kprintf("calloc fails, visited matrix allocating fails");
	else
	{
		for(i = 0; i < g_adj_vexnum; i++)
			vex_visited[i] = (unsigned char)0;
	}
	
	cell_sn = depth_first_search(vex_visited, 0);
	rt_free(vex_visited);
	
	if(cell_sn == -1)
		return -1;
	else
		*entry_ptr = g_cell_entries[cell_sn].left_entry;
	
	return cell_sn;
}


// depth first search
int depth_first_search(unsigned char *visited, int v)
{
	int i;
	int length;
	int w;
	int cell_sn;
	
	visited[v] = (unsigned char)1; //set visited
	if(g_vertices_marked[v] == 0)  //uncovered
		return v;  //has found the objecitve cell node to be covered
	else
	{
		length = L_i_get_length(g_graph + v);
		for(i = 0; i < length; i++)
		{
			w = (int)L_i_retrieve(g_graph + v, i);
			if(visited[w] == (unsigned char)0) //unvisited
			{
				cell_sn = depth_first_search(visited, w);
				if(cell_sn >= 0)
					return cell_sn;
			}
		}
		return -1;
	}
}


// Event Preprocessing, convert the raw triggers to the derived triggers. 
// ### OUTPUT: the 7 trigger flags ###
int event_preproc(T_trigger *trigger)
{
	switch(*trigger)
	{
		case INIT_PROC: 
			g_trig_init_proc = 1; 
			break;
		
		
		case POS_REACHED:
			g_trig_pos_reached = 1;
			if(compute_dist(g_curr_pose_in_lawn, g_orig_pose_in_lawn) <= CHRGST_DOMAIN_RADIUS)
				g_trig_chrgst_reached = 1; 		//##################### TEMPORARY TRY
			break;
		
		
		case WIRE_SENSED: 
			get_pose_in_lawn(); 						//##### NOTICE: not inevitable, can be from "update_map_nav"
		
			if(1 == g_flg_sens_stp_unoccur)	//sensing stop for the first time
			{
				g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
				g_lst_bstr_width = 0.0;
				g_trig_wire_sensed_plain = 1;		//plain boustrophedon in cell
				
				g_flg_sens_stp_unoccur = 0;
			}
			else 
			{
				if(0 == g_lst_bstr_width)		//sensing stop for the second time
				{
					g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
					g_lst_bstr_width = compute_dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
					g_trig_wire_sensed_plain = 1;		//plain boustrophedon in cell
				}
				else
				{
					g_curr_bstr_width = compute_dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
					
					if(g_curr_bstr_width < DEAD_END_CLEARENCE)
						g_trig_cell_end_reached = 1; 		//reach cell end
					else if(g_curr_bstr_width - g_lst_bstr_width >= CELL_WCHG_UPPER_BOUND)
						g_trig_wire_bstr_surge = 1; 		//boustrophedon width grow significantly
					else if(g_curr_bstr_width - g_lst_bstr_width <= -CELL_WCHG_UPPER_BOUND)
						g_trig_wire_bstr_plunge = 1; 		//boustrophedon width reduce significantly
					else
						g_trig_wire_sensed_plain = 1;		//plain boustrophedon in cell
					
					g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
					g_lst_bstr_width = g_curr_bstr_width;
				}
			}
			break;
		
			
		case RIGID_TOUCHED:	
			get_pose_in_lawn(); 								//##### NOTICE: not inevitable, can be from "update_map_nav"
			
			if((compute_dist(g_curr_pose_in_lawn, g_orig_pose_in_lawn) <= CHRGST_DOMAIN_RADIUS) && g_flg_outer_bdry_mapping)
			{
				g_trig_chrgst_reached = 1; 				//reach the charge station
				g_flg_outer_bdry_mapping = 0;     //###################### TEMPORARY TRY ########################
			}
			else if(1 == g_flg_sens_stp_unoccur)	//sensing stop for the 1st time
			{
				g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
				g_lst_bstr_width = 0.0;
				g_trig_rigid_touched_plain = 1; 	//plain boustrophedon in cell
				
				g_flg_sens_stp_unoccur = 0;
			}
			else 
			{
				if(0 == g_lst_bstr_width)		//sensing stop for the 2nd time
				{
					g_lst_sens_stp_pose_in_lawn = g_curr_pose_in_lawn;
					g_lst_bstr_width = compute_dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
					g_trig_rigid_touched_plain = 1; 	//plain boustrophedon in cell
				}
				else	  //sensing stop after 3rd times(included)
				{
					g_curr_bstr_width = compute_dist(g_curr_pose_in_lawn, g_lst_sens_stp_pose_in_lawn);
					
					if(g_curr_bstr_width < DEAD_END_CLEARENCE)	//##### NOTICE: here may another way instead, discuss with @WU HAOWEN
						g_trig_cell_end_reached = 1; 			//reach cell end
					else if(fabs(g_curr_bstr_width - g_lst_bstr_width) >= CELL_WCHG_UPPER_BOUND)
						g_trig_rigid_bstr_sigchg = 1; 		//boustrophedon width change significantly
					else
						g_trig_rigid_touched_plain = 1; 	//plain boustrophedon in cell
					
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
int decision_assert(T_action *action_out, T_params_act *params_out)
{	
	int i;
	int untraversed;
	
	int t1 = g_trig_wire_sensed_plain; 
	int t2 = g_trig_rigid_touched_plain;
	int t3 = g_trig_wire_bstr_plunge; 
	int t4 = g_trig_wire_bstr_surge; 
	int t5 = g_trig_rigid_bstr_sigchg;
	
	int t6 = g_trig_chrgst_reached; //mapping proc, charge station
	int t7 = g_flg_mapping && g_trig_cell_end_reached;	//mapping proc, cell end
	int t8; 																						//nav proc, cell end, has uncov cells
	int t9; 																						//nav proc, cell end, all cells covered
	int t10; 																						//nav proc, bstr, has uncov cells
	int t11;																						//nav proc, bstr over last cell(into a new one)
	
	float dir_bstr_head;
	float dir_bstr_opp; 
	float dir_in_lawn; 
	float dir_enu; 
	T_pose_m pose_on_map;
	T_pose uncov_cell_ent;
	T_pose goal;
	
	L_i_list pos_list;
	L_i_list dir_list;
	
	
	//ASSERT: Initializing Process(NO params out)
	if((1 == g_trig_init_proc) && (0 == g_planner_initialized))
	{
		planner_init();
		return 0;
	}
	
	//ASSERT: TO_POSE
	untraversed = get_uncov_cell_entry(&uncov_cell_ent);
	t8 = (!g_flg_mapping) && g_trig_cell_end_reached && (-1 != untraversed);	
	t9 = (!g_flg_mapping) && g_trig_cell_end_reached && (-1 == untraversed);	
	t10 = (!g_flg_mapping) && g_flg_endline_ever_touched && (t3 || t4 || t5) && (-1 != untraversed); 
	t11 = (!g_flg_mapping) && g_flg_endline_ever_touched && (t3 || t4 || t5) && (-1 == untraversed); 
	
	if(t6 || t7 || t8 || t9 || t10)
	{	
		if(t6)			//mapping proc, charge station ==> go to cell_0 left entry
		{
			if(g_flg_mapping == 1)
			{
				map_clip(); 
				//map_frame_send_sraus_msg.send_map_info_flg = 1; //notify to @Gao Xiang
				
				boundary_filter();
				build_adj_graph(g_rot_bstr_prog_dir); 
				goal = g_cell_entries[0].left_entry; 
			}
			else
			{
				boundary_filter();
				build_adj_graph(g_rot_bstr_prog_dir); 
				goal = g_cell_entries[0].left_entry; 
			}
		}
		else if(t7)	//mapping proc, cell end ==> go to charge station, set flg_mapping to 0
		{
			g_flg_mapping = 0;
			goal = g_orig_pose_in_lawn;
		}
		else if(t8)	//nav proc, cell end, has uncov cells ==> go to the objective cell
		{
			g_flg_endline_ever_touched = 1;
			goal = uncov_cell_ent;
		}
		else if(t9)	//nav proc, cell end, all cells covered ==> go to charge station
			goal = g_orig_pose_in_lawn;
		else if(t10)	//nav proc, bstr, touched endline ever, has uncov cells ==> go to the objective cell
			goal = uncov_cell_ent;
		else if(t11)	//nav proc, bstr, touched endline ever, covering last cell(into a new one) ==> go to charge station
			goal = g_orig_pose_in_lawn;
		
		//call global planner to generate global path
		if(0 == g_path_cursor)
		{
			L_i_init(&pos_list);
			L_i_init(&dir_list);
			
			get_pose_in_lawn();
			get_global_path(g_curr_pose_in_lawn, goal, pos_list, dir_list);
			g_path_length = L_i_get_length(pos_list);
			g_intercell_path = rt_calloc(g_path_length, sizeof(T_pose));
			
			for(i = 0; i < g_path_length; i++)
			{
				ind2sub(g_map.info.width, g_map.info.height, (int)(L_i_retrieve(pos_list, i)), &pose_on_map.pos.x, &pose_on_map.pos.y);
				switch((int)(L_i_retrieve(dir_list, i)))
				{
					case 0: 	//go to right
						pose_on_map.th = 0.0;
						break;
					
					case 1: 	//go to left
						pose_on_map.th = -PI_;
						break;
					
					case 2: 	//go to up
						pose_on_map.th = PI_/2;
						break;
					
					case 3: 	//go to down
						pose_on_map.th = -PI_/2;
						break;
					
					default:
						break;
				}
				
				g_intercell_path[i].pos.x = (pose_on_map.pos.x + g_tr_map_in_lawngrid.x) * g_map.info.resolution;
				g_intercell_path[i].pos.y = (pose_on_map.pos.y + g_tr_map_in_lawngrid.y) * g_map.info.resolution;
				g_intercell_path[i].th = pose_on_map.th;
			}
			
			L_i_delete_all_but_header(pos_list); //delete all nodes but head
			rt_free(pos_list);  //delete head node
			L_i_delete_all_but_header(dir_list);
			rt_free(dir_list);
		}
		
		if(g_path_cursor < g_path_length)
		{
			g_path_cursor++;
			*action_out = TO_POSE; 
			params_out->to_pose_ = tf_lawn2ned(g_intercell_path[g_path_cursor]);
		}
		else
		{
			//ASSERT: DIR_DRIVE(after global path into new cell)
			update_graph_status(1); 
			
			g_path_cursor = 0;
			rt_free(g_intercell_path);
			
			*action_out = DIR_DRIVE; 
			params_out->dir_drive_.rot_side = RIGHT_STEERING; //##### NOTICE: TEMPORARY TRY #####
			dir_enu = angle_sub(goal.th, -g_rot_lawn_in_enu);
			params_out->dir_drive_.fin_vec[1] = cos(dir_enu);
			params_out->dir_drive_.fin_vec[0] = sin(dir_enu);
			params_out->dir_drive_.fin_vec[2] = 0;
		}		
		
		return 0;
	}
	
	
	//ASSERT: DIR_DRIVE(after global path into new cell)
	//########## TO BE IMPLEMENTED ##########
	
	
	//ASSERT: ONTO_WIRE & U-TURN
	if((1 == t1)||(1 == t2)||(1 == t3)||(1 == t4)||(1 == t5))
	{
		if((1 == t3) && (1 == g_flg_mapping))
		{ 
			//assert: ONTO_WIRE
			*action_out = ONTO_WIRE;
			params_out->onto_wire_.enter_side = compute_turn_side();
			params_out->onto_wire_.laps = CIRCLING_LAPS; 
			
			dir_bstr_opp = compute_heading_dir(1); //0: the current robot dir, 1: opposite dir
			dir_in_lawn = angle_sub(dir_bstr_opp, -g_rot_bstr_prog_dir);
			dir_enu = angle_sub(dir_in_lawn, -g_rot_lawn_in_enu);
			params_out->onto_wire_.fin_vec[1] = cos(dir_enu);
			params_out->onto_wire_.fin_vec[0] = sin(dir_enu);
			params_out->onto_wire_.fin_vec[2] = 0;
			
			g_flg_mapping_source = 0;
			++g_wire_srndd_landscape_cnt;
		}
		else
		{
			//assert: U-TURN
			*action_out = U_TURN;
			params_out->u_turn_.turn_side = compute_turn_side();
			
			dir_bstr_opp = compute_heading_dir(1); //0: the current robot dir, 1: opposite dir
			dir_in_lawn = angle_sub(dir_bstr_opp, -g_rot_bstr_prog_dir); 
			dir_enu = angle_sub(dir_in_lawn, -g_rot_lawn_in_enu); 
			params_out->u_turn_.fin_vec[1] = cos(dir_enu); 
			params_out->u_turn_.fin_vec[0] = sin(dir_enu); 
			params_out->u_turn_.fin_vec[2] = 0; 
						
			if(((1 == t5)||(1 == t2)) && (1 == g_flg_mapping))
			{
				//assert: need to record
				g_flg_mapping_source = 1;
				g_rigid_landscape_cnt = 1; //g_rigid_landscape_cnt will always be 1 during mapping.
				record_track();
			}
			
			if(((1 == t3)||(1 == t4)||(1 == t5)) && (0 == g_flg_mapping))
			{
				//assert: has entered a new cell(adjacency graph)
				update_graph_status(2);
			}
		}
		
		return 0;
	}

	//ASSERT: BYPASS
	//########## TO BE IMPLEMENTED ##########
		
	
}


// The general interface with Motion Control module
int make_decision(T_trigger *trigger, T_action *action_out, T_params_act *params_out)
{	
	//reset all triggers
	g_trig_init_proc = 0; 
	g_trig_pos_reached = 0; 
	g_trig_wire_sensed_plain = 0; 
	g_trig_rigid_touched_plain = 0;
	g_trig_wire_bstr_plunge = 0; 
	g_trig_wire_bstr_surge = 0; 
	g_trig_rigid_bstr_sigchg = 0;
	g_trig_cell_end_reached = 0; 
	g_trig_chrgst_reached = 0; 
	
	event_preproc(trigger);
	decision_assert(action_out, params_out);
	
	return 0;
}


// Implement the basic A* algorithm at first(update later)
int get_global_path(T_pose start_in_lawn, T_pose goal_in_lawn, L_i_list pos_list_out, L_i_list dir_list_out)
{
	T_point_m start;
	T_point_m goal;
	int ndx_start;
	int ndx_goal;
	int path_len;
	
	int int_idx_curr;
	int ext_idx_curr;
	int ext_idx_nbr[4];
	int cost_nbr[4];
	int from_dir;
	
	int tmp_test;
		
	L_i_list closed_set; 		//##### NOTICE: need to FREE after use #####
	L_i_list open_set;			//##### NOTICE: need to FREE after use #####
	L_i_list open_set_f_score;	//##### NOTICE: need to FREE after use #####
	//L_i_list path;
	unsigned char *came_from; //##### NOTICE: need to FREE after use #####
	unsigned short *g_score;	//##### NOTICE: need to FREE after use #####
	
	//start.x = (int)round(start_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x; 
	//start.y = (int)round(start_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y; 
	//goal.x = (int)round(goal_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x; 
	//goal.y = (int)round(goal_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y; 
	start.x = (int)(start_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x; 
	start.y = (int)(start_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y; 
	goal.x = (int)(goal_in_lawn.pos.x/g_map.info.resolution) - g_tr_map_in_lawngrid.x; 
	goal.y = (int)(goal_in_lawn.pos.y/g_map.info.resolution) - g_tr_map_in_lawngrid.y; 
	ndx_start = sub2ind(g_map.info.width, g_map.info.height, start.x, start.y); 
	ndx_goal = sub2ind(g_map.info.width, g_map.info.height, goal.x, goal.y); 
	if(ndx_start == ndx_goal)
		return -1; //the goal is very near the start position, no need to plan
	
	//initialize 'closedSet', 'openSet', and 'openSet_fscore'
	L_i_init(&closed_set);
	L_i_init(&open_set);
	L_i_init(&open_set_f_score);
	//L_i_init(&path);
	L_i_prepend(open_set, ndx_start); //initially, only the start node is known
	L_i_prepend(open_set_f_score, 0 + h_cost_est(ndx_start, ndx_goal)); //the g_score at start point is 0
	
	//initialize 'came_from' and 'g_score'
	came_from = rt_calloc(g_map.info.width * g_map.info.height / (BYTE_BITS/DIR_BITS), sizeof(char)); 
	set_all_2b_grids(came_from, g_map.info.width * g_map.info.height, 0);  		//DIR_BITS = OCC_BITS = 2, share the same function "set_all_grids_2b"
	g_score = rt_calloc(g_map.info.width * g_map.info.height, sizeof(unsigned short)); //##### REMEMEBER to free it after use!!!
	set_all_16b_grids(g_score, g_map.info.width * g_map.info.height, MAX_USHORT); 
	g_score[ndx_start] = (unsigned short)0; 
	
	while(1 != L_i_is_empty(open_set))
	{
		int_idx_curr = L_i_get_idx_of_min(open_set_f_score);
		ext_idx_curr = (int)L_i_retrieve(open_set, int_idx_curr);  //### idx STARTS "0", as in array 
		if(ext_idx_curr == ndx_goal)
		{
			reconstruct_path(ext_idx_curr, came_from, ndx_start, pos_list_out, dir_list_out); //NEED TO FREE PATH AND CALLLOC PATH_OUT
			//*path_ptr_out = rt_calloc(path_len, sizeof(T_pose));
			
			L_i_delete_all_but_header(closed_set);
			rt_free(closed_set);
			L_i_delete_all_but_header(open_set);
			rt_free(open_set);
			L_i_delete_all_but_header(open_set_f_score);
			rt_free(open_set_f_score);
			rt_free(came_from);
			rt_free(g_score);
			
			return 0;  //calling function success
		}
		
		L_i_delete_by_idx(open_set, int_idx_curr);  //### idx STARTS "0", as in array
		L_i_delete_by_idx(open_set_f_score, int_idx_curr); //### idx STARTS "0", as in array
		L_i_prepend(closed_set, (unsigned short)ext_idx_curr); 
		
		//get 4 neighbors of current grid
		get_neighbor_cost(g_score, ndx_goal, ext_idx_curr, ext_idx_nbr, cost_nbr);
		for(from_dir = 0; from_dir < 4; from_dir++)
		{
			//if((cost_nbr[from_dir] == MAX_USHORT) && (ext_idx_nbr[from_dir] != ndx_goal)) //### TEMP TRY, FAILED
			if(cost_nbr[from_dir] == MAX_USHORT) //obstacle
				continue;
			else
			{
				if(1 == L_i_is_member(closed_set, ext_idx_nbr[from_dir]))	//if in closedSet, ignore
					continue;
				
				if(0 == L_i_is_member(open_set, ext_idx_nbr[from_dir]))		//if not in openSet, add in
				{
					L_i_prepend(open_set, (unsigned short)(ext_idx_nbr[from_dir]));
					L_i_prepend(open_set_f_score, (unsigned short)MAX_USHORT);
				}
				
				tmp_test = (int)(g_score[ext_idx_nbr[from_dir]]);
				//if((cost_nbr[from_dir] >= tmp_test) && (ext_idx_nbr[from_dir] != ndx_goal)) //### TEMP TRY, FAILED
				if(cost_nbr[from_dir] >= tmp_test)	//if not smaller than old cost, ignore
					continue;
				
				//otherwise, if smaller than old cost, update
				set_2b_grid_value(came_from, ext_idx_nbr[from_dir], from_dir);
				g_score[ext_idx_nbr[from_dir]] = (unsigned short)(cost_nbr[from_dir]);
				L_i_set_by_idx(open_set_f_score, 0, (unsigned short)(cost_nbr[from_dir] + h_cost_est(ext_idx_nbr[from_dir], ndx_goal))); //### idx STARTS "0", as in array
			}
		}
	}
	
	L_i_delete_all_but_header(closed_set);
	rt_free(closed_set);
	L_i_delete_all_but_header(open_set);
	rt_free(open_set); 
	L_i_delete_all_but_header(open_set_f_score); 
	rt_free(open_set_f_score);
	rt_free(came_from);
	rt_free(g_score);
	
	return -1; //failure
}


// Reconstruct the path from goal to start
int reconstruct_path(int ndx_curr, unsigned char *field_ptr, int ndx_start, L_i_list pos_list_out, L_i_list dir_list_out)
{
	int sub_x_curr;
	int sub_y_curr;
	int value;
	int go_to;
	
	L_i_prepend(pos_list_out, (unsigned short)ndx_curr);
	L_i_prepend(dir_list_out, (unsigned short)MAX_USHORT); //the direction pointer of goal is N/A
	while(ndx_curr != ndx_start)
	{
		ind2sub(g_map.info.width, g_map.info.height, ndx_curr, &sub_x_curr, &sub_y_curr);
		value = get_2b_grid_value(field_ptr, ndx_curr);
		switch(value)
		{
			case 0: 	//from right
				ndx_curr = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr+1, sub_y_curr);
				go_to = 1;
				break;
			
			case 1: 	//from left
				ndx_curr = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr-1, sub_y_curr);
				go_to = 0;
				break;
			
			case 2: 	//from up
				ndx_curr = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr, sub_y_curr+1);
				go_to = 3;
				break;
			
			case 3: 	//from down
				ndx_curr = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr, sub_y_curr-1);
				go_to = 2;
				break;
			
			default:
				break;
		}
		
		L_i_prepend(pos_list_out, (unsigned short)ndx_curr);
		L_i_prepend(dir_list_out, (unsigned short)go_to);
	}
	
	return 0;
}


// get cost of the 4 adjacent grid around current
int get_neighbor_cost(unsigned short *cost_chart, int ndx_goal, int ndx_curr, int *ndx_nbr, int *cost_nbr)
{
	int sub_x_curr;
	int sub_y_curr;
	int i;
	int value; 
	
	for(i = 0; i < 4; i++)
	{
		ndx_nbr[i] = 0;
		cost_nbr[i] = MAX_USHORT;
	}
	
	ind2sub(g_map.info.width, g_map.info.height, ndx_curr, &sub_x_curr, &sub_y_curr);
	if(sub_x_curr-1 >= 0)
	{
		ndx_nbr[0] = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr-1, sub_y_curr);
		value = get_2b_grid_value(g_map.data, ndx_nbr[0]);
		if((1 != value) || (ndx_goal == ndx_nbr[0])) //not obstacle
			cost_nbr[0] = cost_chart[ndx_curr] + 1;
	}
	if(sub_x_curr+1 <= g_map.info.width-1)
	{
		ndx_nbr[1] = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr+1, sub_y_curr);
		value = get_2b_grid_value(g_map.data, ndx_nbr[1]);
		if((1 != value) || (ndx_goal == ndx_nbr[1])) //not obstacle
			cost_nbr[1] = cost_chart[ndx_curr] + 1;
	}
	if(sub_y_curr-1 >= 0)
	{
		ndx_nbr[2] = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr, sub_y_curr-1);
		value = get_2b_grid_value(g_map.data, ndx_nbr[2]);
		if((1 != value) || (ndx_goal == ndx_nbr[2])) //not obstacle
			cost_nbr[2] = cost_chart[ndx_curr] + 1;
	}
	if(sub_y_curr+1 <= g_map.info.height-1)
	{
		ndx_nbr[3] = sub2ind(g_map.info.width, g_map.info.height, sub_x_curr, sub_y_curr+1);
		value = get_2b_grid_value(g_map.data, ndx_nbr[3]);
		if((1 != value) || (ndx_goal == ndx_nbr[3])) //not obstacle
			cost_nbr[3] = cost_chart[ndx_curr] + 1;
	}
	
	/*
	for(i = 0; i < 4; i++)
	{
		value = get_2b_grid_value(g_map.data, ndx_nbr[i]);
		if(1 == value) //obstacle
			cost_nbr[i] = MAX_USHORT;
		else
			cost_nbr[i] = cost_chart[ndx_curr] + 1;
	}*/
}


// Compute turn side
T_side compute_turn_side()
{
	T_side side;
	get_pose_bstr();
	
	if(fabs(angle_sub(g_curr_pose_bstr.th, PI_/2)) <= HEADING_ANGLE_TOL)
		side = RIGHT_STEERING; 
	
	if(fabs(angle_sub(g_curr_pose_bstr.th, -PI_/2)) <= HEADING_ANGLE_TOL)
		side = LEFT_STEERING; 
	
	return side;
}


// Compute opposite direction
float compute_heading_dir(int heading_type)
{
	float op_dir;
	
	get_pose_bstr();
	if(0 == heading_type)
	{
		if(fabs(angle_sub(g_curr_pose_bstr.th, PI_/2)) <= HEADING_ANGLE_TOL)
			op_dir = PI/2;
		else if(fabs(angle_sub(g_curr_pose_bstr.th, -PI_/2)) <= HEADING_ANGLE_TOL)
			op_dir = -PI/2;
		else
			op_dir = 0; //############## NOTICE: JUST FOR TEST!!!################
	}
	
	if(1 == heading_type)
	{
		if(fabs(angle_sub(g_curr_pose_bstr.th, PI_/2)) <= HEADING_ANGLE_TOL)
			op_dir = -PI/2;
		else if(fabs(angle_sub(g_curr_pose_bstr.th, -PI_/2)) <= HEADING_ANGLE_TOL)
			op_dir = PI/2;
		else
			op_dir = 0; //############## NOTICE: JUST FOR TEST!!!################
	}
	
	return op_dir;
}


// Euler distance of two point2d
float compute_dist(T_pose pose_a, T_pose pose_b)
{
	return sqrt(pow(pose_a.pos.x-pose_b.pos.x, 2) + pow(pose_a.pos.y-pose_b.pos.y, 2));
}


// Make sure the SUM of 2 angles is in (-pi,+pi]
float angle_add(float ang_a, float ang_b)
{
	float ang_sum = ang_a + ang_b;
	
	while(ang_sum <= -PI_)
		ang_sum += 2*PI_;
	
	while(ang_sum > PI_)
		ang_sum -= 2*PI_;
	
	return ang_sum;
}


// Make sure the DIFFERENCE of 2 angles is in (-pi,+pi]
float angle_sub(float ang_a, float ang_b)
{
	float ang_diff = ang_a - ang_b;
	
	while(ang_diff <= -PI_)
		ang_diff += 2*PI_;
	
	while(ang_diff > PI_)
		ang_diff -= 2*PI_;
	
	return ang_diff;
}


// Rotation transformation
T_pose tf_general(T_pose pose, float new_coord_ang)
{
	T_pose new_pose;
	
	new_pose.pos.x = pose.pos.x*cos(new_coord_ang) + pose.pos.y*sin(new_coord_ang);
	new_pose.pos.y = -pose.pos.x*sin(new_coord_ang) + pose.pos.y*cos(new_coord_ang);
	new_pose.th = angle_sub(pose.th, new_coord_ang);
	
	return new_pose;
}


// Pose transformation from lawn to NED coordinate
T_param_to_pose tf_lawn2ned(T_pose pose_in_lawn)
{
	T_pose pose_enu;
	T_param_to_pose pose_ned;
	
	pose_enu = tf_general(pose_in_lawn, -g_rot_lawn_in_enu); 
	pose_ned.pos[1] = pose_enu.pos.x; 
	pose_ned.pos[0] = pose_enu.pos.y; 
	pose_ned.pos[2] = 0;
	pose_ned.vec[1] = cos(pose_enu.th);
	pose_ned.vec[0] = sin(pose_enu.th);
	pose_ned.vec[2] = 0;
	
	return pose_ned;
}


