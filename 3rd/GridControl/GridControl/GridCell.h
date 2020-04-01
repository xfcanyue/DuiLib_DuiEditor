/////////////////////////////////////////////////////////////////////////////
// GridCell.h : header file
//
// MFC Grid Control - Grid cell class header file
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.20+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGridCtrl;
#include "GridCellBase.h"

// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

class XF_GRIDCONTROL_CTRL CGridCell : public CGridCellBase
{  
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCell)

// Construction/Destruction
public:
    CGridCell();
    virtual ~CGridCell();

// Attributes
public:
    void operator=(CGridCell& cell);

	/********* Added By Liqs99************/
	virtual void SetEditable(BOOL bEditable=TRUE)		 { m_bEditable = bEditable; }
	virtual BOOL IsEditable()							 { return m_bEditable; }
	virtual COLORREF GetHighLightBkColor();
	virtual COLORREF GetHighLightTextClr();
	virtual void SetStatCell(int nStat)					 { m_nStatCell = nStat; }
	virtual int GetStatCell()							 { return m_nStatCell; }

	virtual void SetFixed(BOOL bFixed);

	virtual void SetPicture(BOOL bPicture)				{ m_bPicture = bPicture; }
	virtual BOOL GetPicture()							{ return m_bPicture; }
	/********* Added By Liqs99************/
	//
    virtual void  SetText(LPCTSTR szText)        { m_strText = szText;  }  
	virtual void  SetText(int n)				 { m_strText.Format(_T("%d"), n); }
	virtual void  SetText(const COleDateTime &dt) { if(dt.GetStatus() == COleDateTime::valid) m_strText = dt.Format(_T("%Y-%m-%d %H:%M:%S")); }
	virtual void  SetText(const COleCurrency &cy) { m_strText = cy.Format(); }
	virtual void SetTextV(LPCTSTR lpszFormat, ...)
	{
		ASSERT(AfxIsValidString(lpszFormat));

		va_list argList;
		va_start(argList, lpszFormat);
	 	m_strText.FormatV(lpszFormat, argList);
		va_end(argList);
	}
	virtual void  SetImage(int nImage)           { m_nImage = nImage;   }                        
    virtual void  SetData(LPARAM lParam)         { m_lParam = lParam;   }      
    virtual void  SetGrid(CGridCtrl* pGrid)      { m_pGrid = pGrid;     }                          
    // virtual void SetState(const DWORD nState);  -  use base class version   
    virtual void  SetFormat(DWORD nFormat)       { m_nFormat = nFormat; }                      
    virtual void  SetTextClr(COLORREF clr)       { m_crFgClr = clr;     }                          
    virtual void  SetBackClr(COLORREF clr)       { m_crBkClr = clr;     }                          
    virtual void  SetFont(const LOGFONT* plf);
    virtual void  SetMargin(UINT nMargin)        { m_nMargin = nMargin; }
    virtual CWnd* GetEditWnd() const             { return m_pEditWnd;   }
    virtual void  SetCoords(int /*nRow*/, int /*nCol*/) {}  // don't need to know the row and
                                                            // column for base implementation

    virtual LPCTSTR     GetText() const             { return (m_strText.IsEmpty())? _T("") : LPCTSTR(m_strText); }
    virtual int         GetImage() const            { return m_nImage;  }
    virtual LPARAM      GetData() const             { return m_lParam;  }
    virtual CGridCtrl*  GetGrid() const             { return m_pGrid;   }
    // virtual DWORD    GetState() const - use base class
    virtual DWORD       GetFormat() const;
    virtual COLORREF    GetTextClr() const          { return m_crFgClr; } // TODO: change to use default cell
    virtual COLORREF    GetBackClr() const          { return m_crBkClr; }
    virtual LOGFONT*    GetFont() const;
    virtual CFont*      GetFontObject() const;
    virtual UINT        GetMargin() const;

    virtual BOOL        IsEditing() const           { return m_bEditing; }
    virtual BOOL        IsDefaultFont() const       { return (m_plfFont == NULL); }
    virtual void        Reset();

// editing cells
public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
protected:
    virtual void OnEndEdit();

protected:
	/********* Added By Liqs99************/
	BOOL m_bEditable;
	int m_nStatCell; //统计行,0=空行，1=数据行
	BOOL	   m_bPicture;		//当前格子是否图像;
	/********* Added By Liqs99************/
    CString    m_strText;      // Cell text (or binary data if you wish...)
    LPARAM     m_lParam;       // 32-bit value to associate with item
    int        m_nImage;       // Index of the list view item抯 icon
    DWORD      m_nFormat;
    COLORREF   m_crFgClr;
    COLORREF   m_crBkClr;
    LOGFONT*   m_plfFont;
    UINT       m_nMargin;

    BOOL       m_bEditing;     // Cell being edited?

    CGridCtrl* m_pGrid;        // Parent grid control
    CWnd*      m_pEditWnd;
};

// This class is for storing grid default values. It's a little heavy weight, so
// don't use it in bulk 
class CGridDefaultCell : public CGridCell
{
    DECLARE_DYNCREATE(CGridDefaultCell)

// Construction/Destruction
public:
    CGridDefaultCell();
    virtual ~CGridDefaultCell();

public:
    virtual DWORD GetStyle() const                      { return m_dwStyle;      }
    virtual void  SetStyle(DWORD dwStyle)               { m_dwStyle = dwStyle;   }
    virtual int   GetWidth() const                      { return m_Size.cx;      }
    virtual int   GetHeight() const                     { return m_Size.cy;      }
    virtual void  SetWidth(int nWidth)                  { m_Size.cx = nWidth;    }
    virtual void  SetHeight(int nHeight)                { m_Size.cy = nHeight;   }

    // Disable these properties
    virtual void     SetData(LPARAM /*lParam*/)             { ASSERT(FALSE);         }      
    virtual void     SetState(DWORD /*nState*/)             { ASSERT(FALSE);         }
    virtual DWORD    GetState() const                       { return CGridCell::GetState()|GVIS_READONLY; }
    virtual void     SetCoords( int /*row*/, int /*col*/)   { ASSERT(FALSE);         }
    virtual void     SetFont(const LOGFONT* /*plf*/);
    virtual LOGFONT* GetFont() const;   
    virtual CFont*   GetFontObject() const;

protected:
    CSize m_Size;       // Default Size
    CFont m_Font;       // Cached font
    DWORD m_dwStyle;    // Cell Style - unused
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
