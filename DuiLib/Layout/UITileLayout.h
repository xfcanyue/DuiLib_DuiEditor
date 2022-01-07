#ifndef __UITILELAYOUT_H__
#define __UITILELAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CTileLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CTileLayoutUI)
	public:
		CTileLayoutUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;

		int GetFixedColumns() const;
		void SetFixedColumns(int iColums);
		int GetChildVPadding() const;
		void SetChildVPadding(int iPadding);

		SIZE GetItemSize() const;
		void SetItemSize(SIZE szSize);
		int GetColumns() const;
		int GetRows() const;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	protected:
		SIZE m_szItem;
		int m_nColumns;
		int m_nRows;

		int m_nColumnsFixed;
		int m_iChildVPadding;
		bool m_bIgnoreItemPadding;
	};
}
#endif // __UITILELAYOUT_H__
