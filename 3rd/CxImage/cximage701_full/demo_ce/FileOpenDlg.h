#if !defined(AFX_FILEOPENDLG_H__04B959E8_3973_45C5_BC62_E7BDB26B0B48__INCLUDED_)
#define AFX_FILEOPENDLG_H__04B959E8_3973_45C5_BC62_E7BDB26B0B48__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FileOpenDlg.h : header file
//


#include <deque>
#include "ExDialog.h"


typedef std::deque<WIN32_FIND_DATA>	CFileInfoCont;


//---------------------------------------------------------------------------
//
//	CFileOpenDlg dialog
//
//---------------------------------------------------------------------------


class CFileOpenDlg : public CExDialog
{
	enum ViewState
	{
		vsStandard,		// Shows list and tree
		vsTreeOnly,		// Shows tree only
		vsListOnly		// Shows list only
	};

	enum {nTrackHeight = 6};

public:
	CFileOpenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileOpenDlg)
	enum { IDD = IDD_FILE_OPEN };
	CListCtrl	m_listFile;
	CTreeCtrl	m_treeFolder;
	//}}AFX_DATA

	LPCTSTR GetFileName() {return m_strFile;}
	LPCTSTR GetPath() {return m_strPath;}

	void SetFilter(LPCTSTR pszFilter) {m_strFilter = pszFilter;}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileOpenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList		m_imlHeader,
					m_imlSystem;		//!< The system image list
	CString			m_strPath,
					m_strFile,
					m_strFilter,
					m_strDateFormat;
	CRect			m_rcTree,
					m_rcList;
	ViewState		m_nState;			// Dialog state.
	CCommandBar		m_wndCommandBar;
	CCommandBar*	m_pCmdBar;			//!< The command bar.
	BOOL			m_bOk;				//!< OK button enabled?
	CFileInfoCont	m_cont;
	int				m_nSort;

	//
	// Splitter stuff
	//
	BOOL			m_bTracking;
	int				m_yTrackDel,
					m_ySplit,			//!< The tracking position
					m_yTrack;

	//
	// Enumeration methods
	//
	int EnumDirs(LPCTSTR pszPath, LPCTSTR pszFilter, HTREEITEM hItemParent);
	void EnumFiles(LPCTSTR pszPath, LPCTSTR pszFilter);

	CString GetFullPath(HTREEITEM hItem);
	CString FormatSize(DWORD dwSize);

	CString GetSystemName();
	
	int GetIconIndex(LPCTSTR pszFileName);

	void UpdateControls();

	void DoSortList();
	void SetColumnIcons();

	//! Virtual method called by the painting engine.
	virtual	void DoPaint(CPaintDC &dc);

	// Generated message map functions
	//{{AFX_MSG(CFileOpenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedFolderTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewStandard();
	afx_msg void OnViewTree();
	afx_msg void OnViewList();
	afx_msg void OnBarOk();
	afx_msg void OnBarCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHeaderClickFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemExpandingFolderTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoFolderTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEOPENDLG_H__04B959E8_3973_45C5_BC62_E7BDB26B0B48__INCLUDED_)
