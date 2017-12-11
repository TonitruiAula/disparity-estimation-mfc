#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include <vector>
#include <string>
#include "CCalibrationData.h"

class CCalibrater
{
public:
	int nWidth;
	int nHeight;
	float lWidth;
	float lHeight;
	int imageNum;
	cv::Size imgSize;
	std::vector<std::vector<cv::Point2f>> cornersVect;
	std::vector<std::vector<cv::Point3f>> worldPointsVect;  	
	void shot(cv::Mat frame, bool show = true);
	void calibrate();
	void calibrateByPics(std::vector<std::string> filenames);
	CCalibrationData* pData;
	CCalibrater();
	CCalibrater(int nw, int nh, float lw, float lh);
	~CCalibrater();
	void openFile(std::string path);
	void saveFile(std::string path);
};

