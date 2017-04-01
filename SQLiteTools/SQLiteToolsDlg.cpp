
// SQLiteToolsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SQLiteTools.h"
#include "SQLiteToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSQLiteToolsDlg �Ի���



CSQLiteToolsDlg::CSQLiteToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SQLITETOOLS_DIALOG, pParent)
	, m_editPath(_T(""))
	, m_editKey(_T(""))
	, m_editDefaultValue(_T(""))
	, m_editDescript(_T(""))
	, m_pDataMgr(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSQLiteToolsDlg::~CSQLiteToolsDlg()
{
	if (nullptr != m_pDataMgr)
	{
		delete m_pDataMgr;
		m_pDataMgr = nullptr;
	}
}

void CSQLiteToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_editDescript);
	DDX_Control(pDX, IDC_COMBO1, m_comboboxType);
	DDX_Control(pDX, IDC_COMBO2, m_comboboxOperation);
	DDX_Text(pDX, IDC_EDIT3, m_editPath);
	DDX_Text(pDX, IDC_EDIT4, m_editKey);
	DDX_Control(pDX, IDC_COMBO3, m_comboboxKeyType);
	DDX_Control(pDX, IDC_COMBO4, m_comboboxCompareType);
	DDX_Control(pDX, IDC_COMBO5, m_comboboxComparison);
	DDX_Text(pDX, IDC_EDIT6, m_editDefaultValue);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CSQLiteToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSQLiteToolsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSQLiteToolsDlg ��Ϣ�������

BOOL CSQLiteToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ��ʼ�����ݿ���ָ��
	TCHAR szFilePath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	PathRemoveFileSpec(szFilePath);
	PathAppend(szFilePath, SQLITE_DB_FOLDER);
	if (_taccess(szFilePath, 0) == -1)
	{
		CreateDirectory(szFilePath, NULL);
	}
	PathAppend(szFilePath, SQLITE_DB_FILENAME);

	m_pDataMgr = new CDBDataMgr(szFilePath);

	// ��ʼ�� combobox ����
	m_comboboxType.InsertString(0, _T("����"));
	m_comboboxType.InsertString(1, _T("����"));
	m_comboboxType.SetCurSel(0);

	m_comboboxOperation.InsertString(0, _T("�ļ�����"));
	m_comboboxOperation.InsertString(1, _T("32ע���"));
	m_comboboxOperation.InsertString(2, _T("64ע���"));
	m_comboboxOperation.SetCurSel(0);

	m_comboboxKeyType.InsertString(0, _T("��ֵ����"));
	m_comboboxKeyType.InsertString(1, _T("�ַ�������"));
	m_comboboxKeyType.SetCurSel(0);

	m_comboboxCompareType.InsertString(0, _T("��ֵ����"));
	m_comboboxCompareType.InsertString(1, _T("�ַ�������"));
	m_comboboxCompareType.SetCurSel(0);

	m_comboboxComparison.InsertString(0, _T("<"));
	m_comboboxComparison.InsertString(1, _T("<="));
	m_comboboxComparison.InsertString(2, _T("="));
	m_comboboxComparison.InsertString(3, _T(">="));
	m_comboboxComparison.InsertString(4, _T(">"));
	m_comboboxComparison.SetCurSel(2);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSQLiteToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSQLiteToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSQLiteToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSQLiteToolsDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	PROBLEMITEM stProblemItem = { 0 };

	int nType = m_comboboxType.GetCurSel();
	int nOperation = m_comboboxOperation.GetCurSel();
	int nKeyType = m_comboboxKeyType.GetCurSel();
	int nCompareType = m_comboboxCompareType.GetCurSel();
	int nComparison = m_comboboxComparison.GetCurSel() - 2;

	stProblemItem.strDescription = m_editDescript.GetString();
	stProblemItem.bStatus = 0;
	stProblemItem.nType = nType;
	stProblemItem.nOperation = nOperation;
	stProblemItem.strPath = m_editPath.GetString();
	stProblemItem.strKey = m_editKey.GetString();
	stProblemItem.nKeyType = nKeyType;
	stProblemItem.nCompareType = nCompareType;
	stProblemItem.strValue = m_editDefaultValue;
	stProblemItem.nComparison = nComparison;

	m_pDataMgr->Init();
	m_pDataMgr->InsertItem(stProblemItem);

	UpdateData(FALSE);
}
