
// LedShowForMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLedShowForMFCApp:
// �йش����ʵ�֣������ LedShowForMFC.cpp
//

class CLedShowForMFCApp : public CWinApp
{
public:
	CLedShowForMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLedShowForMFCApp theApp;