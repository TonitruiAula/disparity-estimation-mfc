#include "stdafx.h"
#include "CFrame.h"


CFrame::CFrame()
{

}

CFrame::CFrame(int w, int h)
{
	frame = cv::Mat(w,h,CV_32FC3);
	width = w;
	height = h;
}

CFrame::CFrame(CWnd * pw, int w, int h)
{
	frame = cv::Mat(w, h, CV_32FC3);
	width = w;
	height = h;
	pwnd = pw;
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

}

CFrame::CFrame(CWnd * pw)
{
	pwnd = pw;
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

}


CFrame::~CFrame()
{
}
