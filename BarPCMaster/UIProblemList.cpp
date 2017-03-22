#include "StdAfx.h"
#include "UIProblemList.h"
#include "ControlNames.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/

CProblemListItemUI::CProblemListItemUI()
{

}

CProblemListItemUI::~CProblemListItemUI()
{

}

LPCTSTR CProblemListItemUI::GetClass() const
{
	return _T("TreeNodeUI");
}

LPVOID CProblemListItemUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, bpcProblemListItemUIInferFace) == 0)
	{
		return static_cast<CProblemListItemUI*>(this);
	}

	return CTreeNodeUI::GetInterface(pstrName);
}

/************************************************************************/
/*       CProblemListGroupUI                                            */
/************************************************************************/
CProblemListGroupUI::CProblemListGroupUI()
{

}

LPCTSTR CProblemListGroupUI::GetClass() const
{
	return _T("TreeNodeUI");
}

LPVOID CProblemListGroupUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, bpcProblemListGroupUIInferFace) == 0)
	{
		return static_cast<CProblemListGroupUI*>(this);
	}

	return CTreeNodeUI::GetInterface(pstrName);
}

void CProblemListGroupUI::SetItemText(LPCTSTR pstrValue)
{
	m_GroupName = pstrValue;
	CTreeNodeUI::SetItemText(pstrValue);
}

bool CProblemListGroupUI::Add(CProblemListItemUI* pControl, bool bAutoRefreshIndex /*= true*/)
{
	bool nRet = CTreeNodeUI::Add(pControl);

	if (bAutoRefreshIndex)
	{
		CDuiString text;

		/*int nIndex = GetTreeNodes().GetSize() + 1;
		text.Format(_T("%s[%d]"), m_GroupName.GetData(), GetTreeNodes().GetSize());
		CTreeNodeUI::SetItemText(text.GetData());*/
	}

	return nRet;
}

bool CProblemListGroupUI::AddAt(CProblemListItemUI* pControl, int iIndex, bool bAutoRefreshIndex /*= true*/)
{
	bool nRet = CTreeNodeUI::AddAt(pControl, iIndex);
	CTreeNodeUI::SetItemText(m_GroupName.GetData());

	return nRet;
}

void CProblemListGroupUI::SetProblemList(CProblemListUI* pProblemList)
{
	if (nullptr != pProblemList)
	{
		m_pProblemList = pProblemList;
	}
}

/************************************************************************/
/*       CProblemListUI                                                 */
/************************************************************************/
CProblemListUI::CProblemListUI(CPaintManagerUI& PaintManager)
	:m_PaintManager(PaintManager)
{
	SetItemShowHtml(TRUE);
}


CProblemListUI::~CProblemListUI()
{
}

CControlUI* CProblemListUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, bpcProblemListGroupUIInferFace) == 0)
	{
		return new CProblemListGroupUI();
	}
	else if (_tcsicmp(pstrClass, bpcProblemListItemUIInferFace) == 0)
	{
		return new CProblemListItemUI();
	}

	return nullptr;
}

bool CProblemListUI::AddGroup(LPCTSTR szGroupName, int nIndex /*= -1*/)
{
	CProblemListGroupUI* pListGroup = nullptr;
	if (!m_dlgGroupBuilder.GetMarkup()->IsValid())
	{
		pListGroup = static_cast<CProblemListGroupUI*>(m_dlgGroupBuilder.Create(bpcProblemListGroupXML, (UINT)0, this, &m_PaintManager));
	}
	else
	{
		pListGroup = static_cast<CProblemListGroupUI*>(m_dlgGroupBuilder.Create(this, &m_PaintManager));
	}
	if (pListGroup == nullptr)
	{
		return FALSE;
	}

	pListGroup->SetName(szGroupName);
	pListGroup->SetItemText(szGroupName);
	pListGroup->SetProblemList(this);

	if (nIndex == -1)
	{
		if (!Add(pListGroup))
		{
			delete pListGroup;
			return FALSE;
		}
	}
	else
	{
		if (!AddAt(pListGroup, nIndex))
		{
			delete pListGroup;
			return FALSE;
		}
	}

	return TRUE;
}

bool CProblemListUI::AddItem(LPCTSTR szGroupName, PPROBLEMITEM pstExaminaion, bool bAutoRefreshIndex /*= true*/, int nIndex /*= -1*/)
{
	if (nullptr == GetGroup(szGroupName))
	{
		AddGroup(szGroupName);
	}

	// ��ʼ�� item ������
	CProblemListItemUI* pListElement = nullptr;
	if (!m_dlgItemBuilder.GetMarkup()->IsValid())
	{
		pListElement = static_cast<CProblemListItemUI*>(m_dlgItemBuilder.Create(bpcProblemListItemXML, (UINT)0, this, &m_PaintManager));
	}
	else
	{
		pListElement = static_cast<CProblemListItemUI*>(m_dlgItemBuilder.Create(this, &m_PaintManager));
	}

	if (pListElement == nullptr)
	{
		return nullptr;
	}

	// ���ݿؼ�����Ϣ������ Tag
	pstExaminaion->pControl = reinterpret_cast<void*>(pListElement);
	pListElement->SetTag(reinterpret_cast<UINT_PTR>(pstExaminaion));

	// �� Text �ؼ���ֵ
	CLabelUI* pProblemText = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, bpcProblemItemLabel));
	if (nullptr != pProblemText)
	{
		pProblemText->SetText(pstExaminaion->strDescription.GetData());
	}

	// ���ݴ��ݽ�����������������ؼ�����
	CProblemListGroupUI* pDefaultGroup = static_cast<CProblemListGroupUI*>(m_pManager->FindSubControlByName(this, szGroupName));
	if (nullptr == pDefaultGroup)
	{
		return FALSE;
	}

	// �� Item ���뵽ָ��������
	if (nIndex == -1)
	{
		if (!pDefaultGroup->Add(pListElement))
		{
			delete pListElement;
			return FALSE;
		}
	}
	else
	{
		if (!pDefaultGroup->AddAt(pListElement, nIndex))
		{
			delete pListElement;
			return FALSE;
		}
	}

	return TRUE;
}

CProblemListGroupUI* CProblemListUI::GetGroup(LPCTSTR szGroupName)
{
	// ͨ�������ؼ������Ƿ���ĳ������
	CProblemListGroupUI* pGroup = static_cast<CProblemListGroupUI*>(m_PaintManager.FindControl(szGroupName));
	if (nullptr == pGroup)
	{
		return nullptr;
	}

	return pGroup;
}
