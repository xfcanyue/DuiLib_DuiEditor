#include "StdAfx.h"
#include "FormTest.h"


CFormTest::CFormTest(void)
{
}


CFormTest::~CFormTest(void)
{
}

void CFormTest::InitWindow()
{

}

void CFormTest::OnNotifyClick(TNotifyUI& msg)
{
	if(IsControl(msg, _T("btn_test")))
	{
		MessageBox(NULL, _T(""), _T(""), MB_OK);
	}
}