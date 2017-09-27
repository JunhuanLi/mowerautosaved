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

#define OCC_UNKNOWN 0
#define OCC_OBSTACLE 1
#define OCC_COVERED 2
#define OCC_RESERVED 3


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


/* Variables Declaration */
extern T_occ_grid g_map;								//the active map

//pose data
extern T_pose_m g_pose_on_map;					//a point on map

//coordinate relations
extern T_point_m g_tr_map_in_lawngrid;	//translation(unit) of map orig refer to lawn grid;

//pre-params of map
extern int g_map_color_negate;	//if true, whiter pixels are occupied, and blacker pixels are free
extern float g_map_occ_thh;			//occupied threshold
extern float g_map_free_thh;		//free threshold
extern float g_map_resol;				//resolution of map: distance(m) of one grid
extern float g_max_lawn_width;	//max width of lawn(m)
extern float g_max_lawn_height; //max height of lawn(m)


/* Functions Declaration */
int map_init(); 
int sub2ind(int width, int height, int sub_x, int sub_y);
int ind2sub(int width, int height, int ndx, int *sub_x_out, int *sub_y_out);
int set_all_grids_zero();
int set_grid_value(int ndx, int value);
int get_grid_value(int ndx);
int set_bits(char *x_ptr, int p, int n, int state);
int get_bits(char x, int p, int n);

int map_clip(); 
int map_fill(); 
//int cell_decomp(T_adj_list graph_out); 
int ray_tracing(); 

int map_send(); 
int map_load(); 
int map_save(); 


// ##### Linked List ######
struct L_node; 
typedef struct L_node *L_node_ptr; 
typedef L_node_ptr L_list; 
typedef L_node_ptr L_cursor; 

int L_is_empty(L_list list); 
int L_is_member(int elem, L_list list); 
void L_insert_tail(int elem, L_list list); 
void L_delete(int elem, L_list list); 
int L_get_min(L_list list); 
//int L_is_last(L_cursor curs, L_list list);
//L_cursor L_find_previous(int elem, L_list list);
//int L_retrieve(L_cursor curs); 

// ########################


// ##### Adjacency List ######
/*typedef struct
{
} T_adj_list;				//### adjacency list(unfinished) ###
*/
// ########################





#endif
