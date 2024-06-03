#include "StdAfx.h"
#include <algorithm>

namespace DuiLib{

CUIFrameWndBase::CUIFrameWndBase(void) : m_pApplication(NULL)
{
	m_vctStaticName.push_back(_T("controlui"));
	m_vctStaticName.push_back(_T("textui"));
	m_vctStaticName.push_back(_T("labelui"));
	m_vctStaticName.push_back(_T("containerui"));
	m_vctStaticName.push_back(_T("horizontallayoutui"));
	m_vctStaticName.push_back(_T("verticallayoutui"));
	m_vctStaticName.push_back(_T("tablayoutui"));
	m_vctStaticName.push_back(_T("childlayoutui"));
	m_vctStaticName.push_back(_T("dialoglayoutui"));
	m_vctStaticName.push_back(_T("progresscontainerui"));
}

CUIFrameWndBase::~CUIFrameWndBase(void)
{
	
}

CControlUI *CUIFrameWndBase::GetRoot()
{
	return GetManager()->GetRoot();
}

void CUIFrameWndBase::AttachVirtualForm(CUIFrmBase *pForm)
{
	CUIForm *p = dynamic_cast<CUIForm *>(pForm);
	if(!p) return;

	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm1 = (CUIForm *)m_listForm.GetAt(i);
		if(pForm1 == pForm)
			return;
	}

	p->SetFrameWnd(this);
	m_listForm.Add(pForm);
}

void CUIFrameWndBase::DetachVirtualForm(CUIFrmBase *pForm)
{
	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm1 = (CUIForm *)m_listForm.GetAt(i);
		if(pForm1 == pForm)
		{
			m_listForm.Remove(i);
			return;
		}
	}
}

void CUIFrameWndBase::__InitWindow()
{
	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
		pForm->__InitWindow();
		pForm->InitWindow();
	}
	InitWindow();
}

void CUIFrameWndBase::UIAction(TUIAction *act, bool bAsync)
{
	CControlUI *pControl = GetManager()->FindControl(act->sControlName);
	if(!pControl) return;

	if (act->action == UIACTION_SetText)
	{
		pControl->SetText((LPCTSTR)act->wParam);
		return;
	}

	if (act->action == UIACTION_SetValue)
	{
		CProgressUI *pProgress = dynamic_cast<CProgressUI *>(pControl);
		if(pProgress)
		{
			pProgress->SetValue(act->wParam);
		}
		return;
	}

	if (act->action == UIACTION_SetMinValue)
	{
		CProgressUI *pProgress = dynamic_cast<CProgressUI *>(pControl);
		if(pProgress)
		{
			pProgress->SetMinValue(act->wParam);
		}
		return;
	}

	if (act->action == UIACTION_SetMaxValue)
	{
		CProgressUI *pProgress = dynamic_cast<CProgressUI *>(pControl);
		if(pProgress)
		{
			pProgress->SetMaxValue(act->wParam);
		}
		return;
	}

	if (act->action == UIACTION_SetGridCellText)
	{
		CGridUI *pGrid = dynamic_cast<CGridUI *>(pControl);
		if(pGrid)
		{
			TCellID *p = (TCellID *)act->lParam;
			TCellData *pCell = pGrid->GetCellData(p->row, p->col);
			if(pCell)
			{
				pCell->SetText((LPCTSTR)act->wParam);
			}
		}
		return;
	}

	if (act->action == UIACTION_SetGridCellTextColor)
	{
		CGridUI *pGrid = dynamic_cast<CGridUI *>(pControl);
		if(pGrid)
		{
			TCellID *p = (TCellID *)act->lParam;
			TCellData *pCell = pGrid->GetCellData(p->row, p->col);
			if(pCell)
			{
				pCell->SetTextColor(act->wParam);
			}
		}
		return;
	}

	if (act->action == UIACTION_GridRefresh)
	{
		CGridUI *pGrid = dynamic_cast<CGridUI *>(pControl);
		if(pGrid)
		{
			BOOL bNeedUpdate = (BOOL)act->wParam;
			pGrid->Refresh(bNeedUpdate);
		}
		return;
	}
}

BOOL CUIFrameWndBase::IsInStaticControl(CControlUI *pControl)
{
	BOOL bRet = FALSE;
	if (!pControl)
	{
		return bRet;
	}

	CDuiString strClassName = pControl->GetClass();
	strClassName.MakeLower();

	std::vector<CDuiString>::iterator it = std::find(m_vctStaticName.begin(), m_vctStaticName.end(), strClassName);
	if (m_vctStaticName.end() != it)
	{
		CControlUI* pParent = pControl->GetParent();
		while (pParent)
		{
			strClassName = pParent->GetClass();
			strClassName.MakeLower();
			it = std::find(m_vctStaticName.begin(), m_vctStaticName.end(), strClassName);
			if (m_vctStaticName.end() == it)
			{
				return bRet;
			}

			pParent = pParent->GetParent();
		}

		bRet = TRUE;
	}

	return bRet;
}

LRESULT CUIFrameWndBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return S_FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return S_FALSE;
	}

	return S_FALSE;
}

} //namespace DuiLib{