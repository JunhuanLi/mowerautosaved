
/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: edgeExtract.cpp
  Author		: firefly     
  Version		: V1.0.0      
  Date			: 2017/06/3
  Description	: extract grass lawn    
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#include"edgeExtract.h"


int mainGray()
{
    while(1) 
   {
       int HL=0,HH=0,VH=0,VL=0;
       struct BMP_img img;
       struct Bmp1 img1;
       FILE *f;
       byte *temp,*temp1;
      //char path[80]="C:\\test\\2.bmp";
       char path1[80]="C:\\test\\moban\\";
       char path[80];
       char path3[40];
       printf("Please Input Path\n");
       scanf("%s",path3);
       strcpy(path,path3);
       printf("the path is :%s\n",path);
   // char path1[80]="//mnt//arm1//test//moban//";
    //char path[80]="C:\\Documents and Settings\\liujia\\桌面\\test\\example06.bmp";
    //char path1[80]="C:\\Documents and Settings\\liujia\\桌面\\test\\moban\\";
      if((f=fopen(path,"rb"))==NULL)
     {
        printf( "\nCan not open the path: %s \n", path);
        exit(-1);
      }
      read_img(f, &img);
	  namedWindow("OriginalImage");
//	  double time = getTickCount();

	  int imgType;
	  switch (img.bmp_type)
	  {
	     case 8:
		     imgType = CV_8U;
			 break;
	     case 24:
		     imgType = CV_8UC3;
			 break;
	     default:
			 printf("Incorrect image type !");
		     break;
	  }

	  Mat image(Size(img.width,img.height),imgType);
	  image.data = img.image;

	  byte *grayImage;
	  grayImage = (byte *)malloc(sizeof(byte)*img.height * img.width);
	  getSpecialGreenImage(img,grayImage);

	  cout<<"the width of image is  : "<<img.width<<endl;
	  cout<<"the height of image is : "<<img.height<<endl;

	  int w = img.width;
	  int h = img.height;
	  int radius = 2;
	  byte *dstImage = (byte *)malloc(sizeof(byte)*img.height * img.width);
	  byte *bwImage = (byte *)malloc(sizeof(byte)*img.height * img.width);
	  byte *dilationImage = (byte *)malloc(sizeof(byte)*img.height * img.width);

	  double time = getTickCount();
	  fastMedianBlur(grayImage,dstImage, w,  h, radius);	 
	  thresholdByOtsu(dstImage, bwImage, w, h);
	  
	  Dilation(bwImage,w,h,0,15);
	  Dilation(bwImage,w,h,1,15);
	  Erosion(bwImage,w,h,0,15);
	  Erosion(bwImage,w,h,1,15);

//	  getConectionRegion(bwImage, w, h);
//	  getMaxConectionRegion(bwImage, w, h);

	  int n = 8;
	  int *labels = (int *)malloc(sizeof(int) * w * h);
	  int nObject = bwlabel(bwImage, n, labels, w, h);
	  cout<<"find number of object is : "<<nObject<<endl;
	   
	                                                                                                                                                    
	  getMaxConectionRegion(bwImage, labels, w, h);
	  Dilation(bwImage, w ,h, 0, 15);
	  Dilation(bwImage, w, h, 1, 15);

//	  getContours(bwImage, w, h);

	  POINT pStart ;
	  ivChainCode pChainCode ;
	  TracingContour(bwImage, w, h, w, &pStart, &pChainCode); 
      DrawContour(bwImage, w, h, w, pStart, pChainCode);

	  time = (double)getTickCount() - time;
      time = time / getTickFrequency() *1000;                  
      cout<<"time = "<<time<<" ms"<<endl;

	  Mat grayImg(Size(image.cols,image.rows),CV_8UC1);
//	  grayImg.data = grayImage;
	  grayImg.data = bwImage;
	  imshow("OriginalImage",image);
	  imshow("bwImage",grayImg);
	  waitKey();

      fclose(f);
      free(temp1);
      free(temp);
     getchar();
  }
  return 0;
}


int mainSVM()
{
    while(1) 
   {
       int HL=0,HH=0,VH=0,VL=0;
       struct BMP_img img;
       struct Bmp1 img1;
       FILE *f;
       byte *temp,*temp1;
       char path1[80]="C:\\test\\moban\\";
       char path[80];
       char path3[40];
       printf("Please Input Path\n");
       scanf("%s",path3);
       strcpy(path,path3);
       printf("the path is :%s\n",path);
      if((f=fopen(path,"rb"))==NULL)
     {
        printf( "\nCan not open the path: %s \n", path);
        exit(-1);
      }
      read_img(f, &img);
	  namedWindow("OriginalImage");

	  double time = getTickCount();

	  int imgType;
	  switch (img.bmp_type)
	  {
	     case 8:
		     imgType = CV_8U;
			 break;
	     case 24:
		     imgType = CV_8UC3;
			 break;
	     default:
			 printf("Incorrect image type !");
		     break;
	  }

	  Mat image(Size(img.width,img.height),imgType);
	  image.data = img.image;

	  cout<<"the width of image is  : "<<img.width<<endl;
	  cout<<"the height of image is : "<<img.height<<endl;

	  int w = img.width;
	  int h = img.height;
	  byte *bwImage0 = (byte *)malloc(sizeof(byte)*img.height * img.width);
	  byte *bwImage = (byte *)malloc(sizeof(byte)*img.height * img.width);

	  getSVMbwImage(img,bwImage0);
	  
	  fastMedianBlur(bwImage0,bwImage, w,  h, 3);

	  Dilation(bwImage,w,h,0,15);
	  Dilation(bwImage,w,h,1,15);
	  Erosion(bwImage,w,h,0,15);
	  Erosion(bwImage,w,h,1,15);

	  int n = 8;
	  int *labels = (int *)malloc(sizeof(int) * w * h);
	  int nObject = bwlabel(bwImage, n, labels, w, h);
	  cout<<"find number of object is : "<<nObject<<endl;
	   
	                                                                                                                                                    
	  getMaxConectionRegion(bwImage, labels, w, h);
	  //Dilation(bwImage, w ,h, 0, 5);
	  //Dilation(bwImage, w, h, 1, 5);

	  POINT pStart ;
	  ivChainCode pChainCode ;
	  TracingContour(bwImage, w, h, w, &pStart, &pChainCode); 
      DrawContour(bwImage, w, h, w, pStart, pChainCode);

	  time = (double)getTickCount() - time;
      time = time / getTickFrequency() *1000;                  
      cout<<"time = "<<time<<" ms"<<endl;

	  Mat grayImg(Size(image.cols,image.rows),CV_8UC1);
//	  grayImg.data = grayImage;
	  grayImg.data = bwImage;
	  imshow("OriginalImage",image);
	  imshow("bwImage",grayImg);
	  waitKey();

      fclose(f);
      free(temp1);
      free(temp);
     getchar();
  }
  return 0;
}

/************Video Test******************/
int mainTest()
{
	VideoCapture capture("E:\\Videos4\\41.mp4");
    if (!capture.isOpened())
        cout << "fail toopen!"<< endl;

    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
    cout << "整个视频共" << totalFrameNumber << "帧" << endl;

    long frameToStart = 1;
    capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
    cout << "从第" <<frameToStart << "帧开始读" << endl;

    int frameToStop = 30000;
 
    if (frameToStop < frameToStart)
    {
        cout << "结束帧小于开始帧，程序错误，即将退出！" << endl;
        return -1;
    }
    else
    {
        cout << "结束帧为：第" << frameToStop << "帧" << endl;
    }
 
    double rate = capture.get(CV_CAP_PROP_FPS);
    cout << "帧率为:" << rate<< endl;
    bool stop = false;
 
//    namedWindow("Extractedframe");
    double delay = 1000 / rate;
    long currentFrame = frameToStart;
    Mat frame;
    while (!stop)
    {
        if(!capture.read(frame))
        {
            cout << "读取视频失败" << endl;
            return -1;
        }
      
	  Mat image;
	  resize(frame,image,Size(frame.cols,frame.rows));
	  int w = image.cols;
	  int h = image.rows;

	  byte *bwImage0 = (byte *)malloc(sizeof(byte) * h * w);
	  byte *bwImage = (byte *)malloc(sizeof(byte) * h * w);

	  double time = getTickCount();

	  getFromMatToByte(image, bwImage0);
	  fastMedianBlur(bwImage0,bwImage, w,  h, 1);

	  Dilation(bwImage,w,h,0,5);
	  Dilation(bwImage,w,h,1,5);
	  Erosion(bwImage,w,h,0,5);
	  Erosion(bwImage,w,h,1,5);

	  int n = 8;
	  int *labels = (int *)malloc(sizeof(int) * w * h);
	  int nObject = bwlabel(bwImage, n, labels, w, h);
	  cout<<"find number of object is : "<<nObject<<endl;
	   
	                                                                                                                                                    
	  getMaxConectionRegion(bwImage, labels, w, h);
	  //Dilation(bwImage, w ,h, 0, 5);
	  //Dilation(bwImage, w, h, 1, 5);

	  POINT pStart ;
	  ivChainCode pChainCode ;
	  TracingContour(bwImage, w, h, w, &pStart, &pChainCode); 
      DrawContour(bwImage, w, h, w, pStart, pChainCode);

	  time = (double)getTickCount() - time;
      time = time / getTickFrequency() *1000;                  
      cout<<"time = "<<time<<" ms"<<endl;

	  Mat grayImg(Size(image.cols,image.rows),CV_8UC1);
	  grayImg.data = bwImage;
	  imshow("OriginalImage",image);
	  imshow("bwImage",grayImg);

      int c = waitKey(delay);
      if ((char)c == 27 ||currentFrame > frameToStop)
      {
          stop = true;
      }
      if (c >= 0)
      {
          waitKey(0);
      }
      currentFrame++;
 
    }
    capture.release();
    waitKey(0);
    return 0;
}



int main()
{
//	mainGreen();
//	mainSVM();
	mainTest();
    return 1;
}

/*******************************************************************************
  Function		: read_img      
  Description	: read bmp image 
  Input			: infile, img                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void read_img(FILE *infile, struct BMP_img *img)
{
    DWORD i,j,l,bitcolor;
    DWORD line24;
    DWORD line8;
    struct RGB *bitmap;
	fread(&img->bfType,sizeof(WORD),1,infile);//printf("\n打开的图为 %d",img->bfType);
	fread(&img->size,sizeof(DWORD),1,infile);     //        printf("\nBMP size             :%l",img->size);
	fread(&img->reser,sizeof(DWORD),1,infile);//printf("\n保留位:");
	fread(&img->header_length,sizeof(DWORD),1,infile); //printf("\nheader length    :%l",img->header_length);
	fread(&img->infoheader_length,sizeof(DWORD),1,infile);
	fread(&img->width, sizeof(DWORD), 1, infile);
	fread(&img->height, sizeof(DWORD), 1, infile);     //printf( "\nwidth   :%l\n  height  :%l ", img->width, img->height);
	fread(&img->biplanes, sizeof(WORD), 1, infile);
	fread(&img->bmp_type, sizeof(WORD), 1, infile);  // printf("\nBMP Tpye             :%l ", img->bmp_type);
	fread(&img->compres, sizeof(DWORD), 1, infile);    //if(img->compres==0) {printf("\nbmp图片为非压缩!");}printf(" ");
	fread(&img->datasize, sizeof(DWORD), 1, infile);//printf("\nBMP Data Size        :%l ",img->datasize);
	fread(&img->bixpm, sizeof(DWORD), 1, infile);
	fread(&img->biypm, sizeof(DWORD), 1, infile);
	fread(&img->clrused, sizeof(DWORD), 1, infile);    //printf("\n实际使用颜色数=%d ",img->clrused);printf(" ");
	fread(&img->relclrused, sizeof(DWORD), 1, infile);

	img->lineBytes=(img->width*img->bmp_type+31)/32*4;                                                                                                              //printf("\nLineBytes            :%l\n",img->lineBytes);
    line24=(img->width*24+31)/32*4;
    line8=(img->width*8+31)/32*4;
    if(img->bmp_type==1){bitcolor=2;printf("不能读取退出");exit(-1);}
    if(img->bmp_type==4){bitcolor=16;printf("不能读取退出");exit(-1);}
    if(img->bmp_type==8)
    {
	   byte *temp=(BYTE*)malloc(img->height*line8*sizeof(BYTE));
	   memset(temp,0x00,img->height*img->lineBytes*sizeof(BYTE));

	   bitcolor=256;
	   bitmap=(struct RGB *)calloc(bitcolor,sizeof(struct RGB));
	   img->image=(unsigned char *)malloc(sizeof(unsigned char)*(line8*img->height));
	   memset(img->image,0x00,sizeof(byte)*line8*img->height);

	   if(img->image==NULL) {fprintf(stderr, "\n Allocation error for temp in read_bmp() \n");}

	   fseek(infile,0x36, SEEK_SET);
	   fread(bitmap,sizeof(struct RGB),bitcolor,infile);
	   fseek(infile, img->header_length, SEEK_SET);
	   //fread(temp, sizeof(unsigned char),lineBytes*img->height, infile);
	   fread(temp, img->lineBytes*img->height,1, infile);
	   if(temp==NULL)printf("\n读取失败\n");

	   for(i=0;i<img->height;i++)
	   {
		   for(j=0;j<img->width;j++)
		   {
		       img->image[i*img->width+j]=(byte)(0.299*bitmap[temp[i*line8+j]].bitb+0.578*bitmap[temp[i*line8+j]].bitg+0.114*bitmap[temp[i*line8+j]].bitr);
	           // putpixel(j,img->height-i,RGB(img->image[i*img->width+j],img->image[i*img->width+j],img->image[i*img->width+j]));
		    }
	   }
	   free(temp);
	   temp=NULL;
	}
    if(img->bmp_type==24)
    {
        byte *temp=(byte *)malloc(sizeof(byte)*img->height*img->lineBytes); if(temp==NULL)
        exit(-1);
        img->image=(unsigned char *)malloc(sizeof(unsigned char)*((line24)*img->height));
        if(img->image==NULL) fprintf(stderr, "\n Allocation error for temp in read_bmp() \n");
        fseek(infile, img->header_length, SEEK_SET);
        fread(temp, sizeof(unsigned char), (img->lineBytes)*img->height, infile);
        // byte *temp=(byte *)malloc(sizeof(byte)*img->lineBytes*img->height)
        for(i=0;i<img->height;i++)
        {
            l=0;
            for(j=0;j<img->width*3;j+=3)
            {
                //l=(img->height-i-1)*img->lineBytes+j;
                l = (img->height-i-1) * img->width*3+j;
                img->image[l+2] = *(temp+i * img->lineBytes + j + 2);
                img->image[l+1] = *(temp+i*img->lineBytes + j + 1);
                img->image[l] = *(temp+i*img->lineBytes + j);
             }
         }
         free(temp);
         temp=NULL;
     }
}


/*******************************************************************************
  Function		: getFromMatToByte      
  Description	: transform Mat to Byte
  Input			: frame                    
  Output		: bwImage0       
  Return		: None       
*******************************************************************************/
void  getFromMatToByte(Mat frame, byte *bwImage0)
{
	int iWidth = frame.cols;
	double time = getTickCount();
	uchar *pDstFrame = bwImage0;
	int address = 0;
	for (int j = 0; j < frame.rows; ++j)
	{
	    uchar *pFrame = frame.ptr<uchar>(j);	   
        for (int i = 0; i< frame.cols; ++i)
	    {
			float cx = (float)pFrame[3 * i];
		    float cy = (float)pFrame[3 * i + 1];
			float cz = (float)pFrame[3 * i + 2];
		    float ccx = 2 * cy - cx -cz;
		    float ccy = 3 * cy - cx -2.4 * cz;
		    float ccz = ( cy - cz )/( cy + cz );
			float response = - ( 2.72473902e-001 * cx +  (-5.98878488e-002) * cy + (-2.03766197e-001) * cz + 
				            (-1.88483387e-001) * ccx + (3.69033664e-002 * ccy) )  + (-5.3446250250732703e+000);     // -WX + B
            if (response > 0)
                pDstFrame[address++]  = 255;
            else 
                pDstFrame[address++]  = 0;
        }  
   }
}

/*******************************************************************************
  Function		: getSVMbwImage      
  Description	: predict image
  Input			: img                    
  Output		: grayImage       
  Return		: None       
*******************************************************************************/
void getSVMbwImage(BMP_img img,byte *grayImage)
{
	if(img.bmp_type != 24)
	{
	   printf("Incorrect image type !");
	   return;
	}
	WORD i,j,k;
	float cx, cy, cz;
	byte *grayImg = (byte*)grayImage;
	for(i=0; i<img.height; i++)
	{
	   byte *lineStartAddress = img.image + i * img.lineBytes;
	   for(j=0; j<img.width; j++)
	   {
           cx = (float)lineStartAddress[3*j+0];
		   cy = (float)lineStartAddress[3*j+1];
		   cz = (float)lineStartAddress[3*j+2];
		   float ccx = 2 * cy - cx -cz;
		   float ccy = 3 * cy - cx -2.4 * cz;
		   float ccz = ( cy - cz )/( cy + cz );
		   float response = - ( 2.72473902e-001 * cx +  (-5.98878488e-002) * cy + (-2.03766197e-001) * cz + 
						    (-1.88483387e-001) * ccx + (3.69033664e-002 * ccy) )  + (-5.3446250250732703e+000);     // -WX + B
		   if (response > 0)
			   *(grayImg++)  = 255;
		   else 
			   *(grayImg++)  = 0;
	   }
	}
}


/*********************************-------getSpecialGreenImage---------****************************************************/
void getSpecialGreenImage(BMP_img img,byte *grayImage)
{
	if(img.bmp_type != 24)
	{
	   printf("Incorrect image type !");
	   return;
	}
	WORD i,j,k;
	int pixB, pixG, pixR;
	byte *grayImg = (byte*)grayImage;
	for(i=0; i<img.height; i++)
	{
	   byte *lineStartAddress = img.image + i * img.lineBytes;
	   for(j=0; j<img.width; j++)
	   {
           pixB = lineStartAddress[3*j+0];
		   pixG = lineStartAddress[3*j+1];
		   pixR = lineStartAddress[3*j+2];
		   int gray = 2 * pixG - pixB - pixR ;
//		   float gray = pixB * 0.299 + pixG * 0.578 + pixR * 0.114;
           gray = gray > 255 ? 255 : gray;
		   gray = gray < 0 ? 0:gray;
		   *(grayImg++) = (byte)gray;
	   }
	}
}
/*********************************-------getSpecialGreenImage---------****************************************************/


/*******************************************************************************
  Function		: fastMedianBlur      
  Description	: fast median blur
  Input			: srcImage, w, h, radius                    
  Output		: dstImage       
  Return		: None       
*******************************************************************************/
void fastMedianBlur(byte *srcImage, byte *dstImage, int w, int h, int radius)
{
    int x, y ;
    int a;
    int histGram[256];
	memset(dstImage,0,sizeof(byte)*w*h);
    for(y = radius; y < h - radius; y++)
    {
        for (int i = 0; i < 256; i ++)
                histGram[i] = 0;

        for(int i = y - radius; i < y + radius; i++)
		{
			byte *lineAdd = (byte *)(srcImage + i * w);
            for (int j = 0; j < 2 * radius + 1; j++)
			{
				int k = lineAdd[j];
                histGram[k] ++;
            }
		}

        for(x = radius + 1; x< w - radius; x++)
        {
            a = FastMedianValueCl(srcImage, w ,h ,histGram ,radius ,x ,y);
			*(dstImage + y * w + x) = (byte)a;
        }
    }
}

int FastMedianValueCl(byte *srcImage, int w, int h, int histGram[], int radius, int x, int y)
{
	int i, j, k;                   
    int px, py, midValue;
    int count = 0;                 //用来保存采样窗口的像素数量
    int count_temp = 0;
    k=0;

	for(int i=y-radius;i<y+radius;i++)
	{
        px = x - radius - 1 ;
	    k = *(srcImage + i * w + px);
		histGram[k]--;


		px = x + radius ;
	    k = *(srcImage + i * w + px);
		histGram[k]++;
	}

	int midRange  = (2 * radius + 1) * (2 * radius + 1) / 2 ;
    for(int a = 0; a < 256; a++)
	{
		midValue = a;
        count_temp += histGram[midValue];
        if(count_temp > midRange)
			break;
    }
    return midValue;
}
/*********************************-------fastMedianBlur---------****************************************************/


/*******************************************************************************
  Function		: thresholdByOtsu      
  Description	: image binaryzation by otsu
  Input			: srcImage, w, h                  
  Output		: dstImage       
  Return		: None       
*******************************************************************************/
void thresholdByOtsu(byte *srcImage, byte *bwImage, int w, int h)
{
	int thresholdValue = getThresholdValue(srcImage, bwImage, w, h);
	grayToBinay(srcImage, bwImage, w, h, thresholdValue);
}

int getThresholdValue(byte *srcImage, byte *bwImage, int w, int h)
{
	long Num = w * h;
	int i,j;

    float histogram[256] = {0}; 
	byte *p = (byte *)srcImage;
	for(i=0; i<Num; i++)
	{   
		int grayValue = *p;
	    histogram[grayValue]++;  
	    p++;
	}  

	int threshold;    
	long sum0 = 0, sum1 = 0; //存储前景的灰度总和和背景灰度总和
	long cnt0 = 0, cnt1 = 0; //前景的总个数和背景的总个数
	double w0 = 0, w1 = 0; //前景和背景所占整幅图像的比例
	double u0 = 0, u1 = 0;  //前景和背景的平均灰度
	double variance = 0; //最大类间方差
	double u = 0;
	double maxVariance = 0;
	for(i = 1; i < 256; i++) //一次遍历每个像素
	{  
		sum0 = 0;
		sum1 = 0; 
		cnt0 = 0;
		cnt1 = 0;
		w0 = 0;
	    w1 = 0;
		for(j = 0; j < i; j++)
		{
			cnt0 += histogram[j];
			sum0 += j * histogram[j];
		}

		u0 = (double)sum0 /  cnt0; 
		w0 = (double)cnt0 / Num;

		for(j = i ; j <= 255; j++)
		{
			cnt1 += histogram[j];
			sum1 += j * histogram[j];
		}

		u1 = (double)sum1 / cnt1;
		w1 = 1 - w0; // (double)cnt1 / size;

		u = u0 * w0 + u1 * w1; //图像的平均灰度
//		printf("u = %f\n", u);
		variance =  w0 * w1 *  (u0 - u1) * (u0 - u1);
		if(variance > maxVariance) 
		{  
			maxVariance = variance;  
			threshold = i;  
		} 
	}  
//	printf("threshold = %d\n", threshold);
	return  threshold;
}


/*******************************************************************************
  Function		: grayToBinay      
  Description	: transform gray image to binaryzation image
  Input			: srcImage, w, h, thresholdValue                  
  Output		: bwImage       
  Return		: None       
*******************************************************************************/
void grayToBinay(byte *srcImage, byte *bwImage, int w, int h, int thresholdValue)
{
    int Num = w * h;
	int i;
	for(i = 0; i< Num; i++)
	{
		byte grayValue = *(srcImage + i);
		grayValue = grayValue >  thresholdValue ? 255 : 0;
		*(bwImage + i) = grayValue;
	}
}
/*********************************---------thresholdByOtsu---------****************************************************/

/*******************************************************************************
  Function		: Dilation      
  Description	: dilation with rect element
  Input			: image, width, height, num, type   //type= 0 ,horizental , type = 1; vertical               
  Output		: bwImage       
  Return		: None       
*******************************************************************************/
void Dilation(byte *image,int width,int height,int type,int num)
{
    int dwWidth = width;
    int dwHeight = height;
    int i = 0, j = 0, k = 0;
	BYTE g = 0;
	BYTE *temp;
	temp = (BYTE *)malloc(dwHeight * dwWidth * sizeof(BYTE));

	memcpy(temp, image, dwHeight * dwWidth * sizeof(byte));
//    memset(temp, 0, dwWidth * dwHeight * sizeof(BYTE));

	if(type == 0)  //水平方向
    {
	    for(i = 0 ;i < dwHeight; i++)
	   {
	       for(j = (num - 1) / 2; j < dwWidth - (num - 1) / 2; j++)
	       {
		       for(k = - ( num - 1 ) / 2; k <= (num - 1) / 2; k++)
		       {
		           g = *(image + dwWidth * i + j + k);
		           if(g == 255)
		           {
		               *(temp + dwWidth * i + j) = 255;
		               break;
		            }
		        }
	        }
	    }
	}
	else     //垂直方向
    {
	    for(i = (num - 1) / 2; i< dwHeight - ( num - 1) / 2; i++)
	    {
	        for(j = 0; j < dwWidth; j++)
	        {
		        for(k = - ( num - 1 ) / 2; k <= (num - 1) / 2; k++)
		        {
		            g = *(image + dwWidth * (i + k) + j);
		            if(g == 255)
		            {
		                *(temp + dwWidth * i + j) = 255;
		                break;
		             }
		         }
	         }
	    }
	}
	memcpy(image, temp, sizeof(byte) * width * height);
	free(temp);
	temp=NULL;
}
/*********************************---------Dilation---------****************************************************/

/*******************************************************************************
  Function		: Erosion      
  Description	: erosion with rect element
  Input			: image, width, height, num, type   //type= 0 ,horizental , type = 1; vertical               
  Output		: bwImage       
  Return		: None       
*******************************************************************************/
void Erosion(byte *image,int width,int height,int type,int num)
{
	int dwWidth = width;
	int dwHeight = height;
	int i=0, j = 0;
	 BYTE g = 0;
	 BYTE *temp;
	 int k  =0;
	 temp = (BYTE*)malloc(dwHeight * dwWidth * sizeof(BYTE));
	 memcpy(temp, image, dwHeight * dwWidth * sizeof(byte));

	 if(type == 0) 	  //水平方向
	 {
	     for(i = 0; i < dwHeight; i++)
	     {
	         for(j = (num  -1) / 2; j < dwWidth - (num - 1) / 2; j++)
	         {
		         for(k = - (num - 1) / 2; k <= (num - 1) / 2; k++)
		         {
		             g = *(image + width * i + j + k);
		             if(g == 0)
		             {
		                *(temp + width * i + j) = 0;
		                break;
		             }
		          }
	         }
	     }
	 }
	 else  	  //垂直方向
	 {
	     for(i = (num - 1) / 2; i < dwHeight - (num - 1) / 2; i++)
	     {
	         for(j = 0; j < dwWidth; j++)
	         {
		         for(k = - (num - 1 )/ 2; k <= (num - 1) / 2; k++)
		         {
		             g = *(image + width * (i + k) + j);
		             if(g == 0)
		             {
		                 *(temp + width * i + j) = 0;
		                 break;
		              }
		          }
	          }
	      }
	 }
	 memcpy(image, temp, sizeof(byte)*width*height);
	 free(temp);
	 temp=NULL;
}
/*********************************---------Erosion---------****************************************************/


/*******************************************************************************
  Function		: getConectionRegion      
  Description	: get Conection Region
  Input			: image, width, height          
  Output		: image       
  Return		: None       
*******************************************************************************/
void getConectionRegion(byte *image, int iWidth, int iHeight)
{
	int n = 8;
	int *labels = (int *)malloc(sizeof(int) * iWidth * iHeight);
	bwlabel(image, n, labels, iWidth, iHeight);
	int i;
	for(i=0; i<iWidth * iHeight; i++)
	{
        image[i] = labels[i];
	}
    Mat grayImg(Size(iWidth,iHeight),CV_8UC1);
    grayImg.data = image;
	imshow("conectionImage",grayImg);
    waitKey();
}


#define     NO_OBJECT       0
#define     MIN(x, y)       (((x) < (y)) ? (x) : (y))
#define     ONETWO(L, r, c, col) (L[(r) * (col) + c])

int find( int set[], int x )
{
	int r = x;
	while ( set[r] != r )
		r = set[r];
	return r;
}

int bwlabel(byte *img, int n, int *labels, int iWidth, int iHeight)
{
    if(n != 4 && n != 8)
       n = 4;
	int nr = iHeight;
	int nc = iWidth;
	int total = nc * nr;
	memset(labels, 0, total * sizeof(int));
	int nobj = 0;                               // number of objects found in image
	// other variables                             
	int* lset = new int[total];   // label table
	memset(lset, 0, total * sizeof(int));
	int ntable = 0;
	for( int r = 0; r < nr; r++ ) 
	{
		for( int c = 0; c < nc; c++ ) 
		{            
			if ( *(img + r * iWidth + c) )   // if A is an object  *(img + r * iWidth + c)
			{               
				// get the neighboring pixels B, C, D, and E
				int B, C, D, E;
				if ( c == 0 ) 
					B = 0; 
				else 
					B = find( lset, ONETWO(labels, r, c - 1, nc) );
				if ( r == 0 ) 
					C = 0; 
				else 
					C = find( lset, ONETWO(labels, r - 1, c, nc) );
				if ( r == 0 || c == 0 ) 
					D = 0; 
				else 
					D = find( lset, ONETWO(labels, r - 1, c - 1, nc) );
				if ( r == 0 || c == nc - 1 ) 
					E = 0;
				else 
					E = find( lset, ONETWO(labels, r - 1, c + 1, nc) );
				if ( n == 4 ) 
				{
					// apply 4 connectedness
					if ( B && C ) 
					{        // B and C are labeled
						if ( B == C )
							ONETWO(labels, r, c, nc) = B;
						else {
							lset[C] = B;
							ONETWO(labels, r, c, nc) = B;
						}
					} 
					else if ( B )             // B is object but C is not
						ONETWO(labels, r, c, nc) = B;
					else if ( C )               // C is object but B is not
						ONETWO(labels, r, c, nc) = C;
					else 
					{                      // B, C, D not object - new object
						//   label and put into table
						ntable++;
						ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;
					}
				} 
				else if ( n == 6 ) 
				{
					// apply 6 connected ness
					if ( D )                    // D object, copy label and move on
						ONETWO(labels, r, c, nc) = D;
					else if ( B && C ) 
					{        // B and C are labeled
						if ( B == C )
							ONETWO(labels, r, c, nc) = B;
						else 
						{
							int tlabel = MIN(B,C);
							lset[B] = tlabel;
							lset[C] = tlabel;
							ONETWO(labels, r, c, nc) = tlabel;
						}
					} 
					else if ( B )             // B is object but C is not
						ONETWO(labels, r, c, nc) = B;
					else if ( C )               // C is object but B is not
						ONETWO(labels, r, c, nc) = C;
					else 
					{                      // B, C, D not object - new object
						//   label and put into table
						ntable++;
						ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;
					}
				}
				else if ( n == 8 ) 
				{
					// apply 8 connectedness
					if ( B || C || D || E ) 
					{
						int tlabel = B;
						if ( B ) 
							tlabel = B;
						else if ( C ) 
							tlabel = C;
						else if ( D ) 
							tlabel = D;
						else if ( E ) 
							tlabel = E;
						ONETWO(labels, r, c, nc) = tlabel;
						if ( B && B != tlabel ) 
							lset[B] = tlabel;
						if ( C && C != tlabel ) 
							lset[C] = tlabel;
						if ( D && D != tlabel ) 
							lset[D] = tlabel;
						if ( E && E != tlabel ) 
							lset[E] = tlabel;
					} 
					else 
					{
						//   label and put into table
						ntable++;
						ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;
					}
				}
			} 
			else 
			{
				ONETWO(labels, r, c, nc) = NO_OBJECT;      // A is not an object so leave it
			}
		}
	}
	// consolidate component table
	for( int i = 0; i <= ntable; i++ )
		lset[i] = find( lset, i );                                                                                                 
	// run image through the look-up table
	for( int r = 0; r < nr; r++ )
		for( int c = 0; c < nc; c++ )
			ONETWO(labels, r, c, nc) = lset[ ONETWO(labels, r, c, nc) ];
	// count up the objects in the image
	for( int i = 0; i <= ntable; i++ )
		lset[i] = 0;
	for( int r = 0; r < nr; r++ )
		for( int c = 0; c < nc; c++ )
			lset[ ONETWO(labels, r, c, nc) ]++;
	// number the objects from 1 through n objects
	nobj = 0;
	lset[0] = 0;
	for( int i = 1; i <= ntable; i++ )
		if ( lset[i] > 0 )
			lset[i] = ++nobj;
	// run through the look-up table again
	for( int r = 0; r < nr; r++ )
		for( int c = 0; c < nc; c++ )
			ONETWO(labels, r, c, nc) = lset[ ONETWO(labels, r, c, nc) ];
	//
	delete[] lset;
	return nobj;
}
/*********************************---------提取二值图像连通域---------****************************************/


/*******************************************************************************
  Function		: getMaxValueOfLabels      
  Description	: get Max Value Of Labels
  Input			: arr, size          
  Output		: maxValue        
  Return		: (type) maxValue      
*******************************************************************************/
template<class type>
const type getMaxValueOfLabels(type arr[],int size) 
{ 
    type max = arr[0]; 
	type maxValue ;
    for (int i = 1; i < size; i++) 
	{
        if(max < arr[i]) 
		{
			max = arr[i]; 
			maxValue = i;
		}
	}
    return maxValue; 
}

void getMaxConectionRegion(byte *image, int *labels, int iWidth, int iHeight)
{
	int *histGram = (int *)malloc(sizeof(int)*10000);
    memset(histGram, 0, 10000 * sizeof(int));
	int i, j;
	int Num = iWidth * iHeight;
	for(i = 0 ;i < Num; i++)
	{
	    int grayScales = *(labels + i);
		if(grayScales >0 )
		    histGram[grayScales]++;
	}

	int maxValue = getMaxValueOfLabels(histGram, 10000) ;
	printf("maxValue is %d\n",maxValue);
    for(i = 0 ;i < Num; i++)
	{
	    int grayScales = *(labels + i);
		if(grayScales == maxValue)
		    *(image + i) = 255;
		else
             *(image + i) = 0;    
	}
	free(histGram);
}
/*********************************---------得到最大连通域---------*************************************/ 


/*******************************************************************************
  Function		: getMaxValueOfLabels      
  Description	: get contours of the connect region
  Input			: image, iWidth, iHeight          
  Output		: image        
  Return		: None      
*******************************************************************************/ 
void getContours(byte *image, int iWidth, int iHeight)
{
    int i, j ;
	byte *temp = (byte *)malloc(sizeof(byte) * iWidth *iHeight );
	memcpy(temp,image,sizeof(byte) * iWidth *iHeight);
	for(i = 1; i < iHeight-1; i++)
		for(j = 1; j < iWidth-1; j++)
		{
            int centre = (int)*(image + i * iWidth + j);
			int left = (int)*(image + i * iWidth + j - 1);
			int right = (int)*(image + i * iWidth + j + 1);
			int top = (int)*(image + (i - 1) * iWidth + j);
			int bottom = (int)*(image + (i + 1) * iWidth + j);
			if((centre>0) && (left>0) && (right>0) && (top>0) && (bottom>0))
				*(temp + i * iWidth + j) = 0;
		}
	memcpy(image, temp, sizeof(byte) * iWidth * iHeight);
	free(temp);
}

//#include <vector>
//typedef std::vector<int> ivChainCode;
// 轮廓跟踪
// 1. pImageData   图像数据
// 2. nWidth       图像宽度
// 3. nHeight      图像高度
// 4. nWidthStep   图像行大小
// 5. pStart       起始点
// 6. pChainCode   链码表

//typedef struct PointCoord
//{
//    int x;
//	int y;
//}POINT;
/*******************************************************************************
  Function		: TracingContour      
  Description	: trace Contour
  Input			: pImageData, nWidth, nHeight, nWidthStep, pStart, pChainCode         
  Output		: pStart, ivChainCode     
  Return		: None      
*******************************************************************************/ 
bool TracingContour(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep,
    POINT *pStart, ivChainCode *pChainCode)
{
    int i = 0;
	 
    int j = 0;
    int k = 0;
    int x = 0;
    int y = 0;
    bool bTracing = false;
    POINT ptCurrent = { 0, 0 };
    POINT ptTemp = { 0, 0 };
    unsigned char *pLine = NULL;
    const POINT ptOffset[8] =
    {
        { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 },
        { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }
    };
    // 清空起始点与链码表
    pStart->x = 0;
    pStart->y = 0;
    pChainCode->clear();
    // 轮廓起点
    for (y = 0; y < nHeight; y++)
    {
		if(bTracing)
			break;
        pLine = pImageData + nWidthStep * y;
        for (x = 0; x < nWidth; x++)
        {
            if (pLine[x] == 0xFF)
            {
                bTracing = true;
                pStart->x = x;
                pStart->y = y;
                ptCurrent.x = x;
                ptCurrent.y = y;
            }
        }
    }
    // 轮廓跟踪
    while (bTracing)
    {
        bTracing = false;
        for (i = 0; i < 8; i++, k++)
        {
            k &= 0x07;
            x = ptCurrent.x + ptOffset[k].x;
            y = ptCurrent.y + ptOffset[k].y;
            if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
            {
                // 判断是否为轮廓点
                if (pImageData[nWidthStep * y + x] == 0xFF)
                {
                    for (j = 0; j < 8; j += 2)
                    {
                        ptTemp.x = x + ptOffset[j].x;
                        ptTemp.y = y + ptOffset[j].y;
                        if (ptTemp.x >= 0 && ptTemp.x < nWidth &&
                            ptTemp.y >= 0 && ptTemp.y < nHeight)
                        {
                            if (pImageData[nWidthStep * ptTemp.y + ptTemp.x] == 0)
                            {
                                bTracing = true;
                                ptCurrent.x = x;
                                ptCurrent.y = y;
                                pChainCode->push_back(k);
                                break;
                            }
                        }
                    }
                }
            }
            if (bTracing)
            {
                // 如果当前点为轮廓起点
                if (pStart->x == ptCurrent.x && pStart->y == ptCurrent.y)
                {
                    // 则跟踪完毕
                    bTracing = false;
                }
                break;
            }
        }
        k += 0x06;
    }
    return true;
}
// 轮廓绘制
// 1. pImageData   图像数据
// 2. nWidth       图像宽度
// 3. nHeight      图像高度
// 4. nWidthStep   图像行大小
// 5. ptStart      起始点
// 6. ChainCode    链码表
/*******************************************************************************
  Function		: DrawContour      
  Description	: draw Contour
  Input			: pImageData, nWidth, nHeight, nWidthStep, pStart, pChainCode         
  Output		: pStart, ivChainCode     
  Return		: None      
*******************************************************************************/ 		  
bool DrawContour(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep,
    POINT ptStart, ivChainCode ChainCode)
{
    POINT ptCurrent = { 0, 0 };
    const POINT ptOffset[8] =
    {
        { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 },
        { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }
    };
    // 清空图像
    memset(pImageData, 0, nWidthStep * nHeight);
    // 轮廓绘制
    ptCurrent.x = ptStart.x;
    ptCurrent.y = ptStart.y;
    pImageData[nWidthStep * ptCurrent.y + ptCurrent.x] = 0xFF;
    for (ivChainCode::iterator i = ChainCode.begin(); i != ChainCode.end(); i++)
    {
        ptCurrent.x += ptOffset[*i].x;
        ptCurrent.y += ptOffset[*i].y;
        pImageData[nWidthStep * ptCurrent.y + ptCurrent.x] = 0xFF;
    }
    return true;
}

/*********************************---------提取连通域轮廓---------*************************************/  

/*******************************************************************************************************************************/
/*******************************************************************************************************************************/
/***********************************************-------Add by LiYing------******************************************************/