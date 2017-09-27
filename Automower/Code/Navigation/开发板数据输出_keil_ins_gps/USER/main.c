#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "sdram.h"
#include "usmart.h"
#include "gps.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
/************************************************
 ALIENTEK 阿波罗STM32F429开发板实验34
 MPU9250九轴传感器实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//串口1,发送缓存区
nmea_msg gpsx; 								            			//GPS信息
meas_msg gps_meas[5]; 

__align(4) u8 dtbuf[50];   								      //打印缓存器
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode字符串 
	 
//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
    while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)==RESET){}; 
    USART1->DR=c;  
} 

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0X01~0X1C
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u16*data,u8 len)
{
	u16 send_buf[70];
	u8 i;
	//if(len>45)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X41;	//帧头
	send_buf[1]=0X41;	//帧头
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}


//发送GPS数据
void send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw,short mx,short my,short mz)
{
	  u16 tbuf[63]; 
		tbuf[0] = (gpsx.latitude)>>24&0XFF;            //u32，纬度,分*100
		tbuf[1] = (gpsx.latitude)>>16&0XFF;            //u32,分*100
		tbuf[2] = (gpsx.latitude)>>8&0XFF;             //u32,分*100
		tbuf[3] = gpsx.latitude&0XFF;                  //u32,分*100
		tbuf[4] = (gpsx.longitude)>>24&0XFF;           //u32，经度,分*100
		tbuf[5] = (gpsx.longitude)>>16&0XFF;           //u32,分*100
		tbuf[6] = (gpsx.longitude)>>8&0XFF;            //u32,分*100
		tbuf[7] = gpsx.longitude&0XFF;                 //u32,分*100
		//tbuf[8] = gpsx.gpssta;                         //u8,GPS状态:0,未定位;1,非差分定位;2,差分定位;6,正在估算.				  
		//tbuf[9] = gpsx.posslnum;	                		 //u8,用于定位的GPS卫星数,0~12.
		tbuf[8] = gpsx.fixType;				               //u8,定位类型:1,没有定位;2,2D定位;3,3D定位
	  tbuf[9] = (gpsx.hMSL)>>24&0XFF;          
		tbuf[10] = (gpsx.hMSL)>>16&0XFF;           
		tbuf[11] = (gpsx.hMSL)>>8&0XFF;           
		tbuf[12] = gpsx.hMSL&0XFF;                
	  tbuf[13] = (gpsx.hAcc>>8)&0XFF;
		tbuf[14] = gpsx.hAcc&0XFF;
		tbuf[15] = (gpsx.vAcc>>8)&0XFF;
		tbuf[16] = gpsx.vAcc&0XFF;
		tbuf[17] = (gpsx.sAcc>>8)&0XFF;
		tbuf[18] = gpsx.sAcc&0XFF; 
		tbuf[19] = (gpsx.velN)>>24&0XFF;          
		tbuf[20] = (gpsx.velN)>>16&0XFF;           
		tbuf[21] = (gpsx.velN)>>8&0XFF;           
		tbuf[22] = gpsx.velN&0XFF;          
		tbuf[23] = (gpsx.velE)>>24&0XFF;          
		tbuf[24] = (gpsx.velE)>>16&0XFF;           
		tbuf[25] = (gpsx.velE)>>8&0XFF;           
		tbuf[26] = gpsx.velE&0XFF;          
		tbuf[27] = (gpsx.velD)>>24&0XFF;          
		tbuf[28] = (gpsx.velD)>>16&0XFF;           
		tbuf[29] = (gpsx.velD)>>8&0XFF;           
		tbuf[30] = gpsx.velD&0XFF;     
	
		tbuf[31] = (aacx>>8)&0XFF;
		tbuf[32] = aacx&0XFF;
		tbuf[33] = (aacy>>8)&0XFF;
		tbuf[34] = aacy&0XFF;
		tbuf[35] = (aacz>>8)&0XFF;
		tbuf[36] = aacz&0XFF; 
		tbuf[37] = (gyrox>>8)&0XFF;
		tbuf[38] = gyrox&0XFF;
		tbuf[39] = (gyroy>>8)&0XFF;
		tbuf[40] = gyroy&0XFF;
		tbuf[41] = (gyroz>>8)&0XFF;
		tbuf[42] = gyroz&0XFF;
		tbuf[43] = (mx>>8)&0XFF;//因为开启MPL后,无法直接读取磁力计数据,所以这里直接屏蔽掉.用0替代.
		tbuf[44] = mx&0XFF;
		tbuf[45] = (my>>8)&0XFF;;
		tbuf[46] = my&0XFF;
		tbuf[47] = (mz>>8)&0XFF;;
		tbuf[48] = mz&0XFF;
	
		tbuf[49] = (roll>>8)&0XFF;
		tbuf[50] = roll&0XFF;
		tbuf[51] = (pitch>>8)&0XFF;
		tbuf[52] = pitch&0XFF;
		tbuf[53] = (yaw>>8)&0XFF;
		tbuf[54] = yaw&0XFF;
		tbuf[55] = gpsx.posslnum;				               //u8,用于定位的卫星数目
		tbuf[56] = (gpsx.heading)>>24&0XFF;          
		tbuf[57] = (gpsx.heading)>>16&0XFF;           
		tbuf[58] = (gpsx.heading)>>8&0XFF;           
		tbuf[59] = gpsx.heading&0XFF;     
		tbuf[60] = (gpsx.headAcc>>8)&0XFF;
		tbuf[61] = gpsx.headAcc&0XFF; 

		usart1_niming_report(0X31,tbuf,63);            //数据帧,0X33
}


//通过串口1上报结算后的姿态数据给电脑(状态帧)
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
//csb:超声波高度,单位:cm
//prs:气压计高度,单位:mm
//void usart1_report_imu(short roll,short pitch,short yaw,short csb,int prs)
//{
//	u8 tbuf[12];   	
//	tbuf[0]=(roll>>8)&0XFF;
//	tbuf[1]=roll&0XFF;
//	tbuf[2]=(pitch>>8)&0XFF;
//	tbuf[3]=pitch&0XFF;
//	tbuf[4]=(yaw>>8)&0XFF;
//	tbuf[5]=yaw&0XFF;
//	tbuf[6]=(csb>>8)&0XFF;
//	tbuf[7]=csb&0XFF;
//	tbuf[8]=(prs>>24)&0XFF;
//	tbuf[9]=(prs>>16)&0XFF;
//	tbuf[10]=(prs>>8)&0XFF;
//	tbuf[11]=prs&0XFF;
//	usart1_niming_report(0X31,tbuf,12);//状态帧,0X01
//}  


//显示GPS定位信息 
void Gps_Msg_Show(void)
{
 	float tp;		   
	POINT_COLOR=BLUE;  	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f   ",tp/=100000);	//得到经度字符串
 	LCD_ShowString(30,340,200,16,16,dtbuf);	 	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f   ",tp/=100000);	//得到纬度字符串
 	LCD_ShowString(30,360,200,16,16,dtbuf);	 	 
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//得到高度字符串
 	LCD_ShowString(30,380,200,16,16,dtbuf);	 			   
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//得到速度字符串	 
 	LCD_ShowString(30,400,200,16,16,dtbuf);	 				    
	if(gpsx.fixmode<=3)														//定位状态
	{  
		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
	  LCD_ShowString(30,420,200,16,16,dtbuf);			   
	}	 	    
	sprintf((char *)dtbuf,"GPS+BD Valid satellite:%02d",gpsx.posslnum);	 		//用于定位的GPS卫星数
 	LCD_ShowString(30,440,200,16,16,dtbuf);	    
	sprintf((char *)dtbuf,"GPS Visible satellite:%02d",gpsx.svnum%100);	 		//可见GPS卫星数
 	LCD_ShowString(30,460,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//可见北斗卫星数
 	LCD_ShowString(30,480,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//显示UTC日期
	LCD_ShowString(30,500,200,16,16,dtbuf);		    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//显示UTC时间
  LCD_ShowString(30,520,200,16,16,dtbuf);		  

}   


int main(void)
{ 
	u8 t=0,report=1;	              //默认开启上报
	u16 i,rxlen;
	u16 lenx;
	u8 key;
	u8 key1=0XFF;
	u8 upload = 1;
	float pitch,roll,yaw; 	        //欧拉角
	short aacx,aacy,aacz;	          //加速度传感器原始数据
	short gyrox,gyroy,gyroz;        //陀螺仪原始数据 
	short mx,my,mz;                 //磁罗盘原始数值
	short temp;		                  //温度
	HAL_Init();                     //初始化HAL库   
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);                //初始化延时函数
	uart_init(9600);                //初始化USART
  usart3_init(9600);  						//初始化串口3波特率为38400
	SDRAM_Init();                   //初始化SDRAM
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	LCD_Init();                     //初始化LCD
  usmart_dev.init(90);						//初始化usmart

	while(mpu_dmp_init())         
	{   
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
		LED0=!LED0;//DS0闪烁 
	}

		

//	  if(SkyTra_Cfg_Rate(5)!=0)	//设置定位信息更新速度为5Hz,顺便判断GPS模块是否在位. 
//		{
//   	LCD_ShowString(30,340,200,16,16,"SkyTraF8-BD Setting...");
//		do
//		{
//			usart3_init(9600);			//初始化串口3波特率为9600
//	  	SkyTra_Cfg_Prt(3);			//重新设置模块的波特率为38400
//			usart3_init(38400);			//初始化串口3波特率为38400
//      //key1=SkyTra_Cfg_Tp(100000);	//脉冲宽度为100ms
//		}while(SkyTra_Cfg_Rate(5)!=0);//配置SkyTraF8-BD的更新速率为5Hz
//	  LCD_ShowString(30,340,200,16,16,"SkyTraF8-BD Set Done!!");
//		delay_ms(500);
//		LCD_Fill(30,340,30+200,120+16,WHITE);//清除显示 
//		}

    while(1)
    {
        if(mpu_mpl_get_data(&pitch,&roll,&yaw,&mx,&my,&mz)==0){
					temp=MPU_Get_Temperature();										    	//得到温度值
					MPU_Get_Accelerometer(&aacx,&aacy,&aacz);						//得到加速度传感器数据
					MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);						//得到陀螺仪数据

			/*
			if((t%10)==0)
		    { 
				if(temp<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		    //显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,200,' ',16,0);		//去掉负号 
				
				LCD_ShowNum(30+48+8,200,temp/100,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,200,temp%10,1,16);		//显示小数部分 
				temp=pitch*10;
				if(temp<0)
			    {
					LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,220,temp%10,1,16);		//显示小数部分 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
				   
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,240,temp%10,1,16);		//显示小数部分 
				temp=yaw*10;
				if(temp<0)
				{
				   	LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
				   	temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,260,temp%10,1,16);		//显示小数部分  
				t=0;
				LED0=!LED0;//DS0闪烁 
		 	}*/
		 }
				t++;
		 delay_ms(1);
		 
		 
		if(USART3_RX_STA&0X8000)		//接收到一次数据了
		{
			rxlen=USART3_RX_STA&0X7FFF;	//得到数据长度
			if((USART3_RX_BUF[0] == 0xB5) && (USART3_RX_BUF[1] == 0x62))
			{
				for(i=0;i<rxlen;i++) USART1_TX_BUF[i]=USART3_RX_BUF[i];	   
   			//GPS_Analysis(&gpsx,&gps_meas,(u8*)USART1_TX_BUF);//分析字符串
				if((USART1_TX_BUF[2] == 0x01) &&(USART1_TX_BUF[3] == 0x07) && (USART1_TX_BUF[4] == 0x5C) && (USART1_TX_BUF[5] == 0x00))
				UBX_NAV_PVT(&gpsx,(u8*)USART1_TX_BUF);	
				//UBX_TRK_MEAS(gps_meas,(u8*)USART1_TX_BUF);
				//Gps_Msg_Show();				//显示信息	
			}
  			USART3_RX_STA=0;		   	//启动下一次接收
  			USART1_TX_BUF[i]=0;			//自动添加结束符
 		}
				
		if((lenx%500)==0)
			LED0=!LED0;
		  lenx++;	
    if(upload|report) send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*100),mx,my,mz);
		
	 } 	    
 }

