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
	BOOL		bSuccess = FALSE;
	UINT		uCount = 0;
	CDuiString	strText = _T("");

	// ����Ϊ����״̬
	m_bIsRunning = TRUE;

	// ���ø����ؼ�״̬
	m_pMainFrameWnd->SetBtnText(_T("ֹͣ���"));
	m_pMainFrameWnd->SetBtnBkColor(STOP_BTN_BKCOLOR);
	m_pMainFrameWnd->SetTipsText(_T("���ڼ��ϵͳ�����������ļ�..."));

	// ��������б�
	m_vecProblemList.clear();

	do 
	{
		// ��ȡ��������
		bSuccess = m_pDBMgr->GetAllData(m_vecProblemList);
		if (!bSuccess)
		{
			break;
		}

		// ������ʼɨ��
		for (auto &stRowData : m_vecProblemList)
		{
			CDuiString	strGroup	= _T("");
			CDuiString	strTipsText = _T("");

			strTipsText.Format(_T("��� %s ..."), stRowData.strDescription);
			m_pMainFrameWnd->SetTipsText(strTipsText);

			switch (stRowData.nType)
			{
			case EXT_TYPE_CLEAN:
				{
					if (!Clean(stRowData))
					{
						continue;
					}

					strGroup = _T("����");
					break;
				}
			case EXT_TYPE_SPEEDUP:
				{
					if (!SpeedUp(stRowData))
					{
						continue;
					}

					strGroup = _T("����");
					break;
				}
			}

			m_pMainFrameWnd->AddItemToList(strGroup, &stRowData);
			m_pMainFrameWnd->SetProgressValue(static_cast<int>((static_cast<float>(++uCount) / m_vecProblemList.size()) * 100));
		}
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
	
	LOG(INFO) << _T("Examine finished.");

	m_bIsRunning = FALSE;
}

BOOL CExamination::Clean(PROBLEMITEM& ProblemData)
{
	BOOL bRet = FALSE;

	// �жϲ�������
	switch (ProblemData.nOperation)
	{
	case EXT_OPT_FILES:
		{
			DWORD	 dwError = 0;
			LONGLONG llSize = 0;

			dwError = SearchDirectory(ProblemData.strPath, ProblemData.strKey, llSize);
			if (0 != dwError || 0 == llSize)
			{
				// ������ִ������ɨ���Ŀ¼û�������ļ�����ô����
				LOG(INFO) << _T("Has no files, dwError = ") << dwError
					<< _T(", llSize = ") << llSize;
				break;
			}

			// �޸���ʾ���֣����������ļ���С
			ProblemData.strDescription += _T(" (");
			ProblemData.strDescription += GetSizeString(llSize);
			ProblemData.strDescription += _T(")");
			LOG(INFO) << _T("ɨ��Ŀ¼ ") << ProblemData.strPath.GetString()
				<< _T(" �в����ļ���С ") << llSize << " Byte.";

			bRet = TRUE;
			break;
		}
	case EXT_OPT_REG64:
	case EXT_OPT_REG32:
		{
			DWORD dwError = 0;
			PBYTE pData = nullptr;

			dwError = SearchRegistry(ProblemData, pData);

			if (0 != dwError)
			{
				// û���ҵ�ָ����ע�������ݲ�ƥ��
				LOG(INFO) << _T("Registry key not found, ")
					<< ProblemData.strPath << _T(", key = ")
					<< ProblemData.strKey;
				if (nullptr != pData)
				{
					delete[] pData;
					pData = nullptr;
				}
				break;
			}
			
			if (nullptr != pData)
			{
				delete[] pData;
				pData = nullptr;
			}
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL CExamination::SpeedUp(PROBLEMITEM & ProblemData)
{
	BOOL bRet = FALSE;

	switch (ProblemData.nOperation)
	{
	case EXT_OPT_FILES:
		{
			break;
		}
	case EXT_OPT_REG64:
	case EXT_OPT_REG32:
		{
			DWORD dwError = 0;

			dwError = SearchRegistryEx(ProblemData);
			if (0 != dwError)
			{
				// û���ҵ�ָ����ע�������ݲ�ƥ��
				LOG(INFO) << _T("Registry key not found, ")
					<< ProblemData.strPath << _T(", key = ")
					<< ProblemData.strKey;
				break;
			}

			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

DWORD CExamination::SearchDirectory(LPCTSTR szDirectory, LPCTSTR szType, LONGLONG& dwFileSize)
{
	DWORD dwError = 0;
	TCHAR szFindPath[MAX_PATH] = { 0 };
	WIN32_FIND_DATA ffd = { 0 };
	LARGE_INTEGER nFileSize;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	_tcscpy_s(szFindPath, szDirectory);
	PathAppend(szFindPath, _T("*.*"));

	hFind = FindFirstFile(szFindPath, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		dwError = GetLastError();
		LOG(ERROR) << _T("Find first file return error, error code = ") << dwError;
	}
	else
	{
		do
		{
			// �ų�����վ���Լ�Ŀ¼����Ŀ¼
			if (0 == _tcscmp(ffd.cFileName, _T(".")) ||
				0 == _tcscmp(ffd.cFileName, _T("..")) ||
				0 == _tcscmp(ffd.cFileName, _T("$Recycle.Bin")))
			{
				continue;
			}

			_tcscpy(szFindPath, szDirectory);
			PathAppend(szFindPath, ffd.cFileName);

			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				dwError = SearchDirectory(szFindPath, szType, dwFileSize);
			}
			else
			{
				nFileSize.LowPart = ffd.nFileSizeLow;
				nFileSize.HighPart = ffd.nFileSizeHigh;

				if (_tcscmp(szType, _T("*")) == 0)
				{
					// ���������ļ���С
					dwFileSize += nFileSize.QuadPart;
					LOG(INFO) << _T("Found file ") << szFindPath;
				}
				else
				{
					// ͬ��׺�����ļ��ż����С
					if (_tcscmp(PathFindExtension(szType), PathFindExtension(ffd.cFileName)) == 0)
					{
						LOG(INFO) << _T("Found file ") << szFindPath;
						dwFileSize += nFileSize.QuadPart;
					}
				}
				
			}
		} while (FindNextFile(hFind, &ffd) != 0);
	}

	FindClose(hFind);
	return dwError;
}

DWORD CExamination::SearchRegistry(const PROBLEMITEM& ProblemData, __out LPBYTE& pData)
{
	DWORD	dwError = 0;
	HKEY	hKey = nullptr;
	HKEY	hPrimaryKey = nullptr;
	LONG	lRet = 0;
	DWORD	cbData = 0;
	PBYTE	pszBuffer = nullptr;

	do
	{
		// �ҵ���һ��б��
		int nFind = ProblemData.strPath.Find(_T('\\'));
		if (0 == nFind)
		{
			dwError = ERROR_INVALID_KEY;
			break;
		}

		// �õ�����λ��
		CDuiString strPrimaryKey = ProblemData.strPath.Mid(0, nFind);
		if (strPrimaryKey == _T("HKEY_LOCAL_MACHINE"))
		{
			hPrimaryKey = HKEY_LOCAL_MACHINE;
		}
		else if (strPrimaryKey == _T("HKEY_CURRENT_USER"))
		{
			hPrimaryKey = HKEY_CURRENT_USER;
		}
		else if (strPrimaryKey == _T("HKEY_CLASSES_ROOT"))
		{
			hPrimaryKey = HKEY_CLASSES_ROOT;
		}
		else if (strPrimaryKey == _T("HKEY_USERS"))
		{
			hPrimaryKey = HKEY_USERS;
		}
		else
		{
			dwError = ERROR_INVALID_KEY;
			LOG(ERROR) << _T("Invalid primary key: ") << strPrimaryKey;
			break;
		}

		// �õ��Ӽ�λ��
		CDuiString strSubKey = ProblemData.strPath.Mid(nFind + 1,
			ProblemData.strPath.GetLength());

		lRet = RegOpenKeyEx(hPrimaryKey, strSubKey.GetData(), 0,
			KEY_QUERY_VALUE |
			(ProblemData.nOperation == EXT_OPT_REG32 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY),
			&hKey);
		if (ERROR_SUCCESS != lRet)
		{
			dwError = lRet;
			LOG(ERROR) << _T("Failed to open key ") << strPrimaryKey.GetData()
				<< _T("\\") << strSubKey.GetData() << _T(", error code = ")
				<< lRet;
			break;
		}

		lRet = RegQueryValueEx(hKey,
			ProblemData.strKey.GetString(),
			NULL,
			NULL,
			reinterpret_cast<LPBYTE>(pszBuffer),
			&cbData);
		if (ERROR_SUCCESS != lRet)
		{
			dwError = lRet;
			LOG(ERROR) << _T("Failed to query key ") << strPrimaryKey.GetData()
				<< _T("\\") << strSubKey.GetData() << _T(" -> ")
				<< ProblemData.strKey.GetString() << _T(", error code = ")
				<< lRet;
			break;
		}

		pszBuffer = new BYTE[cbData];
		pData = new BYTE[cbData];

		lRet = RegQueryValueEx(hKey,
			ProblemData.strKey.GetString(),
			NULL,
			NULL,
			reinterpret_cast<LPBYTE>(pszBuffer),
			&cbData);
		if (ERROR_SUCCESS != lRet)
		{
			dwError = lRet;
			LOG(ERROR) << _T("Failed to query key ") << strPrimaryKey.GetData()
				<< _T("\\") << strSubKey.GetData() << _T(" -> ")
				<< ProblemData.strKey.GetString() << _T(", error code = ")
				<< lRet;
			break;
		}

		// ����ѯ����ֵ����
		CopyMemory(pData, pszBuffer, cbData);

		// ������ַ�������
		if (ProblemData.nKeyType == EXT_KEYTYPE_STRING)
		{
			if (_tcsicmp(reinterpret_cast<TCHAR*>(pszBuffer), ProblemData.strValue.GetString()) == 0)
			{
				LOG(INFO) << _T("");
				RegCloseKey(hKey);
				break;
			}
			else
			{
				RegCloseKey(hKey);
				dwError = ERROR_VALUE_ARE_NOT_EQUAL;
				LOG(ERROR) << pszBuffer << _T(" is not equal to ")
					<< ProblemData.strValue.GetString();
				break;
			}
		}

		// ����� DWORD ����
		if (ProblemData.nKeyType == EXT_KEYTYPE_DWORD)
		{
			DWORD nTmpValue = *(reinterpret_cast<DWORD*>(pszBuffer));
			DWORD nValue = _ttoi(ProblemData.strValue.GetString());

			// ����ҵ�ֱ������
			if (nTmpValue == nValue)
			{
				LOG(INFO) << _T("Found value ") << nTmpValue << _T("==") << nValue;
				RegCloseKey(hKey);
				break;
			}
			else
			{
				RegCloseKey(hKey);
				dwError = ERROR_VALUE_ARE_NOT_EQUAL;
				LOG(ERROR) << nTmpValue << _T(" is not equal to ") << nValue;
				break;
			}
		}

		RegCloseKey(hKey);

	} while (FALSE);

	if (nullptr != pszBuffer)
	{
		delete[] pszBuffer;
	}

	return dwError;
}

DWORD CExamination::SearchRegistryEx(const PROBLEMITEM& ProblemData)
{
	DWORD	dwError = 0;
	BOOL	bRet = FALSE;
	PBYTE	pData = nullptr;

	dwError = SearchRegistry(ProblemData, pData);

	do 
	{
		// ����ֵ�����ڵ�ʱ��Ҫ������Ҫ���ٴ���
		if (ERROR_FILE_NOT_FOUND == dwError)
		{
			dwError = 0;
			break;
		}

		// ֵ���ڵ������Ҫ��Ĭ��ֵ���бȶ�
		if (ProblemData.nKeyType == EXT_KEYTYPE_STRING)
		{
			// �Ա��ַ��������������������Ҫ�Ż�������������ʾ�ڽ�����
			bRet = CompareString(pData, 
				ProblemData.strValue.GetString(),
				ProblemData.nComparison, 
				ProblemData.nCompareType);
			dwError = static_cast<DWORD>(bRet);
			break;
		}
		
		if (ProblemData.nKeyType == EXT_KEYTYPE_DWORD)
		{
			// �Ա� DWORD ֵ�����������������Ҫ�Ż�������������ʾ�ڽ�����
			bRet = CompareDWORD(pData,
				ProblemData.strValue.GetString(),
				ProblemData.nComparison,
				ProblemData.nCompareType);
			dwError = static_cast<DWORD>(bRet);
			break;
		}
	} while (FALSE);

	if (nullptr != pData)
	{
		delete[] pData;
		pData = nullptr;
	}

	return dwError;
}

BOOL CExamination::CompareString(const PBYTE pData, CDuiString strDefaultValue, int nComparison, int nCompareType)
{
	BOOL bRet = FALSE;

	// ��ע���ȡ���������п������ַ�����ʽ�ģ�����Ҫת������ֵ�����ٱȶ�
	// ���ݿ��� CompareType ������Ҫ��ʲô��ʽ�Ա�
	// ����Աȷ�ʽ�����ַ�����ʽ�Աȣ���ôֱ�� strcmp���������������Ҫ�����
	if (nCompareType == EXT_KEYTYPE_STRING)
	{
		if (_tcsicmp(reinterpret_cast<TCHAR*>(pData), strDefaultValue.GetData()) == 0)
		{
			bRet = TRUE;
		}
	}
	
	// ����Աȷ�ʽ����DWORD��ֵ���ͶԱȣ���ôǿת�����ٶԱȣ��������ݿ����öԱȴ�С
	if (nCompareType == EXT_KEYTYPE_DWORD)
	{
		TCHAR*	pszData = reinterpret_cast<TCHAR*>(pData);
		DWORD	dwData = _ttoi(pszData);
		DWORD	dwDefaultValue = _ttoi(strDefaultValue.GetData());

		switch (nComparison)
		{
		case EXT_COMPT_LT:
			if (dwData < dwDefaultValue)
			{
				bRet = TRUE;
			}
			break;
		case EXT_COMPT_LTE:
			if (dwData <= dwDefaultValue)
			{
				bRet = TRUE;
			}
			break;
		case EXT_COMPT_EQUAL:
			if (dwData == dwDefaultValue)
			{
				bRet = TRUE;
			}
			break;
		case EXT_COMPT_GTE:
			if (dwData >= dwDefaultValue)
			{
				bRet = TRUE;
			}
			break;
		case EXT_COMPT_GT:
			if (dwData > dwDefaultValue)
			{
				bRet = TRUE;
			}
			break;
		}
	}

	return bRet;
}

BOOL CExamination::CompareDWORD(const PBYTE pData, CDuiString strDefaultValue, int nComparison, int nCompareType)
{
	BOOL bRet = FALSE;

	DWORD dwData = *(reinterpret_cast<DWORD*>(pData));
	DWORD dwDefaultValue = _ttoi(strDefaultValue.GetData());

	switch (nComparison)
	{
	case EXT_COMPT_LT:
		if (dwData < dwDefaultValue)
		{
			bRet = TRUE;
		}
		break;
	case EXT_COMPT_LTE:
		if (dwData <= dwDefaultValue)
		{
			bRet = TRUE;
		}
		break;
	case EXT_COMPT_EQUAL:
		if (dwData == dwDefaultValue)
		{
			bRet = TRUE;
		}
		break;
	case EXT_COMPT_GTE:
		if (dwData >= dwDefaultValue)
		{
			bRet = TRUE;
		}
		break;
	case EXT_COMPT_GT:
		if (dwData > dwDefaultValue)
		{
			bRet = TRUE;
		}
		break;
	}

	return bRet;
}

DWORD CExamination::ScanRegistry(const PROBLEMITEM& refProblemData)
{
	DWORD		dwError = 0;
	HKEY		hKey = nullptr;
	HKEY		hPrimaryKey = nullptr;
	LONG		lRet = 0;
	DWORD		cbData = 0;
	PBYTE		pszBuffer = nullptr;

	do 
	{
		// �ҵ���һ��б��
		int nFind = refProblemData.strPath.Find(_T('\\'));
		if (0 == nFind)
		{
			dwError = -1;
			break;
		}

		// �õ�����λ��
		CDuiString strPrimaryKey = refProblemData.strPath.Mid(0, nFind);
		if (strPrimaryKey == _T("HKEY_LOCAL_MACHINE"))
		{
			hPrimaryKey = HKEY_LOCAL_MACHINE;
		}
		else if (strPrimaryKey == _T("HKEY_CURRENT_USER"))
		{
			hPrimaryKey = HKEY_CURRENT_USER;
		}
		else if (strPrimaryKey == _T("HKEY_CLASSES_ROOT"))
		{
			hPrimaryKey = HKEY_CLASSES_ROOT;
		}
		else if (strPrimaryKey == _T("HKEY_USERS"))
		{
			hPrimaryKey = HKEY_USERS;
		}
		else
		{
			dwError = -1;
			LOG(ERROR) << _T("Invalid primary key: ") << strPrimaryKey;
			break;
		}

		// �õ��Ӽ�λ��
		CDuiString strSubKey = refProblemData.strPath.Mid(nFind + 1,
			refProblemData.strPath.GetLength());

		lRet = RegOpenKeyEx(hPrimaryKey, strSubKey.GetData(), 0,
			KEY_QUERY_VALUE | 
			(refProblemData.nOperation == EXT_OPT_REG32 ? KEY_WOW64_32KEY : KEY_WOW64_64KEY), 
			&hKey);
		if (ERROR_SUCCESS != lRet)
		{
			dwError = lRet;
			LOG(ERROR) << _T("Failed to open key ") << strPrimaryKey.GetData()
				<< _T("\\") << strSubKey.GetData() << _T(", error code = ")
				<< lRet;
			break;
		}

		lRet = RegQueryValueEx(hKey, 
			refProblemData.strKey.GetString(), 
			NULL, 
			NULL, 
			reinterpret_cast<LPBYTE>(pszBuffer),
			&cbData);
		if (ERROR_SUCCESS != lRet)
		{
			dwError = lRet;
			LOG(ERROR) << _T("Failed to query key ") << strPrimaryKey.GetData()
				<< _T("\\") << strSubKey.GetData() << _T(" -> ") 
				<< refProblemData.strKey.GetString() << _T(", error code = ")
				<< lRet;
			break;
		}

		pszBuffer = new BYTE[cbData];
		lRet = RegQueryValueEx(hKey,
			refProblemData.strKey.GetString(),
			NULL,
			NULL,
			reinterpret_cast<LPBYTE>(pszBuffer),
			&cbData);
		if (ERROR_SUCCESS != lRet)
		{
			dwError = lRet;
			LOG(ERROR) << _T("Failed to query key ") << strPrimaryKey.GetData()
				<< _T("\\") << strSubKey.GetData() << _T(" -> ")
				<< refProblemData.strKey.GetString() << _T(", error code = ")
				<< lRet;
			break;
		}

		// ������ַ�������
		if (refProblemData.nKeyType == EXT_KEYTYPE_STRING)
		{
			if (_tcsicmp(reinterpret_cast<TCHAR*>(pszBuffer), refProblemData.strValue.GetString()) == 0)
			{
				LOG(INFO) << _T("");
				break;
			}
			else
			{
				RegCloseKey(hKey);
				dwError = -1;
				LOG(ERROR) << pszBuffer << _T(" is not equal to ")
					<< refProblemData.strValue.GetString();
				if (nullptr != pszBuffer)
				{
					delete[] pszBuffer;
				}
				break;
			}
		}

		// ����� DWORD ����
		if (refProblemData.nKeyType == EXT_KEYTYPE_DWORD)
		{
			DWORD nTmpValue = *(reinterpret_cast<DWORD*>(pszBuffer));
			DWORD nValue = _ttoi(refProblemData.strValue.GetString());

			if (refProblemData.nComparison == EXT_COMPT_LT)
			{
				// С�����������ʵ��ֵ���ڵ��ڲο�ֵ����ô��������
				if (nTmpValue < nValue)
				{
					dwError = -1;
				}
			}
			if (refProblemData.nComparison == EXT_COMPT_LTE)
			{
				if (nTmpValue <= nValue)
				{
					dwError = -1;
				}
			}
			if (refProblemData.nComparison == EXT_COMPT_EQUAL)
			{
				if (nTmpValue == nValue)
				{
					dwError = -1;
				}
			}
			if (refProblemData.nComparison == EXT_COMPT_GTE)
			{
				if (nTmpValue >= nValue)
				{
					dwError = -1;
				}
			}
			if (refProblemData.nComparison == EXT_COMPT_GT)
			{
				if (nTmpValue > nValue)
				{
					dwError = -1;
				}
			}
		}

		if (nullptr != pszBuffer)
		{
			delete[] pszBuffer;
		}

		RegCloseKey(hKey);

	} while (FALSE);

	return dwError;
}

DWORD CExamination::ScanRegistryEx(const PROBLEMITEM& refProblemData)
{
	return 0;
}

CDuiString CExamination::GetSizeString(LONGLONG llSize)
{
	CDuiString strSize = _T("0 MB");

	if (llSize <= 0)
	{
		return strSize;
	}

	if (llSize < 1024)
	{
		strSize.Format(_T("%I64d Byte"), llSize);
		return strSize;
	}

	llSize /= 1024;

	if (llSize < 1024)
	{
		strSize.Format(_T("%I64d KB"), llSize);
		return strSize;
	}

	llSize /= 1024;

	if (llSize < 1024)
	{
		strSize.Format(_T("%I64d MB"), llSize);
		return strSize;
	}

	llSize /= 1024;

	if (llSize < 1024)
	{
		strSize.Format(_T("%I64d GB"), llSize);
		return strSize;
	}

	return strSize;
}

