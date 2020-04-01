// CxTargetButton.cpp : implementation file
/** 
 * CxTargetButton  v1.00 12/Aug/2001
 * CxTargetButton  v1.01 02/Gen/2008
 * (c) Davide Pizzolato - www.xdp.it
 */

#include "stdafx.h"
#include "xTargetButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxTargetButton
CxTargetButton::CxTargetButton()
{
	m_target = true;
	m_Border=1;				//draw 3D border
	m_FocusRectMargin=3;	//focus dotted rect margin
	m_TextColor=GetSysColor(COLOR_BTNTEXT); // default button text color
	m_BkgColor=GetSysColor(COLOR_BTNFACE); // default button background color
	m_flat = m_Checked = m_button_down = m_tracking = false;

	m_Icon=m_IconDown=m_IconHighLight=NULL;	// icon handle
	m_IconBox.SetRectEmpty(); //icon location
	m_IconAlign=BS_CENTER;	//icon alignment
	m_TextAlign=DT_CENTER;	//text alignment
	m_pLF=NULL;				//font structure pointer
}
/////////////////////////////////////////////////////////////////////////////
CxTargetButton::~CxTargetButton()
{
	m_Font.DeleteObject();
	if (m_pLF)	free(m_pLF);
	if (m_IconDown!=m_Icon && m_IconDown) DestroyIcon(m_IconDown);
	if (m_IconHighLight!=m_Icon && m_IconHighLight) DestroyIcon(m_IconHighLight);
	if (m_Icon) DestroyIcon(m_Icon);
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CxTargetButton, CButton)
	//{{AFX_MSG_MAP(CxTargetButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	//ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_CXTGT_RADIO , OnRadioInfo)
	ON_MESSAGE(BM_SETCHECK , OnBMSetCheck)
	ON_MESSAGE(BM_GETCHECK , OnBMGetCheck)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CxTargetButton message handlers
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::PreSubclassWindow() 
{
	m_Style=GetStyle(); //get general button styles
	m_IsPushLike=((m_Style & BS_PUSHLIKE)!=0);
	m_flat=((m_Style & BS_FLAT)!=0);
	SetTextAlign(m_Style & 0x0300);

	m_Style=GetButtonStyle();	///get specific BS_ styles
	if ((m_Style & BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX)
//		||((m_Style & BS_CHECKBOX)==BS_CHECKBOX))
		m_Style=BS_CHECKBOX;
	else if ((m_Style & BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON)
//			||((m_Style & BS_RADIOBUTTON)==BS_RADIOBUTTON))
		m_Style=BS_RADIOBUTTON;
	else { m_Style=BS_PUSHBUTTON; m_IsPushLike=true;}

	//default radio & check-box has no border
	if (!m_IsPushLike) m_Border = false;

	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CxTargetButton::OnEraseBkgnd(CDC* /*pDC*/) 
{ return 1; }	// doesn't erase the button background
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT (lpDrawItemStruct);
    //TRACE("* Captured: %08X\n", ::GetCapture());

    //Check if the button state in not in inconsistent mode...
    POINT mouse_position;
    if ((m_button_down) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position))){
		if (::WindowFromPoint(mouse_position) == m_hWnd){
			if ((GetState() & BST_PUSHED) != BST_PUSHED) {
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(TRUE);
				return;
			}
		} else {
			if ((GetState() & BST_PUSHED) == BST_PUSHED) {
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(FALSE);
				return;
	}	}	}
	
    //TRACE("* Drawing: %08x\n", lpDrawItemStruct->itemState);
	bool btargetdone=false;
	CString sCaption;
	CDC *pRealDC = CDC::FromHandle(lpDrawItemStruct->hDC);	// get device context
	RECT r=lpDrawItemStruct->rcItem;					// context rectangle
	int cx = r.right  - r.left ;						// get width
	int cy = r.bottom - r.top  ;						// get height
	// get text box position
	RECT tr={r.left+m_FocusRectMargin+2,r.top,r.right-m_FocusRectMargin-2,r.bottom};

	CDC hdcMem;	//create a memory DC to avoid flicker
	hdcMem.CreateCompatibleDC(pRealDC);
	CDC* pDC = &hdcMem; //(just use pRealDC to paint directly the screen)
    CBitmap hBitmap; //create a destination for raster operations
	hBitmap.CreateCompatibleBitmap(pRealDC,cx,cy);
	CBitmap* pOldBitmap = hdcMem.SelectObject(&hBitmap); //select the destination for MemDC

	GetWindowText(sCaption);							// get button text
	pDC->SetBkMode(TRANSPARENT);
	// with MemDC we need to select the font...

	//get text font
	CFont* pOldFont=NULL;
	if (m_Font.m_hObject) pOldFont = pDC->SelectObject(&m_Font);
	else pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
//	TEXTMETRIC tm;
//	pDC->GetTextMetrics(&tm);							// get font metrics

	// accommodate text location
	if (m_Icon){
		switch (m_IconAlign){
		case BS_LEFT:
			tr.left+=m_IconBox.Width();		// shift left
			break;
		case BS_RIGHT:
			tr.right-=m_IconBox.Width();	// shift right
			break;
		case BS_CENTER:
			tr.top+=m_IconBox.bottom;		// shift text under the icon
			tr.bottom-=m_FocusRectMargin + 1;
		}
	}
	// Select the correct skin 
	if (lpDrawItemStruct->itemState & ODS_DISABLED){	// DISABLED BUTTON
		pDC->FillSolidRect(&r,GetSysColor(COLOR_BTNFACE));

		if (m_Icon)	//draw the icon
			pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
				m_Icon,DSS_DISABLED,(CBrush*)NULL);

		// if needed, draw the standard 3D rectangular border
		if ((m_Border)&&(m_flat==FALSE)) pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
		// paint the etched button text
		pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));
		pDC->DrawText(sCaption,&tr,m_TextAlign);
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		OffsetRect(&tr,-1,-1);
		pDC->DrawText(sCaption,&tr,m_TextAlign);
	} else {
//---------------------------------------------------------------------------
		if ((lpDrawItemStruct->itemState & ODS_SELECTED)||m_Checked){ //SELECTED (DOWN) BUTTON
 
			if (m_Style==BS_PUSHBUTTON){
				if (m_BkgColor == GetSysColor(COLOR_BTNFACE)){
					pDC->FillSolidRect(&r,GetSysColor(COLOR_3DHILIGHT));
				} else {
					pDC->FillSolidRect(&r,m_BkgColor);
				}
				if (m_tracking)	{
					DrawTarget(pDC,&r,m_TextColor,3,4,3);
				}
			} else {
				if (m_tracking){
					pDC->FillSolidRect(&r,GetSysColor(COLOR_3DHILIGHT));
					DrawTarget(pDC,&r,m_TextColor,3,4,3);
					btargetdone=true;
				} else {
					pDC->FillSolidRect(&r,GetSysColor(COLOR_3DHILIGHT));
				}
			}

			if (m_IconDown){ //draw the pushed icon
				if (m_IsPushLike) m_IconBox.OffsetRect(1,1);
				pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
								m_IconDown,DSS_NORMAL,(CBrush*)NULL);
				if (m_IsPushLike) m_IconBox.OffsetRect(-1,-1);
			}
			//shift text
			if (m_IsPushLike) OffsetRect(&tr,1,1);
			// if needed, draw the standard 3D rectangular border
			if (m_Border){
				if (m_flat) pDC->Draw3dRect(&r,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
				else		pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
			}


		} else {
//---------------------------------------------------------------------------
					// DEFAULT BUTTON
            if (m_tracking){
				if (m_BkgColor == GetSysColor(COLOR_BTNFACE)){
					pDC->FillSolidRect(&r,GetSysColor(COLOR_3DHILIGHT));
				} else {
					pDC->FillSolidRect(&r,m_BkgColor);
				}
				DrawTarget(pDC,&r,m_TextColor,3,4,3);
				btargetdone=true;
			} else {
				pDC->FillSolidRect(&r,m_BkgColor);
			}

			if (m_IconHighLight && m_tracking) { // draw the highlighted icon
				pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
								m_IconHighLight,DSS_NORMAL,(CBrush*)NULL);
			} else if (m_Icon) //draw the icon
				pDC->DrawState(m_IconBox.TopLeft(),m_IconBox.Size(),
								m_Icon,DSS_NORMAL,(CBrush*)NULL);

			// if needed, draw the standard 3D rectangular border
			if ((m_Border)&&((m_tracking)||(m_flat==FALSE))){
				if (lpDrawItemStruct->itemState & ODS_DEFAULT){
					pDC->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
					InflateRect(&r,-1,-1);
					pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
				} else {
					if (m_flat) pDC->Draw3dRect(&r,GetSysColor(COLOR_BTNHILIGHT),GetSysColor(COLOR_BTNSHADOW));
					//else		pDC->DrawEdge(&r,EDGE_RAISED,BF_RECT);
					else pDC->Draw3dRect(&r,GetSysColor(COLOR_BTNHILIGHT),GetSysColor(COLOR_BTNSHADOW));
				}
			}
//---------------------------------------------------------------------------
		}
		// paint the focus rect
		if ((lpDrawItemStruct->itemState & ODS_FOCUS)&&(m_FocusRectMargin>0)){
			if (m_tracking || (lpDrawItemStruct->itemState & ODS_SELECTED)){
				if (!btargetdone) {
					DrawTarget(pDC,&r,m_TextColor,3,4,3,1);
				}
			}else{
				DrawTarget(pDC,&r,GetSysColor(COLOR_INFOBK),3,4,3);
			}

//			InflateRect(&r,-m_FocusRectMargin,-m_FocusRectMargin);
//			m_dh.Draw(pDC->GetSafeHdc(),1+r.left,r.top);
//			m_dh.Draw(pDC->GetSafeHdc(),1+r.left,r.bottom);
//			m_dv.Draw(pDC->GetSafeHdc(),r.left,1+r.top);
//			m_dv.Draw(pDC->GetSafeHdc(),r.right,1+r.top);
		}
		// paint the enabled button text
		pDC->SetTextColor(m_TextColor);
		pDC->DrawText(sCaption,&tr,m_TextAlign);
	}

	if (pOldFont) pDC->SelectObject(pOldFont);	//restore the old font object

	pRealDC->BitBlt(0,0,cx,cy,&hdcMem,0,0,SRCCOPY); //copy in the real world
	//restore the old objects to avoid memory leakage <David Scambler>
	if (pOldBitmap)	hdcMem.SelectObject(pOldBitmap);
//	hdcMem.DeleteDC(); the destructor will do this for us
//	hBitmap.DeleteObject(); the destructor will do this for us

}
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::DrawTarget(CDC *pDC, RECT *r, DWORD color, int border, int width, int height, int offset)
{
	if (m_target){
		int x,y;
		CPen pen;
		if (pen.CreatePen(PS_SOLID, 1,  color))
		{
			CPen* pOldPen = pDC->SelectObject(&pen);

			pDC->MoveTo(offset+border+width,offset+border);
			pDC->LineTo(offset+border,offset+border);
			pDC->LineTo(offset+border,offset+border+height+1);

			x = r->right - border - 2 +(1-m_IsPushLike);
			pDC->MoveTo(offset+x - width,offset+border);
			pDC->LineTo(offset+x,offset+border);
			pDC->LineTo(offset+x,offset+border+height+1);

			y = r->bottom - border - 2+(1-m_IsPushLike);
			pDC->MoveTo(offset+border+width,offset+y);
			pDC->LineTo(offset+border,offset+y);
			pDC->LineTo(offset+border,offset+y-height-1);

			pDC->MoveTo(offset+x - width,offset+y);
			pDC->LineTo(offset+x,offset+y);
			pDC->LineTo(offset+x,offset+y-height-1);

			pDC->SelectObject(pOldPen);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::SetIcon(UINT nIcon, UINT nIconAlign, UINT nIconDown, UINT nIconHighLight)
{
	if (m_IconDown!=m_Icon && m_IconDown) DestroyIcon(m_IconDown);
	if (m_IconHighLight!=m_Icon && m_IconHighLight) DestroyIcon(m_IconHighLight);
	if (m_Icon) DestroyIcon(m_Icon);
		
	m_Icon = (HICON)::LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(nIcon),
								IMAGE_ICON, 0, 0, 0);
	if (m_Icon){							// if success...
		ICONINFO iinfo;						// get icon info
		GetIconInfo(m_Icon, &iinfo);
		m_IconBox.left=m_IconBox.top=0;
		m_IconBox.right = iinfo.xHotspot * 2;
		m_IconBox.bottom = iinfo.yHotspot * 2;

		long x,y;
		RECT rect;
		GetWindowRect(&rect);
		y=rect.bottom-rect.top;
		x=rect.right-rect.left;
		switch (nIconAlign){				//set the icon location
		case BS_RIGHT:
			m_IconAlign=BS_RIGHT;
			m_IconBox.OffsetRect(x - iinfo.xHotspot*2 - m_FocusRectMargin,
								 max(0, (long)(y/2 - iinfo.yHotspot)));
			break;
		case BS_LEFT:
			m_IconAlign=BS_LEFT;
			m_IconBox.OffsetRect(m_FocusRectMargin,
								 max(0, (long)(y/2 - iinfo.yHotspot)));
			break;
		default:
			m_IconAlign=BS_CENTER;
			m_IconBox.OffsetRect(max(0, (long)(x/2 - iinfo.xHotspot)),
								 max(0, (long)(y/2 - 2*iinfo.yHotspot)));
		}

		//release icon mask bitmaps (Orioli Alessandro <aorioli@temaweb.it>)
		if( iinfo.hbmColor ) DeleteObject( iinfo.hbmColor );
		if( iinfo.hbmMask ) DeleteObject( iinfo.hbmMask );

		if (nIconDown > 0){	//load down icon
			m_IconDown = (HICON)::LoadImage(AfxGetInstanceHandle(),
											MAKEINTRESOURCE(nIconDown),
											IMAGE_ICON, 0, 0, 0);
			if (m_IconDown==NULL) m_IconDown= m_Icon;
		} else { m_IconDown= m_Icon; }	//reuse resource handle

		if (nIconHighLight > 0){	//load highlighted icon
			m_IconHighLight = (HICON)::LoadImage(AfxGetInstanceHandle(),
											MAKEINTRESOURCE(nIconHighLight),
											IMAGE_ICON, 0, 0, 0);
			if (m_IconHighLight==NULL) m_IconHighLight= m_Icon;
		} else { m_IconHighLight= m_Icon; }	//reuse resource handle
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::SetTextAlign(UINT nTextAlign)
{
//	see DrawText() styles...
	switch (nTextAlign){
	case BS_RIGHT:
		m_TextAlign=DT_RIGHT;
		break;
	case BS_LEFT:
		m_TextAlign=DT_LEFT;
		break;
	default:
		m_TextAlign=DT_CENTER;
	}
	m_TextAlign|=(DT_SINGLELINE|DT_VCENTER);
}
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::SetFlat(bool bFlag)
{ m_flat=bFlag; }	//set flat border mode
/////////////////////////////////////////////////////////////////////////////
COLORREF CxTargetButton::SetTextColor(COLORREF new_color)
{
	COLORREF tmp_color=m_TextColor;
	m_TextColor=new_color;
	return tmp_color;			//returns the previous color
}
/////////////////////////////////////////////////////////////////////////////
COLORREF CxTargetButton::SetBkgColor(COLORREF new_color)
{
	COLORREF tmp_color=m_BkgColor;
	m_BkgColor=new_color;
	return tmp_color;			//returns the previous color
}
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::SetToolTipText(CString s, CString sDown)
{
	m_ToolTipUp = s;		//normal tooltip
	m_ToolTipDw = sDown;	//tooltip for checked state

	//select the down tooltip if the button is already checked <Jesper Kinnås>
	if (m_Checked && !m_ToolTipDw.IsEmpty()) s = m_ToolTipDw;
	
	if(m_tooltip.m_hWnd==NULL){
		if(m_tooltip.Create(this))	//first assignment
			if(m_tooltip.AddTool(this, (LPCTSTR)s)){
				m_tooltip.Activate(1);
				// enable multiline tooltips <Derek Lakin>
				m_tooltip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 300);
			}
	} else {
		m_tooltip.UpdateTipText((LPCTSTR)s,this);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
// This function will create a MSG structure, fill it in a pass it to
// the ToolTip control, m_ttip.  Note that we ensure the point is in window
// coordinates (relative to the control's window).
	if(NULL != m_tooltip.m_hWnd){
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);

		m_tooltip.RelayEvent(&msg);
	}
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnLButtonDblClk
//Class.......: CxTargetButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: Used only to be forwarded as WM_LBUTTONDOWN message parameters
//Exceptions..: NONE
//------------
//Description :
//  > We do not care about doublelicks - handle this event
//    like an ordinary left-button-down event
//---------------------------------------------------------
void CxTargetButton::OnLButtonDblClk(UINT flags, CPoint point) 
{
    SendMessage(WM_LBUTTONDOWN, flags, MAKELPARAM(point.x, point.y));
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnLButtonDown
//Class.......: CxTargetButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//  > Handle event when left button is pressed down
//---------------------------------------------------------
void CxTargetButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: down\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONDOWN,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are tracking this button, cancel it
    if (m_tracking) {
        TRACKMOUSEEVENT t = {
            sizeof(TRACKMOUSEEVENT),
            TME_CANCEL | TME_LEAVE,
            m_hWnd, 0
		};
        if (::_TrackMouseEvent(&t)) {
            m_tracking = false;
        }
    }

    //Default-process the message
	CButton::OnLButtonDown(nFlags, point);
    m_button_down = true;
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnLButtonUp
//Class.......: CxTargetButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle event when left button is released (goes up)
//---------------------------------------------------------
void CxTargetButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: up\n", ::GetTickCount());

	if (m_Style){ //track mouse for radio & check buttons
		POINT p2 = point;
		::ClientToScreen(m_hWnd, &p2);
		HWND mouse_wnd = ::WindowFromPoint(p2);
		if (mouse_wnd == m_hWnd && m_button_down == true){ // mouse is in button
			if (m_Style==BS_CHECKBOX) SetCheck(m_Checked ? 0 : 1);
			if (m_Style==BS_RADIOBUTTON) SetCheck(1);
		}
	}

	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONUP,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //Default-process the message
    m_button_down = false;
	CButton::OnLButtonUp(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnMouseMove
//Class.......: CxTargetButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//  > Handle change of mouse position: see the comments in the
//    method for further info.
//---------------------------------------------------------
void CxTargetButton::OnMouseMove(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: Mouse\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are in capture mode, button has been pressed down
    //recently and not yet released - therefore check is we are
    //actually over the button or somewhere else. If the mouse
    //position changed considerably (e.g. we moved mouse pointer
    //from the button to some other place outside button area)
    //force the control to redraw
    //
    if ((m_button_down) && (::GetCapture() == m_hWnd)) {
	    POINT p2 = point;
        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        bool pressed = ((GetState() & BST_PUSHED) == BST_PUSHED);
        bool need_pressed = (mouse_wnd == m_hWnd);
        if (pressed != need_pressed) {
            //TRACE("* %08X Redraw\n", GetTickCount());
            SetState(need_pressed ? TRUE : FALSE);
            Invalidate();
        }
    } else {

	//Otherwise the button is released. That means we should
    //know when we leave its area - and so if we are not tracking
    //this mouse leave event yet, start now!
    //
        if (!m_tracking) {
            TRACKMOUSEEVENT t = {
                sizeof(TRACKMOUSEEVENT),
                TME_LEAVE,
                m_hWnd,
                0
            };
            if (::_TrackMouseEvent(&t)) {
                //TRACE("* Mouse enter\n");
                m_tracking = true;
                Invalidate();
            }
        }
    }

    //Forward this event to superclass
    CButton::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnMouseLeave
//Class.......: CxTargetButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NULL
//Parameters..: NOT USED
//Exceptions..: NONE
//------------
//Description :
//  > Handle situation when mouse cursor leaves area of this
//    window (button). This event might be generated ONLY
//    if we explicitely call 'TrackMouseEvent'. This is
//    signalled by setting the m_tracking flag (see the assert
//    precondition) - in 'OnMouseMove' method
//
//  > When a mouse pointer leaves area of this button (i.e.
//    when this method is invoked), presumably the look of
//    the button changes (e.g. when hover/non-hover images are set)
//    and therefore we force the control to redraw.
//---------------------------------------------------------
LRESULT CxTargetButton::OnMouseLeave(WPARAM, LPARAM)
{
    ASSERT (m_tracking);
    //TRACE("* Mouse leave\n");
    m_tracking = false;
    Invalidate();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnKillFocus
//Class.......: CxTargetButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: See superclass documentation
//Exceptions..: NONE
//------------
//Description :
//  > If focus is killed during capture, we may no longer
//    have the exclusive access to user input and therefore
//    release it.
//
//  > Such a situation might happens when the user left-clicks
//    this button, keeps the button down and simultaneously
//    presses TAB key.
//---------------------------------------------------------
void CxTargetButton::OnKillFocus(CWnd *new_wnd)
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
        m_button_down = false;
    }
    CButton::OnKillFocus(new_wnd);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnClicked
//Class.......: CxTargetButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: FALSE (do not stop in this handler - forward to parent)
//Parameters..: NONE
//Exceptions..: NONE
//------------
//Description :
//  > Keep consistency of attributes of this instance before
//    submitting click event to the parent.
//
//  > Currently NOT used. To use, umcomment line
//    "ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)" in message map
//    at the beginning of this file.
//---------------------------------------------------------
BOOL CxTargetButton::OnClicked() 
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
    }
    m_button_down = false;
    //Invalidate();
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//Method......: OnRadioInfo
//Class.......: CxTargetButton
//
//Author......: Rainer Mangold
//Created.....: JUN-2001
//
//Return value: NULL
//Parameters..: WPARAM and LPARAM (LPARAM not used)
//Exceptions..: NONE
//------------
//Description :
//  > Handle notification, that a Button in the same group was pushed
//---------------------------------------------------------
LRESULT CxTargetButton::OnRadioInfo(WPARAM /*wparam*/, LPARAM)
{
	if (m_Checked){	//only checked buttons need to be unchecked
		m_Checked = false;
		if(m_tooltip.m_hWnd!=NULL)
			m_tooltip.UpdateTipText((LPCTSTR)m_ToolTipUp,this);
		Invalidate();
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((m_Style)&&(nChar==' ')){ //needed stuff for check & radio buttons
		if (m_Style==BS_CHECKBOX) SetCheck(m_Checked ? 0 : 1);
		if (m_Style==BS_RADIOBUTTON) SetCheck(1);
	}
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
/////////////////////////////////////////////////////////////////////////////
//Method......: SetCheck
//Class.......: CxTargetButton
//
//Author......: Rainer Mangold
//Created.....: JUN-2001
//
//Return value: NONE
//Parameters..: bool
//Exceptions..: NONE
//------------
//Description :
//  > Set the state of this button (pushed or not). 
//    Works for both, Radio and CheckBox - Buttons
//---------------------------------------------------------
LRESULT CxTargetButton::OnBMSetCheck(WPARAM wparam, LPARAM)
{
	m_Checked=wparam!=0;
	switch (m_Style)
	{
	case BS_RADIOBUTTON:
		if (m_Checked) { //uncheck the other radio buttons (in the same group)
			HWND hthis,hwnd2,hpwnd;
			hpwnd=GetParent()->GetSafeHwnd();	//get button parent handle
			hwnd2=hthis=GetSafeHwnd();			//get this button handle
			if (hthis && hpwnd){				//consistency check
				for( ; ; ){	//scan the buttons within the group
					hwnd2=::GetNextDlgGroupItem(hpwnd,hwnd2,0);
					//until we reach again this button
					if ((hwnd2==hthis)||(hwnd2==NULL)) break;
					//post the uncheck message
					::PostMessage(hwnd2, WM_CXTGT_RADIO, 0, 0);
				}
			}
		}
		break;
	case BS_PUSHBUTTON:
		m_Checked=false;
		ASSERT(false); // Must be a Check or Radio button to use this function
	}

	//set the correct tooltip
	if (m_tooltip.m_hWnd!=NULL){
		m_tooltip.Pop();	//to avoid flicker
		if (m_Checked && !m_ToolTipDw.IsEmpty())
			m_tooltip.UpdateTipText((LPCTSTR)m_ToolTipDw,this);
		else
			m_tooltip.UpdateTipText((LPCTSTR)m_ToolTipUp,this);
	}
	Invalidate();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
LRESULT CxTargetButton::OnBMGetCheck(WPARAM /*wparam*/, LPARAM)
{ return m_Checked; }	//returns the state for check & radio buttons
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::SetButtonStyle(UINT nStyle, BOOL bRedraw)
{
	// see PreSubclassWindow
	m_IsPushLike=((nStyle & BS_PUSHLIKE)!=0);
	m_flat=((nStyle & BS_FLAT)!=0);

	if ((nStyle & BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX)
//		||((m_Style & BS_CHECKBOX)==BS_CHECKBOX))
		m_Style=BS_CHECKBOX;
	else if ((nStyle & BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON)
//			||((m_Style & BS_RADIOBUTTON)==BS_RADIOBUTTON))
		m_Style=BS_RADIOBUTTON;
	else { m_Style=BS_PUSHBUTTON; m_IsPushLike=true; m_Checked=false; }

	//default radio & check-box has no border
	if (!m_IsPushLike) m_Border = false;

	if (bRedraw) Invalidate();
}
/////////////////////////////////////////////////////////////////////////////
bool CxTargetButton::SetFont(CString sFontName,long lSize, long lWeight, BYTE bItalic, BYTE bUnderline)
{
	if (m_pLF==NULL) m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
	if (m_pLF){
		_tcsncpy(m_pLF->lfFaceName,sFontName,LF_FACESIZE-1);
		m_pLF->lfHeight=lSize;
		m_pLF->lfWeight=lWeight;
		m_pLF->lfItalic=bItalic;
		m_pLF->lfUnderline=bUnderline;
		m_Font.DeleteObject();
		if (m_Font.CreateFontIndirect(m_pLF)) return true;
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////
bool CxTargetButton::SetFont(LOGFONT* pNewStyle)
{
	if (pNewStyle){
		if (m_pLF==NULL) m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
		if (m_pLF){
			memcpy(m_pLF,pNewStyle,sizeof(LOGFONT));
			m_Font.DeleteObject();
			if (m_Font.CreateFontIndirect(m_pLF)) return true;
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////////////////
LOGFONT* CxTargetButton::GetFont()
{ return m_pLF; }
/////////////////////////////////////////////////////////////////////////////
void CxTargetButton::EnableTarget(bool enable)
{
	m_target=enable;
}
/////////////////////////////////////////////////////////////////////////////
//EOF

