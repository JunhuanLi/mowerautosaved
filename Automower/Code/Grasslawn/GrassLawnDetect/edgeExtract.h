
/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: edgeExtract.h
  Author		: firefly     
  Version		: V1.0.0      
  Date			: 2017/06/9
  Description	: extract grass lawn    
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef CONNECTIONEXTRACT_H
#define CONNECTIONEXTRACT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "cv.h"
#include "highgui.h"
#include"iostream"
#include "math.h"

#define pi 3.14
#define RADIAN(angle) ((angle)*3.14/180.0)
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef unsigned char       byte;

using namespace cv;
using namespace std;
//#define MAX(a,b,c) a>(b>c?b:c)?a:
struct BMP_img
{
    WORD  bfType;
    DWORD size;
    DWORD reser;
    DWORD header_length;
    DWORD infoheader_length;
	DWORD width;
	DWORD height;
	WORD  biplanes;
	WORD  bmp_type;        /* 8bit 24bit; */
	DWORD compres;
	DWORD datasize;
    DWORD bixpm;
    DWORD biypm;
    DWORD clrused;
    DWORD  relclrused;
    BYTE *image;
    byte *header_info;
    DWORD lineBytes;
};

struct Bmp1
{
    DWORD width;
    DWORD height;
	byte strc[67][800];
	BYTE *image;
	int p1[15];//xl
	int p2[15];//xr
	int p3[15];//yu
	int p4[15];//yd
	int up;
	int down;
    byte strr[7][2500];
    byte string[7];
    float ang;//倾斜角度
};

struct HSV
{
    float H;
    float S;
    int V;
};

 struct RGB
{
	byte bitb;
    byte bitg;
    byte bitr;
	byte  re;
};

typedef std::vector<int> ivChainCode;
typedef struct PointCoord
{
    int x;
	int y;
}POINT;

void read_img(FILE *infile, struct BMP_img *img);
void getSpecialGreenImage(BMP_img img,byte *grayImage); 
void getSVMbwImage(BMP_img img,byte *grayImage);
void  getFromMatToByte(Mat frame, byte *bwImage0);
void fastMedianBlur(byte *srcImage, byte *dstImage, int w, int h, int radius);                 //fast median filter
int FastMedianValueCl(byte *srcImage, int w, int h, int histGram[], int radius, int x, int y); 
void thresholdByOtsu(byte *srcImage, byte *bwImage, int w, int h);                             //大津算法二值化
int getThresholdValue(byte *srcImage, byte *bwImage, int w, int h);                            //大津算法求阈值
void grayToBinay(byte *srcImage, byte *bwImage, int w, int h, int thresholdValue);             //二值分割
void Erosion(byte *image,int width,int height,int type,int num);                               //erotion
void Dilation(byte *image,int width,int height,int type,int num);                              //dilation
void getConectionRegion(byte *image, int iWidth, int iHeight);    //get connection region
int bwlabel(byte *img, int n, int *labels, int iWidth, int iHeight); //得到图像连通域
void getMaxConectionRegion(byte *image, int *labels, int iWidth, int iHeight);//get maxmum connection region
void getContours(byte *image, int iWidth, int iHeight);

bool TracingContour(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep,POINT *pStart, ivChainCode *pChainCode);
bool DrawContour(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep,POINT ptStart, ivChainCode ChainCode);


#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/