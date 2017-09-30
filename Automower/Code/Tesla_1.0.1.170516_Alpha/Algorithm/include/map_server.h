/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: map_server.h
  Author		: kangshaodong
  Version		: V1.0.0      
  Date			: 2017/09/12
  Description	: Head file for the "map_server.c"
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/
#ifndef __MAP_SERVER_H__
#define __MAP_SERVER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <rtthread.h>
#include "global.h"

/* Macro Definition*/
#define BYTE_BITS 8 
#define OCC_BITS 2			//use 2 bits to represent 4 states of grid
#define DIR_BITS 2			//use 2 bits to represent 4 directions of grid

// 4 states of grid occupancy
#define OCC_UNKNOWN 0
#define OCC_OBSTACLE 1
#define OCC_COVERED 2
#define OCC_RESERVED 3

#define MAX_USHORT 65535	//for INF in the g_score costchart

// 4 directions of map navigation
#define DIR_R 0 	//right
#define DIR_L 1 	//left
#define DIR_U 2 	//up
#define DIR_D 3 	//down

#define MAX_CELL_NUM 50				//max number of adjacency list vertices
#define MAX_LANDSCAPE_NUM 10	//max number of landscape(shared by wire-surrounded group or rigid group)
#define MAX_CP_NUM 40

#define LNDSCP_CENTER_TOL 1		//wire surrounded landscape center tolerance
#define SLIDING_WINDOW_LEN 2  //length of sliding window for boundary points smoothing

// 4 types of critical points
#define CELL_SPLIT 0 			//cell split
#define CELL_ORIGINATE 1 	//cell originate
#define CELL_MERGE 2 			//cell merge
#define CELL_CEASE 3 			//cell cease


/* Structs Declaration */
typedef struct
{
	float x; 
	float y; 
} T_point;		//2d position data

typedef struct
{
	int x; 
	int y; 
} T_point_m;	//2d position data on map

typedef struct
{
	T_point pos;
	float th;
} T_pose;			//2d planar pose(general)

typedef struct
{
	T_point_m pos;
	float th;
} T_pose_m;		//2d planar pose(on map)

typedef struct
{
	T_pose charge_st;
	float resolution;
	unsigned int width;
	unsigned int height;
} T_map_metadata;				//map info

typedef struct
{
	T_map_metadata info;
	unsigned char* data;	//map data: 2 bits as 1 state, 4 states included in 1 char(byte)
} T_occ_grid;						//type of the map 


/* Linked List(int node) Declaration */
typedef struct L_i_node_
{
	unsigned short element; 	 //the 'ndx' of coord(x,y)
	struct L_i_node_ *next;
} L_i_node;
typedef L_i_node *L_i_list;
typedef L_i_node *L_i_cursor; 

void L_i_init(L_i_list *list_ptr);
int L_i_is_empty(L_i_list list); 
int L_i_is_member(L_i_list list, unsigned short elem); 
void L_i_prepend(L_i_list list, unsigned short elem); 
void L_i_append(L_i_list list, unsigned short elem); 
unsigned short L_i_retrieve(L_i_list list, int idx); //compatible with negative idx
int L_i_set_by_idx(L_i_list list, int idx, unsigned short elem); 
int L_i_get_idx_of_min(L_i_list list); 
int L_i_get_length(L_i_list list); 
void L_i_insert_by_idx(L_i_list list, int idx, unsigned short elem);
void L_i_delete_by_idx(L_i_list list, int idx); 
void L_i_delete_all_but_header(L_i_list list); 


/* Plural List(int node) Declaration */
// operate as the Adjacency List
typedef L_i_node *PL_i_list; 
typedef struct
{
	T_pose left_entry; 
	T_pose right_entry; 
} T_cell_entry;					//entry of cell

//void A_init(A_graph *graph_ptr);
void PL_i_init(PL_i_list *PL_list_ptr, int nitems); 
void PL_i_clear(PL_i_list PL_list, int nitems);
void PL_i_insert(PL_i_list graph, int p_node, int c_node);


/* Linked List(point node) Declaration */
typedef struct L_p_node_
{
	T_pose element; 	 //the pose in lawn coord
	struct L_p_node_ *next;
} L_p_node;
typedef L_p_node *L_p_list; 
typedef L_p_node *L_p_cursor; 

void L_p_init(L_p_list *list_ptr); 
int L_p_is_empty(L_p_list list); 
//int L_p_is_member(L_p_list list, T_pose elem); 
void L_p_prepend(L_p_list list, T_pose elem); 
void L_p_append(L_p_list list, T_pose elem); 
T_pose L_p_retrieve(L_p_list list, int idx); //compatible with negative idx
int L_p_set_by_idx(L_p_list list, int idx, T_pose elem); 
//int L_p_get_idx_of_min(L_p_list list); 
int L_p_get_length(L_p_list list); 
void L_p_delete_by_idx(L_p_list list, int idx); 
void L_p_delete_all_but_header(L_p_list list); 


/* Plural List(point node) Declaration */
typedef L_p_node *PL_p_list; 

void PL_p_init(PL_p_list *PL_list_ptr, int nitems); 
void PL_p_clear(PL_p_list PL_list, int nitems); 


/* Variables Declaration */
extern T_occ_grid g_map;								//the active map
extern T_pose_m g_current_pose_on_map;	//current pose on map(for @Gao Xiang)
extern T_point_m g_tr_map_in_lawngrid;	//translation(unit) of map orig refer to lawn grid;

extern int g_map_color_negate;	//if true, whiter pixels are occupied, and blacker pixels are free
extern float g_map_occ_thh;			//occupied threshold
extern float g_map_free_thh;		//free threshold
extern float g_map_resol;				//resolution of map: distance(m) of one grid
extern float g_max_lawn_width;	//max width of lawn(m)
extern float g_max_lawn_height; //max height of lawn(m)

extern PL_i_list g_graph; //the global adjacency graph
extern int g_adj_vexnum; 
extern unsigned char g_vertices_marked[MAX_CELL_NUM];
extern T_cell_entry g_cell_entries[MAX_CELL_NUM];

extern L_p_list g_outer_bdry;										//outer boundary
extern PL_p_list g_wire_srndd_landscape_bdrys;	//wire surrounded landscape boundaries, no need to free, rebuild or load as g_map
extern PL_p_list g_rigid_landscape_bdrys; 			//rigid landscape boundaries, no need to free, rebuild or load as g_map
extern int g_wire_srndd_landscape_cnt; 
extern int g_rigid_landscape_cnt; 
extern int g_total_bdrys_cnt;


/* Functions Declaration */
int map_init(); 
int graph_init();
int graph_purge();
int sub2ind(int width, int height, int sub_x, int sub_y);
int ind2sub(int width, int height, int ndx, int *sub_x_out, int *sub_y_out);
int set_all_2b_grids(unsigned char *grids, int value);
int set_all_16b_grids(unsigned short *grids, int value);
int set_2b_grid_value(unsigned char *grids, int ndx, int value);
int get_2b_grid_value(unsigned char *grids, int ndx);
int set_bits(char *x_ptr, int p, int n, int state);
int get_bits(char x, int p, int n);

int map_clip(); 
int map_fill(); //### TO BE IMPLEMENTED
int h_cost_est(int ndx_a, int ndx_b); 
int ray_tracing(); //### TO BE IMPLEMENTED

int map_send(); //### TO BE IMPLEMENTED
int map_load(); //### TO BE IMPLEMENTED
int map_save(); //### TO BE IMPLEMENTED

void boundary_filter(); 
T_point compute_mean_point(L_p_list wire_bdry); 
void moving_avg(L_p_list list, int window_len); 


#endif
