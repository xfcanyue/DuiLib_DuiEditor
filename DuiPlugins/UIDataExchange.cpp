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
	for (int i=0; i<m_arrData.GetSize(); i++)
	{
		_ddx_data *pData = (_ddx_data *)m_arrData.GetAt(i);
		switch (pData->controlType)
		{
		case _control_text:
			_UpdateText(pData, bSaveAndValidate);
			break;
		case _control_checkbox:
			_UpdateCheckBox(pData, bSaveAndValidate);
			break;
		case _control_combo:
			_UpdateCombo(pData, bSaveAndValidate);
			break;
		}
	}
	return true;
}

bool CUIDataExchange::_UpdateText(_ddx_data *pData, bool bSaveAndValidate)
{
	if(bSaveAndValidate)
	{
		switch (pData->valueType)
		{
		case _value_null: break;
		case _value_int:
			*((int *)(pData->pValue)) = _ttoi(pData->pControl->GetText());
			break;
		case _value_duistring:
			*((CDuiString *)(pData->pValue)) = pData->pControl->GetText();
			break;
#ifdef _AFX
		case _value_cstring:
			*((CString *)(pData->pValue)) = pData->pControl->GetText();
			break;
		case _value_coledatetime:
			break;
		case _value_colecurrency:
			break;
#endif
		default: break;
		}	

	}
	else
	{
		switch (pData->valueType)
		{
		case _value_null: break;
		case _value_int:
			{
				CDuiString text;
				text.Format(_T("%d"), *((int *)(pData->pValue)));
				pData->pControl->SetText(text);
			}
			break;
		case _value_duistring:
			pData->pControl->SetText(*((CDuiString *)(pData->pValue)));
			break;
#ifdef _AFX
		case _value_cstring:
			pData->pControl->SetText(*((CString *)(pData->pValue)));
			break;
		case _value_coledatetime:
			break;
		case _value_colecurrency:
			break;
#endif
		default: break;
		}	
	}
	return true;
}

bool CUIDataExchange::_UpdateCheckBox(_ddx_data *pData, bool bSaveAndValidate)
{
	COptionUI *pOption = static_cast<COptionUI *>(pData->pControl->GetInterface(DUI_CTR_OPTION));
	if(!pOption)	return false;

	if(bSaveAndValidate)
	{
		switch (pData->valueType)
		{
		case _value_bool: 
			*((bool *)(pData->pValue)) = pOption->IsSelected();
			break;
		case _value_BOOL:
			*((BOOL *)(pData->pValue)) = pOption->IsSelected();
			break;
		}
	}
	else
	{
		switch (pData->valueType)
		{
		case _value_bool: 
			pOption->Selected(*((bool *)(pData->pValue)));
			break;
		case _value_BOOL:
			pOption->Selected(*((BOOL *)(pData->pValue)) == TRUE);
			break;
		}
	}
	return true;
}

bool CUIDataExchange::_UpdateCombo(_ddx_data *pData, bool bSaveAndValidate)
{
	CComboUI *pCombo = static_cast<CComboUI *>(pData->pControl->GetInterface(DUI_CTR_COMBO));
	if(!pCombo)	return false;

	if(bSaveAndValidate)
	{
		if(pData->valueType == _value_int)
		{
			*((int *)(pData->pValue)) = pCombo->GetCurSel();
		}
	}
	else
	{
		if(pData->valueType == _value_int)
		{
			pCombo->SelectItem(*((int *)(pData->pValue)));
		}
	}
	return true;
}

bool CUIDataExchange::ddxText(CControlUI *pControl, PVOID pValue, _ddx_value_type type)
{
	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->controlType = _control_text;
	dd->valueType = type;
	dd->pValue = pValue;
	m_arrData.Add(dd);
	return true;
}

bool CUIDataExchange::ddxText(CControlUI *pControl, CDuiString &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_duistring);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, CDuiString &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

bool CUIDataExchange::ddxText(CControlUI *pControl, int &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_int);
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
	return ddxText(pControl, (PVOID)&va, _value_cstring);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, CString &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

bool CUIDataExchange::ddxText(CControlUI *pControl, COleDateTime &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_coledatetime);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, COleDateTime &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

bool CUIDataExchange::ddxText(CControlUI *pControl, COleCurrency &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_colecurrency);
}

bool CUIDataExchange::ddxText(LPCTSTR pControlName, COleCurrency &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxText(pControl, va);
}

#endif


bool CUIDataExchange::ddxCheckBox(CControlUI *pControl, bool &va)
{
	ASSERT(pControl);
	ASSERT(pControl->GetInterface(DUI_CTR_OPTION));

	if(pControl->GetInterface(DUI_CTR_OPTION) == NULL)
		return false;

	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->controlType = _control_checkbox;
	dd->valueType = _value_bool;
	dd->pValue = &va;
	m_arrData.Add(dd);
	return true;
}

bool CUIDataExchange::ddxCheckBox(LPCTSTR pControlName, bool &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxCheckBox(pControl, va);
}

bool CUIDataExchange::ddxCheckBox(CControlUI *pControl, BOOL &va)
{
	ASSERT(pControl);
	ASSERT(pControl->GetInterface(DUI_CTR_OPTION));

	if(pControl->GetInterface(DUI_CTR_OPTION) == NULL)
		return false;

	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->controlType = _control_checkbox;
	dd->valueType = _value_BOOL;
	dd->pValue = &va;
	m_arrData.Add(dd);
	return true;
}

bool CUIDataExchange::ddxCheckBox(LPCTSTR pControlName, BOOL &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxCheckBox(pControl, va);
}


bool CUIDataExchange::ddxCombo(CControlUI *pControl, int &va)
{
	ASSERT(pControl);
	ASSERT(pControl->GetInterface(DUI_CTR_COMBO));

	if(pControl->GetInterface(DUI_CTR_COMBO) == NULL)
		return false;

	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->controlType = _control_combo;
	dd->valueType = _value_int;
	dd->pValue = &va;
	m_arrData.Add(dd);
	return true;
}

bool CUIDataExchange::ddxCombo(LPCTSTR pControlName, int &va)
{
	CControlUI *pControl = m_pManager->FindControl(pControlName);
	return ddxCombo(pControl, va);
}


//////////////////////////////////////////////////////////////////////////
COleDateTime CUIDataExchange::MergeCOleDateTime(const SYSTEMTIME &date, const SYSTEMTIME &time)
{
	COleDateTime dt;
	dt.SetDateTime(date.wYear, date.wMonth, date.wDay, time.wHour, time.wMinute, time.wSecond);
	return dt;
}

COleDateTime CUIDataExchange::MergeCOleDateTime(const COleDateTime &date, const COleDateTime &time)
{
	COleDateTime dt;
	dt.SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return dt;
}