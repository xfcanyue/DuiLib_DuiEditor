#include "StdAfx.h"
#include "UIDataExchange.h"


CUIDataExchange::CUIDataExchange(void)
{
	m_pManager = NULL;
}

CUIDataExchange::~CUIDataExchange(void)
{
	for (int i=0; i<m_arrData.GetSize(); i++)
	{
		_ddx_data *pData = (_ddx_data *)m_arrData.GetAt(i);
		delete pData;
	}
}

void CUIDataExchange::ddxSetManager(CPaintManagerUI *pManager)
{
	m_pManager = pManager;
}

bool CUIDataExchange::UpdateDataUI(bool bSaveAndValidate)
{
	if(bSaveAndValidate)
	{
		for (int i=0; i<m_arrData.GetSize(); i++)
		{
			_ddx_data *pData = (_ddx_data *)m_arrData.GetAt(i);
			switch (pData->type_)
			{
			case _ddx_null: break;
			case _ddx_int:
				*((int *)(pData->pValue)) = _ttoi(pData->pControl->GetText());
				break;
			case _ddx_duistring:
				*((CDuiString *)(pData->pValue)) = pData->pControl->GetText();
				break;
#ifdef _AFX
			case _ddx_cstring:
				*((CString *)(pData->pValue)) = pData->pControl->GetText();
				break;
			case _ddx_coledatetime:
				break;
			case _ddx_colecurrency:
				break;
#endif
			default: break;
			}
		}
	}
	else
	{
		for (int i=0; i<m_arrData.GetSize(); i++)
		{
			_ddx_data *pData = (_ddx_data *)m_arrData.GetAt(i);
			switch (pData->type_)
			{
			case _ddx_null: break;
			case _ddx_int:
				{
					CDuiString text;
					text.Format(_T("%d"), *((int *)(pData->pValue)));
					pData->pControl->SetText(text);
				}
				break;
			case _ddx_duistring:
				pData->pControl->SetText(*((CDuiString *)(pData->pValue)));
				break;
#ifdef _AFX
			case _ddx_cstring:
				pData->pControl->SetText(*((CString *)(pData->pValue)));
				break;
			case _ddx_coledatetime:
				break;
			case _ddx_colecurrency:
				break;
#endif
			default: break;
			}
		}
	}
	return true;
}

bool CUIDataExchange::ddxText(CControlUI *pControl, PVOID pValue, _ddx_type type)
{
	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->type_ = type;
	dd->pValue = pValue;
	m_arrData.Add(dd);
	return true;
}

bool CUIDataExchange::ddxText(CControlUI *pControl, CDuiString &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _ddx_duistring);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, CDuiString &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

bool CUIDataExchange::ddxText(CControlUI *pControl, int &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _ddx_int);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, int &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

#ifdef _AFX
bool CUIDataExchange::ddxText(CControlUI *pControl, CString &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _ddx_cstring);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, CString &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

bool CUIDataExchange::ddxText(CControlUI *pControl, COleDateTime &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _ddx_coledatetime);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, COleDateTime &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

bool CUIDataExchange::ddxText(CControlUI *pControl, COleCurrency &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _ddx_colecurrency);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, COleCurrency &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

#endif