#include "stdafx.h"
#include "MutiProcess.h"
#include "ImgProcess.h"
#include "MutiProcessDlg.h"
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"
using namespace log4cxx;
CImgProcess::CImgProcess(CMutiProcessDlg* pCMutiProcessDlg)
{
	this->pCMutiProcessDlg = pCMutiProcessDlg;
}


CImgProcess::~CImgProcess(void)
{

}

BOOL CImgProcess::Init()
{
	// 获取数据事件
	m_hHasData = CreateEvent(NULL, TRUE, FALSE, NULL);  //手动     初始化FALSE

	// 线程参数初始化
	for (int i = 0; i < THREAD_CNT; i++)
	{
		// 队列访问 临界区
		InitializeCriticalSection(&m_staThdPara[i].crtDqOp);
		// 队列
		m_staThdPara[i].dqCache.clear();

		///事件：属性 是否手动 初始状态 名称
		m_staThdPara[i].hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_staThdPara[i].hCanExit = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_staThdPara[i].hHasExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	
	/// 预先分配内存
	for (int i = 0; i < THREAD_CNT; i++)
	{
		///原始图像
		m_staProcData[i].p = new int;

		// 加入获取数据队列
		m_staThdPara[THREAD_GETDATA].dqCache.push_front(&m_staProcData[i]);//将三个空箱子 均压进回去数据对列中
	}

	// 启动线程
	// 获取数据线程：优先级- 0
	m_staThdPara[THREAD_GETDATA].pThd = AfxBeginThread(ThdGetDataProc, (LPVOID)this);
	if (m_staThdPara[THREAD_GETDATA].pThd == NULL)
		return FALSE;

	// 算法处理线程：优先级- 2
	m_staThdPara[THREAD_IMGPROC].pThd = AfxBeginThread(ThdImgProProc, (LPVOID)this, THREAD_PRIORITY_HIGHEST);
	if (m_staThdPara[THREAD_IMGPROC].pThd == NULL)
		return FALSE;

	// 更新视图线程：优先级- 0
	m_staThdPara[THREAD_UPDATAVIEW].pThd = AfxBeginThread(ThdUpdataViewProc, (LPVOID)this);
	if (m_staThdPara[THREAD_UPDATAVIEW].pThd == NULL)
		return FALSE;

	return TRUE;
}

void CImgProcess::Start()
{
	LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::Start"), "Start Threads.");
	// 启动
	for (BYTE i = 0; i < THREAD_CNT; i++)
		SetEvent(m_staThdPara[i].hStart);
}

void CImgProcess::Pause()
{
	LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::Pause()"), "Pause Threads.");
	//暂停
	for (BYTE i = 0; i < THREAD_CNT; i++)
		ResetEvent(m_staThdPara[i].hStart);
}

void CImgProcess::Exit()
{
	// 通知退出
	for (BYTE i = 0; i < THREAD_CNT; i++)
		SetEvent(m_staThdPara[i].hCanExit);

	// 启动线程，预防已经暂停线程的情况
	Start();

	// 等待所有线程退出
	for (BYTE i = 0; i < THREAD_CNT; i++)
	{
		WaitForSingleObject(m_staThdPara[i].hHasExit, INFINITE);

		// 清空队列
		m_staThdPara[i].dqCache.clear();
		// 删除 队列访问 临界区
		DeleteCriticalSection(&m_staThdPara[i].crtDqOp);
		// 关闭事件
		CloseHandle(m_staThdPara[i].hStart);
		CloseHandle(m_staThdPara[i].hCanExit);
		CloseHandle(m_staThdPara[i].hHasExit);
	}
	// 关闭新数据事件
	CloseHandle(m_hHasData);

	// 释放内存
	for (BYTE i = 0; i < THREAD_CNT; i++)
		ReleasePoint(m_staProcData[i].p);  // 原始图像

	LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::Exit()"), "Exit Threads.");
}

UINT CImgProcess::ThdGetDataProc(LPVOID lpVoid)
{
	CImgProcess* pImgProc = (CImgProcess*)lpVoid;
	while (true)
	{
		// 稍等
		Sleep(SLEEP_TIME);		
		// 等待启动事件
		//TRACE("ThdGetDataProc.等待启动事件\n");
		WaitForSingleObject(pImgProc->m_staThdPara[THREAD_GETDATA].hStart, INFINITE);

		// 检查退出事件
		//TRACE("ThdGetDataProc.检查退出事件\n");
		if (::WaitForSingleObject(pImgProc->m_staThdPara[THREAD_GETDATA].hCanExit, 0) == WAIT_OBJECT_0)
			break;

		

		// 检查数据事件，如果没有则继续
		//TRACE("ThdGetDataProc.检查数据事件，如果没有则继续\n");
		if (::WaitForSingleObject(pImgProc->m_hHasData, MAX_WAIT_DATA_TIME) != WAIT_OBJECT_0)
			continue;
		ResetEvent(pImgProc->m_hHasData);

		// 没有缓存则继续
		if (pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.empty())//若没有箱子  则表示为空   当频率过高时，箱子不够装  此时会出漏数据情况（所以在开始时尽量根据实际需要计算出处理过程中内存空间需求可能的最大值） 
			continue;

		// 从队列获取缓存
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);
		PST_PROC_DATA pstProcData = NULL;
		// 取最后帧数据
		pstProcData = pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.back();
		// 该帧从帧队列中移除
		pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.pop_back();
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);

		LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::ThdGetDataProc() has data"), *(pstProcData->p));
		// 获取数据
		pImgProc->GetData(pstProcData);
		

		// 该帧压入 图像处理 队列
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);
		pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.push_front(pstProcData);
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);
	}

	// 表明已退出
	TRACE("ThdGetDataProc has exited.\n");
	LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::ThdGetDataProc  "), "ThdGetDataProc has exited");
	SetEvent(pImgProc->m_staThdPara[THREAD_GETDATA].hHasExit);
	return 0;
}

UINT CImgProcess::ThdImgProProc(LPVOID lpVoid)
{
	CImgProcess* pImgProc = (CImgProcess*)lpVoid;
	while (true)
	{
		// 稍等
		Sleep(SLEEP_TIME);
		
		// 等待启动事件
		WaitForSingleObject(pImgProc->m_staThdPara[THREAD_IMGPROC].hStart, INFINITE);

		// 检查退出事件
		if (::WaitForSingleObject(pImgProc->m_staThdPara[THREAD_IMGPROC].hCanExit, 0) == WAIT_OBJECT_0)
			break;

		// 没有缓存则继续
		if (pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.empty())
			continue;

		// 从队列获取缓存
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);
		PST_PROC_DATA pstProcData = NULL;
		// 取最后帧数据
		pstProcData = pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.back();
		// 该帧从帧队列中移除
		pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.pop_back();
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);

		*(pstProcData->p)=*(pstProcData->p) * 2;
		LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::ThdImgProProc   process data is "), *(pstProcData->p));

		// 该帧压入 更新视图 队列中
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);
		pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.push_front(pstProcData);
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);
	}

	// 表明已退出
	TRACE("THREAD_IMGPROC has exited.\n");
	LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::ThdImgProProc "), "THREAD_IMGPROC has exited.");
	SetEvent(pImgProc->m_staThdPara[THREAD_IMGPROC].hHasExit);

	// 结束
	return 0;
}

UINT CImgProcess::ThdUpdataViewProc(LPVOID lpVoid)
{
	CImgProcess* pImgProc = (CImgProcess*)lpVoid;
	while (true)
	{
		// 稍等
		Sleep(SLEEP_TIME);

		// 等待启动事件
		WaitForSingleObject(pImgProc->m_staThdPara[THREAD_UPDATAVIEW].hStart, INFINITE);

		// 检查退出事件
		if (::WaitForSingleObject(pImgProc->m_staThdPara[THREAD_UPDATAVIEW].hCanExit, 0) == WAIT_OBJECT_0)
			break;

		// 没有缓存则继续
		if (pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.empty())
			continue;

		// 从队列获取缓存
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);
		PST_PROC_DATA pstProcData = NULL;
		// 取最后帧数据
		pstProcData = pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.back();
		// 该帧从帧队列中移除
		pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.pop_back();
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);

		LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::ThdUpdataViewProc "), *(pstProcData->p));
		pImgProc->pCMutiProcessDlg->m_result = *(pstProcData->p);

		// 该帧压入 获取数据 队列中
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);
		pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.push_front(pstProcData);
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);
	}

	// 表明已退出
	TRACE("ThdUpdataViewProc has exited.\n");
	LOG4CXX_INFO(log4cxx::Logger::getLogger("CImgProcess::ThdUpdataViewProc "), "ThdUpdataViewProc has exited.");
	SetEvent(pImgProc->m_staThdPara[THREAD_UPDATAVIEW].hHasExit);

	// 结束
	return 0;
}

BOOL CImgProcess::GetData(PST_PROC_DATA pstProcData)
{
	*(pstProcData->p) = pCMutiProcessDlg->m_push_data;
	return TRUE;
}


