/*
 *
 *	Author		王荣涛
 *	Email		jgshining@163.org/rtwang@corp.netease.com
 *	Date		2009-06-22
 *	Copyright	Hangzhou, Netease Inc.
 *	Brief		高精度定时器类
 *
 */

#ifndef __NE_SUPERTIMER_H__
#define __NE_SUPERTIMER_H__

//#include "../stdafx.h"
//#include "../Log.h"

class CSuperTimer
{
	__int64 __f, __t1, __t2;

public:

	CSuperTimer()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&__f);
		Reset();
	}

	void Reset()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&__t1);
	}

	double Lapse()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&__t2);
		return double(__t2 - __t1) / __f;
	}

	//void Report()
	//{
	//	APP_DLOG(LOG_DETAIL, 0, L"Timer 0x%p: %.6lfms lapses", this, Lapse() * 1000.0);
	//}

	//void FlushReport(CString& sReporter)
	//{
	//	CString sTmp;
	//	sTmp.Format(_T("%s %.6lfms lapses"), sReporter, Lapse() * 1000.0);
	//	NET_DLOG(LOG_DETAIL, 0, sTmp);
	//	sReporter = sTmp;
	//}
};

class CAutoReportTimer
{
public:
	CAutoReportTimer(const wchar_t *tips) : csTips(tips){}
	~CAutoReportTimer() {
		//APP_LOG(LOG_INFO, 1, L"%s %.6lfms lapses", csTips, timer.Lapse() * 1000.0);
		//LogAMessage(cmx::TLog::levelInfo, cmx::TFmtString("% time is : (%)").arg(std::string(CW2A(csTips.c_str())).c_str()).arg(timer.Lapse() * 1000.0).str());

	}

	void display(const wchar_t * substep) {
		//APP_LOG(LOG_INFO, 1, L"%s(%s) %.6lfms lapses", csTips, substep, timer.Lapse() * 1000.0);
		//LogAMessage(cmx::TLog::levelInfo, cmx::TFmtString("% to % time is : (%)").arg(std::string(CW2A(csTips.c_str())).c_str()).arg(std::string(CW2A(substep)).c_str()).arg(timer.Lapse() * 1000.0).str());

	}
protected:
	std::wstring csTips;
	CSuperTimer timer;
};

#define AUTO_REPOTER(tips) CAutoReportTimer __reporter(tips);

#endif // __NE_SUPERTIMER_H__
