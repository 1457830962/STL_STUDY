#pragma once

#include <winbase.h>
#include <deque>

// 线程歇息时间
#define SLEEP_TIME			1
// 最大等待数据到来时间
#define MAX_WAIT_DATA_TIME	1000
//线程池中线程数量
const int THREAD_CNT = 3;
#define THREAD_GETDATA		0	// 获取数据线程标号
#define THREAD_IMGPROC		1	// 书本封面区域（边缘）检测线程标号
#define THREAD_UPDATAVIEW	2	// 更新视图标号

#define IMG_BUFFER_NUM      10    //每个缓存队列大小

// 图像最大大小
#define RESO_XY			307200	// 图像最大像素数
#define RESO_X			640		// 图像最大宽度
#define RESO_Y			480		// 图像最大高度

#define ReleasePoint( p ) { if( p ){ delete p; p = NULL; } }
#define ReleaseArray( p ) { if( p ){ delete [] p; p = NULL; } }

typedef struct _ST_PROC_DATA
{
	int* p;		// 原始图像数据

}ST_PROC_DATA, *PST_PROC_DATA;

// 线程参数
typedef struct _ST_THD_PARA
{
	CWinThread*	pThd;		// 线程指针

	HANDLE hStart;			// 启动事件
	HANDLE hCanExit;		// 允许退出事件
	HANDLE hHasExit;		// 已经退出事件

	CRITICAL_SECTION crtDqOp;		// 缓存队列操作 临界区
	std::deque< PST_PROC_DATA > dqCache;	// 缓存队列

}ST_THD_PARA, *PST_THD_PARA;