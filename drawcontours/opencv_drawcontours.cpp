/********************************************************
    Author: Abhilasha Sancheti & Sukul Bagai
*********************************************************
   return_image = drawcontours(input_image, InputArrayOfArrays contours, int contourIdx, int r_value,int g
   _value , int b_value, int thickness, int lineType, InputArray hierarchy=noArray(), int maxLevel, double offset_x,double offset_y)
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_drawcontours(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piLen2=NULL;
    int *piLen3 = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k;
    int *pstData1=NULL;
    int *pstData2=NULL;
    double off_x,off_y,thickness,linetype,r_value,g_value,b_value,maxlevel,contour_id;
   

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
    Mat src;
    retrieveImage(src,1);
       
     // to retrieve the contours
     sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
             //checking for error if any
     if (sciErr.iErr)
     {
      printError(&sciErr, 0);
      return 0;
     } 
     sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols,&pstData);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    } 
     

    int rows = iRows;
    vector<vector<Point> > cont1;
    for ( i = 0 ; i < iRows ; i++)
    {
        for(j=0;j < cont1[i].size() ; j++)
       {
          Point pt(*(pstData + 0*iRows + j),*(pstData + 1*iRows + j));
          cont1[i].push_back(pt);
      }
    }
    // for contourId parameter of findContours
     sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
             //checking for error if any
     if (sciErr.iErr)
     {
      printError(&sciErr, 0);
      return 0;
     }
     intErr = getScalarDouble(pvApiCtx, piAddr3 ,&contour_id);
     if(intErr)
       return intErr;   
   
     // for r value of the colour
      sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
             //checking for error if any
       if (sciErr.iErr)
       {
        printError(&sciErr, 0);
        return 0;
       }
        intErr = getScalarDouble(pvApiCtx, piAddr4,&r _value);
        if(intErr)
       return intErr;

          // for g value of the colour
        sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        intErr = getScalarDouble(pvApiCtx, piAddr5,&g_value);
        if(intErr)
       return intErr;
  
       // for b value of the colour
      sciErr = getVarAddressFromPosition(pvApiCtx,6,&piAddr6);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        intErr = getScalarDouble(pvApiCtx, piAddr6,&b_value);
        if(intErr)
       return intErr;
  
/// to retrieve the thickness parameter of function
        sciErr = getVarAddressFromPosition(pvApiCtx,7,&piAddr7);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr7 ,&thickness);
    if(intErr)
       return intErr;
/// to retrieve the linetype parameter of function
         sciErr = getVarAddressFromPosition(pvApiCtx,8,&piAddr8);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr8 ,&linetype);
    if(intErr)
       return intErr;

/// to retrieve the maxlevel parameter of function
         sciErr = getVarAddressFromPosition(pvApiCtx,9,&piAddr9);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr9 ,&maxlevel);
    if(intErr)
       return intErr;     

/// to retrieve the x coordinate of offset parameter of function
         sciErr = getVarAddressFromPosition(pvApiCtx,10,&piAddr10);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr10 ,&off_x);
    if(intErr)
       return intErr;     

/// to retrieve the y coordinate of offset parameter of function
         sciErr = getVarAddressFromPosition(pvApiCtx,11,&piAddr11);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr11 ,&off_y);
    if(intErr)
       return intErr;     
/// validating parameters
     if( r_value <0 || r_value >255)
     {
          r_value=0;
          sciprint(" r value of colour should be between 0 and 255 , using 0 instead");
     }
     if( g_value <0 || g_value >255)
     {
          g_value=0;
          sciprint("  g value of colour should be between 0 and 255 , using 0 instead");
     }
    if( b_value <0 || b_value >255)
     {
          b_value=0;
          sciprint(" b value of colour should be between 0 and 255 , using 0 instead");
     }
    if ((linetype!=0) && (linetype!=4)&& (linetype!=8))
    {
             linetype=8;
             sciprint("Only 0/4/8 allowed , using 8 instead");
     }   
    
    Point pt(x,y);
    /// Find contours  
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( src, contours, hierarchy, findmode, findmethod, pt);

    //Assigning the list as the Output Variable
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, contours.size(), &piAddrNew);
    if(sciErr.iErr)
    {
       printError(&sciErr, 0);           
       return 0;
    }

    for(i=0;i<contours.size();i++)
    {
        double *arr = (double *)malloc(sizeof(double) * 2 * contours[i].size());

        for(j=0;j<contours[i].size();j++)
        {
            *(arr + j*2 + 0) = contours[i][j].x;
            *(arr + j*2 + 1) = contours[i][j].y;
        }

        sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, i+1, contours[i].size(), 2, arr);
        free(arr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx); 
    return 0;


 }

}