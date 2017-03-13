#ifndef __EXAMINATION_H__
#define __EXAMINATION_H__

#include <thread>
#include "MainFrameWnd.h"
#include "DBDataMgr.h"

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
	enum EXAMINATION_TYPE
	{
		EXT_TYPE_CLEAN,					// ����
		EXT_TYPE_SPEEDUP				// ����
	};

	enum EXAMINATION_KEYTYPE
	{
		EXT_KEYTYPE_INT,				// ��ֵ����
		EXT_KEYTYPE_STRING,				// �ַ�������
		EXT_KEYTYPE_BINARY				// ����������
	};

	enum EXAMINATION_OPERATION
	{
		EXT_OPT_DIRECTORY,				// ����Ŀ¼
		EXT_OPT_FILE,					// �����ļ�
		EXT_OPT_REG32,					// ����ע���
		EXT_OPT_REG64,					// ����ע���
	};

	enum EXAMINATION_COMPATISON
	{
		EXT_COMPT_LT,
		EXT_COMPT_GT,
		EXT_COMPT_EQUAL,
		EXT_COMPT_LTE,
		EXT_COMPT_GTE
	};

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

	CDBDataMgr*				m_pDBMgr;

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
