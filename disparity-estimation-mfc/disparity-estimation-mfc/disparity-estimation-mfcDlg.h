
// disparity-estimation-mfcDlg.h: 头文件
//

#pragma once
#include <vector>
#include <string>
#include <cstring>
#include "CCamera.h"
#include "CameraDS.h"
//#include "CCalibrater.h"
//#include "CStereoCaliData.h"
#include "CRectifyData.h"

// CdisparityestimationmfcDlg 对话框
class CdisparityestimationmfcDlg : public CDialog
{
// 构造
public:
	CdisparityestimationmfcDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPARITYESTIMATIONMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CFrame* m_pLFrame;
	CFrame* m_pRFrame;
	CCameraDS* m_pLCamera;
	CCameraDS* m_pRCamera;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	std::vector<CString> camera_names;

	void loadCamNames();
	afx_msg void OnCbnSelchangeSelLcam();
	afx_msg void OnCbnSelchangeSelRcam();
	bool m_fromCam;
	afx_msg void OnBnClickedFromcam();
	afx_msg void OnBnClickedFrompic();
	bool m_cali;
	CCalibrater* m_pLCali;
	CCalibrater* m_pRCali;
	afx_msg void OnNMCustomdrawSliderNumw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderNumh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnBnClickedStartCali();
	afx_msg void OnBnClickedShot();
	afx_msg void OnBnClickedCaliRst();
	afx_msg void OnBnClickedSaveLcali();
	afx_msg void OnBnClickedOpenLcali();
	afx_msg void OnBnClickedSaveRcali();
	afx_msg void OnBnClickedOpenRcali();
	CStereoCaliData* m_pSCData;
	CRectifyData* m_pRData;
	afx_msg void OnBnClickedStartScali();
	afx_msg void OnBnClickedOpenScali();
	afx_msg void OnBnClickedSaveScali();
	afx_msg void OnBnClickedStartRect();
	afx_msg void OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOpenRect();
	afx_msg void OnBnClickedSaveRect();
	afx_msg void OnBnClickedVerifyPic();
	afx_msg void OnBnClickedVerifyCam();
	afx_msg void OnNMCustomdrawSliderSadsize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderNdisp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderUniratio(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCalLdis();
	cv::Mat calDisparity(cv::Mat lFrame, cv::Mat rFrame);
	CFrame* m_pDFrame;
	afx_msg void OnBnClickedSaveDisparityPic();
	afx_msg void OnBnClickedSaveDisparityTxt();
	void saveDisp(std::string filename);
};
