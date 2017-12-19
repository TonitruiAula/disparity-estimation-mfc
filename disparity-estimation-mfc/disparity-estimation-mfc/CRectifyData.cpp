#include "stdafx.h"
#include "CRectifyData.h"
#include <cmath>

bool CRectifyData::getData(double alpha)
{
	CCalibrater* pL = pS->pL;
	CCalibrater* pR = pS->pR;
	if (pL == NULL || pR == NULL || pL->pData == NULL || pR->pData == NULL || pS == NULL)
	{
		AfxMessageBox(_T("之前步骤未完成！"));
		return false;
	}
	cv::stereoRectify(pL->pData->cameraMatirx, pL->pData->distCoeffs, pR->pData->cameraMatirx, pR->pData->distCoeffs, pS->imgSize, pS->R, pS->T, R1, R2, P1, P2, Q, CV_CALIB_ZERO_DISPARITY, alpha, cv::Size(), &roi1, &roi2);
	Q.at<double>(3, 2) = -Q.at<double>(3, 2);
	cv::initUndistortRectifyMap(pL->pData->cameraMatirx, pL->pData->distCoeffs, R1, P1, pS->imgSize, CV_32FC1, mx1, my1);
	cv::initUndistortRectifyMap(pR->pData->cameraMatirx, pR->pData->distCoeffs, R2, P2, pS->imgSize, CV_32FC1, mx2, my2);
	return true;
}

CRectifyData::CRectifyData()
{
}


CRectifyData::~CRectifyData()
{
}

void CRectifyData::openFile(std::string path)
{
	if(pS == NULL)
		pS = new CStereoCaliData;
	pS->openFile(path);
	cv::FileStorage fs(path, cv::FileStorage::READ);
	fs["R1"] >> R1;
	fs["R2"] >> R2;
	fs["P1"] >> P1;
	fs["P2"] >> P2;
	fs["mx1"] >> mx1;
	fs["mx2"] >> mx2;
	fs["my1"] >> my1;
	fs["my2"] >> my2;
	fs["Q"] >> Q;
	fs["roi1"] >> roi1;
	fs["roi2"] >> roi2;
	fs.release();
}

void CRectifyData::saveFile(std::string path)
{
	cv::FileStorage fs(path, cv::FileStorage::WRITE);
	cv::Mat l_mtx = pS->pL->pData->cameraMatirx;
	cv::Mat l_dist = pS->pL->pData->distCoeffs;
	cv::Mat r_mtx = pS->pR->pData->cameraMatirx;
	cv::Mat r_dist = pS->pR->pData->distCoeffs;
	fs << "l_mtx" << l_mtx << "l_dist" << l_dist;
	fs << "r_mtx" << r_mtx << "r_dist" << r_dist;
	fs << "R" << pS->R << "T" << pS->T << "E" << pS->E << "F" << pS->F << "imgSize" << pS->imgSize;
	fs << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q << "roi1" << roi1 << "roi2" << roi2;
	fs << "mx1" << mx1 << "my1" << my1 << "mx2" << mx2 << "my2" << my2;
	fs << "l_cornersVect" << pS->pL->cornersVect << "l_worldPointsVect" << pS->pL->worldPointsVect;
	fs << "r_cornersVect" << pS->pR->cornersVect << "r_worldPointsVect" << pS->pR->worldPointsVect;
	fs.release();
}

void CRectifyData::showEffect(cv::Mat lFrame, cv::Mat rFrame)
{
	double p1 = P2.at<double>(1, 3);
	double p0 = P2.at<double>(0, 3);
	cv::Size imageSize = pS->imgSize;
	cv::resize(lFrame, lFrame, imageSize);
	cv::resize(rFrame, rFrame, imageSize);
	bool verti = p1 > p0;
	cv::Mat pair;
	if (verti)
		pair = cv::Mat::zeros(imageSize.height * 2, imageSize.width, CV_8UC3);
		//pair = cv::Mat(imageSize.height * 2, imageSize.width, CV_8UC3);
	else
		pair = cv::Mat::zeros(imageSize.height, imageSize.width * 2, CV_8UC3);
		//pair = cv::Mat(imageSize.height, imageSize.width * 2, CV_8UC3);
	cv::Mat newL, newR;
	cv::remap(lFrame, newL, mx1, my1, cv::INTER_LANCZOS4);
	cv::remap(rFrame, newR, mx2, my2, cv::INTER_LANCZOS4);
	//cv::imshow("lframe", lFrame);
	//cv::imshow("newL", newL);
	if (!verti)
	{
		cv::Mat roi1 = pair(cv::Rect(0, 0, imageSize.width, imageSize.height));
		cv::Mat roi2 = pair(cv::Rect(imageSize.width, 0, imageSize.width, imageSize.height));
		newL.copyTo(roi1);
		newR.copyTo(roi2);
		for (int i = 0; i < imageSize.height; i += 16)
		{
			cv::line(pair, cv::Point(0, i), cv::Point(imageSize.width * 2, i), CV_RGB(0, 255, 0));
		}
	}
	else
	{
		cv::Mat roi1 = pair(cv::Rect(0, 0, imageSize.width, imageSize.height));
		cv::Mat roi2 = pair(cv::Rect(0, imageSize.height, imageSize.width, imageSize.height));
		newL.copyTo(roi1);
		newR.copyTo(roi2);
		for (int i = 0; i < imageSize.width; i += 16)
		{
			cv::line(pair, cv::Point(i, 0), cv::Point(i, imageSize.height * 2), CV_RGB(0, 255, 0));
		}
	}
	cv::imshow("effect", pair);
}
