
// TeamServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTeamServerApp:
// �йش����ʵ�֣������ TeamServer.cpp
//

class CTeamServerApp : public CWinApp
{
public:
	CTeamServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTeamServerApp theApp;