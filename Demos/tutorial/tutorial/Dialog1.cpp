#include "StdAfx.h"
#include "Dialog1.h"


CDialog1::CDialog1(void)
{
	m_sEdit = _T("text");
	m_sCombo = _T("id 1");
	m_nComboSel = 0;
	m_sDateTime = _T("2000-1-2 03:04:05");
	m_sDate = _T("2009-06-07");
	m_sTime = _T("08:09:10");

	m_dtDateTime.SetDateTime(2000,1,2,3,4,5);
	m_dtDate.SetDate(2009,6,7);
	m_dtTime.SetTime(8,9,10);

	m_cyMoney.SetCurrency(10, 3500);
}


CDialog1::~CDialog1(void)
{
}

void CDialog1::InitWindow()
{
	ddxSetManager(GetManager());
	ddxText(_T("edit_1"), m_sEdit);
	ddxCombo(_T("combo_1"), m_nComboSel);
	ddxText(_T("combo_2"), m_sCombo);
	ddxCheckBox(_T("chk_1"), m_bCheck);

// 	ddxText(_T("datetime_1"), m_sDateTime);
// 	ddxText(_T("datetime_2"), m_sDate);
// 	ddxText(_T("datetime_3"), m_sTime);

 	ddxText(_T("datetime_1"), m_dtDateTime);
	ddxText(_T("datetime_2"), m_dtDate);
	ddxText(_T("datetime_3"), m_dtTime);

	ddxText(_T("edit_money"), m_cyMoney);

	UpdateDataUI(false);
	
}

void CDialog1::OnClickOK()
{
	UpdateDataUI(true);

	CString sText, temp;

	sText += m_sEdit;	sText += _T("\r\n");

	temp.Format(_T("sel = %d\r\n"), m_nComboSel);
	sText += temp;

	sText += m_sCombo;	sText += _T("\r\n");

	sText += m_bCheck ? _T("CheckBox seleted") : _T("CheckBox unseleted");	sText += _T("\r\n");


	sText += m_dtDateTime.Format(_T("%Y-%m-%d %H:%M:%S"));	sText += _T("\r\n");
	sText += m_dtDate.Format(_T("%Y-%m-%d"));	sText += _T("\r\n");
	sText += m_dtTime.Format(_T("%H:%M:%S"));	sText += _T("\r\n");

// 	sText += m_sDateTime; 	sText += _T("\r\n");
// 	sText += m_sDate; 	sText += _T("\r\n");
// 	sText += m_sTime; 	sText += _T("\r\n");

	sText += m_cyMoney.Format();	sText += _T("\r\n");

	if(MessageBox(GetMainWnd()->GetHWND(), sText, _T(""), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	__super::OnClickOK();
}

void CDialog1::OnClickCancel()
{
	__super::OnClickCancel();
}