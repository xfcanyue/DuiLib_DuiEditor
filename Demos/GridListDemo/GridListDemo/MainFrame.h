#pragma once

class CMainFrame : public CUIFrameWnd, public CUIDataExchange
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);

	virtual LPCTSTR GetWindowClassName() const  { return _T("MainFrame"); }
	virtual CDuiString GetSkinFile() 			{ return _T("GridListDemo.xml"); }
	virtual CDuiString GetSkinFolder()			{ return _T(""); }
	virtual void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

	virtual void InitWindow();
	void GetGridPara();
	virtual void OnNotifyClick(TNotifyUI& msg);
	virtual void OnNotifyRClick(TNotifyUI& msg);
	virtual void OnNotifyDrawItem(TNotifyUI& msg);
	virtual void OnNotifySortItem(TNotifyUI& msg);
	virtual void OnNotifyItemSelect(TNotifyUI& msg);
	virtual void OnNotifySelectChanged(TNotifyUI& msg);
	virtual void OnNotifyPreDropDown(TNotifyUI& msg);
	virtual void OnNotifyDropDown(TNotifyUI& msg);
	virtual void OnNotifyStartEdit(TNotifyUI& msg);
	virtual void OnNotifyEndEdit(TNotifyUI& msg);
	virtual void OnNotifyTextChanged(TNotifyUI& msg);
	virtual void OnNotifyInitCell(TNotifyUI& msg);
	
	CGridUI *m_pGrid;

	BOOL m_bVirtualGrid;
	int m_nRows;
	int m_nCols;
	int m_nFixedRows;
	int m_nFixedCols;
	BOOL m_bListMode;
	BOOL m_bSingleRowSelection;
	BOOL m_bViewListNumber;
	BOOL m_bViewRowLine;
	BOOL m_bViewColLine;
};

