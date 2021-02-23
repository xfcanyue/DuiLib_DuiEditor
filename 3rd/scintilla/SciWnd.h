#pragma once

#define _SCIWNDCTR_
#include "SciApi.h"

#define WM_SCIWND_RBUTTONUP		WM_USER+987
#define WM_SCIWND_LBUTTONDOWN	WM_USER+988
#define WM_SCIWND_LBUTTONUP		WM_USER+989
#define WM_SCIWND_CLICK			WM_USER+990
#define WM_SCIWND_MOUSEMOVE		WM_USER+991

// CSciWnd, MFC¿‡
typedef  int SciDll_void;
typedef int (*SEND_EDITOR)(void*,int,int,int);

class _SCIWNDCTR_ CSciWnd : public CWnd, public SciApi
{
	DECLARE_DYNAMIC(CSciWnd)
public:
	enum folderStyle {FOLDER_TYPE, FOLDER_STYLE_SIMPLE, FOLDER_STYLE_ARROW, FOLDER_STYLE_CIRCLE, FOLDER_STYLE_BOX, FOLDER_STYLE_NONE};
	void defineMarker(int marker, int markerType, COLORREF fore, COLORREF back, COLORREF foreActive) {
		execute(SCI_MARKERDEFINE, marker, markerType);
		execute(SCI_MARKERSETFORE, marker, fore);
		execute(SCI_MARKERSETBACK, marker, back);
		execute(SCI_MARKERSETBACKSELECTED, marker, foreActive);
	}

	CSciWnd();
	virtual ~CSciWnd();

	virtual BOOL Create (DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);

	BOOL LoadFile(LPCTSTR szPath);
	BOOL SaveFile(LPCTSTR szPath);

	void InitXML();
	void InitCpp();
	void InitReceipt() {}

	BOOL OnParentNotify(SCNotification *pMsg);
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
