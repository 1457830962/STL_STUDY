
// MutiProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MutiProcess.h"
#include "MutiProcessDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMutiProcessDlg 对话框



CMutiProcessDlg::CMutiProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMutiProcessDlg::IDD, pParent)
	, m_push_data(0)
	, m_result(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 图像处理
	m_pcImgProc = new CImgProcess(this);
	
	//InitializeCriticalSection(&m_crtPro);
}

CMutiProcessDlg::~CMutiProcessDlg()
{
	//DeleteCriticalSection(&m_crtPro);
	ReleasePoint(m_pcImgProc);
}

void CMutiProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATA, m_push_data);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_result);
}

BEGIN_MESSAGE_MAP(CMutiProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PUSH_DATA, &CMutiProcessDlg::OnBnClickedBtnPushData)
	ON_BN_CLICKED(IDC_BTN_START_TH, &CMutiProcessDlg::OnBnClickedBtnStartTh)
	ON_BN_CLICKED(IDC_BTN_STOP_TH, &CMutiProcessDlg::OnBnClickedBtnStopTh)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMutiProcessDlg 消息处理程序

BOOL CMutiProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// TODO:  在此添加额外的初始化代码

	// 图像处理线程初始化
	if (!m_pcImgProc->Init())
		MessageBox(_T("图像处理线程创建失败！\n"));
	else
		TRACE("图像处理线程创建成功。\n");

	//m_pcImgProc->Start();

	this->SetTimer(1, 100, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}




void CMutiProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMutiProcessDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMutiProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMutiProcessDlg::OnBnClickedBtnPushData()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	SetEvent(m_pcImgProc->m_hHasData);
}


void CMutiProcessDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类
	KillTimer(1);
	// 退出线程
	m_pcImgProc->Exit();

	CDialogEx::OnCancel();
}



void CMutiProcessDlg::OnBnClickedBtnStartTh()
{
	// TODO:  在此添加控件通知处理程序代码
	m_pcImgProc->Start();
}


void CMutiProcessDlg::OnBnClickedBtnStopTh()
{
	// TODO:  在此添加控件通知处理程序代码
	m_pcImgProc->Pause();
}


void CMutiProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
	{
		str.Format(L"%d", m_result);
		GetDlgItem(IDC_EDIT_RESULT)->SetWindowTextW(str);
		//TRACE("result = %d\n", m_result);
		break;
	}

	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
