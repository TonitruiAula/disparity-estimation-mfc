#pragma once
#include "CCalibrationData.h"
#include "CStereoCaliData.h"

class CRectifyData
{
public:
	CStereoCaliData *pS;
	cv::Mat R1, R2, P1, P2, Q;
	cv::Rect roi1, roi2;
	cv::Mat mx1, mx2, my1, my2;
	bool getData(double alpha);
	void setStereoData(CStereoCaliData *pS) { this->pS = pS; }
	CRectifyData();
	CRectifyData(CStereoCaliData *pS) { this->pS = pS; }
	~CRectifyData();
	void openFile(std::string path);
	void saveFile(std::string path);
	void showEffect(cv::Mat lFrame, cv::Mat rFrame, bool chessBoard = false, int nWIdth = 0, int nHeight = 0);
};

