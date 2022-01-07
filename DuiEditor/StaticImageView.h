#pragma once


// CStaticImageView

class CStaticImageView : public CStatic
{
	DECLARE_DYNAMIC(CStaticImageView)

public:
	CStaticImageView();
	virtual ~CStaticImageView();

private:
	CStdRefPtr<UIRender> m_pRender;
	CStdRefPtr<UIImage> m_imgPreview;

public:
	UIImage *GetImage() { return m_imgPreview; }
	void SetPreviewImage(LPCTSTR pstrImage);
	void SetPreviewImage(HBITMAP hImage);
	void SetPreviewImage(UIRender *pRender);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};


