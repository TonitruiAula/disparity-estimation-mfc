
// disparity-estimation-mfcDlg.h: 头文件
//

#pragma once
#include "CCamera.h"
#include "CameraDS.h"

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
	CCameraDS* m_pLCamera;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
