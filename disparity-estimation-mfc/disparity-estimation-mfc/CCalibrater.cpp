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

double CCalibrater::calibrate()
{
	double rst = -1;
	if (cornersVect.size() > 0 && worldPointsVect.size() > 0)
	{
		pData = new CCalibrationData;
		rst = cv::calibrateCamera(worldPointsVect, cornersVect, imgSize, pData->cameraMatirx, pData->distCoeffs, pData->rvecs, pData->tvecs);
	}
	return rst;
}

double CCalibrater::calRPM()
{
	double err = 0;
	double total_err = 0;
	for (int i = 0; i < cornersVect.size(); i++)
	{
		std::vector<cv::Point2f> imgpoints, imgpoints2;
		cv::projectPoints(worldPointsVect[i], pData->rvecs[i], pData->tvecs[i], pData->cameraMatirx, pData->distCoeffs, imgpoints2);
		imgpoints = cornersVect[i];
		cv::Mat imgpointsMat = cv::Mat(1, imgpoints.size(), CV_32FC2);
		cv::Mat imgpoints2Mat = cv::Mat(1, imgpoints2.size(), CV_32FC2);
		for (int j = 0; j < imgpoints.size(); j++)
		{
			imgpointsMat.at<cv::Vec2f>(0, j) = cv::Vec2f(imgpoints[j].x, imgpoints[j].y);
			imgpoints2Mat.at<cv::Vec2f>(0, j) = cv::Vec2f(imgpoints2[j].x, imgpoints2[j].y);
		}
		err = cv::norm(imgpoints2Mat, imgpointsMat, cv::NORM_L2);
		total_err += err;
	}
	total_err /= cornersVect.size();
	return total_err;
}

double CCalibrater::calibrateByPics(std::vector<std::string> filenames)
{
	cv::Mat frame;
	for (int i = 0; i < filenames.size(); i++)
	{
		frame = cv::imread(filenames[i]);
		imgSize = frame.size();
		//shot(frame, true);
		shot(frame, false);
	}
	double rst = calibrate();
	return rst;
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
	//if (pData == NULL)
	pData = new CCalibrationData;
	fs["mtx"] >> pData->cameraMatirx;
	fs["dist"] >> pData->distCoeffs;
	fs["imgSize"] >> imgSize;
	fs["cornersVect"] >> cornersVect;
	fs["worldPointsVect"] >> worldPointsVect;
	fs.release();

}

void CCalibrater::saveFile(std::string path)
{
	cv::FileStorage fs(path, cv::FileStorage::WRITE);
	cv::Mat mtx = pData->cameraMatirx;
	cv::Mat dist = pData->distCoeffs;
	fs << "mtx" << mtx << "dist" << dist << "imgSize" << imgSize << "cornersVect" << cornersVect << "worldPointsVect" << worldPointsVect;
	fs.release();

}
