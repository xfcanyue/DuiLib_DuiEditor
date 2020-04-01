// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
//#include "resource.h"
#include "logomdi.h"
#include "TwainCpp.h" //<<<TWAIN>>>
#include "BCMenu.h"
#include "DlgHisto.h"
#include "DlgFloodFill.h"

class CMainFrame : public CMDIFrameWnd, public CTwain
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	void SetImage(HANDLE hBitmap,TW_IMAGEINFO& info); //<<<TWAIN>>>
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	inline CStatusBar&	  GetStatusBar() { return m_wndStatusBar; }
//	inline CProgressCtrl& GetProgressBar() { return m_wndProgressBar; }
	HMENU NewMenu();
	HMENU NewDefaultMenu();
	BCMenu m_menu,m_default;
	bool	m_fullscreen;
	DlgHisto m_HistoBar;
	DlgFloodFill* m_pDlgFlood;

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndCmdBar;
	CLogoMdi m_LogoMdiClientWnd;
//	CProgressCtrl m_wndProgressBar;
 
// Generated message map functions
protected:
	protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnFileSelectsource();
	afx_msg void OnFileAcquire();
	afx_msg void OnClose();
	afx_msg void OnFileCapture();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnViewMenustyleNormal();
	afx_msg void OnViewMenustyleXp();
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
