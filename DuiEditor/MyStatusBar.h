#pragma once


// CMyStatusBar

class CMyStatusBar : public CMFCStatusBar
{
	DECLARE_DYNAMIC(CMyStatusBar)

public:
	CMyStatusBar();
	virtual ~CMyStatusBar();

	int GetIndexByID(UINT nID);
	BOOL SetPaneTextByID(UINT nID, LPCTSTR lpszNewText, BOOL bCalcPaneWidth=TRUE);
protected:
	virtual void OnDrawPane(CDC* pDC, CMFCStatusBarPaneInfo* pPane);
protected:
	DECLARE_MESSAGE_MAP()
};


