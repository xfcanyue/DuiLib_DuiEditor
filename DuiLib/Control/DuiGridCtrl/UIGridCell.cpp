#include "StdAfx.h"
#include "UIGridCell.h"
#include "UIGridRow.h"

namespace DuiLib {
//add by liqs99 ±í¸ñÀà
/////////////////////////////////////////////////////////////////////////////////////

CGridCellUI::CGridCellUI(void): m_uTextStyle(DT_VCENTER | DT_CENTER | DT_SINGLELINE), m_dwTextColor(0), m_iFont(-1), m_bShowHtml(false),m_pOwner(NULL)
{
}

CGridCellUI::~CGridCellUI(void)
{
}

void CGridCellUI::SetOwner(CControlUI *pOwnder)
{
	m_pOwner = pOwnder;
}

CControlUI *CGridCellUI::GetOwner()
{
	return m_pOwner;
}


void CGridCellUI::DrawCellText(HDC hDC, const RECT &rcItem)
{
	CGridRowUI *pRow = (CGridRowUI *)GetOwner();
	if(!pRow) return;

	if( m_dwTextColor == 0 ) m_dwTextColor = pRow->GetManager()->GetDefaultFontColor();

	RECT rcText = rcItem;
	rcText.left += m_rcTextPadding.left;
	rcText.top += m_rcTextPadding.top;
	rcText.right -= m_rcTextPadding.right;
	rcText.bottom -= m_rcTextPadding.bottom;

	CDuiString sText = GetText();
	if( sText.IsEmpty() ) return;
	int nLinks = 0;
	if( m_bShowHtml )
		CRenderEngine::DrawHtmlText(hDC, pRow->GetManager(), rcText, sText, m_dwTextColor, \
		NULL, NULL, nLinks, m_iFont, m_uTextStyle);
	else
		CRenderEngine::DrawText(hDC, pRow->GetManager(), rcText, sText, m_dwTextColor, \
		m_iFont, m_uTextStyle);
}

void CGridCellUI::SetText(LPCTSTR pstrText)
{
	m_sText = pstrText;
}

CDuiString CGridCellUI::GetText()
{
	return m_sText;
}


DWORD CGridCellUI::GetTextStyle() const
{
	return m_uTextStyle;
}

void CGridCellUI::SetTextStyle(UINT uStyle)
{
	m_uTextStyle = uStyle;
}

DWORD CGridCellUI::GetTextColor() const
{
	return m_dwTextColor;
}

void CGridCellUI::SetTextColor(DWORD dwTextColor)
{
	m_dwTextColor = dwTextColor;
}

RECT CGridCellUI::GetTextPadding() const
{
	return m_rcTextPadding;
}

void CGridCellUI::SetTextPadding(RECT rc)
{
	m_rcTextPadding = rc;
}

void CGridCellUI::SetFont(int index)
{
	m_iFont = index;
}

bool CGridCellUI::IsShowHtml()
{
	return m_bShowHtml;
}

void CGridCellUI::SetShowHtml(bool bShowHtml)
{
	if( m_bShowHtml == bShowHtml ) return;

	m_bShowHtml = bShowHtml;
}


}
