#pragma once

#define UI_BINDCONTROL(classname, pControl, controlname)  { pControl = static_cast<classname *>(FindControl(controlname)); ASSERT(pControl); }
#define UI_COMMAND(controlname, fun) if(IsControl(msg, controlname)) { fun(); return; }

class UILIB_PLUGIN_API CUIDataExchange
{
public:
	//绑定的数据类型
	enum _ddx_value_type
	{
		_value_null = 0,
		_value_bool,
		_value_BOOL,
		_value_int,
		_value_duistring,
#ifdef _AFX
		_value_cstring,
		_value_coledatetime,
		_value_colecurrency,
#endif
	};

	//绑定的控件类型
	enum _ddx_control_type
	{
		//无特殊标识的，拥有有意义的SetText()和GetText()的控件
		_control_text = 0,  

		//Option类控件
		_control_checkbox, 

		//combo控件
		_control_combo,	
	};

	struct _ddx_data 
	{
		CControlUI *pControl;
		_ddx_control_type controlType;
		_ddx_value_type valueType;
		PVOID pValue;
	};
public:
	CUIDataExchange(void);
	virtual ~CUIDataExchange(void);

	void ddxSetManager(CPaintManagerUI *pManager);
	bool UpdateDataUI(bool bSaveAndValidate); //true: 界面>>变量; false: 变量>>界面

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

	bool ddxCheckBox(CControlUI *pControl, bool &va);
	bool ddxCheckBox(LPCTSTR pControlName, bool &va);
	bool ddxCheckBox(CControlUI *pControl, BOOL &va);
	bool ddxCheckBox(LPCTSTR pControlName, BOOL &va);

	bool ddxCombo(CControlUI *pControl, int &va); //cursel
	bool ddxCombo(LPCTSTR pControlName, int &va); //cursel
protected:
	bool ddxText(CControlUI *pControl, PVOID pValue, _ddx_value_type type);
	bool _UpdateText(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateCheckBox(_ddx_data *pData, bool bSaveAndValidate);
	bool _UpdateCombo(_ddx_data *pData, bool bSaveAndValidate);
private:
	CPaintManagerUI *m_pManager;
	CStdPtrArray m_arrData;
};

