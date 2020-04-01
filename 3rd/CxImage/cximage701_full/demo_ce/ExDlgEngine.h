// ExDlgEngine.h: interface for the CExDlgEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXDLGENGINE_H__87E5E8EA_16C8_4FDD_BE0E_16E80BA4F555__INCLUDED_)
#define AFX_EXDLGENGINE_H__87E5E8EA_16C8_4FDD_BE0E_16E80BA4F555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxdtctl.h>

class CExDlgEngine  
{
protected:
	int		m_nHeaderHeight;
	CFont	m_fontBold;

public:
	CExDlgEngine();
	virtual ~CExDlgEngine();

	void	PaintHeader		(CWnd* pWnd, CDC &dc, LPCTSTR pszTitle);
	void	Underline		(CWnd* pWnd, CDC &dc, UINT *pArrID);
	BOOL	CreateDateTime	(CWnd* pWnd, CDateTimeCtrl &dtc, UINT nID, DWORD dwStyle);

	BOOL	SetBold	(CWnd* pWnd, UINT nID);

	int GetHeaderHeight() {return m_nHeaderHeight;}
};

#endif // !defined(AFX_EXDLGENGINE_H__87E5E8EA_16C8_4FDD_BE0E_16E80BA4F555__INCLUDED_)
