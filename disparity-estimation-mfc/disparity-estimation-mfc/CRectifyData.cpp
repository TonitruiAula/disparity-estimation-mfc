#include "stdafx.h"
#include "CRectifyData.h"


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
	pS = new CStereoCaliData;
	pS->openFile(path);
	cv::FileStorage fs(path, cv::FileStorage::READ);
	fs["R1"] >> R1;
	fs["R2"] >> R2;
	fs["P1"] >> P1;
	fs["P2"] >> P2;
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
	fs << "l_cornersVect" << pS->pL->cornersVect << "l_worldPointsVect" << pS->pL->worldPointsVect;
	fs << "r_cornersVect" << pS->pR->cornersVect << "r_worldPointsVect" << pS->pR->worldPointsVect;
	fs.release();
}
