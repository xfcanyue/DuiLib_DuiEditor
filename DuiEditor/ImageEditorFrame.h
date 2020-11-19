#pragma once
#include "DockPropertyWnd.h"
#include "ImageEditorPaneFile.h"
#include "ImageEditorView.h"
#include "ImageEditorPane.h"
#include "ImageEditorPaneImage.h"
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
	CImageEditorPaneFile	m_wndList;
	CDockPropertyWnd		m_wndProperty;
	CMFCToolBar				m_wndToolBar;
	CImageEditorPaneAdjust	m_wndPaneAdjust;
	CImageEditorPaneImage	m_wndImage;

	CUIPropertyGridCtrl		*m_pPropList;

	BOOL m_bPreview;

public:
	void OnSelectedFile(LPCTSTR lpstrPathName);
	void ParentPreview();

	BOOL FindInternalDivider (CDockablePane* pBar, 
		CPaneContainer*& pContainer, 
		CPaneDivider*& pDivider, BOOL& bLeftSlider, 
		BOOL& bAloneInContainer);
	void SetPaneHeight(int nHeight);
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
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


