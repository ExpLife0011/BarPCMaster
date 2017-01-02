#include "StdAfx.h"
#include "MainFrameWnd.h"


CMainFrameWnd::CMainFrameWnd()
	: m_pWndShadow(NULL)
	, m_pProblemList(NULL)
{
}


CMainFrameWnd::~CMainFrameWnd()
{
	if (NULL != m_pWndShadow)
	{
		delete m_pWndShadow;
		m_pWndShadow = NULL;
	}
}

DuiLib::CDuiString CMainFrameWnd::GetSkinFolder()
{
	return m_PaintManager.GetInstancePath();
}

DuiLib::CDuiString CMainFrameWnd::GetSkinFile()
{
	return _T("main_frame.xml");
}

LPCTSTR CMainFrameWnd::GetWindowClassName(void) const
{
	return _T("MainFrameWnd");
}

CControlUI* CMainFrameWnd::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, szProblemListUIInterFace) == 0)
	{
		return new CProblemListUI(m_PaintManager);
	}
		
	return NULL;
}

void CMainFrameWnd::Notify(TNotifyUI& msg)
{
	__super::Notify(msg);

	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("closebtn"))
		{
			Close();
			PostQuitMessage(0);
		}
	}
}

void CMainFrameWnd::InitWindow()
{
	m_pProblemList = static_cast<CProblemListUI*>(m_PaintManager.FindControl(szProblemList));
	if (m_pProblemList != NULL)
	{
		m_pProblemList->AddGroup(_T("����"));
		m_pProblemList->AddGroup(_T("����"));
	}

	m_pProblemList->AddItem(_T("����"), FALSE);
}

LRESULT CMainFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}

	return FALSE;
}

LRESULT CMainFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnCreate(uMsg, wParam, lParam, bHandled);

	CWndShadow::Initialize(m_PaintManager.GetInstance());
	m_pWndShadow = new CWndShadow;
	m_pWndShadow->Create(m_hWnd);

#if 0
	RECT rcCorner = { 5,5,5,5 };
	RECT rcHoleOffset = { 0,0,0,0 };
	m_pWndShadow->SetImage(_T("bg_main_shadow.png"), rcCorner, rcHoleOffset);
#else
	m_pWndShadow->SetDarkness(50);
	m_pWndShadow->SetSize(10);
	m_pWndShadow->SetPosition(0, 0);
#endif

	return 0;
}
