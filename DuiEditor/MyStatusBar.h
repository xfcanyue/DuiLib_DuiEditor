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
	BOOL SetPaneWidthByID(UINT nID, int cx);

	void SetXmlUpdateStatus(BOOL bUpdateSuccess);
private:
	HICON m_hIconXmlOk;
	HICON m_hIconXmlErr;
	BOOL m_bXmlUpdateSuccess;
protected:
	virtual void OnDrawPane(CDC* pDC, CMFCStatusBarPaneInfo* pPane);
	void OnDrawXmlUpdateStatus(CDC* pDC, CMFCStatusBarPaneInfo* pPane);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


