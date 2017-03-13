#include "stdafx.h"
#include "Examination.h"
#include "ControlNames.h"
#include "MainFrameWnd.h"


CExamination::CExamination(CMainFrameWnd* pMainFrameWnd)
	: m_pMainFrameWnd(pMainFrameWnd)
	, m_bIsRunning(FALSE)
	, m_bIsStop(FALSE)
	, m_pExamationThread(nullptr)
	, m_pDBMgr(nullptr)
{
}


CExamination::~CExamination()
{
	if (nullptr != m_pDBMgr)
	{
		delete m_pDBMgr;
		m_pDBMgr = nullptr;
	}
}

BOOL CExamination::Start()
{
	BOOL bRet = FALSE;

	if (nullptr == m_pDBMgr)
	{
		TCHAR szFilePath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		PathRemoveFileSpec(szFilePath);
		PathAppend(szFilePath, SQLITE_DB_FOLDER);
		if (_taccess(szFilePath, 0) == -1)
		{
			CreateDirectory(szFilePath, NULL);
		}
		PathAppend(szFilePath, SQLITE_DB_FILENAME);

		m_pDBMgr = new CDBDataMgr(szFilePath);
		m_pDBMgr->Init();
	}

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
	CDuiString					strText = _T("");
	std::vector<EXAMINATION>	vecExamination;
	BOOL						bSuccess = FALSE;
	UINT						uCount = 0;

	// ����Ϊ����״̬
	m_bIsRunning = TRUE;

	// ���ø����ؼ�״̬
	m_pMainFrameWnd->SetBtnText(_T("ֹͣ���"));
	m_pMainFrameWnd->SetBtnBkColor(STOP_BTN_BKCOLOR);
	m_pMainFrameWnd->SetTipsText(_T("���ڼ��ϵͳ�����������ļ�..."));


	do 
	{
		// ��ȡ��������
		bSuccess = m_pDBMgr->GetAllData(vecExamination);
		if (!bSuccess)
		{
			break;
		}

		// ������ʼɨ��
		std::vector<EXAMINATION>::iterator iter = vecExamination.begin();
		for (; iter != vecExamination.end() && m_bIsStop == FALSE; iter++)
		{
			EXAMINATION&	stRowData = *iter;
			CDuiString		strGroup = _T("");
			CDuiString		strTipsText = _T("");

			strTipsText.Format(_T("��� %s ..."), stRowData.strDescription);
			m_pMainFrameWnd->SetTipsText(strTipsText);

			switch (stRowData.nOperation)
			{
			case EXT_OPT_DIRECTORY:
				break;
			case EXT_OPT_FILE:
				break;
			case EXT_OPT_REG32:
				break;
			case EXT_OPT_REG64:
				break;
			}

			switch (stRowData.nType)
			{
			case EXT_TYPE_CLEAN:
				strGroup = _T("����");
				break;
			case EXT_TYPE_SPEEDUP:
				strGroup = _T("����");
				break;
			}

			m_pMainFrameWnd->AddItemToList(strGroup, stRowData.strDescription);
			m_pMainFrameWnd->SetProgressValue(static_cast<int>((static_cast<float>(++uCount) / vecExamination.size()) * 100));

			Sleep(1000);
		}

		/*m_pMainFrameWnd->AddGroupToList(_T("����"));
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
		}*/
	} while (FALSE);
	

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

