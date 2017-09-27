/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: FastMatchTemplate.cpp
  Author		: firefly     
  Version		: V1.0.0      
  Date			: 2017/06/3
  Description	: match template    
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/



#include <math.h>
#include "FastMatchTemplate.h"
#include"iostream"

using namespace std;

//=============================================================================
// Assumes that source image exists and numDownPyrs > 1, no ROIs for either 
//  image, and both images have the same depth and number of channels
bool
FastMatchTemplate( const IplImage&  source, 
                   const IplImage&  target,
                   vector<CvPoint>* foundPointsList,
                   vector<double>*  confidencesList,
                   int              matchPercentage,
                   bool             findMultipleTargets,
                   int              numMaxima,
                   int              numDownPyrs,
                   int              searchExpansion )
{
  // make sure that the template image is smaller than the source
  if( target.width > source.width || target.height > source.height )
  {
    printf( "\nSource image must be larger than target image.\n" );
    return false;
  }
  
  if( source.depth != target.depth )
  {
    printf( "\nSource image and target image must have same depth.\n" );
    return false;
  }
  
  if( source.nChannels != target.nChannels )
  {
    printf( 
       "\nSource image and target image must have same number of channels.\n" );
    return false;
  }
  
  CvSize sourceSize = cvGetSize( &source );
  CvSize targetSize = cvGetSize( &target );
  
  int depth = source.depth;
  int numChannels = source.nChannels;
  
  // create copies of the images to modify
  IplImage* copyOfSource = cvCloneImage( &source );
  IplImage* copyOfTarget = cvCloneImage( &target );
  
//  std::cout<<"numDownPyrs = "<<numDownPyrs<<std::endl;
  // down pyramid the images
  double time1 = cvGetTickCount();
  for( int ii = 0; ii < numDownPyrs; ii++ )
  {
    // start with the source image
    sourceSize.width /= 2;
    sourceSize.height /= 2;
    
    IplImage* smallSource = NULL;
    smallSource = cvCreateImage( sourceSize, depth, numChannels );
    cvPyrDown( copyOfSource, smallSource, CV_GAUSSIAN_5x5 );
    
    // prepare for next loop, if any
    cvReleaseImage( &copyOfSource );
    copyOfSource = cvCloneImage( smallSource );
    cvReleaseImage( &smallSource );
    
    // next, do the target    
    targetSize.width /= 2;
    targetSize.height /= 2;
    
    IplImage* smallTarget = NULL;
    smallTarget = cvCreateImage( targetSize, depth, numChannels );
    cvPyrDown( copyOfTarget, smallTarget, CV_GAUSSIAN_5x5 );
    
    // prepare for next loop, if any
    cvReleaseImage( &copyOfTarget );
    copyOfTarget = cvCloneImage( smallTarget );
    cvReleaseImage( &smallTarget );
  }
 time1  = (cvGetTickCount() - time1) / cvGetTickFrequency() / 1000  ;
 // printf(" pydown time1 is : %f ms\n",time1);
  // perform the match on the shrunken images
  CvSize smallTargetSize = cvGetSize( copyOfTarget );
  CvSize smallSourceSize = cvGetSize( copyOfSource );
  
  CvSize resultSize;
  resultSize.width = smallSourceSize.width - smallTargetSize.width + 1;
  resultSize.height = smallSourceSize.height - smallTargetSize.height + 1;
  
  IplImage* result = cvCreateImage( resultSize, IPL_DEPTH_32F, 1 );
  double time2 = cvGetTickCount();
  //for(int t=0;t<10;t++)
  //{
  cvMatchTemplate( copyOfSource, copyOfTarget, result, CV_TM_CCORR_NORMED );
  //}
  time2  = (cvGetTickCount() - time2) / cvGetTickFrequency() / 1000  ;
 // printf(" spend time1 is : %f ms\n",time2);
  
  // release memory we don't need anymore
  cvReleaseImage( &copyOfSource );
  cvReleaseImage( &copyOfTarget );
  
  // find the top match locations
  CvPoint* locations = NULL;
  MultipleMaxLoc( *result, &locations, numMaxima );//locations中存储的是最可能为目标的四个点
 
  float maxLowScaleValue = cvGet2D(result,locations[0].y,locations[0].x).val[0];
  maxLowScaleValue *= 100;
//  cout<<"maxLowScaleValue = "<<maxLowScaleValue<<endl;
  if(maxLowScaleValue < matchPercentage)
  {
//	  printf("no target\n");
	  cvReleaseImage( &result );
	  return true;
  }
  else
  {
	  double time2 = cvGetTickCount();
	  cvReleaseImage( &result );
	  sourceSize = cvGetSize( &source );
	  targetSize = cvGetSize( &target );
	  IplImage* searchImage = cvCloneImage( &source );
	  for( int currMax = 0; currMax < numMaxima; currMax++ )
	  { 
		  locations[currMax].x *= ( int )pow( 2, numDownPyrs );   //恢复到最底层模板坐标
		  locations[currMax].y *= ( int )pow( 2, numDownPyrs );
		  locations[currMax].x += targetSize.width / 2;   //resultSize.height = smallSourceSize.height - smallTargetSize.height + 1;
		  locations[currMax].y += targetSize.height / 2;  
		  const CvPoint& searchPoint = locations[currMax];
		  if( findMultipleTargets && !foundPointsList->empty() )
		  {
		      bool thisTargetFound = false;
		      int numPoints = foundPointsList->size();
		      for( int currPoint = 0; currPoint < numPoints; currPoint++ )
		      {
			      const CvPoint& foundPoint = ( *foundPointsList )[currPoint];
			      if( abs( searchPoint.x - foundPoint.x ) <= searchExpansion * 2 && 
				  abs( searchPoint.y - foundPoint.y ) <= searchExpansion * 2 )
			      {
			          thisTargetFound = true;
			          break;
			       }
		      }
		      if( thisTargetFound )
		      {
			      continue;
		      }
		  }

		  CvRect searchRoi;
		  searchRoi.x = searchPoint.x - ( target.width ) / 2 - searchExpansion;
		  searchRoi.y = searchPoint.y - ( target.height ) / 2 - searchExpansion;
		  searchRoi.width = target.width + searchExpansion * 2;
		  searchRoi.height = target.height + searchExpansion * 2;
		  if( searchRoi.x < 0 )
		  {
		      searchRoi.x = 0;
		  }
		  if( searchRoi.y < 0 )
		  {
		      searchRoi.y = 0;
		  }
		  if( ( searchRoi.x + searchRoi.width ) > ( sourceSize.width - 1 ) )
		  {
				int numPixelsOver = ( searchRoi.x + searchRoi.width ) - ( sourceSize.width - 1 );  
				searchRoi.width -= numPixelsOver;
		  }
		  if( ( searchRoi.y + searchRoi.height ) > ( sourceSize.height - 1 ) )
		  {
				int numPixelsOver = ( searchRoi.y + searchRoi.height ) - ( sourceSize.height - 1 );
				searchRoi.height -= numPixelsOver;
		  }

		  cvSetImageROI( searchImage, searchRoi );
		  resultSize.width = searchRoi.width - target.width + 1;
		  resultSize.height = searchRoi.height - target.height + 1;
		  result = cvCreateImage( resultSize, IPL_DEPTH_32F, 1 );

		  cvMatchTemplate( searchImage, &target, result, CV_TM_CCORR_NORMED );
		  cvResetImageROI( searchImage );
    
		  double minValue, maxValue;
		  CvPoint minLoc, maxLoc;
		  cvMinMaxLoc( result, &minValue, &maxValue, &minLoc, &maxLoc );
		  maxValue *= 100;
    
		// transform point back to original image
		  maxLoc.x += searchRoi.x + target.width / 2;
		  maxLoc.y += searchRoi.y + target.height / 2;
    
		  cvReleaseImage( &result );
    
		  if( maxValue >= matchPercentage )
		  {
		      foundPointsList->push_back( maxLoc );
		      confidencesList->push_back( maxValue );     
		      if( !findMultipleTargets )
		      {
			      break;
		      }
		  }
	  }

	  if( foundPointsList->empty() )
	  {
//		  printf( "\nTarget was not found to required confidence of %d.\n",matchPercentage );
		  delete [] locations;
	      cvReleaseImage( &searchImage );
	      return true;
	  }
	  else
	  {
	      delete [] locations;
	      cvReleaseImage( &searchImage ); 
	      return true;
	  }
  }
}

//=============================================================================

void MultipleMaxLoc( const IplImage& image, 
                     CvPoint**       locations,
                     int             numMaxima  )
{
  // initialize input variable locations
  *locations = new CvPoint[numMaxima];
  
  // create array for tracking maxima
  double maxima[MAXFINDNUMBER];
  for( int i = 0; i < numMaxima; i++ )
  {
    maxima[i] = 0.0;
  }
  
  // extract the raw data for analysis
  float* data;
  int step;
  CvSize size;
  
  cvGetRawData( &image, ( uchar** )&data, &step, &size );
  
  step /= sizeof( data[0] );
  
  for( int y = 0; y < size.height; y++, data += step )
  {
     for( int x = 0; x < size.width; x++ )
     {
      // insert the data value into the arry if it is greater than any of the
      //  other array values, and bump the other values below it, down
          for( int j = 0; j < numMaxima; j++ )
          {
              if( data[x] > maxima[j] ) //如果data[x]比maxima[j]大，则将data[x]插入到maxima[j]的位置，其它maxima[j]均后移一位
              {
                  // move the maxima down
                  for( int k = numMaxima - 1; k > j; k-- )
                  {
                      maxima[k] = maxima[k-1];
                      ( *locations )[k] = ( *locations )[k-1];
                  }      
                   // insert the value
                  maxima[j] = ( double )data[x];
                  ( *locations )[j].x = x;
                  ( *locations )[j].y = y;
                  break;
              }
          }
     }
  }
}

//=============================================================================

void
DrawFoundTargets( IplImage*              image,
                  const CvSize&          size,
                  const vector<CvPoint>& pointsList,
                  const vector<double>&  confidencesList,
                  int                    red,
                  int                    green,
                  int                    blue )
{
  int numPoints = pointsList.size();
  for( int currPoint = 0; currPoint < numPoints; currPoint++ )
  {
    const CvPoint& point = pointsList[currPoint];
    
    // write the confidences to stdout
    printf( "\nTarget found at (%d, %d)\n, with confidence = %3.3f %%.\n", 
            point.x, 
            point.y, 
            confidencesList[currPoint] );
    
    // draw a circle at the center
    cvCircle( image, point, 2, CV_RGB( red, green, blue ) );
    
    // draw a rectangle around the found target
    CvPoint topLeft;
    topLeft.x = point.x - size.width / 2;
    topLeft.y = point.y - size.height / 2;
    
    CvPoint bottomRight;
    bottomRight.x = point.x + size.width / 2;
    bottomRight.y = point.y + size.height / 2;
    
    cvRectangle( image, topLeft, bottomRight, CV_RGB( red, green, blue ) );
  }
}
