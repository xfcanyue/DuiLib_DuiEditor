#ifndef __UICOMBO_H__
#define __UICOMBO_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CComboWnd;

	class UILIB_API CComboUI : public CContainerUI, public IListOwnerUI
	{
		DECLARE_DUICONTROL(CComboUI)
		friend class CComboWnd;
	public:
		CComboUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void DoInit() override;
		virtual UINT GetControlFlags() const override;
	
		bool IsShowShadow();
		void SetShowShadow(bool bShow = true);

		CDuiString GetDropBoxAttributeList();
		void SetDropBoxAttributeList(LPCTSTR pstrList);
		SIZE GetDropBoxSize() const;
		void SetDropBoxSize(SIZE szDropBox);

		virtual UINT GetListType() override;
		virtual TListInfoUI* GetListInfo() override;
		virtual int GetCurSel() const override;  
		bool SelectItem(LPCTSTR pstrText);
		virtual bool SelectItem(int iIndex, bool bTakeFocus = false) override;
		virtual bool SelectMultiItem(int iIndex, bool bTakeFocus = false) override;
		virtual bool UnSelectItem(int iIndex, bool bOthers = false) override;
		virtual bool SetItemIndex(CControlUI* pControl, int iIndex) override;

		virtual bool Add(CControlUI* pControl) override;
		virtual bool AddAt(CControlUI* pControl, int iIndex) override;
		virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false) override;
		virtual bool RemoveAt(int iIndex, bool bDoNotDestroy=false) override;
		virtual void RemoveAll() override;

		virtual bool Activate() override;

		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true) override;
		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	protected:
		CComboWnd* m_pWindow;

		int m_iCurSel;
		
		bool	m_bShowShadow;
		CDuiString m_sDropBoxAttributes;
		SIZE m_szDropBox;

		TListInfoUI m_ListInfo;
	};

} // namespace DuiLib
#endif // __UICOMBO_H__

