#pragma once

//////////////////////////////////////////////////////////////////////////
// CToolElement

class CDockToolBoxElement : public CObject
{
	DECLARE_DYNAMIC(CDockToolBoxElement)

	friend class CDockToolBoxCtrl;

public:
	CDockToolBoxElement(const CString& strTabName);
	CDockToolBoxElement(const CString& strName,int nClass, UINT nIDIcon);
	virtual ~CDockToolBoxElement();

public:
	BOOL AddSubTool(CDockToolBoxElement* pTool);
	BOOL IsToolTab() const { return m_bTab; }
	void Expand(BOOL bExpand = TRUE);
	BOOL IsExpanded() const { return m_bExpanded; }
	BOOL IsEnabled() const { return m_bEnabled; }
	BOOL IsVisible() const { return m_bIsVisible; }
	void SetClass(int nClass) { m_nClass=nClass; }
	CDockToolBoxElement* GetTool(int nClass) const;
	int GetClass() const { return m_nClass; }
	CRect GetRect() const { return m_Rect; }
	virtual BOOL IsSelected() const;
	virtual BOOL IsHovered() const;
	BOOL IsParentExpanded() const;
	int GetExpandedSubTools(BOOL bIncludeHidden = TRUE) const;
	BOOL RemoveSubTool(CDockToolBoxElement*& pTool, BOOL bDelete = TRUE);
	CDockToolBoxElement* HitTest(CPoint point);

	void Redraw();
	virtual void OnDrawExpandBox(CDC* pDC, CRect rectExpand);
	virtual void OnDrawIcon(CDC* pDC, CRect rect);
	virtual void OnDrawName(CDC* pDC, CRect rect);

	void SetItemImage(int nImage) { m_nImage = nImage; }
	int  GetItemImage() { return m_nImage; }
	void SetTag(UINT_PTR pTag) { m_pTag = pTag; }
	UINT_PTR GetTag() { return m_pTag; }
protected:
	void Init();
	BOOL IsSubTool(CDockToolBoxElement* pTool) const;
	void SetOwnerList(CDockToolBoxCtrl* pWndList);
	void Reposition(int& y);

protected:
	CString m_strName;//Tool Name
	int m_nClass;//Tool Class
	BOOL    m_bTab;//Is ToolTab?
	HICON   m_hIcon;//Tool Icon
	CRect m_Rect;//Tool rectangle (in the ToolBox.list coordinates)
	BOOL m_bEnabled;         // Is Tool enabled?
	BOOL m_bIsVisible;       // Is property visible
	BOOL m_bExpanded;        // Is ToolTab expanded (for ToolTab only)
	BOOL m_bNameIsTruncated;

	CDockToolBoxCtrl*     m_pWndList;    // Pointer to the ToolBoxList window
	CDockToolBoxElement*     m_pParent;     // Parent Tool (NULL for ToolTab)
	CList<CDockToolBoxElement*, CDockToolBoxElement*> m_lstSubTools; // Sub-tools list

	int m_nImage;
	UINT_PTR m_pTag;
};

////////////////////////////////////////////////////////////
// CToolBoxCtrl

class CDockToolBoxCtrl : public CWnd
{
	DECLARE_DYNAMIC(CDockToolBoxCtrl)

	friend class CDockToolBoxElement;

public:
	CDockToolBoxCtrl();
	virtual ~CDockToolBoxCtrl();

public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

public:
	int AddToolTab(CDockToolBoxElement* pToolTab, BOOL bRedraw = TRUE, BOOL bAdjustLayout = TRUE);
	BOOL DeleteToolTab(CDockToolBoxElement*& pTool, BOOL bRedraw = TRUE, BOOL bAdjustLayout = TRUE);
	void RemoveAll();
	int GetToolTabCount() const { return (int) m_lstToolTabs.GetCount(); }

	void SetCurSel(CDockToolBoxElement* pTool, BOOL bRedraw = TRUE);
	void SetCurSel(int nClass,BOOL bRedraw=TRUE);
	CDockToolBoxElement* GetCurSel() const { return m_pSel; }

	void ExpandAll(BOOL bExpand = TRUE);

	virtual void AdjustLayout();
	CDockToolBoxElement* HitTest(CPoint pt) const;

	void SetImageList(CImageList *pImageList) { m_pImageList = pImageList; }

	//°´Ãû³ÆÅÅÐò
	void SortByItemName();
public:
	virtual int OnDrawTool(CDC* pDC, CDockToolBoxElement* pTool) const;
	void SetCustomColors(COLORREF clrBackground, COLORREF clrSelected, COLORREF clrHover, COLORREF m_clrSelectedBorder,
		COLORREF clrToolTab);
	void GetCustomColors(COLORREF& clrBackground, COLORREF& clrSelected, COLORREF& clrHover, COLORREF& m_clrSelectedBorder,
		COLORREF& clrToolTab);

protected:
	virtual void OnFillBackground(CDC* pDC, CRect rectClient);
	virtual void OnDrawList(CDC* pDC);
	HFONT SetCurrFont(CDC* pDC);
	void CreateToolTabFont();

	void ReposTools();
	void SetScrollSizes();
	int GetTotalItems(BOOL bIncludeHidden = TRUE) const;

	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
protected:
	CToolTipCtrl        m_ToolTip;       // Tooltip control
	CScrollBar          m_wndScrollVert; // Vertical scroll bar

protected:
	CList<CDockToolBoxElement*, CDockToolBoxElement*> m_lstToolTabs;         // List of ToolTab
	CDockToolBoxElement*                       m_pSel;                // Current selection
	CDockToolBoxElement*                       m_pHover;              //Hover over

	CRect m_rectList; // ToolBox area
	int m_nTabHeight; //ToolTab height of the single row
	int m_nToolHeight;          // Tool height of the single row
	int m_nVertScrollOffset;   // In rows
	int m_nVertScrollTotal;
	int m_nVertScrollPage;

	HFONT m_hFont;    // ToolBox list regular font
	CFont m_fontToolTab; // ToolTab font
	CBrush m_brBackground;

	COLORREF m_clrBackground;
	COLORREF m_clrSelected;
	COLORREF m_clrHover;
	COLORREF m_clrSelectedBorder;
	COLORREF m_clrToolTab;
	
	CImageList *m_pImageList;
	CImageList *m_pDragImage;
	BOOL m_bDraging;
	BOOL m_bValidDrop;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL OnDraging(CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg void OnDestroy();
};
