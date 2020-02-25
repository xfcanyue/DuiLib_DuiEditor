// GridCellBase.cpp : implementation file
//
// MFC Grid Control - Main grid cell base class
//
// Provides the implementation for the base cell type of the
// grid control. No data is stored (except for state) but default
// implementations of drawing, printingetc provided. MUST be derived
// from to be used.
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
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
// Ken Bertelson - 12 Apr 2000 - Split CGridCell into CGridCell and CGridCellBase
// C Maunder     - 19 May 2000 - Fixed sort arrow drawing (Ivan Ilinov)
// C Maunder     - 29 Aug 2000 - operator= checks for NULL font before setting (Martin Richter)
// C Maunder     - 15 Oct 2000 - GetTextExtent fixed (Martin Richter)
// C Maunder     -  1 Jan 2001 - Added ValidateEdit
// Yogurt        - 13 Mar 2004 - GetCellExtent fixed
//
// NOTES: Each grid cell should take care of it's own drawing, though the Draw()
//        method takes an "erase background" paramter that is called if the grid
//        decides to draw the entire grid background in on hit. Certain ambient
//        properties such as the default font to use, and hints on how to draw
//        fixed cells should be fetched from the parent grid. The grid trusts the
//        cells will behave in a certain way, and the cells trust the grid will
//        supply accurate information.
//        
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"
#include "GridCellBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CGridCellBase, CObject)

/////////////////////////////////////////////////////////////////////////////
// GridCellBase

CGridCellBase::CGridCellBase()
{
    Reset();
}

CGridCellBase::~CGridCellBase()
{
}

/////////////////////////////////////////////////////////////////////////////
// GridCellBase Operations

void CGridCellBase::Reset()
{
    m_nState  = 0;

	m_Hide=false;
	m_MergeRange.Set();
	m_IsMergeWithOthers=false;
	m_MergeCellID.row=-1;
	m_MergeCellID.col=-1;
}

//Used for merge cells,remove const
//by Huang Wei
void CGridCellBase::operator=(CGridCellBase& cell)
{
	if (this == &cell) return;

    SetGrid(cell.GetGrid());    // do first in case of dependencies

    SetText(cell.GetText());
    SetImage(cell.GetImage());
    SetData(cell.GetData());
    SetState(cell.GetState());
    SetFormat(cell.GetFormat());
    SetTextClr(cell.GetTextClr());
    SetBackClr(cell.GetBackClr());
    SetFont(cell.IsDefaultFont()? NULL : cell.GetFont());
    SetMargin(cell.GetMargin());

	//Used for merge cells
	//by Huang Wei
	SetMergeCellID(cell.GetMergeCellID());
	SetMergeRange(cell.GetMergeRange());
	Show(cell.IsShow());
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Attributes

// Returns a pointer to a cell that holds default values for this particular type of cell
CGridCellBase* CGridCellBase::GetDefaultCell() const
{
    if (GetGrid())
        return GetGrid()->GetDefaultCell(IsFixedRow(), IsFixedCol());
    return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Operations

// EFW - Various changes to make it draw cells better when using alternate
// color schemes.  Also removed printing references as that's now done
// by PrintCell() and fixed the sort marker so that it doesn't draw out
// of bounds.
BOOL CGridCellBase::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    // Note - all through this function we totally brutalise 'rect'. Do not
    // depend on it's value being that which was passed in.

	//Used for merge cells
	//by Huang Wei
	if(	m_Hide && !IsMerged())
	{
		return TRUE;
	}

    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    if (!pGrid || !pDC)
        return FALSE;

    if( rect.Width() <= 0 || rect.Height() <= 0)  // prevents imagelist item from drawing even
        return FALSE;                             //  though cell is hidden

    //TRACE3("Drawing %scell %d, %d\n", IsFixed()? _T("Fixed ") : _T(""), nRow, nCol);

    int nSavedDC = pDC->SaveDC();
    pDC->SetBkMode(TRANSPARENT);


	if(GetPicture())
	{
		CGridCtrl *pGrid = GetGrid();
		if(pGrid)
		{
			GV_CUSTOMDRAW gvc;
			gvc.rc = rect;
			gvc.pDC = pDC;
			gvc.item.row = nRow;
			gvc.item.col = nCol;
			pGrid->SendCustomDrawToParent(&gvc);
		}
		pDC->RestoreDC(nSavedDC);
		return TRUE;
	}
    // Get the default cell implementation for this kind of cell. We use it if this cell
    // has anything marked as "default"
    CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
    if (!pDefaultCell)
        return FALSE;

    // Set up text and background colours
    COLORREF TextClr, TextBkClr;

    TextClr = (GetTextClr() == CLR_DEFAULT)? pDefaultCell->GetTextClr() : GetTextClr();
    if (GetBackClr() == CLR_DEFAULT)
        TextBkClr = pDefaultCell->GetBackClr();
    else
    {
        bEraseBkgnd = TRUE;
        TextBkClr = GetBackClr();
    }

    // Draw cell background and highlighting (if necessary)
	if(IsFixed())
	{
		CBrush brush(TextBkClr);
		pDC->FillRect(rect, &brush);
		pDC->SetTextColor(TextClr);
	}
    else 
		if (IsFocused() || IsDropHighlighted())
    {
        // Always draw even in list mode so that we can tell where the
        // cursor is at.  Use the highlight colors though.
        if(GetState() & GVIS_SELECTED)
        {
            //TextBkClr = ::GetSysColor(COLOR_HIGHLIGHT);
            //TextClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
			//高亮显示颜色可调，Modify By Liqs99
			if(pGrid->GetEnableHighLightBkColor())
			{
				TextBkClr = GetHighLightBkColor();
				bEraseBkgnd = TRUE;
			}
			if(pGrid->GetEnableHighLightTextColor())
			{
				TextClr = GetHighLightTextClr();
				bEraseBkgnd = TRUE;
			}
        }

        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBkgnd)
        {
            TRY 
            {
                CBrush brush(TextBkClr);
                pDC->FillRect(rect, &brush);
            } 
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }

        // Don't adjust frame rect if no grid lines so that the
        // whole cell is enclosed.
        if(pGrid->GetGridLines() != GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

        if (pGrid->GetFrameFocusCell())
        {
                // Use same color as text to outline the cell so that it shows
                // up if the background is black.
            TRY 
            {
                //CBrush brush(TextClr);
				CBrush brush(pGrid->GetFrameFocusCellColor());	//modify by liqs99
                pDC->FrameRect(rect, &brush);
            }
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }
        pDC->SetTextColor(TextClr);

        // Adjust rect after frame draw if no grid lines
        if(pGrid->GetGridLines() == GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

		//rect.DeflateRect(0,1,1,1);  - Removed by Yogurt
    }
    else if ((GetState() & GVIS_SELECTED) && pGrid->GetEnableHighLightBkColor())
    {
		//高亮显示颜色可调，下面注释掉的2行.Modify By Liqs99
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        //pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillSolidRect(rect, GetHighLightBkColor());
        rect.right--; rect.bottom--;
        //pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		if(pGrid->GetEnableHighLightTextColor())
			pDC->SetTextColor(GetHighLightTextClr());
		else
			pDC->SetTextColor(TextClr);
    }
    else
    {
        if (bEraseBkgnd)
        {
            //rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom // why? ask by liqs99
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
            //rect.right--; rect.bottom--;
        }
        pDC->SetTextColor(TextClr);
    }

    // Draw lines only when wanted
    if (IsFixed() && pGrid->GetGridLines() != GVL_NONE)
    {
        CCellID FocusCell = pGrid->GetFocusCell();

        // As above, always show current location even in list mode so
        // that we know where the cursor is at.
        BOOL bHiliteFixed = pGrid->GetTrackFocusCell() && pGrid->IsValid(FocusCell) &&
                            (FocusCell.row == nRow || FocusCell.col == nCol);

        // If this fixed cell is on the same row/col as the focus cell,
        // highlight it.
        if (bHiliteFixed)
        {
            rect.right++; rect.bottom++;
            pDC->DrawEdge(rect, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT);
            rect.DeflateRect(1,1);
			
			/***************************************************************/
			//Added by liqs99
			//ListMode，列表模式下，选中行的固定列显示一个小三角符号
// 			CRect cellRect;
// 			GetTextRect(&cellRect);
// 			if(IsFixedCol()&&pGrid->GetListMode())	
// 				pDC->DrawFrameControl(rect, DFC_SCROLL, DFCS_SCROLLRIGHT);
			/*************************************************************************/
        }
        else
        {
// 			if(IsMerged())
// 			{
// 				rect.right++; rect.bottom++;
// 			}

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

            pDC->SelectObject(pOldPen);
            rect.DeflateRect(1,1);
        }
    }

    // Draw Text and image
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
    if (!pDC->m_bPrinting)
#endif
    {
        CFont *pFont = GetFontObject();
		ASSERT(pFont);
        if (pFont)
            pDC->SelectObject(pFont);
    }

    //rect.DeflateRect(GetMargin(), 0); - changed by Yogurt
    rect.DeflateRect(GetMargin(), GetMargin());    
    rect.right++;    
    rect.bottom++;

    if (pGrid->GetImageList() && GetImage() >= 0)
    {
        IMAGEINFO Info;
        if (pGrid->GetImageList()->GetImageInfo(GetImage(), &Info))
        {
            //  would like to use a clipping region but seems to have issue
            //  working with CMemDC directly.  Instead, don't display image
            //  if any part of it cut-off
            //
            // CRgn rgn;
            // rgn.CreateRectRgnIndirect(rect);
            // pDC->SelectClipRgn(&rgn);
            // rgn.DeleteObject();

            /*
            // removed by Yogurt
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            int nImageHeight = Info.rcImage.bottom-Info.rcImage.top+1;
            if( nImageWidth + rect.left <= rect.right + (int)(2*GetMargin())
                && nImageHeight + rect.top <= rect.bottom + (int)(2*GetMargin())  )
            {
                pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);
            }
            */
            // Added by Yogurt
            int nImageWidth = Info.rcImage.right-Info.rcImage.left;            
            int nImageHeight = Info.rcImage.bottom-Info.rcImage.top;            
            if ((nImageWidth + rect.left <= rect.right) && (nImageHeight + rect.top <= rect.bottom))                
                pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);

            //rect.left += nImageWidth+GetMargin();
        }
    }

    // Draw sort arrow
    if (pGrid->GetSortColumn() == nCol && nRow == 0)
    {
        CSize size = pDC->GetTextExtent(_T("M"));
        int nOffset = 2;

        // Base the size of the triangle on the smaller of the column
        // height or text height with a slight offset top and bottom.
        // Otherwise, it can get drawn outside the bounds of the cell.
        size.cy -= (nOffset * 2);

        if (size.cy >= rect.Height())
            size.cy = rect.Height() - (nOffset * 2);

        size.cx = size.cy;      // Make the dimensions square

        // Kludge for vertical text
        BOOL bVertical = (GetFont()->lfEscapement == 900);

        // Only draw if it'll fit!
        //if (size.cx + rect.left < rect.right + (int)(2*GetMargin())) - changed / Yogurt
        if (size.cx + rect.left < rect.right)
        {
            int nTriangleBase = rect.bottom - nOffset - size.cy;    // Triangle bottom right
            //int nTriangleBase = (rect.top + rect.bottom - size.cy)/2; // Triangle middle right
            //int nTriangleBase = rect.top + nOffset;                 // Triangle top right

            //int nTriangleLeft = rect.right - size.cx;                 // Triangle RHS
            //int nTriangleLeft = (rect.right + rect.left - size.cx)/2; // Triangle middle
            //int nTriangleLeft = rect.left;                            // Triangle LHS

            int nTriangleLeft;
            if (bVertical)
                nTriangleLeft = (rect.right + rect.left - size.cx)/2; // Triangle middle
            else
                nTriangleLeft = rect.right - size.cx;               // Triangle RHS

            CPen penShadow(PS_SOLID, 0, ::GetSysColor(COLOR_3DSHADOW));
            CPen penLight(PS_SOLID, 0, ::GetSysColor(COLOR_3DHILIGHT));
            if (pGrid->GetSortAscending())
            {
                // Draw triangle pointing upwards
                CPen *pOldPen = (CPen*) pDC->SelectObject(&penLight);
                pDC->MoveTo( nTriangleLeft + 1, nTriangleBase + size.cy + 1);
                pDC->LineTo( nTriangleLeft + (size.cx / 2) + 1, nTriangleBase + 1 );
                pDC->LineTo( nTriangleLeft + size.cx + 1, nTriangleBase + size.cy + 1);
                pDC->LineTo( nTriangleLeft + 1, nTriangleBase + size.cy + 1);

                pDC->SelectObject(&penShadow);
                pDC->MoveTo( nTriangleLeft, nTriangleBase + size.cy );
                pDC->LineTo( nTriangleLeft + (size.cx / 2), nTriangleBase );
                pDC->LineTo( nTriangleLeft + size.cx, nTriangleBase + size.cy );
                pDC->LineTo( nTriangleLeft, nTriangleBase + size.cy );
                pDC->SelectObject(pOldPen);
            }
            else
            {
                // Draw triangle pointing downwards
                CPen *pOldPen = (CPen*) pDC->SelectObject(&penLight);
                pDC->MoveTo( nTriangleLeft + 1, nTriangleBase + 1 );
                pDC->LineTo( nTriangleLeft + (size.cx / 2) + 1, nTriangleBase + size.cy + 1 );
                pDC->LineTo( nTriangleLeft + size.cx + 1, nTriangleBase + 1 );
                pDC->LineTo( nTriangleLeft + 1, nTriangleBase + 1 );
    
                pDC->SelectObject(&penShadow);
                pDC->MoveTo( nTriangleLeft, nTriangleBase );
                pDC->LineTo( nTriangleLeft + (size.cx / 2), nTriangleBase + size.cy );
                pDC->LineTo( nTriangleLeft + size.cx, nTriangleBase );
                pDC->LineTo( nTriangleLeft, nTriangleBase );
                pDC->SelectObject(pOldPen);
            }
            
            if (!bVertical)
                rect.right -= size.cy;
        }
    }



    // We want to see '&' characters so use DT_NOPREFIX
	//原来是 GetTextRect(rect),我改成GetTextRect(&rect); 
	//难道是作者写错了？ Modify By Liqs99
    GetTextRect(&rect);
	
    rect.right++;    
    rect.bottom++;
//	rect.left+=3;

	if(IsFixedCol()&&pGrid->GetViewFixedColNo() && nCol==0)
	{
		if(!IsFixedRow()) //固定列 并且 不是固定行 显示行号
		{
			int nStat = GetStatCell();
			if(nStat == -1) //这不是统计行，显示行号
			{
				CString temp;
				temp.Format(_T("%d"), nRow - pGrid->GetFixedRowCount() + 1);
				DrawText(pDC->m_hDC, temp, -1, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			}
			else 
				DrawText(pDC->m_hDC, GetText(), -1, rect, GetFormat() | DT_NOPREFIX);
		}
		else
			DrawText(pDC->m_hDC, GetText(), -1, rect, GetFormat() | DT_NOPREFIX);
	}
	else
		DrawText(pDC->m_hDC, GetText(), -1, rect, GetFormat() | DT_NOPREFIX);

    pDC->RestoreDC(nSavedDC);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Mouse and Cursor events

// Not yet implemented
void CGridCellBase::OnMouseEnter()
{
    TRACE0("Mouse entered cell\n");
}

void CGridCellBase::OnMouseOver()
{
    //TRACE0("Mouse over cell\n");
}

// Not Yet Implemented
void CGridCellBase::OnMouseLeave()
{
    TRACE0("Mouse left cell\n");
}

void CGridCellBase::OnClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse Left btn up in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnClickDown( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse Left btn down in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnRClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse right-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CGridCellBase::OnDblClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    TRACE2("Mouse double-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

// Return TRUE if you set the cursor
BOOL CGridCellBase::OnSetCursor()
{
#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase editing

void CGridCellBase::OnEndEdit() 
{
	ASSERT( FALSE); 
}

BOOL CGridCellBase::ValidateEdit(LPCTSTR str)
{
    UNUSED_ALWAYS(str);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellBase Sizing

BOOL CGridCellBase::GetTextRect( LPRECT pRect)  // i/o:  i=dims of cell rect; o=dims of text rect
{
    if (GetImage() >= 0)
    {
        IMAGEINFO Info;

        CGridCtrl* pGrid = GetGrid();
        CImageList* pImageList = pGrid->GetImageList();
        
        if (pImageList && pImageList->GetImageInfo( GetImage(), &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            pRect->left += nImageWidth + GetMargin();
        }
    }

    return TRUE;
}

// By default this uses the selected font (which is a bigger font)
CSize CGridCellBase::GetTextExtent(LPCTSTR szText, CDC* pDC /*= NULL*/)
{
    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    BOOL bReleaseDC = FALSE;
    if (pDC == NULL || szText == NULL)
    {
        if (szText)
			pDC = pGrid->GetDC();
        if (pDC == NULL || szText == NULL) 
        {
            CGridDefaultCell* pDefCell = (CGridDefaultCell*) GetDefaultCell();
            ASSERT(pDefCell);
            return CSize(pDefCell->GetWidth(), pDefCell->GetHeight());
        }
        bReleaseDC = TRUE;
    }

    CFont *pOldFont = NULL,
          *pFont = GetFontObject();
    if (pFont)
        pOldFont = pDC->SelectObject(pFont);

    CSize size;
    int nFormat = GetFormat();

    // If the cell is a multiline cell, then use the width of the cell
    // to get the height
    if ((nFormat & DT_WORDBREAK) && !(nFormat & DT_SINGLELINE))
    {
        CString str = szText;
        int nMaxWidth = 0;
        while (TRUE)
        {
            int nPos = str.Find(_T('\n'));
            CString TempStr = (nPos < 0)? str : str.Left(nPos);
            int nTempWidth = pDC->GetTextExtent(TempStr).cx;
            if (nTempWidth > nMaxWidth)
                nMaxWidth = nTempWidth;

            if (nPos < 0)
                break;
            str = str.Mid(nPos + 1);    // Bug fix by Thomas Steinborn
        }
        
        CRect rect;
        rect.SetRect(0,0, nMaxWidth+1, 0);
        pDC->DrawText(szText, -1, rect, nFormat | DT_CALCRECT);
        size = rect.Size();
    }
    else
        size = pDC->GetTextExtent(szText, (int)_tcslen(szText));

    // Removed by Yogurt
    //TEXTMETRIC tm;
    //pDC->GetTextMetrics(&tm);
    //size.cx += (tm.tmOverhang);

    if (pOldFont)
        pDC->SelectObject(pOldFont);
    
    size += CSize(2*GetMargin(), 2*GetMargin());

    // Kludge for vertical text
    LOGFONT *pLF = GetFont();
    if (pLF->lfEscapement == 900 || pLF->lfEscapement == -900)
    {
        int nTemp = size.cx;
        size.cx = size.cy;
        size.cy = nTemp;
        size += CSize(0, 4*GetMargin());
    }
    
    if (bReleaseDC)
        pGrid->ReleaseDC(pDC);

    return size;
}


CSize CGridCellBase::GetCellExtent(CDC* pDC)
{    
    CSize size = GetTextExtent(GetText(), pDC);    
    CSize ImageSize(0,0);    
    
    int nImage = GetImage();    
    if (nImage >= 0)    
    {        
        CGridCtrl* pGrid = GetGrid();        
        ASSERT(pGrid);        
        IMAGEINFO Info;        
        if (pGrid->GetImageList() && pGrid->GetImageList()->GetImageInfo(nImage, &Info))         
        {            
            ImageSize = CSize(Info.rcImage.right-Info.rcImage.left,                                 
                Info.rcImage.bottom-Info.rcImage.top);            
            if (size.cx > 2*(int)GetMargin ())                
                ImageSize.cx += GetMargin();            
            ImageSize.cy += 2*(int)GetMargin ();        
        }    
    }    
    size.cx += ImageSize.cx + 1;    
    size.cy = max(size.cy, ImageSize.cy) + 1;    
    if (IsFixed())    
    {        
        size.cx++;        
        size.cy++;    
    }    
    return size;
}

// EFW - Added to print cells so that grids that use different colors are
// printed correctly.
BOOL CGridCellBase::PrintCell(CDC* pDC, int /*nRow*/, int /*nCol*/, CRect rect)
{
	//Used for merge cells
	//by Huang Wei
	if(	m_Hide && !IsMerged())
	{
		return TRUE;
	}

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

	//Used for merge cells
	//by Huang Wei
	rect.InflateRect(1,1);
	pDC->Rectangle(rect);
	rect.DeflateRect(1,1);

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
    DrawText(pDC->m_hDC, GetText(), -1, rect,
        GetFormat() | /*DT_NOCLIP | */ DT_NOPREFIX);

    pDC->RestoreDC(nSavedDC);

    return TRUE;
#endif
}

/*****************************************************************************
Callable by derived classes, only
*****************************************************************************/
LRESULT CGridCellBase::SendMessageToParent(int nRow, int nCol, int nMessage)
{
    CGridCtrl* pGrid = GetGrid();
    if( pGrid)
        return pGrid->SendMessageToParent(nRow, nCol, nMessage);
    else
        return 0;
}

//Used for merge cells
//by Huang Wei
void CGridCellBase::Show(bool IsShow)
{
	m_Hide=!IsShow;
}
//Used for merge cells
//by Huang Wei
void CGridCellBase::SetMergeRange(CCellRange range)
{
	m_MergeRange=range;
}
//Used for merge cells
//by Huang Wei
CCellRange CGridCellBase::GetMergeRange()
{
	return m_MergeRange;
}
//Used for merge cells
//by Huang Wei
bool CGridCellBase::IsMerged()
{
	return m_MergeRange.Count()>1;
}
//Used for merge cells
//by Huang Wei
void CGridCellBase::SetMergeCellID(CCellID cell)
{
	m_MergeCellID=cell;
	if(cell.row!=-1)
		m_IsMergeWithOthers=true;
	else
		m_IsMergeWithOthers=false;

}
//Used for merge cells
//by Huang Wei
CCellID CGridCellBase::GetMergeCellID()
{
	return m_MergeCellID;
}
//Used for merge cells
//by Huang Wei
bool CGridCellBase::IsMergeWithOthers()
{
	return m_IsMergeWithOthers;
}
//Used for merge cells
//by Huang Wei
bool CGridCellBase::IsShow()
{
	return !m_Hide;
}
//Used for merge cells
//by Huang Wei
void CGridCellBase::UnMerge()
{
	m_Hide=false;
	m_MergeRange.Set();
	m_IsMergeWithOthers=false;
	m_MergeCellID.row=-1;
	m_MergeCellID.col=-1;
}