#pragma once

#include <winbase.h>
#include <deque>

// �߳�ЪϢʱ��
#define SLEEP_TIME			1
// ���ȴ����ݵ���ʱ��
#define MAX_WAIT_DATA_TIME	1000
//�̳߳����߳�����
const int THREAD_CNT = 3;
#define THREAD_GETDATA		0	// ��ȡ�����̱߳��
#define THREAD_IMGPROC		1	// �鱾�������򣨱�Ե������̱߳��
#define THREAD_UPDATAVIEW	2	// ������ͼ���

#define IMG_BUFFER_NUM      10    //ÿ��������д�С

// ͼ������С
#define RESO_XY			307200	// ͼ�����������
#define RESO_X			640		// ͼ�������
#define RESO_Y			480		// ͼ�����߶�

#define ReleasePoint( p ) { if( p ){ delete p; p = NULL; } }
#define ReleaseArray( p ) { if( p ){ delete [] p; p = NULL; } }

typedef struct _ST_PROC_DATA
{
	int* p;		// ԭʼͼ������

}ST_PROC_DATA, *PST_PROC_DATA;

// �̲߳���
typedef struct _ST_THD_PARA
{
	CWinThread*	pThd;		// �߳�ָ��

	HANDLE hStart;			// �����¼�
	HANDLE hCanExit;		// �����˳��¼�
	HANDLE hHasExit;		// �Ѿ��˳��¼�

	CRITICAL_SECTION crtDqOp;		// ������в��� �ٽ���
	std::deque< PST_PROC_DATA > dqCache;	// �������

}ST_THD_PARA, *PST_THD_PARA;