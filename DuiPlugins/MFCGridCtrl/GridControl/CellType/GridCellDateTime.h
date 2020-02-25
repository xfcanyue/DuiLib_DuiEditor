// GridCellDateTime.h: interface for the CGridCellDateTime class.
//  
// Provides the implementation for a datetime picker cell type of the
// grid control.
//
// For use with CGridCtrl v2.22+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATETIMECELL_H__A0B7DA0A_0AFE_4D28_A00E_846C96D7507A__INCLUDED_)
#define AFX_DATETIMECELL_H__A0B7DA0A_0AFE_4D28_A00E_846C96D7507A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GridCell.h"
#include "afxdtctl.h"	// for CDateTimeCtrl

class XF_GRIDCONTROL_CTRL CGridCellDateTime : public CGridCell  
{
  friend class CGridCtrl;
  DECLARE_DYNCREATE(CGridCellDateTime)

  
  DWORD m_dwStyle;

public:
	CGridCellDateTime();
	CGridCellDateTime(DWORD dwStyle);
	virtual ~CGridCellDateTime();
    virtual CSize GetCellExtent(CDC* pDC);

  // editing cells
public:
	void Init(DWORD dwStyle);  //DTS_LONGDATEFORMAT DTS_SHORTDATEFORMAT DTS_TIMEFORMAT
	virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	virtual CWnd* GetEditWnd() const;
	virtual void  EndEdit();


	COleDateTime GetTime() {return m_curDT;};
	void   SetTime(const COleDateTime &time);

	//virtual LPCTSTR     GetText() const{ return m_coleDateTime.Format("%Y-%m-%d %H:%M:%S");}

	COleDateTime m_curDT;
};

class CInPlaceDateTime : public CDateTimeCtrl
{
// Construction
public:
	CInPlaceDateTime(CWnd* pParent,         // parent
                   CRect& rect,           // dimensions & location
                   DWORD dwStyle,         // window/combobox style
                   UINT nID,              // control ID
                   int nRow, int nColumn, // row and column
                   COLORREF crFore, COLORREF crBack,  // Foreground, background colour
                   COleDateTime* pColeDateTime,
          		   UINT nFirstChar);      // first character to pass to control

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceList)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceDateTime();
    void EndEdit();

// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDtnCloseup(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	//afx_msg void OnSelendOK();

	DECLARE_MESSAGE_MAP()

private:
    COleDateTime*   m_pColeDateTime;
	int		 m_nRow;
	int		 m_nCol;
 	UINT     m_nLastChar; 
	BOOL	 m_bExitOnArrows; 
    COLORREF m_crForeClr, m_crBackClr;
};

#endif // !defined(AFX_DATETIMECELL_H__A0B7DA0A_0AFE_4D28_A00E_846C96D7507A__INCLUDED_)
