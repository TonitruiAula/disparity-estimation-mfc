#pragma once
#include "CCalibrater.h"
#include "CameraDS.h"



class CStereoCaliData
{
public:
	cv::Mat R, T, E, F;
	cv::Size imgSize;
	CCalibrater *pL, *pR;
	void setCalibrater(CCalibrater *pL, CCalibrater *pR) { this->pL = pL; this->pR = pR; }
	double error;
	//bool getData(CCameraDS* pCam, bool fullCheck = false);
	bool getData(bool fullCheck = false);
	bool checkObjPoints(bool fullCheck = false);
	//bool getImgSize(CCameraDS * pCam, bool fromCam);
	//bool getImgSize(CCameraDS *pCam);
	CStereoCaliData();
	~CStereoCaliData();
	CStereoCaliData(CCalibrater *pL, CCalibrater *pR) { this->pL = pL; this->pR = pR; }
	void openFile(std::string path);
	void saveFile(std::string path);

};

