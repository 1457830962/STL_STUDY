
// MutiProcessDlg.h : 头文件
//

#pragma once

#include "ImgProcess.h"

// CMutiProcessDlg 对话框
class CMutiProcessDlg : public CDialogEx
{
// 构造
public:
	CMutiProcessDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMutiProcessDlg();
// 对话框数据
	enum { IDD = IDD_MUTIPROCESS_DIALOG };

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
	// 图像处理，线程池类的实体
	CImgProcess* m_pcImgProc;
public:
	afx_msg void OnBnClickedBtnPushData();

	virtual void OnCancel();
	afx_msg void OnBnClickedBtnStartTh();
	afx_msg void OnBnClickedBtnStopTh();

	int m_push_data;
	int m_result;
	CString str;
	// 数据操作保护
	//CRITICAL_SECTION m_crtPro;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
