#include "StdAfx.h"

namespace DuiLib{

CUIForm::CUIForm(void)
{
	m_pManager = NULL;
	m_pManager = NULL;
	m_pFrame = NULL;
}

CUIForm::~CUIForm(void)
{
	
}

CControlUI *CUIForm::GetRoot() { ASSERT(m_pRoot); return m_pRoot; }

void CUIForm::__InitWindow()
{
	CDuiString sFormName = GetFormName();
	if(!sFormName.IsEmpty())
	{
		m_pRoot = static_cast<CControlUI *>(GetManager()->FindControl(sFormName));
	}
}

} // namespace DuiLib{