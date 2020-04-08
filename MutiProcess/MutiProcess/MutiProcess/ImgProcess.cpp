#include "stdafx.h"
#include "MutiProcess.h"
#include "ImgProcess.h"
#include "MutiProcessDlg.h"

CImgProcess::CImgProcess(CMutiProcessDlg* pCMutiProcessDlg)
{
	this->pCMutiProcessDlg = pCMutiProcessDlg;
}


CImgProcess::~CImgProcess(void)
{

}

BOOL CImgProcess::Init()
{
	// ��ȡ�����¼�
	m_hHasData = CreateEvent(NULL, TRUE, FALSE, NULL);  //�ֶ�     ��ʼ��FALSE

	// �̲߳�����ʼ��
	for (int i = 0; i < THREAD_CNT; i++)
	{
		// ���з��� �ٽ���
		InitializeCriticalSection(&m_staThdPara[i].crtDqOp);
		// ����
		m_staThdPara[i].dqCache.clear();

		///�¼������� �Ƿ��ֶ� ��ʼ״̬ ����
		m_staThdPara[i].hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_staThdPara[i].hCanExit = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_staThdPara[i].hHasExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	
	/// Ԥ�ȷ����ڴ�
	for (int i = 0; i < THREAD_CNT; i++)
	{
		///ԭʼͼ��
		m_staProcData[i].p = new int;

		// �����ȡ���ݶ���
		m_staThdPara[THREAD_GETDATA].dqCache.push_front(&m_staProcData[i]);//������������ ��ѹ����ȥ���ݶ�����
	}

	// �����߳�
	// ��ȡ�����̣߳����ȼ�- 0
	m_staThdPara[THREAD_GETDATA].pThd = AfxBeginThread(ThdGetDataProc, (LPVOID)this);
	if (m_staThdPara[THREAD_GETDATA].pThd == NULL)
		return FALSE;

	// �㷨�����̣߳����ȼ�- 2
	m_staThdPara[THREAD_IMGPROC].pThd = AfxBeginThread(ThdImgProProc, (LPVOID)this, THREAD_PRIORITY_HIGHEST);
	if (m_staThdPara[THREAD_IMGPROC].pThd == NULL)
		return FALSE;

	// ������ͼ�̣߳����ȼ�- 0
	m_staThdPara[THREAD_UPDATAVIEW].pThd = AfxBeginThread(ThdUpdataViewProc, (LPVOID)this);
	if (m_staThdPara[THREAD_UPDATAVIEW].pThd == NULL)
		return FALSE;

	return TRUE;
}

void CImgProcess::Start()
{
	TRACE("Start Threads.\n");
	// ����
	for (BYTE i = 0; i < THREAD_CNT; i++)
		SetEvent(m_staThdPara[i].hStart);
}

void CImgProcess::Pause()
{
	TRACE("Pause Threads.\n");
	//��ͣ
	for (BYTE i = 0; i < THREAD_CNT; i++)
		ResetEvent(m_staThdPara[i].hStart);
}

void CImgProcess::Exit()
{
	// ֪ͨ�˳�
	for (BYTE i = 0; i < THREAD_CNT; i++)
		SetEvent(m_staThdPara[i].hCanExit);

	// �����̣߳�Ԥ���Ѿ���ͣ�̵߳����
	Start();

	// �ȴ������߳��˳�
	for (BYTE i = 0; i < THREAD_CNT; i++)
	{
		WaitForSingleObject(m_staThdPara[i].hHasExit, INFINITE);

		// ��ն���
		m_staThdPara[i].dqCache.clear();
		// ɾ�� ���з��� �ٽ���
		DeleteCriticalSection(&m_staThdPara[i].crtDqOp);
		// �ر��¼�
		CloseHandle(m_staThdPara[i].hStart);
		CloseHandle(m_staThdPara[i].hCanExit);
		CloseHandle(m_staThdPara[i].hHasExit);
	}
	// �ر��������¼�
	CloseHandle(m_hHasData);

	// �ͷ��ڴ�
	for (BYTE i = 0; i < THREAD_CNT; i++)
		ReleasePoint(m_staProcData[i].p);  // ԭʼͼ��

	TRACE("Exit Threads.\n");
}

UINT CImgProcess::ThdGetDataProc(LPVOID lpVoid)
{
	CImgProcess* pImgProc = (CImgProcess*)lpVoid;
	while (true)
	{
		// �Ե�
		Sleep(SLEEP_TIME);
		TRACE("-------0-------------------size=%d\n", pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.size());
		// �ȴ������¼�
		//TRACE("ThdGetDataProc.�ȴ������¼�\n");
		WaitForSingleObject(pImgProc->m_staThdPara[THREAD_GETDATA].hStart, INFINITE);

		// ����˳��¼�
		//TRACE("ThdGetDataProc.����˳��¼�\n");
		if (::WaitForSingleObject(pImgProc->m_staThdPara[THREAD_GETDATA].hCanExit, 0) == WAIT_OBJECT_0)
			break;

		

		// ��������¼������û�������
		//TRACE("ThdGetDataProc.��������¼������û�������\n");
		if (::WaitForSingleObject(pImgProc->m_hHasData, MAX_WAIT_DATA_TIME) != WAIT_OBJECT_0)
			continue;
		ResetEvent(pImgProc->m_hHasData);

		// û�л��������
		if (pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.empty())//��û������  ���ʾΪ��   ��Ƶ�ʹ���ʱ�����Ӳ���װ  ��ʱ���©��������������ڿ�ʼʱ��������ʵ����Ҫ���������������ڴ�ռ�������ܵ����ֵ�� 
			continue;

		// �Ӷ��л�ȡ����
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);
		PST_PROC_DATA pstProcData = NULL;
		// ȡ���֡����
		pstProcData = pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.back();
		// ��֡��֡�������Ƴ�
		pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.pop_back();
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);

		// ��ȡ����
		pImgProc->GetData(pstProcData);
		

		// ��֡ѹ�� ͼ���� ����
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);
		pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.push_front(pstProcData);
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);
	}

	// �������˳�
	TRACE("ThdGetDataProc has exited.\n");
	SetEvent(pImgProc->m_staThdPara[THREAD_GETDATA].hHasExit);
	return 0;
}

UINT CImgProcess::ThdImgProProc(LPVOID lpVoid)
{
	CImgProcess* pImgProc = (CImgProcess*)lpVoid;
	while (true)
	{
		// �Ե�
		Sleep(SLEEP_TIME);
		TRACE("-----1----size=%d\n", pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.size());
		// �ȴ������¼�
		WaitForSingleObject(pImgProc->m_staThdPara[THREAD_IMGPROC].hStart, INFINITE);

		// ����˳��¼�
		if (::WaitForSingleObject(pImgProc->m_staThdPara[THREAD_IMGPROC].hCanExit, 0) == WAIT_OBJECT_0)
			break;

		// û�л��������
		if (pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.empty())
			continue;

		// �Ӷ��л�ȡ����
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);
		PST_PROC_DATA pstProcData = NULL;
		// ȡ���֡����
		pstProcData = pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.back();
		// ��֡��֡�������Ƴ�
		pImgProc->m_staThdPara[THREAD_IMGPROC].dqCache.pop_back();
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_IMGPROC].crtDqOp);

		TRACE("------------------IMG Processing start\n" );
		*(pstProcData->p)=*(pstProcData->p) * 2;
		//Sleep(500);
		TRACE("------------------IMG Processing end\n");
		TRACE("pro data=%d\n", *(pstProcData->p));
		// ͼ����
		//{
		//	// ���ñ�Ե����ڴ�ṹ
		//	{
		//		pImgProc->m_stEdgeDetect.lpRGB = pstProcData->lpRGB;
		//		pImgProc->m_stEdgeDetect.pbyEgDtc = pstProcData->pbyBufA;
		//		pImgProc->m_stEdgeDetect.pbyFilte = pstProcData->lpMask;
		//	}
		//}

		// ��֡ѹ�� ������ͼ ������
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);
		pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.push_front(pstProcData);
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);
	}

	// �������˳�
	TRACE("THREAD_IMGPROC has exited.\n");
	SetEvent(pImgProc->m_staThdPara[THREAD_IMGPROC].hHasExit);

	// ����
	return 0;
}

UINT CImgProcess::ThdUpdataViewProc(LPVOID lpVoid)
{
	CImgProcess* pImgProc = (CImgProcess*)lpVoid;
	while (true)
	{
		// �Ե�
		Sleep(SLEEP_TIME);
		TRACE("-----2----size=%d\n", pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.size());
		// �ȴ������¼�
		WaitForSingleObject(pImgProc->m_staThdPara[THREAD_UPDATAVIEW].hStart, INFINITE);

		// ����˳��¼�
		if (::WaitForSingleObject(pImgProc->m_staThdPara[THREAD_UPDATAVIEW].hCanExit, 0) == WAIT_OBJECT_0)
			break;

		// û�л��������
		if (pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.empty())
			continue;

		// �Ӷ��л�ȡ����
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);
		PST_PROC_DATA pstProcData = NULL;
		// ȡ���֡����
		pstProcData = pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.back();
		// ��֡��֡�������Ƴ�
		pImgProc->m_staThdPara[THREAD_UPDATAVIEW].dqCache.pop_back();
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_UPDATAVIEW].crtDqOp);

		// ���½���
		// ������ڿɼ�����ˢ����ʾ
		//if (pImgProc->m_pEffectWnd->IsWindowVisible())
		//{
		//	pImgProc->m_pEffectWnd->RefreshData(pstProcData);
		//}
		//// ������ɼ�������־��Ϊ������
		//else if (!pImgProc->m_pMainWnd->IsIconic())
		//{
		//	// ��ʾ�ر�
		//	pImgProc->m_pMainWnd->OnTbDirectShow();
		//}
		pImgProc->pCMutiProcessDlg->m_result = *(pstProcData->p);

		// ��֡ѹ�� ��ȡ���� ������
		EnterCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);
		pImgProc->m_staThdPara[THREAD_GETDATA].dqCache.push_front(pstProcData);
		LeaveCriticalSection(&pImgProc->m_staThdPara[THREAD_GETDATA].crtDqOp);
	}

	// �������˳�
	TRACE("ThdUpdataViewProc has exited.\n");
	SetEvent(pImgProc->m_staThdPara[THREAD_UPDATAVIEW].hHasExit);

	// ����
	return 0;
}

BOOL CImgProcess::GetData(PST_PROC_DATA pstProcData)
{
	*(pstProcData->p) = pCMutiProcessDlg->m_push_data;
	TRACE("push data=%d\n", *(pstProcData->p));
	return TRUE;
}


