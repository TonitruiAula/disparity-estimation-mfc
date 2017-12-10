#include "stdafx.h"
#include "CFrame.h"


CFrame::CFrame()
{
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
