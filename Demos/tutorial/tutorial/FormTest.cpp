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
		MessageBox(NULL, _T("btn_test"), _T("btn_test"), MB_OK);
	}
}