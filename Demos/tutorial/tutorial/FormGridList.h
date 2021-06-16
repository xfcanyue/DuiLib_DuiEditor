#pragma once
class CFormGridList : public CUIForm, public CUIDataExchange
{
public:
	CFormGridList(void);
	virtual ~CFormGridList(void);

	virtual void InitWindow();
	virtual void OnNotifyClick(TNotifyUI& msg);
	virtual void OnNotifyInitCell(TNotifyUI& msg);
	virtual void OnNotifyDrawItem(TNotifyUI& msg);

	CGridUI *m_pGrid;
};

