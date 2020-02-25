// OutputWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockOutputWnd.h"

//////////////////////////////////////////////////////////////////////////

// COutputWnd

IMPLEMENT_DYNAMIC(CDockOutputWnd, CDockablePane)

CDockOutputWnd::CDockOutputWnd()
{
	
}

CDockOutputWnd::~CDockOutputWnd()
{
}


BEGIN_MESSAGE_MAP(CDockOutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_INSERT_MSG, OnInsertMsg)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_LISTMSG_CLEAR, &CDockOutputWnd::OnListmsgClear)
	ON_UPDATE_COMMAND_UI(ID_LISTMSG_CLEAR, &CDockOutputWnd::OnUpdateListmsgClear)
	ON_COMMAND(ID_LISTMSG_SAVE_AS, &CDockOutputWnd::OnListmsgSaveAs)
	ON_UPDATE_COMMAND_UI(ID_LISTMSG_SAVE_AS, &CDockOutputWnd::OnUpdateListmsgSaveAs)
END_MESSAGE_MAP()



// COutputWnd 消息处理程序

int CDockOutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN;
	if (!m_ctrlRichEdit.Create(dwStyle, rectDummy, this, ID_OUTPUT_CTRL))
	{
		TRACE0("Failed to create ToolBox\n");
		return -1;      // fail to create
	}

	g_hWndMsg = this->m_hWnd;
	m_ctrlRichEdit.SetFont(&afxGlobalData.fontRegular);
	return 0;
}


void CDockOutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_ctrlRichEdit.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}


LRESULT CDockOutputWnd::OnInsertMsg(WPARAM wParam, LPARAM lParam)
{
	//CString str = (LPCTSTR)wParam;

	CString str;
	SYSTEMTIME st;
	GetLocalTime(&st);
	str.Format(_T("%02d:%02d:%02d:%03d: %s"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, (LPCTSTR)wParam);
	
	m_ctrlRichEdit.SetSel(-1, -1);
	m_ctrlRichEdit.ReplaceSel(str, FALSE);
	m_ctrlRichEdit.ReplaceSel(_T("\r\n"), FALSE);

	m_ctrlRichEdit.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return 0;
}

void CDockOutputWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_LIST_MSG);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, TRUE);
}


void CDockOutputWnd::OnListmsgClear()
{
	m_ctrlRichEdit.SetSel(0, -1);
	m_ctrlRichEdit.Clear();
}


void CDockOutputWnd::OnUpdateListmsgClear(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDockOutputWnd::OnListmsgSaveAs()
{
	CString FileName;
	static TCHAR BASED_CODE szFilter[] = _T("TXT 文本文件 (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("txt"), FileName,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	if(fileDlg.DoModal() == IDOK)
	{
		CStdioFile sf;
		CFileException mExcept;
		sf.Open(fileDlg.GetPathName(), CFile::modeCreate|CFile::modeReadWrite, &mExcept);

		CString strContent;
		m_ctrlRichEdit.GetWindowText(strContent);
		sf.Write((LPVOID)(LPCTSTR)strContent, strContent.GetLength());

// 		int line = m_ctrlRichEdit.GetLineCount();
// 		CString temp;
// 		for(int i=0; i<line; i++)
// 		{
// 			m_ctrlRichEdit.GetLine(i, temp);
// 			sf.WriteString(temp);
// 			sf.WriteString(_T("\r\n"));
// 		}


		sf.Close();
	}
}


void CDockOutputWnd::OnUpdateListmsgSaveAs(CCmdUI *pCmdUI)
{
	
}
