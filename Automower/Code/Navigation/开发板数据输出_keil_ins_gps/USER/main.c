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
 ALIENTEK ������STM32F429������ʵ��34
 MPU9250���ᴫ����ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 					//����1,���ͻ�����
nmea_msg gpsx; 								            			//GPS��Ϣ
meas_msg gps_meas[5]; 

__align(4) u8 dtbuf[50];   								      //��ӡ������
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};	//fix mode�ַ��� 
	 
//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
    while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)==RESET){}; 
    USART1->DR=c;  
} 

//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0X01~0X1C
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u16*data,u8 len)
{
	u16 send_buf[70];
	u8 i;
	//if(len>45)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X41;	//֡ͷ
	send_buf[1]=0X41;	//֡ͷ
	send_buf[2]=fun;	//������
	send_buf[3]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//��������
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//����У���	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}


//����GPS����
void send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw,short mx,short my,short mz)
{
	  u16 tbuf[63]; 
		tbuf[0] = (gpsx.latitude)>>24&0XFF;            //u32��γ��,��*100
		tbuf[1] = (gpsx.latitude)>>16&0XFF;            //u32,��*100
		tbuf[2] = (gpsx.latitude)>>8&0XFF;             //u32,��*100
		tbuf[3] = gpsx.latitude&0XFF;                  //u32,��*100
		tbuf[4] = (gpsx.longitude)>>24&0XFF;           //u32������,��*100
		tbuf[5] = (gpsx.longitude)>>16&0XFF;           //u32,��*100
		tbuf[6] = (gpsx.longitude)>>8&0XFF;            //u32,��*100
		tbuf[7] = gpsx.longitude&0XFF;                 //u32,��*100
		//tbuf[8] = gpsx.gpssta;                         //u8,GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
		//tbuf[9] = gpsx.posslnum;	                		 //u8,���ڶ�λ��GPS������,0~12.
		tbuf[8] = gpsx.fixType;				               //u8,��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ
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
		tbuf[43] = (mx>>8)&0XFF;//��Ϊ����MPL��,�޷�ֱ�Ӷ�ȡ����������,��������ֱ�����ε�.��0���.
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
		tbuf[55] = gpsx.posslnum;				               //u8,���ڶ�λ��������Ŀ
		tbuf[56] = (gpsx.heading)>>24&0XFF;          
		tbuf[57] = (gpsx.heading)>>16&0XFF;           
		tbuf[58] = (gpsx.heading)>>8&0XFF;           
		tbuf[59] = gpsx.heading&0XFF;     
		tbuf[60] = (gpsx.headAcc>>8)&0XFF;
		tbuf[61] = gpsx.headAcc&0XFF; 

		usart1_niming_report(0X31,tbuf,63);            //����֡,0X33
}


//ͨ������1�ϱ���������̬���ݸ�����(״̬֡)
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
//csb:�������߶�,��λ:cm
//prs:��ѹ�Ƹ߶�,��λ:mm
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
//	usart1_niming_report(0X31,tbuf,12);//״̬֡,0X01
//}  


//��ʾGPS��λ��Ϣ 
void Gps_Msg_Show(void)
{
 	float tp;		   
	POINT_COLOR=BLUE;  	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f   ",tp/=100000);	//�õ������ַ���
 	LCD_ShowString(30,340,200,16,16,dtbuf);	 	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f   ",tp/=100000);	//�õ�γ���ַ���
 	LCD_ShowString(30,360,200,16,16,dtbuf);	 	 
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//�õ��߶��ַ���
 	LCD_ShowString(30,380,200,16,16,dtbuf);	 			   
	tp=gpsx.speed;	   
 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//�õ��ٶ��ַ���	 
 	LCD_ShowString(30,400,200,16,16,dtbuf);	 				    
	if(gpsx.fixmode<=3)														//��λ״̬
	{  
		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
	  LCD_ShowString(30,420,200,16,16,dtbuf);			   
	}	 	    
	sprintf((char *)dtbuf,"GPS+BD Valid satellite:%02d",gpsx.posslnum);	 		//���ڶ�λ��GPS������
 	LCD_ShowString(30,440,200,16,16,dtbuf);	    
	sprintf((char *)dtbuf,"GPS Visible satellite:%02d",gpsx.svnum%100);	 		//�ɼ�GPS������
 	LCD_ShowString(30,460,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//�ɼ�����������
 	LCD_ShowString(30,480,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//��ʾUTC����
	LCD_ShowString(30,500,200,16,16,dtbuf);		    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//��ʾUTCʱ��
  LCD_ShowString(30,520,200,16,16,dtbuf);		  

}   


int main(void)
{ 
	u8 t=0,report=1;	              //Ĭ�Ͽ����ϱ�
	u16 i,rxlen;
	u16 lenx;
	u8 key;
	u8 key1=0XFF;
	u8 upload = 1;
	float pitch,roll,yaw; 	        //ŷ����
	short aacx,aacy,aacz;	          //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;        //������ԭʼ���� 
	short mx,my,mz;                 //������ԭʼ��ֵ
	short temp;		                  //�¶�
	HAL_Init();                     //��ʼ��HAL��   
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
	delay_init(180);                //��ʼ����ʱ����
	uart_init(9600);                //��ʼ��USART
  usart3_init(9600);  						//��ʼ������3������Ϊ38400
	SDRAM_Init();                   //��ʼ��SDRAM
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	LCD_Init();                     //��ʼ��LCD
  usmart_dev.init(90);						//��ʼ��usmart

	while(mpu_dmp_init())         
	{   
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
		LED0=!LED0;//DS0��˸ 
	}

		

//	  if(SkyTra_Cfg_Rate(5)!=0)	//���ö�λ��Ϣ�����ٶ�Ϊ5Hz,˳���ж�GPSģ���Ƿ���λ. 
//		{
//   	LCD_ShowString(30,340,200,16,16,"SkyTraF8-BD Setting...");
//		do
//		{
//			usart3_init(9600);			//��ʼ������3������Ϊ9600
//	  	SkyTra_Cfg_Prt(3);			//��������ģ��Ĳ�����Ϊ38400
//			usart3_init(38400);			//��ʼ������3������Ϊ38400
//      //key1=SkyTra_Cfg_Tp(100000);	//������Ϊ100ms
//		}while(SkyTra_Cfg_Rate(5)!=0);//����SkyTraF8-BD�ĸ�������Ϊ5Hz
//	  LCD_ShowString(30,340,200,16,16,"SkyTraF8-BD Set Done!!");
//		delay_ms(500);
//		LCD_Fill(30,340,30+200,120+16,WHITE);//�����ʾ 
//		}

    while(1)
    {
        if(mpu_mpl_get_data(&pitch,&roll,&yaw,&mx,&my,&mz)==0){
					temp=MPU_Get_Temperature();										    	//�õ��¶�ֵ
					MPU_Get_Accelerometer(&aacx,&aacy,&aacz);						//�õ����ٶȴ���������
					MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);						//�õ�����������

			/*
			if((t%10)==0)
		    { 
				if(temp<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		    //��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,200,' ',16,0);		//ȥ������ 
				
				LCD_ShowNum(30+48+8,200,temp/100,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,200,temp%10,1,16);		//��ʾС������ 
				temp=pitch*10;
				if(temp<0)
			    {
					LCD_ShowChar(30+48,220,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,220,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,220,temp%10,1,16);		//��ʾС������ 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,240,' ',16,0);		//ȥ������ 
				   
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,240,temp%10,1,16);		//��ʾС������ 
				temp=yaw*10;
				if(temp<0)
				{
				   	LCD_ShowChar(30+48,260,'-',16,0);		//��ʾ����
				   	temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,260,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,260,temp%10,1,16);		//��ʾС������  
				t=0;
				LED0=!LED0;//DS0��˸ 
		 	}*/
		 }
				t++;
		 delay_ms(1);
		 
		 
		if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{
			rxlen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
			if((USART3_RX_BUF[0] == 0xB5) && (USART3_RX_BUF[1] == 0x62))
			{
				for(i=0;i<rxlen;i++) USART1_TX_BUF[i]=USART3_RX_BUF[i];	   
   			//GPS_Analysis(&gpsx,&gps_meas,(u8*)USART1_TX_BUF);//�����ַ���
				if((USART1_TX_BUF[2] == 0x01) &&(USART1_TX_BUF[3] == 0x07) && (USART1_TX_BUF[4] == 0x5C) && (USART1_TX_BUF[5] == 0x00))
				UBX_NAV_PVT(&gpsx,(u8*)USART1_TX_BUF);	
				//UBX_TRK_MEAS(gps_meas,(u8*)USART1_TX_BUF);
				//Gps_Msg_Show();				//��ʾ��Ϣ	
			}
  			USART3_RX_STA=0;		   	//������һ�ν���
  			USART1_TX_BUF[i]=0;			//�Զ���ӽ�����
 		}
				
		if((lenx%500)==0)
			LED0=!LED0;
		  lenx++;	
    if(upload|report) send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*100),mx,my,mz);
		
	 } 	    
 }

