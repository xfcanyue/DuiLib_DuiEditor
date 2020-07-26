
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "DockFileViewWnd.h"
#include "DockControlTreeWnd.h"
#include "DockPropertyWnd.h"
#include "DockOutputWnd.h"
#include "DockXmlWnd.h"
#include "ToolBoxWnd.h"

#include "DuiEditorViewDesign.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 特性
public:
	UINT_PTR m_uTimerReOpenFile;
	CString m_strReOpenFile;
	CDuiEditorDoc *m_pDocReOpen;
// 操作
public:
	CDuiEditorViewDesign *GetActiveUIView();

	void ShowAllPane();
	void HideAllPane();

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();

	CMFCToolBar			 m_wndToolBar2;
	CDockPropertyWnd	 m_wndProperty;
	CDockControlTreeWnd	 m_wndControl;
	CDockFileViewWnd	 m_wndFileView;
	CDockXmlWnd			 m_wndDockXml;
protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CDockOutputWnd	  m_wndOutput;
	CDockToolBoxWnd	  m_wndToolBox;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg LRESULT OnGetTabTooltip(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFileCloseAll();
	afx_msg void OnUpdateFileCloseAll(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI *pCmdUI);
	afx_msg void OnEditOptions();
	afx_msg void OnClose();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg LRESULT OnReOpenFile(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewControlTree();
};


