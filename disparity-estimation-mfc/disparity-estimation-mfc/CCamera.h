#pragma once

#include "stdafx.h"
#include "afxdialogex.h"
#include "CFrame.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
class CCamera
{
public:
	cv::VideoCapture *m_pCapture;
	CFrame *m_pFrame;

	CCamera();
	CCamera(CWnd *pw);
	~CCamera();
	void readFrame();
};

