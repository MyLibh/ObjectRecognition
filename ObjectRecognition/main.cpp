// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MyTypedefs.hpp"

#include <opencv\cv.h>
#include <opencv2\highgui.hpp>

#include <cmath>
#include <iostream>

CONST DOUBLE MIN_RADIUS = 10,
			 MAX_RADIUS = 40,
             MIN_DIST   = 50;

int main()
{
	IplImage *pImage = cvLoadImage("Test.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if(!pImage)
	{
		std::cerr << "Cannot load image for finding\n";

		return FALSE;
	}

	CvMemStorage *pStorage = cvCreateMemStorage();
	CvSeq        *pCircles = cvHoughCircles(pImage, pStorage, CV_HOUGH_GRADIENT, 10, MIN_DIST, 100, 100, MIN_RADIUS, MAX_RADIUS);
	
	cvReleaseImage(&pImage);
    pImage = cvLoadImage("Test.jpg");

	for(auto i = 0; i < pCircles->total; ++i)
	{
		FLOAT *pCircle = reinterpret_cast<FLOAT*>(cvGetSeqElem(pCircles, i));

		cvCircle(pImage, CvPoint(cvRound(pCircle[0]), cvRound(pCircle[1])), cvRound(pCircle[2]), CV_RGB(0, 255, 0), 2);
	}

	cvNamedWindow("Result");
	cvShowImage("Result", pImage);

	cvWaitKey(0);

	cvReleaseMemStorage(&pStorage);
	cvReleaseImage(&pImage);
	cvDestroyAllWindows();

	return 0;
}