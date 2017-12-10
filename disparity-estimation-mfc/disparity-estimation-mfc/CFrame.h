#pragma once

#include "stdafx.h"
#include "afxdialogex.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
class CFrame
{
public:
	cv::Mat frame;
	int width;
	int height;
	CRect rect;
	CDC *pDC;
	HDC hDC;
	CWnd *pwnd;

	CFrame();
	CFrame(int w, int h);
	CFrame(CWnd *pw);
	CFrame(CWnd *pw, int w, int h);
	~CFrame();
};

