
// ModifyProjectName.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CModifyProjectNameApp: 
// �йش����ʵ�֣������ ModifyProjectName.cpp
//

class CModifyProjectNameApp : public CWinApp
{
public:
	CModifyProjectNameApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CModifyProjectNameApp theApp;