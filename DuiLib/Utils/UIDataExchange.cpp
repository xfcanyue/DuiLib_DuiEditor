#include "UIDataExchange.hpp"


#ifndef SOURCE_UIDataExchange_CPP
#define SOURCE_UIDataExchange_CPP
#define implfun inline

namespace DuiLib
{

implfun CUIDataExchange::CUIDataExchange(void)
{
	m_pManager = NULL;
	m_pRoot = NULL;
}

implfun CUIDataExchange::~CUIDataExchange(void)
{
	for (int i=0; i<_ddxDataArray.GetSize(); i++)
	{
		_ddx_data *pData = (_ddx_data *)_ddxDataArray.GetAt(i);
		delete pData;
	}
}

implfun void CUIDataExchange::ddxSetManager(CPaintManagerUI *pManager, CControlUI *pContainer)
{
	m_pManager = pManager;
	if(pContainer)
	{
		m_pRoot = static_cast<CContainerUI *>(pContainer->GetInterface(DUI_CTR_CONTAINER));
		ASSERT(m_pRoot);
	}
}

implfun bool CUIDataExchange::UpdateDataUI(bool bSaveAndValidate)
{
	for (int i=0; i<_ddxDataArray.GetSize(); i++)
	{
		_ddx_data *pData = (_ddx_data *)_ddxDataArray.GetAt(i);
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
		case _control_combo_ex:
			_UpdateComboEx(pData, bSaveAndValidate);
			break;
		case _control_tablayout:
			_UpdateTabLayout(pData, bSaveAndValidate);
			break;
		}
	}
	return true;
}

implfun bool CUIDataExchange::_UpdateText(_ddx_data *pData, bool bSaveAndValidate)
{
	bool m_bUpdate = true;

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
			{
				CDateTimeExUI *pdatetimeex = (CDateTimeExUI *)pData->pControl->GetInterface(DUI_CTR_DATETIMEEX);
				CDateTimeUI *pdatetime = (CDateTimeUI *)pData->pControl->GetInterface(DUI_CTR_DATETIME);
				if(pdatetimeex)
				{
					*((COleDateTime *)(pData->pValue)) = COleDateTime(pdatetimeex->GetTime());
				}
				else if(pdatetime)
				{
					*((COleDateTime *)(pData->pValue)) = COleDateTime(pdatetime->GetTime());
				}
				else
				{
					m_bUpdate = ((COleDateTime *)(pData->pValue))->ParseDateTime(pData->pControl->GetText()) == TRUE;
				}
			}
			break;
		case _value_colecurrency:
			m_bUpdate = ((COleCurrency *)(pData->pValue))->ParseCurrency(pData->pControl->GetText()) == TRUE;
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
			{
				//针对ComboUI和ComboExUI，UpdateData(false)需要特殊处理
				CComboUI *pCombo = static_cast<CComboUI *>(pData->pControl->GetInterface(DUI_CTR_COMBO));
				CComboExUI *pComboEx = static_cast<CComboExUI *>(pData->pControl->GetInterface(DUI_CTR_COMBOEX));
				if(pComboEx)
				{
					if(pComboEx->GetDropType() == CBS_DROPDOWNLIST)
						pCombo->SelectItem(*((CDuiString *)(pData->pValue)));
					else
						pData->pControl->SetText(*((CDuiString *)(pData->pValue)));
				}
				else if(pCombo)
				{
					pCombo->SelectItem(*((CDuiString *)(pData->pValue)));
				}
				else
					pData->pControl->SetText(*((CDuiString *)(pData->pValue)));
			}
			break;
#ifdef _AFX
		case _value_cstring:
			{
				//针对ComboUI和ComboExUI，UpdateData(false)需要特殊处理
				CComboUI *pCombo = static_cast<CComboUI *>(pData->pControl->GetInterface(DUI_CTR_COMBO));
				CComboExUI *pComboEx = static_cast<CComboExUI *>(pData->pControl->GetInterface(DUI_CTR_COMBOEX));
				if(pComboEx)
				{
					if(pComboEx->GetDropType() == CBS_DROPDOWNLIST)
						pCombo->SelectItem(*((CString *)(pData->pValue)));
					else
						pData->pControl->SetText(*((CString *)(pData->pValue)));
				}
				else if(pCombo)
				{
					pCombo->SelectItem(*((CString *)(pData->pValue)));
				}
				else
					pData->pControl->SetText(*((CString *)(pData->pValue)));
			}
			break;
		case _value_coledatetime:
			{
				CDateTimeExUI *pdatetimeex = (CDateTimeExUI *)pData->pControl->GetInterface(DUI_CTR_DATETIMEEX);
				CDateTimeUI *pdatetime = (CDateTimeUI *)pData->pControl->GetInterface(DUI_CTR_DATETIME);
				if(pdatetimeex)
				{
					SYSTEMTIME st;
					m_bUpdate = ((COleDateTime *)(pData->pValue))->GetAsSystemTime(st);
					pdatetimeex->SetTime(&st);
				}
				else if(pdatetime)
				{
					SYSTEMTIME st;
					m_bUpdate = ((COleDateTime *)(pData->pValue))->GetAsSystemTime(st);
					pdatetime->SetTime(&st);
				}
				else
				{
					pData->pControl->SetText(((COleDateTime *)(pData->pValue))->Format(_T("%Y-%m-%d %H:%M:%S")));
				}
			}
			break;
		case _value_colecurrency:
			{
				pData->pControl->SetText(((COleCurrency *)(pData->pValue))->Format());
			}
			break;
#endif
		default: break;
		}	
	}
	return m_bUpdate;
}

implfun bool CUIDataExchange::_UpdateCheckBox(_ddx_data *pData, bool bSaveAndValidate)
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

implfun bool CUIDataExchange::_UpdateCombo(_ddx_data *pData, bool bSaveAndValidate)
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

implfun bool CUIDataExchange::_UpdateComboEx(_ddx_data *pData, bool bSaveAndValidate)
{
	CComboExUI *pCombo = static_cast<CComboExUI *>(pData->pControl->GetInterface(DUI_CTR_COMBOEX));
	if(!pCombo)	return false;

	if(bSaveAndValidate)
	{
		if(pData->valueType == _value_int)
		{
			*((int *)(pData->pValue)) = (int)pCombo->GetCurSelItemData();
		}
	}
	else
	{
		if(pData->valueType == _value_int)
		{
			pCombo->SetCurSelFromItemData(*((int *)(pData->pValue)));
		}
	}
	return true;
}

implfun bool CUIDataExchange::_UpdateTabLayout(_ddx_data *pData, bool bSaveAndValidate)
{
	CTabLayoutUI *pControl = static_cast<CTabLayoutUI *>(pData->pControl->GetInterface(DUI_CTR_TABLAYOUT));
	if(!pControl)	return false;

	if(bSaveAndValidate)
	{
		if(pData->valueType == _value_int)
		{
			*((int *)(pData->pValue)) = pControl->GetCurSel();
		}
	}
	else
	{
		if(pData->valueType == _value_int)
		{
			pControl->SelectItem(*((int *)(pData->pValue)));
		}
	}
	return true;
}

implfun bool CUIDataExchange::ddxText(CControlUI *pControl, PVOID pValue, _ddx_value_type type)
{
	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->controlType = _control_text;
	dd->valueType = type;
	dd->pValue = pValue;
	_ddxDataArray.Add(dd);
	return true;
}

implfun bool CUIDataExchange::ddxText(CControlUI *pControl, CDuiString &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_duistring);
}

implfun bool CUIDataExchange::ddxText(LPCTSTR pControlName, CDuiString &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxText(pControl, va);
}

implfun bool CUIDataExchange::ddxText(CControlUI *pControl, int &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_int);
}

implfun bool CUIDataExchange::ddxText(LPCTSTR pControlName, int &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxText(pControl, va);
}

#ifdef _AFX
implfun bool CUIDataExchange::ddxText(CControlUI *pControl, CString &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_cstring);
}

implfun bool CUIDataExchange::ddxText(LPCTSTR pControlName, CString &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxText(pControl, va);
}

implfun bool CUIDataExchange::ddxText(CControlUI *pControl, COleDateTime &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_coledatetime);
}

implfun bool CUIDataExchange::ddxText(LPCTSTR pControlName, COleDateTime &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxText(pControl, va);
}

implfun bool CUIDataExchange::ddxText(CControlUI *pControl, COleCurrency &va)
{
	ASSERT(pControl);
	return ddxText(pControl, (PVOID)&va, _value_colecurrency);
}

implfun bool CUIDataExchange::ddxText(LPCTSTR pControlName, COleCurrency &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxText(pControl, va);
}

#endif


implfun bool CUIDataExchange::ddxCheckBox(CControlUI *pControl, bool &va)
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
	_ddxDataArray.Add(dd);
	return true;
}

implfun bool CUIDataExchange::ddxCheckBox(LPCTSTR pControlName, bool &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxCheckBox(pControl, va);
}

implfun bool CUIDataExchange::ddxCheckBox(CControlUI *pControl, BOOL &va)
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
	_ddxDataArray.Add(dd);
	return true;
}

implfun bool CUIDataExchange::ddxCheckBox(LPCTSTR pControlName, BOOL &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxCheckBox(pControl, va);
}


implfun bool CUIDataExchange::ddxCombo(CControlUI *pControl, int &va)
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
	_ddxDataArray.Add(dd);
	return true;
}

implfun bool CUIDataExchange::ddxCombo(LPCTSTR pControlName, int &va)
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxCombo(pControl, va);
}

implfun bool CUIDataExchange::ddxComboItemData(CControlUI *pControl, int &va) //绑定ComboItemData
{
	ASSERT(pControl);
	ASSERT(pControl->GetInterface(DUI_CTR_COMBOEX));

	if(pControl->GetInterface(DUI_CTR_COMBOEX) == NULL)
		return false;

	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->controlType = _control_combo_ex;
	dd->valueType = _value_int;
	dd->pValue = &va;
	_ddxDataArray.Add(dd);
	return true;
}

implfun bool CUIDataExchange::ddxComboItemData(LPCTSTR pControlName, int &va) //绑定ComboItemData
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxComboItemData(pControl, va);
}

implfun bool CUIDataExchange::ddxTabLayout(CControlUI *pControl, int &va) //cursel
{
	ASSERT(pControl);
	ASSERT(pControl->GetInterface(DUI_CTR_TABLAYOUT));

	if(pControl->GetInterface(DUI_CTR_TABLAYOUT) == NULL)
		return false;

	_ddx_data *dd = new _ddx_data;
	dd->pControl = pControl;
	dd->controlType = _control_tablayout;
	dd->valueType = _value_int;
	dd->pValue = &va;
	_ddxDataArray.Add(dd);
	return true;
}

implfun bool CUIDataExchange::ddxTabLayout(LPCTSTR pControlName, int &va) //cursel
{
	CControlUI *pControl = NULL;
	if(m_pRoot)
	{
		pControl = m_pRoot->FindSubControl(pControlName);
	}
	if(!pControl)
	{
		ASSERT(m_pManager);
		pControl = m_pManager->FindControl(pControlName);
	}
	return ddxTabLayout(pControl, va);
}

//////////////////////////////////////////////////////////////////////////
#ifdef _AFX
implfun COleDateTime CUIDataExchange::MergeCOleDateTime(const SYSTEMTIME &date, const SYSTEMTIME &time)
{
	COleDateTime dt;
	dt.SetDateTime(date.wYear, date.wMonth, date.wDay, time.wHour, time.wMinute, time.wSecond);
	return dt;
}

implfun COleDateTime CUIDataExchange::MergeCOleDateTime(const COleDateTime &date, const COleDateTime &time)
{
	COleDateTime dt;
	dt.SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	return dt;
}
#endif

} //namespace DuiLib

#endif //#define SOURCE_UIDataExchange_CPP