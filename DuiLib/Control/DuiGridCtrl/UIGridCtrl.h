#ifndef __UIGRIDCTRL_H__
#define __UIGRIDCTRL_H__

//add by liqs99 表格类
#pragma once
#include "UIGridHeader.h"
#include "UIGridBody.h"
#include "UIGridCell.h"
#include "UIGridFixedRow.h"
#include "UIGridFixedCell.h"
#include "UIGridTextRow.h"

namespace DuiLib {
	typedef struct tagTGridInfoUI 
	{
		int nRowCount;
		int nColumnCount;
		int nFixedRowCount;
		int nFixedColumnCount;
		//int nColumnWith[DUI_GRID_MAX_COLUMN];
		RECT rcColumn[DUI_GRID_MAX_COLUMN];
		DWORD dwHeaderBkColor;
		DWORD dwLineColor;
	}TGridInfoUI;

	class UILIB_API CGridCtrlUI : public CVerticalLayoutUI, public INotifyUI
	{
		DECLARE_DUICONTROL(CGridCtrlUI)

	public:
		CGridCtrlUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	protected:
		virtual void DoInit();
		virtual void DoEvent(TEventUI& event);
		virtual void Notify(TNotifyUI& msg);
	
		bool Add(CControlUI* pControl);
		void SetPos(RECT rc, bool bNeedInvalidate = true);
	public:
		int InsertRow();
	public:
		TGridInfoUI *GetGridInfo();

		CGridCellUI *GetCell(int row, int col);
		CGridCellUI &Cell(int row, int col);

		int GetRowCount();
		int GetColumnCount();

		bool IsFixedScrollbar();
		void SetFixedScrollbar(bool bFixed);

		void SetHeaderBackColor(DWORD dwColor);
		DWORD GetHeaderBackColor();

		void SetLineColor(DWORD dwColor);
		DWORD GetLineColor();

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	private:
		TGridInfoUI m_TGridInfo;

		bool m_bFixedScrollbar;

		CGridHeaderUI *m_pGridHeader; //支持多个表头
		CGridBodyUI *m_pGridBody;
	};
	
} // namespace DuiLib

#endif // __UIGRIDCTRL_H__
