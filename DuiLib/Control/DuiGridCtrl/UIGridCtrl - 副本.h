#ifndef __UIGRIDCTRL_H__
#define __UIGRIDCTRL_H__

//add by liqs99 ±Ì∏Ò¿‡
#pragma once
#include "UIGridFixedRow.h"
#include "UIGridFixedCell.h"
#include "UIGridBody.h"
#include "UIGridCell.h"

namespace DuiLib {
	typedef struct tagTGridInfoUI 
	{
		int nRowCount;
		int nColumnCount;
		int nFixedRowCount;
		int nFixedColumnCount;
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
	public: 
		virtual void DoInit();
		virtual void DoEvent(TEventUI& event);
		virtual void Notify(TNotifyUI& msg);

		bool Add(CControlUI* pControl);
	public:
		TGridInfoUI *GetGridInfo();

		CGridCellUI *GetCell(int row, int col);
		CGridCellUI &Cell(int row, int col);

		void SetRowCount(int rowcount);
		int  GetRowCount();

		void SetColumnCount(int columncount);
		int GetColumnCount();

		void SetFixedRowCount(int rowcount);
		int  GetFixedRowCount();

		void SetFixedColumnCount(int columncount);
		int GetFixedColumnCount();

		void SetHeaderBackColor(DWORD dwColor);
		DWORD GetHeaderBackColor();

		void SetLineColor(DWORD dwColor);
		DWORD GetLineColor();

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	private:

		TGridInfoUI m_TGridInfo;

		CGridFixedRowUI *m_pGridHeader;
		CGridBodyUI *m_pGridBody;
	};
	
} // namespace DuiLib

#endif // __UIGRIDCTRL_H__
