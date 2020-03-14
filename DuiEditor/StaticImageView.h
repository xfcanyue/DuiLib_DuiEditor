#pragma once


// CStaticImageView

class CStaticImageView : public CStatic
{
	DECLARE_DYNAMIC(CStaticImageView)

public:
	CStaticImageView();
	virtual ~CStaticImageView();

private:
	CImage m_imgPreview;

public:
	CImage *GetImage() { return &m_imgPreview; }
	void SetPreviewImage(LPCTSTR pstrImage);
	void SetPreviewImage(const CImage &image);

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};


