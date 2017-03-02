#include "StdAfx.h"
#include "MainFrameWnd.h"
#include "ControlNames.h"


CMainFrameWnd::CMainFrameWnd(CDuiString strXMLPath)
	: /*m_pShadowUI(NULL)
	, */m_pProblemList(NULL)
	, m_strXMLPath(strXMLPath)
{
}


CMainFrameWnd::~CMainFrameWnd()
{
	/*if (NULL != m_pShadowUI)
	{
		delete m_pShadowUI;
		m_pShadowUI = NULL;
	}*/
}

DuiLib::CDuiString CMainFrameWnd::GetSkinFolder()
{
	return m_PaintManager.GetInstancePath();
}

DuiLib::CDuiString CMainFrameWnd::GetSkinFile()
{
	return m_strXMLPath;
}

LPCTSTR CMainFrameWnd::GetWindowClassName(void) const
{
	return _T("MainFrameWnd");
}

CControlUI* CMainFrameWnd::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, bPCMasterProblemListUIInterFace) == 0)
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
		if (msg.pSender->GetName() == _T("return"))
		{
			m_pProblemList->AddItem(_T("����"), _T("˳����ά��ʦ��������ļ�"), FALSE);
			m_pProblemList->AddItem(_T("����"), _T("ϵͳ��ʱĿ¼�����ļ�"), FALSE);
			m_pProblemList->AddItem(_T("����"), _T("Chrome ����������ļ�"), FALSE);
			m_pProblemList->AddItem(_T("����"), _T("IE ����������ļ��� Cookies"), FALSE);
			m_pProblemList->AddItem(_T("����"), _T("����ϵͳ��־"), FALSE);
			m_pProblemList->AddItem(_T("����"), _T("ϵͳ������־�ļ�"), FALSE);
			m_pProblemList->AddItem(_T("����"), _T("��������Ŀ¼�����ļ�"), FALSE);
			m_pProblemList->AddItem(_T("����"), _T("ϵͳ������װ���ݳ���"), FALSE);
		}
	}
}

void CMainFrameWnd::InitWindow()
{
	m_pProblemList = static_cast<CProblemListUI*>(m_PaintManager.FindControl(bPCMasterProblemListUIInferFace));
	if (m_pProblemList != NULL)
	{
		m_pProblemList->AddGroup(_T("����"));
		m_pProblemList->AddGroup(_T("����"));

		m_pProblemList->AddItem(_T("����"), _T("˳����ά��ʦ��������ļ�"), FALSE);
		m_pProblemList->AddItem(_T("����"), _T("ϵͳ��ʱĿ¼�����ļ�"), FALSE);
		m_pProblemList->AddItem(_T("����"), _T("Chrome ����������ļ�"), FALSE);
		m_pProblemList->AddItem(_T("����"), _T("IE ����������ļ��� Cookies"), FALSE);
		m_pProblemList->AddItem(_T("����"), _T("����ϵͳ��־"), FALSE);
		m_pProblemList->AddItem(_T("����"), _T("ϵͳ������־�ļ�"), FALSE);
		m_pProblemList->AddItem(_T("����"), _T("��������Ŀ¼�����ļ�"), FALSE);
		m_pProblemList->AddItem(_T("����"), _T("ϵͳ������װ���ݳ���"), FALSE);
	}
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

	/*CShadowUI::Initialize(m_PaintManager.GetInstance());
	m_pShadowUI = new CShadowUI;
	m_pShadowUI->Create(m_hWnd);*/

#if 0
	RECT rcCorner = { 5,5,5,5 };
	RECT rcHoleOffset = { 0,0,0,0 };
	m_pWndShadow->SetImage(_T("bg_main_shadow.png"), rcCorner, rcHoleOffset);
#else
	/*
	m_pShadowUI->SetDarkness(50);
	m_pShadowUI->SetSize(10);
	m_pShadowUI->SetPosition(0, 0);
	*/
#endif

	return 0;
}
