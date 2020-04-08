#pragma once
class CMutiProcessDlg;

class CImgProcess
{
public:
	CImgProcess(CMutiProcessDlg* pCMutiProcessDlg);
	virtual ~CImgProcess(void);
public:
	// ���������¼�����Ƶ����ص���������λ����ȡ���ݺ����и�λ
	HANDLE m_hHasData;

	CMutiProcessDlg* pCMutiProcessDlg;

private:

	// ���ݰ� ͼ��������
	ST_PROC_DATA m_staProcData[THREAD_CNT];
	// �̲߳���
	ST_THD_PARA m_staThdPara[THREAD_CNT];

	BOOL GetData(PST_PROC_DATA pstProcData);

	//------------------�̺߳���-----------------
	// ��ȡ�����߳�
	static UINT	ThdGetDataProc(LPVOID lpVoid);
	// ��Ե����߳�
	static UINT	ThdImgProProc(LPVOID lpVoid);
	// ��ͼ�����߳�
	static UINT	ThdUpdataViewProc(LPVOID lpVoid);

public:
	//------------------�̲߳���------------------
	// �߳� ��ʼ��
	virtual BOOL Init();
	// �߳� ����
	virtual void Start();
	// �߳� ��ͣ
	virtual void Pause();
	// �߳� �˳�
	virtual void Exit();
};

