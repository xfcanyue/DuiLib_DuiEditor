#ifndef __UITABLELAYOUT_H__
#define __UITABLELAYOUT_H__

#pragma once

namespace DuiLib
{
	#define MAX_TABLE_COLUMN_COUNT	16

	//////////////////////////////////////////////////////////////////////////
	//Table布局
	class UILIB_API CTableLayoutUI : public CVerticalLayoutUI
	{
		DECLARE_DUICONTROL(CTableLayoutUI)
	public:
		CTableLayoutUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		void SetColWidth(int col, int nWidth);
		int GetColWidth(int col) const;
		void SetColMinWidth(int col, int nWidth);
		int GetColMinWidth(int col) const;
		void SetColMaxWidth(int col, int nWidth);
		int GetColMaxWidth(int col) const;

		bool IsColAutoCalcWidth(int col) const;
		void SetColAutoCalcWidth(int col, bool bAutoCalcWidth);

		RECT GetColInset(int col) const;
		void SetColInset(int col, RECT rcInset);

		RECT GetColTextPadding(int col) const;
		void SetColTextPadding(int col, RECT rc);

		void SetDefRowHeight(int height);
		int GetDefRowHeight() const;

		virtual void DoInit() override;
		virtual void DoEvent(TEventUI& event) override;

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	protected:
		int m_aColWidth[MAX_TABLE_COLUMN_COUNT];
		int m_aColMinWidth[MAX_TABLE_COLUMN_COUNT];
		int m_aColMaxWidth[MAX_TABLE_COLUMN_COUNT];
		bool m_aColAutoCalcWidth[MAX_TABLE_COLUMN_COUNT];

		RECT m_aColInset[MAX_TABLE_COLUMN_COUNT];
		RECT m_aColTextPadding[MAX_TABLE_COLUMN_COUNT];

		int   m_nDefRowHeight;				//default row's height
	};

	//////////////////////////////////////////////////////////////////////////
	//Table行标记
	class UILIB_API CTRUI : public COptionLayoutUI
	{
		DECLARE_DUICONTROL(CTRUI)
	public:
		CTRUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		CTableLayoutUI *GetTable() const;

		virtual bool Add(CControlUI* pControl) override;
		virtual bool AddAt(CControlUI* pControl, int iIndex) override;

		virtual void DoInit() override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	};

	//////////////////////////////////////////////////////////////////////////
	//Table列标记
	class UILIB_API CTDUI : public COptionLayoutUI
	{
		DECLARE_DUICONTROL(CTDUI)
	public:
		CTDUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		CTRUI *GetRow() const;
		CTableLayoutUI *GetTable() const;

		virtual void SetFixedWidth(int nWidth) override;
		virtual int GetFixedWidth() const override;
		virtual void SetMinWidth(int nWidth) override;
		virtual int GetMinWidth() const override;
		virtual void SetMaxWidth(int nWidth) override;
		virtual int GetMaxWidth() const override;

		virtual bool IsAutoCalcWidth() const override;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth) override;

		virtual RECT GetInset() const override;
		virtual void SetInset(RECT rcInset) override;

		virtual RECT GetTextPadding() const override;
		virtual void SetTextPadding(RECT rc) override;

		virtual void DoInit() override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	};
}
#endif // __UIVERTICALLAYOUT_H__
