#include "StdAfx.h"

namespace DuiLib {
	IMPLEMENT_DUICONTROL(CControlUI)

		CControlUI::CControlUI()
		:m_pManager(NULL), 
		m_pParent(NULL), 
		m_bUpdateNeeded(true),
		m_bMenuUsed(false),
		m_bVisible(true), 
		m_bInternVisible(true),
		m_bPaneVisible(true),
		m_bMouseEnabled(true),
		m_bKeyboardEnabled(true),
		m_bFloat(false),
		m_uFloatAlign(0),
		m_bSetPos(false),
		m_bDragEnabled(false),
		m_bDropEnabled(false),
		m_bAcceptDropFile(false),
		m_bResourceText(false),
		m_nResourceID(0),
		m_chShortcut('\0'),
		m_pTag(NULL),
		m_bGradientVertical(true),
		m_dwBackColor2(0),
		m_dwBackColor3(0),
		m_bColorHSL(false),
		m_nBorderSize(0),
		m_nBorderStyle(PS_SOLID),
		m_nTooltipWidth(300),
		m_wCursor(0),
		m_instance(NULL),
		m_bAutoCalcWidth(false), m_bAutoCalcHeight(false),
		CUIAnimation( this ), m_animation(DuiAnim_null), m_nFrameCount(24), m_nFrameDelay(5),
		m_pExtraParent(NULL)
	{
		m_cXY.cx = m_cXY.cy = 0;
		m_cxyFixed.cx = m_cxyFixed.cy = 0;
		m_cxyFixedPercent.cx = m_cxyFixedPercent.cy = 0;
		m_cxyMin.cx = m_cxyMin.cy = 0;
		m_cxyMax.cx = m_cxyMax.cy = 9999;
		m_cxyBorderRound.cx = m_cxyBorderRound.cy = 0;

		::ZeroMemory(&m_rcPadding, sizeof(RECT));
		//::ZeroMemory(&m_rcItem, sizeof(RECT));
		//::ZeroMemory(&m_rcPaint, sizeof(RECT));
		//::ZeroMemory(&m_rcBorderSize,sizeof(RECT));
		m_piFloatPercent.left = m_piFloatPercent.top = m_piFloatPercent.right = m_piFloatPercent.bottom = 0.0f;

		::ZeroMemory(&m_ptFloatPosition,sizeof(POINT));

		__refCount = 1;
		//////////////////////////////////////////////////////////////////////////

		m_uTextStyle = 0;	

		m_iFont					= -1;
		m_iHotFont				= -1;
		m_iPushedFont			= -1;
		m_iFocusedFont			= -1;
		m_iDisabledFont			= -1;
		m_iSelectedFont			= -1;

		m_dwTextColor			= 0;
		m_dwHotTextColor		= 0;
		m_dwPushedTextColor		= 0;
		m_dwFocusedTextColor	= 0;
		m_dwDisabledTextColor	= 0;
		m_dwSelectedTextColor	= 0;

		m_dwBackColor			= 0;
		m_dwHotBkColor			= 0;
		m_dwFocusBkColor		= 0;
		m_dwPushedBkColor		= 0;
		m_dwDisabledBkColor		= 0;
		m_dwSelectedBkColor		= 0;

		m_dwForeColor			= 0;

		m_dwBorderColor			= 0;
		m_dwHotBorderColor		= 0;
		m_dwPushedBorderColor	= 0;
		m_dwFocusBorderColor	= 0;
		m_dwDisableBorderColor	= 0;
		m_dwSelectedBorderColor	= 0;

		m_ptLastMouse.x = m_ptLastMouse.y = 0;
	}

	CControlUI::~CControlUI()
	{
		if(!m_asOnDestroy.IsEmpty()) GetManager()->ExecuteScript(m_asOnDestroy, this);
		if( OnDestroy ) OnDestroy(this);
		RemoveAllCustomAttribute();	
		if( m_pManager != NULL ) m_pManager->ReapObjects(this);
	}

	CDuiString CControlUI::GetName() const
	{
		return m_sName;
	}

	void CControlUI::SetName(LPCTSTR pstrName)
	{
		m_sName = pstrName;
	}

	LPVOID CControlUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_CONTROL) == 0 ) return this;
		return NULL;
	}

	LPCTSTR CControlUI::GetClass() const
	{
		return _T("ControlUI");
	}

	UINT CControlUI::GetControlFlags() const
	{
		return 0;
	}

	bool CControlUI::Activate()
	{
		if( !IsVisible() ) return false;
		if( !IsEnabled() ) return false;
		return true;
	}

	CPaintManagerUI* CControlUI::GetManager() const
	{
		return m_pManager;
	}

	void CControlUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
	{
		m_pManager = pManager;
		m_pParent = pParent;
		if( bInit && m_pParent ) 
			Init();
	}

	CControlUI* CControlUI::GetParent() const
	{
		return m_pParent;
	}

	bool CControlUI::SetTimer(UINT nTimerID, UINT nElapse)
	{
		if(m_pManager == NULL) return false;

		return m_pManager->SetTimer(this, nTimerID, nElapse);
	}

	void CControlUI::KillTimer(UINT nTimerID)
	{
		if(m_pManager == NULL) return;

		m_pManager->KillTimer(this, nTimerID);
	}

	CDuiString CControlUI::GetText() const
	{
		if (IsResourceText()) 
		{
			CDuiString s = CResourceManager::GetInstance()->GetText(m_sText);
			if(!s.IsEmpty()) return s;
		}

 		CControlUI* pThis = const_cast<CControlUI* >(this);
		CLangPackageUI *pkg = pThis->GetLangPackage();
		if(pkg && GetResourceID() > 0)
		{
			CDuiString s = pkg->GetText(GetResourceID());
			if(!s.IsEmpty()) return s; 
		}
		else if (IsResourceText())
		{
			CDuiString s = CLangManagerUI::LoadString(m_sText);
			if(!s.IsEmpty()) return s;
		}
		return m_sText;
	}

	void CControlUI::SetText(LPCTSTR pstrText)
	{
		if( m_sText == pstrText ) return;

		m_sText = pstrText;
		// 解析xml换行符
		m_sText.Replace(_T("{\\n}"), _T("\n"));

		if(IsAutoCalcWidth())
			NeedParentUpdate();
		else
			Invalidate();
	}

	int  CControlUI::GetTextN()			//add by liqs99
	{
		return _ttoi(GetText());//(m_sText);
	}

	void CControlUI::SetTextN(int n)		//add by liqs99
	{
		CDuiString str;
		str.Format(_T("%d"), n);
		SetText(str);
	}

	void CControlUI::SetTextV(LPCTSTR lpszFormat, ...)
	{
		va_list argList;
		va_start(argList, lpszFormat);
		CDuiString s;
		s.InnerFormat(lpszFormat, argList);
		SetText(s);
		va_end(argList);
	}

	void CControlUI::SetTextStyle(UINT uStyle)
	{
		m_uTextStyle = uStyle;
		Invalidate();
	}

	UINT CControlUI::GetTextStyle() const
	{
		return m_uTextStyle;
	}

	RECT CControlUI::GetTextPadding() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleRect(m_rcTextPadding);
		return m_rcTextPadding;
	}

	void CControlUI::SetTextPadding(RECT rc)
	{
		m_rcTextPadding = rc;
		Invalidate();
	}

	RECT CControlUI::GetPushedTextPadding() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleRect(m_rcPushedTextPadding);
		return m_rcPushedTextPadding;
	}

	void CControlUI::SetPushedTextPadding(RECT rc)
	{
		m_rcPushedTextPadding = rc;
		Invalidate();
	}

	bool CControlUI::IsResourceText() const
	{
		return m_bResourceText;
	}

	void CControlUI::SetResourceText(bool bResource)
	{
		if( m_bResourceText == bResource ) return;
		m_bResourceText = bResource;
		Invalidate();
	}

	int  CControlUI::GetResourceID() const
	{
		return m_nResourceID;
	}

	void CControlUI::SetResourceID(int resid)
	{
		if(m_nResourceID == resid) return;
		m_nResourceID = resid;
		Invalidate();
	}

	CLangPackageUI *CControlUI::GetLangPackage()
	{
		if(!GetManager()) return NULL;
		if(m_sSkinFile.IsEmpty()) return NULL;
		CLangPackageUI *pkg = GetManager()->GetLangManager()->GetPackage(m_sSkinFile);
		return pkg;
	}

	void CControlUI::SetSkinFile(LPCTSTR lpstrSkinFile) 
	{ 
		m_sSkinFile = lpstrSkinFile; 
	}
	CDuiString CControlUI::GetSkinFile() { return m_sSkinFile.GetData(); }

	bool CControlUI::IsDragEnabled() const
	{
		return m_bDragEnabled;
	}

	void CControlUI::SetDragEnable(bool bDrag)
	{
		m_bDragEnabled = bDrag;
	}

	bool CControlUI::IsDropEnabled() const
	{
		return m_bDropEnabled;
	}

	void CControlUI::SetDropEnable(bool bDrop)
	{
		m_bDropEnabled = bDrop;
	}

	bool CControlUI::IsAcceptDropFile() const
	{
		return m_bAcceptDropFile;
	}

	void CControlUI::SetAcceptDropFile(bool bDrop)
	{
		m_bAcceptDropFile = bDrop;
	}
	//////////////////////////////////////////////////////////////////////////
	int CControlUI::GetFont() const					{ return m_iFont;			}
	void CControlUI::SetFont(int index)				{ m_iFont = index;			}
	int CControlUI::GetHotFont() const				{ return m_iHotFont;		}
	void CControlUI::SetHotFont(int index)			{ m_iHotFont = index;		}
	int CControlUI::GetPushedFont() const			{ return m_iPushedFont;		}
	void CControlUI::SetPushedFont(int index)		{ m_iPushedFont = index;	}
	int CControlUI::GetFocusedFont() const			{ return m_iFocusedFont;	}
	void CControlUI::SetFocusedFont(int index)		{ m_iFocusedFont = index;	}
	int CControlUI::GetDisabledFont() const			{ return m_iDisabledFont;	}
	void CControlUI::SetDisabledFont(int index)		{ m_iDisabledFont = index;	}
	int CControlUI::GetSelectedFont() const			{ return m_iSelectedFont;	}
	void CControlUI::SetSelectedFont(int index)		{ m_iSelectedFont = index;	}

	//////////////////////////////////////////////////////////////////////////
	DWORD CControlUI::GetTextColor() const					{ return m_dwTextColor;				}
	void CControlUI::SetTextColor(DWORD dwColor)			{ m_dwTextColor = dwColor;			}
	DWORD CControlUI::GetHotTextColor() const				{ return m_dwHotTextColor;			}
	void CControlUI::SetHotTextColor(DWORD dwColor)			{ m_dwHotTextColor = dwColor;		}
	DWORD CControlUI::GetPushedTextColor() const			{ return m_dwPushedTextColor;		}
	void CControlUI::SetPushedTextColor(DWORD dwColor)		{ m_dwPushedTextColor = dwColor;	}
	DWORD CControlUI::GetFocusedTextColor() const			{ return m_dwFocusedTextColor;		}
	void CControlUI::SetFocusedTextColor(DWORD dwColor)		{ m_dwFocusedTextColor = dwColor;	}
	DWORD CControlUI::GetDisabledTextColor() const			
	{ 
		if(m_dwDisabledTextColor == 0 && m_pManager)
			return m_pManager->GetDefaultDisabledColor();
		return m_dwDisabledTextColor;		
	}
	void CControlUI::SetDisabledTextColor(DWORD dwColor)	{ m_dwDisabledTextColor = dwColor;	}
	DWORD CControlUI::GetSelectedTextColor() const			{ return m_dwSelectedTextColor;		}
	void CControlUI::SetSelectedTextColor(DWORD dwColor)	{ m_dwSelectedTextColor = dwColor;	}

	//////////////////////////////////////////////////////////////////////////
	bool CControlUI::GetGradient()
	{
		return m_bGradientVertical;
	}

	void CControlUI::SetGradient(bool bVertical)
	{
		if(m_bGradientVertical == bVertical) return;
		m_bGradientVertical = bVertical;
		Invalidate();
	}

	DWORD CControlUI::GetBkColor2() const
	{
		return m_dwBackColor2;
	}

	void CControlUI::SetBkColor2(DWORD dwBackColor)
	{
		if( m_dwBackColor2 == dwBackColor ) return;

		m_dwBackColor2 = dwBackColor;
		Invalidate();
	}

	DWORD CControlUI::GetBkColor3() const
	{
		return m_dwBackColor3;
	}

	void CControlUI::SetBkColor3(DWORD dwBackColor)
	{
		if( m_dwBackColor3 == dwBackColor ) return;

		m_dwBackColor3 = dwBackColor;
		Invalidate();
	}

	DWORD CControlUI::GetBkColor() const
	{
		return m_dwBackColor;
	}

	void CControlUI::SetBkColor(DWORD dwBackColor)
	{
		if( m_dwBackColor == dwBackColor ) return;

		m_dwBackColor = dwBackColor;
		Invalidate();
	}

	DWORD CControlUI::GetHotBkColor() const				{ return m_dwHotBkColor; }
	void CControlUI::SetHotBkColor(DWORD dwColor)		{ m_dwHotBkColor = dwColor; }

	DWORD CControlUI::GetFocusBkColor() const			{ return m_dwFocusBkColor; }
	void CControlUI::SetFocusBkColor(DWORD dwColor)		{ m_dwFocusBkColor = dwColor; }

	DWORD CControlUI::GetPushedBkColor() const			{ return m_dwPushedBkColor; }
	void CControlUI::SetPushedBkColor(DWORD dwColor)	{ m_dwPushedBkColor = dwColor; }

	DWORD CControlUI::GetDisabledBkColor() const		{ return m_dwDisabledBkColor; }
	void CControlUI::SetDisabledBkColor(DWORD dwColor)	{ m_dwDisabledBkColor = dwColor; }

	DWORD CControlUI::GetSelectBkColor()				 { return m_dwSelectedBkColor; }
	void CControlUI::SetSelectedBkColor(DWORD dwBkColor) { m_dwSelectedBkColor = dwBkColor; }

	//////////////////////////////////////////////////////////////////////////
	DWORD CControlUI::GetForeColor() const
	{
		return m_dwForeColor;
	}

	void CControlUI::SetForeColor(DWORD dwForeColor)
	{
		if( m_dwForeColor == dwForeColor ) return;

		m_dwForeColor = dwForeColor;
		Invalidate();
	}

	//////////////////////////////////////////////////////////////////////////
	CDuiString CControlUI::GetBkImage()
	{
		return m_sBkImage;
	}

	void CControlUI::SetBkImage(LPCTSTR pStrImage)
	{
		if(m_pManager) m_pManager->RemoveImage(pStrImage);
		if( m_sBkImage == pStrImage ) return;

		m_sBkImage = pStrImage;
		Invalidate();
	}
	
	//////////////////////////////////////////////////////////////////////////
	CDuiString	CControlUI::GetForeImage() const					{ return m_sForeImage;				}
	void		CControlUI::SetForeImage(LPCTSTR pStrImage)			{ m_sForeImage = pStrImage;			}

	CDuiString	CControlUI::GetHotForeImage() const					{ return m_sHotForeImage;			}
	void		CControlUI::SetHotForeImage(LPCTSTR pStrImage)		{ m_sHotForeImage = pStrImage;		}

	CDuiString	CControlUI::GetPushedForeImage() const				{ return m_sPushedForeImage;		}
	void		CControlUI::SetPushedForeImage(LPCTSTR pStrImage)	{ m_sPushedForeImage = pStrImage;	}

	CDuiString	CControlUI::GetFocusedForeImage() const				{ return m_sFocusedForeImage;		}
	void		CControlUI::SetFocusedForeImage(LPCTSTR pStrImage)	{ m_sFocusedForeImage = pStrImage;	}

	CDuiString	CControlUI::GetDisabledForeImage() const			{ return m_sDisabledForeImage;		}
	void		CControlUI::SetDisabledForeImage(LPCTSTR pStrImage){ m_sDisabledForeImage = pStrImage;	}

	CDuiString	CControlUI::GetSelectedForeImage() const			{ return m_sSelectedForeImage;		}
	void		CControlUI::SetSelectedForedImage(LPCTSTR pStrImage){ m_sSelectedForeImage = pStrImage; }

	//////////////////////////////////////////////////////////////////////////
	CDuiString	CControlUI::GetNormalImage() const						{ return m_sNormalImage;		}
	void		CControlUI::SetNormalImage(LPCTSTR pStrImage)			{ m_sNormalImage = pStrImage;	}

	CDuiString	CControlUI::GetHotImage() const							{ return m_sHotImage;			}
	void		CControlUI::SetHotImage(LPCTSTR pStrImage)				{ m_sHotImage = pStrImage;		}

	CDuiString	CControlUI::GetPushedImage() const						{ return m_sPushedImage;		}
	void		CControlUI::SetPushedImage(LPCTSTR pStrImage)			{ m_sPushedImage = pStrImage;	}

	CDuiString	CControlUI::GetFocusedImage() const						{ return m_sFocusedImage;		}
	void		CControlUI::SetFocusedImage(LPCTSTR pStrImage)			{ m_sFocusedImage = pStrImage;	}

	CDuiString	CControlUI::GetDisabledImage() const					{ return m_sDisabledImage;		}
	void		CControlUI::SetDisabledImage(LPCTSTR pStrImage)			{ m_sDisabledImage = pStrImage; }

	CDuiString	CControlUI::GetSelectedImage() const					{ return m_sSelectedImage;		}
	void		CControlUI::SetSelectedImage(LPCTSTR pStrImage)			{ m_sSelectedImage = pStrImage; }

	CDuiString	CControlUI::GetSelectedHotImage() const					{ return m_sSelectedHotImage;			}
	void		CControlUI::SetSelectedHotImage(LPCTSTR pStrImage)		{ m_sSelectedHotImage = pStrImage;		}
	CDuiString	CControlUI::GetSelectedPushedImage() const				{ return m_sSelectedPushedImage;		}
	void		CControlUI::SetSelectedPushedImage(LPCTSTR pStrImage)	{ m_sSelectedPushedImage = pStrImage;	}
	CDuiString	CControlUI::GetSelectedFocusedImage() const				{ return m_sSelectedFocusedImage;		}
	void		CControlUI::SetSelectedFocusedImage(LPCTSTR pStrImage)	{ m_sSelectedFocusedImage = pStrImage;	}
	CDuiString	CControlUI::GetSelectedDisabledImage() const			{ return m_sSelectedDisabledImage;		}
	void		CControlUI::SetSelectedDisabledImage(LPCTSTR pStrImage)	{ m_sSelectedDisabledImage = pStrImage; }

	//////////////////////////////////////////////////////////////////////////
	DWORD CControlUI::GetBorderColor() const					{ return m_dwBorderColor;					}
	void CControlUI::SetBorderColor(DWORD dwBorderColor)		{ m_dwBorderColor = dwBorderColor;			}
		
	DWORD CControlUI::GetHotBorderColor() const					{ return m_dwHotBorderColor;				}
	void CControlUI::SetHotBorderColor(DWORD dwBorderColor)		{ m_dwHotBorderColor = dwBorderColor;		}

	DWORD CControlUI::GetPushedBorderColor() const				{ return m_dwPushedBorderColor;				}
	void CControlUI::SetPushedBorderColor(DWORD dwBorderColor)	{ m_dwPushedBorderColor = dwBorderColor;	}

	DWORD CControlUI::GetFocusBorderColor() const				{ return m_dwFocusBorderColor;				}
	void CControlUI::SetFocusBorderColor(DWORD dwBorderColor)	{ m_dwFocusBorderColor = dwBorderColor;		}

	DWORD CControlUI::GetDisableBorderColor() const				{ return m_dwDisableBorderColor;			}
	void CControlUI::SetDisableBorderColor(DWORD dwBorderColor) { m_dwDisableBorderColor = dwBorderColor;	}

	DWORD CControlUI::GetSelectedBorderColor() const			{ return m_dwSelectedBorderColor;			}
	void CControlUI::SetSelectedBorderColor(DWORD dwBorderColor) { m_dwSelectedBorderColor = dwBorderColor; }

	bool CControlUI::IsColorHSL() const
	{
		return m_bColorHSL;
	}

	void CControlUI::SetColorHSL(bool bColorHSL)
	{
		if( m_bColorHSL == bColorHSL ) return;

		m_bColorHSL = bColorHSL;
		Invalidate();
	}

	int CControlUI::GetBorderSize() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleInt(m_nBorderSize);
		return m_nBorderSize;
	}

	void CControlUI::SetBorderSize(int nSize)
	{
		if( m_nBorderSize == nSize ) return;

		m_nBorderSize = nSize;
		Invalidate();
	}

	void CControlUI::SetBorderRectSize( RECT rc )
	{
		m_rcBorderSize = rc;
		Invalidate();
	}

	RECT CControlUI::GetBorderRectSize() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleRect(m_rcBorderSize);
		return m_rcBorderSize;
	}

	SIZE CControlUI::GetBorderRound() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleSize(m_cxyBorderRound);
		return m_cxyBorderRound;
	}

	void CControlUI::SetBorderRound(SIZE cxyRound)
	{
		m_cxyBorderRound = cxyRound;
		Invalidate();
	}

	bool CControlUI::DrawImage(UIRender *pRender, LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		return pRender->DrawImageString(m_rcItem, m_rcPaint, pStrImage, pStrModify, m_instance);
	}

	const CDuiRect& CControlUI::GetPos() const
	{
		return m_rcItem;
	}

	RECT CControlUI::GetRelativePos() const
	{
		CControlUI* pParent = GetParent();
		if( pParent != NULL ) {
			RECT rcParentPos = pParent->GetPos();
			CDuiRect rcRelativePos(m_rcItem);
			rcRelativePos.Offset(-rcParentPos.left, -rcParentPos.top);
			return rcRelativePos;
		}
		else {
			return CDuiRect(0, 0, 0, 0);
		}
	}

	RECT CControlUI::GetClientPos() const 
	{
		return m_rcItem;
	}
	void CControlUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		if( rc.right < rc.left ) rc.right = rc.left;
		if( rc.bottom < rc.top ) rc.bottom = rc.top;

		CDuiRect invalidateRc = m_rcItem;
		if( invalidateRc.IsEmpty() ) invalidateRc = rc;

		m_rcItem = rc;
		if( m_pManager == NULL ) return;

		if( !m_bSetPos ) {
			m_bSetPos = true;
			if(!m_asOnSize.IsEmpty()) 
				if(GetManager()->ExecuteScript(m_asOnSize, this))
					return;
			if( OnSize ) OnSize(this);
			m_bSetPos = false;
		}

		m_bUpdateNeeded = false;

		if( bNeedInvalidate && IsVisible() ) {
			invalidateRc.Join(m_rcItem);
			CControlUI* pParent = this;
			RECT rcTemp;
			RECT rcParent;
			while( pParent = pParent->GetParent() ) {
				if( !pParent->IsVisible() ) return;
				rcTemp = invalidateRc;
				rcParent = pParent->GetPos();
				//if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) return;
				if( !invalidateRc.Intersect(rcTemp, rcParent) ) return;
			}
			m_pManager->Invalidate(invalidateRc);
		}
	}

	bool CControlUI::CalcPos(CControlUI *pChildControl, CDuiRect &rcChild)
	{
		return false;
	}

	void CControlUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		m_cXY.cx += szOffset.cx;
		m_cXY.cy += szOffset.cy;
		NeedParentUpdate();
	}

	int CControlUI::GetWidth() const
	{
		return m_rcItem.right - m_rcItem.left;
	}

	int CControlUI::GetHeight() const
	{
		return m_rcItem.bottom - m_rcItem.top;
	}

	int CControlUI::GetX() const
	{
		return m_rcItem.left;
	}

	int CControlUI::GetY() const
	{
		return m_rcItem.top;
	}

	RECT CControlUI::GetPadding() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleRect(m_rcPadding);
		return m_rcPadding;
	}

	void CControlUI::SetPadding(RECT rcPadding)
	{
		m_rcPadding = rcPadding;
		NeedParentUpdate();
	}

	SIZE CControlUI::GetFixedXY() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleSize(m_cXY);
		return m_cXY;
	}

	void CControlUI::SetFixedXY(SIZE szXY)
	{
		m_cXY.cx = szXY.cx;
		m_cXY.cy = szXY.cy;
		NeedParentUpdate();
	}

	int CControlUI::GetFixedWidth() const
	{
		if (m_pManager != NULL) {
			return m_pManager->GetDPIObj()->ScaleInt(m_cxyFixed.cx);
		}

		return m_cxyFixed.cx;
	}

	void CControlUI::SetFixedWidth(int cx)
	{
		if( cx < 0 ) return; 
		m_cxyFixed.cx = cx;
		NeedParentUpdate();
	}

	int CControlUI::GetFixedHeight() const
	{
		if (m_pManager != NULL) {
			return m_pManager->GetDPIObj()->ScaleInt(m_cxyFixed.cy);
		}
		
		return m_cxyFixed.cy;
	}

	void CControlUI::SetFixedHeight(int cy)
	{
		if( cy < 0 ) return; 
		m_cxyFixed.cy = cy;
		NeedParentUpdate();
	}

	int CControlUI::GetFixedWidthPercent() const
	{
		return m_cxyFixedPercent.cx;
	}

	void CControlUI::SetFixedWidthPercent(int cx)
	{
		if( cx < 0 || cx > 100) return; 
		m_cxyFixedPercent.cx = cx;
		NeedParentUpdate();
	}

	int CControlUI::GetFixedHeightPercent() const
	{
		return m_cxyFixedPercent.cy;
	}

	void CControlUI::SetFixedHeightPercent(int cy)
	{
		if( cy < 0 || cy > 100) return; 
		m_cxyFixedPercent.cy = cy;
		NeedParentUpdate();
	}

	int CControlUI::GetMinWidth() const
	{
		if (m_pManager != NULL) {
			return m_pManager->GetDPIObj()->ScaleInt(m_cxyMin.cx);
		}
		return m_cxyMin.cx;
	}

	void CControlUI::SetMinWidth(int cx)
	{
		if( m_cxyMin.cx == cx ) return;

		if( cx < 0 ) return; 
		m_cxyMin.cx = cx;
		NeedParentUpdate();
	}

	int CControlUI::GetMaxWidth() const
	{
		if (m_pManager != NULL) {
			return m_pManager->GetDPIObj()->ScaleInt(m_cxyMax.cx);
		}
		return m_cxyMax.cx;
	}

	void CControlUI::SetMaxWidth(int cx)
	{
		if( m_cxyMax.cx == cx ) return;

		if( cx < 0 ) return; 
		m_cxyMax.cx = cx;
		NeedParentUpdate();
	}

	int CControlUI::GetMinHeight() const
	{
		if (m_pManager != NULL) {
			return m_pManager->GetDPIObj()->ScaleInt(m_cxyMin.cy);
		}
		
		return m_cxyMin.cy;
	}

	void CControlUI::SetMinHeight(int cy)
	{
		if( m_cxyMin.cy == cy ) return;

		if( cy < 0 ) return; 
		m_cxyMin.cy = cy;
		NeedParentUpdate();
	}

	int CControlUI::GetMaxHeight() const
	{
		if (m_pManager != NULL) {
			return m_pManager->GetDPIObj()->ScaleInt(m_cxyMax.cy);
		}

		return m_cxyMax.cy;
	}

	void CControlUI::SetMaxHeight(int cy)
	{
		if( m_cxyMax.cy == cy ) return;

		if( cy < 0 ) return; 
		m_cxyMax.cy = cy;
		NeedParentUpdate();
	}

	TPercentInfo CControlUI::GetFloatPercent() const
	{
		return m_piFloatPercent;
	}
	
	void CControlUI::SetFloatPercent(TPercentInfo piFloatPercent)
	{
		m_piFloatPercent = piFloatPercent;
		NeedParentUpdate();
	}

	void CControlUI::SetFloatAlign(UINT uAlign)
	{
		m_uFloatAlign = uAlign;
		NeedParentUpdate();
	}

	UINT CControlUI::GetFloatAlign() const
	{
		return m_uFloatAlign;
	}

	void CControlUI::SetFloatPosition(POINT ptPosition)
	{
		m_ptFloatPosition = ptPosition;
		NeedParentUpdate();
	}

	POINT CControlUI::GetFloatPosition() const
	{
		return m_ptFloatPosition;
	}

	bool CControlUI::IsAutoCalcWidth() const { return m_bAutoCalcWidth; }
	void CControlUI::SetAutoCalcWidth(bool bAutoCalcWidth) { m_bAutoCalcWidth = bAutoCalcWidth; }
	bool CControlUI::IsAutoCalcHeight() const { return m_bAutoCalcHeight; }
	void CControlUI::SetAutoCalcHeight(bool bAutoCalcHeight) { m_bAutoCalcHeight = bAutoCalcHeight; }

	CDuiString CControlUI::GetToolTip() const
	{
		if (IsResourceText()) 
		{
			CDuiString s = CResourceManager::GetInstance()->GetText(m_sToolTip);
			if(!s.IsEmpty()) return s;
		}

		CControlUI* pThis = const_cast<CControlUI* >(this);
		CLangPackageUI *pkg = pThis->GetLangPackage();
		if(pkg && GetResourceID() > 0)
		{
			CDuiString s = pkg->GetToolTip(GetResourceID());
			if(!s.IsEmpty()) return s; 
		}
		else if (IsResourceText())
		{
			CDuiString s = CLangManagerUI::LoadString(m_sToolTip);
			if(!s.IsEmpty()) return s;
		}

		return m_sToolTip;
	}

	void CControlUI::SetToolTip(LPCTSTR pstrText)
	{
		CDuiString strTemp(pstrText);
		if(!strTemp.IsEmpty())
			strTemp.Replace(_T("<n>"),_T("\r\n"));
		m_sToolTip = strTemp;
	}

	void CControlUI::SetToolTipWidth( int nWidth )
	{
		m_nTooltipWidth = nWidth;
	}

	int CControlUI::GetToolTipWidth( void )
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleInt(m_nTooltipWidth);
		return m_nTooltipWidth;
	}
	
	WORD CControlUI::GetCursor()
	{
		return m_wCursor;
	}

	void CControlUI::SetCursor(WORD wCursor)
	{
		m_wCursor = wCursor;
		Invalidate();
	}

	TCHAR CControlUI::GetShortcut() const
	{
		return m_chShortcut;
	}

	void CControlUI::SetShortcut(TCHAR ch)
	{
		m_chShortcut = ch;
	}

	bool CControlUI::IsContextMenuUsed() const
	{
		return m_bMenuUsed;
	}

	void CControlUI::SetContextMenuUsed(bool bMenuUsed)
	{
		m_bMenuUsed = bMenuUsed;
	}

	CDuiString CControlUI::GetUserData()
	{
		return m_sUserData;
	}

	void CControlUI::SetUserData(LPCTSTR pstrText)
	{
		m_sUserData = pstrText;
	}

	UINT_PTR CControlUI::GetTag() const
	{
		return m_pTag;
	}

	void CControlUI::SetTag(UINT_PTR pTag)
	{
		m_pTag = pTag;
	}

	bool CControlUI::IsVisible() const
	{
		return m_bVisible && m_bInternVisible;
	}

	void CControlUI::SetVisible(bool bVisible)
	{
		if( m_bVisible == bVisible ) return;
			
		bool v = IsVisible();
		m_bVisible = bVisible;
		if( IsFocused() ) SetFocusState(false);

		if(!m_bVisible)
		{
			m_state.m_uState &= ~(UISTATE_FOCUSED | UISTATE_HOT | UISTATE_CAPTURED | UISTATE_PUSHED);
		}

		if (!bVisible && m_pManager && m_pManager->GetFocus() == this) 
		{
			m_pManager->SetFocus(NULL) ;
		}

		if( IsVisible() != v ) {
			NeedParentUpdate();
		}
	}

	void CControlUI::SetInternVisible(bool bVisible)
	{
		m_bInternVisible = bVisible;
		if (!bVisible && m_pManager && m_pManager->GetFocus() == this) {
			m_pManager->SetFocus(NULL) ;
		}
	}

	bool CControlUI::IsMouseEnabled() const
	{
		return m_bMouseEnabled;
	}

	void CControlUI::SetMouseEnabled(bool bEnabled)
	{
		m_bMouseEnabled = bEnabled;
	}

	bool CControlUI::IsKeyboardEnabled() const
	{
		return m_bKeyboardEnabled ;
	}

	void CControlUI::SetKeyboardEnabled(bool bEnabled)
	{
		m_bKeyboardEnabled = bEnabled ; 
	}

	bool CControlUI::IsFloat() const
	{
		return m_bFloat;
	}

	void CControlUI::SetFloat(bool bFloat)
	{
		if( m_bFloat == bFloat ) return;

		m_bFloat = bFloat;
		NeedParentUpdate();
	}

	bool CControlUI::IsEnabled() const
	{
		return m_state.IsEnabled();
	}

	void CControlUI::SetEnabled(bool bEnabled)
	{
		if( IsEnabled() == bEnabled ) return;
		m_state.SetEnabled(bEnabled);
		Invalidate();
	}

	bool CControlUI::IsFocused() const					{ return m_state.IsFocused();		}
	void CControlUI::SetFocusState(bool bFocus)			{ m_state.SetFocus(bFocus);			}

	void CControlUI::SetFocus()
	{
		if( m_pManager != NULL ) m_pManager->SetFocus(this);
	}

	bool CControlUI::IsHotState() const					{ return m_state.IsHot();			}
	void CControlUI::SetHotState(bool bHot)				{ m_state.SetHot(bHot);				}

	bool CControlUI::IsCaptureState() const				{ return m_state.IsCapture();		}
	void CControlUI::SetCaptureState(bool bCaptured)	{ m_state.SetCapture(bCaptured);	}

	bool CControlUI::IsPushedState() const				{ return m_state.IsPushed();		}
	void CControlUI::SetPushedState(bool bPushed)		{ m_state.SetPushed(bPushed);		}

	void CControlUI::SetSelectedState(bool bSelected)	{ m_state.SetSelected(bSelected);	}
	bool CControlUI::IsSelectedState() const			{ return m_state.IsSelected();		}

	void CControlUI::SetReadOnly(bool bReadOnly)		{ m_state.SetReadonly(bReadOnly);	}
	bool CControlUI::IsReadOnly() const					{ return m_state.IsReadOnly();		}

	CControlUI* CControlUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
	{
		if( (uFlags & UIFIND_VISIBLE) != 0 && !IsVisible() ) return NULL;
		if( (uFlags & UIFIND_ENABLED) != 0 && !IsEnabled() ) return NULL;
		if(CPaintManagerUI::UIDESIGNMODE)
		{	//设计器中，鼠标点击无法选中控件，!m_bMouseEnabled时，应该也能通过坐标Find出来。
			//if( (uFlags & UIFIND_HITTEST) != 0 && (!::PtInRect(&m_rcItem, * static_cast<LPPOINT>(pData))) ) return NULL;
			if( (uFlags & UIFIND_HITTEST) != 0 && (!m_rcItem.PtInRect(* static_cast<LPPOINT>(pData))) ) return NULL;
		}
		else
		{
			//if( (uFlags & UIFIND_HITTEST) != 0 && (!m_bMouseEnabled || !::PtInRect(&m_rcItem, * static_cast<LPPOINT>(pData))) ) return NULL;
			if( (uFlags & UIFIND_HITTEST) != 0 && (!m_bMouseEnabled || !m_rcItem.PtInRect(* static_cast<LPPOINT>(pData))) ) return NULL;
		}
		return Proc(this, pData);
	}

	void CControlUI::Invalidate()
	{
		if( !IsVisible() ) return;

		CDuiRect invalidateRc = m_rcItem;

		CControlUI* pParent = this;
		RECT rcTemp;
		RECT rcParent;
		while( pParent = pParent->GetParent() )
		{
			rcTemp = invalidateRc;
			rcParent = pParent->GetPos();
			//if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
			if( !invalidateRc.Intersect(rcTemp, rcParent) ) 
			{
				return;
			}
		}

		if( m_pManager != NULL ) m_pManager->Invalidate(invalidateRc);
	}

	bool CControlUI::IsUpdateNeeded() const
	{
		return m_bUpdateNeeded;
	}

	void CControlUI::NeedUpdate()
	{
		if( !IsVisible() ) return;
		m_bUpdateNeeded = true;
		Invalidate();

		if( m_pManager != NULL ) m_pManager->NeedUpdate();
	}

	void CControlUI::NeedParentUpdate()
	{
		if( GetParent() ) {
			GetParent()->NeedUpdate();
			GetParent()->Invalidate();
		}
		else {
			NeedUpdate();
		}

		if( m_pManager != NULL ) m_pManager->NeedUpdate();
	}

	DWORD CControlUI::GetAdjustColor(DWORD dwColor)
	{
		if( !m_bColorHSL && !CPaintManagerUI::IsForceHSL()) return dwColor;
		if(!CPaintManagerUI::IsUseHSL()) return dwColor;
		short H, S, L;
		CPaintManagerUI::GetHSL(&H, &S, &L);
		return UIGlobal::AdjustHslColor(dwColor, H, S, L);
	}

	void CControlUI::Init()
	{
		DoInit();
		if(!m_asOnInit.IsEmpty()) 
			if(GetManager()->ExecuteScript(m_asOnInit, this))
				return;
		if( OnInit ) OnInit(this);
	}

	void CControlUI::DoInit()
	{

	}

	void CControlUI::Event(TEventUI& event)
	{
		if(!m_asOnEvent.IsEmpty()) 
			if(GetManager()->ExecuteScript(m_asOnEvent, this, &event))
				return;
		if( OnEvent(&event) ) DoEvent(event);
	}

	void CControlUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETCURSOR ) 
		{
			if( GetCursor() ) 
			{
				//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetCursor())));
				GetManager()->SetCursor(GetCursor());
			}
			else 
			{
				//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
				GetManager()->SetCursor(DUI_ARROW);
			}
			//DUITRACE(_T("UIEVENT_SETCURSOR: %s, dt=%d"), GetClass(), GetTickCount());
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			SetFocusState(true);
			Invalidate();
			return;
		}

		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			SetFocusState(false);
			Invalidate();
			return;
		}

		if( event.Type == UIEVENT_TIMER )
		{
			if(GetAnimation() != DuiAnim_null)
				OnAnimationElapse( event.wParam );
			m_pManager->SendNotify(this, DUI_MSGTYPE_TIMER, event.wParam, event.lParam);
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() ) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
				return;
			}
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
// 			if( ::PtInRect(&m_rcItem, event.ptMouse ) ) 
// 			{
// 				if(!IsHot())
// 				{
// 					SetHot(true);
// 					Invalidate();
// 				}
// 			}
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
// 			if( IsHot() ) 
// 			{
// 				SetHot(false);
// 				Invalidate();
// 			}
		}

		if( m_pParent != NULL ) m_pParent->DoEvent(event);
	}

	void CControlUI::SetVirtualWnd(LPCTSTR pstrValue)
	{
		m_sVirtualWnd = pstrValue;
		m_pManager->UsedVirtualWnd(true);
	}

	CDuiString CControlUI::GetVirtualWnd() const
	{
		CDuiString str;
		if( !m_sVirtualWnd.IsEmpty() ){
			str = m_sVirtualWnd;
		}
		else{
			CControlUI* pParent = GetParent();
			if( pParent != NULL){
				str = pParent->GetVirtualWnd();
			}
			else{
				str = _T("");
			}
		}
		return str;
	}

	void CControlUI::AddCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr)
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') || pstrAttr == NULL || pstrAttr[0] == _T('\0') ) return;
		CDuiString* pCostomAttr = new CDuiString(pstrAttr);
		if (pCostomAttr != NULL) {
			if (m_mCustomAttrHash.Find(pstrName) == NULL)
				m_mCustomAttrHash.Set(pstrName, (LPVOID)pCostomAttr);
			else
				delete pCostomAttr;
		}
	}

	LPCTSTR CControlUI::GetCustomAttribute(LPCTSTR pstrName) const
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') ) return NULL;
		CDuiString* pCostomAttr = static_cast<CDuiString*>(m_mCustomAttrHash.Find(pstrName));
		if( pCostomAttr ) return pCostomAttr->GetData();
		return NULL;
	}

	bool CControlUI::RemoveCustomAttribute(LPCTSTR pstrName)
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') ) return NULL;
		CDuiString* pCostomAttr = static_cast<CDuiString*>(m_mCustomAttrHash.Find(pstrName));
		if( !pCostomAttr ) return false;

		delete pCostomAttr;
		return m_mCustomAttrHash.Remove(pstrName);
	}

	void CControlUI::RemoveAllCustomAttribute()
	{
		CDuiString* pCostomAttr;
		for( int i = 0; i< m_mCustomAttrHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_mCustomAttrHash.GetAt(i)) {
				pCostomAttr = static_cast<CDuiString*>(m_mCustomAttrHash.Find(key));
				delete pCostomAttr;
			}
		}
		m_mCustomAttrHash.Resize();
	}

	void CControlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		// 是否样式表
		if(m_pManager != NULL) {
			LPCTSTR pStyle = m_pManager->GetStyle(pstrValue);
			if( pStyle != NULL) {
				ApplyAttributeList(pStyle);
				return;
			}
		}

		if( _tcsicmp(pstrName, _T("selectedimage")) == 0 ) SetSelectedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedhotimage")) == 0 ) SetSelectedHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedpushedimage")) == 0 ) SetSelectedPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedforeimage")) == 0 ) SetSelectedForedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedbkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("selectedtextcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("selectedfont")) == 0 ) SetSelectedFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("selectedbordercolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedBorderColor(clrColor);
		}
		//////////////////////////////////////////////////////////////////////////
		else if( _tcsicmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("pushedimage")) == 0 ) SetPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotforeimage")) == 0 ) SetHotForeImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("pushedbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("disabledbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("hottextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("pushedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("focusedtextcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusedTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("hotfont")) == 0 ) SetHotFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("pushedfont")) == 0 ) SetPushedFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("focuedfont")) == 0 ) SetFocusedFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("pushedbordercolor")) == 0 ) //add by liqs99
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetPushedBorderColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("disablebordercolor")) == 0 ) //add by liqs99
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisableBorderColor(clrColor);
		}
		//////////////////////////////////////////////////////////////////////////
		else if( _tcsicmp(pstrName, _T("align")) == 0 ) {
			if( _tcsstr(pstrValue, _T("left")) != NULL ) {
				m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
				m_uTextStyle |= DT_LEFT;
			}
			if( _tcsstr(pstrValue, _T("center")) != NULL ) {
				m_uTextStyle &= ~(DT_LEFT | DT_RIGHT );
				m_uTextStyle |= DT_CENTER;
			}
			if( _tcsstr(pstrValue, _T("right")) != NULL ) {
				m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
				m_uTextStyle |= DT_RIGHT;
			}
		}
		else if( _tcsicmp(pstrName, _T("valign")) == 0 ) {
			if( _tcsstr(pstrValue, _T("top")) != NULL ) {
				m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER | DT_WORDBREAK);
				m_uTextStyle |= (DT_TOP | DT_SINGLELINE);
			}
			else if(( _tcsstr(pstrValue, _T("vcenter")) != NULL ) || ( _tcsstr(pstrValue, _T("center")) != NULL ))
			{
				m_uTextStyle &= ~(DT_TOP | DT_BOTTOM | DT_WORDBREAK);            
				m_uTextStyle |= (DT_VCENTER | DT_SINGLELINE);
			}
			else if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
				m_uTextStyle &= ~(DT_TOP | DT_VCENTER | DT_WORDBREAK);
				m_uTextStyle |= (DT_BOTTOM | DT_SINGLELINE);
			}
		}
		else if( _tcsicmp(pstrName, _T("endellipsis")) == 0 ) {
			if( _tcsicmp(pstrValue, _T("true")) == 0 ) m_uTextStyle |= DT_END_ELLIPSIS;
			else m_uTextStyle &= ~DT_END_ELLIPSIS;
		}   
		else if( _tcsicmp(pstrName, _T("wordbreak")) == 0 ) {
			if( _tcsicmp(pstrValue, _T("true")) == 0 ) {
				m_uTextStyle &= ~DT_SINGLELINE;
				m_uTextStyle |= DT_WORDBREAK | DT_EDITCONTROL;
			}
			else {
				m_uTextStyle &= ~DT_WORDBREAK & ~DT_EDITCONTROL;
				m_uTextStyle |= DT_SINGLELINE;
			}
		}
		else if( _tcsicmp(pstrName, _T("noprefix")) == 0 ) {
			if( _tcsicmp(pstrValue, _T("true")) == 0)
			{
				m_uTextStyle |= DT_NOPREFIX;
			}
			else
			{
				m_uTextStyle = m_uTextStyle & ~DT_NOPREFIX;
			}
		}
		else if( _tcsicmp(pstrName, _T("font")) == 0 ) 
			SetFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("disabledfont")) == 0 ) 
			SetDisabledFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("textcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("disabledtextcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetTextPadding(rcTextPadding);
		}
		else if( _tcsicmp(pstrName, _T("pushedtextpadding")) == 0 ) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetPushedTextPadding(rcTextPadding);
		}
		//////////////////////////////////////////////////////////////////////////
		else if( _tcsicmp(pstrName, _T("pos")) == 0 ) {
			RECT rcPos = { 0 };
			LPTSTR pstr = NULL;
			rcPos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SIZE szXY = {rcPos.left >= 0 ? rcPos.left : rcPos.right, rcPos.top >= 0 ? rcPos.top : rcPos.bottom};
			SetFixedXY(szXY);
			SetFixedWidth(rcPos.right - rcPos.left);
			SetFixedHeight(rcPos.bottom - rcPos.top);
		}
		else if( _tcsicmp(pstrName, _T("float")) == 0 ) {
			CDuiString nValue = pstrValue;
			// 动态计算相对比例
			if(nValue.Find(',') < 0) {
				SetFloat(_tcsicmp(pstrValue, _T("true")) == 0);
			}
			else {
				TPercentInfo piFloatPercent = { 0 };
				LPTSTR pstr = NULL;
				piFloatPercent.left = _tcstod(pstrValue, &pstr);  ASSERT(pstr);
				piFloatPercent.top = _tcstod(pstr + 1, &pstr);    ASSERT(pstr);
				piFloatPercent.right = _tcstod(pstr + 1, &pstr);  ASSERT(pstr);
				piFloatPercent.bottom = _tcstod(pstr + 1, &pstr); ASSERT(pstr);
				SetFloatPercent(piFloatPercent);
				SetFloat(true);
			}
		}
		else if( _tcsicmp(pstrName, _T("floatpercent")) == 0 ) {
			RECT rcPos = { 0 };
			LPTSTR pstr = NULL;
			rcPos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);   

			TPercentInfo piFloatPercent = { 0 };
			piFloatPercent.left = (double)rcPos.left / 100.0f;
			piFloatPercent.top = (double)rcPos.top / 100.0f;
			piFloatPercent.right = (double)rcPos.right / 100.0f;
			piFloatPercent.bottom = (double)rcPos.bottom / 100.0f;
			SetFloatPercent(piFloatPercent);
		}
		else if( _tcsicmp(pstrName, _T("floatalign")) == 0) {
			UINT uAlign = GetFloatAlign();
			// 解析文字属性
			while( *pstrValue != _T('\0') ) {
				CDuiString sValue;
				while( *pstrValue == _T(',') || *pstrValue == _T(' ') ) pstrValue = ::CharNext(pstrValue);

				while( *pstrValue != _T('\0') && *pstrValue != _T(',') && *pstrValue != _T(' ') ) {
					LPTSTR pstrTemp = ::CharNext(pstrValue);
					while( pstrValue < pstrTemp) {
						sValue += *pstrValue++;
					}
				}
				if(sValue.CompareNoCase(_T("null")) == 0) {
					uAlign = 0;
				}
				if( sValue.CompareNoCase(_T("left")) == 0 ) {
					uAlign &= ~(DT_CENTER | DT_RIGHT);
					uAlign |= DT_LEFT;
				}
				else if( sValue.CompareNoCase(_T("center")) == 0 ) {
					uAlign &= ~(DT_LEFT | DT_RIGHT);
					uAlign |= DT_CENTER;
				}
				else if( sValue.CompareNoCase(_T("right")) == 0 ) {
					uAlign &= ~(DT_LEFT | DT_CENTER);
					uAlign |= DT_RIGHT;
				}
				else if( sValue.CompareNoCase(_T("top")) == 0 ) {
					uAlign &= ~(DT_BOTTOM | DT_VCENTER);
					uAlign |= DT_TOP;
				}
				else if( sValue.CompareNoCase(_T("vcenter")) == 0 ) {
					uAlign &= ~(DT_TOP | DT_BOTTOM);
					uAlign |= DT_VCENTER;
				}
				else if( sValue.CompareNoCase(_T("bottom")) == 0 ) {
					uAlign &= ~(DT_TOP | DT_VCENTER);
					uAlign |= DT_BOTTOM;
				}
			}
			SetFloatAlign(uAlign);
		}
		else if( _tcsicmp(pstrName, _T("floatvalign")) == 0) {
			UINT uAlign = GetFloatAlign();
			if( _tcsstr(pstrValue, _T("top")) != NULL ) {
				uAlign &= ~(DT_BOTTOM | DT_VCENTER);
				uAlign |= DT_TOP;
			}
			else if( _tcsstr(pstrValue, _T("vcenter")) != NULL ) {
				uAlign &= ~(DT_TOP | DT_BOTTOM);
				uAlign |= DT_VCENTER;
			}
			else if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
				uAlign &= ~(DT_TOP | DT_VCENTER);
				uAlign |= DT_BOTTOM;
			}
			SetFloatAlign(uAlign);
		}
		else if( _tcsicmp(pstrName, _T("floatposition")) == 0 ) {
			POINT pt = { 0 };
			LPTSTR pstr = NULL;
			pt.x = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			pt.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			SetFloatPosition(pt);
		}
		else if( _tcsicmp(pstrName, _T("padding")) == 0 ) {
			RECT rcPadding = { 0 };
			LPTSTR pstr = NULL;
			rcPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetPadding(rcPadding);
		}
		else if( _tcsicmp(pstrName, _T("gradient")) == 0 ) 
		{
			bool bVer = (_tcsicmp(_T("hor"), pstrValue) != 0);
			SetGradient(bVer);
		}
		else if( _tcsicmp(pstrName, _T("bkcolor")) == 0 || _tcsicmp(pstrName, _T("bkcolor1")) == 0 ) {
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("bkcolor2")) == 0 ) {
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetBkColor2(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("bkcolor3")) == 0 ) {
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetBkColor3(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("hotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("focusbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("forecolor")) == 0 ) {
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetForeColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("bordercolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetBorderColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("focusbordercolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetFocusBorderColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("hotbordercolor")) == 0 ) { //add by liqs99
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotBorderColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("disablebordercolor")) == 0 ) { //add by liqs99
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisableBorderColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("colorhsl")) == 0 ) SetColorHSL(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("bordersize")) == 0 ) {
			CDuiString nValue = pstrValue;
			if(nValue.Find(',') < 0) {
				SetBorderSize(_ttoi(pstrValue));
				RECT rcBorder = {0};
				SetBorderRectSize(rcBorder);
			}
			else {
				RECT rcBorder = { 0 };
				LPTSTR pstr = NULL;
				rcBorder.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
				rcBorder.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
				rcBorder.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
				rcBorder.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
				SetBorderRectSize(rcBorder);
			}
		}
		else if( _tcsicmp(pstrName, _T("leftbordersize")) == 0 ) SetLeftBorderSize(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("topbordersize")) == 0 ) SetTopBorderSize(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("rightbordersize")) == 0 ) SetRightBorderSize(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("bottombordersize")) == 0 ) SetBottomBorderSize(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("borderstyle")) == 0 ) SetBorderStyle(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("borderround")) == 0 ) {
			SIZE cxyRound = { 0 };
			LPTSTR pstr = NULL;
			cxyRound.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			cxyRound.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			SetBorderRound(cxyRound);
		}
		else if( _tcsicmp(pstrName, _T("bkimage")) == 0 ) SetBkImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("foreimage")) == 0 ) SetForeImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("width")) == 0 ) SetFixedWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("height")) == 0 ) SetFixedHeight(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("widthpercent")) == 0 ) SetFixedWidthPercent(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("heightpercent")) == 0 ) SetFixedHeightPercent(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("minwidth")) == 0 ) SetMinWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("minheight")) == 0 ) SetMinHeight(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("maxwidth")) == 0 ) SetMaxWidth(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("maxheight")) == 0 ) SetMaxHeight(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("name")) == 0 ) SetName(pstrValue);
		else if( _tcsicmp(pstrName, _T("drag")) == 0 ) SetDragEnable(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("drop")) == 0 ) SetDropEnable(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("acceptfile")) == 0 ) SetAcceptDropFile(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("resourcetext")) == 0 ) SetResourceText(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("resourceid")) == 0 ) 
			SetResourceID(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("text")) == 0 ) SetText(pstrValue);
		else if( _tcsicmp(pstrName, _T("tooltip")) == 0 ) SetToolTip(pstrValue);
		else if( _tcsicmp(pstrName, _T("userdata")) == 0 ) SetUserData(pstrValue);
		else if( _tcsicmp(pstrName, _T("enabled")) == 0 ) SetEnabled(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("readonly")) == 0 ) SetReadOnly(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("mouse")) == 0 ) SetMouseEnabled(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("keyboard")) == 0 ) SetKeyboardEnabled(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("visible")) == 0 ) SetVisible(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("float")) == 0 ) SetFloat(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("shortcut")) == 0 ) SetShortcut(pstrValue[0]);
		else if( _tcsicmp(pstrName, _T("menu")) == 0 ) SetContextMenuUsed(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("cursor")) == 0 && pstrValue) {
			if( _tcsicmp(pstrValue, _T("arrow")) == 0 )			SetCursor(DUI_ARROW);
			else if( _tcsicmp(pstrValue, _T("ibeam")) == 0 )	SetCursor(DUI_IBEAM);
			else if( _tcsicmp(pstrValue, _T("wait")) == 0 )		SetCursor(DUI_WAIT);
			else if( _tcsicmp(pstrValue, _T("cross")) == 0 )	SetCursor(DUI_CROSS);
			else if( _tcsicmp(pstrValue, _T("uparrow")) == 0 )	SetCursor(DUI_UPARROW);
			else if( _tcsicmp(pstrValue, _T("size")) == 0 )		SetCursor(DUI_SIZE);
			else if( _tcsicmp(pstrValue, _T("icon")) == 0 )		SetCursor(DUI_ICON);
			else if( _tcsicmp(pstrValue, _T("sizenwse")) == 0 )	SetCursor(DUI_SIZENWSE);
			else if( _tcsicmp(pstrValue, _T("sizenesw")) == 0 )	SetCursor(DUI_SIZENESW);
			else if( _tcsicmp(pstrValue, _T("sizewe")) == 0 )	SetCursor(DUI_SIZEWE);
			else if( _tcsicmp(pstrValue, _T("sizens")) == 0 )	SetCursor(DUI_SIZENS);
			else if( _tcsicmp(pstrValue, _T("sizeall")) == 0 )	SetCursor(DUI_SIZEALL);
			else if( _tcsicmp(pstrValue, _T("no")) == 0 )		SetCursor(DUI_NO);
			else if( _tcsicmp(pstrValue, _T("hand")) == 0 )		SetCursor(DUI_HAND);
		}
		else if( _tcsicmp(pstrName, _T("virtualwnd")) == 0 ) SetVirtualWnd(pstrValue);
		else if( _tcsicmp(pstrName, _T("innerstyle")) == 0 ) {
			CDuiString sXmlData = pstrValue;
			sXmlData.Replace(_T("&quot;"), _T("\""));
			LPCTSTR pstrList = sXmlData.GetData();
			CDuiString sItem;
			CDuiString sValue;
			while( *pstrList != _T('\0') ) {
				sItem.Empty();
				sValue.Empty();
				while( *pstrList != _T('\0') && *pstrList != _T('=') ) {
					LPTSTR pstrTemp = ::CharNext(pstrList);
					while( pstrList < pstrTemp) {
						sItem += *pstrList++;
					}
				}
				ASSERT( *pstrList == _T('=') );
				if( *pstrList++ != _T('=') ) return;
				ASSERT( *pstrList == _T('\"') );
				if( *pstrList++ != _T('\"') ) return;
				while( *pstrList != _T('\0') && *pstrList != _T('\"') ) {
					LPTSTR pstrTemp = ::CharNext(pstrList);
					while( pstrList < pstrTemp) {
						sValue += *pstrList++;
					}
				}
				ASSERT( *pstrList == _T('\"') );
				if( *pstrList++ != _T('\"') ) return;
				SetAttribute(sItem, sValue);
				if( *pstrList++ != _T(' ') && *pstrList++ != _T(',') ) return;
			}
		}
		else if( _tcsicmp(pstrName, _T("autocalcwidth")) == 0 ) {
			SetAutoCalcWidth(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcsicmp(pstrName, _T("autocalcheight")) == 0 ) {
			SetAutoCalcHeight(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcsicmp(pstrName, _T("animation")) == 0 ) 
		{
			if(_tcsicmp(pstrValue, _T("null")) == 0)
				SetAnimation(DuiAnim_null);
			else if(_tcsicmp(pstrValue, _T("vertical")) == 0)
				SetAnimation(DuiAnim_vertical);
			else if(_tcsicmp(pstrValue, _T("horizontal")) == 0)
				SetAnimation(DuiAnim_horizontal);
			else if(_tcsicmp(pstrValue, _T("scalevertical")) == 0)
				SetAnimation(DuiAnim_ScaleVertical);
			else if(_tcsicmp(pstrValue, _T("scalehorizontal")) == 0)
				SetAnimation(DuiAnim_ScaleHorizontal);
			else if(_tcsicmp(pstrValue, _T("scalesize")) == 0)
				SetAnimation(DuiAnim_ScaleSize);
			else if(_tcsicmp(pstrValue, _T("alpha")) == 0)
				SetAnimation(DuiAnim_Alpha);
		}
		else if( _tcsicmp(pstrName, _T("framecount")) == 0 ) {
			SetFrameCount(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("framedelay")) == 0 ) {
			SetFrameDelay(_ttoi(pstrValue));
		}
		else if( _tcscmp(pstrName, _T("OnInit"))			== 0 )		m_asOnInit			= pstrValue;
		else if( _tcscmp(pstrName, _T("OnEvent"))			== 0 )		m_asOnEvent			= pstrValue;
		else if( _tcscmp(pstrName, _T("OnNotify"))			== 0 )		m_asOnNotify		= pstrValue;
		else if( _tcscmp(pstrName, _T("OnDestroy"))			== 0 )		m_asOnDestroy		= pstrValue;
		else if( _tcscmp(pstrName, _T("OnSize"))			== 0 )		m_asOnSize			= pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaint"))			== 0 )		m_asOnPaint			= pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaintBkColor"))	== 0 )		m_asOnPaintBkColor	= pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaintBkImage"))	== 0 )		m_asOnPaintBkImage	= pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaintStatusImage"))== 0 )		m_asOnPaintStatusImage = pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaintForeColor"))	== 0 )		m_asOnPaintForeColor = pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaintForeImage"))	== 0 )		m_asOnPaintForeImage = pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaintText"))	== 0 )			m_asOnPaintText		= pstrValue;
		else if( _tcscmp(pstrName, _T("OnPaintBorder"))	== 0 )			m_asOnPaintBorder	= pstrValue;
		else {
			AddCustomAttribute(pstrName, pstrValue);
		}
	}

	void CControlUI::ApplyAttributeList(LPCTSTR pstrValue)
	{
		// 解析样式表
		if(m_pManager != NULL) {
			LPCTSTR pStyle = m_pManager->GetStyle(pstrValue);
			if( pStyle != NULL) {
				return ApplyAttributeList(pStyle);
			}
		}
		CDuiString sXmlData = pstrValue;
		sXmlData.Replace(_T("&quot;"), _T("\""));
		LPCTSTR pstrList = sXmlData.GetData();
		// 解析样式属性
		CDuiString sItem;
		CDuiString sValue;
		while( *pstrList != _T('\0') ) {
			sItem.Empty();
			sValue.Empty();
			while( *pstrList != _T('\0') && *pstrList != _T('=') ) {
				LPTSTR pstrTemp = ::CharNext(pstrList);
				while( pstrList < pstrTemp) {
					sItem += *pstrList++;
				}
			}
			//ASSERT( *pstrList == _T('=') ); //可能让设计器崩溃
			if( *pstrList != _T('=') ) return;	

			if( *pstrList++ != _T('=') ) return;

			//ASSERT( *pstrList == _T('\"') ); //可能让设计器崩溃
			if( *pstrList != _T('\"') )	return;

			if( *pstrList++ != _T('\"') ) return;

			//这段无法满足嵌套的style定义
// 			while( *pstrList != _T('\0') && *pstrList != _T('\"') ) {
// 				LPTSTR pstrTemp = ::CharNext(pstrList);
// 				while( pstrList < pstrTemp) {
// 					sValue += *pstrList++;
// 				}
// 			}
			//改为
			bool bNewAttr = false;
			int deep = 0;
			while( *pstrList != _T('\0') ) {
				if(*pstrList == _T('='))
				{
					bNewAttr = true;
				}
				else if(*pstrList == _T('\"') || *pstrList == _T('\'') )
				{
					if(bNewAttr)
					{
						if(deep == 0)
						{
							deep = 1;
						}
						else
						{
							deep = 0;
							bNewAttr = false;
						}
					}
					else
						break;
				}
				sValue += *pstrList++;
			}

			//ASSERT( *pstrList == _T('\"') ); //可能让设计器崩溃
			if( *pstrList != _T('\"') )	return;

			if( *pstrList++ != _T('\"') ) return;
			SetAttribute(sItem, sValue);
			if( *pstrList++ != _T(' ') && *pstrList++ != _T(',') ) return;
		}
		return;
	}

	SIZE CControlUI::EstimateSize(SIZE szAvailable)
	{
		if(IsAnimationRunning(ANIMATION_ID_SHOW) || IsAnimationRunning(ANIMATION_ID_HIDE)) {
			return m_szAnimationCurrect;
		}

		if(!IsPaneVisible())
		{
			return m_szAnimationCurrect;
		}

		if(m_pManager != NULL)
			return m_pManager->GetDPIObj()->ScaleSize(m_cxyFixed);
		return m_cxyFixed;
	}

	bool CControlUI::Paint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl)
	{
		if (pStopControl == this) return false;
		//if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return true;
		if( !m_rcPaint.Intersect(rcPaint, m_rcItem) ) return true;
		if(!m_asOnPaint.IsEmpty())
		{
			if(GetManager()->ExecuteScript(m_asOnPaint, this, pRender, rcPaint, pStopControl)) 
				return true;
		}
		if( OnPaint ) {
			if( !OnPaint(pRender) ) return true;
		}
		if (!DoPaint(pRender, m_rcPaint, pStopControl)) return false;
		return true;
	}

	bool CControlUI::DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl)
	{
		// 绘制循序：背景颜色->背景图->状态图->文本->边框
		SIZE cxyBorderRound;
		RECT rcBorderSize;
		if (m_pManager) {
			cxyBorderRound = GetManager()->GetDPIObj()->ScaleSize(m_cxyBorderRound);
			rcBorderSize = GetManager()->GetDPIObj()->ScaleRect(m_rcBorderSize);
		}
		else {
			cxyBorderRound = m_cxyBorderRound;
			rcBorderSize = m_rcBorderSize;
		}

		if( cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 ) {
			UIClip roundClip;
			roundClip.GenerateRoundClip(pRender, m_rcPaint,  m_rcItem, cxyBorderRound.cx, cxyBorderRound.cy);
			PaintBkColor(pRender);
			PaintBkImage(pRender);
			PaintStatusImage(pRender);
			PaintForeColor(pRender);
			PaintForeImage(pRender);
			PaintText(pRender);
			PaintBorder(pRender);
		}
		else {
			PaintBkColor(pRender);
			PaintBkImage(pRender);
			PaintStatusImage(pRender);
			PaintForeColor(pRender);
			PaintForeImage(pRender);
			PaintText(pRender);
			PaintBorder(pRender);
		}
		return true;
	}

	//
	//
	//  状态显示的优先级：  禁用  >  选中  >  按下  >  悬停  >  焦点
	//
	//

	void CControlUI::PaintBkColor(UIRender *pRender)
	{
		DWORD dwColor = 0;

		if( !IsEnabled() )
			dwColor = GetDisabledBkColor();

		if( dwColor == 0 && IsSelectedState() )
			dwColor = GetSelectBkColor();

		if( dwColor == 0 && IsPushedState() )
			dwColor = GetPushedBkColor();

		if( dwColor == 0 && IsHotState() )
			dwColor = GetHotBkColor();

		if( dwColor == 0 && IsFocused() )
			dwColor = GetFocusBkColor();

		if(dwColor == 0)
			dwColor = GetBkColor();

		if(dwColor == 0) return;
		pRender->DrawBackColor(m_rcItem, GetBorderRound(), 
			GetAdjustColor(dwColor), 
			GetAdjustColor(GetBkColor2()), 
			GetAdjustColor(GetBkColor3()), 
			GetGradient());
	}

	void CControlUI::PaintBkImage(UIRender *pRender)
	{
		if( m_sBkImage.IsEmpty() ) return;
		DrawImage(pRender, (LPCTSTR)m_sBkImage);
	}

	void CControlUI::PaintStatusImage(UIRender *pRender)
	{
		CDuiString sImage;
		
		if(IsSelectedState())
		{
			if( !IsEnabled() )
				sImage = GetSelectedDisabledImage();

			if( sImage.IsEmpty() && IsPushedState() )
				sImage = GetSelectedPushedImage();

			if( sImage.IsEmpty() && IsHotState() )
				sImage = GetSelectedHotImage();

			if( sImage.IsEmpty() && IsFocused() )
				sImage = GetSelectedFocusedImage();

			if(sImage.IsEmpty())
				sImage = GetSelectedImage();
		}
		else
		{
			if( !IsEnabled() )
				sImage = GetDisabledImage();

			else if( sImage.IsEmpty() && IsPushedState() )
				sImage = GetPushedImage();

			else if( sImage.IsEmpty() && IsHotState() )
				sImage = GetHotImage();

			else if( sImage.IsEmpty() && IsFocused() )
				sImage = GetFocusedImage();
		}

		if(sImage.IsEmpty())
			sImage = GetNormalImage();

		if( sImage.IsEmpty() ) return;
		DrawImage(pRender, sImage);
	}

	void CControlUI::PaintForeColor(UIRender *pRender)
	{
		if(m_dwForeColor == 0) 
			return;
		pRender->DrawColor(m_rcItem, GetBorderRound(), GetAdjustColor(m_dwForeColor));
	}
	
	void CControlUI::PaintForeImage(UIRender *pRender)
	{
		CDuiString sImage;
		if( !IsEnabled() )
			sImage = GetDisabledForeImage();

		else if( IsSelectedState() )
			sImage = GetSelectedForeImage();

		else if( IsPushedState() )
			sImage = GetPushedForeImage();

		else if( IsHotState() )
			sImage = GetHotForeImage();

		else if( IsFocused() )
			sImage = GetFocusedForeImage();

		if(sImage.IsEmpty())
			sImage = GetForeImage();

		if( sImage.IsEmpty() ) return;
		DrawImage(pRender, sImage);
	}

	void CControlUI::PaintText(UIRender *pRender)
	{/*
		CDuiString sText = GetText();
		if(sText.IsEmpty()) return;

		RECT rcText = m_rcItem;
		DWORD dwColor = 0;
		int iFont = -1;

		//////////////////////////////////////////////////////////////////////////
		if( !IsEnabled() )
			iFont = GetDisabledFont();

		else if( IsPushedState() )
			iFont = GetPushedFont();

		else if( IsHotState() )
			iFont = GetHotFont();

		else if( IsSelectedState() )
			iFont = GetSelectedFont();

		else if( IsFocused() )
			iFont = GetFocusedFont();

		if(iFont == -1)
			iFont = GetFont();

		//////////////////////////////////////////////////////////////////////////
		if( !IsEnabled() )
			dwColor = GetDisabledTextColor();

		else if( IsHotState() )
			dwColor = GetHotTextColor();

		else if( IsPushedState() )
			dwColor = GetPushedTextColor();

		else if( IsSelectedState() )
			dwColor = GetSelectedTextColor();

		else if( IsFocused() )
			dwColor = GetFocusedTextColor();

		if(dwColor == 0)
			dwColor = GetTextColor();

		if(dwColor == 0 && m_pManager)
			dwColor = m_pManager->GetDefaultFontColor();

		pRender->DrawText(rcText, GetTextPadding(), sText, dwColor, iFont, GetTextStyle());
		return;
		*/
	}

	void CControlUI::PaintBorder(UIRender *pRender)
	{
		DWORD dwColor = 0;

		if( !IsEnabled() )
			dwColor = GetDisableBorderColor();

		else if( IsSelectedState() )
			dwColor = GetSelectedBorderColor();

		else if( IsPushedState() )
			dwColor = GetPushedBorderColor();

		else if( IsHotState() )
			dwColor = GetHotBorderColor();

		else if( IsFocused() )
			dwColor = GetFocusBorderColor();

		if(dwColor == 0) 
			dwColor = GetBorderColor();

		if(dwColor == 0) return;
		pRender->DrawBorder(m_rcItem, GetBorderSize(), GetBorderRound(), 
			GetBorderRectSize(), GetAdjustColor(dwColor), GetBorderStyle());
	}

	void CControlUI::DoPostPaint(UIRender *pRender, const RECT& rcPaint)
	{
		return;
	}

	int CControlUI::GetLeftBorderSize() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleInt(m_rcBorderSize.left);
		return m_rcBorderSize.left;
	}

	void CControlUI::SetLeftBorderSize( int nSize )
	{
		m_rcBorderSize.left = nSize;
		Invalidate();
	}

	int CControlUI::GetTopBorderSize() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleInt(m_rcBorderSize.top);
		return m_rcBorderSize.top;
	}

	void CControlUI::SetTopBorderSize( int nSize )
	{
		m_rcBorderSize.top = nSize;
		Invalidate();
	}

	int CControlUI::GetRightBorderSize() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleInt(m_rcBorderSize.right);
		return m_rcBorderSize.right;
	}

	void CControlUI::SetRightBorderSize( int nSize )
	{
		m_rcBorderSize.right = nSize;
		Invalidate();
	}

	int CControlUI::GetBottomBorderSize() const
	{
		if(m_pManager != NULL) return m_pManager->GetDPIObj()->ScaleInt(m_rcBorderSize.bottom);
		return m_rcBorderSize.bottom;
	}

	void CControlUI::SetBottomBorderSize( int nSize )
	{
		m_rcBorderSize.bottom = nSize;
		Invalidate();
	}

	int CControlUI::GetBorderStyle() const
	{
		return m_nBorderStyle;
	}

	void CControlUI::SetBorderStyle( int nStyle )
	{
		m_nBorderStyle = nStyle;
		Invalidate();
	}


	void CControlUI::SetPaneVisible(bool bVisible)
	{
		if( m_bPaneVisible == bVisible ) return;

		if(m_animation == DuiAnim_ScaleVertical || m_animation == DuiAnim_ScaleHorizontal || m_animation == DuiAnim_ScaleSize)
		{
			if(bVisible)
			{
				StopAnimation(ANIMATION_ID_SHOW);

				m_bPaneVisible = bVisible;

				if(m_animation == DuiAnim_ScaleVertical) {
					m_szAnimationCurrect.cx = m_szAnimationTotal.cx;
					m_szAnimationCurrect.cy = 1;
				}
				if(m_animation == DuiAnim_ScaleHorizontal) {
					m_szAnimationCurrect.cx = 1;
					m_szAnimationCurrect.cy = m_szAnimationTotal.cy;
				}
				if(m_animation == DuiAnim_ScaleSize) {
					m_szAnimationCurrect.cx = 1;
					m_szAnimationCurrect.cy = 1;
				}

				//SetVisible(true);

				m_szAnimationTotal = m_cxyFixed;
				CDuiRect rcChild;
				if(GetParent() && GetParent()->CalcPos(this, rcChild))
				{
					m_szAnimationTotal.cx = rcChild.right - rcChild.left;
					m_szAnimationTotal.cy = rcChild.bottom - rcChild.top;
				}

				if(m_animation == DuiAnim_ScaleVertical) {
					m_szAnimationCurrect.cx = m_szAnimationTotal.cx;
					m_szAnimationCurrect.cy = 1;
				}
				if(m_animation == DuiAnim_ScaleHorizontal) {
					m_szAnimationCurrect.cx = 1;
					m_szAnimationCurrect.cy = m_szAnimationTotal.cy;
				}
				if(m_animation == DuiAnim_ScaleSize) {
					m_szAnimationCurrect.cx = 1;
					m_szAnimationCurrect.cy = 1;
				}
				StartAnimation(GetFrameDelay(), GetFrameCount(), ANIMATION_ID_SHOW);
			}
			else
			{
				StopAnimation(ANIMATION_ID_HIDE);

				m_szAnimationTotal = m_cxyFixed;
				CDuiRect rcChild;
				if(GetParent() && GetParent()->CalcPos(this, rcChild))
				{
					m_szAnimationTotal.cx = rcChild.right - rcChild.left;
					m_szAnimationTotal.cy = rcChild.bottom - rcChild.top;
				}

				m_szAnimationCurrect = m_szAnimationTotal;

				m_bPaneVisible = bVisible;
				StartAnimation(GetFrameDelay(), GetFrameCount(), ANIMATION_ID_HIDE);
			}
			return;
		}
	}

	bool CControlUI::IsPaneVisible() const
	{
		return m_bPaneVisible;
	}

	void CControlUI::SetAnimation(DuiAnim emAnim) { m_animation = emAnim; }
	DuiAnim CControlUI::GetAnimation() const { return m_animation; }
	void CControlUI::SetFrameCount(int framecount) { m_nFrameCount = framecount; }
	int CControlUI::GetFrameCount() const { return m_nFrameCount; }
	void CControlUI::SetFrameDelay(int nDelay) { m_nFrameDelay = nDelay; }
	int CControlUI::GetFrameDelay() const { return m_nFrameDelay; }

	void CControlUI::OnAnimationStart(int nAnimationID, BOOL bFirstLoop)
	{
		if(nAnimationID == ANIMATION_ID_SHOW)
		{
			SetVisible(true);
		}
	}

	void CControlUI::OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID)
	{
		if(nAnimationID == ANIMATION_ID_SHOW)
		{
			if(m_animation == DuiAnim_ScaleVertical){
				m_szAnimationCurrect.cy = m_szAnimationTotal.cy / nTotalFrame * nCurFrame;
			}
			else if(m_animation == DuiAnim_ScaleHorizontal) {
				m_szAnimationCurrect.cx = m_szAnimationTotal.cx / nTotalFrame * nCurFrame;
			}
			else if(m_animation == DuiAnim_ScaleSize){
				m_szAnimationCurrect.cx = m_szAnimationTotal.cx / nTotalFrame * nCurFrame;
				m_szAnimationCurrect.cy = m_szAnimationTotal.cy / nTotalFrame * nCurFrame;
			}	
			//CMsgWndUI::InsertMsgV(_T("OnAnimationStep SHOW pControl=%p, m_szAnimationCurrect=%s"), this, m_szAnimationCurrect.ToString().toString());
		}
		else if(nAnimationID == ANIMATION_ID_HIDE)
		{
			if(m_animation == DuiAnim_ScaleVertical){
				m_szAnimationCurrect.cy = m_szAnimationTotal.cy - m_szAnimationTotal.cy / nTotalFrame * nCurFrame;
			}
			else if(m_animation == DuiAnim_ScaleHorizontal) {
				m_szAnimationCurrect.cx = m_szAnimationTotal.cx - m_szAnimationTotal.cx / nTotalFrame * nCurFrame;
			}
			else if(m_animation == DuiAnim_ScaleSize){
				m_szAnimationCurrect.cx = m_szAnimationTotal.cx - m_szAnimationTotal.cx / nTotalFrame * nCurFrame;
				m_szAnimationCurrect.cy = m_szAnimationTotal.cy - m_szAnimationTotal.cy / nTotalFrame * nCurFrame;
			}
			//CMsgWndUI::InsertMsgV(_T("OnAnimationStep HIDE pControl=%p, m_szAnimationCurrect=%s"), this, m_szAnimationCurrect.ToString().toString());
		}
		NeedUpdate();
		NeedParentUpdate();
	}

	void CControlUI::OnAnimationStop(int nAnimationID)
	{
		if(nAnimationID == ANIMATION_ID_HIDE)
		{
			if(m_animation == DuiAnim_ScaleVertical){
				m_szAnimationCurrect.cy = 1;
			}
			else if(m_animation == DuiAnim_ScaleHorizontal) {
				m_szAnimationCurrect.cx = 1;
			}
			else if(m_animation == DuiAnim_ScaleSize){
				m_szAnimationCurrect.cx = 1;
				m_szAnimationCurrect.cy = 1;
			}
			SetVisible(false);
		}
		NeedParentUpdate();
	}

	void CControlUI::__AddRef()
	{
		__refCount++;
	}

	void CControlUI::__ReleaseRef()
	{
		if( --__refCount == 0 )
			delete this;
	}

	void CControlUI::SetExtraParent(CControlUI *pControl)
	{
		m_pExtraParent = pControl;
	}

	CControlUI *CControlUI::GetExtraParent() const
	{
		return m_pExtraParent;
	}
} // namespace DuiLib

