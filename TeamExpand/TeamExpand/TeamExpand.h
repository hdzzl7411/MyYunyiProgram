
// TeamExpand.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTeamExpandApp:
// �йش����ʵ�֣������ TeamExpand.cpp
//

class CTeamExpandApp : public CWinApp
{
public:
	CTeamExpandApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTeamExpandApp theApp;