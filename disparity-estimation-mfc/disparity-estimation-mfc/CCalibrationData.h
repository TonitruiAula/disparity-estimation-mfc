#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>

class CCalibrationData
{
public:
	cv::Mat cameraMatirx, distCoeffs;
	std::vector<cv::Mat> rvecs, tvecs;
	CCalibrationData();
	~CCalibrationData();
	CString toCString();
};

