#pragma once

#include "CppSQLite3U.h"
#include <vector>

/*
���ݿ���غ�
*/
#define SQLITE_DB_FOLDER		_T("Data")
#define SQLITE_DB_FILENAME		_T("BarPCMaster.dat")
#define SQLITE_DB_TABLE			_T("EXAMINATION")
#define SQLITE_SQL_CREATE_TABLE	_T( \
"CREATE TABLE EXAMINATION(" \
"ID				INTEGER			PRIMARY KEY AUTOINCREMENT, " \
"DESCRIPTION	VARCHAR(50)		NOT NULL, " \
"STATUS			BOOL			NOT NULL, " \
"TYPE			INTEGER			NOT NULL, " \
"OPERATION		INTEGER			NOT NULL, " \
"PATH			VARCHAR(512)	NOT NULL, " \
"KEY			VARCHAR(256), " \
"KEYTYPE		VARCHAR(32), " \
"COMPARETYPE	INTEGER" \
"VALUE			VARCHAR(256), " \
"COMPARISON		INTEGER, " \
"NOTES			VARCHAR(256))")

/*
	DESCRIPTION		������������ʾ�ڽ�����
	STATUS			�����״̬���Ѿ���������Ѿ�����
	TYPE			�������ͣ������Ǽ��ٻ�������
	OPERATION		��ز���������Ŀ¼���Ż�ע����
	PATH			���������·��
	KEY				����������ļ���Ϣ����ע��� Key ��Ϣ
	KEYTYPE			���������ע���ֵ������Ϣ
	VALUE			���������ע���ֵ��Ϣ��Ҳ��һ�������Ĳο�ֵ
	COMPARISON		���ڻ���С�ڻ��ǵ��ڲο�ֵ
	NOTES			��ע��Ϣ������ʾ����ʱ����
*/

typedef struct _tag_PROBLEMITEM
{
	int				nID;
	CString		strDescription;
	BOOL			bStatus;
	int				nType;
	int				nOperation;
	CString		strPath;
	CString		strKey;
	int				nKeyType;
	int				nCompareType;
	CString		strValue;
	int				nComparison;
	CString		strNotes;
	void*			pControl;
} PROBLEMITEM, *PPROBLEMITEM;

class CDBDataMgr
{
public:
	CDBDataMgr(LPCTSTR lpPath);
	~CDBDataMgr();

	void	Init();
	UINT	GetRowCount();
	BOOL	GetAllData(std::vector<PROBLEMITEM>& vecExaminationList);
	BOOL	InsertItem(const PROBLEMITEM& pProblemItem);

private:
	CppSQLite3DB*	m_pDB;
	CString		m_strPath;
};

