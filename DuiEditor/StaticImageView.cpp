// StaticImageView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "StaticImageView.h"


// CStaticImageView

IMPLEMENT_DYNAMIC(CStaticImageView, CStatic)

CStaticImageView::CStaticImageView()
{

}

CStaticImageView::~CStaticImageView()
{
}


BEGIN_MESSAGE_MAP(CStaticImageView, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CStaticImageView 消息处理程序



void CStaticImageView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	CMemDC memDC(dc, this);
	CDC* pDC = &memDC.GetDC();

	CRect rectClient;
	CRgn rgnClip;
	this->GetClientRect(&rectClient);
	rgnClip.CreateRectRgnIndirect(&rectClient);
	pDC->SelectClipRgn(&rgnClip);
	pDC->FillRect(&rectClient, &afxGlobalData.brBtnFace);
	if(!m_imgPreview.IsNull())
		//m_imgPreview.BitBlt(pDC->GetSafeHdc(), rectClient.left, rectClient.right, rectClient.Width(), rectClient.Height(), 0, 0, SRCCOPY);
	m_imgPreview.Draw(pDC->GetSafeHdc(), rectClient);
}

void CStaticImageView::SetPreviewImage(LPCTSTR pstrImage)
{
	if(!m_imgPreview.IsNull())
		m_imgPreview.Destroy();

	if(*pstrImage != _T('\0'))
		m_imgPreview.Load(pstrImage);
	this->Invalidate(TRUE);
}

void CStaticImageView::SetPreviewImage(const CImage &image)
{
	CDC *pDCsrc,*pDCdrc;
	if (!m_imgPreview.IsNull())
	{
		m_imgPreview.Destroy();
	}
	m_imgPreview.Create(image.GetWidth(), image.GetHeight(), image.GetBPP(),0);

	pDCsrc=CDC::FromHandle(image.GetDC());
	pDCdrc=CDC::FromHandle(m_imgPreview.GetDC());
	pDCdrc->BitBlt(0,0, image.GetWidth(), image.GetHeight(),pDCsrc,0,0,SRCCOPY);
	image.ReleaseDC();
	m_imgPreview.ReleaseDC();
}