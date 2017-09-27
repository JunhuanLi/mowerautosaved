/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: pi.h
  Author		: George 	
  Version		: V1.0.0	  
  Date			: 2017/07/21
  Description	: pi 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/

/* includes *******************************************************************/
#include "motion_control.h"
#include "motor_control.h"
#include "motion_math.h"
#include "bumper_front.h"

#include "usart_driver.h"
#include "motion_imu.h"

/* macros *********************************************************************/

/*vehicle info*/
float Rl = 0.0915; //meter
float Rr = 0.0915; //meter
float l = 0.35; //meter
float omega_max = 6.98; //maximum angluar velocity 3000/45(rpm) == 6.98(rad/s)

/*Temp Mag**********************************************************************/
/*Mag Sensor*/

MAG_STATUE mag_state;
void mag_sensor_initial(void)
{
	mag_state.left_sensor_former = 0;
	mag_state.right_sensor_former = 0;
	mag_state.left_sensor_now = 0;
	mag_state.right_sensor_now = 0;
	mag_state.left_sensor_change = 0;
	mag_state.right_sensor_change = 0;
	
	mag_state.left_sensor_old = 0;
	mag_state.right_sensor_old = 0;
	
}
void mag_sensor_update(void)
{
	if((UART_RX_BUFFER[4] == 0x01)&&(UART_RX_BUFFER[0] == 0xAA)&&((UART_RX_BUFFER[1] == 0xBB)))
	{
		mag_state.left_sensor_now = MOTION_MAG_LINE_INSIDE;
	}
	else if((UART_RX_BUFFER[4] == 0x02)&&(UART_RX_BUFFER[0] == 0xAA)&&((UART_RX_BUFFER[1] == 0xBB)))
	{
		mag_state.left_sensor_now = MOTION_MAG_LINE_OUTSIDE;
	}
	else
	{
		mag_state.left_sensor_now = MOTION_MAG_LINE_MISSING;
	}
	
	if((UART_RX_BUFFER[5] == 0x01)&&(UART_RX_BUFFER[0] == 0xAA)&&((UART_RX_BUFFER[1] == 0xBB)))
	{
		mag_state.right_sensor_now = MOTION_MAG_LINE_INSIDE;
	}
	else if((UART_RX_BUFFER[5] == 0x02)&&(UART_RX_BUFFER[0] == 0xAA)&&((UART_RX_BUFFER[1] == 0xBB)))
	{
		mag_state.right_sensor_now = MOTION_MAG_LINE_OUTSIDE;
	}
	else
	{
		mag_state.right_sensor_now = MOTION_MAG_LINE_MISSING;
	}
	/*
	if((mag_state.right_sensor_former != mag_state.right_sensor_now)&&(mag_state.left_sensor_former != MOTION_MAG_LINE_MISSING)&&(mag_state.left_sensor_now != MOTION_MAG_LINE_MISSING))
	{
		mag_state.right_sensor_change =	1;
	}
	if((mag_state.left_sensor_former != mag_state.left_sensor_now)&&(mag_state.left_sensor_former != MOTION_MAG_LINE_MISSING)&&(mag_state.left_sensor_now != MOTION_MAG_LINE_MISSING))
	{
		mag_state.left_sensor_change =	1;
	}
	*/
	
	if((mag_state.right_sensor_former == MOTION_MAG_LINE_OUTSIDE)&&(mag_state.right_sensor_former == MOTION_MAG_LINE_OUTSIDE)&&(mag_state.right_sensor_old == MOTION_MAG_LINE_OUTSIDE))
	{
		mag_state.right_sensor_change = 1;
	}
	if((mag_state.left_sensor_former == MOTION_MAG_LINE_OUTSIDE)&&(mag_state.left_sensor_former == MOTION_MAG_LINE_OUTSIDE)&&(mag_state.left_sensor_old == MOTION_MAG_LINE_OUTSIDE))
	{
		mag_state.left_sensor_change = 1;
	}
	
	mag_state.right_sensor_old = mag_state.right_sensor_former;
	mag_state.left_sensor_old = mag_state.left_sensor_former;
	mag_state.right_sensor_former = mag_state.right_sensor_now;
	mag_state.left_sensor_former = mag_state.left_sensor_now;
}
/*Temp end*********************************************************************/


/* static variables ***********************************************************/
static __inline void Motion_Norm_2D(float* x, float* y)
{
	float length = sqrtf((*x)*(*x) + (*y)*(*y));
	if(length != 0)
	{
		*x = *x / length;
		*y = *y / length;
	}
}
unsigned short zigzag_flag = 0;
static void Motion_Run_Zigzag(T_motion* motion)
{
	zigzag_flag++;
	if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_IDLE)
	{
		rt_kprintf("Waiting orders! \n");
		return;
	}
	else if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_LINE)
	{
		//运行位姿控制器
		//Motion_Update_2D_Line(&motion->tracker, TEST_LEN, 0.0f, direction[0].x, direction[0].y, 0.2f);
		Motion_Run_Tracker(&motion->tracker);
		
		mag_sensor_update();
		if((mag_state.left_sensor_change == 1)&&(mag_state.right_sensor_change == 1)&&(zigzag_flag > 100))
		{
			motion->zigzag.state = T_MOTION_ZIGZAG_STATE_TURN;
			
			mag_state.right_sensor_change = 0;
			mag_state.left_sensor_change = 0;
			
			zigzag_flag = 0;
		}
		//判断是否碰撞
		/*
		if(motion->tracker.sense.bump_l || motion->tracker.sense.bump_r)
		{
			motion->zigzag.state = T_MOTION_ZIGZAG_STATE_TURN;
		}
		
		//判断是否跨线
		if(motion->tracker.sense.side_l == MOTION_MAG_LINE_OUTSIDE 
			&& motion->tracker.sense.side_r == MOTION_MAG_LINE_OUTSIDE)
		{
			motion->zigzag.state = T_MOTION_ZIGZAG_STATE_TURN;
		}
		*/
		
	}
	//掉头部分
	else if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_TURN)
	{
		
		//float dot_product = (motion->tracker.sense.dir_x*motion->zigzag.heading_x)+(motion->tracker.sense.dir_y*motion->zigzag.heading_y);
		float dot_product = (motion->tracker.sense.dir_x* motion->zigzag.heading_x)+(motion->tracker.sense.dir_y* motion->zigzag.heading_y);
		float k = (1- motion->zigzag.blade_bodywidth_ratio * motion->zigzag.blade_overlaping_ratio) / 2;
		
		zigzag_flag = 0;
		//计算线速度和角速度
		if(motion->zigzag.turn_dir == T_MOTION_ZIGZAG_TURN_COUNTERCLOCKWISE)
		{
			motion->tracker.line_vel = motion->zigzag.target_vel;
			motion->tracker.angular_vel = motion->zigzag.target_vel/(1-k);
			//motion->tracker.line_vel = 0;
		}
		else
		{
			motion->tracker.line_vel = motion->zigzag.target_vel;
			motion->tracker.angular_vel = -motion->zigzag.target_vel/(1-k);
			//motion->tracker.line_vel = 0;
		}
		
		//利用点乘判断是否完成掉头 之后计算下一阶段参数

		/*if(motion->zigzag.f_r == T_MOTION_ZIGZAG_GO_FOWARD)
		{
			if(dot_product < -0.96f) //~170degree
			{
				//motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
				motion->tracker.angular_vel = 0;
				motion->tracker.line_vel = 0;
			}
			//Tracking_Start_2D_Angle(&motion->tracker,-motion->zigzag.heading_x,-motion->zigzag.heading_y,motion->zigzag.target_vel);
		}
		else
		{
			if(dot_product > 0.96f)
			{
				//motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
				motion->tracker.angular_vel = 0;
				motion->tracker.line_vel = 0;
			}
			//Tracking_Start_2D_Angle(&motion->tracker,motion->zigzag.heading_x,motion->zigzag.heading_y,motion->zigzag.target_vel);
		}*/

		if( (dot_product) < -0.96f)  //~170degree
			{
				//turn finished, need to toggle the next turn direction
				/*
				motion->zigzag.turn_dir += 1;
				if(motion->zigzag.turn_dir>2)
					motion->zigzag.turn_dir=1;
				*/

				motion->zigzag.state = T_MOTION_ZIGZAG_STATE_LINE;
				motion->tracker.angular_vel = 0;
				motion->tracker.line_vel = 0;
				
				
				if(motion->zigzag.turn_dir == T_MOTION_ZIGZAG_TURN_COUNTERCLOCKWISE) motion->zigzag.turn_dir = T_MOTION_ZIGZAG_TURN_CLOCKWISE;
				if(motion->zigzag.turn_dir == T_MOTION_ZIGZAG_TURN_CLOCKWISE) motion->zigzag.turn_dir = T_MOTION_ZIGZAG_TURN_COUNTERCLOCKWISE;
				motion->zigzag.heading_x = -motion->zigzag.heading_x;
				Motion_Update_2D_Line(&motion->tracker,(motion->zigzag.heading_x) * 200, 0,(motion->zigzag.heading_x),0, 0.1f);

			}
			
			//Tracking_Start_2D_Angle(&motion->tracker,motion->zigzag.heading_x,motion->zigzag.heading_y,motion->zigzag.target_vel);
		
		
	}
	else if(motion->zigzag.state == T_MOTION_ZIGZAG_STATE_EXCEPTION)
	{
		
	}

}
/* funcitons ******************************************************************/
void Motion_Init(T_motion* motion,uint8_t en)
{
	set_motor_control_type(MOTOR_CONTROL_TYPE_SPEED);
	
	motion->motion_state = 											MOTION_STATE_IDLE;
	motion->exception = 												MOTION_EXCEPTION_NONE;
	motion->enable = 														en;
	
	motion->tracker.tracking = 									MOTION_TRACKING_2D_LINE;
	motion->tracker.acc = 											MOTION_ACC;
	motion->tracker.omega_leftWheel = 					0;
	motion->tracker.omega_rightWheel	= 				0;
	
	//obj->error = TRACKING_ERROR_NONE;
	//obj->sensor.mag_polarity = 1;
	
	Motion_Set_Path_Param(&motion->tracker,0.4);
	Motion_Set_cornerAngle_Param(&motion->tracker,0.015,0.001f,0.1);
	Motion_Set_lineAngle_Param(&motion->tracker,0.015, 0.001f,0.1);
	Motion_Set_Mag_Tracking_Param(&motion->tracker,0,0,0);
	Motion_Set_Mag_Gotoline_Param(&motion->tracker,0,0,0);
	Motion_Update_2D_Line(&motion->tracker,1,0,1,0,0.1);
	
//	Motion_Set_Mag_Tracking_Param(&motion->tracker,0,0,0);
//	Motion_Set_Mag_Gotoline_Param(&motion->tracker,0,0,0);
//	Motion_Update_2D_Angle(&motion->tracker,1,0,0);
	Motion_Zigzag_Init(motion,0.8,0.6);
}

void Motion_Run(T_motion* motion)
{
	if(motion->motion_state == MOTION_STATE_ZIGZAG)
	{
		Motion_Run_Zigzag(motion);
	}
	

}

/*
void Motion_Process_Obstacle(T_motion* motion)
{
	//Run Obsticle
	float vel_sonar = motion->tracker.command_vel;
	float vel_mag_line = motion->tracker.command_vel;
	uint16_t sonar = motion->tracker.sense.sonar_l > motion->tracker.sense.sonar_r ? motion->tracker.sense.sonar_l : motion->tracker.sense.sonar_r;
	
	if(sonar > DECELERATION_SONAR_MIN)
	{
		vel_sonar = (float)(1 - (sonar - DECELERATION_SONAR_MIN))/(float)(DECELERATION_SONAR_MAX - DECELERATION_SONAR_MIN) * (float)(1 -DECELERATION_MIN_SPEED)
		           + (float)DECELERATION_MIN_SPEED;
		vel_sonar = vel_sonar * motion->tracker.command_vel;
	}
	
	motion->tracker.target_vel = vel_sonar < vel_mag_line ? vel_sonar : vel_mag_line;
}
*/

void Motion_Process_Motor_Speed(T_motion* motion)
{
	
	float omega_leftWheel =  1/Rl * motion->tracker.line_vel + l/(2*Rl) * motion->tracker.angular_vel;
	float omega_rightWheel = 1/Rr * motion->tracker.line_vel - l/(2*Rr) * motion->tracker.angular_vel;
	
	//Run Accellation
	if(motion->tracker.omega_leftWheel < omega_leftWheel)
	{
		motion->tracker.omega_leftWheel += motion->tracker.acc;
		if(motion->tracker.omega_leftWheel > omega_leftWheel)
		{
			motion->tracker.omega_leftWheel = omega_leftWheel;
		}
	}
	else if(motion->tracker.omega_leftWheel > omega_leftWheel)
	{
		motion->tracker.omega_leftWheel -= motion->tracker.acc;
		if(motion->tracker.omega_leftWheel < omega_leftWheel)
		{
			motion->tracker.omega_leftWheel = omega_leftWheel;
		}
	}
	
	if(motion->tracker.omega_rightWheel < omega_rightWheel)
	{
		motion->tracker.omega_rightWheel += motion->tracker.acc;
		if(motion->tracker.omega_rightWheel > omega_rightWheel)
		{
			motion->tracker.omega_rightWheel = omega_rightWheel;
		}
	}
	else if(motion->tracker.omega_rightWheel > omega_rightWheel)
	{
		motion->tracker.omega_rightWheel -= motion->tracker.acc;
		if(motion->tracker.omega_rightWheel < omega_rightWheel)
		{
			motion->tracker.omega_rightWheel = omega_rightWheel;
		}
	}
	
		
	float omega_dr_leftWheel = 3600 * motion->tracker.omega_leftWheel/omega_max; // dr == dutyratio
	float omega_dr_rightWheel = 3600 * motion->tracker.omega_rightWheel/omega_max;
	//Set Speed
	set_motor_control_speed_s32(omega_dr_leftWheel,omega_dr_rightWheel);
	//set_motor_control_speed_s32(vl,vr);
}


void Motion_Zigzag_Init(T_motion* motion,float blade_bodywidth_ratio,float blade_overlaping_ratio)
{
	motion->zigzag.blade_bodywidth_ratio =   		blade_bodywidth_ratio;
	motion->zigzag.blade_overlaping_ratio =  		blade_overlaping_ratio;
	motion->zigzag.state = 									    T_MOTION_ZIGZAG_STATE_IDLE;
	motion->zigzag.f_r = 												T_MOTION_ZIGZAG_GO_FOWARD;
	motion->zigzag.state = 											T_MOTION_ZIGZAG_STATE_LINE;
}

void Motion_Zigzag_Update(T_motion* motion,float speed,float heading_x,float heading_y,T_motion_zigzag_turn_dir_type turn_dir)
{
	float x = heading_x,y = heading_y;

	Motion_Norm_2D(&x,&y);
	
	motion->zigzag.heading_x = 							x;
	motion->zigzag.heading_y = 							y;
	motion->zigzag.turn_dir = 							turn_dir;
	motion->zigzag.f_r = 										T_MOTION_ZIGZAG_GO_FOWARD;
	motion->zigzag.target_vel = 						speed;
	motion->zigzag.state = 									T_MOTION_ZIGZAG_STATE_LINE;
	motion->motion_state = 									MOTION_STATE_ZIGZAG;
	
	Motion_Update_2D_Angle(&motion->tracker,motion->zigzag.heading_x,motion->zigzag.heading_y,motion->zigzag.target_vel);
}
