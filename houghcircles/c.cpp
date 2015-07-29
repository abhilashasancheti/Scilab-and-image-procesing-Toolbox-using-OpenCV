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
  
  int opencv_blur(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    unsigned char *pstDataR = NULL;
    unsigned char *pstDataG = NULL;
    unsigned char *pstDataB = NULL;
    int i,j,k;
    double *ksize_width,*ksize_height,*anchorX,*anchorY;

    //checking input argument
    CheckInputArgument(pvApiCtx, 5, 5);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    
    //retrive address of the list
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }
    //retrive the matrix of the R values   
    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDataR);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //retrive address of the list
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }   
    //retrive the matrix of the G values 
    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDataG);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //retrive address of the list
    sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }   
    //retrive the matrix of the B values 
    sciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, piAddr, 3, &iRows, &iCols, &pstDataB);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //creating an image matrix with the no. of rows and columns we retrieved, and assigning it to be of the form 8-bit unsinged integers
    Mat image(iRows,iCols,CV_8UC3);

    /*Now that we have the 3 matrices(R,G,B), we need to assign those values to the pixels. The following code does this*/

    k=0;
    for(i=0;i<iRows;i++)
    {
        for(j=0;j<iCols;j++)
        {
          image.at<Vec3b>(i,j)[2]=pstDataR[k];
          image.at<Vec3b>(i,j)[1]=pstDataG[k];
          image.at<Vec3b>(i,j)[0]=pstDataB[k++];
        }
    }

    //for value of ksize_height
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols ,&ksize_height);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //for value of ksize_width
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows, &iCols ,&ksize_width);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    //for value of sigmaX
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols ,&anchorX);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //for value of sigmaY
    sciErr = getVarAddressFromPosition(pvApiCtx,5,&piAddr5);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr5, &iRows, &iCols ,&anchorY);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }   

    //applying checks on input parameters
    Mat new_image(image.rows,image.cols,CV_8UC3);
    if(ksize_height[0]<0)
    {
        sciprint("Positive Value Required for Height. 1 value was used instead");
        ksize_height[0]+=1;
    }
    if(ksize_width[0]<0)
    {
        sciprint("Positive Value Required for Width. 1 value was used instead");
        ksize_width[0]+=1;
    }

    //defining temporary size and point type variables to use in the function
    Size sz(ksize_height[0],ksize_width[0]);
    Point pt(anchorX[0],anchorY[0]);

    //void blur(InputArray src, OutputArray dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT )
    blur(image,new_image,sz,pt);

    //Since we need to pass a single pointer as an arguement, for a 2-D matrix, we define it in this manner
    unsigned char *r = (unsigned char *)malloc(new_image.rows * new_image.cols * sizeof(unsigned char));
    unsigned char *g = (unsigned char *)malloc(new_image.rows * new_image.cols * sizeof(unsigned char));
    unsigned char *b = (unsigned char *)malloc(new_image.rows * new_image.cols * sizeof(unsigned char));

     //The next block of code retrieves the RGB values at a specified pixel, and assigns it to the matrices
    for(i=0;i<new_image.rows;i++)
    {
        for(j=0;j<new_image.cols;j++)
        {
            Vec3b intensity = new_image.at<Vec3b>(i, j);
            *(r + i*new_image.cols + j)=intensity.val[2];
            *(g + i*new_image.cols + j)=intensity.val[1];
            *(b + i*new_image.cols + j)=intensity.val[0];
        }
    }

     //creating the list that will be passed to the Scilab enviroment
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &piAddrNew);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }


    //Adding the R value matrix to the list
    //Syntax : createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
    sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 1, new_image.rows, new_image.cols, r);
    free(r);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Adding the G value matrix to the list
    sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 2, new_image.rows, new_image.cols, g);
    free(g);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Adding the B value matrix to the list
    sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx)+1 , piAddrNew, 3, new_image.rows, new_image.cols, b);
    free(b);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}