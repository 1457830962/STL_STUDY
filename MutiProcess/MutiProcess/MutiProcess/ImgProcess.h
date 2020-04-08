#pragma once
class CMutiProcessDlg;

class CImgProcess
{
public:
	CImgProcess(CMutiProcessDlg* pCMutiProcessDlg);
	virtual ~CImgProcess(void);
public:
	// 有新数据事件，视频捕获回调函数中置位，获取数据函数中复位
	HANDLE m_hHasData;

	CMutiProcessDlg* pCMutiProcessDlg;

private:

	// 数据包 图像处理数据
	ST_PROC_DATA m_staProcData[THREAD_CNT];
	// 线程参数
	ST_THD_PARA m_staThdPara[THREAD_CNT];

	BOOL GetData(PST_PROC_DATA pstProcData);

	//------------------线程函数-----------------
	// 获取数据线程
	static UINT	ThdGetDataProc(LPVOID lpVoid);
	// 边缘检测线程
	static UINT	ThdImgProProc(LPVOID lpVoid);
	// 视图更新线程
	static UINT	ThdUpdataViewProc(LPVOID lpVoid);

public:
	//------------------线程操作------------------
	// 线程 初始化
	virtual BOOL Init();
	// 线程 启动
	virtual void Start();
	// 线程 暂停
	virtual void Pause();
	// 线程 退出
	virtual void Exit();
};

