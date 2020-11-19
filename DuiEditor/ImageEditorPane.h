#pragma once
#include "ImageEditorPaneDialog.h"

// CImageEditorPane

class CImageEditorPaneAdjust : public CDockablePane
{
	DECLARE_DYNAMIC(CImageEditorPaneAdjust)

public:
	CImageEditorPaneAdjust();
	virtual ~CImageEditorPaneAdjust();

	CImageEditorAdjust *m_pForm;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


