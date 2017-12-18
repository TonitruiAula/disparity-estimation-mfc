
// disparity-estimation-mfcDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "disparity-estimation-mfc.h"
#include "disparity-estimation-mfcDlg.h"
#include "afxdialogex.h"
#include "CCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdisparityestimationmfcDlg 对话框



CdisparityestimationmfcDlg::CdisparityestimationmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DISPARITYESTIMATIONMFC_DIALOG, pParent)
	,m_pLFrame(NULL),m_pLCamera(NULL)
	, m_pRCamera(NULL)
	, m_pRFrame(NULL)
	, m_fromCam(true)
	, m_cali(false)
	, m_pLCali(NULL)
	, m_pRCali(NULL)
	, m_pSCData(NULL)
	, m_pRData(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdisparityestimationmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdisparityestimationmfcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CdisparityestimationmfcDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CdisparityestimationmfcDlg::OnBnClickedStop)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_SEL_LCAM, &CdisparityestimationmfcDlg::OnCbnSelchangeSelLcam)
	ON_CBN_SELCHANGE(IDC_SEL_RCAM, &CdisparityestimationmfcDlg::OnCbnSelchangeSelRcam)
	ON_BN_CLICKED(IDC_FROMCAM, &CdisparityestimationmfcDlg::OnBnClickedFromcam)
	ON_BN_CLICKED(IDC_FROMPIC, &CdisparityestimationmfcDlg::OnBnClickedFrompic)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_NUMW, &CdisparityestimationmfcDlg::OnNMCustomdrawSliderNumw)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_NUMH, &CdisparityestimationmfcDlg::OnNMCustomdrawSliderNumh)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CdisparityestimationmfcDlg::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CdisparityestimationmfcDlg::OnEnChangeEditHeight)
	ON_BN_CLICKED(IDC_START_CALI, &CdisparityestimationmfcDlg::OnBnClickedStartCali)
	ON_BN_CLICKED(IDC_SHOT, &CdisparityestimationmfcDlg::OnBnClickedShot)
	ON_BN_CLICKED(IDC_CALI_RST, &CdisparityestimationmfcDlg::OnBnClickedCaliRst)
	ON_BN_CLICKED(IDC_SAVE_LCALI, &CdisparityestimationmfcDlg::OnBnClickedSaveLcali)
	ON_BN_CLICKED(IDC_OPEN_LCALI, &CdisparityestimationmfcDlg::OnBnClickedOpenLcali)
	ON_BN_CLICKED(IDC_SAVE_RCALI, &CdisparityestimationmfcDlg::OnBnClickedSaveRcali)
	ON_BN_CLICKED(IDC_OPEN_RCALI, &CdisparityestimationmfcDlg::OnBnClickedOpenRcali)
	ON_BN_CLICKED(IDC_START_SCALI, &CdisparityestimationmfcDlg::OnBnClickedStartScali)
	ON_BN_CLICKED(IDC_OPEN_SCALI, &CdisparityestimationmfcDlg::OnBnClickedOpenScali)
	ON_BN_CLICKED(IDC_SAVE_SCALI, &CdisparityestimationmfcDlg::OnBnClickedSaveScali)
	ON_BN_CLICKED(IDC_START_RECT, &CdisparityestimationmfcDlg::OnBnClickedStartRect)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &CdisparityestimationmfcDlg::OnNMCustomdrawSliderAlpha)
	ON_BN_CLICKED(IDC_OPEN_RECT, &CdisparityestimationmfcDlg::OnBnClickedOpenRect)
	ON_BN_CLICKED(IDC_SAVE_RECT, &CdisparityestimationmfcDlg::OnBnClickedSaveRect)
	ON_BN_CLICKED(IDC_VERIFY_PIC, &CdisparityestimationmfcDlg::OnBnClickedVerifyPic)
	ON_BN_CLICKED(IDC_VERIFY_CAM, &CdisparityestimationmfcDlg::OnBnClickedVerifyCam)
END_MESSAGE_MAP()


// CdisparityestimationmfcDlg 消息处理程序

BOOL CdisparityestimationmfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//m_pCamera = new CCamera(GetDlgItem(IDC_LFRAME));
	m_pLFrame = new CFrame(GetDlgItem(IDC_LFRAME));
	m_pRFrame = new CFrame(GetDlgItem(IDC_RFRAME));
	m_pLCamera = new CCameraDS;
	m_pRCamera = new CCameraDS;
	loadCamNames();
	m_fromCam = true;
	m_cali = false;
	((CButton*)GetDlgItem(IDC_FROMCAM))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_FROMPIC))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_SHOT))->EnableWindow(false);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_NUMW))->SetRange(2, 32);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_NUMW))->SetPos(10);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_NUMH))->SetRange(2, 32);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_NUMH))->SetPos(7);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA))->SetRange(0, 100);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA))->SetPos(0);

	m_pLCali = new CCalibrater;
	m_pRCali = new CCalibrater;
	m_pLCali->nHeight = m_pRCali->nHeight = 6;
	m_pLCali->nWidth = m_pRCali->nWidth = 9;

	GetDlgItem(IDC_EDIT_HEIGHT)->SetWindowTextW(_T("12"));
	GetDlgItem(IDC_EDIT_WIDTH)->SetWindowTextW(_T("12"));
	OnEnChangeEditHeight();
	OnEnChangeEditWidth();

	m_pSCData = new CStereoCaliData(m_pLCali, m_pRCali);
	m_pRData = new CRectifyData(m_pSCData);
	((CButton*)GetDlgItem(IDC_ALPHA_NEG))->SetCheck(true);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CdisparityestimationmfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdisparityestimationmfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdisparityestimationmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CdisparityestimationmfcDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pCamera->m_pCapture = new cv::VideoCapture(0);
	//m_pLCamera->OpenCamera(0,false);
	if (m_pLCamera->isConnected())
	{
		SetTimer(1, 1, NULL);
		GetDlgItem(IDC_START_CALI)->EnableWindow(true);
	}
	if (m_pRCamera->isConnected())
	{
		SetTimer(2, 1, NULL);
		GetDlgItem(IDC_START_CALI)->EnableWindow(true);
	}
}


void CdisparityestimationmfcDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pLFrame->frame = cv::Mat::zeros(m_pLFrame->frame.size(), m_pLFrame->frame.type());
	m_pRFrame->frame = cv::Mat::zeros(m_pRFrame->frame.size(), m_pRFrame->frame.type());
	CvvImage cvvImage;
	IplImage *iplimgL = new IplImage(m_pLFrame->frame);
	IplImage *iplimgR = new IplImage(m_pRFrame->frame);
	cvvImage.CopyOf(iplimgL, 1);
	cvvImage.DrawToHDC(m_pLFrame->hDC, &m_pLFrame->rect);
	cvvImage.CopyOf(iplimgR, 1);
	cvvImage.DrawToHDC(m_pRFrame->hDC, &m_pRFrame->rect);
	KillTimer(1);
	KillTimer(2);
	//m_pLCamera->CloseCamera();
	//m_pCamera->m_pCapture->release();

}


void CdisparityestimationmfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_pCamera->readFrame();
	IplImage *iplimg = new IplImage;
	if (nIDEvent == 1)
	{
		iplimg = m_pLCamera->QueryFrame();
		m_pLFrame->frame = cv::cvarrToMat(iplimg);
		CvvImage cvvImage;
		//IplImage *iplimg = new IplImage(m_pCamera->m_pFrame->frame);
		cvvImage.CopyOf(iplimg, 1);
		cvvImage.DrawToHDC(m_pLFrame->hDC, &m_pLFrame->rect);
	}
	if (nIDEvent == 2)
	{
		iplimg = m_pRCamera->QueryFrame();
		m_pRFrame->frame = cv::cvarrToMat(iplimg);
		CvvImage cvvImage;
		//IplImage *iplimg = new IplImage(m_pCamera->m_pFrame->frame);
		cvvImage.CopyOf(iplimg, 1);
		cvvImage.DrawToHDC(m_pRFrame->hDC, &m_pRFrame->rect);
	}

	CDialog::OnTimer(nIDEvent);
}

void CdisparityestimationmfcDlg::loadCamNames()
{
	int cam_count = CCameraDS::CameraCount();
	if (cam_count == 0)
	{
		AfxMessageBox(_T('无法找到摄像头！'));
		return;
	}
	CComboBox *selLCam = (CComboBox*)GetDlgItem(IDC_SEL_LCAM);
	CComboBox *selRCam = (CComboBox*)GetDlgItem(IDC_SEL_RCAM);
	if (cam_count == 1)
	{
		selRCam->EnableWindow(FALSE);
	}
	for (int i = 0; i < cam_count; i++)
	{
		char camera_name[1024];
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name));
		if (retval > 0)
		{
			camera_names.push_back(CString(camera_name,strlen(camera_name)));
		}
	}
	for (int i = 0; i < camera_names.size(); i++)
	{
		selLCam->AddString(camera_names[i]);
		if (selRCam->IsWindowEnabled())
		{
			selRCam->AddString(camera_names[i]);
		}
	}
	//selLCam->SetCurSel(0);
	//if (selRCam->IsWindowEnabled())
	//	selRCam->SetCurSel(1);
}


void CdisparityestimationmfcDlg::OnCbnSelchangeSelLcam()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox *selLCam = (CComboBox*)GetDlgItem(IDC_SEL_LCAM);
	int index = selLCam->GetCurSel();
	m_pLCamera->OpenCamera(index, false);
	GetDlgItem(IDC_OPEN_LCALI)->EnableWindow(true);
}


void CdisparityestimationmfcDlg::OnCbnSelchangeSelRcam()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox *selRCam = (CComboBox*)GetDlgItem(IDC_SEL_RCAM);
	int index = selRCam->GetCurSel();
	if (index == ((CComboBox*)GetDlgItem(IDC_SEL_LCAM))->GetCurSel())
	{
		AfxMessageBox(_T("左右相机不能是同一个！"));
		return;
	}
	m_pRCamera->OpenCamera(index, false);
	GetDlgItem(IDC_OPEN_RCALI)->EnableWindow(true);

}


void CdisparityestimationmfcDlg::OnBnClickedFromcam()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fromCam = true;
}


void CdisparityestimationmfcDlg::OnBnClickedFrompic()
{
	// TODO: 在此添加控件通知处理程序代码
	m_fromCam = false;
}


void CdisparityestimationmfcDlg::OnNMCustomdrawSliderNumw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_NUMW);
	int numw = slider->GetPos();
	m_pLCali->nWidth = m_pRCali->nWidth = numw - 1;
	CString num_label;
	num_label.Format(_T("%d"), numw);
	GetDlgItem(IDC_NUMW)->SetWindowTextW(num_label);
	*pResult = 0;
}


void CdisparityestimationmfcDlg::OnNMCustomdrawSliderNumh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_NUMH);
	int numh = slider->GetPos();
	m_pLCali->nHeight = m_pRCali->nHeight = numh - 1;
	CString num_label;
	num_label.Format(_T("%d"), numh);
	GetDlgItem(IDC_NUMH)->SetWindowTextW(num_label);

	*pResult = 0;
}


void CdisparityestimationmfcDlg::OnEnChangeEditWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_WIDTH)->GetWindowTextW(str);
	m_pLCali->lWidth = _ttof(str);
	m_pRCali->lWidth = _ttof(str);
}


void CdisparityestimationmfcDlg::OnEnChangeEditHeight()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_HEIGHT)->GetWindowTextW(str);
	m_pLCali->lHeight = _ttof(str);
	m_pRCali->lHeight = _ttof(str);
}


void CdisparityestimationmfcDlg::OnBnClickedStartCali()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cali = !m_cali;
	if (m_cali)
	{
		m_pLCali->cornersVect.clear();
		m_pLCali->worldPointsVect.clear();
		m_pRCali->cornersVect.clear();
		m_pRCali->worldPointsVect.clear();
		GetDlgItem(IDC_FROMCAM)->EnableWindow(false);
		GetDlgItem(IDC_FROMPIC)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_NUMW)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_NUMH)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(false);
		if (m_fromCam)
		{
			GetDlgItem(IDC_SHOT)->EnableWindow(true);
		}
		else
		{
			//打开图片文件进行标定
			CString filefilter = _T("Image Files(*.jpg,*.png,*.bmp)|*.jpg;*.png;*.bmp");
			std::vector<std::string> filenames;
			CFileDialog dlg(true, _T("Image Files(*.jpg,*.png,*.bmp)"), _T(""), OFN_OVERWRITEPROMPT| OFN_ALLOWMULTISELECT, filefilter);
			DWORD MAXFILE = 40000;
			dlg.m_ofn.nMaxFile = MAXFILE;
			TCHAR* pc = new TCHAR[MAXFILE];
			dlg.m_ofn.lpstrFile = pc;
			dlg.m_ofn.lpstrFile[0] = NULL;
			bool l = false, r = false;
			if (/*m_pLCamera->isConnected()*/true)
			{
				filenames.clear();
				dlg.m_ofn.lpstrTitle = _T("输入左目图片");
				if (dlg.DoModal() == IDOK)
				{
					POSITION pos = dlg.GetStartPosition();
					while (pos != NULL)
					{
						CString filename = dlg.GetNextPathName(pos);
						std::string s = CW2A(filename.GetString());
						filenames.push_back(s);
					}
					m_pLCali->calibrateByPics(filenames);
					GetDlgItem(IDC_SAVE_LCALI)->EnableWindow(true);
					l = true;
				}
			}
			if (/*m_pRCamera->isConnected()*/true)
			{
				filenames.clear();
				dlg.m_ofn.lpstrTitle = _T("输入右目图片");
				if (dlg.DoModal() == IDOK)
				{
					POSITION pos = dlg.GetStartPosition();
					while (pos != NULL)
					{
						CString filename = dlg.GetNextPathName(pos);
						std::string s = CW2A(filename.GetString());
						filenames.push_back(s);
					}
					m_pRCali->calibrateByPics(filenames);
					GetDlgItem(IDC_SAVE_RCALI)->EnableWindow(true);
					r = true;
				}
			}
			if (/*m_pLCamera->isConnected() && m_pRCamera->isConnected()*/l&&r)
				GetDlgItem(IDC_START_SCALI)->EnableWindow(true);
			m_cali = !m_cali;
			GetDlgItem(IDC_FROMCAM)->EnableWindow(true);
			GetDlgItem(IDC_FROMPIC)->EnableWindow(true);
			GetDlgItem(IDC_SLIDER_NUMW)->EnableWindow(true);
			GetDlgItem(IDC_SLIDER_NUMH)->EnableWindow(true);
			GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(true);
			GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(true);
		}
	}
	else
	{
		if (m_fromCam)
		{
			if (m_pLCamera->isConnected())
			{
				m_pLCali->calibrate();
				GetDlgItem(IDC_SAVE_LCALI)->EnableWindow(true);
			}
			if (m_pRCamera->isConnected())
			{
				m_pRCali->calibrate();
				GetDlgItem(IDC_SAVE_RCALI)->EnableWindow(true);
			}
		}
		if (m_pLCamera->isConnected() && m_pRCamera->isConnected())
			GetDlgItem(IDC_START_SCALI)->EnableWindow(true);
		GetDlgItem(IDC_FROMCAM)->EnableWindow(true);
		GetDlgItem(IDC_FROMPIC)->EnableWindow(true);
		GetDlgItem(IDC_SLIDER_NUMW)->EnableWindow(true);
		GetDlgItem(IDC_SLIDER_NUMH)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(true);
		GetDlgItem(IDC_SHOT)->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_ALPHA_NEG))->SetCheck(1);
	}
}


void CdisparityestimationmfcDlg::OnBnClickedShot()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_fromCam)
	if (m_pLCamera->isConnected())
	{
		m_pLCali->shot(m_pLFrame->frame);
	}
	if (m_pRCamera->isConnected())
	{
		m_pRCali->shot(m_pRFrame->frame);
	}
}




void CdisparityestimationmfcDlg::OnBnClickedCaliRst()	//有问题！
{
	// TODO: 在此添加控件通知处理程序代码
	double mtx[3][3];
	double dist[5];
	if (m_pLCali->pData != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				mtx[i][j] = m_pLCali->pData->cameraMatirx.at<double>(i, j);
			}
		}
		for (int i = 0; i < 5; i++)
			dist[i] = m_pLCali->pData->distCoeffs.at<double>(0, i);
		CString strMtx, strDist;
		//strMtx.Format(_T("%.6lf %.6lf %.6lf\n%.6lf %.6lf %.6lf\n%.6lf %.6lf %.6lf", mtx[0][0], mtx[0][1], mtx[0][2]
		//	, mtx[1][0], mtx[1][1], mtx[1][2]
		//	, mtx[2][0], mtx[2][1], mtx[2][2]));
		//strDist.Format(_T("%.6lf %.6lf %.6lf %.6lf %.6lf", dist[0], dist[1], dist[2], dist[3], dist[4]));
		strMtx.Format(_T("%e %e %e\n%e %e %e\n%e %e %e", mtx[0][0], mtx[0][1], mtx[0][2]
			, mtx[1][0], mtx[1][1], mtx[1][2]
			, mtx[2][0], mtx[2][1], mtx[2][2]));
		strDist.Format(_T("%e %e %e %e %e", dist[0], dist[1], dist[2], dist[3], dist[4]));
		GetDlgItem(IDC_LMTX)->SetWindowTextW(strMtx);
		GetDlgItem(IDC_LDIST)->SetWindowTextW(strDist);

	}
}


void CdisparityestimationmfcDlg::OnBnClickedSaveLcali()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(false,_T("yml"),_T(""), OFN_OVERWRITEPROMPT,_T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot save file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pLCali->saveFile(s);
}


void CdisparityestimationmfcDlg::OnBnClickedOpenLcali()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(true, _T("yml"), _T(""), OFN_OVERWRITEPROMPT, _T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot open file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pLCali->openFile(s);
	GetDlgItem(IDC_SAVE_LCALI)->EnableWindow(true);
}


void CdisparityestimationmfcDlg::OnBnClickedSaveRcali()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(false, _T("yml"), _T(""), OFN_OVERWRITEPROMPT, _T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot save file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pRCali->saveFile(s);

}


void CdisparityestimationmfcDlg::OnBnClickedOpenRcali()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(true, _T("yml"), _T(""), OFN_OVERWRITEPROMPT, _T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot open file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pRCali->openFile(s);
	GetDlgItem(IDC_SAVE_RCALI)->EnableWindow(true);

}


void CdisparityestimationmfcDlg::OnBnClickedStartScali()
{
	// TODO: 在此添加控件通知处理程序代码
	bool fromCam = ((CButton*)GetDlgItem(IDC_ISFROMCAM))->GetCheck();
	bool fullCheck = ((CButton*)GetDlgItem(IDC_FULLCHECK))->GetCheck();
	if (m_pSCData->getData(m_pLCamera, fromCam, fullCheck))
	{
		GetDlgItem(IDC_SAVE_SCALI)->EnableWindow(true);
		GetDlgItem(IDC_START_RECT)->EnableWindow(true);
	}
	else
		AfxMessageBox(_T("双目标定失败！"));
}


void CdisparityestimationmfcDlg::OnBnClickedOpenScali()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(true, _T("yml"), _T(""), OFN_OVERWRITEPROMPT, _T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot open file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pSCData->openFile(s);
	if (m_pSCData->R.empty() || m_pSCData->T.empty() || m_pSCData->E.empty() || m_pSCData->F.empty())
	{
		AfxMessageBox(_T("缺少双目标定数据!"));
		return;
	}
	GetDlgItem(IDC_SAVE_SCALI)->EnableWindow(true);
	GetDlgItem(IDC_START_RECT)->EnableWindow(true);
}


void CdisparityestimationmfcDlg::OnBnClickedSaveScali()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(false, _T("yml"), _T(""), OFN_OVERWRITEPROMPT, _T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot save file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pSCData->saveFile(s);
}


void CdisparityestimationmfcDlg::OnBnClickedStartRect()
{
	// TODO: 在此添加控件通知处理程序代码
	bool neg = ((CButton*)GetDlgItem(IDC_ALPHA_NEG))->GetCheck();
	int pos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA))->GetPos();
	double alpha = neg ? -1.0 : pos / 100.0;
	if (m_pRData->getData(alpha))
	{
		GetDlgItem(IDC_SAVE_RECT)->EnableWindow(true);
		GetDlgItem(IDC_VERIFY_PIC)->EnableWindow(true);
		GetDlgItem(IDC_VERIFY_CAM)->EnableWindow(true);
	}
}


void CdisparityestimationmfcDlg::OnNMCustomdrawSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER_ALPHA);
	int num = slider->GetPos();
	double alpha = num / 100.0;
	CString num_label;
	num_label.Format(_T("%.2lf"), alpha);
	GetDlgItem(IDC_ALPHA)->SetWindowTextW(num_label);

	*pResult = 0;
}


void CdisparityestimationmfcDlg::OnBnClickedOpenRect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(true, _T("yml"), _T(""), OFN_OVERWRITEPROMPT, _T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot open file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pRData->openFile(s);
	if (m_pRData->R1.empty() || m_pRData->R2.empty() || m_pRData->P1.empty() || m_pRData->P2.empty() || m_pRData->Q.empty())
	{
		AfxMessageBox(_T("缺少双目校正数据!"));
		return;
	}
	GetDlgItem(IDC_SAVE_RECT)->EnableWindow(true);
	GetDlgItem(IDC_VERIFY_PIC)->EnableWindow(true);
	GetDlgItem(IDC_VERIFY_CAM)->EnableWindow(true);

}


void CdisparityestimationmfcDlg::OnBnClickedSaveRect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(false, _T("yml"), _T(""), OFN_OVERWRITEPROMPT, _T("yml文件|*.yml"));
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
	{
		//AfxMessageBox(_T("Cannot save file!"));
		return;
	}
	std::string s = CW2A(FilePathName.GetString());
	m_pRData->saveFile(s);
}


void CdisparityestimationmfcDlg::OnBnClickedVerifyPic()
{
	// TODO: 在此添加控件通知处理程序代码
	cv::Mat lFrame, rFrame;
	CString FilePathName;
	CString filefilter = _T("Image Files(*.jpg,*.png,*.bmp)|*.jpg;*.png;*.bmp");
	CFileDialog dlg(true, _T("Image Files(*.jpg,*.png,*.bmp)"), _T(""), OFN_OVERWRITEPROMPT, filefilter);
	dlg.m_ofn.lpstrTitle = _T("输入左目图片");
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
		return;
	std::string s = CW2A(FilePathName.GetString());
	lFrame = cv::imread(s);
	dlg.m_ofn.lpstrTitle = _T("输入右目图片");
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName();
	}
	else
		return;
	s = CW2A(FilePathName.GetString());
	rFrame = cv::imread(s);
	m_pRData->showEffect(lFrame, rFrame);
}


void CdisparityestimationmfcDlg::OnBnClickedVerifyCam()
{
	// TODO: 在此添加控件通知处理程序代码
	int cam_count = CCameraDS::CameraCount();
	if (cam_count < 2)
	{
		AfxMessageBox(_T("相机数目不足！"));
		return;
	}
	m_pRData->showEffect(m_pLFrame->frame, m_pRFrame->frame);
}
