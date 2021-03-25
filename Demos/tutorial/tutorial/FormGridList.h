#pragma once
class CFormGridList : public CUIForm, public CUIDataExchange
{
public:
	CFormGridList(void);
	virtual ~CFormGridList(void);

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

	CGridListUI *m_pGrid;

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
	BOOL m_bSelectRect;
};

