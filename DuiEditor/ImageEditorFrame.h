#pragma once
#include "DockPropertyWnd.h"
#include "ImageEditorTreeWnd.h"
#include "ImageEditorView.h"
//////////////////////////////////////////////////////////////////////////
// CImageEditorFrame 框架

class CImageEditorFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CImageEditorFrame)
protected:
	CImageEditorFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CImageEditorFrame();

public:
	CImageEditorView		m_wndView;
	CImageEditorListWnd		m_wndList;
	CDockPropertyWnd		m_wndProperty;
	CMFCToolBar				m_wndToolBar;

	BOOL m_bPreview;

	void SetViewImage();
public:
	void OnSelectedFile(LPCTSTR lpstrPathName);
	void OnSetSourcePos(CRect &rc);
protected:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM w, LPARAM l);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnPropertyChanged (WPARAM,LPARAM lParam);
	afx_msg void OnTbExit();
	afx_msg void OnUpdateTbExit(CCmdUI *pCmdUI);
	afx_msg void OnTbSave();
	afx_msg void OnUpdateTbSave(CCmdUI *pCmdUI);
	afx_msg void OnTbUiPreview();
	afx_msg void OnUpdateTbUiPreview(CCmdUI *pCmdUI);
};


