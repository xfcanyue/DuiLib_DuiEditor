// MyStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "MyStatusBar.h"


const int nTextMargin = 4; // Gap between image and text
// CMyStatusBar

IMPLEMENT_DYNAMIC(CMyStatusBar, CMFCStatusBar)

CMyStatusBar::CMyStatusBar()
{

}

CMyStatusBar::~CMyStatusBar()
{
}


BEGIN_MESSAGE_MAP(CMyStatusBar, CMFCStatusBar)
END_MESSAGE_MAP()


int CMyStatusBar::GetIndexByID(UINT nID)
{
	for (int i = 0; i < m_nCount; i++)
	{
		CMFCStatusBarPaneInfo *pPaneInfo = ((CMFCStatusBarPaneInfo*)m_pData) + i;
		if(pPaneInfo->nID == nID)
			return i;
	}

	return -1;
}

BOOL CMyStatusBar::SetPaneTextByID(UINT nID, LPCTSTR lpszNewText, BOOL bCalcPaneWidth)
{
	int nIndex = GetIndexByID(nID);
	if(nIndex < 0)	return FALSE;

	SetPaneText(nIndex, lpszNewText);

	if(bCalcPaneWidth)
	{
		CDC *pDC = GetDC();

		CRect rc;
		pDC->DrawText(lpszNewText, rc, DT_CALCRECT);

		int nNewWith = rc.Width() + 10;
		if(nNewWith < 80) nNewWith = 80;
		SetPaneWidth(nIndex, nNewWith);

		ReleaseDC(pDC);
	}	
	return TRUE;
}

// CMyStatusBar 消息处理程序
void CMyStatusBar::OnDrawPane(CDC* pDC, CMFCStatusBarPaneInfo* pPane)
{
	ASSERT_VALID(pDC);
	ENSURE(pPane != NULL);

	CRect rectPane = pPane->rect;
	if (rectPane.IsRectEmpty() || !pDC->RectVisible(rectPane))
	{
		return;
	}

	// Fill pane background:
	if (pPane->clrBackground != (COLORREF)-1)
	{
		CBrush brush(pPane->clrBackground);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		pDC->PatBlt(rectPane.left, rectPane.top, rectPane.Width(), rectPane.Height(), PATCOPY);

		pDC->SelectObject(pOldBrush);
	}

	// Draw pane border:
	CMFCVisualManager::GetInstance()->OnDrawStatusBarPaneBorder(pDC, this, rectPane, pPane->nID, pPane->nStyle);

	if (!(pPane->nStyle & SBPS_NOBORDERS)) // only adjust if there are borders
	{
		rectPane.DeflateRect(2 * AFX_CX_BORDER, AFX_CY_BORDER);
	}

	// Draw icon
	if (pPane->hImage != NULL && pPane->cxIcon > 0)
	{
		CRect rectIcon = rectPane;
		rectIcon.right = rectIcon.left + pPane->cxIcon;

		int x = max(0, (rectIcon.Width() - pPane->cxIcon) / 2);
		int y = max(0, (rectIcon.Height() - pPane->cyIcon) / 2);

		::ImageList_DrawEx(pPane->hImage, pPane->nCurrFrame, pDC->GetSafeHdc(), rectIcon.left + x, rectIcon.top + y, pPane->cxIcon, pPane->cyIcon, CLR_NONE, 0, ILD_NORMAL);
	}

	CRect rectText = rectPane;
	rectText.left += pPane->cxIcon;

	if (pPane->cxIcon > 0)
	{
		rectText.left += nTextMargin;
	}

	if (pPane->nProgressTotal > 0)
	{
		// Draw progress bar:
		CRect rectProgress = rectText;
		rectProgress.DeflateRect(1, 1);

		COLORREF clrBar = (pPane->clrProgressBar == (COLORREF)-1) ? afxGlobalData.clrHilite : pPane->clrProgressBar;

		CMFCVisualManager::GetInstance()->OnDrawStatusBarProgress(pDC, this, rectProgress,
			pPane->nProgressTotal, pPane->nProgressCurr, clrBar, pPane->clrProgressBarDest, pPane->clrProgressText, pPane->bProgressText);
	}
	else
	{
		// Draw text
		if (pPane->lpszText != NULL && pPane->cxText > 0)
		{
			COLORREF clrText = pDC->SetTextColor(CMFCVisualManager::GetInstance()->GetStatusBarPaneTextColor(this, pPane));

			if(pPane->nID > 0)
				pDC->DrawText(pPane->lpszText, lstrlen(pPane->lpszText), rectText, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			else
				pDC->DrawText(pPane->lpszText, lstrlen(pPane->lpszText), rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			pDC->SetTextColor(clrText);
		}
	}
}


