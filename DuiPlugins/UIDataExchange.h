#pragma once
class UILIB_PLUGIN_API CUIDataExchange
{
public:
	enum _ddx_type
	{
		_ddx_null = 0,
		_ddx_int,
		_ddx_duistring,
#ifdef _AFX
		_ddx_cstring,
		_ddx_coledatetime,
		_ddx_colecurrency,
#endif
	};
	struct _ddx_data 
	{
		CControlUI *pControl;
		_ddx_type type_;
		PVOID pValue;
	};
public:
	CUIDataExchange(void);
	virtual ~CUIDataExchange(void);

	void ddxSetManager(CPaintManagerUI *pManager);

	bool UpdateDataUI(bool bSaveAndValidate);

	bool ddxText(CControlUI *pControl, PVOID pValue, _ddx_type type);

	bool ddxText(CControlUI *pControl, CDuiString &va);
	bool ddxText(LPCTSTR pControlName, CDuiString &va);
	bool ddxText(CControlUI *pControl, int &va);
	bool ddxText(LPCTSTR pControlName, int &va);
#ifdef _AFX
	bool ddxText(CControlUI *pControl, CString &va);
	bool ddxText(LPCTSTR pControlName, CString &va);
	bool ddxText(CControlUI *pControl, COleDateTime &va);
	bool ddxText(LPCTSTR pControlName, COleDateTime &va);
	bool ddxText(CControlUI *pControl, COleCurrency &va);
	bool ddxText(LPCTSTR pControlName, COleCurrency &va);
#endif


private:
	CPaintManagerUI *m_pManager;
	CStdPtrArray m_arrData;
};

