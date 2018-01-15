// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <opencv\cv.h>
#include <opencv2\highgui.hpp>

#include <iostream> // std::cerr

#include "ObjectRecognition.hpp"

#ifdef NDEBUG_
	#pragma comment(lib, "opencv_world340d.lib")
#else
	#pragma comment(lib, "opencv_world340.lib")
#endif // NDEBUG_

int FindCircles(std::string_view file, double minDist /* = 50 */, int minR /* = 10 */, int maxR /* = 40 */)
{
	std::string filename(file);
	IplImage *pImage = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	if (!pImage)
	{
		std::cerr << "Cannot load image(1): " << filename << std::endl;

		return -1;
	}

	CvMemStorage *pStorage = cvCreateMemStorage();
	CvSeq        *pCircles = cvHoughCircles(pImage, pStorage, CV_HOUGH_GRADIENT, 10, minDist, 100, 100, minR, maxR);

	cvReleaseImage(&pImage);
	pImage = cvLoadImage(filename.data());
	if (!pImage)
	{
		std::cerr << "Cannot load image(2): " << filename << std::endl;

		cvReleaseMemStorage(&pStorage);
		cvReleaseImage(&pImage);

		return -1;
	}

	for (auto i = 0; i < pCircles->total; ++i)
	{
		float *pCircle = reinterpret_cast<float*>(cvGetSeqElem(pCircles, i));

		cvCircle(pImage, CvPoint(cvRound(pCircle[0]), cvRound(pCircle[1])), cvRound(pCircle[2]), CV_RGB(0, 255, 0), 2);
	}

	int num = pCircles->total;
	cvSaveImage(filename.data(), pImage);

	cvReleaseMemStorage(&pStorage);
	cvReleaseImage(&pImage);

	return num;
}