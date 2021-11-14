#pragma once
#include "StdAfx.h"
#include "UIFrameWnd.h"

namespace DuiLib{

CUIForm::CUIForm(void)
{
	
}

CUIForm::~CUIForm(void)
{
	
}

void CUIForm::__InitWindow()
{
	CDuiString sFormName = GetFormName();
	if(!sFormName.IsEmpty())
	{
		m_pRoot = static_cast<CControlUI *>(GetManager()->FindControl(sFormName));
	}
}

} // namespace DuiLib{