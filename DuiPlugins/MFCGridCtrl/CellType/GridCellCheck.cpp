// GridCellCheck.cpp : implementation file
//
// MFC Grid Control - Main grid cell class
//
// Provides the implementation for a combobox cell type of the
// grid control.
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceList from http://www.codeguru.com/listview
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.22+
//
// History:
// 23 Jul 2001 - Complete rewrite
// 13 Mar 2004 - GetCellExtent and GetCheckPlacement fixed by Yogurt
//             - Read-only now honoured - Yogurt
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../GridCell.h"
#include "../GridCtrl.h"

#include "GridCellCheck.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellCheck, CGridCell)

CGridCellCheck::CGridCellCheck() : CGridCell()
{
    m_bChecked = FALSE;
    //m_Rect.IsRectNull();
}

CSize CGridCellCheck::GetCellExtent(CDC* pDC)
{
    // Using SM_CXHSCROLL as a guide to the size of the checkbox
    int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();	
    CSize	cellSize = CGridCell::GetCellExtent(pDC);	
    cellSize.cx += nWidth;	
    cellSize.cy = max (cellSize.cy, nWidth);	
    return  cellSize;
}

// i/o:  i=dims of cell rect; o=dims of text rect
BOOL CGridCellCheck::GetTextRect( LPRECT pRect)
{
    BOOL bResult = CGridCell::GetTextRect(pRect);
    if (bResult)
    {
        int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();
        pRect->left += nWidth;
        if (pRect->left > pRect->right)
            pRect->left = pRect->right;
    }
    return bResult;
}

void  CGridCellCheck::SetText(LPCTSTR szText)        
{ 
	CString cs = szText;
	if(szText==_T("1"))	
		m_bChecked = TRUE;
	else m_bChecked = FALSE;
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellCheck::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

#ifndef _WIN32_WCE
    // Store the cell's dimensions for later
    m_Rect = rect;

    //CRect CheckRect = GetCheckPlacement();
    //rect.left = CheckRect.right;

    // enough room to draw? 空间不够不画
     //if (CheckRect.Width() < rect.Width() && CheckRect.Height() < rect.Height()) {

    // Do the draw 
    pDC->DrawFrameControl(GetCheckPlacement(), DFC_BUTTON, 
	  (m_bChecked)? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);

	//pDC->DrawEdge(GetCheckPlacement(),BDR_SUNKENINNER | BDR_SUNKENOUTER,BF_RECT);

     //}
#endif
    return bResult;
}

void CGridCellCheck::OnClick(CPoint PointCellRelative)
{
	//此单元格不允许编辑  Added by Liqs99
	if(!m_bEditable)	return;

	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	PointCellRelative += m_Rect.TopLeft();

    // Bail if cell is read-only
    CCellID cell = GetGrid()->GetCellFromPt(PointCellRelative);		
    if (!cell.IsValid() || !GetGrid()->IsCellEditable(cell))		
        return;

	// GetCheckPlacement returns the checkbox dimensions in client coords. Only check/
	// uncheck if the user clicked in the box
	//被我注释掉，点击单元格就可以选择
	//if (GetCheckPlacement().PtInRect(PointCellRelative))
	//{
		m_bChecked = !m_bChecked;
		GetGrid()->InvalidateRect(m_Rect);
//	}
	
		//////////////////////////////////////////////
		//发送信息给主窗口，Added By Liqs99
		CString str;
		str.Format(_T("%d"), GetCheck());
		
		// Send Notification to parent
		GV_DISPINFO dispinfo;
		
		dispinfo.hdr.hwndFrom = GetGrid()->GetSafeHwnd();
		dispinfo.hdr.idFrom   = GetGrid()->GetDlgCtrlID();
		dispinfo.hdr.code     = GVN_ENDLABELEDIT;
		
		dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
		dispinfo.item.row     = cell.row;
		dispinfo.item.col     = cell.col;
		dispinfo.item.strText  = str;
		//dispinfo.item.lParam  = (LPARAM) m_nLastChar;
		
		CWnd* pOwner = GetGrid()->GetOwner();
		if (pOwner)
			pOwner->SendMessage(WM_NOTIFY, GetGrid()->GetDlgCtrlID(), (LPARAM)&dispinfo );
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CGridCellCheck::SetCheck(BOOL bChecked /*=TRUE*/)
{
	BOOL bTemp = m_bChecked;
	m_bChecked = bChecked;
	if (!m_Rect.IsRectEmpty())
		GetGrid()->InvalidateRect(m_Rect);

	return bTemp;
}

BOOL CGridCellCheck::GetCheck()
{
	return m_bChecked;
}

//////////////////////////////////////////////////////////////////////
// Protected implementation
//////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the checkbox in client coords.
CRect CGridCellCheck::GetCheckPlacement()
{
	int nWidth = 13;//GetSystemMetrics(SM_CXHSCROLL);
	CRect place = m_Rect;// + CSize(GetMargin(), GetMargin());
	//CRect place = m_Rect; + CSize(1, 1);

	//for left
    //place.right = place.left + nWidth;
    //place.bottom = place.top + nWidth;

	// for centering
	int nDiff = (place.Width() - nWidth)/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	nDiff = (place.Height() - nWidth)/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    

    if (m_Rect.Height() < nWidth + 2 * static_cast<int> (GetMargin())) 
    {		
        place.top = m_Rect.top + (m_Rect.Height() - nWidth) / 2;	    
        place.bottom = place.top + nWidth;	
    }

	return place;
}

BOOL CGridCellCheck::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{
	return FALSE;
}

BOOL CGridCellCheck::PrintCell(CDC* pDC, int nRow, int nCol, CRect rect)
{
	#if defined(_WIN32_WCE_NO_PRINTING) || defined(GRIDCONTROL_NO_PRINTING)
    return FALSE;
#else
    COLORREF crFG, crBG;
    GV_ITEM Item;

    CGridCtrl* pGrid = GetGrid();
    if (!pGrid || !pDC)
        return FALSE;

    if( rect.Width() <= 0
        || rect.Height() <= 0)  // prevents imagelist item from drawing even
        return FALSE;           //  though cell is hidden

    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (pGrid->GetShadedPrintOut())
    {
        // Get the default cell implementation for this kind of cell. We use it if this cell
        // has anything marked as "default"
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return FALSE;

        // Use custom color if it doesn't match the default color and the
        // default grid background color.  If not, leave it alone.
        if(IsFixed())
            crBG = (GetBackClr() != CLR_DEFAULT) ? GetBackClr() : pDefaultCell->GetBackClr();
        else
            crBG = (GetBackClr() != CLR_DEFAULT && GetBackClr() != pDefaultCell->GetBackClr()) ?
                GetBackClr() : CLR_DEFAULT;

        // Use custom color if the background is different or if it doesn't
        // match the default color and the default grid text color.  
        if(IsFixed())
            crFG = (GetBackClr() != CLR_DEFAULT) ? GetTextClr() : pDefaultCell->GetTextClr();
        else
            crFG = (GetBackClr() != CLR_DEFAULT) ? GetTextClr() : pDefaultCell->GetTextClr();

        // If not printing on a color printer, adjust the foreground color
        // to a gray scale if the background color isn't used so that all
        // colors will be visible.  If not, some colors turn to solid black
        // or white when printed and may not show up.  This may be caused by
        // coarse dithering by the printer driver too (see image note below).
        if(pDC->GetDeviceCaps(NUMCOLORS) == 2 && crBG == CLR_DEFAULT)
            crFG = RGB(GetRValue(crFG) * 0.30, GetGValue(crFG) * 0.59,
                GetBValue(crFG) * 0.11);

        // Only erase the background if the color is not the default
        // grid background color.
        if(crBG != CLR_DEFAULT)
        {
            CBrush brush(crBG);
            rect.right++; rect.bottom++;
            pDC->FillRect(rect, &brush);
            rect.right--; rect.bottom--;
        }
    }
    else
    {
        crBG = CLR_DEFAULT;
        crFG = RGB(0, 0, 0);
    }

    pDC->SetTextColor(crFG);

    CFont *pFont = GetFontObject();
    if (pFont)
        pDC->SelectObject(pFont);

    /*
    // ***************************************************
    // Disabled - if you need this functionality then you'll need to rewrite.
    // Create the appropriate font and select into DC.
    CFont Font;
    // Bold the fixed cells if not shading the print out.  Use italic
    // font it it is enabled.
    const LOGFONT* plfFont = GetFont();
    if(IsFixed() && !pGrid->GetShadedPrintOut())
    {
        Font.CreateFont(plfFont->lfHeight, 0, 0, 0, FW_BOLD, plfFont->lfItalic, 0, 0,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
#ifndef _WIN32_WCE
            PROOF_QUALITY,
#else
            DEFAULT_QUALITY,
#endif
            VARIABLE_PITCH | FF_SWISS, plfFont->lfFaceName);
    }
    else
        Font.CreateFontIndirect(plfFont);

    pDC->SelectObject(&Font);
    // ***************************************************
    */

    // Draw lines only when wanted on fixed cells.  Normal cell grid lines
    // are handled in OnPrint.
    if(pGrid->GetGridLines() != GVL_NONE && IsFixed())
    {
        CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
             darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
            *pOldPen = pDC->GetCurrentPen();

        pDC->SelectObject(&lightpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.left, rect.top);
        pDC->LineTo(rect.left, rect.bottom);

        pDC->SelectObject(&darkpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.right, rect.bottom);
        pDC->LineTo(rect.left, rect.bottom);

        rect.DeflateRect(1,1);
        pDC->SelectObject(pOldPen);
    }

    rect.DeflateRect(GetMargin(), 0);

    if(pGrid->GetImageList() && GetImage() >= 0)
    {
        // NOTE: If your printed images look like fuzzy garbage, check the
        //       settings on your printer driver.  If it's using coarse
        //       dithering and/or vector graphics, they may print wrong.
        //       Changing to fine dithering and raster graphics makes them
        //       print properly.  My HP 4L had that problem.

        IMAGEINFO Info;
        if(pGrid->GetImageList()->GetImageInfo(GetImage(), &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left;
            pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);
            rect.left += nImageWidth+GetMargin();
        }
    }

    // Draw without clipping so as not to lose text when printed for real
	// DT_NOCLIP removed 01.01.01. Slower, but who cares - we are printing!
    //DrawText(pDC->m_hDC, GetText(), -1, rect,
        //GetFormat() | /*DT_NOCLIP | */ DT_NOPREFIX);
	pDC->DrawFrameControl(GetPrintCheckPlacement(rect), DFC_BUTTON, 
	  (m_bChecked)? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);

    pDC->RestoreDC(nSavedDC);

    return TRUE;
#endif
}

CRect CGridCellCheck::GetPrintCheckPlacement(CRect rect)
{
	int nWidth = 13;//GetSystemMetrics(SM_CXHSCROLL);
	CRect place = rect + CSize(GetMargin(), GetMargin());
	//CRect place = m_Rect; + CSize(1, 1);
	
	//for left
    //place.right = place.left + nWidth;
    //place.bottom = place.top + nWidth;
	
	// for centering
	int nDiff = (place.Width() - nWidth)/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	nDiff = (place.Height() - nWidth)/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    
	
    if (rect.Height() < nWidth + 2 * static_cast<int> (GetMargin())) 
    {		
        place.top = rect.top + (rect.Height() - nWidth) / 2;	    
        place.bottom = place.top + nWidth;	
    }
	
	return place;
}