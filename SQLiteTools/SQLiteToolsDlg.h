
// SQLiteToolsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DBDataMgr.h"
#include "GlobalDefine.h"


// CSQLiteToolsDlg �Ի���
class CSQLiteToolsDlg : public CDialogEx
{
// ����
public:
	CSQLiteToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSQLiteToolsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SQLITETOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	afx_msg void OnBnClickedOk();


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString		m_editDescript;
	CComboBox	m_comboboxType;
	CComboBox	m_comboboxOperation;
	CString		m_editPath;
	CString		m_editKey;
	CComboBox	m_comboboxKeyType;
	CComboBox	m_comboboxCompareType;
	CComboBox	m_comboboxComparison;
	CString		m_editDefaultValue;
	CListCtrl	m_listCtrl;

	CDBDataMgr*	m_pDataMgr;
	
};
