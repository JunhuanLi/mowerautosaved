#include "rtk.h"
uint8_t buff[4096];		

/* get fields (little-endian) ------------------------------------------------*/
#define U1(p) (*((unsigned char *)(p)))
#define I1(p) (*((signed char *)(p)))
static unsigned short U2(unsigned char *p) {unsigned short u; memcpy(&u,p,2); return u;}
static unsigned int   U4(unsigned char *p) {unsigned int   u; memcpy(&u,p,4); return u;}
static int            I4(unsigned char *p) {int            u; memcpy(&u,p,4); return u;}
static float          R4(unsigned char *p) {float          r; memcpy(&r,p,4); return r;}
static double         R8(unsigned char *p) {double         r; memcpy(&r,p,8); return r;}
static double         I8(unsigned char *p) {return I4(p+4)*4294967296.0+U4(p);}

/* sync code -----------------------------------------------------------------*/
static int sync_ubx(unsigned char *buff, unsigned char data)
{
    buff[0]=buff[1]; buff[1]=data;
    return buff[0]==0xB5&&buff[1]==0x62;
}

int decode_ubx_pvt(FILE *fp_in,FILE *fp_out){
	
	int data,i,type;
	unsigned char *p;
	float lon,lat,height,hMSL;
	float hAcc,vAcc,sAcc,vn,ve,vd;

	fseek(fp_in,-1L,1);          //判读文件是否到尾部时读取了1字节，故文件指针退后1字节
	for (i=0;;i++) {
		data=fgetc(fp_in);
		if (data==EOF)                           return 0;
		if (sync_ubx(buff,(unsigned char)data))  break;
		if (i>=4096)                             return 0;
	}
	if (fread(buff+2,1,4,fp_in)<4)                  return 0;
	if (fread(buff+6,1,94,fp_in)<(size_t)94)  return INCOMPLETE;
    type = (U1(buff+2)<<8)+U1(buff+3);
	if(type==0x0107){
		/* checksum */
		//if (!checksum(buff,100))              return 0;
		p = buff+6;
		lon = I4(p+24)*1e-7;                          //deg
		lat = I4(p+28)*1e-7;                          //deg
		height = I4(p+32)*0.001;                      //m
		hMSL = I4(p+36)*0.001;                        //m
		hAcc = U4(p+40)*0.001;                        //m
		vAcc = U4(p+44)*0.001;                        //m
		sAcc = U4(p+68);                             //mm/s
		vn = I4(p+48);                               //mm/s
		ve = I4(p+52);                               //mm/s
		vd = I4(p+56);                               //mm/s

		//if(hAcc>100)
		//	hAcc=1;
		
		fprintf(fp_out,"%f,%f,%f,%f,%f\n",lon,lat,hMSL,height,vAcc);
	}
}



void main(void){
	FILE   *fp_in =  fopen("out.ubx", "rb");           						                     
	FILE   *fp_out = fopen("pvt_out.txt", "w");	
	//FILE   *fp_in =  fopen("c/*enter.ubx", "rb");           						                     
	//FILE   *fp_out = fopen("center_out.txt", "w");	

	while((fgetc(fp_in)>EOF)||(fgetc(fp_in)>EOF)){
		decode_ubx_pvt(fp_in,fp_out);
	}

	fclose(fp_in);fclose(fp_out);
}