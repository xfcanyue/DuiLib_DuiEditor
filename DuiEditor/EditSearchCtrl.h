#pragma once


// CEditSearchCtrl

class CEditSearchCtrl : public CMFCEditBrowseCtrl
{
	DECLARE_DYNAMIC(CEditSearchCtrl)

public:
	CEditSearchCtrl();
	virtual ~CEditSearchCtrl();

	virtual void OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot);
	void SetImage();
	virtual void OnBrowse();
protected:
	DECLARE_MESSAGE_MAP()
};


