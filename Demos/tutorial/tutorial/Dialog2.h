#pragma once
class CDialog2 : public CUIDialog, public CUIDataExchange
{
public:
	CDialog2(void);
	virtual ~CDialog2(void);

	virtual LPCTSTR GetWindowClassName() const  { return _T("Dialog2"); }
	virtual CDuiString GetSkinFile() 			{ return _T("Dialog2.xml"); }

	virtual void InitWindow();
	virtual void OnNotifyClick(TNotifyUI& msg);
};

