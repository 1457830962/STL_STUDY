
// MutiProcess.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMutiProcessApp: 
// �йش����ʵ�֣������ MutiProcess.cpp
//

class CMutiProcessApp : public CWinApp
{
public:
	CMutiProcessApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMutiProcessApp theApp;