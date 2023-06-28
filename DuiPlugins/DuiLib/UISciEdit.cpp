#include "StdAfx.h"
#include "UISciEdit.h"


namespace DuiLib
{

	static const char *minus_xpm[] = { 
		"     9     9       16            1", 
		"` c #8c96ac", 
		". c #c4d0da", 
		"# c #daecf4", 
		"a c #ccdeec", 
		"b c #eceef4", 
		"c c #e0e5eb", 
		"d c #a7b7c7", 
		"e c #e4ecf0", 
		"f c #d0d8e2", 
		"g c #b7c5d4", 
		"h c #fafdfc", 
		"i c #b4becc", 
		"j c #d1e6f1", 
		"k c #e4f2fb", 
		"l c #ecf6fc", 
		"m c #d4dfe7", 
		"hbc.i.cbh", 
		"bffeheffb", 
		"mfllkllfm", 
		"gjkkkkkji", 
		"da`````jd", 
		"ga#j##jai", 
		"f.k##k#.a", 
		"#..jkj..#", 
		"hemgdgc#h"
	};

	static const char *plus_xpm[] = { 
		"     9     9       16            1", 
		"` c #242e44", 
		". c #8ea0b5", 
		"# c #b7d5e4", 
		"a c #dcf2fc", 
		"b c #a2b8c8", 
		"c c #ccd2dc", 
		"d c #b8c6d4", 
		"e c #f4f4f4", 
		"f c #accadc", 
		"g c #798fa4", 
		"h c #a4b0c0", 
		"i c #cde5f0", 
		"j c #bcdeec", 
		"k c #ecf1f6", 
		"l c #acbccc", 
		"m c #fcfefc", 
		"mech.hcem", 
		"eldikille", 
		"dlaa`akld", 
		".#ii`ii#.", 
		"g#`````fg", 
		".fjj`jjf.", 
		"lbji`ijbd", 
		"khb#idlhk", 
		"mkd.ghdkm"
	}; 

	class CSciWndUI : public CWindowWnd
	{
	public:
		CSciWndUI(void)
		{
			
		}
		~CSciWndUI(void) 
		{
			if(m_pOwner)
			{
				if(m_pOwner->m_hSciLexer) ::FreeLibrary(m_pOwner->m_hSciLexer);
				m_pOwner->m_hSciLexer = NULL;
			}
		}

		void Init(CSciEditUI* pOwner)
		{
			m_pOwner = pOwner;
			if(m_pOwner->m_hSciLexer == NULL)
			{
#ifdef _WIN64
				m_pOwner->m_hSciLexer = ::LoadLibrary(g_strDuiPluginsPath + _T("SciLexer64.dll"));
#else
				m_pOwner->m_hSciLexer = ::LoadLibrary(g_strDuiPluginsPath + _T("SciLexer.dll"));
#endif
			}

			Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD|WS_VISIBLE, 0, m_rcWindow);
			SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME | WS_BORDER));  //去边框		
		}

		void SetPos(RECT rc, bool bNeedInvalidate  = true)
		{
			m_rcWindow = rc;
			::MoveWindow(GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
		}

		LPCTSTR GetWindowClassName() const
		{
			return _T("SciWndUI");
		}

		void OnFinalMessage(HWND hWnd)
		{
			
		}

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			LRESULT lRes = 0;
			BOOL bHandled = FALSE;

			if( uMsg == WM_CREATE ) 
			{
				m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);

				m_hwndScintilla = CreateWindowEx(0, _T("Scintilla"),_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, 0,0,0,0,
					GetHWND(),(HMENU)NULL, m_pOwner->GetManager()->GetInstance(),NULL);
				ASSERT(m_hwndScintilla);
				m_pOwner->InitSciApi(m_hwndScintilla);
			}
			else if( uMsg == WM_SIZE) 
			{
				CRect rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::MoveWindow(m_hwndScintilla, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), TRUE);
			}
			else if(uMsg == WM_CONTEXTMENU)
			{
				m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_MENU, (WPARAM)m_hwndScintilla, lParam);
			}
			else if(uMsg == WM_NOTIFY)
			{
				bHandled = TRUE;
				NMHDR *phDR = (NMHDR*)lParam;
				if (phDR != NULL && phDR->hwndFrom == m_hwndScintilla)
				{
					SCNotification *pMsg = (SCNotification*)lParam;
					m_pOwner->OnSciNotify(pMsg);
					m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_SCI_NOTIFY, (WPARAM)m_hwndScintilla, lParam);
				}
			}

			if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			return lRes;
		}

	public:
		RECT m_rcWindow;
		CSciEditUI *m_pOwner;
		HWND m_hwndScintilla;
	};

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CSciEditUI)
CSciEditUI::CSciEditUI(void) : m_pWindow(0), m_nCodePage(SC_CP_UTF8),
	m_bLineNumber(true), m_dwLineNumberColor(0xff000000), m_dwLineNumberBkColor(0xffffffff),
	m_bBreakPoint(true),m_bFolder(true)
{
}


CSciEditUI::~CSciEditUI(void)
{
	if(m_pWindow) { delete m_pWindow; }
}

LPCTSTR CSciEditUI::GetClass() const
{
	return _T("SciEditUI");
}

LPVOID CSciEditUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_SCIEDIT) == 0 ) return static_cast<CSciEditUI*>(this);
	return __super::GetInterface(pstrName);
}

void CSciEditUI::DoInit()
{
	m_pWindow = new CSciWndUI;
	((CSciWndUI *)m_pWindow)->Init(this);

	SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)"Courier New");//(LPARAM)"Courier New");//"微软雅黑");
	sci_StyleSetSize(STYLE_DEFAULT, 12);//14);
	sci_StyleSetFore(STYLE_DEFAULT,RGB(0,0,0));

	sci_StyleSetBack(STYLE_DEFAULT, RGB(255,255,255));
	sci_StyleSetBack(STYLE_INDENTGUIDE, RGB(255,255,255));

	for (int i = SCE_H_DEFAULT; i <= SCE_HPHP_OPERATOR; i++)
	{
		sci_StyleSetBack(i,	RGB(255,255,255));
	}

	//设置选中文本背景色
	sci_SetSelBack(STYLE_DEFAULT, RGB(0xA0,0xCA,0xF0));//RGB(0xA0,0xCA,0xF0));

	sci_SetExtraDescent(1);
	sci_SetExtraAscent(1);

	//当前行高亮显示
	sci_SetCaretLineVisible(TRUE);
	sci_SetCaretLineBack(RGB(215,215,247));

	//自动调整滚动条宽度
	sci_SetScrollWidthTracking(TRUE);

	//快捷键不再显示乱码
	execute(SCI_CLEARCMDKEY, (WPARAM)('F'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('H'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('Z'+((SCMOD_CTRL|SCMOD_SHIFT)<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('Y'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)SCK_ESCAPE, SCI_NULL);

	sci_UsePopup(FALSE);
	//////////////////////////////////////////////////////////////////////////
	sci_SetMargins(3);
	SetViewLineNumber();
	SetViewBreakPoint();
	SetViewFolder();
	sci_SetMarginWidthN(3,0);
	sci_SetMarginWidthN(4,0);

}

void CSciEditUI::SetPos(RECT rc, bool bNeedInvalidate /* = true */)
{
	__super::SetPos(rc, bNeedInvalidate);

	RECT rcItem = rc;

	// Adjust for inset
	RECT rcInset = GetInset();
	GetManager()->GetDPIObj()->ScaleRect(&rcInset);
	rcItem.left += rcInset.left;
	rcItem.top += rcInset.top;
	rcItem.right -= rcInset.right;
	rcItem.bottom -= rcInset.bottom;

	if(m_pWindow)
		((CSciWndUI *)m_pWindow)->SetPos(rcItem, bNeedInvalidate);
}

void CSciEditUI::SetInternVisible(bool bVisible)
{
	if(m_pWindow)
		::ShowWindow(m_pWindow->GetHWND(), bVisible);
}

void CSciEditUI::OnSciNotify(SCNotification *pMsg)
{
	switch (pMsg->nmhdr.code)
	{
	case SCN_STYLENEEDED:
		break;
	case SCN_CHARADDED:
		break;
	case SCN_SAVEPOINTREACHED:
		break;
	case SCN_SAVEPOINTLEFT: //文件被修改
		break;
	case SCN_MODIFYATTEMPTRO:
		break;
	case SCN_KEY:
		break;
	case SCN_DOUBLECLICK:
		break;
	case SCN_UPDATEUI:
		break;
	case SCN_MODIFIED:
		if( (pMsg->modificationType & SC_MOD_INSERTTEXT) || (pMsg->modificationType & SC_MOD_DELETETEXT) )
		if(pMsg->linesAdded!=0 && sci_GetMarginWidthN(0)!=0) //自动调整行号的宽度
		{
			int lineNumber = sci_GetLineCount();
			int charWidth = sci_TextWidth(STYLE_LINENUMBER, "9");
			CDuiString tempLine;
			tempLine.Format(_T("%d"), lineNumber);
			sci_SetMarginWidthN(0,tempLine.GetLength()*charWidth+4);
		}
		break;
	case SCN_MACRORECORD:
		break;
	case SCN_MARGINCLICK:
		break;
	case SCN_NEEDSHOWN:
		break;
	case SCN_PAINTED:
		break;
	case SCN_USERLISTSELECTION:
		break;
	case SCN_URIDROPPED:
		break;
	case SCN_DWELLSTART:
		break;
	case SCN_DWELLEND:
		break;
	case SCN_ZOOM:
		break;
	case SCN_HOTSPOTCLICK:
		break;
	case SCN_HOTSPOTDOUBLECLICK:
		break;
	case SCN_HOTSPOTRELEASECLICK:
		break;
	case SCN_INDICATORCLICK:
		break;
	case SCN_INDICATORRELEASE:
		break;
	case SCN_CALLTIPCLICK:
		break;
	case SCN_AUTOCSELECTION:
		break;
	case SCN_AUTOCCANCELLED:
		break;
	case SCN_AUTOCCHARDELETED:
		break;
	case SCI_SETMODEVENTMASK:
		break;
	case SCI_GETMODEVENTMASK:
		break;
	case SCI_SETMOUSEDWELLTIME:
		break;
	case SCI_GETMOUSEDWELLTIME:
		break;
	case SCI_SETIDENTIFIER:
		break;
	case SCI_GETIDENTIFIER:
		break;
	case SCEN_CHANGE:
		break;
	case SCEN_SETFOCUS:
		break;
	case SCEN_KILLFOCUS:
		break;
	case SCN_FOCUSIN:
		GetManager()->SetFocus(this);
		break;
	}
}

CDuiString CSciEditUI::GetText() const
{
	if(!IsValidSciApi()) return _T("");

	CSciEditUI *pThis = const_cast<CSciEditUI *>(this);

	int code = pThis->sci_GetCodePage();

	UISTRING_CONVERSION;
	CStringA strA;
	pThis->sci_GetTextAll(strA);

	if(code == SC_CP_UTF8)
		return UIUTF82T(strA);
	return UIA2T(strA);
}

void CSciEditUI::SetText(LPCTSTR pstrText)
{
	if(!IsValidSciApi()) return;

	int code = sci_GetCodePage();

	UISTRING_CONVERSION;

	if(code == SC_CP_UTF8)
		sci_SetText(UIT2UTF8(pstrText));
	else
		sci_SetText(UIT2A(pstrText));
}

void CSciEditUI::SetViewLineNumber()
{
	if(!IsValidSciApi()) return;
	if(!m_bLineNumber)
	{
		sci_SetMarginWidthN(0,0);
		return;
	}

	//行号
	sci_SetMarginTypeN(0, SC_MARGIN_NUMBER);
	int lineNumber = sci_GetLineCount();
	int charWidth = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, LPARAM("9"));
	CDuiString tempLine;
	tempLine.Format(_T("%d"), lineNumber);
	sci_SetMarginWidthN(0,tempLine.GetLength()*charWidth+4);

	sci_StyleSetBack(STYLE_LINENUMBER, UIARGB_2_RGB(m_dwLineNumberBkColor));
	sci_StyleSetFore(STYLE_LINENUMBER, UIARGB_2_RGB(m_dwLineNumberColor));
}

void CSciEditUI::SetViewBreakPoint()
{
	if(!IsValidSciApi()) return;
	if(!m_bBreakPoint)
	{
		sci_SetMarginWidthN(1,0);
		return;
	}

	//断点
	sci_SetMarginTypeN(1, SC_MARGIN_SYMBOL);
	sci_SetMarginWidthN(1, 10);
	sci_SetMarginMaskN(1, 1);
}

void CSciEditUI::SetViewFolder()
{
	if(!IsValidSciApi()) return;
	if(!m_bFolder)
	{
		sci_SetMarginWidthN(2,0);
		return;
	}

	//代码折叠
	sci_SetMarginTypeN(2, SC_MARGIN_SYMBOL);	//页边类型
	sci_SetMarginMaskN(2, SC_MASK_FOLDERS);	//页边掩码
	sci_SetMarginWidthN(2,20);				//页边宽度
	sci_SetMarginSensitiveN(1, TRUE);		//页边响应鼠标点击

	// 折叠标签样式      
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PIXMAP);  
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_PIXMAP);  
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND,  SC_MARK_PIXMAP);  
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_PIXMAP);  
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);  
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);  
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE); 

	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDER, (sptr_t)plus_xpm);  
	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPEN, (sptr_t)minus_xpm);  
	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEREND, (sptr_t)plus_xpm);  
	SendEditor(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPENMID, (sptr_t)minus_xpm); 

	// 折叠标签颜色       
	sci_MarkerSetFore(SC_MARKNUM_FOLDEROPEN,	RGB(0,0,255));
	sci_MarkerSetFore(SC_MARKNUM_FOLDER,		RGB(0,0,255));
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0xa0a0a0);  
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0xa0a0a0);  
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0xa0a0a0);  

	//如果折叠就在折叠行的下面画一条横线  }  
	sci_SetFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED); 
}

void CSciEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("codepage")) == 0 )
	{
		if( _tcsicmp(pstrName, _T("UTF8")) == 0 )
			m_nCodePage = SC_CP_UTF8;
		else if( _tcsicmp(pstrValue, _T("GBK")) == 0 )
			m_nCodePage = 936;
		else if( _tcsicmp(pstrValue, _T("BIG5")) == 0 )
			m_nCodePage = 950;
		else if( _tcsicmp(pstrValue, _T("JapaneseShiftJIS")) == 0 )
			m_nCodePage = 932;
		else if( _tcsicmp(pstrValue, _T("KoreanUnifiedHangul")) == 0 )
			m_nCodePage = 949;
		else if( _tcsicmp(pstrName, _T("KoreanJohab")) == 0 )
			m_nCodePage = 1361;
		if(IsValidSciApi())
			sci_SetCodePage(m_nCodePage);	//默认使用UTF8编码
	}
	else if( _tcsicmp(pstrName, _T("linenumber")) == 0 )
	{
		m_bLineNumber = _tcsicmp(pstrValue, _T("true")) == 0;
		SetViewLineNumber();
	}
	else if( _tcsicmp(pstrName, _T("linenumbercolor")) == 0 )
	{
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dwLineNumberColor = _tcstoul(pstrValue, &pstr, 16);
		SetViewLineNumber();
	}
	else if( _tcsicmp(pstrName, _T("linenumberbkcolor")) == 0 )
	{
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dwLineNumberBkColor = _tcstoul(pstrValue, &pstr, 16);
		SetViewLineNumber();
	}
	else if( _tcsicmp(pstrName, _T("breakpoint")) == 0 )
	{
		m_bBreakPoint = _tcsicmp(pstrValue, _T("true")) == 0;
		SetViewBreakPoint();
	}
	else if( _tcsicmp(pstrName, _T("folder")) == 0 )
	{
		m_bFolder = _tcsicmp(pstrValue, _T("true")) == 0;
		SetViewFolder();
	}
	__super::SetAttribute(pstrName, pstrValue);
}

}