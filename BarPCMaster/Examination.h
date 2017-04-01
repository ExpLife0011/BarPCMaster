#ifndef __EXAMINATION_H__
#define __EXAMINATION_H__

#include <thread>
#include "MainFrameWnd.h"
#include "DBDataMgr.h"
#include "GlobalDefine.h"

/*

	���� Effective C++ ����ʹ��ȫ�� const ��� #define
	������ȫ�ֱ������ܻ�ܶ࣬�����ʹ�ú����ļ���Ҳ���Ǻܴ�
	���ȫ�� const ������ʶ����ʱʹ�ú괦��
	const DWORD bpcdwStopColor = 0xFFE7614E;
	const DWORD bpcdwStartColor = 0xFFED8033;
*/

#define STOP_BTN_BKCOLOR	0xFFE7614E
#define START_BTN_BKCOLOR	0xFFED8033

#define ERROR_INVALID_KEY				0xE0000001
#define ERROR_VALUE_ARE_NOT_EQUAL		0xE0000002

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

	BOOL		Clean(PROBLEMITEM& ProblemData);
	BOOL		SpeedUp(PROBLEMITEM& ProblemData);

	// ���ܺ���
	DWORD		ScanRegistry(const PROBLEMITEM& refProblemData);
	DWORD		ScanRegistryEx(const PROBLEMITEM& refProblemData);

	DWORD		SearchDirectory(LPCTSTR szDirectory, LPCTSTR szType, LONGLONG& dwFileSize);
	DWORD		SearchRegistry(const PROBLEMITEM& ProblemData, __out LPBYTE& pData);
	DWORD		SearchRegistryEx(const PROBLEMITEM& ProblemData);

	BOOL		CompareString(const PBYTE pData, CDuiString strDefaultValue, int nComparison, int nCompareType);
	BOOL		CompareDWORD(const PBYTE pData, CDuiString strDefaultValue, int nComparison, int nCompareType);

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
