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

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

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

		void SetDefRowHeight(int height)			{ m_nDefRowHeight = height; }
		int GetDefRowHeight() const					{ return m_nDefRowHeight; }

		virtual void DoInit();
		void DoEvent(TEventUI& event);

		void SetPos(RECT rc, bool bNeedInvalidate = true);

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
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

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		virtual UINT GetControlFlags() const;

		CTableLayoutUI *GetTable() const;

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);

		virtual void DoInit();
		virtual SIZE EstimateSize(SIZE szAvailable);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	};

	//////////////////////////////////////////////////////////////////////////
	//Table列标记
	class UILIB_API CTDUI : public COptionLayoutUI
	{
		DECLARE_DUICONTROL(CTDUI)
	public:
		CTDUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		virtual UINT GetControlFlags() const;

		CTRUI *GetRow() const;
		CTableLayoutUI *GetTable() const;

		virtual void SetFixedWidth(int nWidth);
		virtual int GetFixedWidth() const;
		virtual void SetMinWidth(int nWidth);
		virtual int GetMinWidth() const;
		virtual void SetMaxWidth(int nWidth);
		virtual int GetMaxWidth() const;

		virtual bool IsAutoCalcWidth() const;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth);

		virtual RECT GetInset() const;
		virtual void SetInset(RECT rcInset);

		virtual RECT GetTextPadding() const;
		virtual void SetTextPadding(RECT rc);

		virtual void DoInit();
		virtual SIZE EstimateSize(SIZE szAvailable);
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	};
}
#endif // __UIVERTICALLAYOUT_H__
