/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: map_server.c
  Author		: kangshaodong
  Version		: V1.0.0      
  Date			: 2017/09/12
  Description	: Record, save and provide the map.
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#include "map_server.h"

/* Variables Definition and Initialization */
T_occ_grid g_map; 
T_pose_m g_current_pose_on_map; 
T_point_m g_tr_map_in_lawngrid; 

int g_map_color_negate = 0; 
float g_map_occ_thh = 0.65; 
float g_map_free_thh = 0.196; 
float g_map_resol = 0.25; 
float g_max_lawn_width = 30; 
float g_max_lawn_height = 30; 

PL_i_list g_graph; 
int g_adj_vexnum = 0; 
unsigned char g_vertices_marked[MAX_CELL_NUM];
T_cell_entry g_cell_entries[MAX_CELL_NUM];

L_p_list g_outer_bdry;
PL_p_list g_wire_srndd_landscape_bdrys; 
PL_p_list g_rigid_landscape_bdrys; 
int g_wire_srndd_landscape_cnt = 0; 
int g_rigid_landscape_cnt = 0; 
int g_total_bdrys_cnt = 0;


/* Functions Definition */

// Map initialize
int map_init()
{
	//char *mapname = "robotic_mower_map";
	g_map.info.resolution = g_map_resol;
	g_map.info.width = (int)(g_max_lawn_width*1.5*2 / g_map_resol);	//1.5 as about sqrt(2)
	g_map.info.height = (int)(g_max_lawn_height*1.5*2 / g_map_resol);
		
	//charge station(the origin of the 'LAWN GRID' coordinate) in the 'MAP' coordinate. 
	g_map.info.charge_st.pos.x = (int)(g_map.info.width/2); 
	g_map.info.charge_st.pos.y = (int)(g_map.info.height/2); 
	g_map.info.charge_st.th = 0.0; 
	
	//initialize 'MAP' origin in the 'LAWN GRID' coordinate, it will be mended after trim of map
	g_tr_map_in_lawngrid.x = -g_map.info.charge_st.pos.x;
	g_tr_map_in_lawngrid.y = -g_map.info.charge_st.pos.y;
		
	//allocate map space and set to zeros for initialization
	g_map.data = calloc(g_map.info.width * g_map.info.height, (size_t)OCC_BITS); 
	set_all_2b_grids(g_map.data, 0); 
	
	return 0;
}


// Graph initialize
// OUTPUT: g_graph, g_vertices_marked[], g_cell_entries[]
int graph_init()
{
	int i;
	
	PL_i_init(&g_graph, MAX_CELL_NUM);
	
	for(i = 0; i < MAX_CELL_NUM; i++)
	{
		g_vertices_marked[i] = (unsigned char)0;
		g_cell_entries[i].left_entry.pos.x = 0.0;
		g_cell_entries[i].left_entry.pos.y = 0.0;
		g_cell_entries[i].left_entry.th = 0.0;
		g_cell_entries[i].right_entry.pos.x = 0.0;
		g_cell_entries[i].right_entry.pos.y = 0.0;
		g_cell_entries[i].right_entry.th = 0.0;
	}	
	
	return 0;
}


// Graph purge
int graph_purge()
{
	int i;
	
	PL_i_clear(g_graph, MAX_CELL_NUM);
	g_adj_vexnum = 0;
	
	for(i = 0; i < MAX_CELL_NUM; i++)
	{
		g_vertices_marked[i] = (unsigned char)0;
		g_cell_entries[i].left_entry.pos.x = 0.0;
		g_cell_entries[i].left_entry.pos.y = 0.0;
		g_cell_entries[i].left_entry.th = 0.0;
		g_cell_entries[i].right_entry.pos.x = 0.0;
		g_cell_entries[i].right_entry.pos.y = 0.0;
		g_cell_entries[i].right_entry.th = 0.0;
	}	
	
	return 0;
}


// Compute linear index from multiple subscripts ("rows reversion" included)
int sub2ind(int width, int height, int sub_x, int sub_y)
{
	int ndx = sub_x + (height-sub_y-1) * width; 
	
	return ndx; 
}


// Compute multiple subscripts from linear index ("rows reversion" included)
int ind2sub(int width, int height, int ndx, int *sub_x_out, int *sub_y_out)
{
	*sub_x_out = ndx % width;
	*sub_y_out = height - (ndx/width) - 1;

	return 0;
}


// Set all grids equal to 'value'(2 bits for each state grids)
int set_all_2b_grids(unsigned char *grids, int value)
{	
	int ndx;
	int map_length_of_bytes; 
	
	map_length_of_bytes = g_map.info.width*g_map.info.height / (BYTE_BITS/OCC_BITS); //DIR_BITS = OCC_BITS = 2
	for(ndx = 0; ndx < map_length_of_bytes; ndx++)
		grids[ndx] = (unsigned char)value; 
	
	return 0;
}


// set all grids equal to 'value'(16 bits for each state grids)
int set_all_16b_grids(unsigned short *grids, int value)
{
	int ndx;
	int map_length_of_2bytes; 
	
	map_length_of_2bytes = g_map.info.width * g_map.info.height;
	for(ndx = 0; ndx < map_length_of_2bytes; ndx++)
		grids[ndx] = (unsigned short)value; 
	
	return 0;
}


// Write the state(0,1,2,3) to the objective 2bit in the memory space
int set_2b_grid_value(unsigned char *grids, int ndx, int value)
{
	char obj_byte = grids[ndx/(BYTE_BITS/OCC_BITS)];
	int pos_in_byte = 2*((BYTE_BITS/OCC_BITS) - ndx%(BYTE_BITS/OCC_BITS) - 1) + 1;
	set_bits(&obj_byte, pos_in_byte, OCC_BITS, value);
	grids[ndx/(BYTE_BITS/OCC_BITS)] = obj_byte;
	
	return 0;
}


// Read the state of grid out
int get_2b_grid_value(unsigned char *grids, int ndx)
{
	char obj_byte = grids[ndx/(BYTE_BITS/OCC_BITS)];
	int pos_in_byte = 2*((BYTE_BITS/OCC_BITS) - ndx%(BYTE_BITS/OCC_BITS) - 1) + 1;
	int value_out = get_bits(obj_byte, pos_in_byte, OCC_BITS);
	
	return value_out;
}


// Set the desired bits in 'x'(start at 'p'th bit from the right, set following 'n' bits with 'state')
int set_bits(char *x_ptr, int p, int n, int state)
{
	char x = *x_ptr;
	*x_ptr = (x & ~(~(~0 << n) << (p+1-n))) | (state << (p+1-n));
	
	return 0;
}


// Read the desired bits in 'x'(start at 'p'th bit from the right, pick 'n' bits out)
int get_bits(char x, int p, int n)
{
	return (x >> (p+1-n)) & ~(~0 << n);
}


//Map clipping
int map_clip()
{
	int limit_left;
	int limit_right;
	int limit_up;
	int limit_down;
	
	int temp_width;
	int temp_height;
	unsigned char *temp_data; 
	int x, y;
	
	//check out the left limit
  for(x = 0; x < g_map.info.width; x+=4)
	{
		for(y = 0; y < g_map.info.height; y++)
		{
			if(g_map.data[sub2ind(g_map.info.width, g_map.info.height, x, y)/(BYTE_BITS/OCC_BITS)] != 0)
			{
				limit_left = x;
				x = g_map.info.width; //to jump out the outer loop layer
				break;
			}
		}
	}
	
	//check out the right limit
  for(x = g_map.info.width-1; x >= 0; x-=4)
	{
		for(y = 0; y < g_map.info.height; y++)
		{
			if(g_map.data[sub2ind(g_map.info.width, g_map.info.height, x, y)/(BYTE_BITS/OCC_BITS)] != 0)
			{
				limit_right = x;
				x = -1; //to jump out the outer loop layer
				break;
			}
		}
	}
	
	//check out the down limit
  for(y = 0; y < g_map.info.height; y++)
	{
		for(x = 0; x < g_map.info.width; x+=4)
		{
			if(g_map.data[sub2ind(g_map.info.width, g_map.info.height, x, y)/(BYTE_BITS/OCC_BITS)] != 0)
			{
				limit_down = y;
				y = g_map.info.height; //to jump out the outer loop layer
				break;
			}
		}
	}
	
	//check out the up limit
  for(y = g_map.info.height-1; y >=0 ; y--)
	{
		for(x = 0; x < g_map.info.width; x+=4)
		{
			if(g_map.data[sub2ind(g_map.info.width, g_map.info.height, x, y)/(BYTE_BITS/OCC_BITS)] != 0)
			{
				limit_up = y;
				y = -1; //to jump out the outer loop layer
				break;
			}
		}
	}
	
	//compute width and height of the new map
	temp_width = limit_right - limit_left + 1;
	temp_height = limit_up - limit_down + 1;
	temp_data = calloc(temp_width * temp_height, (size_t)OCC_BITS); 
	
	//dump the primary data to temp_data
	for(y = temp_height-1; y >= 0; y++)
	{
		for(x = 0; x < temp_width; x+=4)
		{
			int temp_ndx = sub2ind(temp_width, temp_height, x, y);
			int pri_ndx = sub2ind(g_map.info.width, g_map.info.height, x+limit_left, y+limit_down);
			temp_data[temp_ndx/(BYTE_BITS/OCC_BITS)] = g_map.data[pri_ndx/(BYTE_BITS/OCC_BITS)];
		}
	}
	
	free(g_map.data);  //free the space allocated to initial huge map.
	g_map.data = temp_data;
	
	g_map.info.charge_st.pos.x -= limit_left;
	g_map.info.charge_st.pos.y -= limit_down;
	
	g_tr_map_in_lawngrid.x = -g_map.info.charge_st.pos.x;
	g_tr_map_in_lawngrid.y = -g_map.info.charge_st.pos.y;
	
	return 0;
}


// Fill Hole in the map
// ### refer to the Matlab 'imfill' function ###
int map_fill()
{
	//##### TO BE IMPLEMENTED #####
}


// compute the heuristic estimate(Manhattan distance) of two points
int h_cost_est(int ndx_a, int ndx_b)
{
	int sub_x_a;
	int sub_y_a;
	int sub_x_b;
	int sub_y_b;
	int mht_dist;
	
	ind2sub(g_map.info.width, g_map.info.height, ndx_a, &sub_x_a, &sub_y_a);
	ind2sub(g_map.info.width, g_map.info.height, ndx_b, &sub_x_b, &sub_y_b);
	mht_dist = abs(sub_x_b - sub_x_a) + abs(sub_y_b - sub_y_a);
	
	return mht_dist;
}


// Bresenham's raytracing algorithm
int ray_tracing()
{
	//##### TO BE IMPLEMENTED #####
}


// Send map data to PC
/* here or not? */
int map_send()
{
	//##### TO BE IMPLEMENTED #####
}


// Load map
int map_load()
{
	//##### TO BE IMPLEMENTED #####
}


// Save map
int map_save()
{
	//##### TO BE IMPLEMENTED #####
}


// Boundary preprocessing and filtering
// NOTICE: NEED g_wire_srndd_landscape_cnt > 0 or g_rigid_landscape_cnt > 0
void boundary_filter()
{
	int i; 
	int j;
	T_point *mean_pts; 
	float spacing; 
	L_i_list distinct_wire_bdry_idxs; 

	//01.outer boundary
	//smooth the outer boundary
	moving_avg(g_outer_bdry, SLIDING_WINDOW_LEN);
	
	//02.wire boundary: 
	//compute the mean point of each boundary	
	mean_pts = calloc(g_wire_srndd_landscape_cnt, sizeof(T_point));	
	L_i_init(&distinct_wire_bdry_idxs);
	for(i = 0; i < g_wire_srndd_landscape_cnt; i++)
	{
		mean_pts[i] = compute_mean_point(g_wire_srndd_landscape_bdrys + i);
		L_i_append(distinct_wire_bdry_idxs, (unsigned short)i);
	}
	
	//delete the redundant boundaries
	if(g_wire_srndd_landscape_cnt > 1)
	{
		for(i = g_wire_srndd_landscape_cnt-1; i > 0; i--)
		{
			for(j = 0; j < i; j++)
			{
				spacing = sqrt(pow(mean_pts[i].x-mean_pts[j].x, 2) + pow(mean_pts[i].y-mean_pts[j].y, 2));
				if(spacing <= LNDSCP_CENTER_TOL)
				{
					L_p_delete_all_but_header(g_wire_srndd_landscape_bdrys + i);
					L_i_delete_by_idx(distinct_wire_bdry_idxs, i);
				}
			}
		}
		g_wire_srndd_landscape_cnt = L_i_get_length(distinct_wire_bdry_idxs);
	}
	
	//reorganize the surviving boundaries
	if(g_wire_srndd_landscape_cnt > 1)
	{
		for(i = 1; i < g_wire_srndd_landscape_cnt; i++) //no need to move the 0 index element
		{
			if(i < (int)L_i_retrieve(distinct_wire_bdry_idxs, i))
			{
				g_wire_srndd_landscape_bdrys[i].next = g_wire_srndd_landscape_bdrys[(int)L_i_retrieve(distinct_wire_bdry_idxs, i)].next;
				g_wire_srndd_landscape_bdrys[(int)L_i_retrieve(distinct_wire_bdry_idxs, i)].next = NULL;
			}
		}
	}
	
	free(mean_pts);
	L_i_delete_all_but_header(distinct_wire_bdry_idxs);
	free(distinct_wire_bdry_idxs);
	
	//smooth each wire landscape boundary
	for(i = 0; i < g_wire_srndd_landscape_cnt; i++)
		moving_avg(g_wire_srndd_landscape_bdrys + i, SLIDING_WINDOW_LEN);
	
	
	//(DELAY)03.rigid boundary: 
	//clustering using K-MEANS algorithm 
	//########## TO BE IMPLEMENTED ##########
	
	//re-sort each one using TSP algorithm 
	//########## TO BE IMPLEMENTED ##########
	
	//smooth each rigid landscape boundary 
	//########## TO BE IMPLEMENTED ##########
	
	//g_total_bdrys_cnt = 1 + g_wire_srndd_landscape_cnt + g_rigid_landscape_cnt;
	g_total_bdrys_cnt = 1 + g_wire_srndd_landscape_cnt; //### NOTICE: TEMPORARY, need to switch to above line
}

// Compute the mean of wire-surrounded landscape boundary list points
T_point compute_mean_point(L_p_list wire_bdry)
{
	T_point pt; 
	L_p_list cursor = wire_bdry->next;
	int length = 0;
	float x;
	float y;
	
	pt.x = 0.0;
	pt.y = 0.0;
	while(cursor != NULL)
	{
		++length;
		pt.x += cursor->element.pos.x; 
		pt.y += cursor->element.pos.y; 
		cursor = cursor->next; 
	}
	
	pt.x /= length;
	pt.y /= length;
	
	return pt; 
}

// Moving average(Sliding Window Method)
void moving_avg(L_p_list list, int window_len)
{
	int i; 
	int j; 
	int list_len; 
	T_pose pose_ma; 
	T_pose pose;
	L_p_list tmp_list; 
	
	list_len = L_p_get_length(list);
	pose_ma.pos.x = 0.0;
	pose_ma.pos.y = 0.0;
	pose_ma.th = 0.0;
	
	if((window_len <= 0) || (window_len > list_len)) 
	{
		rt_kprintf("the width of the window is not eligible!");
		return;
	}
	
	//compute average
	L_p_init(&tmp_list);
	for(i = 0; i < list_len; i++)
	{
		if(i < window_len-1)
		{
			for(j = 0; j <= i; j++)
			{
				pose = L_p_retrieve(list, j);
				pose_ma.pos.x += pose.pos.x / (i+1);
				pose_ma.pos.y += pose.pos.y / (i+1);
				pose_ma.th += pose.th / (i+1);
			}
		}
		else
		{
			for(j = i-window_len+1; j <= i; j++)
			{	
				pose = L_p_retrieve(list, j);
				pose_ma.pos.x += pose.pos.x / window_len;
				pose_ma.pos.y += pose.pos.y / window_len;
				pose_ma.th += pose.th / window_len;
			}
		}
		
		L_p_append(tmp_list, pose_ma);
		pose_ma.pos.x = 0.0;
		pose_ma.pos.y = 0.0;
		pose_ma.th = 0.0;
	}
	
	//copy to original list
	for(i = 0; i < list_len; i++)
		L_p_set_by_idx(list, i, L_p_retrieve(tmp_list, i));
	
	L_p_delete_all_but_header(tmp_list);
	free(tmp_list);
	
	return;
}



/* #####################
// Load Map
int load_map(char *mapname)
{
	char *mapmarkupfile = strcat(mapname, ".yaml");
	char *mapdatafile;
	
	//read the YAML file
	FILE *yamlfile = fopen(mapmarkupfile, "r");
	if(!yamlfile)
	{
		rt_kprintf("Couldn't read map file from the YAML file: %s", mapmarkupfile);
		return -1;
	}
	fscanf(yamlfile, "image: %s\nresolution: %f\ncharge_st: [%f, %f]\nnegate: %d\noccupied_thresh: %f\nfree_thresh: %f\n", 
	mapdatafile, &(map->info.resolution), &(map->info.charge_st.position.x), &(map->info.charge_st.position.y), &g_negate, &g_occupied_thh, &g_free_thh);
	map->info.charge_st.orientation.x = 0.0; //initialize the orientation to zero degree.
	map->info.charge_st.orientation.y = 0.0;
	map->info.charge_st.orientation.z = 0.0;
	map->info.charge_st.orientation.w = 1.0;
	fclose(yamlfile);
	
	//read the PGM Header
	FILE *pgmfile = fopen(mapdatafile, "r");
	if(!pgmfile)
	{
		rt_kprintf("Couldn't read map file from the PGM file: %s", mapdatafile);
		return -1;
	}
		
	//read the PGM image header
	char magic[3];
	if((fscanf(pgmfile, "%10s \n", magic) != 1) || (strcmp(magic, "P5") != 0))
	{
		fprintf(stderr, "Incorrect image format, must be PGM(Binary encoded)");
		return -1;
	}
	
	//ignore the comments
  int ch;
	while((ch = fgetc(pgmfile)) == '#')
		while(fgetc(pgmfile) != '\n');
	    ungetc(ch, pgmfile);
  
	//read the image parameter
	if(fscanf(pgmfile, "%d %d\n255\n", &(map->info.width), &(map->info.height)) !=2)
	{
		fprintf(stderr, "Failed of read image dimensions");
		return -1;
	}
	
	//allocate space in the map
	map->data = calloc(map->info.width * map->info.height, sizeof(map->data[0]));
	
	//read the image in
	for(unsigned int y=0; y < map->info.height; y++) 
	{
	  for(unsigned int x=0; x < map->info.width; x++) 
		{
		  unsigned int i = x + (map->info.height-y-1)*map->info.width;
			ch = fgetc(pgmfile);
			if(0 == g_negate)
			{
				if(ch == 254)
					map->data[i] = 0;
				else if(ch == 000)
					map->data[i] = 1;
				else
				  map->data[i] = -1;
			}
			else
			{
				if(ch == 254)
					map->data[i] = 1;
				else if(ch == 000)
					map->data[i] = 0;
				else
				  map->data[i] = -1;
			}
		}
	}
	fclose(pgmfile);
	
	return 0;
}


// Save Map
int save_map(char *mapname)
{
	char *mapmarkupfile = strcat(mapname, ".yaml");
	char *mapdatafile = strcat(mapname, ".pgm");
	
	//write the YAML file
	FILE *yamlfile = fopen(mapmarkupfile, "w");
	if(!yamlfile)
	{
		rt_kprintf("Couldn't save map file to the YAML file: %s", mapmarkupfile);
		return -1;
	}
	fprintf(yamlfile, "image: %s\nresolution: %f\ncharge_st: [%f, %f]\nnegate: %d\noccupied_thresh: %f\nfree_thresh: %f\n", 
	mapdatafile, map->info.resolution, map->info.charge_st.position.x, map->info.charge_st.position.y, g_negate, g_occupied_thh, g_free_thh);
	fclose(yamlfile);
	
	//write the PGM header
	FILE *pgmfile = fopen(mapdatafile, "w");
	if(!pgmfile)
	{
		rt_kprintf("Couldn't save map file to the PGM file: %s", mapdatafile);
		return -1;
	}
	fprintf(pgmfile, "P5\n# CREATOR: map_server.cpp\n%d %d\n255\n",
	map->info.width, map->info.height);
	
	//write the map data in
	for(unsigned int y=0; y < map->info.height; y++) 
	{
	  for(unsigned int x=0; x < map->info.width; x++) 
		{
		  unsigned int i = x + (map->info.height-y-1)*map->info.width;
			if(0 == g_negate){
			  if(map->data[i] == 0) //free, occ[0,0.196)
          fputc(254, pgmfile);
				else if(map->data[i] == 1) //occupied, occ(0.65,1]
          fputc(000, pgmfile);
				else  //unknown, occ[0.1,0.65]
          fputc(205, pgmfile);
			}
			else
			{
				if(map->data[i] == 0) //free, occ[0,0.196)
          fputc(000, pgmfile);
				else if(map->data[i] == 1) //occupied, occ(0.65,1]
          fputc(254, pgmfile);
				else  //unknown, occ[0.1,0.65]
          fputc(205, pgmfile);
		  }
		}
	}
	fclose(pgmfile);
	
	return 0;
}
*/



/* Linked List Definition */
// Initialize(with head node)
void L_i_init(L_i_list *list_ptr)
{
	*list_ptr = (L_i_list)malloc(sizeof(L_i_node));
	if(*list_ptr == NULL)
		rt_kprintf("malloc fails, slist initializing fails");
	else
	{
		memset(*list_ptr, 0, sizeof(L_i_node));
		(*list_ptr)->next = NULL;
		rt_kprintf("malloc finished, slist initializing finished");
	}
}

// Return 1(true) if 'list' is empty
int L_i_is_empty(L_i_list list)
{
	return list->next == NULL;
}

// Return true if 'elem' is a member of 'list'
int L_i_is_member(L_i_list list, unsigned short elem)
{
	L_i_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		if(elem == cursor->element)
			return 1;
		cursor = cursor->next;
	}
	return 0; //refer to logical FALSE, not calling function SUCCESS.
}

// Insert after the head of list
void L_i_prepend(L_i_list list, unsigned short elem)
{
	L_i_cursor insert_ptr;
	
	insert_ptr = (L_i_cursor)malloc(sizeof(L_i_node));
	if(insert_ptr == NULL)
		rt_kprintf("malloc fails, slist initializing fails");
	else
	{
		memset(insert_ptr, 0, sizeof(L_i_node));
		insert_ptr->element = elem;
		insert_ptr->next = NULL;
	}
	
	insert_ptr->next = list->next;
	list->next = insert_ptr;
}

// Insert at the tail of the list
void L_i_append(L_i_list list, unsigned short elem)
{
	L_i_cursor cursor;
	L_i_cursor insert_ptr;
	
	insert_ptr = (L_i_cursor)malloc(sizeof(L_i_node));
	if(insert_ptr == NULL)
		rt_kprintf("malloc fails, slist initializing fails");
	else
	{
		memset(insert_ptr, 0, sizeof(L_i_node));
		insert_ptr->element = elem;
		insert_ptr->next = NULL;
	}
	
	cursor = list;
	while(cursor->next != NULL)   //a trick here, cannot be "cursor != NULL"
		cursor = cursor->next;
	
	cursor->next = insert_ptr;
	
	return;
}

// Retrieve the NO.idx element of the list
unsigned short L_i_retrieve(L_i_list list, int idx)
{
	int n = 0;
	int length;
	L_i_cursor cursor = list->next;
	
	length = L_i_get_length(list);
	while(idx < 0)
		idx += length; 
	while(idx >= length)
		idx -= length;
	
	while(cursor != NULL)
	{
		if(n++ == idx)
			return cursor->element;
		
		cursor = cursor->next;
	}
	return -1; //the desired element not found
}

// Set the designated element
int L_i_set_by_idx(L_i_list list, int idx, unsigned short elem)
{
	int n = 0;
	L_i_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		if(n++ == idx) 
		{
			cursor->element = elem;
			return 0;
		}
		cursor = cursor->next;
	}
	return -1; //the idx element not found
}

// Get the first minimum of the list
int L_i_get_idx_of_min(L_i_list list)
{
	int min = 0;
	int idx = 0;
	int n = 0;
	L_i_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		if((n == 0) || (cursor->element < min))
		{
			min = cursor->element;
			idx = n;
		}
		++n;
		cursor = cursor->next;
	}
	
	return idx;
}

// Get the length of the list
int L_i_get_length(L_i_list list)
{
	int length = 0;
	L_i_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		++length;
		cursor = cursor->next;
	}
	
	return length;
}

// Insert element at idx slot
void L_i_insert_by_idx(L_i_list list, int idx, unsigned short elem)
{
	int n = 0;
	L_i_cursor cursor;
	L_i_cursor insert_ptr;
	
	insert_ptr = (L_i_cursor)malloc(sizeof(L_i_node));
	if(insert_ptr == NULL)
		rt_kprintf("malloc fails, slist initializing fails");
	else
	{
		memset(insert_ptr, 0, sizeof(L_i_node));
		insert_ptr->element = elem;
		insert_ptr->next = NULL;
	}
	
	cursor = list;
	while(cursor->next != NULL)
	{
		if(n++ == idx)
		{
			insert_ptr->next = cursor->next; 
			cursor->next = insert_ptr; 
			return;
		}
		cursor = cursor->next; 
	}
	
	if(n == idx)
		cursor->next = insert_ptr; 
	
	return;
}

// Delete the idx element of the list
void L_i_delete_by_idx(L_i_list list, int idx)
{
	int n = 0;
	L_i_cursor pre_cursor = list;
	L_i_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		if(n++ == idx)
		{
			pre_cursor->next = cursor->next;
			free(cursor);
			cursor = pre_cursor->next;
			return;
		}
		pre_cursor = cursor;
		cursor = cursor->next;
	}
	//NOTICE: no return, must confirm 'elem' is a member 
	//of 'list' before calling this funciton.
	
	return;
}

// Remove all nodes BUT the head node
void L_i_delete_all_but_header(L_i_list list)
{
	L_i_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		list->next = cursor->next;
		free(cursor);
		cursor = list->next;
	}
	
	return;
}


/* Adjacency List Definition*/
// Initialize a graph(with head node)
/*void A_init(A_graph *graph_ptr)
{
	int i;
	
	graph_ptr->vertices = calloc(MAX_CELL_NUM, sizeof(L_i_node));
	if(graph_ptr->vertices == NULL)
		rt_kprintf("calloc fails, graph initializing fails!");
	else
	{
		for(i = 0; i < MAX_CELL_NUM; i++)
		{
			graph_ptr->vertices[i].element = (unsigned short)0;
			graph_ptr->vertices[i].next = NULL;			
			rt_kprintf("calloc finished, graph initializing finished!");
		}
	}
	graph_ptr->vexnum = 0;
	
	return;
}*/


// Initialize a graph(with head node)
void PL_i_init(PL_i_list *PL_list_ptr, int nitems)
{
	int i;
	
	*PL_list_ptr = calloc(nitems, sizeof(L_i_node));
	if(*PL_list_ptr == NULL)
		rt_kprintf("calloc fails, PL_list initializing fails!");
	else
	{
		for(i = 0; i < nitems; i++)
		{
			(*PL_list_ptr)[i].element = (unsigned short)0;
			(*PL_list_ptr)[i].next = NULL; 
			rt_kprintf("calloc finished, p_list initializing finished!");
		}
	}
	
	return;
}

// Delete a graph
void PL_i_clear(PL_i_list PL_list, int nitems)
{
	int i;
	for(i = 0; i < nitems; i++)
	{
		L_i_delete_all_but_header(PL_list + i);
		PL_list[i].element = (unsigned short)0;
	}
	
	//free(*PL_list_ptr);
	//### NOTICE: REMEMBER to set the "g_adj_vexnum" to zero by hand.
	
	return;
}

// Insert a new node into graph
void PL_i_insert(PL_i_list graph, int p_node, int c_node)
{
	if(-1 == p_node)
	{
		if(-1 == c_node) 	//CELL_CEASE
			return; 
		else 							//CELL_ORIGINATE
			(graph + c_node)->element = (unsigned short)c_node;
	}
	else 								//CELL_SPLIT | CELL_MERGE
	{
		L_i_append(graph + p_node, (unsigned short)c_node);
		if((graph + c_node)->element == 0) //it can be guaranteed c_node, as a child node No., will not be 0
			(graph + c_node)->element = (unsigned short)c_node;
		L_i_append(graph + c_node, (unsigned short)p_node);
	}
	
	return;
}


/* Linked List(point node) Definition */
// Initialize(with head node)
void L_p_init(L_p_list *list_ptr)
{
	*list_ptr = (L_p_list)malloc(sizeof(L_p_node));
	if(*list_ptr == NULL)
		rt_kprintf("malloc fails, slist initializing fails");
	else
	{
		memset(*list_ptr, 0, sizeof(L_p_node)); //a struct in element, can also be fill zeros like this?
		(*list_ptr)->next = NULL;
		rt_kprintf("malloc finished, slist initializing finished");
	}
}

// Return 1(true) if 'list' is empty
int L_p_is_empty(L_p_list list)
{
	return list->next == NULL;
}

// Insert after the head of list
void L_p_prepend(L_p_list list, T_pose elem)
{
	L_p_cursor insert_ptr;
	
	insert_ptr = (L_p_cursor)malloc(sizeof(L_p_node));
	if(insert_ptr == NULL)
		rt_kprintf("malloc fails, slist initializing fails");
	else
	{
		memset(insert_ptr, 0, sizeof(L_p_node));
		insert_ptr->element = elem;
		insert_ptr->next = NULL;
	}
	
	insert_ptr->next = list->next;
	list->next = insert_ptr;
}

// Insert at the tail of the list
void L_p_append(L_p_list list, T_pose elem)
{
	L_p_cursor cursor;
	L_p_cursor insert_ptr;
	
	insert_ptr = (L_p_cursor)malloc(sizeof(L_p_node));
	if(insert_ptr == NULL)
		rt_kprintf("malloc fails, slist initializing fails");
	else
	{
		memset(insert_ptr, 0, sizeof(L_p_node));
		insert_ptr->element = elem;
		insert_ptr->next = NULL;
	}
	
	cursor = list;
	while(cursor->next != NULL)   //a trick here, cannot be "cursor != NULL"
		cursor = cursor->next;
	
	cursor->next = insert_ptr;
	
	return;
}

// Retrieve the NO.idx element of the list
T_pose L_p_retrieve(L_p_list list, int idx)
{
	int n = 0;
	int length;
	L_p_cursor cursor = list->next;
	T_pose pose0;
	
	length = L_p_get_length(list);
	while(idx < 0)
		idx += length; 
	while(idx >= length)
		idx -= length; 
	
	while(cursor != NULL)
	{
		if(n++ == idx)
			return cursor->element;
		
		cursor = cursor->next;
	}
	
	pose0.pos.x = 0.0;
	pose0.pos.y = 0.0;
	pose0.th = 0.0;
	return pose0; 	//the desired element not found
}

// Set the designated element
int L_p_set_by_idx(L_p_list list, int idx, T_pose elem)
{
	int n = 0;
	L_p_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		if(n++ == idx) 
		{
			cursor->element = elem;
			return 0;
		}
		cursor = cursor->next;
	}
	return -1; //the idx element not found
}

// Get the length of the list
int L_p_get_length(L_p_list list)
{
	int length = 0;
	L_p_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		++length;
		cursor = cursor->next;
	}
	
	return length;
}

// Delete the idx element of the list
void L_p_delete_by_idx(L_p_list list, int idx)
{
	int n = 0;
	L_p_cursor pre_cursor = list;
	L_p_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		if(n++ == idx)
		{
			pre_cursor->next = cursor->next;
			free(cursor);
			cursor = pre_cursor->next;
			return;
		}
		pre_cursor = cursor;
		cursor = cursor->next;
	}
	//NOTICE: no return, must confirm 'elem' is a member 
	//of 'list' before calling this funciton.
	
	return;
}

// Remove all nodes BUT the head node
void L_p_delete_all_but_header(L_p_list list)
{
	L_p_cursor cursor = list->next;
	
	while(cursor != NULL)
	{
		list->next = cursor->next;
		free(cursor);
		cursor = list->next;
	}
	
	return;
}


/* Plural List Definition */
// Plural list initialization
void PL_p_init(PL_p_list *PL_list_ptr, int nitems)
{
	int i;
	T_pose pose0;
	
	pose0.pos.x = 0.0;
	pose0.pos.y = 0.0;
	pose0.th = 0.0;
	*PL_list_ptr = calloc(nitems, sizeof(L_p_node));
	if(*PL_list_ptr == NULL)
		rt_kprintf("calloc fails, PL_list initializing fails!");
	else
	{
		for(i = 0; i < nitems; i++)
		{
			(*PL_list_ptr)[i].element = pose0;
			(*PL_list_ptr)[i].next = NULL; 
			rt_kprintf("calloc finished, p_list initializing finished!");
		}
	}
	
	return;
}

// Delete the plural list
void PL_p_clear(PL_p_list PL_list, int nitems)
{
	int i;
	for(i = 0; i < nitems; i++)
		L_p_delete_all_but_header(PL_list + i);
	
	//free(*PL_list_ptr);
	//### NOTICE: REMEMBER to SET the "g_wire_srndd_landscape_cnt" and "g_rigid_landscape_cnt" by hand
	
	return;
}


