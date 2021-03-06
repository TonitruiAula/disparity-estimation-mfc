
// disparity-estimation-mfc.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include "CvvImage.h"


// CdisparityestimationmfcApp:
// 有关此类的实现，请参阅 disparity-estimation-mfc.cpp
//

class CdisparityestimationmfcApp : public CWinApp
{
public:
	CdisparityestimationmfcApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CdisparityestimationmfcApp theApp;
