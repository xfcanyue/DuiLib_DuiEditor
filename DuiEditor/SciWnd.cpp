// SciWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SciWnd.h"

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
//#define VIEW_MARGIN_MARKER	2
#define VIEW_MARGIN_FOLD	2

#define BREAK_TYPEN	0

// CSciWnd
IMPLEMENT_DYNAMIC(CSciWnd, CWnd)

CSciWnd::CSciWnd()
{
	m_hSciLexer = ::LoadLibrary(_T("SciLexer.dll"));
	memset(m_arrSpace,' ', 1024);
	memset(m_arrTab, '	', 1024);
}

CSciWnd::~CSciWnd()
{
	::FreeLibrary(m_hSciLexer);
}

BEGIN_MESSAGE_MAP(CSciWnd, CWnd)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CSciWnd 消息处理程序
BOOL CSciWnd::Create (DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if(CreateEx(dwExStyle, _T("Scintilla"), _T(""), dwStyle, rect,
		pParentWnd, nID, NULL))
	{	
		m_sendeditor = (int (__cdecl *)(void *,int,int,int))SendMessage(SCI_GETDIRECTFUNCTION,0,0);
		m_pSendEditor = (void *)SendMessage(SCI_GETDIRECTPOINTER,0,0);

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

	TCHAR test[255] = L"12345";

	TCHAR *pBuffer = new TCHAR[buflen+1];
	memset(pBuffer, 0, sizeof(TCHAR)*(buflen+1));
	file.Read((void *)pBuffer, buflen);

	sci_SetText((LPCTSTR)pBuffer);
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
	TCHAR *pBuffer = new TCHAR[buflen];
	if (pBuffer != NULL)
	{
		SendEditor(SCI_GETTEXT, buflen,(long)pBuffer);
		file.Write((void *)pBuffer, buflen);
		delete [] pBuffer;
	}
	file.Close();
	return TRUE;
}


void CSciWnd::InitXML()
{
	SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"微软雅黑");
	sci_StyleSetSize(STYLE_DEFAULT, 15);
	sci_StyleSetFore(STYLE_DEFAULT,RGB(0,0,0));

	//设置选中文本背景色
	sci_SetSelBack(TRUE, RGB(0xC0,0xC0,0xC0));

	//编码	
	sci_SetCodePage(SC_CP_UTF8);

	//////////////////////////////////////////////////////////////////////////
	sci_SetLexer(SCLEX_XML);
	for (int i=0; i<4; i++)
	{
		SendEditor(SCI_SETKEYWORDS, i, reinterpret_cast<LPARAM>(""));
	}

	sci_StyleSetFore(SCE_C_WORD, RGB(0,0,80));
	sci_StyleSetFore(SCE_C_WORD2, RGB(136,0,0));

	COLORREF crfComment = RGB(0,0,255);
	sci_StyleSetFore(SCE_C_COMMENT,				RGB(0,0,255)); //节点颜色
 	sci_StyleSetFore(SCE_C_COMMENTLINE,			crfComment); //?
 	sci_StyleSetFore(SCE_C_COMMENTDOC,			RGB(255,0,0)); //属性颜色
// 	sci_StyleSetFore(SCE_C_COMMENTLINEDOC,		RGB(255,0,0)); //
// 	sci_StyleSetFore(SCE_C_COMMENTDOCKEYWORD,	RGB(255,0,0));
// 	sci_StyleSetFore(SCE_C_COMMENTDOCKEYWORDERROR, crfComment);

	COLORREF crForce = RGB(0,0,255);
 	sci_StyleSetFore(SCE_C_NUMBER,		crForce);
 	sci_StyleSetFore(SCE_C_STRING,		crForce);
//	sci_StyleSetFore(SCE_C_CHARACTER,	crForce);
// 	sci_StyleSetFore(SCE_C_UUID,		crForce);

	//预处理，宏定义
	sci_StyleSetFore(SCE_C_PREPROCESSOR,	RGB(99,128,0));//RGB(160,0,160));

	//操作符,包括 ( ) { } = ; + - * / % < > <= >= == && || 
	sci_StyleSetFore(SCE_C_OPERATOR,		RGB(0,0,0));

	//标志符
	sci_StyleSetFore(SCE_C_IDENTIFIER,		RGB(0,0,0));

	//字符串双引号未封闭时
	sci_StyleSetFore(SCE_C_STRINGEOL,		RGB(255,0,0));

	//???
	sci_StyleSetFore(SCE_C_VERBATIM,		RGB(0,128,0));

	//???
	sci_StyleSetFore(SCE_C_REGEX,			RGB(0,128,0));	

	//???
	sci_StyleSetFore(SCE_C_GLOBALCLASS,	RGB(255,0,0));

	//???
	sci_StyleSetFore(SCE_C_STRINGRAW,		RGB(255,0,0));

	//???
	sci_StyleSetFore(SCE_C_TRIPLEVERBATIM, RGB(0,128,0));

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

	sci_SetProperty("fold","1");
	sci_SetProperty("fold.html", "1");
	sci_SetProperty("fold.html.preprocessor", "1");
	sci_SetProperty("fold.comment", "1");
	sci_SetProperty("fold.at.else", "1");
	sci_SetProperty("fold.flags", "1");
	sci_SetProperty("fold.preprocessor", "1");
	sci_SetProperty("styling.within.preprocessor","1");
	sci_SetProperty("asp.default.language", "1");

	// 	// 折叠标签样式      
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

	//当前行高亮显示
	sci_SetCaretLineVisible(TRUE);
	sci_SetCaretLineBack(RGB(215,215,247));

	//自动调整滚动条宽度
	sci_SetScrollWidthTracking(TRUE);
}

void CSciWnd::InitCPP()
{
	//sci_UsePopup(FALSE);

	sci_StyleSetFont( STYLE_DEFAULT, "微软雅黑");
	sci_StyleSetSize(STYLE_DEFAULT,12);
	sci_StyleSetFore(STYLE_DEFAULT,RGB(0,0,0));
	sci_StyleSetCharacterSet(SCE_C_STRING, SC_CHARSET_GB2312);
	sci_StyleClearAll();

	//设置选中文本背景色
	sci_SetSelBack(TRUE, RGB(0xC0,0xC0,0xC0));

	//编码	
	sci_SetCodePage(936);

	//////////////////////////////////////////////////////////////////////////
	sci_SetLexer(SCLEX_CPP);

	sci_StyleSetFore(SCE_C_WORD, RGB(0,0,255));
	sci_StyleSetFore(SCE_C_WORD2, RGB(136,0,0));

	COLORREF crfComment = RGB(0,150,0);
	sci_StyleSetFore(SCE_C_COMMENT,			crfComment); //块注释  /*...*/
	sci_StyleSetFore(SCE_C_COMMENTLINE,		crfComment); //行注释  //...
	sci_StyleSetFore(SCE_C_COMMENTDOC,			crfComment); //文档注释  /**...*/
	sci_StyleSetFore(SCE_C_COMMENTLINEDOC,		RGB(255,0,0)); //
	sci_StyleSetFore(SCE_C_COMMENTDOCKEYWORD,	RGB(255,0,0));
	sci_StyleSetFore(SCE_C_COMMENTDOCKEYWORDERROR, crfComment);


	sci_StyleSetFore(SCE_C_NUMBER, RGB(100,100,100));

	sci_StyleSetFore(SCE_C_STRING,			RGB(100,100,100));
	sci_StyleSetFore(SCE_C_CHARACTER,		RGB(100,100,100));
	sci_StyleSetFore(SCE_C_UUID,			RGB(100,100,100));

	//预处理，宏定义
	sci_StyleSetFore(SCE_C_PREPROCESSOR,	RGB(160,0,160));

	//操作符,包括 ( ) { } = ; + - * / % < > <= >= == && || 
	sci_StyleSetFore(SCE_C_OPERATOR,		RGB(0,0,0));

	//标志符
	sci_StyleSetFore(SCE_C_IDENTIFIER,		RGB(0,0,0));

	//字符串双引号未封闭时
	sci_StyleSetFore(SCE_C_STRINGEOL,		RGB(255,0,0));

	//???
	sci_StyleSetFore(SCE_C_VERBATIM,		RGB(0,128,0));

	//???
	sci_StyleSetFore(SCE_C_REGEX,			RGB(0,128,0));	

	//???
	sci_StyleSetFore(SCE_C_GLOBALCLASS,	RGB(255,0,0));

	//???
	sci_StyleSetFore(SCE_C_STRINGRAW,		RGB(255,0,0));

	//???
	sci_StyleSetFore(SCE_C_TRIPLEVERBATIM, RGB(0,128,0));

	//////////////////////////////////////////////////////////////////////////

	//行号
	sci_SetMarginTypeN(VIEW_MARGIN_LINENUMBER, SC_MARGIN_NUMBER);
	int lineNumber = sci_GetLineCount();
	int charWidth = sci_TextWidth(STYLE_LINENUMBER, "9");
	CStringA tempLine;
	tempLine.Format("%d", lineNumber);
	sci_SetMarginWidthN(VIEW_MARGIN_LINENUMBER,tempLine.GetLength()*charWidth+4);

	//断点
	sci_SetMarginTypeN(VIEW_MARGIN_BREAK, SC_MARGIN_SYMBOL);
	sci_SetMarginWidthN(VIEW_MARGIN_BREAK, 10);
	sci_SetMarginMaskN(VIEW_MARGIN_BREAK, 1);
	//sci_MarkerEnableHighlight(TRUE);
	sci_MarkerSetFore(BREAK_TYPEN, RGB(255,0,0));
	sci_MarkerSetBack(BREAK_TYPEN, RGB(255,0,0));

	//代码折叠
	sci_SetMarginWidthN(VIEW_MARGIN_FOLD, SC_MARGIN_SYMBOL);
	sci_SetMarginWidthN(VIEW_MARGIN_FOLD,15);
	sci_SetMarginMaskN(VIEW_MARGIN_FOLD, SC_MASK_FOLDERS);
	sci_SetMarginSensitiveN(VIEW_MARGIN_FOLD, TRUE);
	sci_MarkerDefine(SC_MASK_FOLDERS, SC_MARK_CIRCLEMINUSCONNECTED);

	sci_SetProperty("fold","1");
	sci_SetProperty("fold.html", "1");
	sci_SetProperty("fold.html.preprocessor", "1");
	sci_SetProperty("fold.comment", "1");
	sci_SetProperty("fold.at.else", "1");
	sci_SetProperty("fold.flags", "1");
	sci_SetProperty("fold.preprocessor", "1");
	sci_SetProperty("styling.within.preprocessor","1");
	sci_SetProperty("asp.default.language", "1");

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

	//当前行高亮显示
	sci_SetCaretLineVisible(TRUE);
	sci_SetCaretLineBack(RGB(215,215,247));

	//自动调整滚动条宽度
	sci_SetScrollWidthTracking(TRUE);
}

void CSciWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_SCIWND_RBUTTONUP, (WPARAM)&point, 0);
	CWnd::OnRButtonUp(nFlags, point);
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
		//m_SciWnd.BraceHighlight();
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
		{
			int pos = sci_GetCurrentPos();
			int startPos = sci_WordStartPosition(pos-1, TRUE);
			int endPos = sci_WordEndPosition(pos-1, TRUE);
			CStringA strSelected;
			sci_AutocGetCurrentText(strSelected);

			sci_SetTargetStart(startPos);
			sci_SetTargetEnd(endPos);
			sci_ReplaceTarget(-1, strSelected);
			sci_GoToPos(startPos + strSelected.GetLength());

			sci_AutocCancel(); //不使用默认的插人
			return TRUE;
		}
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