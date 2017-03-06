#pragma once
#include "UIProblemList.h"
#include <thread>

class CExamination
{
public:
	CExamination(CPaintManagerUI& PaintManagerUI);
	~CExamination();

	VOID		Init();
	VOID		Back();

	BOOL		Start();
	BOOL		Stop();
	BOOL		IsRunning();

	static VOID BeginExamation(CExamination* pExamination);
	VOID		ExamationThreadInstance();

private:
	BOOL					m_bIsRunning;
	BOOL					m_bIsStop;
	CPaintManagerUI&		m_PaintManager;

	CProblemListUI*			m_pProblemListUI;		// �����б�
	CButtonUI*				m_pExaminationBtn;		// ������찴ť
	CButtonUI*				m_pReturnBtn;			// ���ذ�ť
	CTextUI*				m_pTipsText;			// ��ʾ��Ϣ
	CProgressUI*			m_pProgressFront;		// ������

	CVerticalLayoutUI*		m_pVLayoutMain;			// ������
	CVerticalLayoutUI*		m_pVLayoutExamination;	// ������

	std::thread*			m_pExamationThread;		// ����߳�

};

