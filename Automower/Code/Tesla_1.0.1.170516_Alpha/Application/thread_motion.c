
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:		thread_motion.c
  Author:			Raymond
  Date:				2017.7.7
  Version:        
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:			2017.9.3
       Author:		Nate
       Modification:zigzag for map planner test
    2. ...
*************************************************/


#include "stm32f4xx.h"
#include <rtthread.h>
//#include <string.h>
#include <stdio.h>
#include "mower_common.h"
#include "motion_control.h"
#include "global.h"
#include "motor_control.h"

#include "usart_driver.h"
#include "lcd12864_io_spi.h"
#include "bumper_front.h"

#include "global_planner.h"

/*********Temp Functions******************/
static void mower_motion_square(T_motion* motion,float speed, uint32_t side_length)
{
	uint8_t state = 0;
	uint32_t count = 0;
	
	rt_uint32_t recved;
	
	Motion_Update_2D_Angle(&motion->tracker,1.0f,0.0f,speed);
	while(1)
	{
		rt_event_recv(&sys_event, SYS_EVN_MOTION, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);

		Motion_Get_Position_2D(&motion->tracker.sense);
		Motion_Get_Sensor(&motion->tracker.sense);
		
		if(state == 0 || state == 2 || state == 4 || state == 6)
		{
			if(count >= side_length)
			{
				state ++;
				count = 0;
			}
			count++;
			Motion_Run_Tracker(&motion->tracker);
		}
		else if(state == 1)
		{
			motion->tracker.line_vel = 0;
			motion->tracker.angular_vel = speed;
			
			if(motion->tracker.sense.dir_y < -0.96f)
			{
				Motion_Update_2D_Angle(&motion->tracker,0.0f,-1.0f,speed);
				state ++;
			}
		}
		else if(state == 3)
		{
			motion->tracker.line_vel = 0;
			motion->tracker.angular_vel = speed;
			
			if(motion->tracker.sense.dir_x < -0.96f)
			{
				Motion_Update_2D_Angle(&motion->tracker,-1.0f,0.0f,speed);
				state ++;
			}
		}
		else if(state == 5)
		{
			motion->tracker.line_vel = 0;
			motion->tracker.angular_vel = speed;
			
			if(motion->tracker.sense.dir_y > 0.96f)
			{
				Motion_Update_2D_Angle(&motion->tracker,0.0f,1.0f,speed);
				state ++;
			}
		}
		else if(state == 7)
		{
			motion->tracker.line_vel = 0;
			motion->tracker.angular_vel = speed;
			
			if(motion->tracker.sense.dir_x > 0.96f)
			{
				Motion_Update_2D_Angle(&motion->tracker,1.0f,0.0f,speed);
				state = 0;
			}
		}
		Motion_Process_Motor_Speed(motion);
		update_motor_control();
	}
}
const float DIR_POS_OFFSET = 0.96f;
//const float DIR_NEG_OFFSET = -DIR_POS_OFFSET;
const float DIR_NEG_OFFSET = 0.007f;




static void mower_motion_circle(T_motion* motion,float line_speed, float angle_speed)
{
	float current_angle = 0;
	float tx = 1.0f;
	float ty = 0.0f;
	rt_uint32_t recved;
	uint8_t buf[100] = "hello!!!!!!!!!\n\r";
	while(1)
	{
		volatile float sin = sinf(current_angle);
		volatile float cos = cosf(current_angle);
		volatile float x = tx;
		volatile float y = ty;
		volatile float tx = x*cos-y*sin;
		volatile float ty = x*sin+y*cos;
		
		rt_event_recv(&sys_event, SYS_EVN_MOTION, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);
		
		//rt_debug(buf,16);
		Motion_Get_Position_2D(&motion->tracker.sense);
		Motion_Get_Sensor(&motion->tracker.sense);
		Motion_Update_2D_Angle(&motion->tracker,tx,ty,line_speed);
		Motion_Run_Tracker(&motion->tracker);
		Motion_Process_Motor_Speed(motion);
		
		current_angle -= angle_speed;
		if(current_angle < 0)
		{
			current_angle += 2*3.1415926;
		}
		
		update_motor_control();
	}
}



/*在分配堆栈空间时，必须要对其*/
ALIGN(RT_ALIGN_SIZE)
char thread_motion_stack[1024];
struct rt_thread thread_motion;



T_frontBumper g_Bumper;
T_trigger g_trigger;
static void mower_motion_square_position(T_motion* motion,float speed, float side_length);
T_motion motion;

void mower_motion_thread(void* parameter)
{
  rt_uint32_t recved;
	
	
	mag_sensor_initial();
	
	rt_thread_delay(2000); //need be removed later 
	Motion_Init(&motion,1);

	/*Test Program*/
	//while(is_odo_ready==0);
	//mower_motion_square_position(&motion,0.1,1.0f);//4 meter
	//mower_motion_square(&motion,0.1,0.2);//10 sec
	//mower_motion_circle(&motion,0.1,0.0031416);//40 sec
	
//	Motion_Update_2D_Angle(&motion.tracker,1.0f,0.0f,0.1);
//	//Motion_Start_Mag_Line(&motion.tracker, 0.1, MOTION_MAG_LINE_DIRECT);
//	//Motion_Update_2D_Line(&motion.tracker, 5.0f, 0.0f, 1.0f, 0.0f, 0.2);
//	Motion_Zigzag_Update(&motion,0.1,1,0,T_MOTION_ZIGZAG_TURN_CLOCKWISE);
//	Motion_Zigzag_Init(&motion, 0.5, 0.5);
//	motion.motion_state = MOTION_STATE_ZIGZAG;
//	motion.zigzag.heading_x = 1.0f;
//	motion.zigzag.heading_y = 0.0f;
	while (1)
	{
		rt_event_recv(&sys_event, SYS_EVN_MOTION, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);
		//rt_enter_critical();	//	LCD_PWM = 0;
		
		/*
		//bumper issue
		get_front_bumper_info(&g_Bumper);
		if(g_Bumper.left == 0)
		{
			g_trigger = RIGID_TOUCHED;
		}
		//sensor issue
		
		mag_sensor_update();
		if((mag_state.right_sensor_change == 1)&&(mag_state.left_sensor_change == 1))
		{
			//g_trigger = WIRE_SENSED;
			motion.zigzag.state = T_MOTION_ZIGZAG_STATE_TURN;
			
			mag_state.right_sensor_change = 0;
			mag_state.left_sensor_change = 0;
		}
		*/
		//Motion_Run( &motion);
		//if(g_front_bumper.left)
		//Update all the sensors first
		Motion_Get_Position_2D(&motion.tracker.sense);
		Motion_Get_Sensor(&motion.tracker.sense);
		//Motion_Process_Obstacle(&motion);
		
		//Run controller
		
		//Motion_Run_Tracker(&motion.tracker);
//		if(motion.motion_state == MOTION_STATE_ZIGZAG)
//		{
//			Motion_Run(&motion);
//		}
//		else if(motion.motion_state == MOTION_STATE_MAGLINE)
//		{
//			Motion_Run_Mag_Line(&motion.tracker);
//		}
		//Motion_Run_Zigzag(&motion);
		
		//角度旋转测试程序
//		if(motion.tracker.path_imu.rotationFinished == FALSE)
//			rotateAngle(&motion.tracker, 90, MOTION_TURN_COUNTERCLOCKWISE);
//		else{
//			motion.tracker.line_vel = 0.1;
//			motion.tracker.angular_vel = 0;
//		}
		
			//沿直线测试程序
		trackPoint(&motion.tracker,100.0, 0.0);
		
		
		//Update Motor Command
		//motion.tracker.line_vel = 0.2;
		//motion.tracker.angular_vel = 0;
		Motion_Process_Motor_Speed(&motion);
		update_motor_control();
		//Debug
		//rt_kprintf("angle = %d                x = %d                 y = %d \n\r",(int)(eul_rad[0]*10*57.3), (int)(motion.tracker.sense.pos_x*100), (int)(motion.tracker.sense.pos_y*100));
		//rt_kprintf("left = %d, right =%d \n\r",(int)(motion.tracker.line_vel),(int)(motion.tracker.angular_vel));
		//rt_kprintf("left = %d, right = %d \r\n\r",leftsensor_data,rightsensor_data);
		
		
		//rt_event_send(&sys_event,SYS_EVN_MOTOR);
			//	LCD_PWM = 1;
			//	rt_exit_critical();
		}
}
float dist2;
static void mower_motion_square_position(T_motion* motion,float speed, float side_length)
{
	uint8_t state = 0;
	uint32_t count = 0;
	float point_x = 0;
	float point_y = 0;
	
	rt_uint32_t recved;
	
	point_x = side_length;
	point_y = 0.0f;
	Motion_Update_2D_Line(&motion->tracker,point_x,point_y,1.0f,0.0f,speed);

//	motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
//	rt_thread_delay(2000);
	
	while(1)
	{
		rt_event_recv(&sys_event, SYS_EVN_MOTION, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);

		/*
		mag_sensor_update();
		if((mag_state.right_sensor_change == 1)&&(mag_state.left_sensor_change == 1))
		{
			//g_trigger = WIRE_SENSED;
			motion->zigzag.state = T_MOTION_ZIGZAG_STATE_TURN;
			
			mag_state.right_sensor_change = 0;
			mag_state.left_sensor_change = 0;
		}
		*/
		
//		if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_TURN)
//		{
//			motion->tracker.line_vel = 0;
//			motion->tracker.angular_vel = speed;
//			//motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
//			
//			
//			Motion_Get_Position_2D(&motion->tracker.sense);
//			Motion_Get_Sensor(&motion->tracker.sense);
//			Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,0.0f,speed);
//			
//			if(motion->tracker.sense.dir_x < -0.96)
//			{
//				motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
//			}
//		}
//		
//		if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_LINE)
//		{
//			motion->tracker.line_vel = speed;
//			motion->tracker.angular_vel = 0;
//			Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,0.0f,speed);	
//		}

		Motion_Get_Position_2D(&motion->tracker.sense);
		Motion_Get_Sensor(&motion->tracker.sense);
		
		if(state == 0 || state == 2 || state == 4 || state == 6)
		{
			Motion_Run_2D_Line(&motion->tracker);
			dist2 = (point_x - motion->tracker.sense.pos_x) * (point_x - motion->tracker.sense.pos_x) + (point_y - motion->tracker.sense.pos_y) * (point_y - motion->tracker.sense.pos_y);
			
			if(dist2 < 0.0025f)  //dist < 0.05m = 5cm
				state ++;
		}
		else if(state == 1)
		{
			if(!motion->tracker.path_imu.rotationFinished)
				rotateAngle(&motion->tracker, 90, MOTION_TURN_COUNTERCLOCKWISE);
			else
			{
				point_x = side_length;
				point_y = -side_length;
				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,-1.0f,speed);	
				state ++;
			}
		}
		else if(state == 3)
		{
			if(!motion->tracker.path_imu.rotationFinished)
				rotateAngle(&motion->tracker, 90, MOTION_TURN_COUNTERCLOCKWISE);
			else
			{
				point_x = 0.0f;
				point_y = -side_length;
				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,-1.0f,0.0f,speed);
				state ++;
			}
		}
		else if(state == 5)
		{
			if(!motion->tracker.path_imu.rotationFinished)
				rotateAngle(&motion->tracker, 90, MOTION_TURN_COUNTERCLOCKWISE);
			else
			{
				point_x = 0.0f;
				point_y = 0.0f;
				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,1.0f,speed);
				state ++;
			}
		}
		else if(state == 7)
		{
			if(!motion->tracker.path_imu.rotationFinished)
				rotateAngle(&motion->tracker, 90, MOTION_TURN_COUNTERCLOCKWISE);
			else
			{
				point_x = side_length;
				point_y = 0.0f;
				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,1.0f,0.0f,speed);
				state = 0;
			}
		}
		
		/*Motor Speed update*/
		Motion_Process_Motor_Speed(motion);
		update_motor_control();
	}
}

/*Turn to mag*/

//static void mower_motion_square_position(T_motion* motion,float speed, float side_length)
//{
//	uint8_t state = 0;
//	uint32_t count = 0;
//	float point_x = 0;
//	float point_y = 0;
//	
//	rt_uint32_t recved;
//	
//	point_x = side_length;
//	point_y = 0.0f;
//	Motion_Update_2D_Line(&motion->tracker,point_x,point_y,1.0f,0.0f,speed);
//	Motion_Process_Motor_Speed(motion);
//	update_motor_control();
//	motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
//	rt_thread_delay(500);
//	while(1)
//	{
//		rt_event_recv(&sys_event, SYS_EVN_MOTION, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);

//		/*
//		mag_sensor_update();
//		if((mag_state.right_sensor_change == 1)&&(mag_state.left_sensor_change == 1))
//		{
//			//g_trigger = WIRE_SENSED;
//			motion->zigzag.state = T_MOTION_ZIGZAG_STATE_TURN;
//			
//			mag_state.right_sensor_change = 0;
//			mag_state.left_sensor_change = 0;
//		}
//		*/
//		
//		if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_TURN)
//		{
//			motion->tracker.line_vel = 0;
//			motion->tracker.angular_vel = speed;
//			//motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
//			
//			
//			Motion_Get_Position_2D(&motion->tracker.sense);
//			Motion_Get_Sensor(&motion->tracker.sense);
//			Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,0.0f,speed);
//			
//			if(motion->tracker.sense.dir_x < -0.96)
//			{
//				motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
//			}
//		}
//		
//		if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_LINE)
//		{
//			motion->tracker.line_vel = speed;
//			motion->tracker.angular_vel = 0;
//			Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,0.0f,speed);	
//		}
//		/*
//		Motion_Get_Position_2D(&motion->tracker.sense);
//		Motion_Get_Sensor(&motion->tracker.sense);
//		
//		if(state == 0 || state == 2 || state == 4 || state == 6)
//		{
//			float dist2 = (point_x - motion->tracker.sense.pos_x) * (point_x - motion->tracker.sense.pos_x) + (point_y - motion->tracker.sense.pos_y) * (point_y - motion->tracker.sense.pos_y);
//			if(dist2 < 0.0025f)  //dist < 0.05m = 5cm
//				state ++;

//			Motion_Run_Tracker(&motion->tracker);
//		}
//		else if(state == 1)
//		{
//			motion->tracker.line_vel = 0;
//			motion->tracker.angular_vel = speed;
//			
//			if(motion->tracker.sense.dir_y < DIR_NEG_OFFSET)
//			{
//				point_x = side_length;
//				point_y = -side_length;
//				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,-1.0f,speed);	
//				state ++;
//			}
//		}
//		else if(state == 3)
//		{
//			motion->tracker.line_vel = 0;
//			motion->tracker.angular_vel = speed;
//			
//			if(motion->tracker.sense.dir_x < DIR_NEG_OFFSET)
//			{
//				point_x = 0.0f;
//				point_y = -side_length;
//				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,-1.0f,0.0f,speed);
//				state ++;
//			}
//		}
//		else if(state == 5)
//		{
//			motion->tracker.line_vel = 0;
//			motion->tracker.angular_vel = speed;
//			
//			if(motion->tracker.sense.dir_y > DIR_POS_OFFSET)
//			{
//				point_x = 0.0f;
//				point_y = 0.0f;
//				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,0.0f,1.0f,speed);
//				state ++;
//			}
//		}
//		else if(state == 7)
//		{
//			motion->tracker.line_vel = 0;
//			motion->tracker.angular_vel = speed;
//			
//			if(motion->tracker.sense.dir_x > DIR_POS_OFFSET)
//			{
//				point_x = side_length;
//				point_y = 0.0f;
//				Motion_Update_2D_Line(&motion->tracker,point_x,point_y,1.0f,0.0f,speed);
//				state = 0;
//			}
//		}
//		*/
//		
//		/*Motor Speed update*/
//		Motion_Process_Motor_Speed(motion);
//		update_motor_control();
//	}
//}








