#ifndef __UICONTAINER_H__
#define __UICONTAINER_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class IContainerUI
	{
	public:
		virtual CControlUI* GetItemAt(int iIndex) const = 0;
		virtual int GetItemIndex(CControlUI* pControl) const  = 0;
		virtual bool SetItemIndex(CControlUI* pControl, int iIndex)  = 0;
		virtual int GetCount() const = 0;
		virtual bool Add(CControlUI* pControl) = 0;
		virtual bool AddAt(CControlUI* pControl, int iIndex)  = 0;
		virtual bool AddAt(CControlUI* pControl,CControlUI* _IndexControl) = 0;
		virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false) = 0;
		virtual bool RemoveAt(int iIndex, bool bDoNotDestroy=false)  = 0;
		virtual void RemoveAll() = 0;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//
	class CScrollBarUI;

	class UILIB_API CContainerUI : public CControlUI, public IContainerUI
	{
		DECLARE_DUICONTROL(CContainerUI)

	public:
		CContainerUI();
		virtual ~CContainerUI();

	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual CControlUI* GetItemAt(int iIndex) const override;
		virtual int GetItemIndex(CControlUI* pControl) const override;
		virtual bool SetItemIndex(CControlUI* pControl, int iIndex) override;
		virtual int GetCount() const override;
		virtual bool Add(CControlUI* pControl) override;
		virtual bool AddAt(CControlUI* pControl, int iIndex) override;
		virtual bool AddAt(CControlUI* pControl,CControlUI* _IndexControl) override;
		virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false) override;
		virtual bool RemoveAt(int iIndex, bool bDoNotDestroy=false) override;
		virtual void RemoveAll() override;

		virtual void DoEvent(TEventUI& event) override;
		virtual void SetVisible(bool bVisible = true) override;
		virtual void SetInternVisible(bool bVisible = true) override;
		virtual void SetEnabled(bool bEnabled) override;
		virtual void SetMouseEnabled(bool bEnable = true) override;

		virtual RECT GetInset() const;
		virtual void SetInset(RECT rcInset); // 设置内边距，相当于设置客户区
		virtual int GetChildPadding() const;
		virtual void SetChildPadding(int iPadding);
		virtual UINT GetChildAlign() const;
		virtual void SetChildAlign(UINT iAlign);
		virtual UINT GetChildVAlign() const;
		virtual void SetChildVAlign(UINT iVAlign);
		virtual bool IsAutoDestroy() const;
		virtual void SetAutoDestroy(bool bAuto);
		virtual bool IsDelayedDestroy() const;
		virtual void SetDelayedDestroy(bool bDelayed);
		virtual bool IsMouseChildEnabled() const;
		virtual void SetMouseChildEnabled(bool bEnable = true);

		virtual int FindSelectable(int iIndex, bool bForward = true) const;

		RECT GetClientPos() const;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual bool CalcPos(CControlUI *pChildControl, CDuiRect &rcChild) override; //子控件调用, 询问父控件，你将会给我分配多大的rect。
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true) override;
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true) override;
		CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		bool SetSubControlText(LPCTSTR pstrSubControlName,LPCTSTR pstrText);
		bool SetSubControlFixedHeight(LPCTSTR pstrSubControlName,int cy);
		bool SetSubControlFixedWdith(LPCTSTR pstrSubControlName,int cx);
		bool SetSubControlUserData(LPCTSTR pstrSubControlName,LPCTSTR pstrText);

		CDuiString GetSubControlText(LPCTSTR pstrSubControlName);
		int GetSubControlFixedHeight(LPCTSTR pstrSubControlName);
		int GetSubControlFixedWdith(LPCTSTR pstrSubControlName);
		const CDuiString GetSubControlUserData(LPCTSTR pstrSubControlName);
		CControlUI* FindSubControl(LPCTSTR pstrSubControlName);

		virtual SIZE GetScrollPos() const;
		virtual SIZE GetScrollRange() const;
		virtual void SetScrollPos(SIZE szPos, bool bMsg = true);
		virtual void SetScrollStepSize(int nSize);
		virtual int GetScrollStepSize() const;
		virtual void LineUp();
		virtual void LineDown();
		virtual void PageUp();
		virtual void PageDown();
		virtual void HomeUp();
		virtual void EndDown();
		virtual void LineLeft();
		virtual void LineRight();
		virtual void PageLeft();
		virtual void PageRight();
		virtual void HomeLeft();
		virtual void EndRight();
		virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
		virtual CScrollBarUI* GetVerticalScrollBar() const;
		virtual CScrollBarUI* GetHorizontalScrollBar() const;

	protected:
		virtual void SetFloatPos(int iIndex);
		virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);
	protected:
		void SetPosHorizontalLayout(RECT rc, bool bNeedInvalidate = true);
		void SetPosVerticalLayout(RECT rc, bool bNeedInvalidate = true);
	protected:
		CStdPtrArray m_items;
		CDuiRect m_rcInset;
		int m_iChildPadding;
		UINT m_iChildAlign;
		UINT m_iChildVAlign;
		bool m_bAutoDestroy;
		bool m_bDelayedDestroy;
		bool m_bMouseChildEnabled;
		int	 m_nScrollStepSize;

		CScrollBarUI* m_pVerticalScrollBar;
		CScrollBarUI* m_pHorizontalScrollBar;
		CDuiString	m_sVerticalScrollBarStyle;
		CDuiString	m_sHorizontalScrollBarStyle;

		CControlUI *m_pCalcControl;
		CDuiRect m_rcCalcChild;
		bool m_bCalcResult;
	};

} // namespace DuiLib

#endif // __UICONTAINER_H__
