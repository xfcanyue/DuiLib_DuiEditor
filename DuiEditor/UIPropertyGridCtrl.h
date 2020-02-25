#pragma once


class CUIPropertyGridStringProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CUIPropertyGridStringProperty)

public:
	CUIPropertyGridStringProperty(const CString& strName, const CString &strValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);

public:
	virtual void OnClickButton(CPoint point);
};

class CUIPropertyGridColorProperty : public CMFCPropertyGridColorProperty
{
public:
	CUIPropertyGridColorProperty(const CString& strName,const COLORREF& color,CPalette* pPalette=NULL,LPCTSTR lpszDescr=NULL,DWORD_PTR dwData=0);

public:
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	virtual BOOL OnUpdateValue();
	void SetUIColor(LPCTSTR szColor);
	CString GetUIColor();
};

class CUIPropertyGridImageProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CUIPropertyGridImageProperty)

public:
	CUIPropertyGridImageProperty(const CString& strName, const CString &strValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);

public:
	virtual void OnClickButton(CPoint point);
};

class CUIPropertyGridAttributeListProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CUIPropertyGridAttributeListProperty)

public:
	CUIPropertyGridAttributeListProperty(const CString& strName, const CString &strValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);

	CString m_strClassName;
public:
	virtual void OnClickButton(CPoint point);
};

class CUIPropertyGridFontProperty : public CMFCPropertyGridFontProperty
{
	DECLARE_DYNAMIC(CUIPropertyGridFontProperty)

public:
	CUIPropertyGridFontProperty(const CString& strName, LOGFONT& lf, DWORD dwFontDialogFlags = CF_EFFECTS | CF_SCREENFONTS, 
		LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0, COLORREF color = (COLORREF)-1);
public:
	virtual void OnClickButton(CPoint point);
	virtual CString FormatProperty();

	void SetFontName(LPCTSTR lfFaceName);
	LPCTSTR GetFontName();

	void SetFontSize(int size);
	int GetFontSize();

	void SetBold(BOOL bTrue);
	BOOL GetBold();

	void SetUnderline(BOOL bTrue);
	BOOL GetUnderline();

	void SetItalic(BOOL bTrue);
	BOOL GetItalic();

	BOOL m_bOpenDialog;
};

// CUIPropertyGridCtrl
class CUIPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CUIPropertyGridCtrl)

public:
	CUIPropertyGridCtrl();
	virtual ~CUIPropertyGridCtrl();

	CString m_strFilter;
	void InitProp(xml_node TreeNode);

	void InsertDuiLibProperty(xml_node TreeNode, xml_node node, CMFCPropertyGridProperty* pGroupParent);
	void OnOpenFontDialog(CUIPropertyGridFontProperty *pfntProp);

	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	void OnPropertyFontChangedFromFontDialog(CUIPropertyGridFontProperty* pProp) const;

	BOOL IsRepeatName(xml_node nodeDoc, LPCTSTR lpszName) const;

	xml_node GetXmlNode() const { return m_TreeNode; }
	xml_node m_TreeNode;

	CDuiEditorViewDesign *GetView() const;
	BOOL m_bMainFrame; //是否主框架显示的GRID
protected:
	void _initProp(xml_node nodeProperty);
	BOOL _isRepeatProperty(LPCTSTR propName);
private:
	CMFCPropertyGridProperty *m_pPropFocused;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnGridpropertyCopyValue();
	afx_msg void OnUpdateGridpropertyCopyValue(CCmdUI *pCmdUI);
	afx_msg void OnGridpropertyCopyValueEx();
	afx_msg void OnUpdateGridpropertyCopyValueEx(CCmdUI *pCmdUI);
	afx_msg void OnGridpropertySetDefaultValue();
	afx_msg void OnUpdateGridpropertySetDefaultValue(CCmdUI *pCmdUI);
};


