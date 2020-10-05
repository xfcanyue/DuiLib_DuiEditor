// SciWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SciWnd.h"
#include "colors.h"
#include "xmlMatchedTagsHighlighter.h"

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
	if(CreateEx(0, _T("Scintilla"), _T(""), dwStyle, rect, pParentWnd, nID, NULL))
	{	
		m_sendeditor = (int (__cdecl *)(void *,int,int,int))SendMessage(SCI_GETDIRECTFUNCTION,0,0);
		m_pSendEditor = (void *)SendMessage(SCI_GETDIRECTPOINTER,0,0);
		//ModifyStyleEx(0, WS_EX_LAYERED);
		//SetLayeredWindowAttributes(0xFF0000, 255, LWA_ALPHA);

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

	CHAR *pBuffer = new CHAR[buflen + 1];
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
		execute(SCI_GETTEXT, buflen + 1,(long)pBuffer);
		file.Write((void *)pBuffer, buflen);
		delete [] pBuffer;
	}
	file.Close();
	return TRUE;
}

void CSciWnd::InitXML(const tagXmlEditorOpt &opt)
{
	//sci_UsePopup(FALSE);
	//sci_SetReadOnly(TRUE);

	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WINDOWS_7);


	//编码	
	sci_SetCodePage(SC_CP_UTF8);

	sci_SetLexer(SCLEX_XML);

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

	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_SMART, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_INC, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGMATCH, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_TAGATTR, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT1, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT2, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT3, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT4, 100);
	execute(SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_EXT5, 100);	

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
	//execute(SCI_CLEARALLCMDKEYS);

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
		{
			XmlMatchedTagsHighlighter xmlTagMatchHiliter(this);
			xmlTagMatchHiliter.tagMatch(false);
		}
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