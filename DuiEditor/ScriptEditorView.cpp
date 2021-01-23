// ScriptEditorView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ScriptEditorView.h"
#include "MainFrm.h"

#include "DlgScriptTest.h"
#include "../DuiScript/angelscript/add_on/scriptstdtime/AsTime.h"
#include "../DuiScript/angelscript/add_on/scriptstdtime/AsTime.cpp"
#include <sstream>

#define WM_REFRESH_STACK	WM_USER + 2001
static BOOL CALLBACK ScriptMessageCallback(TScriptMessage *pMsg, UINT_PTR userdata)
{
	CScriptEditorView *pView = (CScriptEditorView *)userdata;
	if(pMsg->nType == 0)
	{
		int pos1 = pView->sci.sci_PositionFromLine(pMsg->line);
		int pos2 = pView->sci.sci_GetLineEndPosition(pMsg->line);
		pView->sci.sci_SetSel(pos1, pos2);
	}
	else if(pMsg->nType == 1)
	{
		return pView->sci.sci_MarkerGet(pMsg->line) == 1;
	}
	else if(pMsg->nType == 2)
	{
		pView->SendMessage(WM_REFRESH_STACK, (WPARAM)pMsg->ctx, 1);
	}
	else if(pMsg->nType == 3)
	{
		InsertMsg(LSA2T(pMsg->lpszNotifyText));
	}
	else if(pMsg->nType == 5)
	{	
		InsertMsgV(_T("返回值：%d"), pMsg->ctx->GetReturnWord());
		int pos = pView->sci.sci_GetCurrentPos();
		pView->sci.sci_SetSel(-1, pos);
		pView->SendMessage(WM_REFRESH_STACK, 0, 0);
	}
	else if(pMsg->nType == 6)
	{	
		int pos = pView->sci.sci_GetCurrentPos();
		pView->sci.sci_SetSel(-1, pos);
		pView->SendMessage(WM_REFRESH_STACK, 0, 0);
	}

	return TRUE;
}

static void print(LPCTSTR str)
{
	InsertMsg(str);
}

// CScriptEditorView

IMPLEMENT_DYNCREATE(CScriptEditorView, CView)

CScriptEditorView::CScriptEditorView()
{
	memset(m_arrSpace,' ', 1024);
	memset(m_arrTab, '	', 1024);
	m_pHelper = NULL;
}

CScriptEditorView::~CScriptEditorView()
{
}

BEGIN_MESSAGE_MAP(CScriptEditorView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_SCIWND_MOUSEMOVE, OnSciMouseMove)
	ON_COMMAND(ID_EDIT_UNDO, &CScriptEditorView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CScriptEditorView::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CScriptEditorView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CScriptEditorView::OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_CUT, &CScriptEditorView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CScriptEditorView::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CScriptEditorView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CScriptEditorView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, &CScriptEditorView::OnEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CScriptEditorView::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_PASTE, &CScriptEditorView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CScriptEditorView::OnUpdateEditPaste)
	ON_COMMAND(ID_SCRIPT_INSERT_BREAK_POINT, &CScriptEditorView::OnScriptInsertBreakPoint)
	ON_MESSAGE(WM_REFRESH_STACK, &CScriptEditorView::OnRefreshStack)
	ON_COMMAND(ID_SCRIPT_RUN, &CScriptEditorView::OnScriptRun)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_RUN, &CScriptEditorView::OnUpdateScriptRun)
	ON_COMMAND(ID_SCRIPT_ABORT, &CScriptEditorView::OnScriptAbort)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_ABORT, &CScriptEditorView::OnUpdateScriptAbort)
	ON_COMMAND(ID_SCRIPT_STEP_INTO, &CScriptEditorView::OnScriptStepInto)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_STEP_INTO, &CScriptEditorView::OnUpdateScriptStepInto)
	ON_COMMAND(ID_SCRIPT_STEP_OVER, &CScriptEditorView::OnScriptStepOver)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_STEP_OVER, &CScriptEditorView::OnUpdateScriptStepOver)
	ON_COMMAND(ID_SCRIPT_STEP_RETURN, &CScriptEditorView::OnScriptStepReturn)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_STEP_RETURN, &CScriptEditorView::OnUpdateScriptStepReturn)
	ON_COMMAND(ID_SCRIPT_STEP_CURSOR, &CScriptEditorView::OnScriptStepCursor)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_STEP_CURSOR, &CScriptEditorView::OnUpdateScriptStepCursor)
	ON_WM_DESTROY()
	ON_COMMAND(ID_SCRIPT_EXCUTE, &CScriptEditorView::OnScriptExcute)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_EXCUTE, &CScriptEditorView::OnUpdateScriptExcute)
	ON_COMMAND(ID_SCRIPT_ADD_FUN1, &CScriptEditorView::OnScriptAddFun1)
	ON_COMMAND(ID_SCRIPT_ADD_FUN2, &CScriptEditorView::OnScriptAddFun2)
	ON_COMMAND(ID_SCRIPT_ADD_FUN3, &CScriptEditorView::OnScriptAddFun3)
	ON_COMMAND(ID_SCRIPT_ADD_FUN4, &CScriptEditorView::OnScriptAddFun4)
	ON_COMMAND(ID_SCRIPT_ADD_FUN5, &CScriptEditorView::OnScriptAddFun5)
	ON_COMMAND(ID_SCRIPT_ADD_FUN6, &CScriptEditorView::OnScriptAddFun6)
	ON_COMMAND(ID_SCRIPT_ADD_FUN7, &CScriptEditorView::OnScriptAddFun7)
	ON_COMMAND(ID_SCRIPT_ADD_FUN8, &CScriptEditorView::OnScriptAddFun8)
	ON_COMMAND(ID_SCRIPT_ADD_FUN9, &CScriptEditorView::OnScriptAddFun9)
	ON_COMMAND(ID_SCRIPT_ADD_FUN10, &CScriptEditorView::OnScriptAddFun10)
	ON_COMMAND(ID_SCRIPT_ADD_FUN11, &CScriptEditorView::OnScriptAddFun11)
	ON_COMMAND(ID_SCRIPT_ADD_FUN12, &CScriptEditorView::OnScriptAddFun12)
	ON_COMMAND(ID_SCRIPT_ADD_FUN13, &CScriptEditorView::OnScriptAddFun13)
	ON_COMMAND(ID_SCRIPT_PARSE_TOKEN, &CScriptEditorView::OnScriptParseToken)
END_MESSAGE_MAP()


// CScriptEditorView 绘图

void CScriptEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CScriptEditorView 消息处理程序


int CScriptEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	CRect rectDummy;
	rectDummy.SetRect(0,0,100,100);
	if (!sci.Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, rectDummy, this, ID_SCI_WND))
	{
		return -1;      // 未能创建
	}
	sci.InitCpp();
	sci.sci_UsePopup(FALSE);

	sci.sci_StyleSetFore(SCE_C_WORD, RGB(0,0,255));
	sci.sci_StyleSetFore(SCE_C_WORD2, RGB(136,0,0));

	COLORREF crfComment = RGB(0,150,0);
	sci.sci_StyleSetFore(SCE_C_COMMENT,			crfComment); //块注释  /*...*/
	sci.sci_StyleSetFore(SCE_C_COMMENTLINE,		crfComment); //行注释  //...
	sci.sci_StyleSetFore(SCE_C_COMMENTDOC,			crfComment); //文档注释  /**...*/
	sci.sci_StyleSetFore(SCE_C_COMMENTLINEDOC,		RGB(255,0,0)); //
	sci.sci_StyleSetFore(SCE_C_COMMENTDOCKEYWORD,	RGB(255,0,0));
	sci.sci_StyleSetFore(SCE_C_COMMENTDOCKEYWORDERROR, crfComment);

	sci.sci_StyleSetFore(SCE_C_NUMBER, RGB(100,100,100));

	sci.sci_StyleSetFore(SCE_C_STRING,			RGB(100,100,100));
	sci.sci_StyleSetFore(SCE_C_CHARACTER,		RGB(100,100,100));
	sci.sci_StyleSetFore(SCE_C_UUID,			RGB(100,100,100));

	//预处理，宏定义
	sci.sci_StyleSetFore(SCE_C_PREPROCESSOR,	RGB(160,0,160));

	//操作符,包括 ( ) { } = ; + - * / % < > <= >= == && || 
	sci.sci_StyleSetFore(SCE_C_OPERATOR,		RGB(0,0,0));

	//标志符
	sci.sci_StyleSetFore(SCE_C_IDENTIFIER,		RGB(0,0,0));

	//字符串双引号未封闭时
	sci.sci_StyleSetFore(SCE_C_STRINGEOL,		RGB(255,0,0));

	//???
	sci.sci_StyleSetFore(SCE_C_VERBATIM,		RGB(0,128,0));

	//???
	sci.sci_StyleSetFore(SCE_C_REGEX,			RGB(0,128,0));	

	//???
	sci.sci_StyleSetFore(SCE_C_GLOBALCLASS,	RGB(255,0,0));

	//???
	sci.sci_StyleSetFore(SCE_C_STRINGRAW,		RGB(255,0,0));

	//???
	sci.sci_StyleSetFore(SCE_C_TRIPLEVERBATIM, RGB(0,128,0));

	
	if(((CDuiEditorApp *)AfxGetApp())->m_funCreateScriptHelper)
	{
		m_pHelper = (* ((CDuiEditorApp *)AfxGetApp())->m_funCreateScriptHelper)();
		//m_pHelper->GetEngine()->RegisterGlobalFunction("void print(LPCTSTR)", asFUNCTION(print), asCALL_CDECL);
		m_pHelper->SetScriptMessageCallBack(ScriptMessageCallback, (UINT_PTR)this);
		m_lexer.SetEngine(m_pHelper->GetEngine());
	}

	sci.execute(SCI_SETMOUSEDWELLTIME, 500);
	sci.execute(SCI_CALLTIPSETFORE, RGB(0,0,0));

	InitLexer();
	return 0;
}


void CScriptEditorView::InitLexer()
{
	std::string strKetyWord1, strKetyWord2, strKetyWord3, strKetyWord4;
	m_lexer.InitLexer(strKetyWord1, strKetyWord2, strKetyWord3, strKetyWord4);
	sci.sci_SetKeyWords(0, strKetyWord1.c_str());
	sci.sci_SetKeyWords(1, strKetyWord2.c_str());
	sci.sci_SetKeyWords(2, strKetyWord3.c_str());
	sci.sci_SetKeyWords(3, strKetyWord4.c_str());	
}

int CScriptEditorView::ReplaceSpace(int start, int end, int lineIndent)
{
	int tabWidth = sci.sci_GetTabWidth();
	int lastTabNum = lineIndent/tabWidth;
	int lastSpaceNum = lineIndent%tabWidth;

	sci.sci_SetTargetStart(start);
	sci.sci_SetTargetEnd(end);
	sci.sci_ReplaceTarget(lastTabNum, m_arrTab);
	if(lastSpaceNum>0)
	{
		sci.sci_SetTargetStart(start+lastTabNum);
		sci.sci_SetTargetEnd(start+lastTabNum);
		sci.sci_ReplaceTarget(lastSpaceNum, m_arrSpace);
	}

	return start+lastTabNum+lastSpaceNum;
	//sci_GoToPos(start+lastTabNum+lastSpaceNum);
}

void CScriptEditorView::AddNewLine()
{
	int lineEndings = sci.sci_GetEolMode();
	if(lineEndings==SC_EOL_CRLF)
	{
		sci.sci_AddText(2, "\r\n");
	}
	else if (lineEndings==SC_EOL_CR)
	{
		sci.sci_AddText(1, "\r");
	}
	else
	{
		sci.sci_AddText(1, "\n");
	}
}
void CScriptEditorView::AutoIndentation(char ch)
{
	int pos = sci.sci_GetCurrentPos();
	int line = sci.sci_LineFromPosition(pos);	
	int tabWidth = sci.sci_GetTabWidth();
	if(line<=0)	return;

	int lastLineIndent = sci.sci_GetLineIndentation(line-1);


	//换行时
	int lineEndings = sci.sci_GetEolMode();
	if((lineEndings==SC_EOL_CRLF && ch=='\n') ||
		(lineEndings==SC_EOL_LF  && ch=='\n') ||
		(lineEndings==SC_EOL_CR  && ch=='\r'))
	{
		//检查当前pos后面是不是有空格,也替换掉.
		int lineBeginPos = sci.sci_PositionFromLine(line);
		int lineEndPos	= sci.sci_GetLineEndPosition(line);
		int endpos=pos;
		for (int i=lineBeginPos; i<=lineEndPos; i++)
		{
			if(IsSpace(sci.sci_GetCharAt(i)))
				endpos++;
			else
				break;
		}
		sci.sci_GoToPos(ReplaceSpace(pos, endpos, lastLineIndent));
	}
	else if(ch == '{') //如果'{'左边全是空格，说明是开始一个块
	{
		BOOL bBlock = TRUE;
		int lineBeginPos = sci.sci_PositionFromLine(line);	
		for (int i=lineBeginPos; i<pos-1; i++)
		{
			if(!IsSpace(sci.sci_GetCharAt(i)))
			{
				bBlock = FALSE;
				break;
			}
		}

		if(bBlock) //开始"{"
		{
			int posBegin = ReplaceSpace(lineBeginPos, pos-1, lastLineIndent)+1;
			sci.sci_GoToPos(posBegin);

			//补上"}"
			AddNewLine();	
			sci.sci_AddText(1, "}");
			AutoIndentation('}');

			sci.sci_GoToPos(posBegin);
			AddNewLine();
			int curPos = sci.sci_GetCurrentPos();
			sci.sci_GoToPos(ReplaceSpace(curPos, curPos, lastLineIndent+tabWidth));
		}		
	}
	else if(ch == '}')
	{
		BOOL bBlock = TRUE;
		int lineBeginPos = sci.sci_PositionFromLine(line);
		for (int i=lineBeginPos; i<pos-1; i++)
		{
			if(!IsSpace(sci.sci_GetCharAt(i)))
			{
				bBlock = FALSE;
				break;
			}
		}	

		if(bBlock)
		{
			sci.sci_GoToPos(ReplaceSpace(lineBeginPos, pos-1, lastLineIndent)+1);
		}
	}
}

void CScriptEditorView::AutoCompletion(int ich)
{
	CStringA str = LSUTF82A(&ich);
	char ch = str.GetAt(0);
	if((BYTE)ch > 127) return;

	int pos = sci.sci_GetCurrentPos();
	if(ch == '(')
	{
		//判断是否需要加 ")"
		int deep = 1;
		for (int i=pos; i<m_strTextCode.GetLength(); i++)
		{
			char chcode = m_strTextCode.GetAt(i);
			if(chcode == ';') break;
			else if(chcode == ')') deep--;
			else if(chcode == '(') deep++;
			else if(chcode == '/' && m_strTextCode.GetAt(i+1) == '/') break;
		}
		if(deep > 0)
			sci.sci_InsertText(pos, ")");
	}
	else if(ch == '\"')
	{
		//判断是否闭合引号
		int deep = 1;
		for (int i=pos; i<m_strTextCode.GetLength(); i++)
		{
			char chcode = m_strTextCode.GetAt(i);
			if(chcode == ';') break;
			else if(chcode == '\"') deep--;
			else if(chcode == '\"') deep++;
			else if(chcode == '/' && m_strTextCode.GetAt(i+1) == '/') break;
			else if(chcode == '/' && m_strTextCode.GetAt(i+1) == '*') break;
		}
		if(deep > 0)
			sci.sci_InsertText(pos, "\"");
	}
	else if(ch == ';' || ch == '\n' || ch == '}') //当输入这些字符时，需要扫描源代码
	{
		m_lexer.ParseToken(m_strTextCode, m_strTextCode.GetLength());
		return;
	}
	else if( (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '.') 
	{
		AutoCompletionShow2();
	}
}

void CScriptEditorView::AutoCompletionShow2()
{
	CStringA strWord;
	int pos = sci.sci_GetCurrentPos();
	int startPos = sci.sci_WordStartPosition(pos, TRUE);
	int endPos = sci.sci_WordEndPosition(pos, TRUE);
	sci.sci_GetTextRange(startPos, endPos, strWord);
	strWord = LSUTF82A(strWord);

	CStringA strShow;
	strShow = m_lexer.GetLexerText(m_strTextCode, pos, strWord).c_str();
	if(!strShow.IsEmpty())
	{
		sci.sci_AutocShow(0, strShow);
	}
}

void CScriptEditorView::CallTips(int pos)
{
	CStringA strTips;
	CStringA strWord;
	int startPos = sci.sci_WordStartPosition(pos, TRUE);
	int endPos = sci.sci_WordEndPosition(pos, TRUE);
	sci.sci_GetTextRange(startPos, endPos, strWord);
	strWord = LSUTF82A(strWord);

	char ch = sci.sci_GetCharAt(startPos-1);
	if(ch == '.')
	{
		CStringA strClass;
		int startPos2 = sci.sci_WordStartPosition(startPos-2, TRUE);
		int endPos2 = sci.sci_WordEndPosition(startPos-2, TRUE);
		sci.sci_GetTextRange(startPos2, endPos2, strClass);
		strClass = LSUTF82A(strClass);
		strTips = m_lexer.GetMethodTips(strClass, strWord).c_str();
	}
	else
	{
		strTips = m_lexer.GetFunctionTips(strWord).c_str();
	}

	if(!strTips.IsEmpty())
	{
		sci.sci_CallTipShow(startPos, strTips);
	}
}

void CScriptEditorView::BraceHighlight()
{
	sci.sci_BraceHighlight(-1,-1);

	int pos = sci.sci_GetCurrentPos();
	int startbe, endbe;
	m_arrBrace.empty();
	if(FindBrace("[]", pos, startbe, endbe))
	{
		int lend = sci.sci_BraceMatch(startbe, endbe);
		if(lend>=0 && pos<=lend)
		{
			sci.sci_BraceHighlight(startbe, lend);
			return;
		}			
	}

	if(FindBrace("()", pos, startbe, endbe))
	{
		int lend = sci.sci_BraceMatch(startbe, endbe);
		if(lend>=0 && pos<=lend)
		{
			sci.sci_BraceHighlight(startbe, lend);
			return;
		}			
	}

	if(FindBrace("{}", pos, startbe, endbe))
	{
		int lend = sci.sci_BraceMatch(startbe, endbe);
		if(lend>=0 && pos<=lend)
		{
			sci.sci_BraceHighlight(startbe, lend);
			return;
		}			
	}
}

BOOL CScriptEditorView::FindBrace(const char * brace, int pos, int &start, int &end)
{
	//往上搜索, 寻找一个}
	sci.sci_SetSearchFlags(SCFIND_REGEXP);
	sci.sci_SetTargetStart(pos);
	sci.sci_SetTargetEnd(0);
	CStringA temp;
	temp.Format("[%s]", brace);
	int findbe = sci.sci_SearchInTarget(1, temp);	
	if(findbe<0) return FALSE;

	start = sci.sci_GetTargetStart();
	end   = sci.sci_GetTargetEnd();

	char ch = sci.sci_GetCharAt(start);
	if(ch == brace[0])
	{
		if(m_arrBrace.size()==0)
		{
			return TRUE;
		}
		m_arrBrace.pop();
		FindBrace(brace, start, start, end);
	}
	else if(ch == brace[1])
	{
		m_arrBrace.push(ch);
		FindBrace(brace, start, start, end);
	}

	return findbe;
}

BOOL CScriptEditorView::CheckBraceChar(int pos)
{
	int line = sci.sci_LineFromPosition(pos);
	int end = sci.sci_GetLineEndPosition(line);
	int x=1;
	for (int i=pos; i<end; i++)
	{
		if(sci.sci_GetCharAt(i) == '"' && sci.sci_GetCharAt(i-1) != '\\')
		{
			if(x==0) x=1;
			if(x==1) x=0;
		}
	}
	return x;
}

void CScriptEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	m_bFirstLoad = TRUE;
	sci.LoadFile(GetDocument()->m_strLoadFileName);
	sci.sci_GetTextAll(m_strTextCode);
	m_lexer.ParseToken(m_strTextCode, m_strTextCode.GetLength());
	sci.sci_SetSavePoint();

	if(GetDocument()->m_bLoadFileFromBackup)
	{
		GetDocument()->SetModifiedFlag(TRUE);
	}
}


void CScriptEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(!sci.GetSafeHwnd())	return;

	CRect rcClient;
	GetClientRect(rcClient);
	sci.MoveWindow(rcClient);
}


BOOL CScriptEditorView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;

	if (phDR == NULL || phDR->hwndFrom != sci.m_hWnd)
	{
		return __super::OnNotify(wParam, lParam, pResult);
	}

	SCNotification *pMsg = (SCNotification*)lParam;
	sci.OnParentNotify(pMsg);

	switch (pMsg->nmhdr.code)
	{
	case SCK_BACK:
		{
			//InsertMsg(_T("SCK_BACK"));
		}
		break;
	case SCN_CHARADDED:	//输入字符
		{
			AutoIndentation(pMsg->ch);
			AutoCompletion(pMsg->ch);
			//InsertMsg(_T("SCN_CHARADDED"));
		}
		break;
	case SCN_AUTOCSELECTION:
		{
			CStringA strSelected;
			sci.sci_AutocGetCurrentText(strSelected);

			int pos = sci.sci_GetCurrentPos();
			int charpos = pos - 1;
			if(sci.sci_GetCharAt(charpos) == '.')
			{
				sci.sci_InsertText(pos, strSelected);
				sci.sci_GoToPos(pos + strSelected.GetLength());
			}
			else
			{
				int startPos = sci.sci_WordStartPosition(charpos, TRUE);
				int endPos = sci.sci_WordEndPosition(charpos, TRUE);

				sci.sci_SetTargetStart(startPos);
				sci.sci_SetTargetEnd(endPos);
				sci.sci_ReplaceTarget(-1, strSelected);
				sci.sci_GoToPos(startPos + strSelected.GetLength());
			}	

			sci.sci_AutocCancel();
		}
		break;
	case SCN_AUTOCCOMPLETED:
		break;
	case SCN_MODIFIED:
		{
			if(pMsg->modificationType & SC_MOD_INSERTTEXT)
			{
				//InsertMsg(_T("SC_MOD_INSERTTEXT"));
				if(m_bFirstLoad) m_bFirstLoad = FALSE;
				else
				{
					m_strTextCode.Insert(pMsg->position, CStringA(pMsg->text, pMsg->length));
					//AutoCompletionShow2();
				}
			}
			if(pMsg->modificationType & SC_MOD_DELETETEXT)
			{
				//InsertMsg(_T("SC_MOD_DELETETEXT"));
				if(m_bFirstLoad) m_bFirstLoad = FALSE;
				else
				{
					m_strTextCode.Delete(pMsg->position, pMsg->length);
					AutoCompletionShow2();
				}
			}	

			//保存备份文件
			if((pMsg->modificationType & SC_MOD_INSERTTEXT) || (pMsg->modificationType & SC_MOD_DELETETEXT))
			{
				CString filename = GetDocument()->GetPathName();
				if(!filename.IsEmpty())
				{
					GetDocument()->m_fileSession.attribute_auto("filename").set_value(LST2UTF8(filename));

					xml_attribute attrBackup = GetDocument()->m_fileSession.attribute_auto("backup");
					CString backupfile = LSUTF82T(attrBackup.as_string());
					if(backupfile.IsEmpty())
					{
						// always capture the complete file name including extension (if present)
						LPCTSTR lpszPathName = (LPCTSTR)filename;
						LPTSTR lpszTemp = (LPTSTR)lpszPathName;
						for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
						{
							// remember last directory/drive separator
							if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
								lpszTemp = (LPTSTR)_tcsinc(lpsz);
						}
						CString fileTitle = lpszTemp;

						SYSTEMTIME st;
						GetLocalTime(&st);
						CString file;
						file.Format(_T("%s %04d-%02d-%02d %02d.%02d.%02d.%d.as"), fileTitle, 
							st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
						backupfile = g_strAppPath + _T("DuiBackup\\");
						CreateDirectory(backupfile,NULL);
						backupfile += file;

						attrBackup.set_value(LST2UTF8(backupfile));
						g_cfg.SaveConfig();
					}
					sci.SaveFile(backupfile);
				}		
			}
		}
		break;
	case SCN_SAVEPOINTREACHED:	//文件被保存
		{
			GetDocument()->SetModifiedFlag(FALSE);
		}
		break;
	case SCN_SAVEPOINTLEFT: //文件被修改
		{			
			GetDocument()->SetModifiedFlag(TRUE);
		}
		break;
	case SCN_UPDATEUI:
		//BraceHighlight();
		UpdateFrameStatus();
		break;
	case SCN_DWELLSTART:
		{
			//sci.sci_CallTipShow(pMsg->position, "SCN_DWELLSTART");
			CallTips(pMsg->position);
		}
		break;
	case SCN_DWELLEND:
		sci.sci_CallTipCancel();
		break;
	}	

	// 	CString temp;
	// 	temp.Format(_T("sciwnd msg=%d"), pMsg->nmhdr.code);
	// 	InsertMsg(temp);
	return __super::OnNotify(wParam, lParam, pResult);
}

void CScriptEditorView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(((CDuiEditorApp *)AfxGetApp())->m_funDeleteScriptHelper)
	{
		m_pHelper->DebugStop();
		(*((CDuiEditorApp *)AfxGetApp())->m_funDeleteScriptHelper)(m_pHelper);
		m_pHelper= NULL;
	}
}

BOOL CScriptEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return CView::PreCreateWindow(cs);
}

void CScriptEditorView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&pt);
	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_SCRIPT_EDIT);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, AfxGetMainWnd(), TRUE);
}

void CScriptEditorView::UpdateFrameStatus()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	int curLine = sci.sci_GetCurLine();
	int curPos = sci.sci_GetCurrentPos();

	CString temp;
	temp.Format(_T("行: %d"), curLine+1);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LINE, temp);

	int col = curPos - sci.sci_PositionFromLine(curLine);
	temp.Format(_T("列: %d"), col);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_COL, temp);

	temp.Format(_T("位置: %d"), curPos);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURRENT_POS, temp);

	temp.Format(_T("总长度: %d"), sci.sci_GetLength());
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LENGTH, temp);

	temp.Format(_T("总行数: %d"), sci.sci_GetLineCount());
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_TOTAL_LINE, temp);
}

LRESULT CScriptEditorView::OnSciMouseMove(WPARAM WParam, LPARAM LParam)
{
	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(sci.GetSafeHwnd(), &pt);

	int pos = sci.sci_PostionFromPoint(pt.x, pt.y);
	int row = sci.sci_LineFromPosition(pos);
	int col = pos - sci.sci_PositionFromLine(row);

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CString temp;
	temp.Format(_T("%d"), pos);
	pMain->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURSOR_POS, temp);
	return 0;
}

void CScriptEditorView::OnEditUndo()
{
	sci.sci_Undo();
}

void CScriptEditorView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanUndo());
}

void CScriptEditorView::OnEditRedo()
{
	sci.sci_Redo();
}

void CScriptEditorView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanRedo());
}

void CScriptEditorView::OnEditCut()
{
	sci.sci_Cut();
}

void CScriptEditorView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanCut());
}

void CScriptEditorView::OnEditCopy()
{
	sci.sci_Copy();
}

void CScriptEditorView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanCopy());
}

void CScriptEditorView::OnEditPaste()
{
	sci.sci_Paste();
}

void CScriptEditorView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanPaste());
}

void CScriptEditorView::OnEditClear()
{
	sci.sci_Clear();
}

void CScriptEditorView::OnEditSelectAll()
{
	sci.sci_SelectAll();
}

void CScriptEditorView::OnScriptInsertBreakPoint()
{
	int line = sci.sci_GetCurLine();
	int marker = sci.sci_MarkerGet(line);
	if(marker == 0)
		sci.sci_MarkerAdd(line, 0);
	else
		sci.sci_MarkerDelete(line, 0);
}

LRESULT CScriptEditorView::OnRefreshStack(WPARAM wparam, LPARAM lparam)
{
	CMFCPropertyGridCtrl *pPropList = GetDocument()->m_pPropList;
	CLockWindowUpdate lock(pPropList);
	pPropList->RemoveAll();
	if(lparam == 0)
	{
		return 0;
	}

	asIScriptContext *ctx = (asIScriptContext *)wparam;
	if(!ctx) return FALSE;

	asIScriptFunction *func = ctx->GetFunction();
	if( !func ) return FALSE;
	asIScriptModule *mod = ctx->GetEngine()->GetModule(func->GetModuleName(), asGM_ONLY_IF_EXISTS);
	if( !mod ) return FALSE;

	for( asUINT n = 0; n < ctx->GetCallstackSize(); n++ )
	{
		asIScriptFunction *fun = ctx->GetFunction(n);

		CString strTitle;
		strTitle.Format(_T("栈 %s"), LSUTF82T(fun->GetDeclaration(true)));
		CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(strTitle);

		for (asUINT i=0; i<fun->GetVarCount(); i++)
		{
			if(ctx->IsVarInScope(i)) 
			{	
				CString name = LSUTF82T(fun->GetVarDecl(i));
				CString val = LSUTF82T(VariantToString(ctx->GetAddressOfVar(i), ctx->GetVarTypeId(i), false, ctx->GetEngine()));
				CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T(""),  _T(""), _T(""));
				pProp->AllowEdit(FALSE);
				pProp->SetName(name);
				pProp->SetValue(val);
				pProp->SetDescription(name + " = " + val);
				pGroup->AddSubItem(pProp);
			}
		}
		pPropList->AddProperty(pGroup);
		pGroup->Expand(TRUE);
	}

	CString strTitle = _T("全局属性");
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(strTitle);
	for( asUINT n = 0; n < mod->GetGlobalVarCount(); n++ )
	{
		const char *vname;
		int typeId;
		mod->GetGlobalVar(n, &vname, 0, &typeId);
		CString name = LSUTF82T(mod->GetGlobalVarDeclaration(n));
		CString val = LSUTF82T(VariantToString(mod->GetAddressOfGlobalVar(n), typeId, false, m_pHelper->GetEngine()));
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T(""),  _T(""), _T(""));
		pProp->AllowEdit(FALSE);
		pProp->SetName(name);
		pProp->SetValue(val);
		pProp->SetDescription(name + " = " + val);
		pGroup->AddSubItem(pProp);
	}
	pPropList->AddProperty(pGroup);
	pGroup->Expand(TRUE);


	return TRUE;
}

CStringA CScriptEditorView::VariantToString(void *value, asUINT typeId, int expandMembers, asIScriptEngine *engine)
{
	if( value == 0 )
		return "<null>";

	stringstream s;
	if( typeId == asTYPEID_VOID )
		return "<void>";
	else if( typeId == asTYPEID_BOOL )
		return *(bool*)value ? "true" : "false";
	else if( typeId == asTYPEID_INT8 )
		s << (int)*(signed char*)value;
	else if( typeId == asTYPEID_INT16 )
		s << (int)*(signed short*)value;
	else if( typeId == asTYPEID_INT32 )
		s << *(signed int*)value;
	else if( typeId == asTYPEID_INT64 )
#if defined(_MSC_VER) && _MSC_VER <= 1200
		s << "{...}"; // MSVC6 doesn't like the << operator for 64bit integer
#else
		s << *(asINT64*)value;
#endif
	else if( typeId == asTYPEID_UINT8 )
		s << (unsigned int)*(unsigned char*)value;
	else if( typeId == asTYPEID_UINT16 )
		s << (unsigned int)*(unsigned short*)value;
	else if( typeId == asTYPEID_UINT32 )
		s << *(unsigned int*)value;
	else if( typeId == asTYPEID_UINT64 )
#if defined(_MSC_VER) && _MSC_VER <= 1200
		s << "{...}"; // MSVC6 doesn't like the << operator for 64bit integer
#else
		s << *(asQWORD*)value;
#endif
	else if( typeId == asTYPEID_FLOAT )
		s << *(float*)value;
	else if( typeId == asTYPEID_DOUBLE )
		s << *(double*)value;
	else if( (typeId & asTYPEID_MASK_OBJECT) == 0 )
	{
		// The type is an enum
		s << *(asUINT*)value;

		// Check if the value matches one of the defined enums
		if( engine )
		{
			asITypeInfo *t = engine->GetTypeInfoById(typeId);
			for( int n = t->GetEnumValueCount(); n-- > 0; )
			{
				int enumVal;
				const char *enumName = t->GetEnumValueByIndex(n, &enumVal);
				if( enumVal == *(int*)value )
				{
					s << ", " << enumName;
					break;
				}
			}
		}
	}
	else if( typeId & asTYPEID_SCRIPTOBJECT )
	{
		// Dereference handles, so we can see what it points to
		if( typeId & asTYPEID_OBJHANDLE )
			value = *(void**)value;

		asIScriptObject *obj = (asIScriptObject *)value;

		// Print the address of the object
		s << "{" << obj << "}";

		// Print the members
		if( obj && expandMembers > 0 )
		{
			asITypeInfo *type = obj->GetObjectType();
			for( asUINT n = 0; n < obj->GetPropertyCount(); n++ )
			{
				if( n == 0 )
					s << " ";
				else
					s << ", ";

				s << type->GetPropertyDeclaration(n) << " = " << VariantToString(obj->GetAddressOfProperty(n), obj->GetPropertyTypeId(n), expandMembers - 1, type->GetEngine());
			}
		}
	}
	else
	{
		// Dereference handles, so we can see what it points to
		if( typeId & asTYPEID_OBJHANDLE )
			value = *(void**)value;

		// Print the address for reference types so it will be
		// possible to see when handles point to the same object
		if( engine )
		{
			asITypeInfo *type = engine->GetTypeInfoById(typeId);
			if(strcmp(type->GetName(), "datetime") == 0)
			{
				s << ((datetime *)value)->Format("%Y-%m-%d %H:%M:%S");
			}
			if(strcmp(type->GetName(), "string") == 0)
			{
				CDuiString *sValue = (CDuiString *)value;
				CStringA ss = LST2A(sValue->GetData());
				s << ss;
			}
			else if( type->GetFlags() & asOBJ_REF )
				s << "{" << value << "}";
		}
		else
			s << "{no engine}";
	}
	CStringA strRet = s.str().c_str();
	return strRet;
}

void CScriptEditorView::OnScriptExcute()
{
	AfxGetApp()->SaveAllModified();

	CDlgScriptTest dlg;
	if(dlg.DoModal() != IDOK)
		return;
	
	//清理之前加载的脚本内容
	m_pHelper->DeleteModule();
	m_pHelper->CreateModule(GetDocument()->GetPathName());

	//加载脚本文件
	m_pHelper->AddScriptFile(GetDocument()->GetPathName());

	//编译脚本
	m_pHelper->CompileScript();

	//设置入口函数
	m_pHelper->SetMainFun(_T("int main(datetime t1, datetime t2)"));

	datetime t1, t2;
	t1.SetDateTime(CDlgScriptTest::m_t1.GetYear(), CDlgScriptTest::m_t1.GetMonth(), CDlgScriptTest::m_t1.GetDay(),
		CDlgScriptTest::m_t1.GetHour(), CDlgScriptTest::m_t1.GetMinute(), CDlgScriptTest::m_t1.GetSecond());
	t2.SetDateTime(CDlgScriptTest::m_t2.GetYear(), CDlgScriptTest::m_t2.GetMonth(), CDlgScriptTest::m_t2.GetDay(),
		CDlgScriptTest::m_t2.GetHour(), CDlgScriptTest::m_t2.GetMinute(), CDlgScriptTest::m_t2.GetSecond());

	//传入参数
	m_pHelper->SetArgObject(0, &t1); 
	m_pHelper->SetArgObject(1, &t2); 

	//运行脚本
	if(m_pHelper->Excute())
	{
		//获取返回值
		InsertMsgV(_T("返回值：%d"), m_pHelper->GetReturnWord());
	}
}


void CScriptEditorView::OnUpdateScriptExcute(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pHelper->IsCanDebugStop());
}

void CScriptEditorView::OnScriptRun()
{
	if(m_pHelper->IsRunning()) 
	{
		m_pHelper->DebugRun();
		return;
	}

	AfxGetApp()->SaveAllModified();

	CDlgScriptTest dlg;
	if(dlg.DoModal() != IDOK)
		return;

	//清理之前加载的脚本内容
	m_pHelper->DeleteModule();
	m_pHelper->CreateModule(GetDocument()->GetPathName());

	//加载脚本文件
	m_pHelper->AddScriptFile(GetDocument()->GetPathName());

	//编译脚本
	m_pHelper->CompileScript();

	//设置入口函数
	m_pHelper->SetMainFun(_T("int main(datetime t1, datetime t2)"));

	datetime t1, t2;
	t1.SetDateTime(CDlgScriptTest::m_t1.GetYear(), CDlgScriptTest::m_t1.GetMonth(), CDlgScriptTest::m_t1.GetDay(),
		CDlgScriptTest::m_t1.GetHour(), CDlgScriptTest::m_t1.GetMinute(), CDlgScriptTest::m_t1.GetSecond());
	t2.SetDateTime(CDlgScriptTest::m_t2.GetYear(), CDlgScriptTest::m_t2.GetMonth(), CDlgScriptTest::m_t2.GetDay(),
		CDlgScriptTest::m_t2.GetHour(), CDlgScriptTest::m_t2.GetMinute(), CDlgScriptTest::m_t2.GetSecond());

	//传入参数
	m_pHelper->SetArgObject(0, &t1); 
	m_pHelper->SetArgObject(1, &t2); 

	//进入调试
	m_pHelper->DebugRun();

	//调试脚本是在线程运行的，所以应该在回调函数获取返回值。
}


void CScriptEditorView::OnUpdateScriptRun(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHelper->IsCanDebugRun());
}


void CScriptEditorView::OnScriptAbort()
{
	m_pHelper->DebugStop();
}


void CScriptEditorView::OnUpdateScriptAbort(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHelper->IsCanDebugStop());
}


void CScriptEditorView::OnScriptStepInto()
{
	m_pHelper->DebugStepInto();
}


void CScriptEditorView::OnUpdateScriptStepInto(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHelper->IsCanDebugStepInto());
}


void CScriptEditorView::OnScriptStepOver()
{
	m_pHelper->DebugStepOver();
}


void CScriptEditorView::OnUpdateScriptStepOver(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHelper->IsCanDebugStepOver());
}


void CScriptEditorView::OnScriptStepReturn()
{
	m_pHelper->DebugStepReturn();
}


void CScriptEditorView::OnUpdateScriptStepReturn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHelper->IsCanDebugStepReturn());
}


void CScriptEditorView::OnScriptStepCursor()
{
	m_pHelper->DebugStepCursor();
}


void CScriptEditorView::OnUpdateScriptStepCursor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pHelper->IsCanDebugStepCursor());
}



void CScriptEditorView::OnScriptAddFun1()
{
	CStringA strFun = "bool On_Control_Init(CControlUI &obj)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}


void CScriptEditorView::OnScriptAddFun2()
{
	CStringA strFun = "bool On_Control_Event(CControlUI &obj, TEventUI &ev)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}


void CScriptEditorView::OnScriptAddFun3()
{
	CStringA strFun = "bool On_Control_Notify(CControlUI &obj, TNotifyUI &notify)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}


void CScriptEditorView::OnScriptAddFun4()
{
	CStringA strFun = "bool On_Control_Destroy(CControlUI &obj, TNotifyUI &notify)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}


void CScriptEditorView::OnScriptAddFun5()
{
	CStringA strFun = "bool On_Control_Size(CControlUI &obj)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun6()
{
	CStringA strFun = "bool On_Control_PaintBkColor(CControlUI &obj, HDC hDC)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun7()
{
	CStringA strFun = "bool On_Control_PaintBkImage(CControlUI &obj, HDC hDC)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun8()
{
	CStringA strFun = "bool On_Control_PaintStatusImage(CControlUI &obj, HDC hDC)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun9()
{
	CStringA strFun = "bool On_Control_PaintForeColor(CControlUI &obj, HDC hDC)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun10()
{
	CStringA strFun = "bool On_Control_PaintForeImage(CControlUI &obj, HDC hDC)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun11()
{
	CStringA strFun = "bool On_Control_PaintText(CControlUI &obj, HDC hDC)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun12()
{
	CStringA strFun = "bool On_Control_PaintBorder(CControlUI &obj, HDC hDC)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptAddFun13()
{
	CStringA strFun = "bool On_Control_Paint(CControlUI &obj, HDC &hDC, const RECT& rcPaint, CControlUI@ pStopControl)\r\n{\r\n\t\r\n}";
	sci.sci_InsertText(sci.sci_GetCurrentPos(), strFun);
	sci.sci_SetSel(-1, sci.sci_GetCurrentPos() + strFun.GetLength() - 3);
}

void CScriptEditorView::OnScriptParseToken()
{
	m_lexer.ParseToken(m_strTextCode, m_strTextCode.GetLength());
}
