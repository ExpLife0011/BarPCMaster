// BarPCMaster.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "BarPCMaster.h"
#include "MainFrameWnd.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	HRESULT hr = ::CoInitialize(NULL);
	if (FAILED(hr))
	{
		return 0;
	}

	// ���ù���ʵ��
	CPaintManagerUI::SetInstance(hInstance);
	// ����Ƥ��·��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin"));
	//CPaintManagerUI::SetResourceZip(_T("Skin.zip"));

	CMainFrameWnd* pFrame = new CMainFrameWnd();

	pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0);
	pFrame->CenterWindow();
	pFrame->ShowWindow();

	CPaintManagerUI::MessageLoop();

	if (NULL != pFrame)
	{
		delete pFrame;
	}

	::CoUninitialize();

	return 0;
}

