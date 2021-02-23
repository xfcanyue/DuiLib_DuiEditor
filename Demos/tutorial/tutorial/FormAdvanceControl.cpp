#include "StdAfx.h"
#include "FormAdvanceControl.h"


CFormAdvanceControl::CFormAdvanceControl(void)
{
}


CFormAdvanceControl::~CFormAdvanceControl(void)
{
}

void CFormAdvanceControl::InitWindow()
{
	UI_BINDCONTROL(CGridCtrlUI, m_pGrid, _T("gridctrl_1"));	
}

void CFormAdvanceControl::OnNotifyWindowPrepare(TNotifyUI& msg)
{
	for (int i=1; i<=5; i++)
	{
		int row = m_pGrid->InsertRow();
		for (int j=1; j<=5; j++)
		{
			CString temp;
			temp.Format(_T("%02d,%02d"), i, j);
			m_pGrid->Cell(i,j).SetText(temp);
		}
	}
}