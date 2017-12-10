#include "stdafx.h"
#include "CCamera.h"


CCamera::CCamera()
{
	m_pCapture = NULL;
}

CCamera::CCamera(CWnd *pw)
{
	m_pFrame = new CFrame(pw,320,240);
}


CCamera::~CCamera()
{
	delete m_pFrame;
}

void CCamera::readFrame()
{
	cv::Mat f;
	(*m_pCapture) >> f;
	m_pFrame->frame = f;
}
