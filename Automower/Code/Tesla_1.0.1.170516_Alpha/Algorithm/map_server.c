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
T_pose_m g_pose_on_map;
T_point_m g_tr_map_in_lawngrid; 

int g_map_color_negate = 0; 
float g_map_occ_thh = 0.65; 
float g_map_free_thh = 0.196; 
float g_map_resol = 0.25; 
float g_max_lawn_width = 30; 
float g_max_lawn_height = 30; 


/* Functions Definition */

// Initialize
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
	set_all_grids_zero(); 
	
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


// Set all grids to zero when remapping starts
int set_all_grids_zero()
{	
	int map_length_of_bytes = g_map.info.height * g_map.info.width / (BYTE_BITS/OCC_BITS);
	for(int ndx = 0; ndx < map_length_of_bytes; ndx++)
		g_map.data[ndx] = OCC_UNKNOWN; 
	
	return 0;
}


// Write the state(0,1,2,3) to the objective 2bit in the memory space
int set_grid_value(int ndx, int value)
{
	char obj_byte = g_map.data[ndx/(BYTE_BITS/OCC_BITS)];
	int pos_in_byte = 2*((BYTE_BITS/OCC_BITS) - ndx%(BYTE_BITS/OCC_BITS) - 1) + 1;
	set_bits(&obj_byte, pos_in_byte, OCC_BITS, value);
	g_map.data[ndx/(BYTE_BITS/OCC_BITS)] = obj_byte;
	
	return 0;
}


// Read the state of grid out
int get_grid_value(int ndx)
{
	char obj_byte = g_map.data[ndx/(BYTE_BITS/OCC_BITS)];
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
	
	//check out the left limit
  for(int x = 0; x < g_map.info.width; x+=4)
	{
		for(int y = 0; y < g_map.info.height; y++)
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
  for(int x = g_map.info.width-1; x >= 0; x-=4)
	{
		for(int y = 0; y < g_map.info.height; y++)
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
  for(int y = 0; y < g_map.info.height; y++)
	{
		for(int x = 0; x < g_map.info.width; x+=4)
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
  for(int y = g_map.info.height-1; y >=0 ; y--)
	{
		for(int x = 0; x < g_map.info.width; x+=4)
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
	for(int y = temp_height-1; y >= 0; y++)
	{
		for(int x = 0; x < temp_width; x+=4)
		{
			int temp_ndx = sub2ind(temp_width, temp_height, x, y);
			int pri_ndx = sub2ind(g_map.info.width, g_map.info.height, x+limit_left, y+limit_down);
			temp_data[temp_ndx/(BYTE_BITS/OCC_BITS)] = g_map.data[pri_ndx/(BYTE_BITS/OCC_BITS)];
		}
	}
	
	free(g_map.data);
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
	
}

/*
// Cell decomposition
int cell_decomp(T_adj_list graph_out)
{
	
}
*/

// Bresenham's raytracing algorithm
int ray_tracing()
{
	
}


// Send map data to PC
/* here or not? */
int map_send()
{
	
}


// Load map
int map_load()
{
}


// Save map
int map_save()
{
}




// ##### Linked List ######
struct L_node
{
	int element;
	L_cursor next;
};

// Return true if 'list' is empty
int L_is_empty(L_list list)
{
	return list->next == NULL;
}

// Return true if 'elem' is a member of 'list'
int L_is_member(int elem, L_list list)
{
	
}

// Insert (after legal position 'curs')
void L_insert_tail(int elem, L_list list)
{
	L_cursor temp = malloc(sizeof(struct L_node));
	
	if(temp == NULL)
		printf("Out of space!");
	
	/*temp->element = elem;
	temp->next = curs->next;
	curs->next = temp;*/
}

// Delete first occurrence of 'elem' from a list
void L_delete(int elem, L_list list)
{
	L_cursor curs, temp;
	
	/*curs = L_find_previous(elem, list);
	
	if(!L_is_last(curs, list))
	{
		temp = curs->next;
		curs->next = temp->next;
		free(temp);
	}*/
}

// Get the minimum of the list
int L_get_min(L_list list)
{
	
}

// ########################





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
		printf("Couldn't read map file from the YAML file: %s", mapmarkupfile);
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
		printf("Couldn't read map file from the PGM file: %s", mapdatafile);
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
		printf("Couldn't save map file to the YAML file: %s", mapmarkupfile);
		return -1;
	}
	fprintf(yamlfile, "image: %s\nresolution: %f\ncharge_st: [%f, %f]\nnegate: %d\noccupied_thresh: %f\nfree_thresh: %f\n", 
	mapdatafile, map->info.resolution, map->info.charge_st.position.x, map->info.charge_st.position.y, g_negate, g_occupied_thh, g_free_thh);
	fclose(yamlfile);
	
	//write the PGM header
	FILE *pgmfile = fopen(mapdatafile, "w");
	if(!pgmfile)
	{
		printf("Couldn't save map file to the PGM file: %s", mapdatafile);
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
