#ifndef __MAINFRAMEWND_H__
#define __MAINFRAMEWND_H__

#include "UIProblemList.h"
#include "Examination.h"

class CExamination;
class CMainFrameWnd : public WindowImplBase
{
public:
	CMainFrameWnd(CDuiString strXMLPath);
	~CMainFrameWnd();

protected:
	virtual CDuiString	GetSkinFolder();
	virtual CDuiString	GetSkinFile();
	virtual LPCTSTR		GetWindowClassName(void) const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);

	virtual void		Notify(TNotifyUI& msg);
	virtual void		InitWindow();

	virtual LRESULT		MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual LRESULT		OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	BOOL AddGroupToList(LPCTSTR lpGroupName);
	BOOL AddItemToList(LPCTSTR lpGroupName, LPCTSTR lpItemValue);
	void SetProgressValue(int value);
	void SetBtnText(LPCTSTR lpText);
	void SetBtnBkColor(DWORD dwColor);
	void SetTipsText(LPCTSTR lpTipsText);
	void ShowReturnButton(bool bIsShow = TRUE);

	void ShowMainLayout();
	void ShowExaminationLayout();

private:
	// CShadowUI*			m_pShadowUI;
	CDuiString				m_strXMLPath;
	CExamination*			m_Examination;

	CProblemListUI*			m_pProblemListUI;		// �����б�
	CButtonUI*				m_pExaminationBtn;		// ������찴ť
	CButtonUI*				m_pReturnBtn;			// ���ذ�ť
	CTextUI*				m_pTipsText;			// ��ʾ��Ϣ
	CProgressUI*			m_pProgressFront;		// ������

	CVerticalLayoutUI*		m_pVLayoutMain;			// ������
	CVerticalLayoutUI*		m_pVLayoutExamination;	// ������
};

#endif
