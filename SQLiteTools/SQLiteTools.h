
// SQLiteTools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSQLiteToolsApp: 
// �йش����ʵ�֣������ SQLiteTools.cpp
//

class CSQLiteToolsApp : public CWinApp
{
public:
	CSQLiteToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSQLiteToolsApp theApp;
