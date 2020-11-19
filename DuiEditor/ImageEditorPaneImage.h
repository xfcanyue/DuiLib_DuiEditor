#pragma once
#include "ImageEditorImagePreview.h"

// CImageEditorThumbnail

class CImageEditorPaneImage : public CDockablePane
{
	DECLARE_DYNAMIC(CImageEditorPaneImage)

public:
	CImageEditorPaneImage();
	virtual ~CImageEditorPaneImage();

	void SetCaptionText(LPCTSTR lpszCaption);

	CImageEditorImagePreview *m_pView;
	CString m_strCaption;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


