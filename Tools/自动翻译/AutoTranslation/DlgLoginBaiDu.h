#pragma once
class CDlgLoginBaiDu : public CUIDialog, public CUIDataExchange
{
public:
	CDlgLoginBaiDu();
	virtual ~CDlgLoginBaiDu();
	virtual LPCTSTR GetWindowClassName() const { return _T("DlgLoginBaiDu"); }
	virtual CDuiString GetSkinFile() { return _T("DlgLoginBaiDu.xml"); }

	virtual void InitWindow();

	virtual void OnClickOK();

	CDuiString m_sAppid;
	CDuiString m_sKey;
};

