
// MutiProcessDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MutiProcess.h"
#include "MutiProcessDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMutiProcessDlg �Ի���



CMutiProcessDlg::CMutiProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMutiProcessDlg::IDD, pParent)
	, m_push_data(0)
	, m_result(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ͼ����
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


// CMutiProcessDlg ��Ϣ�������

BOOL CMutiProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	// ͼ�����̳߳�ʼ��
	if (!m_pcImgProc->Init())
		MessageBox(_T("ͼ�����̴߳���ʧ�ܣ�\n"));
	else
		TRACE("ͼ�����̴߳����ɹ���\n");

	//m_pcImgProc->Start();

	this->SetTimer(1, 100, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMutiProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMutiProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMutiProcessDlg::OnBnClickedBtnPushData()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	SetEvent(m_pcImgProc->m_hHasData);
}


void CMutiProcessDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���
	KillTimer(1);
	// �˳��߳�
	m_pcImgProc->Exit();

	CDialogEx::OnCancel();
}



void CMutiProcessDlg::OnBnClickedBtnStartTh()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pcImgProc->Start();
}


void CMutiProcessDlg::OnBnClickedBtnStopTh()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_pcImgProc->Pause();
}


void CMutiProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
