// DlgMix.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "demoDoc.h"
#include "DlgMix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgMix dialog


DlgMix::DlgMix(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMix::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgMix)
	m_xoffset = 0;
	m_yoffset = 0;
	m_mixalpha = FALSE;
	//}}AFX_DATA_INIT
}


void DlgMix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgMix)
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_BUTTON1, m_preview);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDC_SRC, m_cbSrc);
	DDX_Control(pDX, IDC_OP, m_cbOpType);
	DDX_Control(pDX, IDC_DST, m_cbDst);
	DDX_Text(pDX, IDC_EDIT1, m_xoffset);
	DDX_Text(pDX, IDC_EDIT2, m_yoffset);
	DDX_Check(pDX, IDC_CHECK1, m_mixalpha);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgMix, CDialog)
	//{{AFX_MSG_MAP(DlgMix)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_DST, OnSelchangeDst)
	ON_CBN_SELCHANGE(IDC_SRC, OnSelchangeSrc)
	ON_CBN_SELCHANGE(IDC_OP, OnSelchangeOp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgMix message handlers

BOOL DlgMix::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bitmap=0;
	m_ratio=1.0f;
	m_xoffset=m_yoffset=0;
	pDocSrc = pDocDst = 0;
	OpType=0;
	m_mixalpha = 0;

	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos != NULL) {
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2 != NULL) {
			CDemoDoc* pDoc = (CDemoDoc*) pTemplate->GetNextDoc(pos2);
			ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
			CString title = pDoc->GetTitle();
			m_cbSrc.AddString(title);
			m_cbSrc.SetItemData(m_cbSrc.GetCount()-1, (DWORD)pDoc);
			m_cbDst.AddString(title);
			m_cbDst.SetItemData(m_cbDst.GetCount()-1, (DWORD)pDoc);
		}
	}
	m_cbSrc.SetCurSel(0);

	if (m_cbDst.GetCount()>1){
		m_cbDst.SetCurSel(1);
	} else {
		m_cbDst.SetCurSel(0);
	}
	
	m_cbOpType.AddString(_T("OpAvg"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpAvg);
	m_cbOpType.AddString(_T("OpAdd"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpAdd);
	m_cbOpType.AddString(_T("OpAnd"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpAnd);
	m_cbOpType.AddString(_T("OpXor"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpXor);
	m_cbOpType.AddString(_T("OpOr"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpOr);
	m_cbOpType.AddString(_T("OpScreen"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpScreen);
	m_cbOpType.AddString(_T("OpMask"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpMask);
	m_cbOpType.AddString(_T("OpSrcCopy"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpSrcCopy);
	m_cbOpType.AddString(_T("OpDstCopy"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpDstCopy);
	m_cbOpType.AddString(_T("OpSub"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpSub);
	m_cbOpType.AddString(_T("OpSrcBlend"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpSrcBlend);
	m_cbOpType.AddString(_T("OpBlendAlpha"));
	m_cbOpType.SetItemData(m_cbOpType.GetCount()-1, (DWORD)CxImage::OpBlendAlpha);
	m_cbOpType.SetCurSel(0);


	SetMix();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgMix::OnButton1() 
{
	UpdateData(1);
	SetMix();
}

void DlgMix::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_bitmap) DeleteObject(m_bitmap);
}

void DlgMix::OnSelchangeDst() 
{
	UpdateData(1);
	SetMix();
}

void DlgMix::OnSelchangeSrc() 
{
	UpdateData(1);
	SetMix();
}

void DlgMix::OnSelchangeOp() 
{
	UpdateData(1);
	SetMix();
}

void DlgMix::SetThumbs(CxImage* pSrc, CxImage* pDst)
{
	if (pSrc==0 || pDst==0) return;

	if (pDst->GetWidth() > pDst->GetHeight()){
		m_ratio = 200.0f/pDst->GetWidth();
	} else {
		m_ratio = 200.0f/pDst->GetHeight();
	}
	if (m_ratio > 1.0f) m_ratio = 1.0f;

	pSrc->Resample((long)(pSrc->GetWidth()*m_ratio),(long)(pSrc->GetHeight()*m_ratio),1,&m_imageSrc);
	pDst->Resample((long)(pDst->GetWidth()*m_ratio),(long)(pDst->GetHeight()*m_ratio),1,&m_imageDst);
}

void DlgMix::SetMix()
{
	pDocSrc = (CDemoDoc*) m_cbSrc.GetItemData(m_cbSrc.GetCurSel());
	pDocDst = (CDemoDoc*) m_cbDst.GetItemData(m_cbDst.GetCurSel());
	OpType = m_cbOpType.GetItemData(m_cbOpType.GetCurSel());

	if (pDocSrc==0 || pDocDst==0) return;

	CxImage* pImageSrc = pDocSrc->GetImage();
	CxImage* pImageDst = pDocDst->GetImage();

	SetThumbs(pImageSrc,pImageDst);

	CxImage tmp;
	tmp.Copy(m_imageDst);
	tmp.Mix(m_imageSrc,(CxImage::ImageOpType)OpType,(long)(m_xoffset*m_ratio),(long)(m_yoffset*m_ratio),m_mixalpha!=0);

#if CXIMAGE_SUPPORT_ALPHA
	if (m_mixalpha!=0){
		RGBQUAD c={255,255,255,0};
		tmp.SetTransColor(c);
		tmp.AlphaStrip();
	}
#endif

	if (m_bitmap) DeleteObject(m_bitmap);
	m_bitmap = tmp.MakeBitmap(m_picture.GetDC()->m_hDC);
	m_picture.SetBitmap(m_bitmap);
}


void DlgMix::OnOK() 
{
	SetMix();
	CDialog::OnOK();
}
