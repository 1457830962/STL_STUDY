
// MutiProcessDlg.h : ͷ�ļ�
//

#pragma once

#include "ImgProcess.h"

// CMutiProcessDlg �Ի���
class CMutiProcessDlg : public CDialogEx
{
// ����
public:
	CMutiProcessDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMutiProcessDlg();
// �Ի�������
	enum { IDD = IDD_MUTIPROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ͼ�����̳߳����ʵ��
	CImgProcess* m_pcImgProc;
public:
	afx_msg void OnBnClickedBtnPushData();

	virtual void OnCancel();
	afx_msg void OnBnClickedBtnStartTh();
	afx_msg void OnBnClickedBtnStopTh();

	int m_push_data;
	int m_result;
	CString str;
	// ���ݲ�������
	//CRITICAL_SECTION m_crtPro;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
