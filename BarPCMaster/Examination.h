#ifndef __EXAMINATION_H__
#define __EXAMINATION_H__

#include <thread>
#include "MainFrameWnd.h"

/*

	���� Effective C++ ����ʹ��ȫ�� const ��� #define
	������ȫ�ֱ������ܻ�ܶ࣬�����ʹ�ú����ļ���Ҳ���Ǻܴ�
	���ȫ�� const ������ʶ����ʱʹ�ú괦��
	const DWORD bpcdwStopColor = 0xFFE7614E;
	const DWORD bpcdwStartColor = 0xFFED8033;
*/

#define STOP_BTN_BKCOLOR	0xFFE7614E
#define START_BTN_BKCOLOR	0xFFED8033

class CMainFrameWnd;
class CExamination
{
public:
	CExamination(CMainFrameWnd* pMainFrameWnd);
	~CExamination();

	BOOL		Start();
	BOOL		Stop();
	BOOL		IsRunning();

	static VOID BeginExamation(CExamination* pExamination);
	VOID		ExamationThreadInstance();

private:
	BOOL					m_bIsRunning;
	BOOL					m_bIsStop;

	CMainFrameWnd*			m_pMainFrameWnd;

	//CProblemListUI*			m_pProblemListUI;		// �����б�
	//CButtonUI*				m_pExaminationBtn;		// ������찴ť
	//CButtonUI*				m_pReturnBtn;			// ���ذ�ť
	//CTextUI*				m_pTipsText;			// ��ʾ��Ϣ
	//CProgressUI*			m_pProgressFront;		// ������

	//CVerticalLayoutUI*		m_pVLayoutMain;			// ������
	//CVerticalLayoutUI*		m_pVLayoutExamination;	// ������

	std::thread*			m_pExamationThread;		// ����߳�

};

#endif
