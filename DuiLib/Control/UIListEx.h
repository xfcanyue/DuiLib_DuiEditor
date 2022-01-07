#ifndef __UILISTEX_H__
#define __UILISTEX_H__

#pragma once

#include "Layout/UIVerticalLayout.h"
#include "Layout/UIHorizontalLayout.h"

namespace DuiLib {

	class IListComboCallbackUI
	{
	public:
		virtual void GetItemComboTextArray(CControlUI* pCtrl, int iItem, int iSubItem) = 0;
	};

	//class CEditUI;
	//class CComboUI;
	class UILIB_API CListExUI : public CListUI, public INotifyUI
	{
		DECLARE_DUICONTROL(CListExUI)

	public:
		CListExUI();

		virtual LPCTSTR GetClass() const override;
		virtual UINT GetControlFlags() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	public: 
		virtual void DoEvent(TEventUI& event) override;

	public:
		void InitListCtrl();

	protected:
		CEditUI*		m_pEditUI;
		CComboUI*	m_pComboBoxUI;

	public:
		virtual BOOL CheckColumEditable(int nColum);
		virtual CEditUI* GetEditUI();

		virtual BOOL CheckColumComboBoxable(int nColum);
		virtual CComboUI* GetComboBoxUI();

		virtual BOOL CheckColumCheckBoxable(int nColum);

	public:
		virtual void Notify(TNotifyUI& msg);
		BOOL	m_bAddMessageFilter;
		int		m_nRow,m_nColum;
		void	SetEditRowAndColum(int nRow,int nColum) { m_nRow = nRow; m_nColum = nColum; };

	public:
		IListComboCallbackUI* m_pXCallback;
		virtual IListComboCallbackUI* GetTextArrayCallback() const;
		virtual void SetTextArrayCallback(IListComboCallbackUI* pCallback);

	public:
		void OnListItemClicked(int nIndex, int nColum, RECT* lpRCColum, LPCTSTR lpstrText);
		void OnListItemChecked(int nIndex, int nColum, BOOL bChecked);

	public:
		void SetColumItemColor(int nIndex, int nColum, DWORD iBKColor);
		BOOL GetColumItemColor(int nIndex, int nColum, DWORD& iBKColor);

	private:
		void HideEditAndComboCtrl();
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API CListContainerHeaderItemUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CListContainerHeaderItemUI)

	public:
		CListContainerHeaderItemUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		bool IsHeaderDragEnable() const;
		void SetHeaderDragEnable(bool bDragable);

		CDuiString GetSepImage() const;
		void SetSepImage(LPCTSTR pStrImage);

		virtual void DoEvent(TEventUI& event) override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		RECT GetThumbRect(bool bUseNew = false) const;

		virtual void PaintText(UIRender *pRender) override;
		virtual void PaintStatusImage(UIRender *pRender) override;

	protected:
		bool m_bDragable;
		CDuiString m_sSepImage;
		CDuiString m_sSepImageModify;

		//支持编辑
		BOOL m_bEditable;

		//支持组合框
		BOOL m_bComboable;

		//支持复选框
		BOOL m_bCheckBoxable;

	public:
		BOOL GetColumeEditable();
		void SetColumeEditable(BOOL bEnable);

		BOOL GetColumeComboable();
		void SetColumeComboable(BOOL bEnable);

		BOOL GetColumeCheckable();
		void SetColumeCheckable(BOOL bEnable);

	public:
		void SetCheck(BOOL bCheck);
		BOOL GetCheck();

	private:
		TButtonState	m_uCheckBoxState;
		BOOL			m_bChecked;

		CDuiString m_sCheckBoxNormalImage;
		CDuiString m_sCheckBoxHotImage;
		CDuiString m_sCheckBoxPushedImage;
		CDuiString m_sCheckBoxFocusedImage;
		CDuiString m_sCheckBoxDisabledImage;

		CDuiString m_sCheckBoxSelectedImage;
		CDuiString m_sCheckBoxForeImage;

		SIZE m_cxyCheckBox;

	public:
		BOOL DrawCheckBoxImage(UIRender *pRender, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
		LPCTSTR GetCheckBoxNormalImage();
		void SetCheckBoxNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxHotImage();
		void SetCheckBoxHotImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxPushedImage();
		void SetCheckBoxPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxFocusedImage();
		void SetCheckBoxFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxDisabledImage();
		void SetCheckBoxDisabledImage(LPCTSTR pStrImage);

		LPCTSTR GetCheckBoxSelectedImage();
		void SetCheckBoxSelectedImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxForeImage();
		void SetCheckBoxForeImage(LPCTSTR pStrImage);

		void GetCheckBoxRect(RECT &rc);	

		int GetCheckBoxWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxWidth(int cx);      // 预设的参考值
		int GetCheckBoxHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxHeight(int cy);     // 预设的参考值


	public:
		CContainerUI* m_pOwner;
		void SetOwner(CContainerUI* pOwner);
		CContainerUI* GetOwner();



	};
	 
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListTextExtElementUI : public CListLabelElementUI
	{
		DECLARE_DUICONTROL(CListTextExtElementUI)

	public:
		CListTextExtElementUI();
		~CListTextExtElementUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		CDuiString GetText(int iIndex) const;
		void SetText(int iIndex, LPCTSTR pstrText);

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
		CListUI* m_pOwner;
		CStdPtrArray m_aTexts;

	private:
		UINT	m_uCheckBoxState;
		BOOL	m_bChecked;

		CDuiString m_sCheckBoxNormalImage;
		CDuiString m_sCheckBoxHotImage;
		CDuiString m_sCheckBoxPushedImage;
		CDuiString m_sCheckBoxFocusedImage;
		CDuiString m_sCheckBoxDisabledImage;

		CDuiString m_sCheckBoxSelectedImage;
		CDuiString m_sCheckBoxForeImage;

		SIZE m_cxyCheckBox;

	public:
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		virtual void PaintStatusImage(UIRender *pRender);
		BOOL DrawCheckBoxImage(UIRender *pRender, LPCTSTR pStrImage, LPCTSTR pStrModify, RECT& rcCheckBox);
		LPCTSTR GetCheckBoxNormalImage();
		void SetCheckBoxNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxHotImage();
		void SetCheckBoxHotImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxPushedImage();
		void SetCheckBoxPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxFocusedImage();
		void SetCheckBoxFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxDisabledImage();
		void SetCheckBoxDisabledImage(LPCTSTR pStrImage);

		LPCTSTR GetCheckBoxSelectedImage();
		void SetCheckBoxSelectedImage(LPCTSTR pStrImage);
		LPCTSTR GetCheckBoxForeImage();
		void SetCheckBoxForeImage(LPCTSTR pStrImage);

		void GetCheckBoxRect(int nIndex, RECT &rc);	
		void GetColumRect(int nColum, RECT &rc);

		int GetCheckBoxWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxWidth(int cx);      // 预设的参考值
		int GetCheckBoxHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		void SetCheckBoxHeight(int cy);     // 预设的参考值

		void SetCheck(BOOL bCheck);
		BOOL GetCheck() const;

	public:
		int HitTestColum(POINT ptMouse);
		BOOL CheckColumEditable(int nColum);

	private:
		typedef struct tagColumColorNode
		{
			BOOL  bEnable;
			DWORD iTextColor;
			DWORD iBKColor;
		}COLUMCOLORNODE;

		COLUMCOLORNODE ColumCorlorArray[UILIST_MAX_COLUMNS];

	public:
		void SetColumItemColor(int nColum, DWORD iBKColor);
		BOOL GetColumItemColor(int nColum, DWORD& iBKColor);

	};
} // namespace DuiLib

#endif // __UILISTEX_H__
