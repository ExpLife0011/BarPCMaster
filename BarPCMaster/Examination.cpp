#include "stdafx.h"
#include "Examination.h"
#include "ControlNames.h"


CExamination::CExamination(CPaintManagerUI& PaintManagerUI)
	: m_PaintManager(PaintManagerUI)
	, m_bIsRunning(FALSE)
	, m_bIsStop(FALSE)
	, m_pProblemListUI(nullptr)
	, m_pExaminationBtn(nullptr)
	, m_pReturnBtn(nullptr)
	, m_pTipsText(nullptr)
	, m_pExamationThread(nullptr)
	, m_pVLayoutMain(nullptr)
	, m_pVLayoutExamination(nullptr)
	, m_pProgressFront(nullptr)
{
}


CExamination::~CExamination()
{
}

VOID CExamination::Init()
{
	m_pProblemListUI = static_cast<CProblemListUI*>(m_PaintManager.FindControl(bpcProblemList));
	m_pExaminationBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(bpcBtnExamination));
	m_pReturnBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(bpcBtnReturn));
	m_pTipsText = static_cast<CTextUI*>(m_PaintManager.FindControl(bpcTextTooltip));
	m_pVLayoutMain = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(bpcTabLayoutMain));
	m_pVLayoutExamination = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(bpcTabLayoutExamination));
	m_pProgressFront = static_cast<CProgressUI*>(m_PaintManager.FindControl(bpcProgressFront));
}

VOID CExamination::Back()
{
	m_pReturnBtn->SetVisible(FALSE);
	m_pVLayoutExamination->SetVisible(FALSE);
	m_pVLayoutMain->SetVisible(TRUE);

	m_pTipsText->SetText(_T(""));
	m_pExaminationBtn->SetText(_T("��������"));
	m_pProblemListUI->RemoveAll();
}

BOOL CExamination::Start()
{
	BOOL bRet = FALSE;

	if (nullptr != m_pProblemListUI)
	{
		m_bIsStop = FALSE;
		m_pVLayoutMain->SetVisible(FALSE);
		m_pVLayoutExamination->SetVisible(TRUE);
		m_pExamationThread = new std::thread(BeginExamation, this);
		bRet = TRUE;
	}

	return bRet;
}

BOOL CExamination::Stop()
{
	m_bIsStop = TRUE;
	return TRUE;
}

BOOL CExamination::IsRunning()
{
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

	m_pExaminationBtn->SetText(_T("ֹͣ���"));
	m_pExaminationBtn->SetBkColor(0xFFE7614E);

	m_pProblemListUI->RemoveAll();

	m_pTipsText->SetText(_T("���ڼ��ϵͳ�����������ļ�..."));

	m_pProblemListUI->AddGroup(_T("����"));
	for (int i = 0; i < 8 && m_bIsStop == FALSE; i++)
	{
		strText.Format(_T("���������� %d ���������"), i);
		m_pProblemListUI->AddItem(_T("����"), strText.GetData());
		m_pProgressFront->SetValue(static_cast<int>((static_cast<float>(i) / 8) * 50));
		Sleep(300);
	}

	m_pProblemListUI->AddGroup(_T("����"));
	for (int i = 0; i < 8 && m_bIsStop == FALSE; i++)
	{
		strText.Format(_T("ϵͳ���ٵ� %d ���������"), i);
		m_pProblemListUI->AddItem(_T("����"), strText.GetData());
		m_pProgressFront->SetValue(static_cast<int>((static_cast<float>(i) / 8) * 50) + 50);
		Sleep(300);
	}

	m_pProgressFront->SetValue(100);
	m_pReturnBtn->SetVisible(TRUE);
	m_pTipsText->SetText(_T("�п��Ż�������������д���"));
	m_pExaminationBtn->SetBkColor(0xFFED8033);
	m_pExaminationBtn->SetText(_T("��������"));
	m_bIsRunning = FALSE;
}

