// GridCtrl.cpp : implementation file
//
// MFC Grid Control v2.26
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridMemDC.h"
#include "GridCtrl.h"
#include <algorithm>

// OLE stuff for clipboard operations
#include <afxadv.h>            // For CSharedFile
#include <afxconv.h>           // For LPTSTR -> LPSTR macros

#include "../ExcelFile/ExcelFile.h"
#include "../ExcelFile/ExcelFile.cpp"
#include "../ExcelFile/ExcelApplication.h"
#include "../ExcelFile/ExcelBook.h"
#include "../ExcelFile/ExcelBooks.h"
#include "../ExcelFile/ExcelSheet.h"
#include "../ExcelFile/ExcelSheets.h"
#include "../ExcelFile/ExcelRange.h"
#include "../ExcelFile/ExcelRangeEx.h"
#include "../ExcelFile/ExcelRangeEx.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Spit out some messages as a sanity check for programmers
#ifdef GRIDCONTROL_NO_TITLETIPS
#pragma message(" -- CGridCtrl: No titletips for cells with large data")
#endif
#ifdef GRIDCONTROL_NO_DRAGDROP
#pragma message(" -- CGridCtrl: No OLE drag and drop")
#endif
#ifdef GRIDCONTROL_NO_CLIPBOARD
#pragma message(" -- CGridCtrl: No clipboard support")
#endif
#ifdef GRIDCONTROL_NO_PRINTING
#pragma message(" -- CGridCtrl: No printing support")
#endif


IMPLEMENT_DYNCREATE(CGridCtrl, CWnd)


// Get the number of lines to scroll with each mouse wheel notch
// Why doesn't windows give us this function???
UINT GetMouseScrollLines()
{
    int nScrollLines = 3;            // reasonable default

#ifndef _WIN32_WCE
    // Do things the hard way in win95
    OSVERSIONINFO VersionInfo;
    VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!GetVersionEx(&VersionInfo) || 
        (VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && VersionInfo.dwMinorVersion == 0))
    {
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
            0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
        {
            TCHAR szData[128];
            DWORD dwKeyDataType;
            DWORD dwDataBufSize = sizeof(szData);
            
            if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
            {
                nScrollLines = _tcstoul(szData, NULL, 10);
            }
            RegCloseKey(hKey);
        }
    }
    // win98 or greater
    else
           SystemParametersInfo (SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);
#endif

    return nScrollLines;
}

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl

CGridCtrl::CGridCtrl(int nRows, int nCols, int nFixedRows, int nFixedCols)
{
    RegisterWindowClass();

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState->m_bNeedTerm && !AfxOleInit())
        AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
#endif

/*******************************
我自己添加的内容，Added By liqs99
*******************************/

//高亮显示时的背景色和文字颜色
m_bHighLightBkColor = TRUE;
m_crHighLightBkColor = ::GetSysColor(COLOR_HIGHLIGHT);
m_bHighLightTextColor = TRUE;
m_ctHighLightTextClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

m_bPrintCenterTitle	= TRUE;
m_bPrintRightTitle	= TRUE;
m_bPrintLeftSecondTile = FALSE;

m_bViewFixedColNo = FALSE;

m_nStatRows = 0;

m_RectTracker.pGrid = this;
m_RectTracker.m_nStyle = CRectTracker::resizeOutside|CRectTracker::solidLine;
m_RectTracker.m_nHandleSize = 6;
m_RectTracker.m_rect.SetRect(0,0,0,0);
/******************************/
    // Store the system colours in case they change. The gridctrl uses
    // these colours, and in OnSysColorChange we can check to see if
    // the gridctrl colours have been changed from the system colours.
    // If they have, then leave them, otherwise change them to reflect
    // the new system colours.
    m_crWindowText        = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour      = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace            = ::GetSysColor(COLOR_3DFACE);
    m_crShadow            = ::GetSysColor(COLOR_3DSHADOW);
    m_crGridLineColour    = RGB(192,192,192);

    m_nRows               = 0;
    m_nCols               = 0;
    m_nFixedRows          = 0;
    m_nFixedCols          = 0;
	m_InDestructor        = false;

    m_bVirtualMode        = FALSE;
    m_pfnCallback         = NULL;

    m_nVScrollMax         = 0;          // Scroll position
    m_nHScrollMax         = 0;
    m_nRowsPerWheelNotch  = GetMouseScrollLines(); // Get the number of lines
                                                   // per mouse wheel notch to scroll
    m_nBarState           = GVL_NONE;
    m_MouseMode           = MOUSE_NOTHING;
    m_nGridLines          = GVL_BOTH;
    m_bEditable           = TRUE;
    m_bListMode           = FALSE;
    m_bSingleRowSelection = FALSE;
    m_bSingleColSelection = FALSE;
    m_bLMouseButtonDown   = FALSE;
    m_bRMouseButtonDown   = FALSE;
    m_bAllowDraw          = TRUE;       // allow draw updates
    m_bEnableSelection    = TRUE;
    m_bFixedColumnSelection = TRUE;
    m_bFixedRowSelection  = TRUE;
    m_bAllowRowResize     = TRUE;
    m_bAllowColumnResize  = TRUE;
    m_bSortOnClick        = TRUE;      //允许点击标题头排序 Sort on header row click
    m_bHandleTabKey       = TRUE;
#ifdef _WIN32_WCE
    m_bDoubleBuffer       = FALSE;      // Use double buffering to avoid flicker?
#else
    m_bDoubleBuffer       = TRUE;       // Use double buffering to avoid flicker?
#endif
    m_bTitleTips          = FALSE;       // 不允许单元格浮动显示don't show cell title tips

    m_bWysiwygPrinting    = TRUE;      // use size-to-width printing,表格实际大小打印

    m_bHiddenColUnhide    = TRUE;       // 0-width columns can be expanded via mouse
    m_bHiddenRowUnhide    = TRUE;       // 0-Height rows can be expanded via mouse

    m_bAllowColHide       = TRUE;       // Columns can be contracted to 0-width via mouse
    m_bAllowRowHide       = TRUE;       // Rows can be contracted to 0-height via mouse

    m_bAscending          = TRUE;       // sorting stuff
    m_nSortColumn         = -1;
	m_pfnCompare		  = NULL;
	m_pfnVirtualCompare   = NULL;
    m_nAutoSizeColumnStyle = GVS_BOTH;  // Autosize grid using header and data info

    m_nTimerID            = 0;          // For drag-selection
    m_nTimerInterval      = 25;         // (in milliseconds)
    m_nResizeCaptureRange = 3;          // When resizing columns/row, the cursor has to be
                                        // within +/-3 pixels of the dividing line for
                                        // resizing to be possible
    m_pImageList          = NULL;       // Images in the grid
    m_bAllowDragAndDrop   = FALSE;      // for drag and drop - EFW - off by default
    m_bTrackFocusCell     = TRUE;       // Track Focus cell?
    m_bFrameFocus         = TRUE;       // Frame the selected cell?
	m_AllowReorderColumn	= false;
	m_QuitFocusOnTab = false;
	m_AllowSelectRowInFixedCol = false;
	m_bDragRowMode = TRUE; // allow to drop a line over another one to change row order
    m_pRtcDefault = RUNTIME_CLASS(CGridCell);

    SetupDefaultCells();
    SetGridBkColor(m_crShadow);

    // Set up the initial grid size
    SetRowCount(nRows);
    SetColumnCount(nCols);
    SetFixedRowCount(nFixedRows);
    SetFixedColumnCount(nFixedCols);

    SetTitleTipTextClr(CLR_DEFAULT);  //FNA
    SetTitleTipBackClr(CLR_DEFAULT); 

    // set initial selection range (ie. none)
    m_SelectedCellMap.RemoveAll();
    m_PrevSelectedCellMap.RemoveAll();

#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
    // EFW - Added to support shaded/unshaded printout and
    // user-definable margins.
    m_bShadedPrintOut = TRUE;
    SetPrintMarginInfo(2, 2, 4, 4, 1, 1, 1);
#endif
}

CGridCtrl::~CGridCtrl()
{
	m_InDestructor = true;
    DeleteAllItems();

#ifndef GRIDCONTROL_NO_TITLETIPS
    if (m_bTitleTips && ::IsWindow(m_TitleTip.GetSafeHwnd())) 
        m_TitleTip.DestroyWindow();
#endif

    DestroyWindow();

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    // BUG FIX - EFW
    COleDataSource *pSource = COleDataSource::GetClipboardOwner();
    if(pSource)
        COleDataSource::FlushClipboard();
#endif
}

// Register the window class if it has not already been registered.
BOOL CGridCtrl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
        wndcls.hCursor          = 0;
#endif
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CGridCtrl::Initialise()
{
    // Stop re-entry problems
    static BOOL bInProcedure = FALSE;
    if (bInProcedure)
        return FALSE;
    bInProcedure = TRUE;

#ifndef GRIDCONTROL_NO_TITLETIPS
    m_TitleTip.SetParentWnd(this);
#endif

	// This would be a good place to register the droptarget but
	// unfortunately this causes problems if you are using the 
	// grid in a view. 
	// Moved from OnSize.
//#ifndef GRIDCONTROL_NO_DRAGDROP
//    m_DropTarget.Register(this);
//#endif

    if (::IsWindow(m_hWnd))
        ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	// Kludge: Make sure the client edge shows
	// This is so horrible it makes my eyes water.
	CRect rect;
	GetWindowRect(rect);
	CWnd* pParent = GetParent();
	if (pParent != NULL)
		pParent->ScreenToClient(rect);
	rect.InflateRect(1,1);	MoveWindow(rect);
	rect.DeflateRect(1,1);  MoveWindow(rect);
 
    bInProcedure = FALSE;
    return TRUE;
}

BOOL CGridCtrl::Create(int x, int y, int nWidth, int nHeight, 
					   CWnd* parent, UINT nID,DWORD dwStyle)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x+nWidth;
	rect.bottom = y+nHeight;
	
	return	Create(rect, parent, nID, dwStyle); 
}
// creates the control - use like any other window create control
BOOL CGridCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
    ASSERT(pParentWnd->GetSafeHwnd());

    if (!CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
        return FALSE;

    //Initialise(); - called in PreSubclassWnd

    // The number of rows and columns will only be non-zero if the constructor
    // was called with non-zero initialising parameters. If this window was created
    // using a dialog template then the number of rows and columns will be 0 (which
    // means that the code below will not be needed - which is lucky 'cause it ain't
    // gonna get called in a dialog-template-type-situation.

    TRY
    {
        m_arRowHeights.SetSize(m_nRows);    // initialize row heights
        m_arColWidths.SetSize(m_nCols);     // initialize column widths
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        return FALSE;
    }
    END_CATCH

    int i;
    for (i = 0; i < m_nRows; i++)
        m_arRowHeights[i] = m_cellDefault.GetHeight();
    for (i = 0; i < m_nCols; i++)
        m_arColWidths[i] = m_cellDefault.GetWidth();

    return TRUE;
}




void CGridCtrl::SetupDefaultCells()
{
    m_cellDefault.SetGrid(this);            // Normal editable cell
    m_cellFixedColDef.SetGrid(this);        // Cell for fixed columns
    m_cellFixedRowDef.SetGrid(this);        // Cell for fixed rows
    m_cellFixedRowColDef.SetGrid(this);     // Cell for area overlapped by fixed columns/rows

    m_cellDefault.SetTextClr(m_crWindowText);   
    m_cellDefault.SetBackClr(m_crWindowColour); 
    m_cellFixedColDef.SetTextClr(m_crWindowText);
    m_cellFixedColDef.SetBackClr(m_cr3DFace);
    m_cellFixedRowDef.SetTextClr(m_crWindowText);
    m_cellFixedRowDef.SetBackClr(m_cr3DFace);
    m_cellFixedRowColDef.SetTextClr(m_crWindowText);
    m_cellFixedRowColDef.SetBackClr(m_cr3DFace);
}

void CGridCtrl::PreSubclassWindow()
{
    CWnd::PreSubclassWindow();

    //HFONT hFont = ::CreateFontIndirect(m_cellDefault.GetFont());
    //OnSetFont((LPARAM)hFont, 0);
    //DeleteObject(hFont);

    Initialise();
}

// Sends a message to the parent in the form of a WM_NOTIFY message with
// a NM_GRIDVIEW structure attached
LRESULT CGridCtrl::SendMessageToParent(int nRow, int nCol, int nMessage) const
{
    if (!IsWindow(m_hWnd))
        return 0;

    NM_GRIDVIEW nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

// Send a request to the parent to return information on a given cell
LRESULT CGridCtrl::SendDisplayRequestToParent(GV_DISPINFO* pDisplayInfo) const
{
    if (!IsWindow(m_hWnd))
        return 0;

    // Fix up the message headers
    pDisplayInfo->hdr.hwndFrom = m_hWnd;
    pDisplayInfo->hdr.idFrom   = GetDlgCtrlID();
    pDisplayInfo->hdr.code     = GVN_GETDISPINFO;

    // Send the message
    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, pDisplayInfo->hdr.idFrom, (LPARAM)pDisplayInfo);
    else
        return 0;
}

// Send a hint to the parent about caching information
LRESULT CGridCtrl::SendCacheHintToParent(const CCellRange& range) const
{
    if (!IsWindow(m_hWnd))
        return 0;

    GV_CACHEHINT CacheHint;

    // Fix up the message headers
    CacheHint.hdr.hwndFrom = m_hWnd;
    CacheHint.hdr.idFrom   = GetDlgCtrlID();
    CacheHint.hdr.code     = GVN_ODCACHEHINT;

    CacheHint.range = range;

    // Send the message
    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, CacheHint.hdr.idFrom, (LPARAM)&CacheHint);
    else
        return 0;
}

LRESULT CGridCtrl::SendCustomDrawToParent(GV_CUSTOMDRAW *pCustomDraw) const
{
	if (!IsWindow(m_hWnd))
		return 0;

	// Fix up the message headers
	pCustomDraw->hdr.hwndFrom = m_hWnd;
	pCustomDraw->hdr.idFrom   = GetDlgCtrlID();
	pCustomDraw->hdr.code     = GVN_CUSTOMDRAW;

	// Send the message
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		return pOwner->SendMessage(WM_NOTIFY, pCustomDraw->hdr.idFrom, (LPARAM)pCustomDraw);
	else
		return 0;
}

#define LAYER_SIGNATURE (0x5FD4E64)
int CGridCtrl::GetLayer(int** pLayer) // used to save and restore order of columns
{ //  gives back the size of the area (do not forget to delete pLayer)
	int Length = 2+GetColumnCount()*2;
	int *Layer = new int[Length];	// the caller is supposed to delete it
	Layer[0]= LAYER_SIGNATURE;
	Layer[1]= GetColumnCount();
	memcpy(&Layer[2], &m_arColOrder[0], GetColumnCount()*sizeof(int));
	memcpy(&Layer[2+GetColumnCount()], &m_arColWidths[0], GetColumnCount()*sizeof(int));
	*pLayer = Layer;
	return Length;
}
void CGridCtrl::SetLayer(int* pLayer)
{ // coming from a previous GetLayer (ignored if not same number of column, or the same revision number)
	if(pLayer[0] != LAYER_SIGNATURE) return;
	if(pLayer[1] != GetColumnCount()) return;
/*	TRACE("  %d == %d \n",m_arColOrder[0],pLayer[2]);
	TRACE("  %d == %d \n",m_arColOrder[1],pLayer[3]);
	TRACE("  %d == %d \n",m_arColOrder[2],pLayer[4]);
	TRACE("  %d == %d \n",m_arColWidths[0],pLayer[2+3]);
	TRACE("  %d == %d \n",m_arColWidths[1],pLayer[3+3]);
	TRACE("  %d == %d \n",m_arColWidths[2],pLayer[4+3]);
	TRACE("  %d == %d \n",GetColumnCount(),3);

	
	ASSERT(m_arColOrder[0]==pLayer[2]);
	ASSERT(m_arColOrder[1]==pLayer[3]);
	ASSERT(m_arColOrder[2]==pLayer[4]);
	ASSERT(m_arColWidths[0]==pLayer[2+3]);
	ASSERT(m_arColWidths[1]==pLayer[3+3]);
	ASSERT(m_arColWidths[2]==pLayer[4+3]);
	ASSERT(GetColumnCount()==3);
*/	memcpy(&m_arColOrder[0],&pLayer[2], GetColumnCount()*sizeof(int));
	memcpy(&m_arColWidths[0],&pLayer[2+GetColumnCount()], GetColumnCount()*sizeof(int));
}

BEGIN_MESSAGE_MAP(CGridCtrl, CWnd)
//EFW - Added ON_WM_RBUTTONUP
//{{AFX_MSG_MAP(CGridCtrl)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_GETDLGCODE()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
    ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
    ON_WM_SYSKEYDOWN()
//}}AFX_MSG_MAP
#ifndef _WIN32_WCE_NO_CURSOR
    ON_WM_SETCURSOR()
#endif
#ifndef _WIN32_WCE
    ON_WM_RBUTTONUP()
    ON_WM_SYSCOLORCHANGE()
    ON_WM_CAPTURECHANGED()
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
#endif
#if (_WIN32_WCE >= 210)
    ON_WM_SETTINGCHANGE()
#endif
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
    ON_WM_MOUSEWHEEL()
#endif
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    ON_MESSAGE(WM_GETFONT, OnGetFont)
    ON_MESSAGE(WM_IME_CHAR, OnImeChar)
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_INPLACE_CONTROL, OnEndInPlaceEdit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridCtrl message handlers

void CGridCtrl::OnPaint()
{
    CPaintDC dc(this);      // device context for painting

    if (m_bDoubleBuffer)    // Use a memory DC to remove flicker
    {
        CGridMemDC MemDC(&dc);
        OnDraw(&MemDC);
    }
    else                    // Draw raw - this helps in debugging vis problems.
        OnDraw(&dc);
}

BOOL CGridCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;    // Don't erase the background.
}

// Custom background erasure. This gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CGridCtrl::EraseBkgnd(CDC* pDC)
{
    CRect  VisRect, ClipRect, rect;
    CBrush FixedRowColBack(GetDefaultCell(TRUE, TRUE)->GetBackClr()),
           FixedRowBack(GetDefaultCell(TRUE, FALSE)->GetBackClr()),
           FixedColBack(GetDefaultCell(FALSE, TRUE)->GetBackClr()),
           TextBack(GetDefaultCell(FALSE, FALSE)->GetBackClr());
    CBrush Back(GetGridBkColor()); 
    //CBrush Back(GetTextBkColor());

    if (pDC->GetClipBox(ClipRect) == ERROR)
        return;
    GetVisibleNonFixedCellRange(VisRect);

    int nFixedColumnWidth = GetFixedColumnWidth();
    int nFixedRowHeight = GetFixedRowHeight();

    // Draw Fixed row/column background
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < nFixedRowHeight)
        pDC->FillRect(CRect(ClipRect.left, ClipRect.top, 
                      nFixedColumnWidth, nFixedRowHeight),
                      &FixedRowColBack);

    // Draw Fixed columns background
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom)
        pDC->FillRect(CRect(ClipRect.left, ClipRect.top, 
                      nFixedColumnWidth, VisRect.bottom),
                      &FixedColBack);
        
    // Draw Fixed rows background
    if (ClipRect.top < nFixedRowHeight && 
        ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right)
        pDC->FillRect(CRect(nFixedColumnWidth-1, ClipRect.top,
                      VisRect.right, nFixedRowHeight),
                      &FixedRowBack);

    // Draw non-fixed cell background
    if (rect.IntersectRect(VisRect, ClipRect)) 
    {
        CRect CellRect(__max(nFixedColumnWidth, rect.left), 
                       __max(nFixedRowHeight, rect.top),
                       rect.right, rect.bottom);
        pDC->FillRect(CellRect, &TextBack);
    }

    // Draw right hand side of window outside grid
    if (VisRect.right < ClipRect.right) 
        pDC->FillRect(CRect(VisRect.right, ClipRect.top, 
                      ClipRect.right, ClipRect.bottom),
                      &Back);

    // Draw bottom of window below grid
    if (VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right) 
        pDC->FillRect(CRect(ClipRect.left, VisRect.bottom,
                      VisRect.right, ClipRect.bottom),
                      &Back);
}

void CGridCtrl::OnSize(UINT nType, int cx, int cy)
{  
    static BOOL bAlreadyInsideThisProcedure = FALSE;
    if (bAlreadyInsideThisProcedure)
        return;

    if (!::IsWindow(m_hWnd))
        return;

	// This is not the ideal place to register the droptarget
#ifndef GRIDCONTROL_NO_DRAGDROP
	m_DropTarget.Register(this);
#endif

    // Start re-entry blocking
    bAlreadyInsideThisProcedure = TRUE;

    EndEditing();        // destroy any InPlaceEdit's
    CWnd::OnSize(nType, cx, cy);
    ResetScrollBars();

    // End re-entry blocking
    bAlreadyInsideThisProcedure = FALSE;
}

UINT CGridCtrl::OnGetDlgCode()
{
    UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS; // DLGC_WANTALLKEYS; //

    if (m_bHandleTabKey && !IsCTRLpressed())
        nCode |= DLGC_WANTTAB;

    return nCode;
}

#ifndef _WIN32_WCE
// If system colours change, then redo colours
void CGridCtrl::OnSysColorChange()
{
    CWnd::OnSysColorChange();

    if (GetDefaultCell(FALSE, FALSE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, FALSE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(FALSE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, FALSE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, FALSE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(TRUE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(FALSE, TRUE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, TRUE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(FALSE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, TRUE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, TRUE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(TRUE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetGridBkColor() == m_crShadow)
        SetGridBkColor(::GetSysColor(COLOR_3DSHADOW));

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);
}
#endif

#ifndef _WIN32_WCE_NO_CURSOR
// If we are drag-selecting cells, or drag and dropping, stop now
void CGridCtrl::OnCaptureChanged(CWnd *pWnd)
{
    if (pWnd->GetSafeHwnd() == GetSafeHwnd())
        return;

    // kill timer if active
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

#ifndef GRIDCONTROL_NO_DRAGDROP
    // Kill drag and drop if active
    if (m_MouseMode == MOUSE_DRAGGING)
        m_MouseMode = MOUSE_NOTHING;
#endif
}
#endif

#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
// If system settings change, then redo colours
void CGridCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CWnd::OnSettingChange(uFlags, lpszSection);

    if (GetDefaultCell(FALSE, FALSE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, FALSE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(FALSE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, FALSE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, FALSE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, FALSE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(TRUE, FALSE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(FALSE, TRUE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(FALSE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(FALSE, TRUE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(FALSE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetDefaultCell(TRUE, TRUE)->GetTextClr() == m_crWindowText)                   // Still using system colours
        GetDefaultCell(TRUE, TRUE)->SetTextClr(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetDefaultCell(TRUE, TRUE)->GetBackClr() == m_crWindowColour)
        GetDefaultCell(TRUE, TRUE)->SetBackClr(::GetSysColor(COLOR_WINDOW));

    if (GetGridBkColor() == m_crShadow)
        SetGridBkColor(::GetSysColor(COLOR_3DSHADOW));

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);

    m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
}
#endif

// For drag-selection. Scrolls hidden cells into view
// TODO: decrease timer interval over time to speed up selection over time
void CGridCtrl::OnTimer(UINT_PTR nIDEvent)
{
    ASSERT(nIDEvent == WM_LBUTTONDOWN);
    if (nIDEvent != WM_LBUTTONDOWN)
        return;

    CPoint pt, origPt;

#ifdef _WIN32_WCE
    if (m_MouseMode == MOUSE_NOTHING)
        return;
    origPt = GetMessagePos();
#else
    if (!GetCursorPos(&origPt))
        return;
#endif

    ScreenToClient(&origPt);

    CRect rect;
    GetClientRect(rect);

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth = GetFixedColumnWidth();

    pt = origPt;
    if (pt.y > rect.bottom)
    {
        //SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        SendMessage(WM_KEYDOWN, VK_DOWN, 0);

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = rect.bottom;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.y < nFixedRowHeight)
    {
        //SendMessage(WM_VSCROLL, SB_LINEUP, 0);
        SendMessage(WM_KEYDOWN, VK_UP, 0);

        if (pt.x < rect.left)
            pt.x = rect.left;
        if (pt.x > rect.right)
            pt.x = rect.right;
        pt.y = nFixedRowHeight + 1;
        OnSelecting(GetCellFromPt(pt));
    }

    pt = origPt;
    if (pt.x > rect.right)
    {
        // SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = rect.right;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.x < nFixedColWidth)
    {
        //SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        SendMessage(WM_KEYDOWN, VK_LEFT, 0);

        if (pt.y < rect.top)
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = nFixedColWidth + 1;
        OnSelecting(GetCellFromPt(pt));
    }
}

// move about with keyboard
void CGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!IsValid(m_idCurrentCell))
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }
    CCellID next = m_idCurrentCell;
    BOOL bChangeLine = FALSE;
	BOOL bHorzScrollAction = FALSE;
	BOOL bVertScrollAction = FALSE;

    if (IsCTRLpressed())
    {
        switch (nChar)
        {
        case 'A':
            OnEditSelectAll();
            break;
        case 'k':		// This is ctrl+ on french keyboard, may need to be better processed for other locales
            AutoSizeColumns();
			Invalidate();
            break;
#ifndef GRIDCONTROL_NO_CLIPBOARD
        case 'X':
            OnEditCut();
            break;
        case VK_INSERT:
        case 'C':
            OnEditCopy();
            break;
        case 'V':
            OnEditPaste();
            break;
#endif
        }
    }

#ifndef GRIDCONTROL_NO_CLIPBOARD
    if (IsSHIFTpressed() &&(nChar == VK_INSERT))
        OnEditPaste();
#endif

    BOOL bFoundVisible;
    int iOrig;

    if (nChar == VK_DELETE)
    {
		CutSelectedText();
    }
    else if (nChar == VK_DOWN)
    {
        // don't let user go to a hidden row
        bFoundVisible = FALSE;
        iOrig = next.row;
        next.row++;
        while( next.row < GetRowCount())
        {
            if( GetRowHeight( next.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.row++;
        }
        if( !bFoundVisible)
            next.row = iOrig;
    }
    else if (nChar == VK_UP)
    {
        // don't let user go to a hidden row
        bFoundVisible = FALSE;
        iOrig = next.row;
        next.row--;
        while( next.row >= m_nFixedRows)
        {
            if( GetRowHeight( next.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.row--;
        }
        if( !bFoundVisible)
            next.row = iOrig;
    }
    else if (nChar == VK_RIGHT || (nChar == VK_TAB && !IsSHIFTpressed()) )
    {
		if( (nChar == VK_TAB) &&  m_QuitFocusOnTab )
		{
			CDialog* p= (CDialog*) GetParent();
			if(p) p->NextDlgCtrl();
			return;

		}
        // don't let user go to a hidden column
        bFoundVisible = FALSE;
        iOrig = next.col;
        next.col++;

        if (nChar == VK_TAB)
        {
			// If we're at the end of a row, go down a row till we find a non-hidden row
            if (next.col == (GetColumnCount()) && next.row < (GetRowCount() - 1))
            {
				next.row++;
				while( next.row < GetRowCount())
				{
					if( GetRowHeight(next.row) > 0)
					{
						bFoundVisible = TRUE;
						break;
					}
					next.row++; 
				}

                next.col = m_nFixedCols;	// Place focus on first non-fixed column
                bChangeLine = TRUE;
            }
            else
                CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        }

		// We're on a non-hidden row, so look across for the next non-hidden column
        while( next.col < GetColumnCount())
        {
            if( GetColumnWidth( next.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.col++;
        }

		// If nothing worked then don't bother
        if( !bFoundVisible)
            next.col = iOrig;
    }
    else if (nChar == VK_LEFT || (nChar == VK_TAB && IsSHIFTpressed()) )
    {
        // don't let user go to a hidden column
        bFoundVisible = FALSE;
        iOrig = next.col;
        next.col--;

        if (nChar == VK_TAB)
        {
            if (next.col == (GetFixedColumnCount()-1) && next.row > GetFixedRowCount())
            {
                next.row--;
				while( next.row > GetFixedRowCount())
				{
					if( GetRowHeight(next.row) > 0)
					{
						bFoundVisible = TRUE;
						break;
					}
					next.row--; 
				}

                next.col = GetColumnCount() - 1; 
                bChangeLine = TRUE;
            }
            else
                CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        }

        while( next.col >= m_nFixedCols)
        {
            if( GetColumnWidth( next.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            next.col--;
        }
        if( !bFoundVisible)
            next.col = iOrig;
    }
    else if (nChar == VK_NEXT)
    {
        CCellID idOldTopLeft = GetTopleftNonFixedCell();
        SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
		bVertScrollAction = TRUE;
        CCellID idNewTopLeft = GetTopleftNonFixedCell();

        int increment = idNewTopLeft.row - idOldTopLeft.row;
        if (increment)
        {
            next.row += increment;
            if (next.row >(GetRowCount() - 1))
                next.row = GetRowCount() - 1;
        }
        else
            next.row = GetRowCount() - 1;
    }
    else if (nChar == VK_PRIOR)
    {
        CCellID idOldTopLeft = GetTopleftNonFixedCell();
        SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
		bVertScrollAction = TRUE;
        CCellID idNewTopLeft = GetTopleftNonFixedCell();
            
        int increment = idNewTopLeft.row - idOldTopLeft.row;
        if (increment) 
        {
            next.row += increment;
            if (next.row < m_nFixedRows) 
                next.row = m_nFixedRows;
        }
        else
            next.row = m_nFixedRows;
    }
    else if (nChar == VK_HOME)
    {
        // Home and Ctrl-Home work more like Excel
        //  and don't let user go to a hidden cell
        if (IsCTRLpressed())
        {
            SendMessage(WM_VSCROLL, SB_TOP, 0);
            SendMessage(WM_HSCROLL, SB_LEFT, 0);
			bVertScrollAction = TRUE;
			bHorzScrollAction = TRUE;
            next.row = m_nFixedRows;
            next.col = m_nFixedCols;
        }
        else
        {
            SendMessage(WM_HSCROLL, SB_LEFT, 0);
 			bHorzScrollAction = TRUE;
            next.col = m_nFixedCols;
        }
        // adjust column to avoid hidden columns and rows
        while( next.col < GetColumnCount() - 1)
        {
            if( GetColumnWidth( next.col) > 0)
                break;
            next.col++;
        }
        while( next.row < GetRowCount() - 1)
        {
            if( GetRowHeight( next.row) > 0)
                break;
            next.row++;
        }
    }
    else if (nChar == VK_END)
    {
        // End and Ctrl-End work more like Excel
        //  and don't let user go to a hidden cell
        if (IsCTRLpressed())
        {
            SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
            SendMessage(WM_HSCROLL, SB_RIGHT, 0);
			bHorzScrollAction = TRUE;
			bVertScrollAction = TRUE;
            next.row = GetRowCount() - 1;
            next.col = GetColumnCount() - 1;
        }
        else
        {
            SendMessage(WM_HSCROLL, SB_RIGHT, 0);
			bHorzScrollAction = TRUE;
            next.col = GetColumnCount() - 1;
        }
        // adjust column to avoid hidden columns and rows
        while( next.col > m_nFixedCols + 1)
        {
            if( GetColumnWidth( next.col) > 0)
                break;
            next.col--;
        }
        while( next.row > m_nFixedRows + 1)
        {
            if( GetRowHeight( next.row) > 0)
                break;
            next.row--;
        }
    }
    else if (nChar == VK_F2)
    {
        OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, CPoint( -1, -1), VK_LBUTTON);
    }
    else
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    if (next != m_idCurrentCell)
    {
        // While moving with the Cursorkeys the current ROW/CELL will get selected
        // OR Selection will get expanded when SHIFT is pressed
        // Cut n paste from OnLButtonDown - Franco Bez
        // Added check for NULL mouse mode - Chris Maunder.
        if (m_MouseMode == MOUSE_NOTHING)
        {
            m_PrevSelectedCellMap.RemoveAll();
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            if (!IsSHIFTpressed() || nChar == VK_TAB)
                m_SelectionStartCell = next;

			// Notify parent that selection is changing - Arthur Westerman/Scot Brennecke 
            SendMessageToParent(next.row, next.col, GVN_SELCHANGING);
			OnSelecting(next);
			SendMessageToParent(next.row, next.col, GVN_SELCHANGED);

            m_MouseMode = MOUSE_NOTHING;
        }

        SetFocusCell(next);

        if (!IsCellVisible(next))
        {

            switch (nChar)
            {
            case VK_RIGHT:  
                SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
				bHorzScrollAction = TRUE;
                break;
                
            case VK_LEFT:   
                SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
				bHorzScrollAction = TRUE;
                break;
                
            case VK_DOWN:   
                SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
				bVertScrollAction = TRUE;
                break;
                
            case VK_UP:     
                SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
				bVertScrollAction = TRUE;
                break;                
                
            case VK_TAB:    
                if (IsSHIFTpressed())
                {
                    if (bChangeLine) 
                    {
                        SendMessage(WM_VSCROLL, SB_LINEUP, 0);
						bVertScrollAction = TRUE;
                        SetScrollPos32(SB_HORZ, m_nHScrollMax);
                        break;
                    }
                    else 
					{
                        SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
						bHorzScrollAction = TRUE;
					}
                }
                else
                {
                    if (bChangeLine) 
                    {
                        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
						bVertScrollAction = TRUE;
                        SetScrollPos32(SB_HORZ, 0);
                        break;
                    }
                    else 
					{
						SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
						bHorzScrollAction = TRUE;
					}
                }
                break;
            }
            EnsureVisible(next); // Make sure cell is visible
            Invalidate();
        }
        EnsureVisible(next); // Make sure cell is visible

		if (bHorzScrollAction)
			SendMessage(WM_HSCROLL, SB_ENDSCROLL, 0);
		if (bVertScrollAction)
			SendMessage(WM_VSCROLL, SB_ENDSCROLL, 0);
    }
}

void CGridCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGridCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef GRIDCONTROL_USE_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif

    CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

// Instant editing of cells when keys are pressed
void CGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // EFW - BUG FIX
    if (!IsCTRLpressed() && m_MouseMode == MOUSE_NOTHING && nChar != VK_ESCAPE)
    {
        if (!m_bHandleTabKey || (m_bHandleTabKey && nChar != VK_TAB))
            OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, CPoint( -1, -1), nChar);
    }

    CWnd::OnChar(nChar, nRepCnt, nFlags);
}

// Added by KiteFly
LRESULT CGridCtrl::OnImeChar(WPARAM wCharCode, LPARAM)
{
    // EFW - BUG FIX
    if (!IsCTRLpressed() && m_MouseMode == MOUSE_NOTHING && wCharCode != VK_ESCAPE) 
        OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, CPoint( -1, -1), (UINT)wCharCode);
    return 0;
}

// Callback from any CInPlaceEdits that ended. This just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void CGridCtrl::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;

    // In case OnEndInPlaceEdit called as window is being destroyed
    if (!IsWindow(GetSafeHwnd()))
        return;

    OnEndEditCell(pgvItem->row, pgvItem->col, pgvItem->strText);
    //InvalidateCellRect(CCellID(pgvItem->row, pgvItem->col));

    switch (pgvItem->lParam)
    {
    case VK_TAB:
    case VK_DOWN:
    case VK_UP:
    case VK_RIGHT:
    case VK_LEFT:
    case VK_NEXT:
    case VK_PRIOR:
    case VK_HOME:
    case VK_END:
        OnKeyDown((UINT)pgvItem->lParam, 0, 0);
        OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, CPoint( -1, -1), (UINT)pgvItem->lParam);
    }

    *pResult = 0;
}

// Handle horz scrollbar notifications
void CGridCtrl::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    EndEditing();

#ifndef GRIDCONTROL_NO_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif

    int scrollPos = GetScrollPos32(SB_HORZ);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
    case SB_LINERIGHT:
        if (scrollPos < m_nHScrollMax)
        {
            // may have contiguous hidden columns.  Blow by them
            while (idTopLeft.col < (GetColumnCount()-1)
                    && GetColumnWidth( idTopLeft.col) < 1 )
            {
                idTopLeft.col++;
            }
            int xScroll = GetColumnWidth(idTopLeft.col);
            SetScrollPos32(SB_HORZ, scrollPos + xScroll);
            if (GetScrollPos32(SB_HORZ) == scrollPos)
                break;          // didn't work

            rect.left = GetFixedColumnWidth();
            //rect.left = GetFixedColumnWidth() + xScroll;
            //ScrollWindow(-xScroll, 0, rect);
            //rect.left = rect.right - xScroll;
            InvalidateRect(rect);
        }
        break;

    case SB_LINELEFT:
        if (scrollPos > 0 && idTopLeft.col > GetFixedColumnCount())
        {
            int iColToUse = idTopLeft.col-1;
            // may have contiguous hidden columns.  Blow by them
            while(  iColToUse > GetFixedColumnCount()
                    && GetColumnWidth( iColToUse) < 1 )
            {
                iColToUse--;
            }

            int xScroll = GetColumnWidth(iColToUse);
            SetScrollPos32(SB_HORZ, __max(0, scrollPos - xScroll));
            rect.left = GetFixedColumnWidth();
            //ScrollWindow(xScroll, 0, rect);
            //rect.right = rect.left + xScroll;
            InvalidateRect(rect);
        }
        break;

    case SB_PAGERIGHT:
        if (scrollPos < m_nHScrollMax)
        {
            rect.left = GetFixedColumnWidth();
            int offset = rect.Width();
            int pos = min(m_nHScrollMax, scrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            InvalidateRect(rect);
        }
        break;
        
    case SB_PAGELEFT:
        if (scrollPos > 0)
        {
            rect.left = GetFixedColumnWidth();
            int offset = -rect.Width();
            int pos = __max(0, scrollPos + offset);
            SetScrollPos32(SB_HORZ, pos);
            rect.left = GetFixedColumnWidth();
            InvalidateRect(rect);
        }
        break;
        
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
            m_idTopLeftCell.row = -1;
            CCellID idNewTopLeft = GetTopleftNonFixedCell();
            if (idNewTopLeft != idTopLeft)
            {
                rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
            }
        }
        break;
        
    case SB_LEFT:
        if (scrollPos > 0)
        {
            SetScrollPos32(SB_HORZ, 0);
            Invalidate();
        }
        break;
        
    case SB_RIGHT:
        if (scrollPos < m_nHScrollMax)
        {
            SetScrollPos32(SB_HORZ, m_nHScrollMax);
            Invalidate();
        }
        break;
        
        
    default: 
        break;
    }
}

// Handle vert scrollbar notifications
void CGridCtrl::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    EndEditing();

#ifndef GRIDCONTROL_NO_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif

    // Get the scroll position ourselves to ensure we get a 32 bit value
    int scrollPos = GetScrollPos32(SB_VERT);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
    case SB_LINEDOWN:
        if (scrollPos < m_nVScrollMax)
        {
            // may have contiguous hidden rows.  Blow by them
            while(  idTopLeft.row < (GetRowCount()-1)
                    && GetRowHeight( idTopLeft.row) < 1 )
            {
                idTopLeft.row++;
            }

            int yScroll = GetRowHeight(idTopLeft.row);
            SetScrollPos32(SB_VERT, scrollPos + yScroll);
            if (GetScrollPos32(SB_VERT) == scrollPos)
                break;          // didn't work

            rect.top = GetFixedRowHeight();
            //rect.top = GetFixedRowHeight() + yScroll;
            //ScrollWindow(0, -yScroll, rect);
            //rect.top = rect.bottom - yScroll;
            InvalidateRect(rect);
        }
        break;
        
    case SB_LINEUP:
        if (scrollPos > 0 && idTopLeft.row > GetFixedRowCount())
        {
            int iRowToUse = idTopLeft.row-1;
            // may have contiguous hidden rows.  Blow by them
            while(  iRowToUse > GetFixedRowCount()
                    && GetRowHeight( iRowToUse) < 1 )
            {
                iRowToUse--;
            }

            int yScroll = GetRowHeight( iRowToUse);
            SetScrollPos32(SB_VERT, __max(0, scrollPos - yScroll));
            rect.top = GetFixedRowHeight();
            //ScrollWindow(0, yScroll, rect);
            //rect.bottom = rect.top + yScroll;
            InvalidateRect(rect);
        }
        break;
        
    case SB_PAGEDOWN:
        if (scrollPos < m_nVScrollMax)
        {
            rect.top = GetFixedRowHeight();
            scrollPos = min(m_nVScrollMax, scrollPos + rect.Height());
            SetScrollPos32(SB_VERT, scrollPos);
            rect.top = GetFixedRowHeight();
            InvalidateRect(rect);
        }
        break;
        
    case SB_PAGEUP:
        if (scrollPos > 0)
        {
            rect.top = GetFixedRowHeight();
            int offset = -rect.Height();
            int pos = __max(0, scrollPos + offset);
            SetScrollPos32(SB_VERT, pos);
            rect.top = GetFixedRowHeight();
            InvalidateRect(rect);
        }
        break;
        
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        {
            SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
            m_idTopLeftCell.row = -1;
            CCellID idNewTopLeft = GetTopleftNonFixedCell();
            if (idNewTopLeft != idTopLeft)
            {
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
        }
        break;
        
    case SB_TOP:
        if (scrollPos > 0)
        {
            SetScrollPos32(SB_VERT, 0);
            Invalidate();
        }
        break;
        
    case SB_BOTTOM:
        if (scrollPos < m_nVScrollMax)
        {
            SetScrollPos32(SB_VERT, m_nVScrollMax);
            Invalidate();
        }
        
    default: 
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl implementation functions

void CGridCtrl::OnDraw(CDC* pDC)
{
	if (!m_bAllowDraw)
		return;

	CRect clipRect;
	if (pDC->GetClipBox(&clipRect) == ERROR)
		return;

	EraseBkgnd(pDC);            // OnEraseBkgnd does nothing, so erase bkgnd here.
	// This necessary since we may be using a Memory DC.

#ifdef _DEBUG
	LARGE_INTEGER iStartCount;
	QueryPerformanceCounter(&iStartCount);
#endif

	CRect rect;
	int row, col;
	CGridCellBase* pCell;

	int nFixedRowHeight = GetFixedRowHeight();
	int nFixedColWidth  = GetFixedColumnWidth();

	CCellID idTopLeft = GetTopleftNonFixedCell();
	int minVisibleRow = idTopLeft.row,
		minVisibleCol = idTopLeft.col;

	CRect VisRect;
	CCellRange VisCellRange = GetVisibleNonFixedCellRange(VisRect);
	int maxVisibleRow = VisCellRange.GetMaxRow(),
		maxVisibleCol = VisCellRange.GetMaxCol();

	if (GetVirtualMode())
		SendCacheHintToParent(VisCellRange);

	CPen pen;
	pen.CreatePen(PS_SOLID, 0, m_crGridLineColour);
	pDC->SelectObject(&pen);

	// draw vertical lines (drawn at ends of cells) 竖线
	if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT)
	{
		int x = nFixedColWidth;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (GetColumnWidth(col) <= 0) continue;

			x += GetColumnWidth(col);
			pDC->MoveTo(x-1, nFixedRowHeight);
			pDC->LineTo(x-1, VisRect.bottom);
		}
	}
	
	// draw horizontal lines (drawn at bottom of each cell) 横线
	if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
	{
		int y = nFixedRowHeight;
		for (row = minVisibleRow; row <= maxVisibleRow; row++)
		{
			if (GetRowHeight(row) <= 0) continue;

			y += GetRowHeight(row);
			pDC->MoveTo(nFixedColWidth, y-1);
			pDC->LineTo(VisRect.right,  y-1);
		}
	}
	
	pDC->SelectStockObject(NULL_PEN);

	// draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1  最左上角的格子
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		if (GetRowHeight(row) <= 0) continue;

		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;
		rect.right = -1;

		for (col = 0; col < m_nFixedCols; col++)
		{
			if (GetColumnWidth(col) <= 0) continue;

			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;

			pCell = GetCell(row, col);
			if (pCell)
			{
				//Used for merge cells by Huang Wei
				//bugfix by Luther Bruck

				if(!pCell->IsMerged())
				{
					if(!pCell->IsMergeWithOthers())
					{
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, rect, FALSE);
					}
					else
					{
						CGridCellBase* pMergedCell=GetCell(pCell->GetMergeCellID());
						CRect mergerect=rect;
						if(GetCellRangeRect(pMergedCell->m_MergeRange,&mergerect))
						{
							mergerect.DeflateRect(0,0,1,1);
							pMergedCell->SetCoords(pCell->GetMergeCellID().row,pCell->GetMergeCellID().col);
							pMergedCell->Draw(pDC, pCell->GetMergeCellID().row,pCell->GetMergeCellID().col, mergerect, TRUE);
						}
					}
				}
				else
				{
					CRect mergerect=rect;

					if(GetCellRangeRect(pCell->m_MergeRange,&mergerect))
					{
						mergerect.DeflateRect(0,0,1,1);
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, mergerect, TRUE);
					}
				}
			}
		}
	}

	// draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1 固定列
	rect.bottom = nFixedRowHeight-1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		if (GetRowHeight(row) <= 0) continue;

		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;

		// rect.bottom = bottom pixel of previous row
		if (rect.top > clipRect.bottom)
			break;                // Gone past cliprect
		if (rect.bottom < clipRect.top)
			continue;             // Reached cliprect yet?

		rect.right = -1;
		for (col = 0; col < m_nFixedCols; col++)
		{
			if (GetColumnWidth(col) <= 0) continue;

			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;

			if (rect.left > clipRect.right)
				break;            // gone past cliprect
			if (rect.right < clipRect.left)
				continue;         // Reached cliprect yet?

			pCell = GetCell(row, col);
			if (pCell)
			{
				//Used for merge cells
				//by Huang Wei

				if(!pCell->IsMerged())
				{
					if(!pCell->IsMergeWithOthers())
					{
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, rect, FALSE);
					}
					else
					{
						CGridCellBase* pMergedCell=GetCell(pCell->GetMergeCellID());
						CRect mergerect=rect;
						if(GetCellRangeRect(pMergedCell->m_MergeRange,&mergerect))
						{
							mergerect.DeflateRect(0,0,1,1);
							pMergedCell->SetCoords(pCell->GetMergeCellID().row,pCell->GetMergeCellID().col);
							pMergedCell->Draw(pDC, pCell->GetMergeCellID().row,pCell->GetMergeCellID().col, mergerect, TRUE);
						}
					}
				}
				else
				{
					CRect mergerect=rect;

					if(GetCellRangeRect(pCell->m_MergeRange,&mergerect))
					{
						mergerect.DeflateRect(0,0,1,1);
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, mergerect, TRUE);
					}
				}
			}
		}
	}

	// draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n   固定横
	rect.bottom = -1;
	for (row = 0; row < m_nFixedRows; row++)
	{
		if (GetRowHeight(row) <= 0) continue;

		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;

		// rect.bottom = bottom pixel of previous row
		if (rect.top > clipRect.bottom)
			break;                // Gone past cliprect
		if (rect.bottom < clipRect.top)
			continue;             // Reached cliprect yet?

		rect.right = nFixedColWidth-1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (GetColumnWidth(col) <= 0) continue;

			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;

			if (rect.left > clipRect.right)
				break;        // gone past cliprect
			if (rect.right < clipRect.left)
				continue;     // Reached cliprect yet?

			pCell = GetCell(row, col);
			if (pCell)
			{
				//Used for merge cells
				//by Huang Wei

				if(!pCell->IsMerged())
				{
					if(!pCell->IsMergeWithOthers())
					{
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, rect, FALSE);
					}
					else
					{
						CGridCellBase* pMergedCell=GetCell(pCell->GetMergeCellID());
						CRect mergerect=rect;
						if(GetCellRangeRect(pMergedCell->m_MergeRange,&mergerect))
						{
							mergerect.DeflateRect(0,0,1,1);
							pMergedCell->SetCoords(pCell->GetMergeCellID().row,pCell->GetMergeCellID().col);
							pMergedCell->Draw(pDC, pCell->GetMergeCellID().row,pCell->GetMergeCellID().col, mergerect, TRUE);
						}
					}
				}
				else
				{
					CRect mergerect=rect;

					if(GetCellRangeRect(pCell->m_MergeRange,&mergerect))
					{
						mergerect.DeflateRect(0,0,1,1);
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, mergerect, TRUE);
					}
				}
			}
		}
	}
	
	// draw rest of non-fixed cells 普通的格子
	rect.bottom = nFixedRowHeight-1;
	for (row = minVisibleRow; row <= maxVisibleRow; row++)
	{
		if (GetRowHeight(row) <= 0) continue;

		rect.top = rect.bottom+1;
		rect.bottom = rect.top + GetRowHeight(row)-1;

		// rect.bottom = bottom pixel of previous row
		if (rect.top > clipRect.bottom)
			break;                // Gone past cliprect
		if (rect.bottom < clipRect.top)
			continue;             // Reached cliprect yet?

		rect.right = nFixedColWidth-1;
		for (col = minVisibleCol; col <= maxVisibleCol; col++)
		{
			if (GetColumnWidth(col) <= 0) continue;

			rect.left = rect.right+1;
			rect.right = rect.left + GetColumnWidth(col)-1;

			if (rect.left > clipRect.right)
				break;        // gone past cliprect
			if (rect.right < clipRect.left)
				continue;     // Reached cliprect yet?

			pCell = GetCell(row, col);
			// TRACE(_T("Cell %d,%d type: %s\n"), row, col, pCell->GetRuntimeClass()->m_lpszClassName);
			if (pCell)
			{
				//Used for merge cells
				//by Huang Wei

				if(!pCell->IsMerged())
				{
					if(!pCell->IsMergeWithOthers())
					{
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, rect, FALSE);
					}
					else
					{
						CGridCellBase* pMergedCell=GetCell(pCell->GetMergeCellID());
						CRect mergerect=rect;
						if(GetCellRangeRect(pMergedCell->m_MergeRange,&mergerect))
						{
							mergerect.DeflateRect(0,0,1,1);
							pMergedCell->SetCoords(pCell->GetMergeCellID().row,pCell->GetMergeCellID().col);
							pMergedCell->Draw(pDC, pCell->GetMergeCellID().row,pCell->GetMergeCellID().col, mergerect, TRUE);
						}
					}
				}
				else
				{
					CRect mergerect=rect;

					if(GetCellRangeRect(pCell->m_MergeRange,&mergerect))
					{
						mergerect.DeflateRect(0,0,1,1);
						pCell->SetCoords(row,col);
						pCell->Draw(pDC, row, col, mergerect, TRUE);
					}
				}
			}
		}
	}
	



	// Let parent know it can discard it's data if it needs to.
	if (GetVirtualMode())
		SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

#ifdef _DEBUG
	LARGE_INTEGER iEndCount;
	QueryPerformanceCounter(&iEndCount);
	TRACE1("Draw counter ticks: %d\n", iEndCount.LowPart-iStartCount.LowPart);
#endif

}

////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrl Cell selection stuff

// Is a given cell designation valid (ie within the bounds of our number
// of columns/rows)?
BOOL CGridCtrl::IsValid(int nRow, int nCol) const
{
    return (nRow >= 0 && nRow < m_nRows && nCol >= 0 && nCol < m_nCols);
}

BOOL CGridCtrl::IsValid(const CCellID& cell) const
{
    return IsValid(cell.row, cell.col);
}

// Is a given cell range valid (ie within the bounds of our number
// of columns/rows)?
BOOL CGridCtrl::IsValid(const CCellRange& range) const
{
    return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 &&
        range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
        range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nCols &&
        range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CGridCtrl::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
//    TRACE(_T("%s: Setting redraw to %s\n"),
//             GetRuntimeClass()->m_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));

    if (bAllowDraw && !m_bAllowDraw)
    {
        m_bAllowDraw = TRUE;
        Refresh();
    }

    m_bAllowDraw = bAllowDraw;
    if (bResetScrollBars)
        ResetScrollBars();
}

// Forces a redraw of a cell immediately (using a direct DC construction,
// or the supplied dc)
BOOL CGridCtrl::RedrawCell(const CCellID& cell, CDC* pDC /* = NULL */)
{
    return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CGridCtrl::RedrawCell(int nRow, int nCol, CDC* pDC /* = NULL */)
{
    BOOL bResult = TRUE;
    BOOL bMustReleaseDC = FALSE;

    if (!m_bAllowDraw || !IsCellVisible(nRow, nCol))
        return FALSE;

    CRect rect;
    if (!GetCellRect(nRow, nCol, rect))
        return FALSE;

    if (!pDC)
    {
        pDC = GetDC();
        if (pDC)
            bMustReleaseDC = TRUE;
    }

    if (pDC)
    {
        // Redraw cells directly
        if (nRow < m_nFixedRows || nCol < m_nFixedCols)
        {
            CGridCellBase* pCell = GetCell(nRow, nCol);
            if (pCell)
                bResult = pCell->Draw(pDC, nRow, nCol, rect, TRUE);
        }
        else
        {
            CGridCellBase* pCell = GetCell(nRow, nCol);
            if (pCell)
                bResult = pCell->Draw(pDC, nRow, nCol, rect, TRUE);

            // Since we have erased the background, we will need to redraw the gridlines
            CPen pen;
            pen.CreatePen(PS_SOLID, 0, m_crGridLineColour);

            CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
            {
                pDC->MoveTo(rect.left,    rect.bottom);
                pDC->LineTo(rect.right + 1, rect.bottom);
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT)
            {
                pDC->MoveTo(rect.right, rect.top);
                pDC->LineTo(rect.right, rect.bottom + 1);
            }
            pDC->SelectObject(pOldPen);
        }
    } else
        InvalidateRect(rect, TRUE);     // Could not get a DC - invalidate it anyway
    // and hope that OnPaint manages to get one

    if (bMustReleaseDC)
        ReleaseDC(pDC);

    return bResult;
}

// redraw a complete row
BOOL CGridCtrl::RedrawRow(int row)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int col = 0; col < GetColumnCount(); col++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC)
        ReleaseDC(pDC);

    return bResult;
}

// redraw a complete column
BOOL CGridCtrl::RedrawColumn(int col)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int row = 0; row < GetRowCount(); row++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC)
        ReleaseDC(pDC);

    return bResult;
}


// Sets the currently selected cell, returning the previous current cell
CCellID CGridCtrl::SetFocusCell(int nRow, int nCol)
{
    return SetFocusCell(CCellID(nRow, nCol));
}

CCellID CGridCtrl::SetFocusCell(CCellID cell)
{
    if (cell == m_idCurrentCell)
        return m_idCurrentCell;

    CCellID idPrev = m_idCurrentCell;

    // EFW - Bug Fix - Force focus to be in a non-fixed cell
    if (cell.row != -1 && cell.row < GetFixedRowCount())
        cell.row = GetFixedRowCount();
    if (cell.col != -1 && cell.col < GetFixedColumnCount())
        cell.col = GetFixedColumnCount();

    m_idCurrentCell = cell;

    if (IsValid(idPrev))
    {
        SetItemState(idPrev.row, idPrev.col,
            GetItemState(idPrev.row, idPrev.col) & ~GVIS_FOCUSED);
        RedrawCell(idPrev); // comment to reduce flicker

        if (GetTrackFocusCell() && idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++)
                RedrawCell(row, idPrev.col);
        if (GetTrackFocusCell() && idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++)
                RedrawCell(idPrev.row, col);
    }

    if (IsValid(m_idCurrentCell))
    {
        SetItemState(m_idCurrentCell.row, m_idCurrentCell.col,
            GetItemState(m_idCurrentCell.row, m_idCurrentCell.col) | GVIS_FOCUSED);

        RedrawCell(m_idCurrentCell); // comment to reduce flicker

        if (GetTrackFocusCell() && idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++)
                RedrawCell(row, m_idCurrentCell.col);
        if (GetTrackFocusCell() && idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++)
                RedrawCell(m_idCurrentCell.row, col);

        // EFW - New addition.  If in list mode, make sure the selected
        // row highlight follows the cursor.
        // Removed by C Maunder 27 May
        //if (m_bListMode)
        //{
        //    m_PrevSelectedCellMap.RemoveAll();
        //    m_MouseMode = MOUSE_SELECT_ROW;
        //    OnSelecting(m_idCurrentCell);

            // Leave this off so that you can still drag the highlight around
            // without selecting rows.
            // m_MouseMode = MOUSE_NOTHING;
        //}

	}

    return idPrev;
}

// Sets the range of currently selected cells
void CGridCtrl::SetSelectedRange(const CCellRange& Range,
                                 BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
    SetSelectedRange(Range.GetMinRow(), Range.GetMinCol(),
                     Range.GetMaxRow(), Range.GetMaxCol(),
                     bForceRepaint, bSelectCells);
}

void CGridCtrl::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                                 BOOL bForceRepaint /* = FALSE */, BOOL bSelectCells/*=TRUE*/)
{
    if (!m_bEnableSelection)
        return;

	//CWaitCursor wait; // Thomas Haase 

    CDC* pDC = NULL;
    if (bForceRepaint)
        pDC = GetDC();

	// Only redraw visible cells
    CCellRange VisCellRange, FixedVisCellRange;
	if (IsWindow(GetSafeHwnd()))

	{
		VisCellRange = GetVisibleNonFixedCellRange();
		FixedVisCellRange = GetVisibleFixedCellRange();
	}
   
    // EFW - Bug fix - Don't allow selection of fixed rows
	int Left= (m_AllowSelectRowInFixedCol ? 0 : GetFixedColumnCount());

    if(nMinRow >= 0 && nMinRow < GetFixedRowCount())
        nMinRow = GetFixedRowCount();
    if(nMaxRow >= 0 && nMaxRow < GetFixedRowCount())
        nMaxRow = GetFixedRowCount();
    if(nMinCol >= 0 && nMinCol < Left)
        nMinCol = GetFixedColumnCount();
    if(nMaxCol >= 0 && nMaxCol < Left)
        nMaxCol = GetFixedColumnCount();

    // If we are selecting cells, then first clear out the list of currently selected cells, then
    if (bSelectCells)
    {
        POSITION pos;

        // Unselect all previously selected cells
        for (pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
        {
            DWORD key;
            CCellID cell;
            m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

            // Reset the selection flag on the cell
            if (IsValid(cell))
            {
                // This will remove the cell from the m_SelectedCellMap map
                SetItemState(cell.row, cell.col,
                    GetItemState(cell.row, cell.col) & ~GVIS_SELECTED);

                // If this is to be reselected, continue on past the redraw
                if (nMinRow <= cell.row && cell.row <= nMaxRow &&
                    nMinCol <= cell.col && cell.col <= nMaxCol)
                    continue;

				if ( (VisCellRange.IsValid() && VisCellRange.InRange( cell )) || FixedVisCellRange.InRange( cell ) )
				{
					if (bForceRepaint && pDC)                    // Redraw NOW
						RedrawCell(cell.row, cell.col, pDC);
					else
						InvalidateCellRect(cell);                // Redraw at leisure
				}
            }
            else
            {
                m_SelectedCellMap.RemoveKey( key);  // if it's not valid, get rid of it!
            }
        }

        // if we are selecting cells, and there are previous selected cells to be retained 
        // (eg Ctrl is being held down) then copy them to the newly created list, and mark 
        // all these cells as selected
        // Note that if we are list mode, single row selection, then we won't be adding 
        // the previous cells. Only the current row of cells will be added (see below)
        if (!GetSingleRowSelection() &&
            nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0)
        {
            for (pos = m_PrevSelectedCellMap.GetStartPosition(); pos != NULL; /* nothing */)
            {
                DWORD key;
                CCellID cell;
                m_PrevSelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

                if (!IsValid(cell))
                    continue;

                int nState = GetItemState(cell.row, cell.col);

                // Set state as Selected. This will add the cell to m_SelectedCellMap
                SetItemState(cell.row, cell.col, nState | GVIS_SELECTED);

				if (( VisCellRange.IsValid() && VisCellRange.InRange( cell ))  || FixedVisCellRange.InRange( cell )  )
				{
					// Redraw (immediately or at leisure)
					if (bForceRepaint && pDC)
					    RedrawCell(cell.row, cell.col, pDC);
					else
					    InvalidateCellRect(cell);
				}
            }
        }
    }

    // Now select/deselect all cells in the cell range specified. If selecting, and the cell 
    // has already been marked as selected (above) then ignore it. If we are deselecting and
    // the cell isn't selected, then ignore
    if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0 &&
        nMaxRow < m_nRows && nMaxCol < m_nCols &&
        nMinRow <= nMaxRow && nMinCol <= nMaxCol)
    {
        for (int row = nMinRow; row <= nMaxRow; row++)
            for (int col = nMinCol; col <= nMaxCol; col++)
            {
                BOOL bCellSelected = IsCellSelected(row, col);
                if (bSelectCells == bCellSelected)
                    continue;    // Already selected or deselected - ignore

                // Set the selected state. This will add/remove the cell to m_SelectedCellMap
                if (bSelectCells)
                    SetItemState(row, col, GetItemState(row, col) | GVIS_SELECTED);
                else
                    SetItemState(row, col, GetItemState(row, col) & ~GVIS_SELECTED);

				if ( (VisCellRange.IsValid() && VisCellRange.InRange(row, col))  || FixedVisCellRange.InRange(row, col) )
				{
	                // Redraw (immediately or at leisure)
	                if (bForceRepaint && pDC)
	                    RedrawCell(row, col, pDC);
	                else
	                    InvalidateCellRect(row, col);
				}
            }
    }
    //    TRACE(_T("%d cells selected.\n"), m_SelectedCellMap.GetCount());

    if (pDC != NULL)
        ReleaseDC(pDC);
}

// selects all cells
void CGridCtrl::SelectAllCells()
{
    if (!m_bEnableSelection)
        return;

    SetSelectedRange(m_nFixedRows, m_nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

// selects columns
void CGridCtrl::SelectColumns(CCellID currentCell, 
                              BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
    if (!m_bEnableSelection)
        return;

    //if (currentCell.col == m_idCurrentCell.col) return;
    if (currentCell.col < m_nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    if (GetSingleColSelection())
        SetSelectedRange(GetFixedRowCount(), currentCell.col,
                         GetRowCount()-1,    currentCell.col,
                         bForceRedraw, bSelectCells);
    else
        SetSelectedRange(GetFixedRowCount(),
                         min(m_SelectionStartCell.col, currentCell.col),
                         GetRowCount()-1,
                         max(m_SelectionStartCell.col, currentCell.col),
                         bForceRedraw, bSelectCells);
}

// selects rows
void CGridCtrl::SelectRows(CCellID currentCell, 
                           BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
    if (!m_bEnableSelection)
        return;

    //if (currentCell.row; == m_idCurrentCell.row) return;
    if (currentCell.row < m_nFixedRows)
        return;
    if (!IsValid(currentCell))
        return;
	int Left = ( m_AllowSelectRowInFixedCol ? 0 : GetFixedColumnCount());
    if (GetSingleRowSelection())
        SetSelectedRange(currentCell.row, Left ,
                         currentCell.row, GetColumnCount()-1, 
                         bForceRedraw, bSelectCells);
    else
        SetSelectedRange(min(m_SelectionStartCell.row, currentCell.row),
                         Left,
                         __max(m_SelectionStartCell.row, currentCell.row),
                         GetColumnCount()-1,
                         bForceRedraw, bSelectCells);
}

// selects cells
void CGridCtrl::SelectCells(CCellID currentCell, 
                            BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
    if (!m_bEnableSelection)
        return;

    int row = currentCell.row;
    int col = currentCell.col;
    if (row < m_nFixedRows || col < m_nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    // Prevent unnecessary redraws
    //if (currentCell == m_LeftClickDownCell)  return;
    //else if (currentCell == m_idCurrentCell) return;

    SetSelectedRange(min(m_SelectionStartCell.row, row),
                     min(m_SelectionStartCell.col, col),
                     __max(m_SelectionStartCell.row, row),
                     __max(m_SelectionStartCell.col, col),
                     bForceRedraw, bSelectCells);
}

// Called when mouse/keyboard selection is a-happening.
void CGridCtrl::OnSelecting(const CCellID& currentCell)
{
    if (!m_bEnableSelection)
        return;

    switch (m_MouseMode)
    {
    case MOUSE_SELECT_ALL:
        SelectAllCells();
        break;
    case MOUSE_SELECT_COL:
        SelectColumns(currentCell, FALSE);
        break;
    case MOUSE_SELECT_ROW:
        SelectRows(currentCell, FALSE);
        break;
    case MOUSE_SELECT_CELLS:
        SelectCells(currentCell, FALSE);
        break;
    }

    // EFW - Bug fix [REMOVED CJM: this will cause infinite loop in list mode]
    // SetFocusCell(max(currentCell.row, m_nFixedRows), max(currentCell.col, m_nFixedCols));
}

void CGridCtrl::ValidateAndModifyCellContents(int nRow, int nCol, LPCTSTR strText)
{
    if (!IsCellEditable(nRow, nCol))
        return;

    if (SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT) >= 0)
    {
        CString strCurrentText = GetItemText(nRow, nCol);
        if (strCurrentText != strText)
        {
            SetItemText(nRow, nCol, strText);
            if (ValidateEdit(nRow, nCol, strText) && 
                SendMessageToParent(nRow, nCol, GVN_ENDLABELEDIT) >= 0)
            {
                SetModified(TRUE, nRow, nCol);
                RedrawCell(nRow, nCol);
            }
            else
            {
                SetItemText(nRow, nCol, strCurrentText);
            }
        }
    }
}

void CGridCtrl::ClearCells(CCellRange Selection)
{
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            // don't clear hidden cells
            if ( m_arRowHeights[row] > 0 && m_arColWidths[col] > 0 )
            {
                ValidateAndModifyCellContents(row, col, _T(""));
            }
		}
	}
    Refresh();
}

#ifndef GRIDCONTROL_NO_CLIPBOARD

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

// Deletes the contents from the selected cells
void CGridCtrl::CutSelectedText()
{
    if (!IsEditable())
        return;

    for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
		DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
        ValidateAndModifyCellContents(cell.row, cell.col, _T(""));
    }
}

// Copies text from the selected cells to the clipboard
COleDataSource* CGridCtrl::CopyTextFromGrid()
{
    USES_CONVERSION;

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection))
        return NULL;

    if (GetVirtualMode())
        SendCacheHintToParent(Selection);

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    CString str;
    CGridCellBase *pCell;
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        // don't copy hidden cells
        if( m_arRowHeights[row] <= 0 )
            continue;

        str.Empty();
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            // don't copy hidden cells
            if( m_arColWidths[col] <= 0 )
                continue;

            pCell = GetCell(row, col);
            if (pCell &&(pCell->GetState() & GVIS_SELECTED))
            {
                // if (!pCell->GetText())
                //    str += _T(" ");
                // else 
                str += pCell->GetText();
            }
            if (col != Selection.GetMaxCol()) 
                str += _T("\t");
        }

        if (row != Selection.GetMaxRow()) 
            str += _T("\r\n");
        
        sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
        str.ReleaseBuffer();
    }
    
    char c = '\0';
    sf.Write(&c, 1);

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    DWORD dwLen = (DWORD) sf.GetLength();
    HGLOBAL hMem = sf.Detach();
    if (!hMem)
        return NULL;

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
    if (!hMem)
        return NULL;

    // Cache data
    COleDataSource* pSource = new COleDataSource();
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}

// Pastes text from the clipboard to the selected cells
BOOL CGridCtrl::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject, 
								BOOL bSelectPastedCells /*=TRUE*/)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

    // Get the text from the COleDataObject
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), (UINT)::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPTSTR szBuffer = new TCHAR[::GlobalSize(hmem)];
    if (!szBuffer)
        return FALSE;

    sf.Read(szBuffer, (UINT)::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    CString strText = szBuffer;
    delete szBuffer;

    // Parse text data and set in cells...
    strText.LockBuffer();
    CString strLine = strText;
    int nLine = 0;

    // Find the end of the first line
	CCellRange PasteRange(cell.row, cell.col,-1,-1);
    int nIndex;
    do
    {
        int nColumn = 0;
        nIndex = strLine.Find(_T("\n"));

        // Store the remaining chars after the newline
        CString strNext = (nIndex < 0)? _T("")  : strLine.Mid(nIndex + 1);

        // Remove all chars after the newline
        if (nIndex >= 0)
            strLine = strLine.Left(nIndex);

        int nLineIndex = strLine.FindOneOf(_T("\t,"));
        CString strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

        // skip hidden rows
        int iRowVis = cell.row + nLine;
        while( iRowVis < GetRowCount())
        {
            if( GetRowHeight( iRowVis) > 0)
                break;
            nLine++;
            iRowVis++;
        }

        while (!strLine.IsEmpty())
        {
            // skip hidden columns
            int iColVis = cell.col + nColumn;
            while( iColVis < GetColumnCount())
            {
                if( GetColumnWidth( iColVis) > 0)
                    break;
                nColumn++;
                iColVis++;
            }

            CCellID TargetCell(iRowVis, iColVis);
            if (IsValid(TargetCell))
            {
                strCellText.TrimLeft();
                strCellText.TrimRight();

                ValidateAndModifyCellContents(TargetCell.row, TargetCell.col, strCellText);

                // Make sure cell is not selected to avoid data loss
                SetItemState(TargetCell.row, TargetCell.col,
                    GetItemState(TargetCell.row, TargetCell.col) & ~GVIS_SELECTED);

				if (iRowVis > PasteRange.GetMaxRow()) PasteRange.SetMaxRow(iRowVis);
				if (iColVis > PasteRange.GetMaxCol()) PasteRange.SetMaxCol(iColVis);
            }

            strLine = (nLineIndex >= 0)? strLine.Mid(nLineIndex + 1) : _T("");
            nLineIndex = strLine.FindOneOf(_T("\t,"));
            strCellText = (nLineIndex >= 0)? strLine.Left(nLineIndex) : strLine;

            nColumn++;
        }

        strLine = strNext;
        nLine++;
    } while (nIndex >= 0);

    strText.UnlockBuffer();

	if (bSelectPastedCells)
		SetSelectedRange(PasteRange, TRUE);
	else
	{
		ResetSelectedRange();
		Refresh();
	}

    return TRUE;
}
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP

// Start drag n drop
void CGridCtrl::OnBeginDrag()
{
    COleDataSource* pSource = NULL;
    if (!m_bAllowDragAndDrop && m_CurCol==-1)
        return;

    if (m_CurCol>=0)	pSource = new COleDataSource();
    if (!pSource && m_bAllowDragAndDrop) pSource = CopyTextFromGrid();
    if (pSource)
    {
        SendMessageToParent(GetSelectedCellRange().GetTopLeft().row,
            GetSelectedCellRange().GetTopLeft().col,
            GVN_BEGINDRAG);

        m_MouseMode = MOUSE_DRAGGING;
        m_bLMouseButtonDown = FALSE;

        DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);

        if (dropEffect & DROPEFFECT_MOVE)
            CutSelectedText();

        if (pSource)
            delete pSource;    // Did not pass source to clipboard, so must delete

    }
}

// Handle drag over grid
DROPEFFECT CGridCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState,
                                 CPoint point)
{
    // Find which cell we are over and drop-highlight it
    CCellID cell = GetCellFromPt(point, FALSE);
	bool Valid;
    // Any text data available for us?
	if(m_CurCol==-1)
	{
		if(m_bDragRowMode)
		{
			Valid = cell.col>=GetFixedColumnCount() && cell.row>=GetFixedRowCount();
		}
		else
		{
    if (!m_bAllowDragAndDrop || !IsEditable() || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;
			Valid = IsValid(cell)!=0;
		}
	}
	else
	{
		Valid = cell.col>=GetFixedColumnCount() &&   cell.row<GetFixedRowCount() ;
	}


    // If not valid, set the previously drop-highlighted cell as no longer drop-highlighted
    if (!Valid)
    {
        OnDragLeave();
        m_LastDragOverCell = CCellID(-1,-1);
        return DROPEFFECT_NONE;
    }
	if(m_CurCol==-1)
	{
		if (!m_bDragRowMode && !IsCellEditable(cell))
        return DROPEFFECT_NONE;
	}

    // Have we moved over a different cell than last time?
    if (cell != m_LastDragOverCell)
    {
        // Set the previously drop-highlighted cell as no longer drop-highlighted
        if (IsValid(m_LastDragOverCell))
        {
            UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
            SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                nState & ~GVIS_DROPHILITED);
            RedrawCell(m_LastDragOverCell);
        }

        m_LastDragOverCell = cell;

        // Set the new cell as drop-highlighted
        if (IsValid(m_LastDragOverCell))
        {
            UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
            SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                nState | GVIS_DROPHILITED);
            RedrawCell(m_LastDragOverCell);
        }
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

// Something has just been dragged onto the grid
DROPEFFECT CGridCtrl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState,
                                  CPoint point)
{
    // Any text data available for us?
    m_LastDragOverCell = GetCellFromPt(point, m_CurCol>=0);
	bool Valid;
	if(m_CurCol==-1)
	{
    if (!m_bAllowDragAndDrop || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    if (!IsValid(m_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (!IsCellEditable(m_LastDragOverCell))
        return DROPEFFECT_NONE;
		Valid = IsValid(m_LastDragOverCell)!=0;

	}
	else
	{
		Valid = m_LastDragOverCell.row>=0 && m_LastDragOverCell.row<GetFixedRowCount() && m_LastDragOverCell.col>=GetFixedColumnCount();
	}

    if (Valid)
    {
        UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
        SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
            nState | GVIS_DROPHILITED);
        RedrawCell(m_LastDragOverCell);
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

// Something has just been dragged away from the grid
void CGridCtrl::OnDragLeave()
{
    // Set the previously drop-highlighted cell as no longer drop-highlighted
    if (IsValid(m_LastDragOverCell))
    {
        UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
        SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
            nState & ~GVIS_DROPHILITED);
        RedrawCell(m_LastDragOverCell);
    }
}

// Something has just been dropped onto the grid
BOOL CGridCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/,
                       CPoint /* point */)
{
    m_MouseMode = MOUSE_NOTHING;
    if(m_CurCol ==-1)
	{
		if (!m_bAllowDragAndDrop || (!IsCellEditable(m_LastDragOverCell) && !m_bDragRowMode))
        return FALSE;
	}

    OnDragLeave();
    if (m_CurCol>=0)
	{
		if(m_LastDragOverCell.col == m_CurCol || m_LastDragOverCell.row >= GetFixedRowCount()) 
			return FALSE;
		else
		{
			int New = m_arColOrder[m_CurCol];
			m_arColOrder.erase(m_arColOrder.begin()+m_CurCol);
			m_arColOrder.insert(m_arColOrder.begin()+m_LastDragOverCell.col, New);
			m_CurCol=-1;
			Invalidate();
			return TRUE;
		}
	}
	else
	{
		if(m_bDragRowMode)
		{
			Reorder(m_CurRow,m_LastDragOverCell.row);
			Invalidate();
			return TRUE;
		}
		else
    return PasteTextToGrid(m_LastDragOverCell, pDataObject, FALSE);
}
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CGridCtrl::OnEditCut()
{
    if (!IsEditable())
        return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource)
        return;

    pSource->SetClipboard();
    CutSelectedText();
}

void CGridCtrl::OnEditCopy()
{
    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource)
        return;

    pSource->SetClipboard();
}

void CGridCtrl::OnEditPaste()
{
    if (!IsEditable())
        return;

	CCellRange cellRange = GetSelectedCellRange();

	// Get the top-left selected cell, or the Focus cell, or the topleft (non-fixed) cell
	CCellID cell;
	if (cellRange.IsValid())
	{
		cell.row = cellRange.GetMinRow();
		cell.col = cellRange.GetMinCol();
	}
	else
	{
		cell = GetFocusCell();
		if (!IsValid(cell))
			cell = GetTopleftNonFixedCell();
		if (!IsValid(cell))
			return;
	}

	// If a cell is being edited, then call it's edit window paste function.
    if ( IsItemEditing(cell.row, cell.col) )
    {
        CGridCellBase* pCell = GetCell(cell.row, cell.col);
        ASSERT(pCell);
        if (!pCell) return;

		CWnd* pEditWnd = pCell->GetEditWnd();
		if ( pEditWnd && pEditWnd->IsKindOf(RUNTIME_CLASS(CEdit)) )
		{
			((CEdit*)pEditWnd)->Paste();
			return;
		}
    }

    // Attach a COleDataObject to the clipboard and paste the data to the grid
    COleDataObject obj;
    if (obj.AttachClipboard())
        PasteTextToGrid(cell, &obj);
}
#endif

void CGridCtrl::OnEditSelectAll()
{
    SendMessageToParent(m_LeftClickDownCell.row, m_LeftClickDownCell.col, GVN_SELCHANGING);
    SelectAllCells();
    SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED);
}

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CGridCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetSelectedCount() > 0); // - Thomas Haase 
    //CCellRange Selection = GetSelectedCellRange();
    //pCmdUI->Enable(Selection.Count() && IsValid(Selection));
}

void CGridCtrl::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsEditable() && GetSelectedCount() > 0); // - Thomas Haase 
    //CCellRange Selection = GetSelectedCellRange();
    //pCmdUI->Enable(IsEditable() && Selection.Count() && IsValid(Selection));
}

void CGridCtrl::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    CCellID cell = GetFocusCell();

    BOOL bCanPaste = IsValid(cell) && IsCellEditable(cell) &&
        ::IsClipboardFormatAvailable(CF_TEXT);

    pCmdUI->Enable(bCanPaste);
}
#endif

void CGridCtrl::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_bEnableSelection);
}

////////////////////////////////////////////////////////////////////////////////////////
// hittest-like functions

// TRUE if the mouse is over a row resize area
BOOL CGridCtrl::MouseOverRowResizeArea(CPoint& point)
{
    if (point.x >= GetFixedColumnWidth())
        return FALSE;

    CCellID idCurrentCell = GetCellFromPt(point);
    CPoint start;
    if (!GetCellOrigin(idCurrentCell, &start))
        return FALSE;

    //int endy = start.y + GetRowHeight(idCurrentCell.row);
	//Merge the selected cells 
	//by Huang Wei
	int endy = start.y + GetMergeCellHeight(idCurrentCell);

    if ((point.y - start.y < m_nResizeCaptureRange && idCurrentCell.row != 0) ||
        endy - point.y < m_nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

// TRUE if the mouse is over a column resize area. point is in Client coords
BOOL CGridCtrl::MouseOverColumnResizeArea(CPoint& point)
{
    if (point.y >= GetFixedRowHeight())
        return FALSE;

    CCellID idCurrentCell = GetCellFromPt(point);
    CPoint start;
    if (!GetCellOrigin(idCurrentCell, &start))
        return FALSE;

    //int endx = start.x + GetColumnWidth(idCurrentCell.col);
	//Merge the selected cells 
	//by Huang Wei
	int endx = start.x + GetMergeCellWidth(idCurrentCell);

    if ((point.x - start.x < m_nResizeCaptureRange && idCurrentCell.col != 0) ||
        endx - point.x < m_nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

// Get cell from point.
// point - client coordinates
// bAllowFixedCellCheck - if TRUE then fixed cells are checked
CCellID CGridCtrl::GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/)
{
    CCellID cellID; // return value

    CCellID idTopLeft = GetTopleftNonFixedCell();
	if (!bAllowFixedCellCheck && !IsValid(idTopLeft))
		return cellID;

    // calculate column index
    int fixedColWidth = GetFixedColumnWidth();

    if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth)) // not in window
        cellID.col = -1;
    else if (point.x < fixedColWidth) // in fixed col
    {
        int xpos = 0;
        int col = 0;
        while (col < m_nFixedCols)
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x)
                break;
			col++;
        }

        cellID.col = col;
    }
    else    // in non-fixed col
    {
        int xpos = fixedColWidth;
		int col = idTopLeft.col; //m_nFixedCols;
        while ( col < GetColumnCount())
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x)
                break;
			col++;
        }

        if (col >= GetColumnCount())
            cellID.col = -1;
        else
            cellID.col = col;
    }

    // calculate row index
    int fixedRowHeight = GetFixedRowHeight();
    if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
        cellID.row = -1;
    else if (point.y < fixedRowHeight) // in fixed col
    {
        int ypos = 0;
        int row = 0;
        while (row < m_nFixedRows) 
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y)
                break;
			row++;
        }
        cellID.row = row;
    }
    else
    {
        int ypos = fixedRowHeight;
		int row = idTopLeft.row; //m_nFixedRows;
        while ( row < GetRowCount() )
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y)
                break;
			row++;
        }

        if (row >= GetRowCount())
            cellID.row = -1;
        else
            cellID.row = row;
    }

	//Used for merge cells
	//by Huang Wei
	cellID=GetMergeCellID(cellID);
    return cellID;
}

////////////////////////////////////////////////////////////////////////////////
// CGridCtrl cellrange functions

// Gets the first non-fixed cell ID
CCellID CGridCtrl::GetTopleftNonFixedCell(BOOL bForceRecalculation /*=FALSE*/)
{
    // Used cached value if possible
    if (m_idTopLeftCell.IsValid() && !bForceRecalculation)
        return m_idTopLeftCell;

    int nVertScroll = GetScrollPos(SB_VERT), 
        nHorzScroll = GetScrollPos(SB_HORZ);

    m_idTopLeftCell.col = m_nFixedCols;
    int nRight = 0;
    while (nRight < nHorzScroll && m_idTopLeftCell.col < (GetColumnCount()-1))
        nRight += GetColumnWidth(m_idTopLeftCell.col++);

    m_idTopLeftCell.row = m_nFixedRows;
    int nTop = 0;
    while (nTop < nVertScroll && m_idTopLeftCell.row < (GetRowCount()-1))
        nTop += GetRowHeight(m_idTopLeftCell.row++);

    //TRACE2("TopLeft cell is row %d, col %d\n",m_idTopLeftCell.row, m_idTopLeftCell.col);
    return m_idTopLeftCell;
}





// This gets even partially visible cells
CCellRange CGridCtrl::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/, 
                                                  BOOL bForceRecalculation /*=FALSE*/)
{
    int i;
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell(bForceRecalculation);

    // calc bottom
    int bottom = GetFixedRowHeight();
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom)
        {
            bottom = rect.bottom;
            break;
        }
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);

    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right)
        {
            right = rect.right;
            break;
        }
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (pRect)
    {
        pRect->left = pRect->top = 0;
        pRect->right = right;
        pRect->bottom = bottom;
    }

    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}


CCellRange CGridCtrl::GetVisibleFixedCellRange(LPRECT pRect /*=NULL*/, 
                                                  BOOL bForceRecalculation /*=FALSE*/)
{
    int i;
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell(bForceRecalculation);

    // calc bottom
    int bottom = GetFixedRowHeight();
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom)
        {
            bottom = rect.bottom;
            break;
        }
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);

    // calc right
    int right = 0;
    for (i = 0; i < GetFixedColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right)
        {
            right = rect.right;
            break;
        }
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (pRect)
    {
        pRect->left = pRect->top = 0;
        pRect->right = right;
        pRect->bottom = bottom;
    }

    return CCellRange(idTopLeft.row, 0, maxVisibleRow, maxVisibleCol);
}




// used by ResetScrollBars() - This gets only fully visible cells
CCellRange CGridCtrl::GetUnobstructedNonFixedCellRange(BOOL bForceRecalculation /*=FALSE*/)
{
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell(bForceRecalculation);

    // calc bottom
    int i;
    int bottom = GetFixedRowHeight();
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom)
            break;
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);
    if (maxVisibleRow > 0 && bottom > rect.bottom)
        maxVisibleRow--;

    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right)
            break;
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (maxVisibleCol > 0 && right > rect.right)
        maxVisibleCol--;


    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// Returns the minimum bounding range of the current selection
// If no selection, then the returned CCellRange will be invalid
CCellRange CGridCtrl::GetSelectedCellRange() const
{
    CCellRange Selection(GetRowCount(), GetColumnCount(), -1,-1);

    for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        Selection.SetMinRow( min(Selection.GetMinRow(), cell.row) );
        Selection.SetMinCol( min(Selection.GetMinCol(), cell.col) );
        Selection.SetMaxRow( __max(Selection.GetMaxRow(), cell.row) );
        Selection.SetMaxCol( __max(Selection.GetMaxCol(), cell.col) );
    }
    return Selection;
}

// Returns ALL the cells in the grid
CCellRange CGridCtrl::GetCellRange() const
{
    return CCellRange(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

// Resets the selected cell range to the empty set.
void CGridCtrl::ResetSelectedRange()
{
    m_PrevSelectedCellMap.RemoveAll();
    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}

// Get/Set scroll position using 32 bit functions
int CGridCtrl::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

BOOL CGridCtrl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    m_idTopLeftCell.row = -1;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

void CGridCtrl::EnableScrollBars(int nBar, BOOL bEnable /*=TRUE*/)
{
    if (bEnable)
    {
        if (!IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            m_nBarState |= GVL_HORZ;
            CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
        }
        
        if (!IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            m_nBarState |= GVL_VERT;
            CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
        }
    }
    else
    {
        if ( IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
        {
            m_nBarState &= ~GVL_HORZ; 
            CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
        }
        
        if ( IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
        {
            m_nBarState &= ~GVL_VERT;
            CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
        }
    }
}

// If resizing or cell counts/sizes change, call this - it'll fix up the scroll bars
void CGridCtrl::ResetScrollBars()
{
    // Force a refresh. 
    m_idTopLeftCell.row = -1;

    if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
        return;
    
    CRect rect;
    
    // This would have caused OnSize event - Brian 
    //EnableScrollBars(SB_BOTH, FALSE); 
    
    GetClientRect(rect);
    
    if (rect.left == rect.right || rect.top == rect.bottom)
        return;
    
    if (IsVisibleVScroll())
        rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);
    
    if (IsVisibleHScroll())
        rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);
    
    rect.left += GetFixedColumnWidth();
    rect.top += GetFixedRowHeight();
    
    
    if (rect.left >= rect.right || rect.top >= rect.bottom)
    {
        EnableScrollBarCtrl(SB_BOTH, FALSE);
        return;
    }
    
    CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), 
		              rect.right, rect.bottom);
    CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(),
		              GetVirtualWidth(), GetVirtualHeight());
    
    // Removed to fix single row scrollbar problem (Pontus Goffe)
    // CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
    // if (!IsValid(visibleCells)) return;
        
    //TRACE(_T("Visible: %d x %d, Virtual %d x %d.  H %d, V %d\n"), 
    //      VisibleRect.Width(), VisibleRect.Height(),
    //      VirtualRect.Width(), VirtualRect.Height(),
    //      IsVisibleHScroll(), IsVisibleVScroll());

    // If vertical scroll bar, horizontal space is reduced
    if (VisibleRect.Height() < VirtualRect.Height())
        VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
    // If horz scroll bar, vert space is reduced
    if (VisibleRect.Width() < VirtualRect.Width())
        VisibleRect.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);
    
    // Recheck vertical scroll bar
    //if (VisibleRect.Height() < VirtualRect.Height())
    // VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
    
    if (VisibleRect.Height() < VirtualRect.Height())
    {
        EnableScrollBars(SB_VERT, TRUE); 
        m_nVScrollMax = VirtualRect.Height() - 1;
    }
    else
    {
        EnableScrollBars(SB_VERT, FALSE); 
        m_nVScrollMax = 0;
    }

    if (VisibleRect.Width() < VirtualRect.Width())
    {
        EnableScrollBars(SB_HORZ, TRUE); 
        m_nHScrollMax = VirtualRect.Width() - 1;
    }
    else
    {
        EnableScrollBars(SB_HORZ, FALSE); 
        m_nHScrollMax = 0;
    }

    ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine

    /* Old code - CJM
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE;
    si.nPage = (m_nHScrollMax>0)? VisibleRect.Width() : 0;
    SetScrollInfo(SB_HORZ, &si, FALSE); 
    si.nPage = (m_nVScrollMax>0)? VisibleRect.Height() : 0;
    SetScrollInfo(SB_VERT, &si, FALSE);

    SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE);
    SetScrollRange(SB_HORZ, 0, m_nHScrollMax, TRUE);
    */

    // New code - Paul Runstedler 
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE | SIF_RANGE;
    si.nPage = (m_nHScrollMax>0)? VisibleRect.Width() : 0;
    si.nMin = 0;
    si.nMax = m_nHScrollMax;
    SetScrollInfo(SB_HORZ, &si, TRUE);

    si.fMask |= SIF_DISABLENOSCROLL;
    si.nPage = (m_nVScrollMax>0)? VisibleRect.Height() : 0;
    si.nMin = 0;
    si.nMax = m_nVScrollMax;
    SetScrollInfo(SB_VERT, &si, TRUE);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

// returns the top left point of the cell. Returns FALSE if cell not visible.
BOOL CGridCtrl::GetCellOrigin(int nRow, int nCol, LPPOINT p)
{
    int i;

    if (!IsValid(nRow, nCol))
        return FALSE;

    CCellID idTopLeft;
    if (nCol >= m_nFixedCols || nRow >= m_nFixedRows)
        idTopLeft = GetTopleftNonFixedCell();

	//Merge the selected cells 
	//by Huang Wei
//     if ((nRow >= m_nFixedRows && nRow < idTopLeft.row) ||
//         (nCol>= m_nFixedCols && nCol < idTopLeft.col))
//         return FALSE;

	p->x = 0;
	if (nCol < m_nFixedCols)                      // is a fixed column
		for (i = 0; i < nCol; i++)
			p->x += GetColumnWidth(i);
	else 
	{                                        // is a scrollable data column
		for (i = 0; i < m_nFixedCols; i++)
			p->x += GetColumnWidth(i);
		//Merge the selected cells 
		//by Huang Wei
		if(nCol>idTopLeft.col)
		{
			for (i = idTopLeft.col; i < nCol; i++)
				p->x += GetColumnWidth(i);
		}
		else
		{
			for (i = nCol; i <idTopLeft.col ; i++)
				p->x -= GetColumnWidth(i);
		}
	}

	p->y = 0;
	if (nRow < m_nFixedRows)                      // is a fixed row
		for (i = 0; i < nRow; i++)
			p->y += GetRowHeight(i);
	else 
	{                                        // is a scrollable data row
		for (i = 0; i < m_nFixedRows; i++)
			p->y += GetRowHeight(i);
		//Merge the selected cells 
		//by Huang Wei
		if(nRow>idTopLeft.row)
		{
			for (i = idTopLeft.row; i < nRow; i++)
				p->y += GetRowHeight(i);
		}
		else
		{
			for (i = nRow; i <idTopLeft.row; i++)
				p->y -= GetRowHeight(i);
		}
	}

	return TRUE;
}

BOOL CGridCtrl::GetCellOrigin(const CCellID& cell, LPPOINT p)
{
    return GetCellOrigin(cell.row, cell.col, p);
}

// Returns the bounding box of the cell
BOOL CGridCtrl::GetCellRect(const CCellID& cell, LPRECT pRect)
{
    return GetCellRect(cell.row, cell.col, pRect);
}

BOOL CGridCtrl::GetCellRect(int nRow, int nCol, LPRECT pRect)
{
    CPoint CellOrigin;
    if (!GetCellOrigin(nRow, nCol, &CellOrigin))
        return FALSE;

	//Merge the selected cells 
	//by Huang Wei
	CGridCellBase *pCell = (CGridCellBase*) GetCell(nRow,nCol);
	if(!pCell->IsMerged())
	{
		pRect->left   = CellOrigin.x;
		pRect->top    = CellOrigin.y;
		pRect->right  = CellOrigin.x + GetColumnWidth(nCol)-1;
		pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;
	}
	else
	{
		GetCellRangeRect(pCell->m_MergeRange,pRect);
	}
//     pRect->left   = CellOrigin.x;
//     pRect->top    = CellOrigin.y;
//     pRect->right  = CellOrigin.x + GetColumnWidth(nCol)-1;
//     pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;

    //TRACE("Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d\n",
    //      nRow,nCol, CellOrigin.x, CellOrigin.y, GetColumnWidth(nCol), GetRowHeight(nRow),
    //      pRect->left, pRect->top, pRect->right, pRect->bottom);

    return TRUE;
}

BOOL CGridCtrl::GetTextRect(const CCellID& cell, LPRECT pRect)
{
    return GetTextRect(cell.row, cell.col, pRect);
}

BOOL CGridCtrl::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
    CGridCellBase* pCell = GetCell( nRow, nCol);
    if( pCell == NULL)
        return FALSE;
    
    if( !GetCellRect( nRow, nCol, pRect) )
        return FALSE;

    return pCell->GetTextRect( pRect);
}

// Returns the bounding box of a range of cells
BOOL CGridCtrl::GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect)
{
    CPoint MinOrigin,MaxOrigin;

    if (!GetCellOrigin(cellRange.GetMinRow(), cellRange.GetMinCol(), &MinOrigin))
        return FALSE;
    if (!GetCellOrigin(cellRange.GetMaxRow(), cellRange.GetMaxCol(), &MaxOrigin))
        return FALSE;

    lpRect->left   = MinOrigin.x;
    lpRect->top    = MinOrigin.y;
    lpRect->right  = MaxOrigin.x + GetColumnWidth(cellRange.GetMaxCol()) - 1;
    lpRect->bottom = MaxOrigin.y + GetRowHeight(cellRange.GetMaxRow()) - 1;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CGridCtrl::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
    LRESULT result = Default();

    // Get the logical font
    LOGFONT lf;
    if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
        return result;

    m_cellDefault.SetFont(&lf);
    m_cellFixedColDef.SetFont(&lf);
    m_cellFixedRowDef.SetFont(&lf);
    m_cellFixedRowColDef.SetFont(&lf);

    Refresh();

    return result;
}

LRESULT CGridCtrl::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    //LOGFONT    lf;
    //m_cellDefault.GetFontObject()->GetLogFont(&lf);

    return (LRESULT) m_cellDefault.GetFontObject()->GetSafeHandle();
}

#ifndef _WIN32_WCE_NO_CURSOR
BOOL CGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT)
    {
        switch (m_MouseMode)
        {
        case MOUSE_OVER_COL_DIVIDE:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
            break;
        case MOUSE_OVER_ROW_DIVIDE:
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
            break;
#ifndef GRIDCONTROL_NO_DRAGDROP
        case MOUSE_DRAGGING:
            break;
#endif
        default:
            if (!GetVirtualMode())
            {
                CPoint pt(GetMessagePos());
                ScreenToClient(&pt);
                CCellID cell = GetCellFromPt(pt);
                if (IsValid(cell))
                {
                    CGridCellBase* pCell = GetCell(cell.row, cell.col);
                    if (pCell)
                        return pCell->OnSetCursor();
                }
            }

            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        }
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CGridCtrl::SetFixedRowCount(int nFixedRows)
{
    if (m_nFixedRows == nFixedRows)
        return TRUE;

    ASSERT(nFixedRows >= 0);

    ResetSelectedRange();

    // Force recalculation
    m_idTopLeftCell.col = -1;

    if (nFixedRows > GetRowCount())
        if (!SetRowCount(nFixedRows))
            return FALSE;
        
        if (m_idCurrentCell.row < nFixedRows)
            SetFocusCell(-1, - 1);
        
        if (!GetVirtualMode())
        {
            if (nFixedRows > m_nFixedRows)
            {
                for (int i = m_nFixedRows; i < nFixedRows; i++)
                    for (int j = 0; j < GetColumnCount(); j++)
                    {
                        SetItemState(i, j, GetItemState(i, j) | GVIS_FIXED | GVIS_FIXEDROW);
                        SetItemBkColour(i, j, CLR_DEFAULT );
                        SetItemFgColour(i, j, CLR_DEFAULT );
                    }
            }
            else
            {
                for (int i = nFixedRows; i < m_nFixedRows; i++)
                {
                    int j;
                    for (j = 0; j < GetFixedColumnCount(); j++)
                        SetItemState(i, j, GetItemState(i, j) & ~GVIS_FIXEDROW );

                    for (j = GetFixedColumnCount(); j < GetColumnCount(); j++)
                    {
                        SetItemState(i, j, GetItemState(i, j) & ~(GVIS_FIXED | GVIS_FIXEDROW) );
                        SetItemBkColour(i, j, CLR_DEFAULT );
                        SetItemFgColour(i, j, CLR_DEFAULT );
                    }
                }
            }
        }

        m_nFixedRows = nFixedRows;
        
        Refresh();
        
        return TRUE;
}

BOOL CGridCtrl::SetFixedColumnCount(int nFixedCols)
{
    if (m_nFixedCols == nFixedCols)
        return TRUE;

    ASSERT(nFixedCols >= 0);

    if (nFixedCols > GetColumnCount())
        if (!SetColumnCount(nFixedCols))
            return FALSE;

    if (m_idCurrentCell.col < nFixedCols)
        SetFocusCell(-1, - 1);

    ResetSelectedRange();

    // Force recalculation
    m_idTopLeftCell.col = -1;

    if (!GetVirtualMode())
    {
        if (nFixedCols > m_nFixedCols)
        {
            for (int i = 0; i < GetRowCount(); i++)
                for (int j = m_nFixedCols; j < nFixedCols; j++)
                {
                    SetItemState(i, j, GetItemState(i, j) | GVIS_FIXED | GVIS_FIXEDCOL);
                    SetItemBkColour(i, j, CLR_DEFAULT );
                    SetItemFgColour(i, j, CLR_DEFAULT );
                }
        }
        else
        {
			{ // Scope limit i,j
	            for (int i = 0; i < GetFixedRowCount(); i++)
		            for (int j = nFixedCols; j < m_nFixedCols; j++)
			            SetItemState(i, j, GetItemState(i, j) & ~GVIS_FIXEDCOL );
			}
			{// Scope limit i,j
	            for (int i = GetFixedRowCount(); i < GetRowCount(); i++)
		            for (int j = nFixedCols; j < m_nFixedCols; j++)
			        {
				        SetItemState(i, j, GetItemState(i, j) & ~(GVIS_FIXED | GVIS_FIXEDCOL) );
					    SetItemBkColour(i, j, CLR_DEFAULT );
						SetItemFgColour(i, j, CLR_DEFAULT );
	                }
			}
        }
    }
        
    m_nFixedCols = nFixedCols;
        
    Refresh();
        
    return TRUE;
}

BOOL CGridCtrl::SetRowCount(int nRows)
{
    BOOL bResult = TRUE;

    ASSERT(nRows >= 0);
    if (nRows == GetRowCount())
        return bResult;

    // Force recalculation
    m_idTopLeftCell.col = -1;

    if (nRows < m_nFixedRows)
        m_nFixedRows = nRows;

    if (m_idCurrentCell.row >= nRows)
        SetFocusCell(-1, - 1);

    int addedRows = nRows - GetRowCount();

    // If we are about to lose rows, then we need to delete the GridCell objects
    // in each column within each row
    if (addedRows < 0)
    {
        if (!GetVirtualMode())
        {
            for (int row = nRows; row < m_nRows; row++)
            {
                // Delete cells
                for (int col = 0; col < m_nCols; col++)
                    DestroyCell(row, col);
            
                // Delete rows
                GRID_ROW* pRow = m_RowData[row];
                if (pRow)
                    delete pRow;
            }
        }
        m_nRows = nRows;
    }
    
    TRY
    {
        m_arRowHeights.SetSize(nRows);

        if (GetVirtualMode())
        {
            m_nRows = nRows;
            if (addedRows > 0)
            {
                int startRow = nRows - addedRows;
                for (int row = startRow; row < nRows; row++)
                    m_arRowHeights[row] = m_cellDefault.GetHeight();
            }
			ResetVirtualOrder();
        }
        else
        {
            // Change the number of rows.
            m_RowData.SetSize(nRows);

            // If we have just added rows, we need to construct new elements for each cell
            // and set the default row height
            if (addedRows > 0)
            {
                // initialize row heights and data
                int startRow = nRows - addedRows;
                for (int row = startRow; row < nRows; row++)
                {
                    m_arRowHeights[row] = m_cellDefault.GetHeight();

                    m_RowData[row] = new GRID_ROW;
                    m_RowData[row]->SetSize(m_nCols);
                    for (int col = 0; col < m_nCols; col++)
                    {
                        GRID_ROW* pRow = m_RowData[row];
                        if (pRow && !GetVirtualMode())
                            pRow->SetAt(col, CreateCell(row, col));
                    }
                    m_nRows++;
                }
            }
        }
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        bResult = FALSE;
    }
    END_CATCH

    SetModified();
    ResetScrollBars();
    Refresh();

    return bResult;
}

BOOL CGridCtrl::SetColumnCount(int nCols)
{
    BOOL bResult = TRUE;

    ASSERT(nCols >= 0);

    if (nCols == GetColumnCount())
        return bResult;

    // Force recalculation
    m_idTopLeftCell.col = -1;

    if (nCols < m_nFixedCols)
        m_nFixedCols = nCols;

    if (m_idCurrentCell.col >= nCols)
        SetFocusCell(-1, - 1);

    int addedCols = nCols - GetColumnCount();

    // If we are about to lose columns, then we need to delete the GridCell objects
    // within each column
    if (addedCols < 0 && !GetVirtualMode())
    {
        for (int row = 0; row < m_nRows; row++)
            for (int col = nCols; col < GetColumnCount(); col++)
                DestroyCell(row, col);
    }

    TRY 
    {
        // Change the number of columns.
        m_arColWidths.SetSize(nCols);
    
        // Change the number of columns in each row.
        if (!GetVirtualMode())
            for (int i = 0; i < m_nRows; i++)
                if (m_RowData[i])
                    m_RowData[i]->SetSize(nCols);
        
        // If we have just added columns, we need to construct new elements for each cell
        // and set the default column width
        if (addedCols > 0)
        {
            int row, col;

            // initialized column widths
            int startCol = nCols - addedCols;
            for (col = startCol; col < nCols; col++)
                m_arColWidths[col] = m_cellFixedColDef.GetWidth();
        
            // initialise column data
            if (!GetVirtualMode())
            {
                for (row = 0; row < m_nRows; row++)
                    for (col = startCol; col < nCols; col++)
                    {
                        GRID_ROW* pRow = m_RowData[row];
                        if (pRow)
                            pRow->SetAt(col, CreateCell(row, col));
                    }
            }
        }
        // else    // check for selected cell ranges
        //    ResetSelectedRange();
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        bResult = FALSE;
    }
    END_CATCH

	m_arColOrder.resize(nCols);  // Reset Column Order
    for (int i = 0; i < nCols; i++)
	{
		m_arColOrder[i] = i;	
	}

    m_nCols = nCols;

    SetModified();
    ResetScrollBars();
    Refresh();

    return bResult;
}

// Insert a column at a given position, or add to end of columns (if nColumn = -1)
int CGridCtrl::InsertColumn(LPCTSTR strHeading,
                            UINT nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int nColumn  /* = -1 */)
{
	ASSERT(!m_AllowReorderColumn); // function not implemented in case of m_AllowReorderColumn option
    if (nColumn >= 0 && nColumn < m_nFixedCols)
    {

        // TODO: Fix it so column insertion works for in the fixed column area
        ASSERT(FALSE);
        return -1;
    }

    // If the insertion is for a specific column, check it's within range.
    if (nColumn >= 0 && nColumn > GetColumnCount())
        return -1;

    // Force recalculation
    m_idTopLeftCell.col = -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the column.
    if (m_nRows < 1)
        SetRowCount(1);

    // Allow the user to insert after the last of the columns, but process it as a
    // "-1" column, meaning it gets flaged as being the last column, and not a regular
    // "insert" routine.
    if (nColumn == GetColumnCount())
        nColumn = -1;

    TRY
    {
        if (nColumn < 0)
        {
            nColumn = m_nCols;
            m_arColWidths.Add(0);
            if (!GetVirtualMode())
            {
                for (int row = 0; row < m_nRows; row++)
                {
                    GRID_ROW* pRow = m_RowData[row];
                    if (!pRow)
                        return -1;
                    pRow->Add(CreateCell(row, nColumn));
                }
				intlist::iterator it = m_arColOrder.end();
				m_arColOrder.insert(it, 1, nColumn);
            }
        } 
        else
        {
            m_arColWidths.InsertAt(nColumn, (UINT)0);
            if (!GetVirtualMode())
            {
                for (int row = 0; row < m_nRows; row++) 
                {
                    GRID_ROW* pRow = m_RowData[row];
                    if (!pRow)
                        return -1;
                    pRow->InsertAt(nColumn, CreateCell(row, nColumn));
                }
            }
// 			m_arColOrder::iterator it = m_arColOrder.end();
// 			m_arColOrder.insert(it, sizeof(int), nColumn);
        }
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        return -1;
    }
    END_CATCH

	m_nCols++;

    // Initialise column data
    SetItemText(0, nColumn, strHeading);
    for (int row = 0; row < m_nRows; row++) 
        SetItemFormat(row, nColumn, nFormat);
    
    // initialized column width, 以文字的长度作为列宽, 改掉吧
    //m_arColWidths[nColumn] = GetTextExtent(0, nColumn, strHeading).cx;

	//Modify by liqs99
	m_arColWidths[nColumn] = m_cellFixedColDef.GetWidth();
    
    if (m_idCurrentCell.col != -1 && nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col++;
    
    ResetScrollBars();

    SetModified();
    
    return nColumn;
}

// Insert a row at a given position, or add to end of rows (if nRow = -1)
int CGridCtrl::InsertRow(LPCTSTR strHeading, int nRow /* = -1 */)
{
    if (nRow >= 0 && nRow < m_nFixedRows)
    {
        // TODO: Fix it so column insertion works for in the fixed row area
        ASSERT(FALSE);
        return -1;
    }

    // If the insertion is for a specific row, check it's within range.
    if (nRow >= 0 && nRow >= GetRowCount())
        return -1;

    // Force recalculation
    m_idTopLeftCell.col = -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (m_nCols < 1)
        SetColumnCount(1);

    TRY
    {
        // Adding a row to the bottom
        if (nRow < 0)
        {
            nRow = m_nRows;
            m_arRowHeights.Add(0);
            if (!GetVirtualMode())
                m_RowData.Add(new GRID_ROW);
			else
				m_arRowOrder.push_back(m_nRows);

        }
        else
        {
            m_arRowHeights.InsertAt(nRow, (UINT)0);
            if (!GetVirtualMode())
                m_RowData.InsertAt(nRow, new GRID_ROW);
			else
			{
				ResetVirtualOrder();
			}
        }

        if (!GetVirtualMode())
            m_RowData[nRow]->SetSize(m_nCols);
    }
    CATCH (CMemoryException, e)
    {
        e->ReportError();
        return -1;
    }
    END_CATCH

    m_nRows++;

    // Initialise cell data
    if (!GetVirtualMode())
    {
        for (int col = 0; col < m_nCols; col++)
        {
            GRID_ROW* pRow = m_RowData[nRow];
            if (!pRow)
                return -1;
            pRow->SetAt(col, CreateCell(nRow, col));
        }
    }

    // Set row title
    SetItemText(nRow, 0, strHeading);

    // initialized row height
    if (strHeading && strHeading[0])
        m_arRowHeights[nRow] = GetTextExtent(nRow, 0, strHeading).cy;
    else
        m_arRowHeights[nRow] = m_cellFixedRowDef.GetHeight();

    if (m_idCurrentCell.row != -1 && nRow < m_idCurrentCell.row)
        m_idCurrentCell.row++;

    ResetScrollBars();

    SetModified();

    return nRow;
}

///////////////////////////////////////////////////////////////////////////////
// Cell creation stuff

BOOL CGridCtrl::SetCellType(int nRow, int nCol, CRuntimeClass* pRuntimeClass)
{
    if (GetVirtualMode())
        return FALSE;

    ASSERT(IsValid(nRow, nCol));
    if (!IsValid(nRow, nCol))
        return FALSE;

    if (!pRuntimeClass->IsDerivedFrom(RUNTIME_CLASS(CGridCellBase)))
    {
        ASSERT( FALSE);
        return FALSE;
    }

    CGridCellBase* pNewCell = (CGridCellBase*) pRuntimeClass->CreateObject();

    CGridCellBase* pCurrCell = GetCell(nRow, nCol);
    if (pCurrCell)
        *pNewCell = *pCurrCell;

    SetCell(nRow, nCol, pNewCell);
    delete pCurrCell;

    return TRUE;
}

BOOL CGridCtrl::SetDefaultCellType( CRuntimeClass* pRuntimeClass)
{
    ASSERT( pRuntimeClass != NULL );
    if (!pRuntimeClass->IsDerivedFrom(RUNTIME_CLASS(CGridCellBase)))
    {
        ASSERT( FALSE);
        return FALSE;
    }
    m_pRtcDefault = pRuntimeClass;
    return TRUE;
}

// Creates a new grid cell and performs any necessary initialisation
/*virtual*/ CGridCellBase* CGridCtrl::CreateCell(int nRow, int nCol)
{
    ASSERT(!GetVirtualMode());

    if (!m_pRtcDefault || !m_pRtcDefault->IsDerivedFrom(RUNTIME_CLASS(CGridCellBase)))
    {
        ASSERT( FALSE);
        return NULL;
    }
    CGridCellBase* pCell = (CGridCellBase*) m_pRtcDefault->CreateObject();
    if (!pCell)
        return NULL;

    pCell->SetGrid(this);
    pCell->SetCoords(nRow, nCol); 

    if (nCol < m_nFixedCols)
        pCell->SetState(pCell->GetState() | GVIS_FIXED | GVIS_FIXEDCOL);
    if (nRow < m_nFixedRows)
        pCell->SetState(pCell->GetState() | GVIS_FIXED | GVIS_FIXEDROW);
    
    pCell->SetFormat(pCell->GetDefaultCell()->GetFormat());

    return pCell;
}

// Performs any cell cleanup necessary to maintain grid integrity
/*virtual*/ void CGridCtrl::DestroyCell(int nRow, int nCol)
{
    // Should NEVER get here in virtual mode.
    ASSERT(!GetVirtualMode());

    // Set the cells state to 0. If the cell is selected, this
    // will remove the cell from the selected list.
    SetItemState(nRow, nCol, 0);

    delete GetCell(nRow, nCol);
}

BOOL CGridCtrl::DeleteColumn(int nColumn)
{
    if (nColumn < 0 || nColumn >= GetColumnCount())
        return FALSE;

    ResetSelectedRange();

    if (!GetVirtualMode())
    {
        for (int row = 0; row < GetRowCount(); row++)
        {
            GRID_ROW* pRow = m_RowData[row];
            if (!pRow)
                return FALSE;

            DestroyCell(row, nColumn);
        
            pRow->RemoveAt(nColumn);
        }
    }
    m_arColWidths.RemoveAt(nColumn);
    m_nCols--;
    if (nColumn < m_nFixedCols)
        m_nFixedCols--;
    
    if (nColumn == m_idCurrentCell.col)
        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col--;
    
    ResetScrollBars();

    SetModified();

    return TRUE;
}


void CGridCtrl::AddSubVirtualRow(int Num, int Nb)
{
	ASSERT(Nb == -1 || Nb ==1); // only these vlaues are implemented now
	if(!GetVirtualMode()) return;
	for(int ind = 0; ind<m_nRows ; ind++)
		if(m_arRowOrder[ind]>Num) m_arRowOrder[ind]+=Nb;
	if(Nb>0)
		m_arRowOrder.insert(m_arRowOrder.begin()+Num,Num);
	else
		m_arRowOrder.erase(m_arRowOrder.begin()+Num);
}


BOOL CGridCtrl::DeleteRow(int nRow)
{
    if (nRow < 0 || nRow >= GetRowCount())
        return FALSE;

    ResetSelectedRange();

    if (!GetVirtualMode())
    {
        GRID_ROW* pRow = m_RowData[nRow];
        if (!pRow)
            return FALSE;

        for (int col = 0; col < GetColumnCount(); col++)
            DestroyCell(nRow, col);

        delete pRow;
        m_RowData.RemoveAt(nRow);
    }
    else
    	AddSubVirtualRow(nRow,-1);

    m_arRowHeights.RemoveAt(nRow);

    m_nRows--;
    if (nRow < m_nFixedRows)
        m_nFixedRows--;
    
    if (nRow == m_idCurrentCell.row)
        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nRow < m_idCurrentCell.row)
        m_idCurrentCell.row--;
    
    ResetScrollBars();

    SetModified();
    Refresh();
    return TRUE;
}

// Handy function that removes all non-fixed rows
BOOL CGridCtrl::DeleteNonFixedRows()
{
    ResetSelectedRange();
    int nFixed = GetFixedRowCount();
    int nCount = GetRowCount();
    if (GetVirtualMode())
	{
		if(nCount != nFixed)
		{
			SetRowCount(nFixed);
			m_arRowOrder.resize(nFixed);
			m_arRowHeights.SetSize(nFixed);
			m_idCurrentCell.row = m_idCurrentCell.col = -1;
			ResetScrollBars();
			SetModified();
		}
	}
	else
	{
    // Delete all data rows
    for (int nRow = nCount; nRow >= nFixed; nRow--)
        DeleteRow(nRow);
	}
    return TRUE;
}

// Removes all rows, columns and data from the grid.
BOOL CGridCtrl::DeleteAllItems()
{
    ResetSelectedRange();

    m_arColWidths.RemoveAll();
    m_arRowHeights.RemoveAll();

    // Delete all cells in the grid
    if (!GetVirtualMode())
    {
        for (int row = 0; row < m_nRows; row++)
        {
            for (int col = 0; col < m_nCols; col++)
                DestroyCell(row, col);

            GRID_ROW* pRow = m_RowData[row];
            delete pRow;
        }

        // Remove all rows
        m_RowData.RemoveAll();
    }
	else
		m_arRowOrder.clear();


    m_idCurrentCell.row = m_idCurrentCell.col = -1;
    m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;

    ResetScrollBars();

    SetModified();

    return TRUE;
}

void CGridCtrl::AutoFill()
{
    if (!::IsWindow(m_hWnd))
        return;

    CRect rect;
    GetClientRect(rect);

    SetColumnCount(rect.Width() / m_cellDefault.GetWidth() + 1);
    SetRowCount(rect.Height() / m_cellDefault.GetHeight() + 1);
    SetFixedRowCount(1);
    SetFixedColumnCount(1);
    ExpandToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

// Set CListCtrl::GetNextItem for details
CCellID CGridCtrl::GetNextItem(CCellID& cell, int nFlags) const
{
    if ((nFlags & GVNI_ALL) == GVNI_ALL)
    {    // GVNI_ALL Search whole Grid beginning from cell
        //          First row (cell.row) -- ONLY Columns to the right of cell
        //          following rows       -- ALL  Columns
        int row = cell.row , col = cell.col + 1;
        if (row <= 0)
            row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            if (col <= 0)
                col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++)
            {
                int nState = GetItemState(row, col);
                if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                    (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                    (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                    (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                    (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                    (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                    return CCellID(row, col);
            }
            // go to First Column
            col = GetFixedColumnCount();
        }
    }
    else if ((nFlags & GVNI_BELOW) == GVNI_BELOW && 
             (nFlags & GVNI_TORIGHT) == GVNI_TORIGHT)
    {   // GVNI_AREA Search Grid beginning from cell to Lower-Right of Grid
        //           Only rows starting with  cell.row and below
        //           All rows   -- ONLY Columns to the right of cell
        int row = cell.row;
        if (row <= 0)
            row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            int col = cell.col + 1;
            if (col <= 0)
                col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++) 
            {
                int nState = GetItemState(row, col);
                if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                    (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                    (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                    (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                    (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                    (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                    return CCellID(row, col);
            }
        }
    }
    else if ((nFlags & GVNI_ABOVE) == GVNI_ABOVE) 
    {
        for (int row = cell.row - 1; row >= GetFixedRowCount(); row--) 
        {
            int nState = GetItemState(row, cell.col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(row, cell.col);
        }
    }
    else if ((nFlags & GVNI_BELOW) == GVNI_BELOW)
    {
        for (int row = cell.row + 1; row < GetRowCount(); row++) 
        {
            int nState = GetItemState(row, cell.col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(row, cell.col);
        }
    } 
    else if ((nFlags & GVNI_TOLEFT) == GVNI_TOLEFT)
    {
        for (int col = cell.col - 1; col >= GetFixedColumnCount(); col--) 
        {
            int nState = GetItemState(cell.row, col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(cell.row, col);
        }
    }
    else if ((nFlags & GVNI_TORIGHT) == GVNI_TORIGHT)
    {
        for (int col = cell.col + 1; col < GetColumnCount(); col++) 
        {
            int nState = GetItemState(cell.row, col);
            if ((nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) || 
                (nFlags & GVNI_FOCUSED     && nState & GVIS_FOCUSED)     ||
                (nFlags & GVNI_SELECTED    && nState & GVIS_SELECTED)    ||
                (nFlags & GVNI_READONLY    && nState & GVIS_READONLY)    ||
                (nFlags & GVNI_FIXED       && nState & GVIS_FIXED)       ||
                (nFlags & GVNI_MODIFIED    && nState & GVIS_MODIFIED))
                return CCellID(cell.row, col);
        }
    }
    
    return CCellID(-1, -1);
}

// Sorts on a given column using the cell text
BOOL CGridCtrl::SortTextItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
    return CGridCtrl::SortItems(pfnCellTextCompare, nCol, bAscending, data);
}

void CGridCtrl::SetCompareFunction(PFNLVCOMPARE pfnCompare)
{
	m_pfnCompare = pfnCompare;
}

// Sorts on a given column using the cell text and using the specified comparison
// function
BOOL CGridCtrl::SortItems(int nCol, BOOL bAscending, LPARAM data /* = 0 */)
{
    SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, - 1);

	if (m_pfnCompare == NULL)
		return CGridCtrl::SortItems(pfnCellTextCompare, nCol, bAscending, data);
	else
	    return CGridCtrl::SortItems(m_pfnCompare, nCol, bAscending, data);
}

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending,
                          LPARAM data /* = 0 */)
{
    SetSortColumn(nCol);
    SetSortAscending(bAscending);
    ResetSelectedRange();
    SetFocusCell(-1, -1);
    return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

int CALLBACK CGridCtrl::pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNUSED_ALWAYS(lParamSort);

	CGridCellBase* pCell1 = (CGridCellBase*) lParam1;
	CGridCellBase* pCell2 = (CGridCellBase*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	BOOL bAscending = (BOOL)lParamSort;
	CString text1 = pCell1->GetText();
	CString text2 = pCell2->GetText();
	if(pCell1->GetStatCell() != -1 || pCell2->GetStatCell() != -1)	
	{
		if(bAscending)	
			return 1;
		else	
			return -1;
	}
	return _tcscmp(pCell1->GetText(), pCell2->GetText());
}

int CALLBACK CGridCtrl::pfnCellNumericCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNUSED_ALWAYS(lParamSort);

	CGridCellBase* pCell1 = (CGridCellBase*) lParam1;
	CGridCellBase* pCell2 = (CGridCellBase*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	int nValue1 = _ttol(pCell1->GetText());
	int nValue2 = _ttol(pCell2->GetText());

	if (nValue1 < nValue2)
		return -1;
	else if (nValue1 == nValue2)
		return 0;
	else
		return 1;
}


CGridCtrl *  CGridCtrl::m_This;
// private recursive sort implementation
bool CGridCtrl::NotVirtualCompare(int c1, int c2)
{
	return ! CGridCtrl::m_This->m_pfnVirtualCompare(c1, c2);
}

BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
    if (nCol >= GetColumnCount())
        return FALSE;

	if (high == -1)
		high = GetRowCount() - 1;

    int lo = low;
    int hi = high;
  
    if (hi <= lo)
        return FALSE;

    if (GetVirtualMode())
	{
		ASSERT(m_pfnVirtualCompare);
		m_CurCol = m_arColOrder[nCol];
		m_This = this;
		std::stable_sort(m_arRowOrder.begin() + m_nFixedRows, m_arRowOrder.end()-m_nStatRows, 
                         bAscending ? m_pfnVirtualCompare : NotVirtualCompare);
		return TRUE;
	}
   
    
    //LPARAM midItem = GetItemData((lo + hi)/2, nCol);
	LPARAM pMidCell = (LPARAM) GetCell((lo + hi)/2, nCol);
   
    // loop through the list until indices cross
    while (lo <= hi)
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if (bAscending)
            while (lo < high  && pfnCompare( (LPARAM)GetCell(lo, nCol), (LPARAM) pMidCell, bAscending) < 0)
                ++lo;
        else
            while (lo < high && pfnCompare((LPARAM)GetCell(lo, nCol), pMidCell, bAscending) > 0)
				++lo;
                
		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if (bAscending)
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, bAscending) > 0)
				--hi;
		else
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, bAscending) < 0)
				--hi;
                        
        // If the indexes have not crossed, swap if the items are not equal	
        if (lo <= hi)
        {
			// swap only if the items are not equal
// 			CGridCellBase *pCellLo = GetCell(lo, 0);
// 			CGridCellBase *pCellHi = GetCell(hi, 0);
// 			if(pCellLo->GetStatCell() != -1 || pCellHi->GetStatCell() != -1){return FALSE;}
            if (pfnCompare((LPARAM)GetCell(lo, nCol), (LPARAM)GetCell(hi, nCol), bAscending) != 0)
            {
				for (int col = 0; col < GetColumnCount(); col++)
                {
					CGridCellBase *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell); 
                }
                UINT nRowHeight = m_arRowHeights[lo];
                m_arRowHeights[lo] = m_arRowHeights[hi];
				m_arRowHeights[hi] = nRowHeight;
			}
                            
            ++lo;
            --hi;
         }
    }
    
    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if (low < hi)
        SortItems(pfnCompare, nCol, bAscending, data, low, hi);
    
    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if (lo < high)
        SortItems(pfnCompare, nCol, bAscending, data, lo, high);
    
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

BOOL CGridCtrl::SetItem(const GV_ITEM* pItem)
{
    if (!pItem || GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell)
        return FALSE;

    SetModified(TRUE, pItem->row, pItem->col);

    if (pItem->mask & GVIF_TEXT)
        pCell->SetText(pItem->strText);
    if (pItem->mask & GVIF_PARAM)
        pCell->SetData(pItem->lParam);
    if (pItem->mask & GVIF_IMAGE)
        pCell->SetImage(pItem->iImage);
    if (pItem->mask & GVIF_STATE)
        SetItemState(pItem->row, pItem->col, pItem->nState);
    if (pItem->mask & GVIF_FORMAT)
        pCell->SetFormat(pItem->nFormat);
    if (pItem->mask & GVIF_BKCLR)
        pCell->SetBackClr(pItem->crBkClr);
    if (pItem->mask & GVIF_FGCLR)
        pCell->SetTextClr(pItem->crFgClr);
    if (pItem->mask & GVIF_FONT)
        pCell->SetFont(&(pItem->lfFont));
    if( pItem->mask & GVIF_MARGIN)
        pCell->SetMargin( pItem->nMargin);
    
    return TRUE;
}

BOOL CGridCtrl::GetItem(GV_ITEM* pItem)
{
    if (!pItem)
        return FALSE;
    CGridCellBase* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell)
        return FALSE;

    if (pItem->mask & GVIF_TEXT)
        pItem->strText = GetItemText(pItem->row, pItem->col);
    if (pItem->mask & GVIF_PARAM)
        pItem->lParam  = pCell->GetData();;
    if (pItem->mask & GVIF_IMAGE)
        pItem->iImage  = pCell->GetImage();
    if (pItem->mask & GVIF_STATE)
        pItem->nState  = pCell->GetState();
    if (pItem->mask & GVIF_FORMAT)
        pItem->nFormat = pCell->GetFormat();
    if (pItem->mask & GVIF_BKCLR)
        pItem->crBkClr = pCell->GetBackClr();
    if (pItem->mask & GVIF_FGCLR)
        pItem->crFgClr = pCell->GetTextClr();
    if (pItem->mask & GVIF_FONT)
        memcpy(&(pItem->lfFont), pCell->GetFont(), sizeof(LOGFONT));
    if( pItem->mask & GVIF_MARGIN)
        pItem->nMargin = pCell->GetMargin();

    return TRUE;
}

BOOL CGridCtrl::SetItemText(int nRow, int nCol, LPCTSTR str)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetText(str);

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 210)
// EFW - 06/13/99 - Added to support printf-style formatting codes
BOOL CGridCtrl::SetItemTextFmt(int nRow, int nCol, LPCTSTR szFmt, ...)
{
    if (GetVirtualMode())
        return FALSE;

    CString strText;

    va_list argptr;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    // Format the message text
    va_start(argptr, szFmt);
    strText.FormatV(szFmt, argptr);
    va_end(argptr);

    pCell->SetText(strText);

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

// EFW - 06/13/99 - Added to support string resource ID.  Supports
// a variable argument list too.
BOOL CGridCtrl::SetItemTextFmtID(int nRow, int nCol, UINT nID, ...)
{
    if (GetVirtualMode())
        return FALSE;

    CString strFmt, strText;
    va_list argptr;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    // Format the message text
    va_start(argptr, nID);
    VERIFY(strFmt.LoadString(nID));
    strText.FormatV(strFmt, argptr);
    va_end(argptr);

    pCell->SetText(strText);

    SetModified(TRUE, nRow, nCol);
    return TRUE;
}
#endif

BOOL CGridCtrl::SetItemData(int nRow, int nCol, LPARAM lParam)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetData(lParam);
    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

LPARAM CGridCtrl::GetItemData(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return (LPARAM) 0;

    return pCell->GetData();
}

BOOL CGridCtrl::SetItemImage(int nRow, int nCol, int iImage)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->SetImage(iImage);
    SetModified(TRUE, nRow, nCol);
    return TRUE;
}

int CGridCtrl::GetItemImage(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return -1;

    return pCell->GetImage();
}

BOOL CGridCtrl::SetItemState(int nRow, int nCol, UINT state)
{
    BOOL bSelected = IsCellSelected(nRow, nCol);

    // If the cell is being unselected, remove it from the selected list
    if (bSelected && !(state & GVIS_SELECTED))
    {
        CCellID cell;
        DWORD key = MAKELONG(nRow, nCol);

        if (m_SelectedCellMap.Lookup(key, (CCellID&)cell))
            m_SelectedCellMap.RemoveKey(key);
    }

    // If cell is being selected, add it to the list of selected cells
    else if (!bSelected && (state & GVIS_SELECTED))
    {
        CCellID cell(nRow, nCol);
        m_SelectedCellMap.SetAt(MAKELONG(nRow, nCol), cell);
    }

    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    // Set the cell's state
    pCell->SetState(state);

    return TRUE;
}

UINT CGridCtrl::GetItemState(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetState();
}

BOOL CGridCtrl::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetFormat(nFormat);
    return TRUE;
}

UINT CGridCtrl::GetItemFormat(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetFormat();
}

BOOL CGridCtrl::SetItemBkColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->SetBackClr(cr);
    return TRUE;
}

COLORREF CGridCtrl::GetItemBkColour(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->GetBackClr();
}

BOOL CGridCtrl::SetItemFgColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;
    
    pCell->SetTextClr(cr);
    return TRUE;
}

COLORREF CGridCtrl::GetItemFgColour(int nRow, int nCol) const
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;
    
    return pCell->GetTextClr();
}

BOOL CGridCtrl::SetItemFont(int nRow, int nCol, const LOGFONT* plf)
{
    if (GetVirtualMode())
        return FALSE;

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;
    
    pCell->SetFont(plf);
    
    return TRUE;
}

const LOGFONT* CGridCtrl::GetItemFont(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) 
        return GetDefaultCell(nRow < GetFixedRowCount(), nCol < GetFixedColumnCount())->GetFont();
    
    return pCell->GetFont();
}

BOOL CGridCtrl::IsItemEditing(int nRow, int nCol)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    return pCell->IsEditing();
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CGridCtrl::GetVirtualWidth() const
{
    long lVirtualWidth = 0;
    int iColCount = GetColumnCount();
    for (int i = 0; i < iColCount; i++)
        lVirtualWidth += m_arColWidths[i];

    return lVirtualWidth;
}

long CGridCtrl::GetVirtualHeight() const
{
    long lVirtualHeight = 0;
    int iRowCount = GetRowCount();
    for (int i = 0; i < iRowCount; i++)
        lVirtualHeight += m_arRowHeights[i];

    return lVirtualHeight;
}

int CGridCtrl::GetRowHeight(int nRow) const
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows)
        return -1;

    return m_arRowHeights[nRow];
}

int CGridCtrl::GetColumnWidth(int nCol) const
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols)
        return -1;

    return m_arColWidths[m_arColOrder[nCol]];
}

BOOL CGridCtrl::SetRowHeight(int nRow, int height)
{
    ASSERT(nRow >= 0 && nRow < m_nRows && height >= 0);
    if (nRow < 0 || nRow >= m_nRows || height < 0)
        return FALSE;

    m_arRowHeights[nRow] = height;
    ResetScrollBars();

    return TRUE;
}

BOOL CGridCtrl::SetColumnWidth(int nCol, int width)
{
    ASSERT(nCol >= 0 && nCol < m_nCols && width >= 0);
    if (nCol < 0 || nCol >= m_nCols || width < 0)
        return FALSE;

    m_arColWidths[m_arColOrder[nCol]] = width;
    ResetScrollBars();

    return TRUE;
}

int CGridCtrl::GetFixedRowHeight() const
{
    int nHeight = 0;
    for (int i = 0; i < m_nFixedRows; i++)
        nHeight += GetRowHeight(i);

    return nHeight;
}

int CGridCtrl::GetFixedColumnWidth() const
{
    int nWidth = 0;
    for (int i = 0; i < m_nFixedCols; i++)
        nWidth += GetColumnWidth(i);

    return nWidth;
}

BOOL CGridCtrl::AutoSizeColumn(int nCol, UINT nAutoSizeStyle /*=GVS_DEFAULT*/, 
                               BOOL bResetScroll /*=TRUE*/)
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols)
        return FALSE;

    //  Skip hidden columns when autosizing
    if( GetColumnWidth( nCol) <=0 )
        return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC)
        return FALSE;

    int nWidth = 0;

    ASSERT(GVS_DEFAULT <= nAutoSizeStyle && nAutoSizeStyle <= GVS_BOTH);
    if (nAutoSizeStyle == GVS_DEFAULT)
        nAutoSizeStyle = GetAutoSizeStyle();

    int nStartRow = (nAutoSizeStyle & GVS_HEADER)? 0 : GetFixedRowCount();
    int nEndRow   = (nAutoSizeStyle & GVS_DATA)? GetRowCount()-1 : GetFixedRowCount()-1;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nStartRow, nCol, nEndRow, nCol));

    for (int nRow = nStartRow; nRow <= nEndRow; nRow++)
    {
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            size = pCell->GetCellExtent(pDC);
        if (size.cx > nWidth)
            nWidth = size.cx;
    }

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    m_arColWidths[nCol] = nWidth;

    ReleaseDC(pDC);
    if (bResetScroll)
        ResetScrollBars();

    return TRUE;
}

BOOL CGridCtrl::AutoSizeRow(int nRow, BOOL bResetScroll /*=TRUE*/)
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows)
        return FALSE;

    //  Skip hidden rows when autosizing
    if( GetRowHeight( nRow) <=0 )
        return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC)
        return FALSE;

    int nHeight = 0;
    int nNumColumns = GetColumnCount();

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nRow, 0, nRow, nNumColumns));

    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            size = pCell->GetCellExtent(pDC);
        if (size.cy > nHeight)
            nHeight = size.cy;
    }
    m_arRowHeights[nRow] = nHeight;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    ReleaseDC(pDC);
    if (bResetScroll)
        ResetScrollBars();

    return TRUE;
}

void CGridCtrl::AutoSizeColumns(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
    int nNumColumns = GetColumnCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {
        //  Skip hidden columns when autosizing
        if( GetColumnWidth( nCol) > 0 )
            AutoSizeColumn(nCol, nAutoSizeStyle, FALSE);
    }
    ResetScrollBars();
}

void CGridCtrl::AutoSizeRows()
{
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
    {
        //  Skip hidden rows when autosizing
        if( GetRowHeight( nRow) > 0 )
            AutoSizeRow(nRow, FALSE);
    }
    ResetScrollBars();
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns() and AutoSizeRows()
void CGridCtrl::AutoSize(UINT nAutoSizeStyle /*=GVS_DEFAULT*/)
{
    CDC* pDC = GetDC();
    if (!pDC)
        return;

    int nNumColumns = GetColumnCount();

    int nCol, nRow;

    ASSERT(GVS_DEFAULT <= nAutoSizeStyle && nAutoSizeStyle <= GVS_BOTH);
    if (nAutoSizeStyle == GVS_DEFAULT)
        nAutoSizeStyle = GetAutoSizeStyle();

    int nStartRow = (nAutoSizeStyle & GVS_HEADER)? 0 : GetFixedRowCount();
    int nEndRow   = (nAutoSizeStyle & GVS_DATA)? GetRowCount()-1 : GetFixedRowCount()-1;

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(nStartRow, 0, nEndRow, nNumColumns));

    // Row initialisation - only work on rows whose height is > 0
    for (nRow = nStartRow; nRow <= nEndRow; nRow++)
    {
        if( GetRowHeight( nRow) > 0 )
            m_arRowHeights[nRow] = 1;
    }

    CSize size;
    for (nCol = 0; nCol < nNumColumns; nCol++)
    {
        //  Don't size hidden columns or rows
        if( GetColumnWidth( nCol) > 0 )
        {
            // Skip columns that are hidden, but now initialize
            m_arColWidths[nCol] = 0;
            for (nRow = nStartRow; nRow <= nEndRow; nRow++)
            {
                if( GetRowHeight( nRow) > 0 )
                {
                    CGridCellBase* pCell = GetCell(nRow, nCol);
                    if (pCell)
                        size = pCell->GetCellExtent(pDC);
                    if (size.cx >(int) m_arColWidths[nCol])
                        m_arColWidths[nCol] = size.cx;
                    if (size.cy >(int) m_arRowHeights[nRow])
                        m_arRowHeights[nRow] = size.cy;
                }
            }
        }
    }

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    ReleaseDC(pDC);

    ResetScrollBars();
    Refresh();
}

// Expands the columns to fit the screen space. If bExpandFixed is FALSE then fixed 
// columns will not be affected
void CGridCtrl::ExpandColumnsToFit(BOOL bExpandFixed /*=TRUE*/)
{
    if (bExpandFixed)
	{
		if (GetColumnCount() <= 0) return;
	} 
	else
	{
		if (GetColumnCount() <= GetFixedColumnCount()) return;
	}

    EnableScrollBars(SB_HORZ, FALSE);

    int col;
    CRect rect;
    GetClientRect(rect);

    //int nFirstColumn = (bExpandFixed)? 0 : GetFixedColumnCount();
	int nFirstColumn = GetFixedColumnCount(); //固定列不需要扩展

    int nNumColumnsAffected = 0;
    for (col = nFirstColumn; col < GetColumnCount(); col++)
    {
        if (m_arColWidths[col] > 0)
            nNumColumnsAffected++;
    }

    if (nNumColumnsAffected <= 0)
        return;

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() -(int) virtualWidth;
    int nColumnAdjustment = nDifference / nNumColumnsAffected;

    for (col = nFirstColumn; col < GetColumnCount(); col++)
    {
        if (m_arColWidths[col] > 0)
            m_arColWidths[col] += nColumnAdjustment;    
    }

    if (nDifference > 0)
    {
        int leftOver = nDifference % nNumColumnsAffected;
        for (int nCount = 0, col = nFirstColumn; 
             (col < GetColumnCount()) && (nCount < leftOver); col++, nCount++)
        {
            if (m_arColWidths[col] > 0)
                m_arColWidths[col] += 1;
        }
    }
    else 
    {
        int leftOver = (-nDifference) % nNumColumnsAffected;
        for (int nCount = 0, col = nFirstColumn; 
             (col < GetColumnCount()) && (nCount < leftOver); col++, nCount++)
        {
            if (m_arColWidths[col] > 0)
                m_arColWidths[col] -= 1;
        }
    }

    Refresh();

    ResetScrollBars();
}

void CGridCtrl::ExpandLastColumn()
{
    if (GetColumnCount() <= 0)
        return;

    // Search for last non-hidden column
    int nLastColumn = GetColumnCount()-1;
    while (m_arColWidths[nLastColumn] <= 0)
        nLastColumn--;

    if (nLastColumn <= 0)
        return;

    EnableScrollBars(SB_HORZ, FALSE);

    CRect rect;
    GetClientRect(rect);

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() -(int) virtualWidth;

    if (nDifference > 0)
    {
        //if (GetVirtualHeight() > rect.Height())
        //    nDifference -= GetSystemMetrics(SM_CXVSCROLL);

        m_arColWidths[ nLastColumn ] += nDifference;
        Refresh();
    }

    ResetScrollBars();
}

// Expands the rows to fit the screen space. If bExpandFixed is FALSE then fixed 
// rows will not be affected
void CGridCtrl::ExpandRowsToFit(BOOL bExpandFixed /*=TRUE*/)
{
    if (bExpandFixed)
	{
		if (GetRowCount() <= 0) return;
	} 
	else
	{
		if (GetRowCount() <= GetFixedRowCount()) return;
	}

    EnableScrollBars(SB_VERT, FALSE); 

    int row;
    CRect rect;
    GetClientRect(rect);
    
    int nFirstRow = (bExpandFixed)? 0 : GetFixedRowCount();

    int nNumRowsAffected = 0;
    for (row = nFirstRow; row < GetRowCount(); row++)
    {
        if (m_arRowHeights[row] > 0)
            nNumRowsAffected++;
    }

    if (nNumRowsAffected <= 0)
        return;

    long virtualHeight = GetVirtualHeight();
    int nDifference = rect.Height() -(int) virtualHeight;
    int nRowAdjustment = nDifference / nNumRowsAffected;
    
    for (row = nFirstRow; row < GetRowCount(); row++)
    {
        if (m_arRowHeights[row] > 0)
            m_arRowHeights[row] += nRowAdjustment;    
    }
    
    if (nDifference > 0)
    {
        int leftOver = nDifference % nNumRowsAffected;
        for (int nCount = 0, row = nFirstRow; 
             (row < GetRowCount()) && (nCount < leftOver); row++, nCount++)
        {
            if (m_arRowHeights[row] > 0)
                m_arRowHeights[row] += 1;
        }
    } 
    else 
    {
        int leftOver = (-nDifference) % nNumRowsAffected;
        for (int nCount = 0, row = nFirstRow; 
             (row < GetRowCount()) && (nCount < leftOver); row++, nCount++)
        {
            if (m_arRowHeights[row] > 0)
                m_arRowHeights[row] -= 1;
        }
    }

    Refresh();

    ResetScrollBars();
}

// Expands the cells to fit the screen space. If bExpandFixed is FALSE then fixed 
// cells  will not be affected
void CGridCtrl::ExpandToFit(BOOL bExpandFixed /*=TRUE*/)
{
    ExpandColumnsToFit(bExpandFixed);   // This will remove any existing horz scrollbar
    ExpandRowsToFit(bExpandFixed);      // This will remove any existing vert scrollbar
    ExpandColumnsToFit(bExpandFixed);   // Just in case the first adjustment was with a vert
                                        // scrollbar in place
    Refresh();
}

/////////////////////////////////////////////////////////////////////////////////////
// Attributes

void CGridCtrl::SetVirtualMode(BOOL bVirtual)
{
    DeleteAllItems();
    m_bVirtualMode = bVirtual;

    // Force some defaults here.
    if (m_bVirtualMode)
    {
        SetEditable(FALSE);
        SetAutoSizeStyle(GVS_HEADER);
        SetFixedColumnSelection(FALSE);
        SetFixedRowSelection(FALSE);
		ResetVirtualOrder();
    }
}

void CGridCtrl::SetGridLines(int nWhichLines /*=GVL_BOTH*/) 
{
    m_nGridLines = nWhichLines;
    Refresh();
}

void CGridCtrl::SetListMode(BOOL bEnableListMode /*=TRUE*/)
{
    ResetSelectedRange();
    SetSortColumn(-1);
    m_bListMode = bEnableListMode;
    SetFixedRowSelection(FALSE);
    Refresh();
}

void CGridCtrl::SetSortColumn(int nCol)
{
    if (m_nSortColumn >= 0)
        InvalidateCellRect(0, m_nSortColumn);
    m_nSortColumn = nCol;
    if (nCol >= 0)
        InvalidateCellRect(0, nCol);
}

BOOL CGridCtrl::IsCellFixed(int nRow, int nCol)
{
    return (nRow < GetFixedRowCount() || nCol < GetFixedColumnCount());
}

void CGridCtrl::SetModified(BOOL bModified /*=TRUE*/, int nRow /*=-1*/, int nCol /*=-1*/)
{
    // Cannot guarantee sorting anymore...
    if (nCol < 0 || nCol == GetSortColumn())
        SetSortColumn(-1);

    if (nRow >= 0 && nCol >= 0)
    {
        if (bModified)
        {
            SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_MODIFIED);
            m_bModified = TRUE;
        }
        else
            SetItemState(nRow, nCol, GetItemState(nRow, nCol) & ~GVIS_MODIFIED);
    }
    else
        m_bModified = bModified;

    if (!m_bModified)
    {
        for (int row = 0; row < GetRowCount(); row++)
            for (int col = 0; col < GetColumnCount(); col++)
                SetItemState(row, col, GetItemState(row, col) & ~GVIS_MODIFIED);
    }
}

BOOL CGridCtrl::GetModified(int nRow /*=-1*/, int nCol /*=-1*/)
{
    if (nRow >= 0 && nCol >= 0)
        return ( (GetItemState(nRow, nCol) & GVIS_MODIFIED) == GVIS_MODIFIED );
    else
        return m_bModified;
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

void CGridCtrl::Refresh()
{
    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();
}

// EnsureVisible supplied by Roelf Werkman
void CGridCtrl::EnsureVisible(int nRow, int nCol)
{
    if (!m_bAllowDraw)
        return;

	CCellRange VisibleCells = GetVisibleNonFixedCellRange();
    int right = nCol - VisibleCells.GetMaxCol();
    int left  = VisibleCells.GetMinCol() - nCol;
    int down  = nRow - VisibleCells.GetMaxRow();
    int up    = VisibleCells.GetMinRow() - nRow;
	if(down<=0 && up<=0)	return;

    CRect rectWindow;
//	/*	    
    // set the scroll to the approximate position of row (Nigel Page-Jones)
	int nPos = (int)((float)nRow / GetRowCount() * 1000);
    float fPos = (float)nPos / 1000;
    SCROLLINFO scrollInfo;
    GetScrollInfo(SB_VERT, &scrollInfo);
    scrollInfo.nPos = (int)(scrollInfo.nMax * fPos);
    SetScrollInfo(SB_VERT, &scrollInfo, FALSE);
    
    GetClientRect(rectWindow);

    // redraw cells    if necessary (Nigel Page-Jones)
    CCellID idTopLeft = GetTopleftNonFixedCell(FALSE);
    CCellID idNewTopLeft = GetTopleftNonFixedCell(TRUE);
    if (idNewTopLeft != idTopLeft)
    {
        rectWindow.top = GetFixedRowHeight();
        InvalidateRect(rectWindow);
    }
//	*/
	//注：以下被我去掉了，因为太慢了，Modify By Liqs99
	/*
    // We are going to send some scroll messages, which will steal the focus 
    // from it's rightful owner. Squirrel it away ourselves so we can give
    // it back. (Damir)
    CWnd* pFocusWnd = GetFocus();

    CCellRange VisibleCells = GetVisibleNonFixedCellRange();

    int right = nCol - VisibleCells.GetMaxCol();
    int left  = VisibleCells.GetMinCol() - nCol;
    int down  = nRow - VisibleCells.GetMaxRow();
    int up    = VisibleCells.GetMinRow() - nRow;

    int iColumnStart;
    int iRowStart;

    iColumnStart = VisibleCells.GetMaxCol() + 1;
    while( right > 0 )
    {
        if( GetColumnWidth( iColumnStart ) > 0 )
            SendMessage( WM_HSCROLL, SB_LINERIGHT, 0 );

        right--;
        iColumnStart++;
    }

    iColumnStart = VisibleCells.GetMinCol() - 1;
    while( left > 0 )
    {
        if( GetColumnWidth( iColumnStart ) > 0 )
            SendMessage( WM_HSCROLL, SB_LINELEFT, 0 );
        left--;
        iColumnStart--;
    }

    iRowStart = VisibleCells.GetMaxRow() + 1;
    while( down > 0 )
    {
        if( GetRowHeight( iRowStart ) > 0 )
            SendMessage( WM_VSCROLL, SB_LINEDOWN, 0 );
        down--;
        iRowStart++;
    }

    iRowStart = VisibleCells.GetMinRow() - 1;
    while( up > 0 )
    {
        if( GetRowHeight( iRowStart ) > 0 )
            SendMessage( WM_VSCROLL, SB_LINEUP, 0 );
        up--;
        iRowStart--;
    }

    // Move one more if we only see a snall bit of the cell
    CRect rectCell;
    if (!GetCellRect(nRow, nCol, rectCell))
    {
		if (pFocusWnd && ::IsWindow(pFocusWnd->GetSafeHwnd()))
			pFocusWnd->SetFocus(); 
        return;
    }

    GetClientRect(rectWindow);

    // The previous fix was fixed properly by Martin Richter 
    while (rectCell.right > rectWindow.right
        && rectCell.left > GetFixedColumnWidth()
        && IsVisibleHScroll() // Junlin Xu: added to prevent infinite loop
        )
    {
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        if (!GetCellRect(nRow, nCol, rectCell))
        {
            pFocusWnd->SetFocus(); 
            return;
        }
    }

    while (rectCell.bottom > rectWindow.bottom
        && rectCell.top > GetFixedRowHeight()
        && IsVisibleVScroll() // Junlin Xu: added to prevent infinite loop
        )
    {
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        if (!GetCellRect(nRow, nCol, rectCell))
        {
            pFocusWnd->SetFocus(); 
            return;
        }
    }

    // restore focus to whoever owned it
    pFocusWnd->SetFocus(); 
*/	
}

BOOL CGridCtrl::IsCellEditable(CCellID &cell) const
{
    return IsCellEditable(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellEditable(int nRow, int nCol) const
{
    return IsEditable() && ((GetItemState(nRow, nCol) & GVIS_READONLY) != GVIS_READONLY);
}

BOOL CGridCtrl::IsCellSelected(CCellID &cell) const
{
    return IsCellSelected(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellSelected(int nRow, int nCol) const
{
    if (GetVirtualMode())
    {   
        if (!IsSelectable())
            return FALSE;

        CCellID cell;
        DWORD key = MAKELONG(nRow, nCol);

        return (m_SelectedCellMap.Lookup(key, (CCellID&)cell));       
    }
    else
        return IsSelectable() && ((GetItemState(nRow, nCol) & GVIS_SELECTED) == GVIS_SELECTED);
}

BOOL CGridCtrl::IsCellVisible(CCellID cell) 
{
    return IsCellVisible(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellVisible(int nRow, int nCol)
{
    if (!IsWindow(m_hWnd))
        return FALSE;

    int x, y;

    CCellID TopLeft;
    if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
    {
        TopLeft = GetTopleftNonFixedCell();
        if (nCol >= GetFixedColumnCount() && nCol < TopLeft.col)
            return FALSE;
        if (nRow >= GetFixedRowCount() && nRow < TopLeft.row)
            return FALSE;
    }
    
    CRect rect;
    GetClientRect(rect);
    if (nCol < GetFixedColumnCount())
    {
        x = 0;
        for (int i = 0; i <= nCol; i++) 
        {
            if (x >= rect.right)
                return FALSE;
            x += GetColumnWidth(i);    
        }
    } 
    else 
    {
        x = GetFixedColumnWidth();
        for (int i = TopLeft.col; i <= nCol; i++) 
        {
            if (x >= rect.right)
                return FALSE;
            x += GetColumnWidth(i);    
        }
    }
    
    if (nRow < GetFixedRowCount())
    {
        y = 0;
        for (int i = 0; i <= nRow; i++) 
        {
            if (y >= rect.bottom)
                return FALSE;
            y += GetRowHeight(i);    
        }
    } 
    else 
    {
        if (nRow < TopLeft.row)
            return FALSE;
        y = GetFixedRowHeight();
        for (int i = TopLeft.row; i <= nRow; i++) 
        {
            if (y >= rect.bottom)
                return FALSE;
            y += GetRowHeight(i);    
        }
    }
    
    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellID& cell)
{
    return InvalidateCellRect(cell.row, cell.col);
}

BOOL CGridCtrl::InvalidateCellRect(const int row, const int col)
{
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw)
        return FALSE;

    if (!IsValid(row, col))
        return FALSE;

    if (!IsCellVisible(row, col))
        return FALSE;

    CRect rect;
    if (!GetCellRect(row, col, rect))
        return FALSE;
    rect.right++;
    rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellRange& cellRange)
{
    ASSERT(IsValid(cellRange));
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw)
        return FALSE;

    CCellRange visibleCellRange = GetVisibleNonFixedCellRange().Intersect(cellRange);

    CRect rect;
    if (!GetCellRangeRect(visibleCellRange, rect))
        return FALSE;

    rect.right++;
    rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if this doesn't work for win95 then use OnRegisteredMouseWheel instead
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
BOOL CGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    if (m_nRowsPerWheelNotch == -1)
    {
        int nPagesScrolled = zDelta / 120;

        if (nPagesScrolled > 0)
		{
            for (int i = 0; i < nPagesScrolled; i++)
			{
                PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
			}
		}
        else
		{
            for (int i = 0; i > nPagesScrolled; i--)
			{
                PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
			}
		}
    }
    else
    {
        int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;

        if (nRowsScrolled > 0)
		{
            for (int i = 0; i < nRowsScrolled; i++)
			{
                PostMessage(WM_VSCROLL, SB_LINEUP, 0);
			}
		}
        else
		{
            for (int i = 0; i > nRowsScrolled; i--)
			{
                PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
			}
		}
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

void CGridCtrl::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
    CRect rect;
    GetClientRect(rect);

#ifndef GRIDCONTROL_NO_DRAGDROP
    // If outside client area, return (unless we are drag n dropping)
    if (m_MouseMode != MOUSE_DRAGGING && !rect.PtInRect(point))
        return;
#endif

    // Sometimes a MOUSEMOVE message can come after the left buttons
    // has been let go, but before the BUTTONUP message hs been processed.
    // We'll keep track of mouse buttons manually to avoid this.
    // All bMouseButtonDown's have been replaced with the member m_bLMouseButtonDown
    // BOOL bMouseButtonDown = ((nFlags & MK_LBUTTON) == MK_LBUTTON);

    // If the left mouse button is up, then test to see if row/column sizing is imminent
    if (!m_bLMouseButtonDown ||
        (m_bLMouseButtonDown && m_MouseMode == MOUSE_NOTHING))
    {
        if (m_bAllowColumnResize && MouseOverColumnResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
#endif
                m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (m_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
#endif
                m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        else if (m_MouseMode != MOUSE_NOTHING)
        {
#ifndef _WIN32_WCE_NO_CURSOR
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
            m_MouseMode = MOUSE_NOTHING;
        }

        if (m_MouseMode == MOUSE_NOTHING)
        {
            CGridCellBase* pCell = NULL;
            CCellID idCurrentCell;
            if (!GetVirtualMode() || m_bTitleTips)
            {
                // Let the cell know that a big fat cursor is currently hovering
                // over it.
                idCurrentCell = GetCellFromPt(point);
                pCell = GetCell(idCurrentCell.row, idCurrentCell.col);
                if (pCell)
                    pCell->OnMouseOver();
            }

#ifndef GRIDCONTROL_NO_TITLETIPS
            // Titletips anyone? anyone?
            if (m_bTitleTips)
            {
                CRect TextRect, CellRect;
                if (pCell)
                {
                    LPCTSTR szTipText = pCell->GetTipText();
                    if (!m_bRMouseButtonDown
						&& szTipText && szTipText[0]
                        && !pCell->IsEditing()
                        && GetCellRect( idCurrentCell.row, idCurrentCell.col, &TextRect)
                        && pCell->GetTipTextRect( &TextRect)
                        && GetCellRect(idCurrentCell.row, idCurrentCell.col, CellRect) )
                    {
//						TRACE0("Showing TitleTip\n");
						m_TitleTip.Show(TextRect, pCell->GetTipText(),  0, CellRect,
                                        pCell->GetFont(),  GetTitleTipTextClr(), GetTitleTipBackClr());
                    }
                }
            }
#endif
        }

        m_LastMousePoint = point;
        return;
    }

    if (!IsValid(m_LeftClickDownCell))
    {
        m_LastMousePoint = point;
        return;
    }

    // If the left mouse button is down, then process appropriately
    if (m_bLMouseButtonDown)
    {
        switch (m_MouseMode)
        {
        case MOUSE_SELECT_ALL:
            break;

        case MOUSE_SELECT_COL:
        case MOUSE_SELECT_ROW:
        case MOUSE_SELECT_CELLS:
            {
                CCellID idCurrentCell = GetCellFromPt(point);
                if (!IsValid(idCurrentCell))
                    return;

                if (idCurrentCell != GetFocusCell())
                {
                    OnSelecting(idCurrentCell);

                    // EFW - BUG FIX - Keep the appropriate cell row and/or
                    // column focused.  A fix in SetFocusCell() will place
                    // the cursor in a non-fixed cell as needed.
                    if((idCurrentCell.row >= m_nFixedRows &&
                      idCurrentCell.col >= m_nFixedCols) ||
                      m_MouseMode==MOUSE_SELECT_COL ||
                      m_MouseMode==MOUSE_SELECT_ROW)
                    {
                        SetFocusCell(idCurrentCell);
                    }
                }
                break;
            }

        case MOUSE_SIZING_COL:
            {
                CDC* pDC = GetDC();
                if (!pDC)
                    break;

                CRect oldInvertedRect(m_LastMousePoint.x, rect.top,
                    m_LastMousePoint.x + 2, rect.bottom);
                pDC->InvertRect(&oldInvertedRect);
                CRect newInvertedRect(point.x, rect.top, 
                    point.x + 2, rect.bottom);
                pDC->InvertRect(&newInvertedRect);
                ReleaseDC(pDC);
            }
            break;
            
        case MOUSE_SIZING_ROW:        
            {
                CDC* pDC = GetDC();
                if (!pDC)
                    break;
                
                CRect oldInvertedRect(rect.left, m_LastMousePoint.y,
                    rect.right, m_LastMousePoint.y + 2);
                pDC->InvertRect(&oldInvertedRect);
                CRect newInvertedRect(rect.left, point.y, 
                    rect.right, point.y + 2);
                pDC->InvertRect(&newInvertedRect);
                ReleaseDC(pDC);
            }
            break;
            
#ifndef GRIDCONTROL_NO_DRAGDROP
        case MOUSE_PREPARE_EDIT:
        case MOUSE_PREPARE_DRAG:
            m_MouseMode = MOUSE_PREPARE_DRAG;
            OnBeginDrag();    
            break;
#endif
        }    
    }

    m_LastMousePoint = point;
}

// Returns the point inside the cell that was clicked (coords relative to cell top left)
CPoint CGridCtrl::GetPointClicked(int nRow, int nCol, const CPoint& point)
{
    CPoint PointCellOrigin;
    if( !GetCellOrigin( nRow, nCol, &PointCellOrigin)  )
        return CPoint( 0, 0);

    CPoint PointClickedCellRelative( point);
    PointClickedCellRelative -= PointCellOrigin;
    return PointClickedCellRelative;
}

void CGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    TRACE0("CGridCtrl::OnLButtonDblClk\n");

    CCellID cell = GetCellFromPt(point);
    if( !IsValid( cell) )
    {
        //ASSERT(FALSE);
        return;
    }

#ifdef _WIN32_WCE
    if (MouseOverColumnResizeArea(point))
#else
    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE)
#endif
    {
        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start))
            return;

        if (point.x - start.x < m_nResizeCaptureRange)     // Clicked right of border
            cell.col--;

        //  ignore columns that are hidden and look left towards first visible column
        BOOL bFoundVisible = FALSE;
        while( cell.col >= 0)
        {
            if( GetColumnWidth( cell.col) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            cell.col--;
        }
        if( !bFoundVisible)
            return;

        AutoSizeColumn(cell.col, GetAutoSizeStyle());
        Invalidate();
    }
#ifdef _WIN32_WCE
    else if (MouseOverRowResizeArea(point))
#else
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
#endif
    {
        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start))
            return;

        if (point.y - start.y < m_nResizeCaptureRange)     // Clicked below border
            cell.row--;

        //  ignore rows that are hidden and look up towards first visible row
        BOOL bFoundVisible = FALSE;
        while( cell.row >= 0)
        {
            if( GetRowHeight( cell.row) > 0)
            {
                bFoundVisible = TRUE;
                break;
            }
            cell.row--;
        }
        if( !bFoundVisible)
            return;

        AutoSizeRow(cell.row);
        Invalidate();
    }
    else if (m_MouseMode == MOUSE_NOTHING)
    {
        CPoint pointClickedRel;
        pointClickedRel = GetPointClicked( cell.row, cell.col, point);

        CGridCellBase* pCell = NULL;
        if (IsValid(cell))
            pCell = GetCell(cell.row, cell.col);

        // Clicked in the text area? Only then will cell selection work
        BOOL bInTextArea = FALSE;
        if (pCell)
        {
            CRect rectCell;
            if (GetCellRect(cell.row, cell.col, rectCell) && pCell->GetTextRect(rectCell))
                bInTextArea = rectCell.PtInRect(point);
        }

        if (cell.row >= m_nFixedRows && IsValid(m_LeftClickDownCell) && 
            cell.col >= m_nFixedCols && bInTextArea)
        {
            OnEditCell(cell.row, cell.col, pointClickedRel, VK_LBUTTON);
        }
        else if (m_bListMode)
        {
            if (!IsValid(cell))
                return;
            if (cell.row >= m_nFixedRows && cell.col >= m_nFixedCols && bInTextArea)
                OnEditCell(cell.row, cell.col, pointClickedRel, VK_LBUTTON);
        }

        if (IsValid(cell))
        {
            CGridCellBase* pCell = GetCell(cell.row, cell.col);
            if (pCell)
                pCell->OnDblClick(pointClickedRel);
            SendMessageToParent(cell.row, cell.col, NM_DBLCLK);
        }
    }

    CWnd::OnLButtonDblClk(nFlags, point);
}

void CGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef GRIDCONTROL_USE_TITLETIPS
    // EFW - Bug Fix
    m_TitleTip.Hide();  // hide any titletips
#endif
    // TRACE0("CGridCtrl::OnLButtonDown\n");
    // CWnd::OnLButtonDown(nFlags, point);

    SetFocus();
	m_CurCol = -1;
    m_bLMouseButtonDown   = TRUE;
    m_LeftClickDownPoint = point;
    m_LeftClickDownCell  = GetCellFromPt(point);
    if (!IsValid(m_LeftClickDownCell))   
	{
		ResetSelectedRange();//不是单元格，清除选择

		if(GetSingleRowSelection())	return;
		//开始CRectTracker的工作
		m_MouseMode = MOUSE_TRACKER;
		Invalidate();
		m_RectTracker.TrackRubberBand(this,point,TRUE);
		m_RectTracker.m_rect.NormalizeRect();

		return;
	}
	m_CurRow = m_LeftClickDownCell.row;

    // If the SHIFT key is not down, then the start of the selection area should be the 
    // cell just clicked. Otherwise, keep the previous selection-start-cell so the user
    // can add to their previous cell selections in an intuitive way. If no selection-
    // start-cell has been specified, then set it's value here and now.
    if ((nFlags & MK_SHIFT) != MK_SHIFT)
        m_SelectionStartCell = m_LeftClickDownCell;
    else
    {
        if (!IsValid(m_SelectionStartCell))
            m_SelectionStartCell = m_idCurrentCell;
    }

    EndEditing();

    // tell the cell about it 
    CGridCellBase* pCell = GetCell(m_LeftClickDownCell.row, m_LeftClickDownCell.col);
    if (pCell)
        pCell->OnClickDown(GetPointClicked( m_LeftClickDownCell.row, m_LeftClickDownCell.col, point));

    // Clicked in the text area? Only then will cell selection work
    BOOL bInTextArea = FALSE;
    if (pCell)
    {
        CRect rectCell;
        if (GetCellRect(m_LeftClickDownCell.row, m_LeftClickDownCell.col, rectCell) &&
            pCell->GetTextRect(rectCell))
        {
            bInTextArea = rectCell.PtInRect(point);
        }
    }

    // If the user clicks on the current cell, then prepare to edit it.
    // (If the user moves the mouse, then dragging occurs)
	//如果点击当前选中格，准备编辑
	//如果按住左键盘移动鼠标，拖拉
    if (m_LeftClickDownCell == m_idCurrentCell && 
        !(nFlags & MK_CONTROL) && bInTextArea &&
        IsCellEditable(m_LeftClickDownCell))
    {
        m_MouseMode = MOUSE_PREPARE_EDIT;
        return;
    }
    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)
    else if (IsCellSelected(m_LeftClickDownCell))
    {
        SetFocusCell(m_LeftClickDownCell.row, m_LeftClickDownCell.col);

        // If control is pressed then unselect the cell or row (depending on the list mode)
        if (nFlags & MK_CONTROL)
        {
            SetFocusCell(m_LeftClickDownCell);
            if (GetListMode())
                SelectRows(m_LeftClickDownCell, TRUE, FALSE);
            else
                SelectCells(m_LeftClickDownCell, TRUE, FALSE);
            return;
        }
#ifndef GRIDCONTROL_NO_DRAGDROP
        else if (m_bAllowDragAndDrop)
            m_MouseMode = MOUSE_PREPARE_DRAG;
#endif
    }
    else if (m_MouseMode != MOUSE_OVER_COL_DIVIDE &&
             m_MouseMode != MOUSE_OVER_ROW_DIVIDE)        
    {
		if (m_LeftClickDownCell.row >= GetFixedRowCount() && 
			m_LeftClickDownCell.col >= GetFixedColumnCount())
// 		if(m_LeftClickDownCell.row >= GetFixedRowCount() &&
// 			m_LeftClickDownCell.col >= GetFixedColumnCount() &&		//Modify by liqs99
// 			!pCell->IsFixed())
		{
            SetFocusCell(m_LeftClickDownCell.row, m_LeftClickDownCell.col);
		}
		else
			SetFocusCell(-1, -1);

		//Don't set focus on any cell if the user clicked on a fixed cell - David Pritchard
        //if (GetRowCount() > GetFixedRowCount() && 
        //    GetColumnCount() > GetFixedColumnCount())
        //    SetFocusCell(max(m_LeftClickDownCell.row, m_nFixedRows),
        //                 max(m_LeftClickDownCell.col, m_nFixedCols));
    }
    
    SetCapture();
    
    if (m_MouseMode == MOUSE_NOTHING)
    {
        if (m_bAllowColumnResize && MouseOverColumnResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
#endif
                m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (m_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
#ifndef _WIN32_WCE_NO_CURSOR
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
#endif
                m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        // else if (m_MouseMode != MOUSE_NOTHING)
        //{
        //    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        //    m_MouseMode = MOUSE_NOTHING;
        //}
    }
    
    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
        m_MouseMode = MOUSE_SIZING_COL;

        // Kludge for if we are over the last column...
        if (GetColumnWidth(GetColumnCount()-1) < m_nResizeCaptureRange)
        {
            CRect VisRect;
            GetVisibleNonFixedCellRange(VisRect);
            if (abs(point.x - VisRect.right) < m_nResizeCaptureRange)
                m_LeftClickDownCell.col = GetColumnCount()-1;
        }

        CPoint start;
        if (!GetCellOrigin(0, m_LeftClickDownCell.col, &start))
            return;

        if( !m_bHiddenColUnhide)
        {
            //  ignore columns that are hidden and look left towards first visible column
            BOOL bLookForVisible = TRUE;
            BOOL bIsCellRightBorder = point.x - start.x >= m_nResizeCaptureRange;

            if( bIsCellRightBorder
                && m_LeftClickDownCell.col + 1 >= GetColumnCount() )
            {
                // clicked on last column's right border

                // if last column is visible, don't do anything
                if( m_LeftClickDownCell.col >= 0)
                    bLookForVisible = FALSE;
            }

            if( bLookForVisible)
            {
                // clicked on column divider other than last right border
                BOOL bFoundVisible = FALSE;
                int iOffset = 1;

                if( bIsCellRightBorder)
                    iOffset = 0;

                while( m_LeftClickDownCell.col - iOffset >= 0)
                {
                    if( GetColumnWidth( m_LeftClickDownCell.col - iOffset) > 0)
                    {
                        bFoundVisible = TRUE;
                        break;
                    }
                    m_LeftClickDownCell.col--;
                }
                if( !bFoundVisible)
                    return;
            }
        }


        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        // If we clicked to the right of the colimn divide, then reset the click-down cell
        // as the cell to the left of the column divide - UNLESS we clicked on the last column
        // and the last column is teensy (kludge fix)
        if (point.x - start.x < m_nResizeCaptureRange)
        {
            if (m_LeftClickDownCell.col < GetColumnCount()-1 || 
                GetColumnWidth(GetColumnCount()-1) >= m_nResizeCaptureRange)
            {
                if (!GetCellOrigin(0, --m_LeftClickDownCell.col, &start))
                    return;
            }
        }

		// Allow a cell resize width no greater than that which can be viewed within
		// the grid itself
		int nMaxCellWidth = rect.Width()-GetFixedColumnWidth();
        rect.left  = start.x + 1;
		rect.right = rect.left + nMaxCellWidth;

        ClientToScreen(rect);
#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
        m_MouseMode = MOUSE_SIZING_ROW;

        // Kludge for if we are over the last column...
        if (GetRowHeight(GetRowCount()-1) < m_nResizeCaptureRange)
        {
            CRect VisRect;
            GetVisibleNonFixedCellRange(VisRect);
            if (abs(point.y - VisRect.bottom) < m_nResizeCaptureRange)
                m_LeftClickDownCell.row = GetRowCount()-1;
        }

        CPoint start;
        if (!GetCellOrigin(m_LeftClickDownCell, &start))
            return;

        if( !m_bHiddenRowUnhide)
        {
            //  ignore rows that are hidden and look up towards first visible row
            BOOL bLookForVisible = TRUE;
            BOOL bIsCellBottomBorder = point.y - start.y >= m_nResizeCaptureRange;

            if( bIsCellBottomBorder
                && m_LeftClickDownCell.row + 1 >= GetRowCount() )
            {
                // clicked on last row's bottom border

                // if last row is visible, don't do anything
                if( m_LeftClickDownCell.row >= 0)
                    bLookForVisible = FALSE;
            }

            if( bLookForVisible)
            {
                // clicked on row divider other than last bottom border
                BOOL bFoundVisible = FALSE;
                int iOffset = 1;

                if( bIsCellBottomBorder)
                    iOffset = 0;

                while( m_LeftClickDownCell.row - iOffset >= 0)
                {
                    if( GetRowHeight( m_LeftClickDownCell.row - iOffset) > 0)
                    {
                        bFoundVisible = TRUE;
                        break;
                    }
                    m_LeftClickDownCell.row--;
                }
                if( !bFoundVisible)
                    return;
            }
        }

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, point.y, rect.right, point.y + 2);

        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        // If we clicked below the row divide, then reset the click-down cell
        // as the cell above the row divide - UNLESS we clicked on the last row
        // and the last row is teensy (kludge fix)
        if (point.y - start.y < m_nResizeCaptureRange)            // clicked below border
        {
            if (m_LeftClickDownCell.row < GetRowCount()-1 || 
                GetRowHeight(GetRowCount()-1) >= m_nResizeCaptureRange)
            {
                if (!GetCellOrigin(--m_LeftClickDownCell.row, 0, &start))
                    return;
            }
        }

		int nMaxCellHeight = rect.Height()-GetFixedRowHeight();
        rect.top = start.y + 1;
		rect.bottom = rect.top + nMaxCellHeight;

        ClientToScreen(rect);

#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else
#ifndef GRIDCONTROL_NO_DRAGDROP
    if (m_MouseMode != MOUSE_PREPARE_DRAG) // not sizing or editing -- selecting
#endif
    {
	    SendMessageToParent(m_LeftClickDownCell.row, m_LeftClickDownCell.col, GVN_SELCHANGING);

        // If Ctrl pressed, save the current cell selection. This will get added
        // to the new cell selection at the end of the cell selection process
        m_PrevSelectedCellMap.RemoveAll();
        if (nFlags & MK_CONTROL)
        {
            for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
            {
                DWORD key;
                CCellID cell;
                m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
                m_PrevSelectedCellMap.SetAt(key, cell);
            }
        }
        
        if (m_LeftClickDownCell.row < GetFixedRowCount())
		{
            OnFixedRowClick(m_LeftClickDownCell);
            if(m_AllowReorderColumn && m_LeftClickDownCell.col >=  GetFixedColumnCount())
			{
				ResetSelectedRange(); // TODO : This is not the better solution, as we do not see why clicking in column header should reset selection
											//but the state of selection is instable after drag (at least until someone debugs it), so better clear it allways.
				m_MouseMode = MOUSE_PREPARE_DRAG;
				m_CurCol = m_LeftClickDownCell.col;
			}
		}
        else if (m_LeftClickDownCell.col < GetFixedColumnCount())
            OnFixedColumnClick(m_LeftClickDownCell);
        else
        {
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            OnSelecting(m_LeftClickDownCell);

            m_nTimerID = SetTimer(WM_LBUTTONDOWN, m_nTimerInterval, 0);
        }
    }   
    m_LastMousePoint = point;
}

void CGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TRACE0("CGridCtrl::OnLButtonUp\n");
    CWnd::OnLButtonUp(nFlags, point);

//////////////////////////////////////////////////////////////////////////
	if(m_MouseMode == MOUSE_TRACKER)
	{
		m_MouseMode = MOUSE_NOTHING;
		Refresh();
	}
//////////////////////////////////////////////////////////////////////////

    m_bLMouseButtonDown = FALSE;

#ifndef _WIN32_WCE_NO_CURSOR
    ClipCursor(NULL);
#endif

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
    {
        ReleaseCapture();
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

    CPoint pointClickedRel;
    pointClickedRel = GetPointClicked( m_idCurrentCell.row, m_idCurrentCell.col, point);

    // m_MouseMode == MOUSE_PREPARE_EDIT only if user clicked down on current cell
    // and then didn't move mouse before clicking up (releasing button)
    if (m_MouseMode == MOUSE_PREPARE_EDIT)
    {
		//AfxMessageBox("开始编辑格子");
        OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, pointClickedRel, VK_LBUTTON);
    }
#ifndef GRIDCONTROL_NO_DRAGDROP
    // m_MouseMode == MOUSE_PREPARE_DRAG only if user clicked down on a selected cell
    // and then didn't move mouse before clicking up (releasing button)
    else if (m_MouseMode == MOUSE_PREPARE_DRAG) 
    {
        CGridCellBase* pCell = GetCell(m_idCurrentCell.row, m_idCurrentCell.col);
        if (pCell)
            pCell->OnClick( GetPointClicked( m_idCurrentCell.row, m_idCurrentCell.col, point) );
        SendMessageToParent(m_LeftClickDownCell.row, m_LeftClickDownCell.col, NM_CLICK);
	    SendMessageToParent(m_LeftClickDownCell.row, m_LeftClickDownCell.col, GVN_SELCHANGING);
        ResetSelectedRange();
	    SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED);
    }
#endif
    else if (m_MouseMode == MOUSE_SIZING_COL)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(m_LastMousePoint.x, rect.top, m_LastMousePoint.x + 2, rect.bottom);
        
        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }
        
        if (m_LeftClickDownPoint != point && (point.x != 0 || point.y != 0)) // 0 pt fix by email1@bierling.net
        {   
            CPoint start;
			//Used for merge cells
			//by Huang Wei
			m_LeftClickDownCell=GetMergeCellID(m_LeftClickDownCell);
			if (!GetCellOrigin(m_LeftClickDownCell, &start))
				return;

			int nColumnWidth = max(point.x - start.x, m_bAllowColHide? 0 : 1);
			//Used for merge cells
			//by Huang Wei
			int mergewidth=GetMergeCellWidth(m_LeftClickDownCell)-GetColumnWidth(m_LeftClickDownCell.col);
			SetColumnWidth(m_LeftClickDownCell.col, nColumnWidth-mergewidth);

			ResetScrollBars();
			Invalidate();
        }
    }
    else if (m_MouseMode == MOUSE_SIZING_ROW)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, m_LastMousePoint.y, rect.right, m_LastMousePoint.y + 2);

        CDC* pDC = GetDC();
        if (pDC)
        {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }
        
        if (m_LeftClickDownPoint != point  && (point.x != 0 || point.y != 0)) // 0 pt fix by email1@bierling.net
        {
            CPoint start;
			//Used for merge cells
			//by Huang Wei
			m_LeftClickDownCell=GetMergeCellID(m_LeftClickDownCell);
			if (!GetCellOrigin(m_LeftClickDownCell, &start))
				return;

			int nRowHeight = max(point.y - start.y, m_bAllowRowHide? 0 : 1);
			//Used for merge cells
			//by Huang Wei
			int mergeheight=GetMergeCellHeight(m_LeftClickDownCell)-GetRowHeight(m_LeftClickDownCell.row);

			SetRowHeight(m_LeftClickDownCell.row, nRowHeight-mergeheight);
			ResetScrollBars();
			Invalidate();
        }
    }
    else
    {
	    SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED);

        CGridCellBase* pCell = GetCell(m_idCurrentCell.row, m_idCurrentCell.col);
        if (pCell)
            pCell->OnClick( GetPointClicked( m_idCurrentCell.row, m_idCurrentCell.col, point) );
        SendMessageToParent(m_LeftClickDownCell.row, m_LeftClickDownCell.col, NM_CLICK);
    }
    
    m_MouseMode = MOUSE_NOTHING;
    
#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
    
    if (!IsValid(m_LeftClickDownCell))
        return;
    
    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED),
        (LPARAM) GetSafeHwnd());
}

#ifndef _WIN32_WCE
void CGridCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    CWnd::OnRButtonDown(nFlags, point);
	m_bRMouseButtonDown = TRUE;

	

#ifdef GRIDCONTROL_USE_TITLETIPS
	TRACE0("Hiding TitleTip\n");
    m_TitleTip.Hide();  // hide any titletips
#endif
}

// EFW - Added to forward right click to parent so that a context
// menu can be shown without deriving a new grid class.
void CGridCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnRButtonUp(nFlags, point);
	
	m_bRMouseButtonDown = FALSE;

    CCellID FocusCell;
    FocusCell = GetCellFromPt(point);

	

    EndEditing();        // Auto-destroy any InPlaceEdit's

    // If not a valid cell, pass -1 for row and column
    if(!IsValid(FocusCell))
	{
		ResetSelectedRange();
		SendMessageToParent(-1, -1, NM_RCLICK);
	}
    else
    {
        SetFocusCell(-1,-1);
        SetFocusCell(__max(FocusCell.row, m_nFixedRows),
            __max(FocusCell.col, m_nFixedCols));

        // tell the cell about it
        CGridCellBase* pCell = GetCell(FocusCell.row, FocusCell.col);
        if (pCell)
            pCell->OnRClick( GetPointClicked( FocusCell.row, FocusCell.col, point) );

        SendMessageToParent(FocusCell.row, FocusCell.col, NM_RCLICK);
    }
/*
	if(Cell(FocusCell.row, FocusCell.col).IsFixedRow())
	{	//如果右键点击固定行，将选中本列
	//选中本列
	//ResetSelectedRange();
	SetSelectedRange(0, FocusCell.col ,GetRowCount()-1, FocusCell.col, 
		TRUE, TRUE);
	}
*/
}
#endif

#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing

// EFW - New print margin support functions
void CGridCtrl::SetPrintMarginInfo(int nHeaderHeight, int nFooterHeight,
    int nLeftMargin, int nRightMargin, int nTopMargin,
    int nBottomMargin, int nGap)
{
    // If any parameter is -1, keep the existing setting
    if(nHeaderHeight > -1)
        m_nHeaderHeight = nHeaderHeight;
    if(nFooterHeight > -1)
        m_nFooterHeight = nFooterHeight;
    if(nLeftMargin > -1)
        m_nLeftMargin = nLeftMargin;
    if(nRightMargin > -1)
        m_nRightMargin = nRightMargin;
    if(nTopMargin > -1)
        m_nTopMargin = nTopMargin;
    if(nBottomMargin > -1)
        m_nBottomMargin = nBottomMargin;
    if(nGap > -1)
        m_nGap = nGap;
}

void CGridCtrl::GetPrintMarginInfo(int &nHeaderHeight, int &nFooterHeight,
    int &nLeftMargin, int &nRightMargin, int &nTopMargin,
    int &nBottomMargin, int &nGap)
{
    nHeaderHeight = m_nHeaderHeight;
    nFooterHeight = m_nFooterHeight;
    nLeftMargin = m_nLeftMargin;
    nRightMargin = m_nRightMargin;
    nTopMargin = m_nTopMargin;
    nBottomMargin = m_nBottomMargin;
    nGap = m_nGap;
}

void CGridCtrl::Print(CPrintDialog* pPrntDialog /*=NULL*/)
{
    CDC dc;

    if (pPrntDialog == NULL)
    {
        CPrintDialog printDlg(FALSE);
        if (printDlg.DoModal() != IDOK)             // Get printer settings from user
            return;

        dc.Attach(printDlg.GetPrinterDC());         // attach a printer DC
    }
    else
        dc.Attach(pPrntDialog->GetPrinterDC());     // attach a printer DC
        
    dc.m_bPrinting = TRUE;

    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);

    if( strTitle.IsEmpty() )
    {
        CWnd *pParentWnd = GetParent();
        while (pParentWnd)
        {
            pParentWnd->GetWindowText(strTitle);
            if (strTitle.GetLength())  // can happen if it is a CView, CChildFrm has the title
                break;
            pParentWnd = pParentWnd->GetParent();
        }
    }

    DOCINFO di;                                 // Initialise print doc details
    memset(&di, 0, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);        // Begin a new print job

    CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);                // Initialise printing
    for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
    {
        dc.StartPage();                         // begin new page
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);                    // Print page
        bPrintingOK = (dc.EndPage() > 0);       // end page
    }
    OnEndPrinting(&dc, &Info);                  // Clean up after printing

    if (bPrintingOK)
        dc.EndDoc();                            // end a print job
    else
        dc.AbortDoc();                          // abort job.

    dc.Detach();                                // detach the printer DC
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing overridables - for Doc/View print/print preview framework

// EFW - Various changes in the next few functions to support the
// new print margins and a few other adjustments.
void CGridCtrl::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    // OnBeginPrinting() is called after the user has committed to
    // printing by OK'ing the Print dialog, and after the framework
    // has created a CDC object for the printer or the preview view.

    // This is the right opportunity to set up the page range.
    // Given the CDC object, we can determine how many rows will
    // fit on a page, so we can in turn determine how many printed
    // pages represent the entire document.

    ASSERT(pDC && pInfo);
    if (!pDC || !pInfo) return;

    // Get a DC for the current window (will be a screen DC for print previewing)
    CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
    if (!pCurrentDC) return;

    CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
    CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));

    // Create the printer font
    int nFontSize = -10;
    CString strFontName = _T("Arial");
    m_PrinterFont.CreateFont(nFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
                             OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, strFontName);

    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Get the average character width (in GridCtrl units) and hence the margins
    m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
    m_CharSize.cx /= 52;
    int nMargins = (m_nLeftMargin+m_nRightMargin)*m_CharSize.cx;

    // Get the page sizes (physical and logical)
    m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));

    if( m_bWysiwygPrinting)
    {
        m_LogicalPageSize.cx = ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx * 3 / 4;
        m_LogicalPageSize.cy = ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy * 3 / 4;
    }
    else
    {
        m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));

        m_LogicalPageSize.cx = GetVirtualWidth()+nMargins;
#ifdef _WIN32_WCE
        m_LogicalPageSize.cy = (m_LogicalPageSize.cx * m_PaperSize.cy) / m_PaperSize.cx;
#else
        m_LogicalPageSize.cy = MulDiv(m_LogicalPageSize.cx, m_PaperSize.cy, m_PaperSize.cx);
#endif
    }

    m_nPageHeight = m_LogicalPageSize.cy - GetFixedRowHeight()
                       - (m_nHeaderHeight+m_nFooterHeight + 2*m_nGap)*m_CharSize.cy;

    // Get the number of pages. Assumes no row is bigger than the page size.
    int nTotalRowHeight = 0;
    m_nNumPages = 1;
    for (int row = GetFixedRowCount(); row < GetRowCount(); row++)
    {
        nTotalRowHeight += GetRowHeight(row);
        if (nTotalRowHeight > m_nPageHeight) {
            m_nNumPages++;
            nTotalRowHeight = GetRowHeight(row);
        }
    }

    // now, figure out how many additional pages must print out if rows ARE bigger
    //  than page size
    int iColumnOffset = 0;
    int i1;
    for( i1=0; i1 < GetFixedColumnCount(); i1++)
    {
        iColumnOffset += GetColumnWidth( i1);
    }
    m_nPageWidth = m_LogicalPageSize.cx - iColumnOffset
                    - nMargins;
    m_nPageMultiplier = 1;

    if( m_bWysiwygPrinting)
    {
        int iTotalRowWidth = 0;
        for( i1 = GetFixedColumnCount(); i1 < GetColumnCount(); i1++)
        {
            iTotalRowWidth += GetColumnWidth( i1);
            if( iTotalRowWidth > m_nPageWidth)
            {
                m_nPageMultiplier++;
                iTotalRowWidth = GetColumnWidth( i1);
            }
        }
        m_nNumPages *= m_nPageMultiplier;
    }

    // Set up the print info
    pInfo->SetMaxPage(m_nNumPages);
    pInfo->m_nCurPage = 1;                        // start printing at page# 1

    ReleaseDC(pCurrentDC);
    pDC->SelectObject(pOldFont);
}

void CGridCtrl::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
    if (!pDC || !pInfo)
        return;

    //CRect rcPage(pInfo->m_rectDraw);
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Set the page map mode to use GridCtrl units, and setup margin
    pDC->SetMapMode(MM_ANISOTROPIC);	//映射模式
    pDC->SetWindowExt(m_LogicalPageSize);
    pDC->SetViewportExt(m_PaperSize);
    pDC->SetWindowOrg(-m_nLeftMargin * m_CharSize.cx, 0);

    // Header
    pInfo->m_rectDraw.top    = 0;
    pInfo->m_rectDraw.left   = 0;
    pInfo->m_rectDraw.right  = m_LogicalPageSize.cx - (m_nLeftMargin + m_nRightMargin) * m_CharSize.cx;
    pInfo->m_rectDraw.bottom = m_nHeaderHeight * m_CharSize.cy;
    PrintHeader(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -m_nHeaderHeight * m_CharSize.cy);

    // Gap between header and column headings
    pDC->OffsetWindowOrg(0, -m_nGap * m_CharSize.cy);

    pDC->OffsetWindowOrg(0, -GetFixedRowHeight());

    // We need to find out which row to start printing for this page.
    int nTotalRowHeight = 0;
    UINT nNumPages = 1;
    m_nCurrPrintRow = GetFixedRowCount();


    // Not only the row, but we need to figure out column, too

    // Can print 4 pages, where page 1 and 2 represent the same rows but
    // with different WIDE columns.
    //
    // .......
    // .1 .2 .  If representing page 3  -->    iPageIfIgnoredWideCols = 2
    // .......                                 iWideColPageOffset = 0
    // .3 .4 .  If representing page 2  -->    iPageIfIgnoredWideCols = 1
    // .......                                 iWideColPageOffset = 1

    int iPageIfIgnoredWideCols = pInfo->m_nCurPage / m_nPageMultiplier;
    int iWideColPageOffset = pInfo->m_nCurPage - ( iPageIfIgnoredWideCols * m_nPageMultiplier);
    if( iWideColPageOffset > 0)
        iPageIfIgnoredWideCols++;

    if( iWideColPageOffset == 0)
        iWideColPageOffset = m_nPageMultiplier;
    iWideColPageOffset--;

    // calculate current print row based on iPageIfIgnoredWideCols
    while(  m_nCurrPrintRow < GetRowCount()
            && (int)nNumPages < iPageIfIgnoredWideCols)
    {
        nTotalRowHeight += GetRowHeight(m_nCurrPrintRow);
        if (nTotalRowHeight > m_nPageHeight) {
            nNumPages++;
            if ((int)nNumPages == iPageIfIgnoredWideCols) break;
            nTotalRowHeight = GetRowHeight(m_nCurrPrintRow);
        }
        m_nCurrPrintRow++;
    }

    m_nPrintColumn = GetFixedColumnCount();
    int iTotalRowWidth = 0;
    int i1, i2;

    // now, calculate which print column to start displaying
    for( i1 = 0; i1 < iWideColPageOffset; i1++)
    {
        for( i2 = m_nPrintColumn; i2 < GetColumnCount(); i2++)
        {
            iTotalRowWidth += GetColumnWidth( i2);
            if( iTotalRowWidth > m_nPageWidth)
            {
                m_nPrintColumn = i2;
                iTotalRowWidth = 0;
                break;
            }
        }
    }


    PrintRowButtons( pDC, pInfo);   // print row buttons on each page
    int iColumnOffset = 0;
    for( i1=0; i1 < GetFixedColumnCount(); i1++)
    {
        iColumnOffset += GetColumnWidth( i1);
    }

    // Print the column headings
    pInfo->m_rectDraw.bottom = GetFixedRowHeight();

    if( m_nPrintColumn == GetFixedColumnCount())
    {
        // have the column headings fcn draw the upper left fixed cells
        //  for the very first columns, only
        pDC->OffsetWindowOrg( 0, +GetFixedRowHeight());

        m_nPageWidth += iColumnOffset;
        m_nPrintColumn = 0;
        PrintColumnHeadings(pDC, pInfo);
        m_nPageWidth -= iColumnOffset;
        m_nPrintColumn = GetFixedColumnCount();

        pDC->OffsetWindowOrg( -iColumnOffset, -GetFixedRowHeight());
    }
    else
    {
        // changed all of this here to match above almost exactly same
        pDC->OffsetWindowOrg( 0, +GetFixedRowHeight());

        m_nPageWidth += iColumnOffset;

        // print from column 0 ... last column that fits on the current page
        PrintColumnHeadings(pDC, pInfo);
        
        m_nPageWidth -= iColumnOffset;
 
        pDC->OffsetWindowOrg( -iColumnOffset, -GetFixedRowHeight());
    }


    if (m_nCurrPrintRow >= GetRowCount()) return;

    // Draw as many rows as will fit on the printed page.
    // Clip the printed page so that there is no partially shown
    // row at the bottom of the page (the same row which will be fully
    // shown at the top of the next page).

    BOOL bFirstPrintedRow = TRUE;
    CRect rect;
    rect.bottom = -1;

	//Used for merge cells
	//by Huang Wei
	int Row=m_nCurrPrintRow;
	CRect range;
	range.bottom = -1;
	while (Row < GetRowCount())
	{
		range.top = range.bottom+1;
		range.bottom = range.top + GetRowHeight(Row) - 1;

		if (range.bottom > m_nPageHeight) 
		{
			range.bottom=range.top;
			break;            
		}
		Row++;
	}

    while (m_nCurrPrintRow < GetRowCount())
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(m_nCurrPrintRow) - 1;

        if (rect.bottom > m_nPageHeight) break;            // Gone past end of page

        rect.right = -1;

        // modified to allow printing of wide grids on multiple pages
        for (int col = m_nPrintColumn; col < GetColumnCount(); col++)
        {
            rect.left = rect.right+1;
            rect.right =    rect.left
                            + GetColumnWidth( col)
                            - 1;

            if( rect.right > m_nPageWidth)
                break;

            CGridCellBase* pCell = GetCell(m_nCurrPrintRow, col);
            if (pCell)
			{
				//Used for merge cells
				//by Huang Wei
				int row=m_nCurrPrintRow;
				if(!pCell->IsMerged())
				{
					if(!pCell->IsMergeWithOthers())
					{
						pCell->PrintCell(pDC, row, col, rect);
					}
				}
				else
				{
					CRect mergerect=rect;
					if(GetCellRangeRect(pCell->m_MergeRange,&mergerect))
					{
						//mergerect.DeflateRect(0,0,1,1);
						mergerect.OffsetRect(rect.TopLeft()-mergerect.TopLeft());
						pCell->PrintCell(pDC, row, col, mergerect);
					}
				}
			}
			/*
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
            {
                int Overlap = (col == 0)? 0:1;
                pDC->MoveTo(rect.left-Overlap, rect.bottom);
                pDC->LineTo(rect.right, rect.bottom);
                if (m_nCurrPrintRow == 0) {
                    pDC->MoveTo(rect.left-Overlap, rect.top);
                    pDC->LineTo(rect.right, rect.top);
                }
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT)
            {
                int Overlap = (bFirstPrintedRow)? 0:1;
                pDC->MoveTo(rect.right, rect.top-Overlap);
                pDC->LineTo(rect.right, rect.bottom);
                if (col == 0) {
                    pDC->MoveTo(rect.left, rect.top-Overlap);
                    pDC->LineTo(rect.left, rect.bottom);
                }
            }
			*/
        }
        m_nCurrPrintRow++;
        bFirstPrintedRow = FALSE;
    }

	//Used for merge cells
	//by Huang Wei
	CRect white_rect(CPoint(0,0),m_LogicalPageSize);
	white_rect.top=range.bottom+1;
	pDC->FillSolidRect(white_rect,RGB(255,255,255));

    // Footer
    pInfo->m_rectDraw.bottom = m_nFooterHeight * m_CharSize.cy;
    pDC->SetWindowOrg( -m_nLeftMargin * m_CharSize.cx,
        -m_LogicalPageSize.cy + m_nFooterHeight * m_CharSize.cy);
    PrintFooter(pDC, pInfo);

    // SetWindowOrg back for next page
    pDC->SetWindowOrg(0,0);

    pDC->SelectObject(pOldFont);
}


// added by M.Fletcher 12/17/00
void CGridCtrl::PrintFixedRowCells(int nStartColumn, int nStopColumn, int& row, CRect& rect,
                                   CDC *pDC, BOOL& bFirst)
{
   // print all cells from nStartColumn to nStopColumn on row
   for (int col =nStartColumn; col < nStopColumn; col++)
   {
      rect.left = rect.right+1;
      rect.right =    rect.left + GetColumnWidth( col) - 1;

      if( rect.right > m_nPageWidth)
         break;
      
      CGridCellBase* pCell = GetCell(row, col);
      if (pCell)
	  {
		  //Used for merge cells
		  //by Huang Wei
		  int row=m_nCurrPrintRow;
		  if(!pCell->IsMerged())
		  {
			  if(!pCell->IsMergeWithOthers())
			  {
				  pCell->PrintCell(pDC, row, col, rect);
			  }
		  }
		  else
		  {
			  CRect mergerect=rect;
			  if(GetCellRangeRect(pCell->m_MergeRange,&mergerect))
			  {
				  //mergerect.DeflateRect(0,0,1,1);
				  mergerect.OffsetRect(rect.TopLeft()-mergerect.TopLeft());
				  pCell->PrintCell(pDC, row, col, mergerect);
			  }
		  }
	  }
      /*
      if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
      {
         int Overlap = (col == 0)? 0:1;
         
         pDC->MoveTo(rect.left-Overlap, rect.bottom);
         pDC->LineTo(rect.right, rect.bottom);
         
         if (row == 0)
         {
            pDC->MoveTo(rect.left-Overlap, rect.top);
            pDC->LineTo(rect.right, rect.top);
          }
      }

      if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT)
      {
         int Overlap = (row == 0)? 0:1;

         pDC->MoveTo(rect.right, rect.top-Overlap);
         pDC->LineTo(rect.right, rect.bottom);
         
         if( bFirst)
         {
            pDC->MoveTo(rect.left-1, rect.top-Overlap);
            pDC->LineTo(rect.left-1, rect.bottom);
            bFirst = FALSE;
          }
		 
       }*/

   } // end of column cells loop


} // end of CGridCtrl::PrintFixedRowCells

void CGridCtrl::PrintColumnHeadings(CDC *pDC, CPrintInfo* /*pInfo*/)
{
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    CRect rect;
    rect.bottom = -1;

    BOOL bFirst = TRUE;
    BOOL bOriginal;
    

    // modified to allow column hdr printing of multi-page wide grids
    for (int row = 0; row < GetFixedRowCount(); row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row) - 1;

        rect.right = -1;
 
        // if printColumn > fixedcolumncount we are on page 2 or more
        // lets printout those fixed cell headings again the 1 or more that would be missed
        // added by M.Fletcher 12/17/00
        if(m_nPrintColumn>= GetFixedColumnCount())
        {
           bOriginal=bFirst;
           // lets print the missing fixed cells on left first out to last fixed column
           PrintFixedRowCells(0,GetFixedColumnCount(), row, rect, pDC, bFirst);
           bFirst=bOriginal;
        }

        // now back to normal business print cells in heading after all fixed columns
        PrintFixedRowCells(m_nPrintColumn, GetColumnCount(), row, rect, pDC, bFirst);
        
    } // end of Row Loop

    pDC->SelectObject(pOldFont);
} // end of CGridCtrl::PrintColumnHeadings



/*****************************************************************************
Prints line of row buttons on each page of the printout.  Assumes that
the window origin is setup before calling

*****************************************************************************/
void CGridCtrl::PrintRowButtons(CDC *pDC, CPrintInfo* /*pInfo*/)
{
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    CRect rect;
    rect.right = -1;

    BOOL bFirst = TRUE;
    for( int iCol = 0; iCol < GetFixedColumnCount(); iCol++)
    {
        rect.left = rect.right+1;
        rect.right =    rect.left
                        + GetColumnWidth( iCol)
                        - 1;

        rect.bottom = -1;
        for( int iRow = m_nCurrPrintRow; iRow < GetRowCount(); iRow++)
        {
            rect.top = rect.bottom+1;
            rect.bottom = rect.top + GetRowHeight( iRow) - 1;

            if( rect.bottom > m_nPageHeight)
                break;

            CGridCellBase* pCell = GetCell(iRow, iCol);
            if (pCell)
                pCell->PrintCell(pDC, iRow, iCol, rect);

            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
            {
                int Overlap = (iCol == 0)? 0:1;
                pDC->MoveTo(rect.left-Overlap, rect.bottom);
                pDC->LineTo(rect.right, rect.bottom);
                if( bFirst) {
                    pDC->MoveTo(rect.left-Overlap, rect.top-1);
                    pDC->LineTo(rect.right, rect.top-1);
                    bFirst = FALSE;
                }
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT)
            {
                int Overlap = (iRow == 0)? 0:1;
                pDC->MoveTo(rect.right, rect.top-Overlap);
                pDC->LineTo(rect.right, rect.bottom);
                if (iCol == 0) {
                    pDC->MoveTo(rect.left, rect.top-Overlap);
                    pDC->LineTo(rect.left, rect.bottom);
                }
            }

        }
    }
    pDC->SelectObject(pOldFont);
}

void CGridCtrl::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
    // print App title on top right margin
    CString strRight;
	if(m_bPrintRightTitle)
	{
		strRight = m_strRightTitle;
		//strRight.LoadString(AFX_IDS_APP_TITLE);
		if(strRight.IsEmpty())
			strRight =  AfxGetApp()->m_pszAppName;
	}

    // print parent window title in the centre (Gert Rijs)
    CString strCenter;
	if(m_bPrintCenterTitle)
    {
		strCenter = m_strCenterTitle;
		if(strCenter.IsEmpty())
		{
			CWnd *pParentWnd = GetParent();
			while (pParentWnd)
			{
				pParentWnd->GetWindowText(strCenter);
				if (strCenter.GetLength())  // can happen if it is a CView, CChildFrm has the title
					break;
				pParentWnd = pParentWnd->GetParent();
			}
		}
	}

	CString strLeftSecondTile;
	if(m_bPrintLeftSecondTile)
	{
		strLeftSecondTile = m_strLeftSecondTile;
	}

    CFont   BoldFont;
    LOGFONT lf;

    //create bold font for header and footer
    VERIFY(m_PrinterFont.GetLogFont(&lf));
    lf.lfWeight = FW_BOLD;
    VERIFY(BoldFont.CreateFontIndirect(&lf));

    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    CRect   rc(pInfo->m_rectDraw);
	if( !strCenter.IsEmpty() )
		pDC->DrawText( strCenter, &rc, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
	if( !strRight.IsEmpty() )
		pDC->DrawText( strRight, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
	
	if(m_bPrintLeftSecondTile)
	{
		CRect xrc;
		xrc.left = rc.left;
		xrc.right = rc.right;
		xrc.top = rc.top+rc.top/2;
		xrc.bottom = rc.bottom;
		pDC->DrawText(strLeftSecondTile, &rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_BOTTOM);

	}
    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();

    // draw ruled-line across top
    pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(rc.left, rc.bottom);
    pDC->LineTo(rc.right, rc.bottom);
}

//print footer with a line and date, and page number
void CGridCtrl::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
    // page numbering on left
    CString strLeft;
    //strLeft.Format(_T("Page %d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage() );
	strLeft.Format(_T("page %d   total %d"), pInfo->m_nCurPage, pInfo->GetMaxPage() );

    // date and time on the right
    CString strRight;
    COleDateTime t = COleDateTime::GetCurrentTime();
    //strRight = t.Format(_T("%c"));
	strRight = t.Format(_T("%Y-%m-%d %H:%M:%S"));
    
    CRect rc(pInfo->m_rectDraw);

    // draw ruled line on bottom
    pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(rc.left, rc.top);
    pDC->LineTo(rc.right, rc.top);

    CFont BoldFont;
    LOGFONT lf;

    //create bold font for header and footer
    m_PrinterFont.GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;
    BoldFont.CreateFontIndirect(&lf);

    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // EFW - Bug fix - Force text color to black.  It doesn't always
    // get set to a printable color when it gets here.
    pDC->SetTextColor(RGB(0, 0, 0));

    if( !strLeft.IsEmpty() )
        pDC->DrawText( strLeft, &rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
    if( !strRight.IsEmpty() )
        pDC->DrawText( strRight, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();
}

void CGridCtrl::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    m_PrinterFont.DeleteObject();
}

#endif  // !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)


#ifndef _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// CGridCtrl persistance

BOOL CGridCtrl::Save(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
    CStdioFile File;
    CFileException ex;
	CString strSeparator(chSeparator);

    if (!File.Open(filename, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex))
    {
        ex.ReportError();
        return FALSE;
    }

    TRY
    {
        int nNumColumns = GetColumnCount();
        for (int i = 0; i < GetRowCount(); i++)
        {
            for (int j = 0; j < nNumColumns; j++)
            {
                File.WriteString(GetItemText(i,j));
                File.WriteString((j==(nNumColumns-1))? _T("\n"): strSeparator);
            }
        }

        File.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to save grid list"));
        return FALSE;
    }
    END_CATCH

    return TRUE;
}

BOOL CGridCtrl::Load(LPCTSTR filename, TCHAR chSeparator/*=_T(',')*/)
{
    if (GetVirtualMode())
        return FALSE;

    TCHAR *token, *end;
    TCHAR buffer[1024];
    CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeRead | CFile::typeText))
    {
        ex.ReportError();
        return FALSE;
    }

    DeleteAllItems();

    TRY
    {
        // Read Header off file
        File.ReadString(buffer, 1024);

        // Get first token
        for (token=buffer, end=buffer;
             *end && (*end != chSeparator) && (*end != _T('\n')); 
             end++)
            ;

        if ((*end == _T('\0')) && (token == end))
            token = NULL;

        *end = _T('\0');

        while (token)
        {
            InsertColumn(token);

            // Get next token
            for (token=++end; *end && (*end != chSeparator) && (*end != _T('\n'));
               end++)
                ;

            if ((*end == _T('\0')) && (token == end))
                token = NULL;

            *end = _T('\0');
        }

        // Read in rest of data
        int nItem = 1;
        while (File.ReadString(buffer, 1024))
        {
            // Get first token
            for (token=buffer, end=buffer;
              *end && (*end != chSeparator) && (*end != _T('\n')); end++)
                ;

            if ((*end == _T('\0')) && (token == end))
                token = NULL;

            *end = _T('\0');

            int nSubItem = 0;
            while (token)
            {
                if (!nSubItem)
                    InsertRow(token);
                else
                    SetItemText(nItem, nSubItem, token);

                // Get next token
                for (token=++end; *end && (*end != chSeparator) && (*end != _T('\n'));
                  end++)
                    ;

                if ((*end == _T('\0')) && (token == end))
                    token = NULL;

                *end = _T('\0');

                nSubItem++;
            }
            nItem++;
        }

        AutoSizeColumns(GetAutoSizeStyle());
        File.Close();
    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to load grid data"));
        return FALSE;
    }
    END_CATCH

    return TRUE;
}

BOOL CGridCtrl::SaveXml(CString &file, BOOL bMemory)
{
	return TRUE;
	/*
	int rowCount = GetRowCount();
	int colCount = GetColumnCount();

	pugi::xml_document xml;
	pugi::xml_node nodeGrid = xml.root().append_child(_T("MFCGridCtrl"));
	nodeGrid.append_attribute(_T("RowCount")).set_value(rowCount);
	nodeGrid.append_attribute(_T("ColCount")).set_value(colCount);

	BOOL bSaveColWidth = TRUE;
	for (int row=0; row<rowCount; row++)
	{
		pugi::xml_node nodeRow = nodeGrid.append_child(_T("Row"));
		nodeRow.append_attribute(_T("No")).set_value(row);
		nodeRow.append_attribute(_T("Height")).set_value(GetRowHeight(row));

		for (int col=0; col<colCount; col++)
		{
			pugi::xml_node nodeCol = nodeRow.append_child(_T("Col"));
			nodeCol.append_attribute(_T("No")).set_value(col);

			if(bSaveColWidth) nodeCol.append_attribute(_T("Width")).set_value(GetColumnWidth(col));

			if(Cell(row,col).IsMerged())
			{
				nodeCol.append_attribute("Merged").set_value(1);
				nodeCol.append_attribute("MergeBeginRow").set_value(Cell(row,col).m_MergeRange.GetMinRow());
				nodeCol.append_attribute("MergeBeginCol").set_value(Cell(row,col).m_MergeRange.GetMinCol());
				nodeCol.append_attribute("MergeEndRow").set_value(Cell(row,col).m_MergeRange.GetMaxRow());
				nodeCol.append_attribute("MergeEndCol").set_value(Cell(row,col).m_MergeRange.GetMaxCol());
			}
			else if(Cell(row,col).IsMergeWithOthers())
			{
				nodeCol.append_attribute("MergeWithOthers").set_value(1);
				continue;
			}

			nodeCol.append_attribute("Align").set_value((int)Cell(row,col).GetFormat());

			if(Cell(row,col).GetTextClr() != CLR_DEFAULT)
			{
				nodeCol.append_attribute("Color").set_value((int)Cell(row,col).GetTextClr());
			}

			nodeCol.text().set(Cell(row, col).GetText());
		}

		bSaveColWidth = FALSE;
	}

	if(bMemory)
	{
		pugi::xml_writer_string w;
		xml.print(w);
		file = w.strText.c_str();
		return TRUE;
	}

	return xml.save_file(file);
	*/
}

BOOL CGridCtrl::LoadXml(LPCTSTR file, BOOL bMemory)
{
/*
	CString temp;
	pugi::xml_document xml;
	if(bMemory)
	{
		if(!xml.load_string(file))	return FALSE;
	}
	else
	{
		if(!xml.load_file(file))	return FALSE;
	}

	pugi::xml_node nodeGrid = xml.root().child(_T("MFCGridCtrl"));
	if(!nodeGrid)	return FALSE;

	int rowCount = nodeGrid.attribute("RowCount").as_int();
	int colCount = nodeGrid.attribute("ColCount").as_int();

	SetRowCount(rowCount);
	SetColumnCount(colCount);

	for (pugi::xml_node nodeRow=nodeGrid.first_child(); nodeRow; nodeRow=nodeRow.next_sibling())
	{
		int row = nodeRow.attribute("No").as_int();
		if(nodeRow.attribute("Height"))
			SetRowHeight(row, nodeRow.attribute("Height").as_int());

		for (pugi::xml_node nodeCol=nodeRow.first_child(); nodeCol; nodeCol=nodeCol.next_sibling())
		{
			int col = nodeCol.attribute("No").as_int();
			if(nodeCol.attribute("Width"))
			{
				SetColumnWidth(col, nodeCol.attribute("Width").as_int());
			}

			BOOL MergeWithOthers = nodeCol.attribute("MergeWithOthers").as_int(0);
			if(MergeWithOthers)	continue;

			BOOL Merged = nodeCol.attribute("Merged").as_int(0);
			if(Merged)
			{
				int MergeBeginRow = nodeCol.attribute("MergeBeginRow").as_int();
				int MergeBeginCol = nodeCol.attribute("MergeBeginCol").as_int();
				int MergeEndRow = nodeCol.attribute("MergeEndRow").as_int();
				int MergeEndCol = nodeCol.attribute("MergeEndCol").as_int();
				MergeCells(MergeBeginRow, MergeBeginCol, MergeEndRow, MergeEndCol);
			}

			int format;
			if(nodeCol.attribute("Align"))
			{
				format = nodeCol.attribute("Align").as_int();
				Cell(row,col).SetFormat(format);
			}

			int TextClr;
			if(nodeCol.attribute("Color"))
			{
				TextClr = nodeCol.attribute("Color").as_int();
				Cell(row,col).SetTextClr(TextClr);
			}

			Cell(row,col).SetText(nodeCol.text().as_string());
		}
	}
*/
	return TRUE;
}

#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl overrideables

#ifndef GRIDCONTROL_NO_DRAGDROP
// This is no longer needed since I've changed to OLE drag and drop - but it's
// still cool code. :)
CImageList* CGridCtrl::CreateDragImage(CPoint *pHotSpot)
{
    CDC* pDC = GetDC();
    if (!pDC)
        return NULL;

    CRect rect;
    CCellID cell = GetFocusCell();
    if (!GetCellRect(cell.row, cell.col, rect))
        return NULL;
    
    // Translate coordinate system
    rect.BottomRight() = CPoint(rect.Width(), rect.Height());
    rect.TopLeft()     = CPoint(0, 0);
    *pHotSpot = rect.BottomRight(); 
    
    // Create a new imagelist (the caller of this function has responsibility
    // for deleting this list)
    CImageList* pList = new CImageList;
    if (!pList || !pList->Create(rect.Width(), rect.Height(), ILC_MASK, 1, 1))
    {    
        if (pList)
            delete pList;
        return NULL;
    }
    
    // Create mem DC and bitmap
    CDC MemDC;
    CBitmap bm;
    MemDC.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = MemDC.SelectObject(&bm);
    MemDC.SetWindowOrg(0, 0);
    
    // Draw cell onto bitmap in memDC
    CGridCellBase* pCell = GetCell(cell.row, cell.col);
    if (pCell)
        pCell->Draw(&MemDC, cell.row, cell.col, rect, FALSE);
    
    // Clean up
    MemDC.SelectObject(pOldBitmap);
    ReleaseDC(pDC);
    
    // Add the bitmap we just drew to the image list.
    pList->Add(&bm, GetDefaultCell(FALSE, FALSE)->GetBackClr());
    bm.DeleteObject();

    return pList;
}
#endif

void CGridCtrl::OnFixedRowClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;

    if (GetHeaderSort())
    {
        //CWaitCursor waiter;
        if (cell.col == GetSortColumn())
            SortItems(cell.col, !GetSortAscending());
        else
            SortItems(cell.col, TRUE);
        Invalidate();
    }

	// Did the user click on a fixed column cell (so the cell was within the overlap of
	// fixed row and column cells) - (fix by David Pritchard)
    if (GetFixedColumnSelection())
    {
        if (cell.col < GetFixedColumnCount())
        {
            m_MouseMode = MOUSE_SELECT_ALL;
            OnSelecting(cell);
        }
        else 
        {
            m_MouseMode = MOUSE_SELECT_COL;
            OnSelecting(cell);
        }
    }
}

void CGridCtrl::OnFixedColumnClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;

//    if (m_bListMode && (GetItemState(cell.row, m_nFixedCols) & GVNI_SELECTED))
//    {
//        OnEditCell(cell.row, cell.col, VK_LBUTTON);
//        return;
//    }

	// Did the user click on a fixed row cell (so the cell was within the overlap of
	// fixed row and column cells) - (fix by David Pritchard)
    if (GetFixedRowSelection())
    {
        if (cell.row < GetFixedRowCount())
        {
            m_MouseMode = MOUSE_SELECT_ALL;
            OnSelecting(cell);
        }
        else
        {
            m_MouseMode = MOUSE_SELECT_ROW;
            OnSelecting(cell);
        }
    }
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default this uses the selected font (which is a bigger font)
CSize CGridCtrl::GetTextExtent(int nRow, int nCol, LPCTSTR str)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return CSize(0, 0);
    else
        return pCell->GetTextExtent(str);
}

// virtual
void CGridCtrl::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
#ifndef GRIDCONTROL_NO_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif

    // Can we do it?
    CCellID cell(nRow, nCol);
    if (!IsValid(cell) || !IsCellEditable(nRow, nCol))
	{
		return;
	}

    // Can we see what we are doing?
    EnsureVisible(nRow, nCol);
    if (!IsCellVisible(nRow, nCol))
        return;

    // Where, exactly, are we gonna do it??
    CRect rect;
    if (!GetCellRect(cell, rect))
        return;

    // Check we can edit...
    if (SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT) >= 0)
    {
        // Let's do it...
        CGridCellBase* pCell = GetCell(nRow, nCol);
        if (pCell)
            pCell->Edit(nRow, nCol, rect, point, IDC_INPLACE_CONTROL, nChar);
		//AfxMessageBox("编辑完毕");
    }
}

// virtual
void CGridCtrl::EndEditing()
{
    CCellID cell = GetFocusCell();
    if (!IsValid(cell)) return;
    CGridCellBase *pCell = GetCell(cell.row, cell.col);
    if (pCell)
        pCell->EndEdit();
}

// virtual
void CGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
    CString strCurrentText = GetItemText(nRow, nCol);
    if (strCurrentText != str)
    {
        SetItemText(nRow, nCol, str);
        if (ValidateEdit(nRow, nCol, str) && 
            SendMessageToParent(nRow, nCol, GVN_ENDLABELEDIT) >= 0)
        {
            SetModified(TRUE, nRow, nCol);
            RedrawCell(nRow, nCol);
        }
        else
        {
            SetItemText(nRow, nCol, strCurrentText);
        }
    }

    CGridCellBase* pCell = GetCell(nRow, nCol);
    if (pCell)
        pCell->OnEndEdit();
}

// If this returns FALSE then the editing isn't allowed
// virtual
BOOL CGridCtrl::ValidateEdit(int nRow, int nCol, LPCTSTR str)
{
    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return TRUE;

    return pCell->ValidateEdit(str);
}

// virtual
CString CGridCtrl::GetItemText(int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
        return _T("");

    CGridCellBase* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return _T("");

    return pCell->GetText();
}

void CGridCtrl::ResetVirtualOrder()
{
	m_arRowOrder.resize(m_nRows);  
    for (int i = 0; i < m_nRows; i++)
	{
		m_arRowOrder[i] = i;	
	}
}


void CGridCtrl::Reorder(int From, int To)
{
		// Set line From just after Line To
	ASSERT(From>= GetFixedRowCount() && To>=GetFixedRowCount()-1 && From<m_nRows && To<m_nRows);
	int Value = m_arRowOrder[From];
	m_arRowOrder.erase(m_arRowOrder.begin()+From);
	int Offset = (From>=To ? 1:0);
	m_arRowOrder.insert(m_arRowOrder.begin()+To+Offset, Value);

}

//////////////////////////////////////////////////////////////////////////
//Merge the selected cells 
//by Huang Wei
CCellID CGridCtrl::GetMergeCellID(CCellID cell)
{
	CGridCellBase *pCell = (CGridCellBase*) GetCell(cell);
	if(pCell && pCell->IsMergeWithOthers())
		return pCell->GetMergeCellID();
	return cell;
}
//Merge the selected cells 
//by Huang Wei
CGridCellBase* CGridCtrl::GetCell(CCellID cell)
{
	return GetCell(cell.row,cell.col);
}
//Merge the selected cells 
//by Huang Wei
void CGridCtrl::MergeSelectedCells()
{
	CCellRange range=GetSelectedCellRange();
	if(range.GetColSpan()<=1 && range.GetRowSpan()<=1)
		return;
	MergeCells(range.GetMinRow(), range.GetMinCol(), range.GetMaxRow(), range.GetMaxCol());
}
//Merge the selected cells 
//by Huang Wei
void CGridCtrl::MergeCells(int nStartRow, int nStartCol, int nEndRow, int nEndCol)
{
	for(int row=nStartRow;row<=nEndRow;row++)
	{
		for(int col=nStartCol;col<=nEndCol;col++)
		{
			CGridCellBase *pCell = (CGridCellBase*) GetCell(row,col);
			pCell->Show(false);
			if(row==nStartRow && col==nStartCol)
			{
				CCellRange range(nStartRow,  nStartCol,  nEndRow,  nEndCol);
				pCell->SetMergeRange(range);
			}
			else
			{
				CCellID cell(nStartRow,nStartCol);
				pCell->SetMergeCellID(cell);
			}

		}
	}
	Invalidate();
}
//Merge the selected cells 
//by Huang Wei
void CGridCtrl::UnMergeSelectedCells()
{
	CCellRange range=GetSelectedCellRange();
	UnMergeCells(range.GetMinRow(), range.GetMinCol(), range.GetMaxRow(), range.GetMaxCol());
}
//Merge the selected cells 
//by Huang Wei
void CGridCtrl::UnMergeCells(int nStartRow, int nStartCol, int nEndRow, int nEndCol)
{
	for(int row=nStartRow;row<=nEndRow;row++)
	{
		for(int col=nStartCol;col<=nEndCol;col++)
		{
			CGridCellBase *pCell = (CGridCellBase*) GetCell(row,col);
			if(pCell->IsMerged())
			{
				for(int mergerow=pCell->GetMergeRange().GetMaxRow();mergerow>=pCell->GetMergeRange().GetMinRow();mergerow--)
					for(int mergecol=pCell->GetMergeRange().GetMaxCol();mergecol>=pCell->GetMergeRange().GetMinCol();mergecol--)
					{
						if(pCell->GetMergeRange().GetMaxRow()==-1 || pCell->GetMergeRange().GetMaxCol()==-1)
							break;
						CGridCellBase *pMergedCell = (CGridCellBase*) GetCell(mergerow,mergecol);
						pMergedCell->UnMerge();
					}

			}
			else
				pCell->UnMerge();
		}
	}
	Invalidate();
}
//Merge the selected cells 
//by Huang Wei
int  CGridCtrl::GetMergeCellWidth(CCellID cell)
{
	CCellID mergecell =GetMergeCellID(cell);
	CGridCellBase *pCell = (CGridCellBase*) GetCell(mergecell);
	if(!pCell->IsMerged())
		return GetColumnWidth(cell.col);
	int width=0;
	for(int mergecol=pCell->GetMergeRange().GetMaxCol();mergecol>=pCell->GetMergeRange().GetMinCol();mergecol--)
	{
		width+=GetColumnWidth(mergecol);
	}
	return width;
}
//Merge the selected cells 
//by Huang Wei
int  CGridCtrl::GetMergeCellHeight(CCellID cell)
{
	CCellID mergecell =GetMergeCellID(cell);
	CGridCellBase *pCell = (CGridCellBase*) GetCell(mergecell);
	if(!pCell->IsMerged())
		return GetRowHeight(cell.row);
	int height=0;
	for(int mergerow=pCell->GetMergeRange().GetMaxRow();mergerow>=pCell->GetMergeRange().GetMinRow();mergerow--)
	{
		height+=GetRowHeight(mergerow);
	}
	return height;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//以下是我自己添加的函数 Added By Liqs99

/*----------------------
	导出Excel
	列标题不能为空，左固定列不导出，并且假设左固定列是1;
----------------------*/
BOOL CGridCtrl::ExportToExcel(CString FileName, BOOL bShowFileDlg)
{
	CString strFileName;

	if(bShowFileDlg)//显示文件选择对话框
	{
		static TCHAR BASED_CODE szFilter[] = _T("Excel(*.xls)|*.xls|All Files (*.*)|*.*||");
		CFileDialog fileDlg(FALSE,_T("xls"),FileName,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return FALSE;
		strFileName = fileDlg.GetPathName();
	}
	else	//不显示文件选择对话框,此时传送的文件名必须包含路径
		strFileName = FileName;

	//如果存在，删除之
	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	if(bFind)	::DeleteFile(strFileName);

	CExcelFile f;
	if(!f.CreateFile())	return FALSE;
	f.LoadSheet(1);

	int rowcount = GetRowCount();
	int colcount = GetColumnCount();
	CString cellText;
	int xlsRow=1;
	int xlsCol=1;
	for (int row=0; row<rowcount; row++)
	{
		xlsCol=1;
		for (int col=0; col<colcount; col++)
		{
			CGridCell *pCell = (CGridCell *)GetCell(row,col);

			if(row==0 && col==0)
				f.SetColumnWidth(xlsCol, GetColumnWidth(col));
			if(col==0)
				f.SetRowHeight(xlsRow, GetRowHeight(row));

			if(pCell->IsMergeWithOthers())	
			{
				xlsCol++;
				continue;
			}

			CExcelRangeEx rg;

			if(pCell->IsMerged())
			{
				CCellRange cr = pCell->GetMergeRange();
				rg.SetRange(f._sheet, cr.GetMinRow(), cr.GetMinCol(), cr.GetMaxRow(), cr.GetMaxCol());
				rg.Merge();
			}
			else
				rg.SetRange(f._sheet, row, col, row, col);

			if(pCell != NULL && pCell->IsKindOf(cellCheckBox))
			{
				CGridCellCheck *pCellCheck = (CGridCellCheck *)GetCell(row, col);
				if(pCellCheck->GetCheck())
					cellText = "1";
				else cellText = "0";
			}
			else 
				cellText = GetItemText(row, col);

			f.SetCell(xlsRow, xlsCol, cellText);
			rg.SetBorder();
			rg.SetFormat(pCell->GetFormat());

			COLORREF clr = pCell->GetBackClr();
			if(clr == CLR_DEFAULT)
			{
				if(pCell->IsFixed())	clr = GetFixedBkColor();
				else clr = GetTextBkColor();
			}
			rg.SetBackColor(clr);

			xlsCol++;
		}
		xlsRow++;
	}

	f.SaveAs(strFileName);

	CString temp;
	temp.Format(_T("Exporting excel successfully, file's directory is : %s"), strFileName);
	AfxMessageBox(temp);

	return TRUE;
}

/*----------------------
	导出Html网页
	列标题不能为空，左固定列不导出，并且假设左固定列1个;
----------------------*/
BOOL CGridCtrl::ExportToHtml(CString FileName, BOOL bShowFileDlg)
{
	static TCHAR BASED_CODE szFilter[] = _T("HTML(*.html)|*.html|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE,_T("html"),FileName,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	if(fileDlg.DoModal() != IDOK)	return FALSE;

	CString strFileName = fileDlg.GetPathName();
	CString strFileTitle = fileDlg.GetFileTitle();

	//如果存在，删除之
	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	if(bFind)	::DeleteFile(strFileName);

	//创建文件
	CStdioFile htmlFile;
	CFileException mExcept;

	if(!htmlFile.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite, &mExcept))
		return FALSE;

	//开始写文件
	CString temp;
	temp = _T("<meta http-equiv=Content-Type content=text/html; charset=gb2312 />");
	htmlFile.WriteString(_T("<html>\n"));
	htmlFile.WriteString(_T("<head>\n"));
	htmlFile.WriteString(temp+_T("\n"));
	htmlFile.WriteString(_T("<title>"));
	htmlFile.WriteString(strFileTitle);
	htmlFile.WriteString(_T("</title>\n"));
	htmlFile.WriteString(_T("<style type=text/css>"));
	htmlFile.WriteString(_T("	<!--"));
	htmlFile.WriteString(_T("	.STYLE2 {"));
	htmlFile.WriteString(_T("	font-family: \"宋体\";"));
	htmlFile.WriteString(_T("	font-size: 12px;"));
	htmlFile.WriteString(_T("}"));
	htmlFile.WriteString(_T("-->"));
	htmlFile.WriteString(_T("</style>"));
	htmlFile.WriteString(_T("</head>\n"));
	htmlFile.WriteString(_T("<Body>\n"));
	htmlFile.WriteString(_T("<table border=1 align=left cellpadding=0 cellspacing=0>\n"));

	int nRow = GetRowCount();
	int nCol = GetColumnCount();

	for (int i=0; i<nRow; i++)
	{
		if(GetCell(i,0)->IsFixedRow())	//固定行加背景色
			htmlFile.WriteString(_T("<tr class=\"STYLE2\" valign=middle  height=25 bgcolor=#CCCCCC>\n"));
		else htmlFile.WriteString(_T("<tr class=\"STYLE2\" valign=middle  height=24>\n"));
		for (int j=0; j<nCol; j++)
		{
			if(GetCell(i,j)->IsFixedCol())	continue;	//不写固定列

			temp.Format(_T("<td width=%d>"), GetItemText(i,j).GetLength()*8);
			htmlFile.WriteString(temp);

			if(!GetItemText(i,j).IsEmpty())	
				htmlFile.WriteString(GetItemText(i,j));
			else htmlFile.WriteString(_T("&nbsp;"));

			htmlFile.WriteString(_T("</td>\n"));
		}
		htmlFile.WriteString(_T("</tr>\n"));
	}

	htmlFile.WriteString(_T("</table>\n"));
	//htmlFile.WriteString("<br><p align=left>Design By liqs99</p>\n");
	htmlFile.WriteString(_T("</Body>\n"));
	htmlFile.WriteString(_T("</html>\n"));

	htmlFile.Close();
	return TRUE;
}

//是否只选中一行
BOOL CGridCtrl::IsSelectedSingleRow()
{
	//如果不是列模式
	if(!m_bListMode)	return TRUE;

	int row = -1;
	for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
	{
		DWORD key;
		CCellID cell;
		m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

		if(row!=-1 && row!=cell.row)
		{
			return FALSE;
		}

		row = cell.row;
	}

	return row != -1;
}

BOOL CGridCtrl::GetSelectedRows(CUIntArray &arRow)
{
	arRow.RemoveAll();

	//如果不是列模式
	if(!m_bListMode)	return FALSE;

	for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
	{
		DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
		
		//检查是否行号已经保存
		int j=0;
		for(j=0; j<arRow.GetSize(); j++)
		{
			if(arRow.GetAt(j) == cell.row)	break;
		}
		if(j == arRow.GetSize())	//没有保存
		{
			arRow.Add(cell.row);
		}
	}

	//行号从小到大排序
	int temp;
	INT_PTR size = arRow.GetSize();
	for (INT_PTR i=0; i<size-1; i++)
	{
		for (int j=1; j<size-i; j++)
		{
			if(arRow[j-1]>arRow[j])
			{
				temp = arRow[j-1];
				arRow[j-1] = arRow[j];
				arRow[j] = temp;
			}
		}
	}
		
	return TRUE;
}

int CGridCtrl::GetSelectedRow()
{
	//如果不是列模式
	if(!m_bListMode)	return -1;
	
	CUIntArray ar;
	GetSelectedRows(ar);
	if(ar.GetSize() == 0)	return -1;

	return ar.GetAt(0);	
}

//是否选中行
BOOL CGridCtrl::IsRowSelected(int row)
{
	if(!m_bListMode)	return FALSE;
	if(row <= 0)	return FALSE;
	return IsCellSelected(row,1);
}

GRID_ROW &CGridCtrl::Row(int nRow)
{
	//GRID_ROW* pRow = m_RowData[nRow];
	//return *pRow;
	return *m_RowData[nRow];
}

void CGridCtrl::FormatReport()
{
	SetFixedColumnSelection(FALSE);	//不允许选中固定列时选中整列
	SetFixedRowSelection(TRUE);		//允许选中固定行时选中整行
	SetFrameFocusCell(FALSE);	//选中单元格不显示外边框
	//SetTrackFocusCell(FALSE); //同行/列中的固定单元格作为焦点单元格时是否高亮显示并且使用凹陷边缘
	//EnableHiddenColUnhide(TRUE);
	//EnableColumnHide(TRUE);
	SetListMode(TRUE);			//列表模式
	SetViewFixedColNo();
	SetEditable(FALSE);			//不能编辑
	SetSingleRowSelection();
	ExpandColumnsToFit(TRUE);	//扩展最后一列到控件宽度

	SetDefCellMargin(0);	//边距；单元格文字到单元格边框的大小
	//m_cellDefault.SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);//单元格对齐方式
	m_cellFixedRowDef.SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);//

	SetRowCount(1);
	SetColumnCount(1);
	SetFixedRowCount(1);
	SetFixedColumnCount(1);
	SetColumnWidth(0, 30);	//左固定列宽20	
}

//返回单元格引用
CGridCell & CGridCtrl::Cell(int nRow, int nCol)
{
	return *(CGridCell *)GetCell(nRow, nCol);
	//return *cellBase;
}

//返回默认单元格引用
CGridDefaultCell & CGridCtrl::CellDef()
{
	return m_cellDefault;
}
CGridDefaultCell & CGridCtrl::CellFixedRowDef()
{
	return m_cellFixedRowDef;
}
CGridDefaultCell & CGridCtrl::CellFixedColDef()
{
	return m_cellFixedColDef;
}
CGridDefaultCell & CGridCtrl::CellFixedRowColDef()
{
	return m_cellFixedRowColDef;
}

//是否重画表格
void CGridCtrl::SetAutoReDraw(BOOL bReDraw)
{
	m_bAllowDraw = bReDraw;
	ResetScrollBars();
}
BOOL CGridCtrl::GetAutoDraw()
{
	return m_bAllowDraw;
}

//返回ComboBox引用类
CGridCellCombo &CGridCtrl::ComboBox(int nRow, int nCol)
{
	//CRuntimeClass *pRuntimeClass = get
	ASSERT(Cell(nRow, nCol).IsKindOf(RUNTIME_CLASS(CGridCellCombo)));

	return *(CGridCellCombo *)GetCell(nRow, nCol);
}

//返回CheckBox引用类
CGridCellCheck &CGridCtrl::CheckBox(int nRow, int nCol)
{
	ASSERT(Cell(nRow, nCol).IsKindOf(RUNTIME_CLASS(CGridCellCheck)));
	return *(CGridCellCheck *)GetCell(nRow, nCol);
}

CGridCellDateTime &CGridCtrl::DateTime(int nRow, int nCol)
{
	ASSERT(Cell(nRow, nCol).IsKindOf(RUNTIME_CLASS(CGridCellDateTime)));
	return *(CGridCellDateTime *)GetCell(nRow, nCol);
}

//选中一行，使获得焦点，确保该行可见
BOOL CGridCtrl::SelectRow(int row,
						  int col,
						  BOOL bForceRedraw /*=FALSE*/, BOOL bSelectCells /*=TRUE*/)
{
	if (!m_bEnableSelection)
        return FALSE;
	
    //if (currentCell.row; == m_idCurrentCell.row) return;
    if (row < m_nFixedRows)
        return	FALSE;
	int Left = ( m_AllowSelectRowInFixedCol ? 0 : GetFixedColumnCount());

     SetSelectedRange(row, Left ,row, GetColumnCount()-1, 
 		bForceRedraw, bSelectCells);
	
// 	if(col == -1)
// 		SetFocusCell(row,Left);
// 	else SetFocusCell(row, col);
	EnsureVisible(row,Left);
	Refresh();

	return TRUE;
}

//设置整行的文字颜色
void CGridCtrl::SetRowTextColor(int row, COLORREF clr)
{
	int nfixed = GetFixedColumnCount();
	int colcount = GetColumnCount();
	for (int i=0; i<colcount; i++)
	{
		if(i >= nfixed)
			Cell(row,i).SetTextClr(clr);
	}
}

//选择所有
BOOL CGridCtrl::SelectRowsAll()
{
	SelectAllCells();
	SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED);
	return TRUE;
}

//反向选择
BOOL CGridCtrl::SelectRowsInvert()
{
	for (int row=1; row<GetRowCount(); row++)
	{
		for (int col=1; col<GetColumnCount(); col++)
		{
			UINT state = GetItemState(row, col);
			if(state & GVIS_SELECTED)
				SetItemState(row, col, state&~GVIS_SELECTED);
			else
				SetItemState(row, col, state|GVIS_SELECTED);
		}
	}
	Refresh();
	SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED);
// 	SetAutoReDraw(FALSE);
// 	for (int row=1; row<GetRowCount(); row++)
// 	{
// 		m_PrevSelectedCellMap.RemoveAll();
// 		for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
// 		{
// 			DWORD key;
// 			CCellID cell;
// 			m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
// 			m_PrevSelectedCellMap.SetAt(key, cell);
// 		}
// 		CCellID c(row,1);
// 		m_SelectionStartCell = c;
// 		if(IsCellSelected(row,1))
// 			SelectRows(c, TRUE, FALSE);
// 		else
// 			SelectRows(c, TRUE, TRUE);
// 	}
// 	SetAutoReDraw(TRUE);
// 	Refresh();
 	return TRUE;
}

//取消所有选中行
void CGridCtrl::ClearSelectedRows()
{
	ResetSelectedRange();
}

//设置打印表格中间标题
void CGridCtrl::SetPrintCenterTitle(LPCTSTR str, BOOL bPrint)
{
	m_bPrintCenterTitle = bPrint;
	m_strCenterTitle = str;
}

//设置打印表格右标题
void CGridCtrl::SetPrintRightTitle(LPCTSTR str, BOOL bPrint)
{
	m_bPrintRightTitle = bPrint;
	m_strRightTitle = str;
}

//设置打印表格左边第二副标题
void CGridCtrl::SetPrintLeftSecondTile(LPCTSTR str, BOOL bPrint)
{
	m_bPrintLeftSecondTile = bPrint;
	m_strLeftSecondTile = str;	
}

void CGridCtrl::SetStatRow(int row, int nStat)
{
	int colCount = GetColumnCount();
	for (int col=0; col<colCount; col++)
	{
		Cell(row,col).SetStatCell(nStat);
	}
// 	if(nStat == 0)
// 	{
// 		MergeCells(row,1,row,GetColumnCount()-1);
// 	}
	m_nStatRows++;
}

void CGridTracker::DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// first, normalize the rectangle for drawing
	CRect rect = *lpRect;
	rect.NormalizeRect();

	// convert to client coordinates
	if (pWndClipTo != NULL)
	{
		pWnd->ClientToScreen(&rect);
		pWndClipTo->ScreenToClient(&rect);
	}

	CSize size(0, 0);
	if (!m_bFinalErase)
	{
		// otherwise, size depends on the style
		if (m_nStyle & hatchedBorder)
		{
			size.cx = size.cy = max(1, GetHandleSize(rect)-1);
			rect.InflateRect(size);
		}
		else
		{
			size.cx = 1;
			size.cy = 1;
		}
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
	{
		CBrush brush(pGrid->GetHighLightBkColor());
		pDC->DrawDragRect(rect, size, m_rectLast, m_sizeLast, &brush, &brush);
	}

	// remember last rectangles
	m_rectLast = rect;
	m_sizeLast = size;
}

void CGridTracker::OnChangedRect(const CRect& rectOld)
{
	CRect rectTracker;// = m_rectLast;
	GetTrueRect(&rectTracker);

	if(rectTracker.IsRectEmpty())	
	{
		pGrid->SendMessageToParent(pGrid->m_idCurrentCell.row, pGrid->m_idCurrentCell.col, GVN_SELCHANGED);
		return;
	}

	CCellID cellend  = pGrid->GetCellFromPt(rectTracker.TopLeft(), FALSE);
	if(!cellend.IsValid()) 
	{
		CRect rcCell;
		if(pGrid->GetCellRect(pGrid->GetRowCount()-1, 1, rcCell))
		{
			if(rectTracker.top > rcCell.top)
			{
				pGrid->ResetSelectedRange();
				pGrid->SendMessageToParent(pGrid->m_idCurrentCell.row, pGrid->m_idCurrentCell.col, GVN_SELCHANGED);
				return;
			}
			else
			{
				pGrid->SelectRowsAll();
				pGrid->SendMessageToParent(pGrid->m_idCurrentCell.row, pGrid->m_idCurrentCell.col, GVN_SELCHANGED);
				return;
			}
		}
		else
		{
			pGrid->ResetSelectedRange();
			pGrid->SendMessageToParent(pGrid->m_idCurrentCell.row, pGrid->m_idCurrentCell.col, GVN_SELCHANGED);
			return;
		}
	}

	pGrid->m_SelectionStartCell = CCellID(pGrid->GetRowCount()-1,1);
	pGrid->SelectRows(cellend, TRUE, TRUE);
	pGrid->SendMessageToParent(pGrid->m_idCurrentCell.row, pGrid->m_idCurrentCell.col, GVN_SELCHANGED);
}

BOOL CGridTracker::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo)
{
	BOOL bRet = CRectTracker::Track(pWnd, point, bAllowInvert, pWndClipTo);
	pGrid->SendMessage(WM_LBUTTONUP, (WPARAM)&point, (WPARAM)&point);
	return bRet;
}
BOOL CGridTracker::TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert)
{
	BOOL bRet = CRectTracker::TrackRubberBand(pWnd, point, bAllowInvert);
	pGrid->SendMessage(WM_LBUTTONUP, (WPARAM)&point, (WPARAM)&point);
	return bRet;
}