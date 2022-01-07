// StaticImageView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "StaticImageView.h"


// CStaticImageView

IMPLEMENT_DYNAMIC(CStaticImageView, CStatic)

CStaticImageView::CStaticImageView()
{
	m_pRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
	m_pRender->Init(NULL, NULL);
	m_imgPreview = MakeRefPtr<UIImage>(UIGlobal::CreateImage());
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

// 	CMemDC memDC(dc, this);
// 	CDC* pDC = &memDC.GetDC();
// 
// 	CRect rectClient;
// 	CRgn rgnClip;
// 	this->GetClientRect(&rectClient);
// 	rgnClip.CreateRectRgnIndirect(&rectClient);
// 	pDC->SelectClipRgn(&rgnClip);
// 	pDC->FillRect(&rectClient, &afxGlobalData.brBlack);
// 	if(!m_imgPreview.IsNull())
// 		m_imgPreview.BitBlt(pDC->GetSafeHdc(), rectClient.left, rectClient.right, rectClient.Width(), rectClient.Height(), 0, 0, SRCCOPY);
// 	m_imgPreview.Draw(pDC->GetSafeHdc(), rectClient);

// 	CRect rcClient;
// 	GetClientRect(&rcClient);
// 	m_pRender->Resize(rcClient);
// 	m_pRender->SelectObject(m_imgPreview->bitmap);
// 	::BitBlt(dc.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(), m_pRender->GetDC(), 0, 0, SRCCOPY);
// 	m_pRender->RestoreObject();

	CRect rcClient;
	GetClientRect(&rcClient);
	//::BitBlt(dc.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(), m_pRender->GetDC(), 0, 0, SRCCOPY);
	::StretchBlt(dc.GetSafeHdc(), 0, 0, rcClient.Width(), rcClient.Height(), m_pRender->GetDC(), 0, 0, m_pRender->GetBitmap()->GetWidth(), m_pRender->GetBitmap()->GetHeight(), SRCCOPY);
	m_pRender->RestoreObject();
}

void CStaticImageView::SetPreviewImage(LPCTSTR pstrImage)
{	
	m_imgPreview->LoadImage(pstrImage);
	Invalidate(TRUE);
}

void CStaticImageView::SetPreviewImage(HBITMAP hImage)
{
	m_imgPreview->CreateImage(hImage,false);
}

void CStaticImageView::SetPreviewImage(UIRender *pRender)
{
	UIBitmap *pBitmap = pRender->GetBitmap();
	m_pRender->Resize(pBitmap->GetWidth(), pBitmap->GetHeight());
	m_pRender->BitBlt(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight(), pRender, 0, 0, SRCCOPY);
}