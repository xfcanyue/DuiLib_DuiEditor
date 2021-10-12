// SciWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SciWnd.h"
#include "colors.h"

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


#define VIEW_MARGIN_LINENUMBER 0
#define VIEW_MARGIN_BREAK	1
#define VIEW_MARGIN_FOLD	2

#define BREAK_TYPEN	0

// CSciWnd
IMPLEMENT_DYNAMIC(CSciWnd, CWnd)

CSciWnd::CSciWnd()
{
#ifdef _WIN64
	m_hSciLexer = ::LoadLibrary(_T("SciLexer64.dll"));
#else
	m_hSciLexer = ::LoadLibrary(_T("SciLexer.dll"));
#endif
}

CSciWnd::~CSciWnd()
{
	::FreeLibrary(m_hSciLexer);
}

BEGIN_MESSAGE_MAP(CSciWnd, CWnd)
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CSciWnd 消息处理程序
BOOL CSciWnd::Create (DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if(CreateEx(0, _T("Scintilla"), _T(""), dwStyle, rect, pParentWnd, nID, NULL))
	{	
		InitSciApi(m_hWnd);
		return TRUE;
	}
	return FALSE;
}

BOOL CSciWnd::LoadFile(LPCTSTR szPath)
{
	if (szPath == NULL || *szPath == '\0')
		return TRUE;

	CFile file;
	if(!file.Open(szPath, CFile::modeRead))	return FALSE;
	UINT buflen = (UINT)file.GetLength();

	char *pBuffer = new char[buflen + 1];
	file.Read((void *)pBuffer, buflen);
	pBuffer[buflen] = 0;
	sci_SetText(pBuffer);
	sci_SetSavePoint();//这是未修改的文档

	delete []pBuffer;
	file.Close();

	return TRUE;
}

BOOL CSciWnd::SaveFile(LPCTSTR szPath)
{

	if (szPath == NULL || *szPath == '\0')
		return TRUE;

	CFile file;
	if(!file.Open(szPath, CFile::modeCreate|CFile::modeWrite))	return FALSE;

	int buflen = sci_GetLength();
	TCHAR *pBuffer = new TCHAR[buflen + 1];
	if (pBuffer != NULL)
	{
		execute(SCI_GETTEXT, buflen + 1, (sptr_t)pBuffer);
		file.Write((void *)pBuffer, buflen);
		delete [] pBuffer;
	}
	file.Close();
	return TRUE;
}

void CSciWnd::InitXML()
{
	sci_SetLexer(SCLEX_XML);		//设定词法解析器
	sci_SetCodePage(SC_CP_UTF8);	//编码
	//sci_SetCodePage(936);	//编码	

	SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier New");//(LPARAM)"Courier New");//"微软雅黑");
	sci_StyleSetSize(STYLE_DEFAULT, 14);//14);
	sci_StyleSetFore(STYLE_DEFAULT,RGB(0,0,0));

	sci_StyleSetBack(STYLE_DEFAULT, RGB(255,255,255));
	sci_StyleSetBack(STYLE_LINENUMBER, RGB(255,255,255));
	sci_StyleSetBack(STYLE_INDENTGUIDE, RGB(255,255,255));

	for (int i = SCE_H_DEFAULT; i <= SCE_H_SGML_BLOCK_DEFAULT; i++)
	{
		sci_StyleSetBack(i,	RGB(255,255,255));
	}

	//设置选中文本背景色
	sci_SetSelBack(STYLE_DEFAULT, RGB(0xA0,0xCA,0xF0));//RGB(0xA0,0xCA,0xF0));

	sci_SetExtraDescent(1);
	sci_SetExtraAscent(1);

	//xml注释内容
	COLORREF crfComment = RGB(0,150,0);
	sci_StyleSetFore(SCE_H_COMMENT,				crfComment);
	//unknow attribute
	sci_StyleSetFore(SCE_H_ATTRIBUTEUNKNOWN,	crfComment);
	//unkow tag
	sci_StyleSetFore(SCE_H_TAGUNKNOWN,	crfComment);

	//字符串双引号未封闭时
	sci_StyleSetFore(SCE_H_DOUBLESTRING,	RGB(128,0,255));
	sci_StyleSetFore(SCE_H_SINGLESTRING,	RGB(128,0,255));

	//标签未配对时颜色
	sci_StyleSetFore(SCE_H_TAGUNKNOWN,		RGB(255,0,255));

	//标签配对时颜色
	sci_StyleSetFore(SCE_H_TAG,				RGB(0,0,255));
	sci_StyleSetFore(SCE_H_TAGEND,			RGB(0,0,255));

	//属性颜色
	sci_StyleSetFore(SCE_H_ATTRIBUTE,		RGB(255,0,0));
	sci_StyleSetBold(SCE_H_ATTRIBUTE,		true);

	//////////////////////////////////////////////////////////////////////////
	//行号
	sci_SetMarginTypeN(VIEW_MARGIN_LINENUMBER, SC_MARGIN_NUMBER);
	int lineNumber = sci_GetLineCount();
	//int charWidth = sci_TextWidth(STYLE_LINENUMBER, _T("9"));
	int charWidth = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, LPARAM("9"));
	CString tempLine;
	tempLine.Format(_T("%d"), lineNumber);
	sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);

	//断点
	sci_SetMarginTypeN(VIEW_MARGIN_BREAK, SC_MARGIN_SYMBOL);
	sci_SetMarginWidthN(VIEW_MARGIN_BREAK, 10);
	sci_SetMarginMaskN(VIEW_MARGIN_BREAK, 1);
	//sci_MarkerEnableHighlight(TRUE);
	sci_MarkerSetFore(BREAK_TYPEN, RGB(255,0,0));
	sci_MarkerSetBack(BREAK_TYPEN, RGB(255,0,0));

	//代码折叠
	sci_SetMarginTypeN(VIEW_MARGIN_FOLD, SC_MARGIN_SYMBOL);	//页边类型
	sci_SetMarginMaskN(VIEW_MARGIN_FOLD, SC_MASK_FOLDERS);	//页边掩码
	sci_SetMarginWidthN(VIEW_MARGIN_FOLD,20);				//页边宽度
	sci_SetMarginSensitiveN(VIEW_MARGIN_FOLD, TRUE);		//页边响应鼠标点击
	
	sci_SetProperty("lexer.xml.allow.scripts", "1");
	sci_SetProperty("fold", "1");
	sci_SetProperty("fold.html", "1");
	sci_SetProperty("fold.compact", "0");
	sci_SetProperty("fold.hypertext.comment", "1");

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

	sci_SetTabWidth(4);	//制表符宽度
	sci_SetIndentationGuides(SC_IV_LOOKBOTH);	//缩进提示线
	sci_SetIndent(4);	//缩进提示线的字符宽度
	sci_SetUseTabs(TRUE);

	//括号匹配的颜色
	sci_StyleSetBack(STYLE_BRACELIGHT, RGB(0,255,0));
	//括号未匹配的
	sci_StyleSetBack(STYLE_BRACEBAD, RGB(255,0,0));

	//当前行高亮显示
	sci_SetCaretLineVisible(TRUE);
	sci_SetCaretLineBack(RGB(215,215,247));

	//自动调整滚动条宽度
	sci_SetScrollWidthTracking(TRUE);

	//高亮格式
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE, red);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_SMART, liteGreen);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_INC, blue);
	sci_IndicSetFore(SCE_UNIVERSAL_TAGMATCH, purple);
	sci_IndicSetFore(SCE_UNIVERSAL_TAGATTR, yellow);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT1, cyan);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT2, orange);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT3, yellow);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT4, purple);
	sci_IndicSetFore(SCE_UNIVERSAL_FOUND_STYLE_EXT5, darkGreen);

	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_SMART, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_INC, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_TAGMATCH, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_TAGATTR, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT1, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT2, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT3, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT4, INDIC_ROUNDBOX);
	execute(SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_EXT5, INDIC_ROUNDBOX);	

	int aplpa = 80;
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_SMART, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_INC, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGMATCH, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGATTR, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT1, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT2, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT3, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT4, aplpa);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT5, aplpa);	

	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_SMART, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_INC, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_TAGMATCH, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_TAGATTR, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT1, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT2, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT3, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT4, true);
	execute(SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_EXT5, true);

	//快捷键不再显示乱码
	execute(SCI_CLEARCMDKEY, (WPARAM)('F'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('H'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('Z'+((SCMOD_CTRL|SCMOD_SHIFT)<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)('Y'+(SCMOD_CTRL<<16)), SCI_NULL);
	execute(SCI_CLEARCMDKEY, (WPARAM)SCK_ESCAPE, SCI_NULL);

	execute(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("lexer.xml.allow.scripts"), reinterpret_cast<LPARAM>("0"));
}

void CSciWnd::InitCpp()
{
	sci_SetLexer(SCLEX_CPP);		//设定词法解析器
	sci_SetCodePage(SC_CP_UTF8);	//编码

	SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier New");//(LPARAM)"Courier New");//"微软雅黑");
	sci_StyleSetSize(STYLE_DEFAULT, 13);//13);
	sci_StyleSetFore(STYLE_DEFAULT,RGB(0,0,0));

	sci_StyleSetBack(STYLE_DEFAULT, RGB(255,255,255));
	sci_StyleSetBack(STYLE_LINENUMBER, RGB(255,255,255));
	sci_StyleSetBack(STYLE_INDENTGUIDE, RGB(255,255,255));

	//设置选中文本背景色
	sci_SetSelBack(STYLE_DEFAULT, RGB(0xA0,0xCA,0xF0));//RGB(0xA0,0xCA,0xF0));

	sci_SetExtraDescent(1);
	sci_SetExtraAscent(1);

	//////////////////////////////////////////////////////////////////////////
	//行号
	sci_SetMarginTypeN(VIEW_MARGIN_LINENUMBER, SC_MARGIN_NUMBER);
	int lineNumber = sci_GetLineCount();
	//int charWidth = sci_TextWidth(STYLE_LINENUMBER, _T("9"));
	int charWidth = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, LPARAM("9"));
	CString tempLine;
	tempLine.Format(_T("%d"), lineNumber);
	sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);

	//断点
	sci_SetMarginTypeN(VIEW_MARGIN_BREAK, SC_MARGIN_SYMBOL);
	sci_SetMarginWidthN(VIEW_MARGIN_BREAK, 10);
	sci_SetMarginMaskN(VIEW_MARGIN_BREAK, 1);
	//sci_MarkerEnableHighlight(TRUE);
	sci_MarkerSetFore(BREAK_TYPEN, RGB(255,0,0));
	sci_MarkerSetBack(BREAK_TYPEN, RGB(255,0,0));

	//代码折叠
	sci_SetMarginTypeN(VIEW_MARGIN_FOLD, SC_MARGIN_SYMBOL);	//页边类型
	sci_SetMarginMaskN(VIEW_MARGIN_FOLD, SC_MASK_FOLDERS);	//页边掩码
	sci_SetMarginWidthN(VIEW_MARGIN_FOLD,20);				//页边宽度
	sci_SetMarginSensitiveN(VIEW_MARGIN_FOLD, TRUE);		//页边响应鼠标点击

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

	sci_SetTabWidth(4);	//制表符宽度
	sci_SetIndentationGuides(SC_IV_LOOKBOTH);	//缩进提示线
	sci_SetIndent(4);	//缩进提示线的字符宽度
	sci_SetUseTabs(TRUE);

	//括号匹配的颜色
	sci_StyleSetBack(STYLE_BRACELIGHT, RGB(0,255,0));

	//括号未匹配的
	sci_StyleSetBack(STYLE_BRACEBAD, RGB(255,0,0));

	//当前行高亮显示
	//sci_SetCaretLineVisible(TRUE);
	sci_SetCaretLineBack(RGB(215,215,247));

	//自动调整滚动条宽度
	sci_SetScrollWidthTracking(TRUE);

	execute(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold"), reinterpret_cast<LPARAM>("1"));
	execute(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold.compact"), reinterpret_cast<LPARAM>("0"));

	execute(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold.comment"), reinterpret_cast<LPARAM>("1"));
	execute(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold.preprocessor"), reinterpret_cast<LPARAM>("1"));

	// Disable track preprocessor to avoid incorrect detection.
	// In the most of cases, the symbols are defined outside of file.
	execute(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("lexer.cpp.track.preprocessor"), reinterpret_cast<LPARAM>("0"));
}

void CSciWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_SCIWND_RBUTTONUP, (WPARAM)&point, 0);
	CWnd::OnRButtonUp(nFlags, point);
}

void CSciWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_SCIWND_LBUTTONDOWN, (WPARAM)sci_GetCurrentPos(), 0);
	CWnd::OnLButtonDown(nFlags, point);
}

void CSciWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_SCIWND_MOUSEMOVE, (WPARAM)sci_GetCurrentPos(), 0);

	CWnd::OnMouseMove(nFlags, point);
}

void CSciWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	int pos = sci_GetCurrentPos();

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
	{
		pOwner->SendMessage(WM_SCIWND_LBUTTONUP, (WPARAM)pos, 0);
		pOwner->SendMessage(WM_SCIWND_CLICK, (WPARAM)pos, 0);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CSciWnd::OnParentNotify(SCNotification *pMsg)
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
		{
			if(pMsg->linesAdded!=0 &&
				sci_GetMarginWidthN(VIEW_MARGIN_LINENUMBER)!=0) //自动调整行号的宽度
			{
				int lineNumber = sci_GetLineCount();
				int charWidth = sci_TextWidth(STYLE_LINENUMBER, "9");
				CString tempLine;
				tempLine.Format(_T("%d"), lineNumber);
				sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);
			}
		}
		break;
	case SCN_MACRORECORD:
		break;
	case SCN_MARGINCLICK:
		{
			int line = sci_LineFromPosition(pMsg->position);
			sci_ToggleFold(line);
		}
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
	}

	return FALSE;
}



