#ifndef __UILIST_H__
#define __UILIST_H__

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	typedef int (CALLBACK *PULVCompareFunc)(UINT_PTR, UINT_PTR, UINT_PTR);

	class CListHeaderUI;

#define UILIST_MAX_COLUMNS 32

	struct UILIB_API TListInfoUI
	{
		TListInfoUI();

		bool SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void SetItemFont(int index);
		int  GetItemFont() const;

		void SetItemTextStyle(UINT uStyle);
		UINT GetItemTextStyle() const;

		void SetItemTextPadding(RECT rc);
		RECT GetItemTextPadding() const;

		void SetItemTextColor(DWORD dwTextColor);
		DWORD GetItemTextColor() const;

		void SetItemBkColor(DWORD dwBkColor);
		DWORD GetItemBkColor() const;

		void SetItemBkImage(LPCTSTR pStrImage);
		CDuiString GetItemBkImage() const;

		void SetAlternateBk(bool bAlternateBk);
		bool IsAlternateBk() const;

		void SetSelectedItemTextColor(DWORD dwTextColor);
		DWORD GetSelectedItemTextColor() const;

		void SetSelectedItemBkColor(DWORD dwBkColor);
		DWORD GetSelectedItemBkColor() const;

		void SetSelectedItemImage(LPCTSTR pStrImage); 
		CDuiString GetSelectedItemImage() const;

		void SetHotItemTextColor(DWORD dwTextColor);
		DWORD GetHotItemTextColor() const;

		void SetHotItemBkColor(DWORD dwBkColor);
		DWORD GetHotItemBkColor() const;

		void SetHotItemImage(LPCTSTR pStrImage);
		CDuiString GetHotItemImage() const;

		void SetDisabledItemTextColor(DWORD dwTextColor);
		DWORD GetDisabledItemTextColor() const;

		void SetDisabledItemBkColor(DWORD dwBkColor);
		DWORD GetDisabledItemBkColor() const;

		void SetDisabledItemImage(LPCTSTR pStrImage);
		CDuiString GetDisabledItemImage() const;

		void SetItemLineColor(DWORD dwLineColor);
		DWORD GetItemLineColor() const;

		bool IsItemShowRowLine() const;
		void SetItemShowRowLine(bool bShowLine = false);

		bool IsItemShowColumnLine() const;
		void SetItemShowColumnLine(bool bShowLine = false);

		bool IsItemRSelected();
		void SetItemRSelected(bool bSelected = true);

		void SetMultiExpanding(bool bMultiExpandable); 
		int GetExpandedItem() const;

		bool IsScrollSelect() const;
		void SetScrollSelect(bool bScrollSelect);
		//////////////////////////////////////////////////////////////////////////
		int nColumns;
		RECT rcColumn[UILIST_MAX_COLUMNS];

		int nFont;
		UINT uTextStyle;
		RECT rcTextPadding;

		DWORD dwTextColor;
		DWORD dwBkColor;
		CDuiString sBkImage;
		bool bAlternateBk;

		DWORD dwSelectedTextColor;
		DWORD dwSelectedBkColor;
		CDuiString sSelectedImage;

		DWORD dwHotTextColor;
		DWORD dwHotBkColor;
		CDuiString sHotImage;

		DWORD dwDisabledTextColor;
		DWORD dwDisabledBkColor;
		CDuiString sDisabledImage;

		DWORD dwLineColor;
		bool bShowRowLine;
		bool bShowColumnLine;
		bool bMultiExpandable;
		bool bRSelected;

		bool bScrollSelect;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class IListCallbackUI
	{
	public:
		virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem) = 0;
	};

	class IListOwnerUI
	{
	public:
		virtual UINT GetListType() = 0;
		virtual TListInfoUI* GetListInfo() = 0;
		virtual int GetCurSel() const = 0;
		virtual bool SelectItem(int iIndex, bool bTakeFocus = false) = 0;
		virtual bool SelectMultiItem(int iIndex, bool bTakeFocus = false) = 0;
		virtual bool UnSelectItem(int iIndex, bool bOthers = false) = 0;
		virtual void DoEvent(TEventUI& event) = 0;
	};

	class IListUI : public IListOwnerUI
	{
	public:
		virtual CListHeaderUI* GetHeader() const = 0;
		virtual CContainerUI* GetList() const = 0;
		virtual IListCallbackUI* GetTextCallback() const = 0;
		virtual void SetTextCallback(IListCallbackUI* pCallback) = 0;
		virtual bool ExpandItem(int iIndex, bool bExpand = true) = 0;
		virtual int GetExpandedItem() const = 0;

		virtual void SetMultiSelect(bool bMultiSel) = 0;
		virtual bool IsMultiSelect() const = 0;
		virtual void SelectAllItems() = 0;
		virtual void UnSelectAllItems() = 0;
		virtual int GetSelectItemCount() const = 0;
		virtual int GetNextSelItem(int nItem) const = 0;
	};

	class IListItemUI
	{
	public:
		virtual int GetIndex() const = 0;
		virtual void SetIndex(int iIndex) = 0;
		virtual IListOwnerUI* GetOwner() = 0;
		virtual void SetOwner(CControlUI* pOwner) = 0;
		virtual bool IsSelected() const = 0;
		virtual bool Select(bool bSelect = true) = 0;
		virtual bool SelectMulti(bool bSelect = true) = 0;
		virtual bool IsExpanded() const = 0;
		virtual bool Expand(bool bExpand = true) = 0;
		virtual void DrawItemText(UIRender *pRender, const RECT& rcItem) = 0;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CListBodyUI;
	class CListHeaderUI;
	//class CEditUI;
	class CComboUI;
	class UILIB_API CListUI : public CVerticalLayoutUI, public IListUI
	{
		DECLARE_DUICONTROL(CListUI)

	public:
		CListUI();

		virtual LPCTSTR GetClass() const override;
		virtual UINT GetControlFlags() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual int GetCurSel() const override;
		int GetCurSelActivate() const;
		virtual bool SelectItem(int iIndex, bool bTakeFocus = false) override;
		bool SelectItemActivate(int iIndex);    // 双击选中

		virtual bool SelectMultiItem(int iIndex, bool bTakeFocus = false) override;
		virtual void SetMultiSelect(bool bMultiSel) override;
		virtual bool IsMultiSelect() const override;
		virtual bool UnSelectItem(int iIndex, bool bOthers = false) override;
		virtual void SelectAllItems() override;
		virtual void UnSelectAllItems() override;
		virtual int GetSelectItemCount() const override;
		virtual int GetNextSelItem(int nItem) const override;

		bool IsFixedScrollbar();
		void SetFixedScrollbar(bool bFixed);

		CListHeaderUI* GetHeader() const;  
		CContainerUI* GetList() const;
		UINT GetListType();
		TListInfoUI* GetListInfo();

		virtual CControlUI* GetItemAt(int iIndex) const override;
		virtual int GetItemIndex(CControlUI* pControl) const override;
		virtual bool SetItemIndex(CControlUI* pControl, int iIndex) override;
		virtual int GetCount() const override;
		virtual bool Add(CControlUI* pControl) override;
		virtual bool AddAt(CControlUI* pControl, int iIndex) override;
		virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false) override;
		virtual bool RemoveAt(int iIndex, bool bDoNotDestroy=false) override;
		virtual void RemoveAll() override;

		void EnsureVisible(int iIndex);
		void Scroll(int dx, int dy);

		virtual bool IsDelayedDestroy() const override;
		virtual void SetDelayedDestroy(bool bDelayed) override;
		virtual int GetChildPadding() const override;
		virtual void SetChildPadding(int iPadding) override;

		virtual int GetExpandedItem() const override;
		virtual bool ExpandItem(int iIndex, bool bExpand = true) override;

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true) override;
		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		IListCallbackUI* GetTextCallback() const;
		void SetTextCallback(IListCallbackUI* pCallback);

		virtual SIZE GetScrollPos() const override;
		virtual SIZE GetScrollRange() const override;
		virtual void SetScrollPos(SIZE szPos, bool bMsg = true) override;
		virtual void LineUp() override;
		virtual void LineDown() override;
		virtual void PageUp() override;
		virtual void PageDown() override;
		virtual void HomeUp() override;
		virtual void EndDown() override;
		virtual void LineLeft() override;
		virtual void LineRight() override;
		virtual void PageLeft() override;
		virtual void PageRight() override;
		virtual void HomeLeft() override;
		virtual void EndRight() override;
		virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false) override;
		virtual CScrollBarUI* GetVerticalScrollBar() const override;
		virtual CScrollBarUI* GetHorizontalScrollBar() const override;
		BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);

		virtual BOOL CheckColumEditable(int nColum) { return FALSE; };
		virtual CEditUI* GetEditUI() { return NULL; };
		virtual BOOL CheckColumComboBoxable(int nColum) { return FALSE; };
		virtual CComboUI* GetComboBoxUI() { return NULL; };

	protected:
		int GetMinSelItemIndex();
		int GetMaxSelItemIndex();

	protected:
		bool m_bFixedScrollbar;
		bool m_bScrollSelect;
		int m_iCurSel;
		bool m_bMultiSel;
		CStdPtrArray m_aSelItems;
		int m_iCurSelActivate;  // 双击的列
		int m_iExpandedItem;
		IListCallbackUI* m_pCallback;
		CListBodyUI* m_pList;
		CListHeaderUI* m_pHeader;
		TListInfoUI m_ListInfo;

	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListBodyUI : public CVerticalLayoutUI
	{
	public:
		CListBodyUI(CListUI* pOwner);


		virtual int GetScrollStepSize() const override;
		virtual void SetScrollPos(SIZE szPos, bool bMsg = true) override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void DoEvent(TEventUI& event) override;
		BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);
	protected:
		static int __cdecl ItemComareFunc(void *pvlocale, const void *item1, const void *item2);
		int __cdecl ItemComareFunc(const void *item1, const void *item2);
	protected:
		CListUI* m_pOwner;
		PULVCompareFunc m_pCompareFunc;
		UINT_PTR m_compareData;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListHeaderUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CListHeaderUI)
	public:
		CListHeaderUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		void SetScaleHeader(bool bIsScale);
		bool IsScaleHeader() const;

	private:
		bool m_bIsScaleHeader;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListHeaderItemUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CListHeaderItemUI)

	public:
		CListHeaderItemUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		bool IsHeaderDragEnable() const;
		void SetHeaderDragEnable(bool bDragable);
		DWORD GetSepWidth() const;
		void SetSepWidth(int iWidth);
		
		LPCTSTR GetSepImage() const;
		void SetSepImage(LPCTSTR pStrImage);
		void SetScale(int nScale);
		int GetScale() const;

		virtual void DoEvent(TEventUI& event) override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		RECT GetThumbRect() const;

		virtual void PaintText(UIRender *pRender) override;
		virtual void PaintStatusImage(UIRender *pRender) override;

	protected:
		bool m_bDragable;
		int m_iSepWidth;
		
		CDuiString m_sSepImage;
		CDuiString m_sSepImageModify;
		int m_nScale;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListElementUI : public CControlUI, public IListItemUI
	{
	public:
		CListElementUI();

		virtual LPCTSTR GetClass() const override;
		virtual UINT GetControlFlags() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;


		virtual int GetIndex() const override;
		virtual void SetIndex(int iIndex) override;

		virtual IListOwnerUI* GetOwner() override;
		virtual void SetOwner(CControlUI* pOwner) override;
		virtual void SetVisible(bool bVisible = true) override;

		virtual bool IsSelected() const override;
		virtual bool Select(bool bSelect = true) override;
		virtual bool SelectMulti(bool bSelect = true) override;
		virtual bool IsExpanded() const override;
		virtual bool Expand(bool bExpand = true) override;

		virtual void Invalidate() override; // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
		virtual bool Activate() override;

		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		void DrawItemBk(UIRender *pRender, const RECT& rcItem);

	protected:
		int m_iIndex;
		bool m_bSelected;
		IListOwnerUI* m_pOwner;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListLabelElementUI : public CListElementUI
	{
		DECLARE_DUICONTROL(CListLabelElementUI)
	public:
		CListLabelElementUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void DoEvent(TEventUI& event) override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;

		virtual void DrawItemText(UIRender *pRender, const RECT& rcItem) override;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListTextElementUI : public CListLabelElementUI
	{
		DECLARE_DUICONTROL(CListTextElementUI)
	public:
		CListTextElementUI();
		~CListTextElementUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		CDuiString GetText(int iIndex) const;
		void SetText(int iIndex, LPCTSTR pstrText);
		
		void SetText(int iIndex, int nDigit);	//modify by liqs99

		virtual void SetOwner(CControlUI* pOwner) override;
		CDuiString* GetLinkContent(int iIndex);

		virtual void DoEvent(TEventUI& event) override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;

		virtual void DrawItemText(UIRender *pRender, const RECT& rcItem) override;

	protected:
		enum { MAX_LINK = 8 };
		int m_nLinks;
		RECT m_rcLinks[MAX_LINK];
		CDuiString m_sLinks[MAX_LINK];
		int m_nHoverLink;
		IListUI* m_pOwner;
		CStdPtrArray m_aTexts;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListContainerElementUI : public CHorizontalLayoutUI, public IListItemUI
	{
		DECLARE_DUICONTROL(CListContainerElementUI)
	public:
		CListContainerElementUI();

		virtual LPCTSTR GetClass() const override;
		virtual UINT GetControlFlags() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual int GetIndex() const override;
		virtual void SetIndex(int iIndex) override;

		virtual IListOwnerUI* GetOwner() override;
		virtual void SetOwner(CControlUI* pOwner) override;
		virtual void SetVisible(bool bVisible = true) override;

		virtual bool IsSelected() const override;
		virtual bool Select(bool bSelect = true) override;
		virtual bool SelectMulti(bool bSelect = true) override;
		virtual bool IsExpanded() const override;
		virtual bool Expand(bool bExpand = true) override;

		virtual void Invalidate() override; // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
		virtual bool Activate() override;

		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;

		virtual void DrawItemText(UIRender *pRender, const RECT& rcItem) override;    
		void DrawItemBk(UIRender *pRender, const RECT& rcItem);

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;

	protected:
		int m_iIndex;
		bool m_bSelected;
		IListOwnerUI* m_pOwner;
	};

} // namespace DuiLib

#endif // __UILIST_H__
