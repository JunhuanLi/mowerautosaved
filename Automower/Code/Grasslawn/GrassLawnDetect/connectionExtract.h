
/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: connectionExtract.h
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

#include"stdio.h"
#include<vector>
#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace cv;

typedef struct _TopBandImage
{
	unsigned short imageWidth;
	unsigned short imageHeight;
	unsigned short depth;
	unsigned short nChannals;
	unsigned short widthStep;
	unsigned char *imagePtr;
}_TopBandImage;

typedef struct _ImageSize
{
	unsigned short imgWidth;
	unsigned short imgHeight;
}_ImageSize;

typedef struct _RunLengthEncode
{
	short row;               /* line number (row) of run                     */
	short startCol;              /* column index of beginning of run             */
	short endCol;              /* column index of ending of run                */
}_RunLengthEncode;

vector<vector<_RunLengthEncode> >connectRegion;

typedef struct _ConnectionRegion
{
	_RunLengthEncode *connectRegion;
	unsigned int rowNumber;
}_ConnectionRegion;

void CopyTopBandImage(_TopBandImage *inputImage, _TopBandImage *outputImage);

extern void fillRunVectors(const Mat& bwImage, int& NumberOfRuns, vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun);
extern void firstPass(vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun, int NumberOfRuns,vector<int>& runLabels, vector<pair<int, int>>& equivalences, int offset);
extern void replaceSameLabel(vector<int>& runLabels, vector<pair<int, int>>&equivalence);

#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/