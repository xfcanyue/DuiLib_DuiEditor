#pragma once

namespace DuiLib
{

template <typename T>
class regCControlUI
{
public:
	asIScriptEngine *engine;
	CStringA classname;
	DECL_CONTROL_FACTORY(CControlUI);
	DECL_CONTROL_REGFACT(CControlUI);
public:
	virtual void reg(asIScriptEngine *engine_)
	{
		engine = engine_;
		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 

		REG_CONTROL_FACTORY();

		REG_CONTROL_FUNPR(void,		 SetName,			(LPCTSTR)		);
		REG_CONTROL_FUNPR(CDuiString,  GetName,			() const		);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetClass,			() const		);
		REG_CONTROL_FUNPR(LPVOID,	 GetInterface,		(LPCTSTR pstrName));
		REG_CONTROL_FUNPR(UINT,		 GetControlFlags,	() const		);
		REG_CONTROL_FUNPR(bool,		 Activate,			()				);

		REG_CONTROL_FUNPR(CPaintManagerUI*, GetManager, ()const								);
		REG_CONTROL_FUNPR(void,  SetManager, (CPaintManagerUI*, CControlUI*, bool)			);
		REG_CONTROL_FUNPR(CControlUI*, GetParent, ()const									);

		REG_CONTROL_FUNPR(bool,		  SetTimer,				(UINT nTimerID, UINT nElapse)	);
		REG_CONTROL_FUNPR(void,		  KillTimer,			(UINT nTimerID)					);
		REG_CONTROL_FUNPR(CDuiString, GetText,				() const						);
		REG_CONTROL_FUNPR(void,		  SetText,				(LPCTSTR pstrText)				);
		REG_CONTROL_FUNPR(int,		  GetText_to_int,		()								);
		REG_CONTROL_FUNPR(void,		  SetText,				(int n)							);
		REG_CONTROL_FUNPR(bool,		  IsResourceText,		() const						);
		REG_CONTROL_FUNPR(void,		  SetResourceText,		(bool bResource)				);
		REG_CONTROL_FUNPR(int,		  GetResourceID,		() const						);
		REG_CONTROL_FUNPR(void,		  SetResourceID,		(int resid)						);
		//REG_CONTROL_FUNPR(CLangPackageUI *, GetLangPackage, ()								);
		REG_CONTROL_FUNPR(void,		  SetSkinFile,			(LPCTSTR lpstrSkinFile)			);
		REG_CONTROL_FUNPR(CDuiString, GetSkinFile,			()								);
		REG_CONTROL_FUNPR(bool,		  IsDragEnabled,		() const						);
		REG_CONTROL_FUNPR(void,		  SetDragEnable,		(bool bDrag)					);
		REG_CONTROL_FUNPR(bool,		  IsDropEnabled,		() const						);
		REG_CONTROL_FUNPR(void,		  SetDropEnable,		(bool bDrop)					);

		// 图形相关
		REG_CONTROL_FUNPR(LPCTSTR,	   GetGradient,			()								);
		REG_CONTROL_FUNPR(void,		   SetGradient,			(LPCTSTR pStrImage)				);
		REG_CONTROL_FUNPR(DWORD,		GetBkColor,			() const						);
		REG_CONTROL_FUNPR(void,		   SetBkColor,			(DWORD dwBackColor)				);
		REG_CONTROL_FUNPR(DWORD,		GetBkColor2,		() const						);
		REG_CONTROL_FUNPR(void,		   SetBkColor2,			(DWORD dwBackColor)				);
		REG_CONTROL_FUNPR(DWORD,		GetBkColor3,		() const						);
		REG_CONTROL_FUNPR(void,		   SetBkColor3,			(DWORD dwBackColor)				);
		REG_CONTROL_FUNPR(DWORD,		GetForeColor,		() const						);
		REG_CONTROL_FUNPR(void,		   SetForeColor,		(DWORD dwForeColor)				);
		REG_CONTROL_FUNPR(LPCTSTR,	   GetBkImage,			()								);
		REG_CONTROL_FUNPR(void,		   SetBkImage,			(LPCTSTR pStrImage)				);
		REG_CONTROL_FUNPR(LPCTSTR,	   GetForeImage,		() const						);
		REG_CONTROL_FUNPR(void,		   SetForeImage,		(LPCTSTR pStrImage)				);

		REG_CONTROL_FUNPR(DWORD,	   GetFocusBorderColor,		() const					);
		REG_CONTROL_FUNPR(void,		   SetFocusBorderColor,		(DWORD dwBorderColor)		);
		REG_CONTROL_FUNPR(DWORD,	   GetHotBorderColor,		() const					); 
		REG_CONTROL_FUNPR(void,		   SetHotBorderColor,		(DWORD dwBorderColor)		); 
		REG_CONTROL_FUNPR(bool,		   IsColorHSL,				() const					);
		REG_CONTROL_FUNPR(void,		   SetColorHSL,				(bool bColorHSL)			);
		REG_CONTROL_FUNPR(SIZE,		   GetBorderRound,			() const					);
		REG_CONTROL_FUNPR(void,		   SetBorderRound,			(SIZE cxyRound)				);
		//REG_CONTROL_FUNPR(bool,		   DrawImage,				(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify)		);

		//边框相关
		REG_CONTROL_FUNPR(int,		   GetBorderSize,			() const					);
		REG_CONTROL_FUNPR(void,		   SetBorderSize,			(int nSize)					);
		REG_CONTROL_FUNPR(DWORD,	   GetBorderColor,			() const					);
		REG_CONTROL_FUNPR(void,		   SetBorderColor,			(DWORD dwBorderColor)		);
		REG_CONTROL_FUNPR(void,		   SetBorderSize,			(RECT rc)					);
		REG_CONTROL_FUNPR(int,		   GetLeftBorderSize,		() const					);
		REG_CONTROL_FUNPR(void,		   SetLeftBorderSize,		(int nSize)					);
		REG_CONTROL_FUNPR(int,		   GetTopBorderSize,		() const					);
		REG_CONTROL_FUNPR(void,		   SetTopBorderSize,		(int nSize)					);
		REG_CONTROL_FUNPR(int,		   GetRightBorderSize,		() const					);
		REG_CONTROL_FUNPR(void,		   SetRightBorderSize,		(int nSize)					);
		REG_CONTROL_FUNPR(int,		   GetBottomBorderSize,		() const					);
		REG_CONTROL_FUNPR(void,		   SetBottomBorderSize,		(int nSize)					);
		REG_CONTROL_FUNPR(int,		   GetBorderStyle,			() const					);
		REG_CONTROL_FUNPR(void,		   SetBorderStyle,			(int nStyle)				);

		// 位置相关
		REG_CONTROL_FUNPR(RECT,			  GetRelativePos,	() const				); 
		REG_CONTROL_FUNPR(RECT,			  GetClientPos,		() const				); 
		REG_CONTROL_FUNPR(const RECT&,	  GetPos,			() const				);
		REG_CONTROL_FUNPR(void,			  SetPos,			(RECT rc, bool bNeedInvalidate)				);
		//REG_CONTROL_FUN(bool,			  CalcPos,			(CControlUI *pChildControl, RECT &rcChild)	); 
		REG_CONTROL_FUNPR(void,			  Move,				(SIZE szOffset, bool bNeedInvalidate)		);
		REG_CONTROL_FUNPR(int,			  GetWidth,			() const				);
		REG_CONTROL_FUNPR(int,			  GetHeight,		() const				);
		REG_CONTROL_FUNPR(int,			  GetX,				() const				);
		REG_CONTROL_FUNPR(int,			  GetY,				() const				);
		REG_CONTROL_FUNPR(RECT,			  GetPadding,		() const				);
		REG_CONTROL_FUNPR(void,			  SetPadding,		(RECT rcPadding)		); 
		REG_CONTROL_FUNPR(SIZE,			  GetFixedXY,		() const				);         
		REG_CONTROL_FUNPR(void,			  SetFixedXY,		(SIZE szXY)				);     
		REG_CONTROL_FUNPR(int,			  GetFixedWidth,	() const				);       
		REG_CONTROL_FUNPR(void,			  SetFixedWidth,	(int cx)				);     
		REG_CONTROL_FUNPR(int,			  GetFixedHeight,	() const				);   
		REG_CONTROL_FUNPR(void,			  SetFixedHeight,	(int cy)				); 
		REG_CONTROL_FUNPR(int,			  GetMinWidth,		() const				);
		REG_CONTROL_FUNPR(void,			  SetMinWidth,		(int cx)				);
		REG_CONTROL_FUNPR(int,			  GetMaxWidth,		() const				);
		REG_CONTROL_FUNPR(void,			  SetMaxWidth,		(int cx)				);
		REG_CONTROL_FUNPR(int,			  GetMinHeight,		() const				);
		REG_CONTROL_FUNPR(void,			  SetMinHeight,		(int cy)				);
		REG_CONTROL_FUNPR(int,			  GetMaxHeight,		() const				);
		REG_CONTROL_FUNPR(void,			  SetMaxHeight,		(int cy)				);
		//REG_CONTROL_FUN(TPercentInfo,	  GetFloatPercent,	() const				);
		//REG_CONTROL_FUN(void,			  SetFloatPercent,	(TPercentInfo piFloatPercent));
		REG_CONTROL_FUNPR(void,			  SetFloatAlign,	(UINT uAlign)			);
		REG_CONTROL_FUNPR(UINT,			  GetFloatAlign,	() const				);


		REG_CONTROL_FUNPR(bool,	 IsAutoCalcWidth,		() const				);
		REG_CONTROL_FUNPR(void,	 SetAutoCalcWidth,		(bool bAutoCalcWidth)	);
		REG_CONTROL_FUNPR(bool,	 IsAutoCalcHeight,		() const				);
		REG_CONTROL_FUNPR(void,	 SetAutoCalcHeight,		(bool bAutoCalcHeight)	);

		// 鼠标提示
		REG_CONTROL_FUNPR(CDuiString,	  GetToolTip,		() const				);
		REG_CONTROL_FUNPR(void,			  SetToolTip,		(LPCTSTR pstrText)		);
		REG_CONTROL_FUNPR(void,			  SetToolTipWidth,	(int nWidth)			);
		REG_CONTROL_FUNPR(int,			  GetToolTipWidth,	(void)					);	

		// 光标
		REG_CONTROL_FUNPR(WORD,			  GetCursor,		()						);
		REG_CONTROL_FUNPR(void,			  SetCursor,		(WORD wCursor)			);

		// 快捷键
		REG_CONTROL_FUNPR(TCHAR,		  GetShortcut,		() const				);
		REG_CONTROL_FUNPR(void,			  SetShortcut,		(TCHAR ch)				);

		// 菜单
		REG_CONTROL_FUNPR(bool,			  IsContextMenuUsed,	() const			);
		REG_CONTROL_FUNPR(void,			  SetContextMenuUsed,	(bool bMenuUsed)	);

		// 用户属性
		REG_CONTROL_FUNPR(CDuiString,	  GetUserData,		()						);
		REG_CONTROL_FUNPR(void,			  SetUserData,		(LPCTSTR pstrText)		);
		REG_CONTROL_FUNPR(UINT_PTR,		  GetTag,			() const				); 
		REG_CONTROL_FUNPR(void,			  SetTag,			(UINT_PTR pTag)			); 


		// 一些重要的属性
		REG_CONTROL_FUNPR(bool,		 IsVisible,				() const				);
		REG_CONTROL_FUNPR(void,		 SetVisible,			(bool bVisible)			);
		REG_CONTROL_FUNPR(void,		 SetInternVisible,		(bool bVisible)			);
		REG_CONTROL_FUNPR(bool,		 IsEnabled,				() const				);
		REG_CONTROL_FUNPR(void,		 SetEnabled,			(bool bEnable)			);
		REG_CONTROL_FUNPR(bool,		 IsMouseEnabled,		() const				);
		REG_CONTROL_FUNPR(void,		 SetMouseEnabled,		(bool bEnable)			);
		REG_CONTROL_FUNPR(bool,		 IsKeyboardEnabled,		() const				);
		REG_CONTROL_FUNPR(void,		 SetKeyboardEnabled,	(bool bEnable)			);
		REG_CONTROL_FUNPR(bool,		 IsFocused,				() const				);
		REG_CONTROL_FUNPR(void,		 SetFocus,				()						);
		REG_CONTROL_FUNPR(bool,		 IsFloat,				() const				);
		REG_CONTROL_FUNPR(void,		 SetFloat,				(bool bFloat)			);

		REG_CONTROL_FUNPR(bool,		 IsHot,					() const				); 
		REG_CONTROL_FUNPR(void,		 SetHot,				(bool bFloat)			); 

		//REG_CONTROL_FUN(CControlUI*, FindControl,				(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		REG_CONTROL_FUNPR(void,		 Invalidate,				()					);
		REG_CONTROL_FUNPR(bool,		 IsUpdateNeeded,			() const			);
		REG_CONTROL_FUNPR(void,		 NeedUpdate,				()					);
		REG_CONTROL_FUNPR(void,		 NeedParentUpdate,			()					);
		REG_CONTROL_FUNPR(DWORD,	 GetAdjustColor,			(DWORD dwColor)		);

		REG_CONTROL_FUNPR(void,		 Init,						()					);
		REG_CONTROL_FUNPR(void,		 DoInit,					()					);

		REG_CONTROL_FUNPR(void,		 Event,						(TEventUI& event)	);
		REG_CONTROL_FUNPR(void,		 DoEvent,					(TEventUI& event)	);

		// 自定义(未处理的)属性
		REG_CONTROL_FUNPR(void,		 AddCustomAttribute,		(LPCTSTR pstrName, LPCTSTR pstrAttr)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetCustomAttribute,		(LPCTSTR pstrName) const				);
		REG_CONTROL_FUNPR(bool,		 RemoveCustomAttribute,		(LPCTSTR pstrName)						);
		REG_CONTROL_FUNPR(void,		 RemoveAllCustomAttribute,	()										);

		REG_CONTROL_FUNPR(void,		 SetAttribute,				(LPCTSTR pstrName, LPCTSTR pstrValue)	);
		r = engine->RegisterObjectMethod(classname, "CControlUI @ApplyAttributeList()", asMETHODPR(T, ApplyAttributeList, (LPCTSTR pstrList), CControlUI*), asCALL_THISCALL); assert( r >= 0 );

		REG_CONTROL_FUNPR(SIZE,		 EstimateSize,				(SIZE szAvailable)						);
// 		REG_CONTROL_FUN(bool,		 Paint,						(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)		); 
// 		REG_CONTROL_FUN(bool,		 DoPaint,					(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)		);
// 		REG_CONTROL_FUNPR(void,		 PaintBkColor,				(HDC hDC)						);
// 		REG_CONTROL_FUNPR(void,		 PaintBkImage,				(HDC hDC)						);
// 		REG_CONTROL_FUNPR(void,		 PaintStatusImage,			(HDC hDC)						);
// 		REG_CONTROL_FUNPR(void,		 PaintForeColor,			(HDC hDC)						);
// 		REG_CONTROL_FUNPR(void,		 PaintForeImage,			(HDC hDC)						);
// 		REG_CONTROL_FUNPR(void,		 PaintText,					(HDC hDC)						);
// 		REG_CONTROL_FUNPR(void,		 PaintBorder,				(HDC hDC)						);

		//REG_CONTROL_FUNPR2("void DoPostPaint(HDC,const RECT &)", void,	 DoPostPaint,				(HDC hDC, const RECT& rcPaint)	);

		//虚拟窗口参数
		REG_CONTROL_FUNPR(void,		 SetVirtualWnd,				(LPCTSTR pstrValue)				);
		REG_CONTROL_FUNPR(CDuiString,GetVirtualWnd,				() const						);
	
	}
	
};

//////////////////////////////////////////////////////////////////////////
//CActiveXUI
template <typename T>
class regCActiveXUI : public regCControlUI<T>
{
	DECL_CONTROL_FACTORY(CActiveXUI);
	DECL_CONTROL_REGFACT(CActiveXUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(bool,			 IsDelayCreate,		() const			);
		REG_CONTROL_FUNPR(void,			 SetDelayCreate,	(bool bDelayCreate)	);
		REG_CONTROL_FUNPR(bool,			 IsMFC,				() const			);
		REG_CONTROL_FUNPR(void,			 SetMFC,			(bool bMFC)			);

		//REG_CONTROL_FUNPR(bool,			 CreateControl,		(LPCTSTR pstrCLSID)	);
		REG_CONTROL_FUNPR(CLSID,		 GetClisd,			() const			);
		REG_CONTROL_FUNPR(CDuiString,	 GetModuleName,		() const			);
		REG_CONTROL_FUNPR(void,			 SetModuleName,		(LPCTSTR pstrText)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CWebBrowserUI
template <typename T>
class regCWebBrowserUI : public regCActiveXUI<T>
{
	DECL_CONTROL_FACTORY(CWebBrowserUI);
	DECL_CONTROL_REGFACT(CWebBrowserUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,		SetHomePage,	(LPCTSTR lpszUrl)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetHomePage,	()					);

		REG_CONTROL_FUNPR(void,	 SetAutoNavigation,	(bool bAuto)		);
		REG_CONTROL_FUNPR(bool,	 IsAutoNavigation,	()					);

		REG_CONTROL_FUNPR(void,	 Navigate2,			(LPCTSTR lpszUrl)	);
		REG_CONTROL_FUNPR(void,	 Refresh,			()					);
		REG_CONTROL_FUNPR(void,	 Refresh2,			(int Level)	);
		REG_CONTROL_FUNPR(void,	 GoBack,			()					);
		REG_CONTROL_FUNPR(void,	 GoForward,			()					);
		REG_CONTROL_FUNPR(void,	 NavigateHomePage,	()					);
		REG_CONTROL_FUNPR(void,	 NavigateUrl,		(LPCTSTR lpszUrl)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CColorPaletteUI
template <typename T>
class regCColorPaletteUI : public regCControlUI<T>
{
	DECL_CONTROL_FACTORY(CColorPaletteUI);
	DECL_CONTROL_REGFACT(CColorPaletteUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(DWORD,	 GetSelectColor,	()					);
		REG_CONTROL_FUNPR(void,	 SetSelectColor,	(DWORD dwColor)		);

		REG_CONTROL_FUNPR(void,	 SetPalletHeight,	(int nHeight)		);
		REG_CONTROL_FUNPR(int,	GetPalletHeight,	() const			);

		REG_CONTROL_FUNPR(void,	 SetBarHeight,		(int nHeight)		);
		REG_CONTROL_FUNPR(int,	 GetBarHeight,		() const			);

		REG_CONTROL_FUNPR(void,	 SetThumbImage,		(LPCTSTR pszImage)	);
		REG_CONTROL_FUNPR(LPCTSTR, GetThumbImage,		() const			);
	}
};

///////////////////////////////////////////////////////////////////////////
//CContainerUI
template <typename T>
class regCContainerUI : public regCControlUI<T>
{
	DECL_CONTROL_FACTORY(CContainerUI);
	DECL_CONTROL_REGFACT(CContainerUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(CControlUI*,	GetItemAt,		(int iIndex) const					);
		REG_CONTROL_FUNPR(int,			GetItemIndex,	(CControlUI* pControl) const		);
		REG_CONTROL_FUNPR(bool,			SetItemIndex,	(CControlUI* pControl, int iIndex)	);
		REG_CONTROL_FUNPR(int,			GetCount,		() const							);
		REG_CONTROL_FUNPR(bool,			Add,			(CControlUI* pControl)				);
		REG_CONTROL_FUNPR(bool,			AddAt,			(CControlUI* pControl, int iIndex)	);
		REG_CONTROL_FUNPR(bool,			Remove,			(CControlUI* pControl, bool bDoNotDestroy));
		REG_CONTROL_FUNPR(bool,			RemoveAt,		(int iIndex, bool bDoNotDestroy		));
		REG_CONTROL_FUNPR(void,			RemoveAll,		()									);

		REG_CONTROL_FUNPR(RECT,	 GetInset,				() const				);
		REG_CONTROL_FUNPR(void,	 SetInset,				(RECT rcInset)			);
		REG_CONTROL_FUNPR(int,	 GetChildPadding,		() const				);
		REG_CONTROL_FUNPR(void,	 SetChildPadding,		(int iPadding)			);
		REG_CONTROL_FUNPR(UINT,	 GetChildAlign,			() const				);
		REG_CONTROL_FUNPR(void,	 SetChildAlign,			(UINT iAlign)			);
		REG_CONTROL_FUNPR(UINT,	 GetChildVAlign,		() const				);
		REG_CONTROL_FUNPR(void,	 SetChildVAlign,		(UINT iVAlign)			);
		REG_CONTROL_FUNPR(bool,	 IsAutoDestroy,			() const				);
		REG_CONTROL_FUNPR(void,	 SetAutoDestroy,		(bool bAuto)			);
		REG_CONTROL_FUNPR(bool,	 IsDelayedDestroy,		() const				);
		REG_CONTROL_FUNPR(void,	 SetDelayedDestroy,		(bool bDelayed)			);
		REG_CONTROL_FUNPR(bool,	 IsMouseChildEnabled,	() const				);
		REG_CONTROL_FUNPR(void,	 SetMouseChildEnabled,	(bool bEnable)			);

		REG_CONTROL_FUNPR(int,	 FindSelectable,			(int iIndex, bool bForward) const				);

		REG_CONTROL_FUNPR(bool,	 SetSubControlText,			(LPCTSTR pstrSubControlName,LPCTSTR pstrText)	);
		REG_CONTROL_FUNPR(bool,	 SetSubControlFixedHeight,	(LPCTSTR pstrSubControlName,int cy)				);
		REG_CONTROL_FUNPR(bool,	 SetSubControlFixedWdith,	(LPCTSTR pstrSubControlName,int cx)				);
		REG_CONTROL_FUNPR(bool,	 SetSubControlUserData,		(LPCTSTR pstrSubControlName,LPCTSTR pstrText)	);

		REG_CONTROL_FUNPR(CDuiString,	GetSubControlText,			(LPCTSTR pstrSubControlName)					);
		REG_CONTROL_FUNPR(int,			GetSubControlFixedHeight,	(LPCTSTR pstrSubControlName)					);
		REG_CONTROL_FUNPR(int,			GetSubControlFixedWdith,	(LPCTSTR pstrSubControlName)					);
		REG_CONTROL_FUNPR(CControlUI*,	FindSubControl,				(LPCTSTR pstrSubControlName));

		REG_CONTROL_FUNPR(SIZE,	 GetScrollPos,		() const				);
		REG_CONTROL_FUNPR(SIZE,	 GetScrollRange,	() const				);
		REG_CONTROL_FUNPR(void,	 SetScrollPos,		(SIZE szPos, bool bMsg)	);
		REG_CONTROL_FUNPR(void,	 SetScrollStepSize,	(int nSize)				);
		REG_CONTROL_FUNPR(int,	 GetScrollStepSize,	() const				);
		REG_CONTROL_FUNPR(void,	 LineUp,	()	);
		REG_CONTROL_FUNPR(void,	 LineDown,	()	);
		REG_CONTROL_FUNPR(void,	 PageUp,	()	);
		REG_CONTROL_FUNPR(void,	 PageDown,	()	);
		REG_CONTROL_FUNPR(void,	 HomeUp,	()	);
		REG_CONTROL_FUNPR(void,	 EndDown,	()	);
		REG_CONTROL_FUNPR(void,	 LineLeft,	()	);
		REG_CONTROL_FUNPR(void,	 LineRight,	()	);
		REG_CONTROL_FUNPR(void,	 PageLeft,	()	);
		REG_CONTROL_FUNPR(void,	 PageRight,	()	);
		REG_CONTROL_FUNPR(void,	 HomeLeft,	()	);
		REG_CONTROL_FUNPR(void,	 EndRight,	()	);
		REG_CONTROL_FUNPR(void,	 EnableScrollBar, (bool bEnableVertical, bool bEnableHorizontal));
	}
};

//////////////////////////////////////////////////////////////////////////
//CGifAnimUI
template <typename T>
class regCGifAnimUI : public regCControlUI<T>
{
	DECL_CONTROL_FACTORY(CGifAnimUI);
	DECL_CONTROL_REGFACT(CGifAnimUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,	SetAutoPlay,	(bool bIsAuto)	);
		REG_CONTROL_FUNPR(bool,	IsAutoPlay,		() const		);
		REG_CONTROL_FUNPR(void,	SetAutoSize,	(bool bIsAuto)	);
		REG_CONTROL_FUNPR(bool,	IsAutoSize,		() const		);
		REG_CONTROL_FUNPR(void,	PlayGif,		()				);
		REG_CONTROL_FUNPR(void,	PauseGif,		()				);
		REG_CONTROL_FUNPR(void,	StopGif,		()				);
	}
};

//////////////////////////////////////////////////////////////////////////
//CLabelUI
template <typename T>
class regCLabelUI : public regCControlUI<T>
{
	DECL_CONTROL_FACTORY(CLabelUI);
	DECL_CONTROL_REGFACT(CLabelUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,	 SetTextStyle,			(UINT uStyle)		);
		REG_CONTROL_FUNPR(UINT,	 GetTextStyle,			() const			);
		REG_CONTROL_FUNPR(void,	 SetTextColor,			(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	 GetTextColor,			() const			);
		REG_CONTROL_FUNPR(void,	 SetDisabledTextColor,	(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	 GetDisabledTextColor,	() const			);
		REG_CONTROL_FUNPR(void,	 SetFont,				(int index)			);
		REG_CONTROL_FUNPR(int,	 GetFont,				() const			);
		REG_CONTROL_FUNPR(RECT,	 GetTextPadding,		() const			);
		REG_CONTROL_FUNPR(void,	 SetTextPadding,		(RECT rc)			);
		REG_CONTROL_FUNPR(bool,	 IsShowHtml,			()					);
		REG_CONTROL_FUNPR(void,	 SetShowHtml,			(bool bShowHtml)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CListElementUI

template <typename T>
class regCListElementUI : public regCControlUI<T>
{
//	DECL_CONTROL_FACTORY(CListElementUI);
//	DECL_CONTROL_REGFACT(CListElementUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(int,	 GetIndex,			() const				);
		REG_CONTROL_FUNPR(void,	 SetIndex,			(int iIndex)			);

		//IListOwnerUI* GetOwner();
		REG_CONTROL_FUNPR(void,	 SetOwner,			(CControlUI* pOwner)	);

		REG_CONTROL_FUNPR(bool,	 IsSelected,		() const				);
		REG_CONTROL_FUNPR(bool,	 Select,			(bool bSelect)			);
		REG_CONTROL_FUNPR(bool,	 SelectMulti,		(bool bSelect)			);
		REG_CONTROL_FUNPR(bool,	 IsExpanded,		() const				);
		REG_CONTROL_FUNPR(bool,	 Expand,			(bool bExpand)			);

		//REG_CONTROL_FUN(void,	 DrawItemBk,		(HDC hDC, const RECT& rcItem)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CListLabelElementUI
template <typename T>
class regCListLabelElementUI : public regCListElementUI<T>
{
	DECL_CONTROL_FACTORY(CListLabelElementUI);
	DECL_CONTROL_REGFACT(CListLabelElementUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;
	}
};

//////////////////////////////////////////////////////////////////////////
//CScrollBarUI
template <typename T>
class regCScrollBarUI : public regCControlUI<T>
{
	DECL_CONTROL_FACTORY(CScrollBarUI);
	DECL_CONTROL_REGFACT(CScrollBarUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(CContainerUI*,	 GetOwner,	() const	);
		REG_CONTROL_FUNPR(void,	 SetOwner,	(CContainerUI* pOwner)	);

		REG_CONTROL_FUNPR(bool,	 IsHorizontal,		()							);
		REG_CONTROL_FUNPR(void,	 SetHorizontal,		(bool bHorizontal)			);
		REG_CONTROL_FUNPR(int,	 GetScrollRange,	() const					);
		REG_CONTROL_FUNPR(void,	 SetScrollRange,	(int nRange)				);
		REG_CONTROL_FUNPR(int,	 GetScrollPos,		() const					);
		REG_CONTROL_FUNPR(void,	 SetScrollPos,		(int nPos)					);
		REG_CONTROL_FUNPR(int,	 GetLineSize,		() const					);
		REG_CONTROL_FUNPR(void,	 SetLineSize,		(int nSize)					);

		REG_CONTROL_FUNPR(bool,		GetShowButton1,				()					);
		REG_CONTROL_FUNPR(void,		SetShowButton1,				(bool bShow)		);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton1NormalImage,		()					);
		REG_CONTROL_FUNPR(void,		 SetButton1NormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton1HotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetButton1HotImage,			(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton1PushedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetButton1PushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton1DisabledImage,	()					);
		REG_CONTROL_FUNPR(void,		SetButton1DisabledImage,	(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(bool,		GetShowButton2,				()					);
		REG_CONTROL_FUNPR(void,		SetShowButton2,				(bool bShow)		);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton2NormalImage,		()					);
		REG_CONTROL_FUNPR(void,		SetButton2NormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton2HotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetButton2HotImage,			(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton2PushedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetButton2PushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetButton2DisabledImage,	()					);
		REG_CONTROL_FUNPR(void,		SetButton2DisabledImage,	(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(LPCTSTR,	 GetThumbNormalImage,		()					);
		REG_CONTROL_FUNPR(void,		SetThumbNormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetThumbHotImage,			()					);
		REG_CONTROL_FUNPR(void,		SetThumbHotImage,			(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetThumbPushedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetThumbPushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetThumbDisabledImage,		()					);
		REG_CONTROL_FUNPR(void,		SetThumbDisabledImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(LPCTSTR,	 GetRailNormalImage,	()					);
		REG_CONTROL_FUNPR(void,		SetRailNormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetRailHotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetRailHotImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetRailPushedImage,	()					);
		REG_CONTROL_FUNPR(void,		SetRailPushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetRailDisabledImage,	()					);
		REG_CONTROL_FUNPR(void,		SetRailDisabledImage,	(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(LPCTSTR,	 GetBkNormalImage,	()					);
		REG_CONTROL_FUNPR(void,		SetBkNormalImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetBkHotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetBkHotImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetBkPushedImage,	()					);
		REG_CONTROL_FUNPR(void,		SetBkPushedImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	 GetBkDisabledImage,()					);
		REG_CONTROL_FUNPR(void,		SetBkDisabledImage,	(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,	 PaintBk,		(HDC hDC)	);
		REG_CONTROL_FUNPR(void,	 PaintButton1,	(HDC hDC)	);
		REG_CONTROL_FUNPR(void,	 PaintButton2,	(HDC hDC)	);
		REG_CONTROL_FUNPR(void,	 PaintThumb,	(HDC hDC)	);
		REG_CONTROL_FUNPR(void,	 PaintRail,		(HDC hDC)	);
	}
};

///////////////////////////////////////////////////////////////////////////
//CVerticalLayoutUI
template <typename T>
class regCVerticalLayoutUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CVerticalLayoutUI);
	DECL_CONTROL_REGFACT(CVerticalLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,	 SetSepHeight,	(int iHeight)					);
		REG_CONTROL_FUNPR(int,	 GetSepHeight,	() const						);
		REG_CONTROL_FUNPR(void,	 SetSepImmMode,	(bool bImmediately)				);
		REG_CONTROL_FUNPR(bool,	 IsSepImmMode,	() const						);
		REG_CONTROL_FUNPR(RECT,	 GetThumbRect,	(bool bUseNew) const			);
	}
};

//////////////////////////////////////////////////////////////////////////
//CGroupBoxUI
template <typename T>
class regCGroupBoxUI : public regCVerticalLayoutUI<T>
{
	DECL_CONTROL_FACTORY(CGroupBoxUI);
	DECL_CONTROL_REGFACT(CGroupBoxUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,		SetTextColor,			(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetTextColor,			() const			);
		REG_CONTROL_FUNPR(void,		SetDisabledTextColor,	(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledTextColor,	() const			);
		REG_CONTROL_FUNPR(void,		SetFont,				(int index)			);
		REG_CONTROL_FUNPR(int,		GetFont,				() const			);
	}
};

//////////////////////////////////////////////////////////////////////////
//CListBodyUI

template <typename T>
class regCListBodyUI : public regCVerticalLayoutUI<T>
{
//	DECL_CONTROL_FACTORY(CListBodyUI);
//	DECL_CONTROL_REGFACT(CListBodyUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

	}
};

//////////////////////////////////////////////////////////////////////////
//CListUI
template <typename T>
class regCListUI : public regCVerticalLayoutUI<T>
{
	DECL_CONTROL_FACTORY(CListUI);
	DECL_CONTROL_REGFACT(CListUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(bool,		GetScrollSelect,			()								);
		REG_CONTROL_FUNPR(void,		SetScrollSelect,			(bool bScrollSelect)			);
		REG_CONTROL_FUNPR(int,		GetCurSel,					() const						);
		REG_CONTROL_FUNPR(int,		GetCurSelActivate,			() const						);
		REG_CONTROL_FUNPR(bool,		SelectItem,					(int iIndex, bool bTakeFocus)	);
		REG_CONTROL_FUNPR(bool,		SelectItemActivate,			(int iIndex)					);

		REG_CONTROL_FUNPR(bool,		SelectMultiItem,			(int iIndex, bool bTakeFocus)	);
		REG_CONTROL_FUNPR(void,		SetMultiSelect,				(bool bMultiSel)				);
		REG_CONTROL_FUNPR(bool,		IsMultiSelect,				() const						);
		REG_CONTROL_FUNPR(bool,		UnSelectItem,				(int iIndex, bool bOthers)		);
		REG_CONTROL_FUNPR(void,		SelectAllItems,				()					);
		REG_CONTROL_FUNPR(void,		UnSelectAllItems,			()					);
		REG_CONTROL_FUNPR(int,		GetSelectItemCount,			() const			);
		REG_CONTROL_FUNPR(int,		GetNextSelItem,				(int nItem) const	);

		REG_CONTROL_FUNPR(bool,		IsFixedScrollbar,			()					);
		REG_CONTROL_FUNPR(void,		SetFixedScrollbar,			(bool bFixed)		);

		REG_CONTROL_FUNPR(CListHeaderUI*,	GetHeader,	() const	);  
		REG_CONTROL_FUNPR(CContainerUI*,	GetList,	() const	);
		REG_CONTROL_FUNPR(UINT,		GetListType,				()					);
		//REG_CONTROL_FUNPR(TListInfoUI*,		GetListInfo()	);

		REG_CONTROL_FUNPR(void,		EnsureVisible,				(int iIndex)		);
		REG_CONTROL_FUNPR(void,		Scroll,						(int dx, int dy)	);

		REG_CONTROL_FUNPR(void,		SetItemFont,				(int index)			);
		REG_CONTROL_FUNPR(void,		SetItemTextStyle,			(UINT uStyle)		);
		REG_CONTROL_FUNPR(void,		SetItemTextPadding,			(RECT rc)			);
		REG_CONTROL_FUNPR(void,		SetItemTextColor,			(DWORD dwTextColor	));
		REG_CONTROL_FUNPR(void,		SetItemBkColor,				(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(void,		SetItemBkImage,				(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(void,		SetAlternateBk,				(bool bAlternateBk)	);
		REG_CONTROL_FUNPR(void,		SetSelectedItemTextColor,	(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(void,		SetSelectedItemBkColor,		(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(void,		SetSelectedItemImage,		(LPCTSTR pStrImage)	); 
		REG_CONTROL_FUNPR(void,		SetHotItemTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(void,		SetHotItemBkColor,			(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(void,		SetHotItemImage,			(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(void,		SetDisabledItemTextColor,	(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(void,		SetDisabledItemBkColor,		(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(void,		SetDisabledItemImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(void,		SetItemLineColor,			(DWORD dwLineColor)	);
		REG_CONTROL_FUNPR(void,		SetItemShowRowLine,			(bool bShowLine)	);
		REG_CONTROL_FUNPR(void,		SetItemShowColumnLine,		(bool bShowLine)	);
		REG_CONTROL_FUNPR(bool,		IsItemShowHtml,				()					);
		REG_CONTROL_FUNPR(void,		SetItemShowHtml,			(bool bShowHtml)	);
		REG_CONTROL_FUNPR(bool,		IsItemRSelected,			()					);
		REG_CONTROL_FUNPR(void,		SetItemRSelected,			(bool bSelected)	);
		REG_CONTROL_FUNPR(RECT,		GetItemTextPadding,			() const	);
		REG_CONTROL_FUNPR(DWORD,	GetItemTextColor,			() const	);
		REG_CONTROL_FUNPR(DWORD,	GetItemBkColor,				() const	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetItemBkImage,				() const	);
		REG_CONTROL_FUNPR(bool,		IsAlternateBk,				() const	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedItemTextColor,	() const	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedItemBkColor,		() const	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedItemImage,		() const	);
		REG_CONTROL_FUNPR(DWORD,	GetHotItemTextColor,		() const	);
		REG_CONTROL_FUNPR(DWORD,	GetHotItemBkColor,			() const	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotItemImage,			() const	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledItemTextColor,	() const	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledItemBkColor,		() const	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetDisabledItemImage,		() const	);
		REG_CONTROL_FUNPR(DWORD,	GetItemLineColor,			() const	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CHorizontalLayoutUI
template <typename T>
class regCHorizontalLayoutUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CHorizontalLayoutUI);
	DECL_CONTROL_REGFACT(CHorizontalLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,	 SetSepWidth,	(int iWidth)					);
		REG_CONTROL_FUNPR(int,	 GetSepWidth,	() const						);
		REG_CONTROL_FUNPR(void,	 SetSepImmMode,	(bool bImmediately)				);
		REG_CONTROL_FUNPR(bool,	 IsSepImmMode,	() const						);
		REG_CONTROL_FUNPR(RECT,	 GetThumbRect,	(bool bUseNew) const			);
	}
};

//////////////////////////////////////////////////////////////////////////
//CListContainerElementUI
template <typename T>
class regCListContainerElementUI : public regCHorizontalLayoutUI<T>
{
	DECL_CONTROL_FACTORY(CListContainerElementUI);
	DECL_CONTROL_REGFACT(CListContainerElementUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(int,	GetIndex,	() const			);
		REG_CONTROL_FUNPR(void,	SetIndex,	(int iIndex)		);

		REG_CONTROL_FUNPR(void, SetOwner, (CControlUI* pOwner)	);

		REG_CONTROL_FUNPR(bool,	IsSelected,		() const		);
		REG_CONTROL_FUNPR(bool,	Select,			(bool bSelect)	);
		REG_CONTROL_FUNPR(bool,	SelectMulti,	(bool bSelect)	);
		REG_CONTROL_FUNPR(bool,	IsExpanded,		() const		);
		REG_CONTROL_FUNPR(bool,	Expand,			(bool bExpand)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CListContainerHeaderItemUI
template <typename T>
class regCListContainerHeaderItemUI : public regCHorizontalLayoutUI<T>
{
	DECL_CONTROL_FACTORY(CListContainerHeaderItemUI);
	DECL_CONTROL_REGFACT(CListContainerHeaderItemUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(BOOL,		IsDragable,			() const			);
		REG_CONTROL_FUNPR(void,		SetDragable,		(BOOL bDragable)	);
		REG_CONTROL_FUNPR(DWORD,	GetTextStyle,		() const			);
		REG_CONTROL_FUNPR(void,		SetTextStyle,		(UINT uStyle)		);
		REG_CONTROL_FUNPR(DWORD,	GetTextColor,		() const			);
		REG_CONTROL_FUNPR(void,		SetTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(void,		SetTextPadding,		(RECT rc)			);
		REG_CONTROL_FUNPR(RECT,		GetTextPadding,		() const			);
		REG_CONTROL_FUNPR(void,		SetFont,			(int index)			);
		REG_CONTROL_FUNPR(BOOL,		IsShowHtml,			()					);
		REG_CONTROL_FUNPR(void,		SetShowHtml,		(BOOL bShowHtml)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetNormalImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetNormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetHotImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetPushedImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetPushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetFocusedImage,	() const			);
		REG_CONTROL_FUNPR(void,		SetFocusedImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetSepImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetSepImage,		(LPCTSTR pStrImage)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CListHeaderUI
template <typename T>
class regCListHeaderUI : public regCHorizontalLayoutUI<T>
{
	DECL_CONTROL_FACTORY(CListHeaderUI);
	DECL_CONTROL_REGFACT(CListHeaderUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void, SetScaleHeader, (bool bIsScale)	);
		REG_CONTROL_FUNPR(bool, IsScaleHeader, () const			);
	}
};

//////////////////////////////////////////////////////////////////////////
//CTreeNodeUI
template <typename T>
class regCTreeNodeUI : public regCListContainerElementUI<T>
{
	DECL_CONTROL_FACTORY(CTreeNodeUI);
	DECL_CONTROL_REGFACT(CTreeNodeUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,	SetVisibleTag,	(bool _IsVisible)	);
		REG_CONTROL_FUNPR(bool,	GetVisibleTag,	()	);
		REG_CONTROL_FUNPR(void,	SetItemText,	(LPCTSTR pstrValue)	);
		REG_CONTROL_FUNPR(CDuiString,	GetItemText,	()	);
		REG_CONTROL_FUNPR(void,	CheckBoxSelected,	(bool _Selected)	);
		REG_CONTROL_FUNPR(bool,	IsCheckBoxSelected,	() const	);
		REG_CONTROL_FUNPR(bool,	IsHasChild,	() const	);
		REG_CONTROL_FUNPR(long,	GetTreeLevel,	() const	);
		REG_CONTROL_FUNPR2("bool,	AddChildNode(CTreeNodeUI@ _pTreeNodeUI)", bool,	AddChildNode,	(CTreeNodeUI* _pTreeNodeUI)	);
		REG_CONTROL_FUNPR2("bool RemoveNode(CTreeNodeUI@ _pTreeNodeUI)", bool, RemoveNode, (CTreeNodeUI* _pTreeNodeUI) );
		REG_CONTROL_FUNPR2("void SetParentNode(CTreeNodeUI@ _pParentTreeNode)", void, SetParentNode,	(CTreeNodeUI* _pParentTreeNode)	);
		REG_CONTROL_FUNPR2("CTreeNodeUI@ GetParentNode()", CTreeNodeUI*, GetParentNode, ()	);
		REG_CONTROL_FUNPR(long, GetCountChild, ()	);
		//REG_CONTROL_FUNPR(void SetTreeView(CTreeViewUI* _CTreeViewUI);
		//REG_CONTROL_FUNPR(CTreeViewUI* GetTreeView();
		REG_CONTROL_FUNPR2("CTreeNodeUI@ GetChildNode(int _nIndex)", CTreeNodeUI*, GetChildNode, (int _nIndex)	);
		REG_CONTROL_FUNPR(void,	SetVisibleFolderBtn,	(bool _IsVisibled)	);
		REG_CONTROL_FUNPR(bool,	GetVisibleFolderBtn,	()	);
		REG_CONTROL_FUNPR(void,	SetVisibleCheckBtn,	(bool _IsVisibled)	);
		REG_CONTROL_FUNPR(bool,	GetVisibleCheckBtn,	()	);
		REG_CONTROL_FUNPR(void,	SetItemTextColor,	(DWORD _dwItemTextColor)	);
		REG_CONTROL_FUNPR(DWORD,GetItemTextColor,	() const	);
		REG_CONTROL_FUNPR(void,	SetItemHotTextColor,	(DWORD _dwItemHotTextColor)	);
		REG_CONTROL_FUNPR(DWORD,GetItemHotTextColor,	() const	);
		REG_CONTROL_FUNPR(void,	SetSelItemTextColor,	(DWORD _dwSelItemTextColor)	);
		REG_CONTROL_FUNPR(DWORD,GetSelItemTextColor,	() const	);
		REG_CONTROL_FUNPR(void,	SetSelItemHotTextColor,	(DWORD _dwSelHotItemTextColor)	);
		REG_CONTROL_FUNPR(DWORD,GetSelItemHotTextColor,	() const	);

		//CStdPtrArray GetTreeNodes();
		REG_CONTROL_FUNPR(int,	GetTreeIndex,	()	);
		REG_CONTROL_FUNPR(int,	GetNodeIndex,	()	);

		REG_CONTROL_FUNPR2("CHorizontalLayoutUI@ GetTreeNodeHoriznotal() const", CHorizontalLayoutUI*, GetTreeNodeHoriznotal, () const	);
		REG_CONTROL_FUNPR2("CCheckBoxUI@ GetFolderButton() const", CCheckBoxUI*, GetFolderButton, () const	);
		REG_CONTROL_FUNPR2("CLabelUI@ GetDottedLine() const", CLabelUI*, GetDottedLine, () const	);
		REG_CONTROL_FUNPR2("CCheckBoxUI@ GetCheckBox() const", CCheckBoxUI*, GetCheckBox, () const	);
		REG_CONTROL_FUNPR2("COptionUI@ GetItemButton() const", COptionUI*, GetItemButton, () const	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CTreeViewUIdeUI
template <typename T>
class regCTreeViewUI : public regCListUI<T>
{
	DECL_CONTROL_FACTORY(CTreeViewUI);
	DECL_CONTROL_REGFACT(CTreeViewUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(UINT, GetListType, ());
		REG_CONTROL_FUNPR(bool, SetItemCheckBox, (bool _Selected, CTreeNodeUI* _TreeNode));
		REG_CONTROL_FUNPR(void, SetItemExpand, (bool _Expanded, CTreeNodeUI* _TreeNode));
		REG_CONTROL_FUNPR(void, SetVisibleFolderBtn, (bool _IsVisibled));
		REG_CONTROL_FUNPR(bool, GetVisibleFolderBtn, ());
		REG_CONTROL_FUNPR(void, SetVisibleCheckBtn, (bool _IsVisibled));
		REG_CONTROL_FUNPR(bool, GetVisibleCheckBtn, ());
		REG_CONTROL_FUNPR(void, SetItemMinWidth, (UINT _ItemMinWidth));
		REG_CONTROL_FUNPR(UINT, GetItemMinWidth, ());
		REG_CONTROL_FUNPR(void, SetItemTextColor, (DWORD _dwItemTextColor));
		REG_CONTROL_FUNPR(void, SetItemHotTextColor, (DWORD _dwItemHotTextColor));
		REG_CONTROL_FUNPR(void, SetSelItemTextColor, (DWORD _dwSelItemTextColor));
		REG_CONTROL_FUNPR(void, SetSelItemHotTextColor, (DWORD _dwSelHotItemTextColor));
	}
};

//////////////////////////////////////////////////////////////////////////
//CChildLayoutUI
template <typename T>
class regCChildLayoutUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CChildLayoutUI);
	DECL_CONTROL_REGFACT(CChildLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void, SetChildLayoutXML, (CDuiString pXML)	);
		REG_CONTROL_FUNPR(CDuiString, GetChildLayoutXML, () );
	}
};

//////////////////////////////////////////////////////////////////////////
//CChildWindowUI
template <typename T>
class regCChildWindowUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CChildWindowUI);
	DECL_CONTROL_REGFACT(CChildWindowUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;
		REG_CONTROL_FUNPR(void, RefreshWindow, () );
	}
};

//////////////////////////////////////////////////////////////////////////
//CComboUI
template <typename T>
class regCComboUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CComboUI);
	DECL_CONTROL_REGFACT(CComboUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,		SetTextStyle,				(UINT uStyle)		);
		REG_CONTROL_FUNPR(UINT,		GetTextStyle,				() const			);
		REG_CONTROL_FUNPR(void,		SetTextColor,				(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetTextColor,				() const			);
		REG_CONTROL_FUNPR(void,		SetDisabledTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledTextColor,		() const			);
		REG_CONTROL_FUNPR(void,		SetFont,					(int index)			);
		REG_CONTROL_FUNPR(int,		GetFont,					() const			);
		REG_CONTROL_FUNPR(RECT,		GetTextPadding,				() const			);
		REG_CONTROL_FUNPR(void,		SetTextPadding,				(RECT rc)			);
		REG_CONTROL_FUNPR(bool,		IsShowHtml,					()					);
		REG_CONTROL_FUNPR(void,		SetShowHtml,				(bool bShowHtml)	);
		REG_CONTROL_FUNPR(bool,		IsShowShadow,				()					);
		REG_CONTROL_FUNPR(void,		SetShowShadow,				(bool bShow)		);

		REG_CONTROL_FUNPR(CDuiString,	GetDropBoxAttributeList,()					);
		REG_CONTROL_FUNPR(void,		SetDropBoxAttributeList,	(LPCTSTR pstrList)	);
		REG_CONTROL_FUNPR(SIZE,		GetDropBoxSize,				() const			);
		REG_CONTROL_FUNPR(void,		SetDropBoxSize,				(SIZE szDropBox)	);

		//UINT GetListType();
		//TListInfoUI* GetListInfo();
		REG_CONTROL_FUNPR(int,		GetCurSel,			() const						);  
		REG_CONTROL_FUNPR(bool,		SelectItem,			(int iIndex, bool bTakeFocus)	);
		REG_CONTROL_FUNPR(bool,		SelectMultiItem,	(int iIndex, bool bTakeFocus)	);
		REG_CONTROL_FUNPR(bool,		UnSelectItem,		(int iIndex, bool bOthers)		);

		REG_CONTROL_FUNPR(LPCTSTR,	GetNormalImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetNormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetHotImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetPushedImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetPushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetFocusedImage,	() const			);
		REG_CONTROL_FUNPR(void,		SetFocusedImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetDisabledImage,	() const			);
		REG_CONTROL_FUNPR(void,		SetDisabledImage,	(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(bool,		GetScrollSelect,	()					);
		REG_CONTROL_FUNPR(void,		SetScrollSelect,	(bool bScrollSelect));

		REG_CONTROL_FUNPR(void,		SetItemFont,				(int index)			);
		REG_CONTROL_FUNPR(void,		SetItemTextStyle,			(UINT uStyle)		);
		REG_CONTROL_FUNPR(RECT,		GetItemTextPadding,			() const			);
		REG_CONTROL_FUNPR(void,		SetItemTextPadding,			(RECT rc)			);
		REG_CONTROL_FUNPR(DWORD,	GetItemTextColor,			() const			);
		REG_CONTROL_FUNPR(void,		SetItemTextColor,			(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetItemBkColor,				() const			);
		REG_CONTROL_FUNPR(void,		SetItemBkColor,				(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetItemBkImage,				() const			);
		REG_CONTROL_FUNPR(void,		SetItemBkImage,				(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(bool,		IsAlternateBk,				() const			);
		REG_CONTROL_FUNPR(void,		SetAlternateBk,				(bool bAlternateBk)	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedItemTextColor,	() const			);
		REG_CONTROL_FUNPR(void,		SetSelectedItemTextColor,	(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedItemBkColor,		() const			);
		REG_CONTROL_FUNPR(void,		SetSelectedItemBkColor,		(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedItemImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetSelectedItemImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(DWORD,	GetHotItemTextColor,		() const	);
		REG_CONTROL_FUNPR(void,		SetHotItemTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetHotItemBkColor,			() const			);
		REG_CONTROL_FUNPR(void,		SetHotItemBkColor,			(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotItemImage,			() const			);
		REG_CONTROL_FUNPR(void,		SetHotItemImage,			(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledItemTextColor,	() const			);
		REG_CONTROL_FUNPR(void,		SetDisabledItemTextColor,	(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledItemBkColor,		() const			);
		REG_CONTROL_FUNPR(void,		SetDisabledItemBkColor,		(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetDisabledItemImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetDisabledItemImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(DWORD,	GetItemLineColor,			() const			);
		REG_CONTROL_FUNPR(void,		SetItemLineColor,			(DWORD dwLineColor)	);
		REG_CONTROL_FUNPR(bool,		IsItemShowHtml,				()					);
		REG_CONTROL_FUNPR(void,		SetItemShowHtml,			(bool bShowHtml)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CLabelLayoutUI
template <typename T>
class regCLabelLayoutUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CLabelLayoutUI);
	DECL_CONTROL_REGFACT(CLabelLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,	 SetTextStyle,			(UINT uStyle)		);
		REG_CONTROL_FUNPR(UINT,	 GetTextStyle,			() const			);
		REG_CONTROL_FUNPR(void,	 SetTextColor,			(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD, GetTextColor,			() const			);
		REG_CONTROL_FUNPR(void,	 SetDisabledTextColor,	(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD, GetDisabledTextColor,	() const			);
		REG_CONTROL_FUNPR(void,	 SetFont,				(int index)			);
		REG_CONTROL_FUNPR(int,	 GetFont,				() const			);
		REG_CONTROL_FUNPR(RECT,	 GetTextPadding,		() const			);
		REG_CONTROL_FUNPR(void,	 SetTextPadding,		(RECT rc)			);
		REG_CONTROL_FUNPR(bool,	 IsShowHtml,			()					);
		REG_CONTROL_FUNPR(void,	 SetShowHtml,			(bool bShowHtml)	);

// 		REG_CONTROL_FUNPR(bool,	 GetAutoCalcWidth,		() const				);
// 		REG_CONTROL_FUNPR(void,	 SetAutoCalcWidth,		(bool bAutoCalcWidth)	);
// 		REG_CONTROL_FUNPR(bool,	 GetAutoCalcHeight,		() const				);
// 		REG_CONTROL_FUNPR(void,	 SetAutoCalcHeight,		(bool bAutoCalcHeight)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CButtonLayoutUI
template <typename T>
class regCButtonLayoutUI : public regCLabelLayoutUI<T>
{
	DECL_CONTROL_FACTORY(CButtonLayoutUI);
	DECL_CONTROL_REGFACT(CButtonLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(LPCTSTR,	GetNormalImage,		()					);
		REG_CONTROL_FUNPR(void,		SetNormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetHotImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetPushedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetPushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetFocusedImage,	()					);
		REG_CONTROL_FUNPR(void,		SetFocusedImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetDisabledImage,	()					);
		REG_CONTROL_FUNPR(void,		SetDisabledImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotForeImage,	()					);
		REG_CONTROL_FUNPR(void,		SetHotForeImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(void,		SetStateCount,		(int nCount)		);
		REG_CONTROL_FUNPR(int,		GetStateCount,		() const			);
		REG_CONTROL_FUNPR(LPCTSTR,	GetStateImage,		()					);
		REG_CONTROL_FUNPR(void,		SetStateImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		BindTabIndex,			(int _BindTabIndex)			);
		REG_CONTROL_FUNPR(void,		BindTabLayoutName,		(LPCTSTR _TabLayoutName)	);
		REG_CONTROL_FUNPR(void,		BindTriggerTabSel,		(int _SetSelectIndex)		);
		REG_CONTROL_FUNPR(void,		RemoveBindTabIndex,		()			);
		REG_CONTROL_FUNPR(int,		GetBindTabLayoutIndex,	()			);
		REG_CONTROL_FUNPR(LPCTSTR,	GetBindTabLayoutName,	()			);

		REG_CONTROL_FUNPR(void,		SetHotFont,				(int index)	);
		REG_CONTROL_FUNPR(int,		GetHotFont,				() const	);
		REG_CONTROL_FUNPR(void,		SetPushedFont,			(int index)	);
		REG_CONTROL_FUNPR(int,		GetPushedFont,			() const	);
		REG_CONTROL_FUNPR(void,		SetFocusedFont,			(int index)	);
		REG_CONTROL_FUNPR(int,		GetFocusedFont,			() const	);

		REG_CONTROL_FUNPR(void,		SetHotBkColor,			(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetHotBkColor,			() const		);
		REG_CONTROL_FUNPR(void,		SetPushedBkColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetPushedBkColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetDisabledBkColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledBkColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetHotTextColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetHotTextColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetPushedTextColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetPushedTextColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetFocusedTextColor,	(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetFocusedTextColor,	() const		);

		REG_CONTROL_FUNPR(void,		SetPushedBorderColor,	(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetPushedBorderColor,	() const		);
		REG_CONTROL_FUNPR(void,		SetDisableBorderColor,	(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisableBorderColor,	() const		);

	}
};

//////////////////////////////////////////////////////////////////////////
//COptionLayoutUI
template <typename T>
class regCOptionLayoutUI : public regCButtonLayoutUI<T>
{
	DECL_CONTROL_FACTORY(COptionLayoutUI);
	DECL_CONTROL_REGFACT(COptionLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedImage,			()					);
		REG_CONTROL_FUNPR(void,		SetSelectedImage,			(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedHotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedHotImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedPushedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedPushedImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		SetSelectedTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedTextColor,		()					);

		REG_CONTROL_FUNPR(void,		SetSelectedBkColor,			(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectBkColor,			()					);

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedForedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedForedImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		SetSelectedStateCount,		(int nCount)		);
		REG_CONTROL_FUNPR(int,		GetSelectedStateCount,		() const			);
		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedStateImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedStateImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		SetSelectedFont,			(int index)			);
		REG_CONTROL_FUNPR(int,		GetSelectedFont,			() const			);

		REG_CONTROL_FUNPR(void,		SetSelectedBorderColor,		(DWORD dwColor)		);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedBorderColor,		() const			);

		REG_CONTROL_FUNPR(LPCTSTR,	GetGroup,					() const				);
		REG_CONTROL_FUNPR(void,		SetGroup,					(LPCTSTR pStrGroupName)	);
		REG_CONTROL_FUNPR(bool,		IsSelected,					() const				);
		REG_CONTROL_FUNPR(void,		Selected,					(bool bSelected, bool bTriggerEvent)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CListHeaderItemUI
template <typename T>
class regCListHeaderItemUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CListHeaderItemUI);
	DECL_CONTROL_REGFACT(CListHeaderItemUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(bool,		IsDragable,			() const			);
		REG_CONTROL_FUNPR(void,		SetDragable,		(bool bDragable)	);
		REG_CONTROL_FUNPR(DWORD,	GetSepWidth,		() const			);
		REG_CONTROL_FUNPR(void,		SetSepWidth,		(int iWidth)		);
		REG_CONTROL_FUNPR(DWORD,	GetTextStyle,		() const			);
		REG_CONTROL_FUNPR(void,		SetTextStyle,		(UINT uStyle)		);
		REG_CONTROL_FUNPR(DWORD,	GetTextColor,		() const			);
		REG_CONTROL_FUNPR(void,		SetTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(void,		SetTextPadding,		(RECT rc)			);
		REG_CONTROL_FUNPR(RECT,		GetTextPadding,		() const			);
		REG_CONTROL_FUNPR(void,		SetFont,			(int index)			);
		REG_CONTROL_FUNPR(bool,		IsShowHtml,			()					);
		REG_CONTROL_FUNPR(void,		SetShowHtml,		(bool bShowHtml)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetNormalImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetNormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetHotImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetPushedImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetPushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetFocusedImage,	() const			);
		REG_CONTROL_FUNPR(void,		SetFocusedImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetSepImage,		() const			);
		REG_CONTROL_FUNPR(void,		SetSepImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(void,		SetScale,			(int nScale)		);
		REG_CONTROL_FUNPR(int,		GetScale,			() const			);
	}
};

//////////////////////////////////////////////////////////////////////////
//CRichEditUI
template <typename T>
class regCRichEditUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CRichEditUI);
	DECL_CONTROL_REGFACT(CRichEditUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(bool,		IsMultiLine,		()	);
		REG_CONTROL_FUNPR(void,		SetMultiLine,		(bool bMultiLine)	);
		REG_CONTROL_FUNPR(bool,		IsWantTab,			()	);
		REG_CONTROL_FUNPR(void,		SetWantTab,			(bool bWantTab)	);
		REG_CONTROL_FUNPR(bool,		IsWantReturn,		()	);
		REG_CONTROL_FUNPR(void,		SetWantReturn,		(bool bWantReturn)	);
		REG_CONTROL_FUNPR(bool,		IsWantCtrlReturn,	()	);
		REG_CONTROL_FUNPR(void,		SetWantCtrlReturn,	(bool bWantCtrlReturn)	);
		REG_CONTROL_FUNPR(bool,		IsTransparent,		()	);
		REG_CONTROL_FUNPR(void,		SetTransparent,		(bool bTransparent)	);
		REG_CONTROL_FUNPR(bool,		IsRich,				()	);
		REG_CONTROL_FUNPR(void,		SetRich,			(bool bRich)	);
		REG_CONTROL_FUNPR(bool,		IsReadOnly,			()	);
		REG_CONTROL_FUNPR(void,		SetReadOnly,		(bool bReadOnly)	);
		REG_CONTROL_FUNPR(bool,		IsWordWrap,			()	);
		REG_CONTROL_FUNPR(void,		SetWordWrap,		(bool bWordWrap)	);
		REG_CONTROL_FUNPR(int,		GetFont,			()	);
		REG_CONTROL_FUNPR(void,		SetFont,			(int index)	);
		REG_CONTROL_FUNPR(void,		SetFont,			(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)	);
		REG_CONTROL_FUNPR(LONG,		GetWinStyle,		()	);
		REG_CONTROL_FUNPR(void,		SetWinStyle,		(LONG lStyle)	);
		REG_CONTROL_FUNPR(DWORD,	GetTextColor,		()	);
		REG_CONTROL_FUNPR(void,		SetTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(int,		GetLimitText,		()	);
		REG_CONTROL_FUNPR(void,		SetLimitText,		(int iChars)	);
		REG_CONTROL_FUNPR(long,		GetTextLength,		(DWORD dwFlags) const	);
		REG_CONTROL_FUNPR(bool,		IsModify,			() const	);
		REG_CONTROL_FUNPR(void,		SetModify,			(bool bModified) const	);
		//void GetSel(CHARRANGE &cr) const;
		//REG_CONTROL_FUNPR(void,		GetSel,				(long& nStartChar, long& nEndChar) const	);
		//int SetSel(CHARRANGE &cr);
		REG_CONTROL_FUNPR(int,		SetSel,				(long nStartChar, long nEndChar)	);
		REG_CONTROL_FUNPR(void,		ReplaceSel,			(LPCTSTR lpszNewText, bool bCanUndo)	);
		//REG_CONTROL_FUNPR(void,		ReplaceSelW,		(LPCWSTR lpszNewText, bool bCanUndo)	);
		REG_CONTROL_FUNPR(CDuiString,GetSelText,		() const	);
		REG_CONTROL_FUNPR(int,		SetSelAll,			()	);
		REG_CONTROL_FUNPR(int,		SetSelNone,			()	);
		REG_CONTROL_FUNPR(WORD,		GetSelectionType,	() const	);
		//REG_CONTROL_FUNPR(bool,		GetZoom,			(int& nNum, int& nDen) const	);
		REG_CONTROL_FUNPR(bool,		SetZoom,			(int nNum, int nDen)	);
		REG_CONTROL_FUNPR(bool,		SetZoomOff,			()	);
		REG_CONTROL_FUNPR(bool,		GetAutoURLDetect,	() const	);
		REG_CONTROL_FUNPR(bool,		SetAutoURLDetect,	(bool bAutoDetect)						);
		REG_CONTROL_FUNPR(DWORD,	GetEventMask,		() const								);
		REG_CONTROL_FUNPR(DWORD,	SetEventMask,		(DWORD dwEventMask)						);
		REG_CONTROL_FUNPR(CDuiString,GetTextRange,		(long nStartChar, long nEndChar) const	);
		REG_CONTROL_FUNPR(void,		HideSelection,		(bool bHide, bool bChangeStyle)			);
		REG_CONTROL_FUNPR(void,		ScrollCaret,		()										);
		REG_CONTROL_FUNPR(int,		InsertText,			(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo)	);
		REG_CONTROL_FUNPR(int,		AppendText,			(LPCTSTR lpstrText, bool bCanUndo)		);
// 		DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
// 		bool SetDefaultCharFormat(CHARFORMAT2 &cf);
// 		DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
// 		bool SetSelectionCharFormat(CHARFORMAT2 &cf);
// 		bool SetWordCharFormat(CHARFORMAT2 &cf);
// 		DWORD GetParaFormat(PARAFORMAT2 &pf) const;
// 		bool SetParaFormat(PARAFORMAT2 &pf);
		REG_CONTROL_FUNPR(bool,		CanUndo,			()	);
		REG_CONTROL_FUNPR(bool,		CanRedo,			()	);
		REG_CONTROL_FUNPR(bool,		CanPaste,			()	);
		REG_CONTROL_FUNPR(bool,		Redo,				()	);
		REG_CONTROL_FUNPR(bool,		Undo,				()	);
		REG_CONTROL_FUNPR(void,		Clear,				()	);
		REG_CONTROL_FUNPR(void,		Copy,				()	);
		REG_CONTROL_FUNPR(void,		Cut,				()	);
		REG_CONTROL_FUNPR(void,		Paste,				()	);
		REG_CONTROL_FUNPR(int,		GetLineCount,		() const							);
		REG_CONTROL_FUNPR(CDuiString,GetLine,			(int nIndex, int nMaxLength) const	);
		REG_CONTROL_FUNPR(int,		LineIndex,			(int nLine) const				);
		REG_CONTROL_FUNPR(int,		LineLength,			(int nLine) const				);
		REG_CONTROL_FUNPR(bool,		LineScroll,			(int nLines, int nChars)		);
		REG_CONTROL_FUNPR(CDuiPoint,GetCharPos,			(long lChar) const				);
		REG_CONTROL_FUNPR(long,		LineFromChar,		(long nIndex) const				);
		REG_CONTROL_FUNPR(CDuiPoint,PosFromChar,		(UINT nChar) const				);
		//REG_CONTROL_FUNPR(int,		CharFromPos,		(CDuiPoint pt) const			);
		REG_CONTROL_FUNPR(void,		EmptyUndoBuffer,	()								);
		REG_CONTROL_FUNPR(UINT,		SetUndoLimit,		(UINT nLimit)					);
// 		REG_CONTROL_FUNPR(long,		StreamIn,			(int nFormat, EDITSTREAM &es)	);
// 		REG_CONTROL_FUNPR(long,		StreamOut,			(int nFormat, EDITSTREAM &es)	);
		REG_CONTROL_FUNPR(void,		SetAccumulateDBCMode,(bool bDBCMode)		);
		REG_CONTROL_FUNPR(bool,		IsAccumulateDBCMode,()						);

		REG_CONTROL_FUNPR(RECT,		GetTextPadding,			() const			);
		REG_CONTROL_FUNPR(void,		SetTextPadding,			(RECT rc)			);
		REG_CONTROL_FUNPR(LPCTSTR,	GetNormalImage,			()					);
		REG_CONTROL_FUNPR(void,		SetNormalImage,			(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotImage,			()					);
		REG_CONTROL_FUNPR(void,		SetHotImage,			(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetFocusedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetFocusedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetDisabledImage,		()					);
		REG_CONTROL_FUNPR(void,		SetDisabledImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		SetTipValue,			(LPCTSTR pStrTipValue)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetTipValue,			()					);
		REG_CONTROL_FUNPR(void,		SetTipValueColor,		(LPCTSTR pStrColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetTipValueColor,		()					);
		REG_CONTROL_FUNPR(void,		SetTipValueAlign,		(UINT uAlign)		);
		REG_CONTROL_FUNPR(UINT,		GetTipValueAlign,		()					);

		REG_CONTROL_FUNPR(bool,		SetDropAcceptFile,		(bool bAccept)		);
	}
};

//////////////////////////////////////////////////////////////////////////
//CTabLayoutUI
template <typename T>
class regCTabLayoutUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CTabLayoutUI);
	DECL_CONTROL_REGFACT(CTabLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(int, GetCurSel, () const );
		REG_CONTROL_FUNPR(bool, SelectItem, (int iIndex) );
//		REG_CONTROL_FUNPR2("bool SelectItem(CControlUI@ pControl)", bool, SelectItem, (CControlUI* pControl) );
	}
};

//////////////////////////////////////////////////////////////////////////
//CAnimationTabLayoutUI
template <typename T>
class regCAnimationTabLayoutUI : public regCTabLayoutUI<T>
{
	DECL_CONTROL_FACTORY(CAnimationTabLayoutUI);
	DECL_CONTROL_REGFACT(CAnimationTabLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void, AnimationSwitch,()					);
		REG_CONTROL_FUNPR(void, SetFrameCount,	(int framecount)	);
		REG_CONTROL_FUNPR(int,  GetFrameCount,	() const			);
		REG_CONTROL_FUNPR(void, SetFrameDelay,	(int nDelay)		);
		REG_CONTROL_FUNPR(int,  GetFrameDelay,	() const			);
	}
};

//////////////////////////////////////////////////////////////////////////
//CTileLayoutUI
template <typename T>
class regCTileLayoutUI : public regCContainerUI<T>
{
	DECL_CONTROL_FACTORY(CTileLayoutUI);
	DECL_CONTROL_REGFACT(CTileLayoutUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(int, GetFixedColumns, () const	);
		REG_CONTROL_FUNPR(void, SetFixedColumns, (int iColums)	);
		REG_CONTROL_FUNPR(int, GetChildVPadding, () const	);
		REG_CONTROL_FUNPR(void, SetChildVPadding, (int iPadding)	);

		REG_CONTROL_FUNPR(SIZE, GetItemSize, () const	);
		REG_CONTROL_FUNPR(void, SetItemSize, (SIZE szSize)	);
		REG_CONTROL_FUNPR(int, GetColumns, () const	);
		REG_CONTROL_FUNPR(int, GetRows, () const	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CButtonUI
template <typename T>
class regCButtonUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CButtonUI);
	DECL_CONTROL_REGFACT(CButtonUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(LPCTSTR,	GetNormalImage,		()					);
		REG_CONTROL_FUNPR(void,		SetNormalImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetHotImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetPushedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetPushedImage,		(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetFocusedImage,	()					);
		REG_CONTROL_FUNPR(void,		SetFocusedImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetDisabledImage,	()					);
		REG_CONTROL_FUNPR(void,		SetDisabledImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotForeImage,	()					);
		REG_CONTROL_FUNPR(void,		SetHotForeImage,	(LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(void,		SetStateCount,		(int nCount)		);
		REG_CONTROL_FUNPR(int,		GetStateCount,		() const			);
		REG_CONTROL_FUNPR(LPCTSTR,	GetStateImage,		()					);
		REG_CONTROL_FUNPR(void,		SetStateImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		BindTabIndex,			(int _BindTabIndex)			);
		REG_CONTROL_FUNPR(void,		BindTabLayoutName,		(LPCTSTR _TabLayoutName)	);
		REG_CONTROL_FUNPR(void,		BindTriggerTabSel,		(int _SetSelectIndex)		);
		REG_CONTROL_FUNPR(void,		RemoveBindTabIndex,		()			);
		REG_CONTROL_FUNPR(int,		GetBindTabLayoutIndex,	()			);
		REG_CONTROL_FUNPR(LPCTSTR,	GetBindTabLayoutName,	()			);

		REG_CONTROL_FUNPR(void,		SetHotFont,				(int index)	);
		REG_CONTROL_FUNPR(int,		GetHotFont,				() const	);
		REG_CONTROL_FUNPR(void,		SetPushedFont,			(int index)	);
		REG_CONTROL_FUNPR(int,		GetPushedFont,			() const	);
		REG_CONTROL_FUNPR(void,		SetFocusedFont,			(int index)	);
		REG_CONTROL_FUNPR(int,		GetFocusedFont,			() const	);

		REG_CONTROL_FUNPR(void,		SetHotBkColor,			(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetHotBkColor,			() const		);
		REG_CONTROL_FUNPR(void,		SetPushedBkColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetPushedBkColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetDisabledBkColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisabledBkColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetHotTextColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetHotTextColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetPushedTextColor,		(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetPushedTextColor,		() const		);
		REG_CONTROL_FUNPR(void,		SetFocusedTextColor,	(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetFocusedTextColor,	() const		);

		REG_CONTROL_FUNPR(void,		SetPushedBorderColor,	(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetPushedBorderColor,	() const		);
		REG_CONTROL_FUNPR(void,		SetDisableBorderColor,	(DWORD dwColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetDisableBorderColor,	() const		);
	}
};

//////////////////////////////////////////////////////////////////////////
//CFadeButtonUI
template <typename T>
class regCFadeButtonUI : public regCButtonUI<T>
{
	DECL_CONTROL_FACTORY(CFadeButtonUI);
	DECL_CONTROL_REGFACT(CFadeButtonUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

	}
};

//////////////////////////////////////////////////////////////////////////
//COptionUI
template <typename T>
class regCOptionUI : public regCButtonUI<T>
{
	DECL_CONTROL_FACTORY(COptionUI);
	DECL_CONTROL_REGFACT(COptionUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedImage,			()					);
		REG_CONTROL_FUNPR(void,		SetSelectedImage,			(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedHotImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedHotImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedPushedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedPushedImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		SetSelectedTextColor,		(DWORD dwTextColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedTextColor,		()					);

		REG_CONTROL_FUNPR(void,		SetSelectedBkColor,			(DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetSelectBkColor,			()					);

		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedForedImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedForedImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		SetSelectedStateCount,		(int nCount)		);
		REG_CONTROL_FUNPR(int,		GetSelectedStateCount,		() const			);
		REG_CONTROL_FUNPR(LPCTSTR,	GetSelectedStateImage,		()					);
		REG_CONTROL_FUNPR(void,		SetSelectedStateImage,		(LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void,		SetSelectedFont,			(int index)			);
		REG_CONTROL_FUNPR(int,		GetSelectedFont,			() const			);

		REG_CONTROL_FUNPR(void,		SetSelectedBorderColor,		(DWORD dwColor)		);
		REG_CONTROL_FUNPR(DWORD,	GetSelectedBorderColor,		() const			);

		REG_CONTROL_FUNPR(LPCTSTR,	GetGroup,					() const				);
		REG_CONTROL_FUNPR(void,		SetGroup,					(LPCTSTR pStrGroupName)	);
		REG_CONTROL_FUNPR(bool,		IsSelected,					() const				);
		REG_CONTROL_FUNPR(void,		Selected,					(bool bSelected, bool bTriggerEvent)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CCheckBoxUI
template <typename T>
class regCCheckBoxUI : public regCOptionUI<T>
{
	DECL_CONTROL_FACTORY(CCheckBoxUI);
	DECL_CONTROL_REGFACT(CCheckBoxUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void, SetCheck, (bool bCheck) );
		REG_CONTROL_FUNPR(bool, GetCheck, () const		);
	}
};

//////////////////////////////////////////////////////////////////////////
//CTabCtrlUI
template <typename T>
class regCTabCtrlUI : public regCOptionUI<T>
{
	DECL_CONTROL_FACTORY(CTabCtrlUI);
	DECL_CONTROL_REGFACT(CTabCtrlUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

	}
};

//////////////////////////////////////////////////////////////////////////
//CDateTimeUI
template <typename T>
class regCDateTimeUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CDateTimeUI);
	DECL_CONTROL_REGFACT(CDateTimeUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		//SYSTEMTIME& GetTime();
		//void SetTime(SYSTEMTIME* pst);

		REG_CONTROL_FUNPR(void, SetReadOnly, (bool bReadOnly)	);
		REG_CONTROL_FUNPR(bool, IsReadOnly, () const			);

		REG_CONTROL_FUNPR(void, SetFormatStyle, (UINT uStyle)	);
		REG_CONTROL_FUNPR(UINT, GetFormatStyle, ()				);
	}	
};

//////////////////////////////////////////////////////////////////////////
//CEditUI
template <typename T>
class regCEditUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CEditUI);
	DECL_CONTROL_REGFACT(CEditUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void,		SetMaxChar,				(UINT uMax)				);
		REG_CONTROL_FUNPR(UINT,		GetMaxChar,				()	);
		REG_CONTROL_FUNPR(void,		SetReadOnly,			(bool bReadOnly	)		);
		REG_CONTROL_FUNPR(bool,		IsReadOnly,				() const				);
		REG_CONTROL_FUNPR(void,		SetPasswordMode,		(bool bPasswordMode)	);
		REG_CONTROL_FUNPR(bool,		IsPasswordMode,			() const				);
		REG_CONTROL_FUNPR(void,		SetPasswordChar,		(TCHAR cPasswordChar)	);
		REG_CONTROL_FUNPR(TCHAR,	GetPasswordChar,		() const				);
		REG_CONTROL_FUNPR(void,		SetNumberOnly,			(bool bNumberOnly)		);
		REG_CONTROL_FUNPR(bool,		IsNumberOnly,			() const				);
		REG_CONTROL_FUNPR(int,		GetWindowStyls,			() const				);

		REG_CONTROL_FUNPR(LPCTSTR,	GetNormalImage,			()						);
		REG_CONTROL_FUNPR(void,		SetNormalImage,			(LPCTSTR pStrImage)		);
		REG_CONTROL_FUNPR(LPCTSTR,	GetHotImage,			()						);
		REG_CONTROL_FUNPR(void,		SetHotImage,			(LPCTSTR pStrImage)		);
		REG_CONTROL_FUNPR(LPCTSTR,	GetFocusedImage,		()						);
		REG_CONTROL_FUNPR(void,		SetFocusedImage,		(LPCTSTR pStrImage)		);
		REG_CONTROL_FUNPR(LPCTSTR,	GetDisabledImage,		()						);
		REG_CONTROL_FUNPR(void,		SetDisabledImage,		(LPCTSTR pStrImage)		);
		REG_CONTROL_FUNPR(void,		SetNativeEditBkColor,	(DWORD dwBkColor)		);
		REG_CONTROL_FUNPR(DWORD,	GetNativeEditBkColor,	() const				);
		REG_CONTROL_FUNPR(void,		SetNativeEditTextColor,	( LPCTSTR pStrColor)	);
		REG_CONTROL_FUNPR(DWORD,	GetNativeEditTextColor,	() const				);

		REG_CONTROL_FUNPR(bool,		IsAutoSelAll,			()						);
		REG_CONTROL_FUNPR(void,		SetAutoSelAll,			(bool bAutoSelAll)		);
		REG_CONTROL_FUNPR(void,		SetSel,					(long nStartChar, long nEndChar)	);
		REG_CONTROL_FUNPR(void,		SetSelAll,				()						);
		REG_CONTROL_FUNPR(void,		SetReplaceSel,			(LPCTSTR lpszReplace)	);

		REG_CONTROL_FUNPR(void,		SetTipValue,			(LPCTSTR pStrTipValue)	);
		REG_CONTROL_FUNPR(LPCTSTR,	GetTipValue,			()						);
		REG_CONTROL_FUNPR(void,		SetTipValueColor,		(LPCTSTR pStrColor)		);
		REG_CONTROL_FUNPR(DWORD,	GetTipValueColor,		()						);
	}
};

//////////////////////////////////////////////////////////////////////////
//CHotKeyUI
template <typename T>
class regCHotKeyUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CHotKeyUI);
	DECL_CONTROL_REGFACT(CHotKeyUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(LPCTSTR, GetNormalImage, ()	);
		REG_CONTROL_FUNPR(void, SetNormalImage, (LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR, GetHotImage, ()	);
		REG_CONTROL_FUNPR(void, SetHotImage, (LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR, GetFocusedImage, ()	);
		REG_CONTROL_FUNPR(void, SetFocusedImage, (LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR, GetDisabledImage, ()	);
		REG_CONTROL_FUNPR(void, SetDisabledImage, (LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(void, SetNativeBkColor, (DWORD dwBkColor)	);
		REG_CONTROL_FUNPR(DWORD, GetNativeBkColor, () const	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CIPAddressUI
template <typename T>
class regCIPAddressUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CIPAddressUI);
	DECL_CONTROL_REGFACT(CIPAddressUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(DWORD, GetIP, () );
		REG_CONTROL_FUNPR(void, SetIP, (DWORD dwIP) );
	}
};

//////////////////////////////////////////////////////////////////////////
//CProgressUI
template <typename T>
class regCProgressUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CProgressUI);
	DECL_CONTROL_REGFACT(CProgressUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(bool, IsShowText, ()	);
		REG_CONTROL_FUNPR(void, SetShowText, (bool bShowText)	);
		REG_CONTROL_FUNPR(bool, IsHorizontal, ()	);
		REG_CONTROL_FUNPR(void, SetHorizontal, (bool bHorizontal)	);
		REG_CONTROL_FUNPR(bool, IsStretchForeImage, ()	);
		REG_CONTROL_FUNPR(void, SetStretchForeImage, (bool bStretchForeImage)	);
		REG_CONTROL_FUNPR(int, GetMinValue, () const	);
		REG_CONTROL_FUNPR(void, SetMinValue, (int nMin)	);
		REG_CONTROL_FUNPR(int, GetMaxValue, () const	);
		REG_CONTROL_FUNPR(void, SetMaxValue, (int nMax)	);
		REG_CONTROL_FUNPR(int, GetValue, () const	);
		REG_CONTROL_FUNPR(void, SetValue, (int nValue)	);
	}
};

//////////////////////////////////////////////////////////////////////////
//CSliderUI
template <typename T>
class regCSliderUI : public regCProgressUI<T>
{
	DECL_CONTROL_FACTORY(CSliderUI);
	DECL_CONTROL_REGFACT(CSliderUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(int, GetChangeStep, ()	);
		REG_CONTROL_FUNPR(void, SetChangeStep, (int step)	);
		REG_CONTROL_FUNPR(void, SetThumbSize, (SIZE szXY)	);
		REG_CONTROL_FUNPR(RECT, GetThumbRect, () const	);
		REG_CONTROL_FUNPR(LPCTSTR, GetThumbImage, () const	);
		REG_CONTROL_FUNPR(void, SetThumbImage, (LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR, GetThumbHotImage, () const	);
		REG_CONTROL_FUNPR(void, SetThumbHotImage, (LPCTSTR pStrImage)	);
		REG_CONTROL_FUNPR(LPCTSTR, GetThumbPushedImage, () const	);
		REG_CONTROL_FUNPR(void, SetThumbPushedImage, (LPCTSTR pStrImage)	);

		REG_CONTROL_FUNPR(void, SetCanSendMove, (bool bCanSend)	);
		REG_CONTROL_FUNPR(bool, GetCanSendMove, () const		);
	}
};

//////////////////////////////////////////////////////////////////////////
//CRingUI
template <typename T>
class regCRingUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CRingUI);
	DECL_CONTROL_REGFACT(CRingUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

	}
};

//////////////////////////////////////////////////////////////////////////
//CRollTextUI
template <typename T>
class regCRollTextUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CRollTextUI);
	DECL_CONTROL_REGFACT(CRollTextUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

		REG_CONTROL_FUNPR(void, BeginRoll, (int nDirect, LONG lTimeSpan, LONG lMaxTimeLimited) );
		REG_CONTROL_FUNPR(void, EndRoll, () );
	}
};

//////////////////////////////////////////////////////////////////////////
//CTextUI
template <typename T>
class regCTextUI : public regCLabelUI<T>
{
	DECL_CONTROL_FACTORY(CTextUI);
	DECL_CONTROL_REGFACT(CTextUI);
public:
	virtual void reg(asIScriptEngine *engine)
	{
		__super::reg(engine);
		int r = 0;

	}
};


} //namespace DuiLib

