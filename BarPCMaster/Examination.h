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

#define TOTALBYTES			2048

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
		EXT_KEYTYPE_DWORD,				// 32λ DWORD ����
		EXT_KEYTYPE_STRING,				// �ַ�������
	};

	enum EXAMINATION_OPERATION
	{
		EXT_OPT_FILES,					// ����ָ���ļ�
		EXT_OPT_REG32,					// 32λע���
		EXT_OPT_REG64,					// 64λע���
	};

	enum EXAMINATION_COMPATISON
	{
		EXT_COMPT_LT = -2,				// С��
		EXT_COMPT_LTE,					// С�ڵ���
		EXT_COMPT_EQUAL,				// ����
		EXT_COMPT_GTE,					// ���ڵ���
		EXT_COMPT_GT,					// ����
	};

public:
	CExamination(CMainFrameWnd* pMainFrameWnd);
	~CExamination();

	BOOL		Start();
	BOOL		Stop();
	BOOL		IsRunning();

	static VOID BeginExamation(CExamination* pExamination);
	VOID		ExamationThreadInstance();

	// ���ܺ���
	DWORD		ScanDirectory(LPCTSTR szDirectory, LPCTSTR szType, LONGLONG& dwFileSize);
	DWORD		ScanRegistry(const PROBLEMITEM& refProblemData);

	CDuiString	GetSizeString(LONGLONG llSize);

private:
	BOOL						m_bIsRunning;
	BOOL						m_bIsStop;
	CDBDataMgr*					m_pDBMgr;
	CMainFrameWnd*				m_pMainFrameWnd;
	std::thread*				m_pExamationThread;	// ����߳�
	std::vector<PROBLEMITEM>	m_vecProblemList;	// �����б�

};

#endif
