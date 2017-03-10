#include "stdafx.h"
#include "Examination.h"
#include "ControlNames.h"
#include "MainFrameWnd.h"


CExamination::CExamination(CMainFrameWnd* pMainFrameWnd)
	: m_pMainFrameWnd(pMainFrameWnd)
	, m_bIsRunning(FALSE)
	, m_bIsStop(FALSE)
	, m_pExamationThread(nullptr)
{
}


CExamination::~CExamination()
{
}

BOOL CExamination::Start()
{
	BOOL bRet = FALSE;

	/*
		1����ʼ���
		2��������ѭ�� bIsStop ����Ϊ TRUE�����߳��ڲ���������ִ�� while ѭ��
		3����������߳�
	*/
	m_bIsStop = FALSE;
	m_pExamationThread = new std::thread(BeginExamation, this);
	if (nullptr != m_pExamationThread)
	{
		bRet = TRUE;
	}

	return bRet;
}

BOOL CExamination::Stop()
{
	// ֹͣ�߳������ڽ������� while ѭ��
	m_bIsStop = TRUE;
	return TRUE;
}

BOOL CExamination::IsRunning()
{
	// ���ص�ǰ�߳��Ƿ���������
	return m_bIsRunning;
}

VOID CExamination::BeginExamation(CExamination* pExamination)
{
	if (nullptr != pExamination)
	{
		pExamination->ExamationThreadInstance();
	}
}

VOID CExamination::ExamationThreadInstance()
{
	m_bIsRunning = TRUE;
	CDuiString strText;

	m_pMainFrameWnd->SetBtnText(_T("ֹͣ���"));
	m_pMainFrameWnd->SetBtnBkColor(STOP_BTN_BKCOLOR);
	m_pMainFrameWnd->SetTipsText(_T("���ڼ��ϵͳ�����������ļ�..."));

	m_pMainFrameWnd->AddGroupToList(_T("����"));
	for (int i = 0; i < 8 && m_bIsStop == FALSE; i++)
	{
		strText.Format(_T("���������� %d ���������"), i);
		m_pMainFrameWnd->AddItemToList(_T("����"), strText.GetData());
		m_pMainFrameWnd->SetProgressValue(static_cast<int>((static_cast<float>(i) / 8) * 50));
		Sleep(300);
	}

	m_pMainFrameWnd->AddGroupToList(_T("����"));
	for (int i = 0; i < 8 && m_bIsStop == FALSE; i++)
	{
		strText.Format(_T("ϵͳ���ٵ� %d ���������"), i);
		m_pMainFrameWnd->AddItemToList(_T("����"), strText.GetData());
		m_pMainFrameWnd->SetProgressValue(static_cast<int>((static_cast<float>(i) / 8) * 50) + 50);
		Sleep(300);
	}

	// �ж��Ƿ��п��Ż���
	if (TRUE)
	{
		m_pMainFrameWnd->SetTipsText(_T("�п��Ż�������������д���"));
		m_pMainFrameWnd->SetBtnBkColor(START_BTN_BKCOLOR);
		m_pMainFrameWnd->SetBtnText(_T("��������"));
	}

	// ������ֶ�ֹͣ�������ý�������100%
	if (!m_bIsStop)
	{
		m_pMainFrameWnd->SetProgressValue(100);
	}

	// ��ʾ���ذ�ť
	m_pMainFrameWnd->ShowReturnButton();
	
	m_bIsRunning = FALSE;
}

