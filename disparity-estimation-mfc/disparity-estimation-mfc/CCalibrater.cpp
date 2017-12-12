#include "stdafx.h"
#include "CCalibrater.h"


void CCalibrater::shot(cv::Mat frame, bool show)
{
	imgSize = frame.size();
	cv::Mat gray;
	cv::cvtColor(frame, gray, CV_BGR2GRAY);
	std::vector<cv::Point2f> corners;
	std::vector<cv::Point3f> worldPoints;
	cv::Size patternSize(nWidth, nHeight);
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			worldPoints.push_back(cv::Point3f(j*lWidth, i*lHeight, 0));
		}
	}
	if (cv::findChessboardCorners(gray, patternSize, corners))
	{
		cv::cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1)
			, cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.001));
		cv::Mat img;
		frame.copyTo(img);
		cv::drawChessboardCorners(img, patternSize, corners, true);
		if (show)
			cv::imshow("corners", img);
		cornersVect.push_back(corners);
		worldPointsVect.push_back(worldPoints);
	}
}

void CCalibrater::calibrate()
{
	if (cornersVect.size() > 0 && worldPointsVect.size() > 0)
	{
		pData = new CCalibrationData;
		cv::calibrateCamera(worldPointsVect, cornersVect, imgSize, pData->cameraMatirx, pData->distCoeffs, pData->rvecs, pData->tvecs);
	}
}

void CCalibrater::calibrateByPics(std::vector<std::string> filenames)
{
	cv::Mat frame;
	for (int i = 0; i < filenames.size(); i++)
	{
		frame = cv::imread(filenames[i]);
		shot(frame, false);
	}
	calibrate();
}

CCalibrater::CCalibrater()
{
	nWidth = nHeight = 0;
	lWidth = lHeight = 0.0f;
	imageNum = 0;
}

CCalibrater::CCalibrater(int nw, int nh, float lw, float lh)
{
	nWidth = nw;
	nHeight = nh;
	lWidth = lw;
	lHeight = lh;
	imageNum = 0;
}


CCalibrater::~CCalibrater()
{
}

void CCalibrater::openFile(std::string path)
{
	cv::FileStorage fs(path, cv::FileStorage::READ);
	if (pData == NULL)
		pData = new CCalibrationData;
	fs["mtx"] >> pData->cameraMatirx;
	fs["dist"] >>pData->distCoeffs;
	fs.release();

}

void CCalibrater::saveFile(std::string path)
{
	cv::FileStorage fs(path, cv::FileStorage::WRITE);
	cv::Mat mtx = pData->cameraMatirx;
	cv::Mat dist = pData->distCoeffs;
	fs << "mtx" << mtx << "dist" << dist;
	fs.release();

}
