// GridURLCell.h: interface for the CGridURLCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDURLCELL_H__9F4A50B4_D773_11D3_A439_F7E60631F563__INCLUDED_)
#define AFX_GRIDURLCELL_H__9F4A50B4_D773_11D3_A439_F7E60631F563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GridCell.h"

typedef struct {
    LPCTSTR szURLPrefix;
    size_t  nLength;
} URLStruct;



class CGridURLCell : public CGridCell  
{
    DECLARE_DYNCREATE(CGridURLCell)

public:
	CGridURLCell();
	virtual ~CGridURLCell();

    virtual BOOL     Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
    virtual BOOL     Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual LPCTSTR  GetTipText() { return NULL; }
	void SetAutoLaunchUrl(BOOL bLaunch = TRUE) { m_bLaunchUrl = bLaunch;	}
	BOOL GetAutoLaunchUrl() { return m_bLaunchUrl;	}

protected:
    virtual BOOL OnSetCursor();
    virtual void OnClick(CPoint PointCellRelative);

	BOOL HasUrl(CString str);
    BOOL OverURL(CPoint& pt, CString& strURL);

protected:
#ifndef _WIN32_WCE
    static HCURSOR g_hLinkCursor;		// Hyperlink mouse cursor
	HCURSOR GetHandCursor();
#endif
    static URLStruct g_szURIprefixes[];

protected:
	COLORREF m_clrUrl;
	BOOL     m_bLaunchUrl;
    CRect    m_Rect;
};

#endif // !defined(AFX_GRIDURLCELL_H__9F4A50B4_D773_11D3_A439_F7E60631F563__INCLUDED_)
