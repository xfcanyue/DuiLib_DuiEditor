#pragma once
#include <list>

namespace DuiLib {

typedef MenuCmd MENUCOMMAND;

class CUIForm;
class UILIB_API CUIFrameWndBase 
	: public IDialogBuilderCallback
	, public IQueryControlText
	, public CUIFrmBase
{
public:
	CUIFrameWndBase(void);
	virtual ~CUIFrameWndBase(void);

	virtual CDuiString GetSkinFile() = 0;
	virtual CDuiString GetSkinType() { return _T(""); }
	virtual LPCTSTR GetManagerName() { return NULL; }

	virtual CControlUI* CreateControl(LPCTSTR pstrClass) override { return NULL; }

	virtual CDuiString QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType) { return _T(""); }

	CControlUI *GetRoot(); 
	void AttachVirtualForm(CUIFrmBase *pForm);
	void DetachVirtualForm(CUIFrmBase *pForm);

public:
	virtual void InitWindow(){}

protected:
	virtual void __InitWindow();
	virtual void UIAction(TUIAction *act, bool bAsync);
	virtual BOOL IsInStaticControl(CControlUI *pControl);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

public:
	CUIApplication *m_pApplication;

protected:
	std::vector<CDuiString> m_vctStaticName;
	CStdPtrArray	m_listForm;
};

} //namespace DuiLib {