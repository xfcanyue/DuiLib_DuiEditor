// ImageEditorPaneDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorPaneDialog.h"
#include "afxdialogex.h"

#include "ImageEditor.h"
#include "ImageEditorFrame.h"
// CImageEditorPaneDialog 对话框

IMPLEMENT_DYNAMIC(CImageEditorAdjust, CDialogEx)

CImageEditorAdjust::CImageEditorAdjust(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageEditorAdjust::IDD, pParent)
{
	m_bDoEditChanged = FALSE;
	m_bDoInitData = TRUE;
}

CImageEditorAdjust::~CImageEditorAdjust()
{
}

void CImageEditorAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageEditorAdjust, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_DEST_WIDTH, &CImageEditorAdjust::OnChangeEditDestWidth)
	ON_EN_CHANGE(IDC_EDIT_DEST_HEIGHT, &CImageEditorAdjust::OnChangeEditDestHeight)
	ON_EN_CHANGE(IDC_EDIT_SOURCE_WIDTH, &CImageEditorAdjust::OnChangeEditSourceWidth)
	ON_EN_CHANGE(IDC_EDIT_SOURCE_HEIGHT, &CImageEditorAdjust::OnChangeEditSourceHeight)
	ON_EN_CHANGE(IDC_EDIT_CORNER_WIDTH, &CImageEditorAdjust::OnChangeEditCornerWidth)
	ON_EN_CHANGE(IDC_EDIT_CORNER_HEIGHT, &CImageEditorAdjust::OnChangeEditCornerHeight)

	ON_BN_CLICKED(IDC_BUTTON_DEST_LEFT, &CImageEditorAdjust::OnBnClickedButtonDestLeft)
	ON_BN_CLICKED(IDC_BUTTON_DEST_RIGHT, &CImageEditorAdjust::OnBnClickedButtonDestRight)
	ON_BN_CLICKED(IDC_BUTTON_DEST_TOP, &CImageEditorAdjust::OnBnClickedButtonDestTop)
	ON_BN_CLICKED(IDC_BUTTON_DEST_BOTTOM, &CImageEditorAdjust::OnBnClickedButtonDestBottom)

	ON_BN_CLICKED(IDC_BUTTON_SOURCE_LEFT, &CImageEditorAdjust::OnBnClickedButtonSourceLeft)
	ON_BN_CLICKED(IDC_BUTTON_SOURCE_RIGHT, &CImageEditorAdjust::OnBnClickedButtonSourceRight)
	ON_BN_CLICKED(IDC_BUTTON_SOURCE_TOP, &CImageEditorAdjust::OnBnClickedButtonSourceTop)
	ON_BN_CLICKED(IDC_BUTTON_SOURCE_BOTTOM, &CImageEditorAdjust::OnBnClickedButtonSourceBottom)

	ON_BN_CLICKED(IDC_BUTTON_CORNER_LEFT, &CImageEditorAdjust::OnBnClickedButtonCornerLeft)
	ON_BN_CLICKED(IDC_BUTTON_CORNER_RIGHT, &CImageEditorAdjust::OnBnClickedButtonCornerRight)
	ON_BN_CLICKED(IDC_BUTTON_CORNER_TOP, &CImageEditorAdjust::OnBnClickedButtonCornerTop)
	ON_BN_CLICKED(IDC_BUTTON_CORNER_BOTTOM, &CImageEditorAdjust::OnBnClickedButtonCornerBottom)
	ON_BN_CLICKED(IDC_BUTTON_DEST_RESET, &CImageEditorAdjust::OnBnClickedButtonDestReset)
	ON_BN_CLICKED(IDC_BUTTON_SOURCE_RESET, &CImageEditorAdjust::OnBnClickedButtonSourceReset)
	ON_BN_CLICKED(IDC_BUTTON_CORNER_RESET, &CImageEditorAdjust::OnBnClickedButtonCornerReset)
END_MESSAGE_MAP()


// CImageEditorPaneDialog 消息处理程序


BOOL CImageEditorAdjust::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN3))->SetRange32(-9999, 9999);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN4))->SetRange32(-9999, 9999);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN5))->SetRange32(-9999, 9999);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN6))->SetRange32(-9999, 9999);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN7))->SetRange32(-9999, 9999);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN8))->SetRange32(-9999, 9999);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CImageEditorAdjust::GetIntPara(UINT nID)
{
	CString text;
	GetDlgItem(nID)->GetWindowText(text);
	return _ttoi(text);
}

void CImageEditorAdjust::InitData()
{
	m_bDoEditChanged = FALSE;

	if(m_bDoInitData)
	{
		CImageEditor *pDlgMain = (CImageEditor *)GetParent()->GetParent()->GetParent();
		xml_node nodedata = pDlgMain->m_nodeImage.child(XTEXT("IMAGE"));

		{
			xml_attribute attr = nodedata.attribute(XTEXT("dest"));
			CDuiRect rc(XML2T(attr.value()));
			GetDlgItem(IDC_EDIT_DEST_WIDTH)->SetWindowText(NumberToString(rc.GetWidth()));
			GetDlgItem(IDC_EDIT_DEST_HEIGHT)->SetWindowText(NumberToString(rc.GetHeight()));
		}
		{
			xml_attribute attr = nodedata.attribute(XTEXT("source"));
			CDuiRect rc(XML2T(attr.value()));
			GetDlgItem(IDC_EDIT_SOURCE_WIDTH)->SetWindowText(NumberToString(rc.GetWidth()));
			GetDlgItem(IDC_EDIT_SOURCE_HEIGHT)->SetWindowText(NumberToString(rc.GetHeight()));
		}
		{
			xml_attribute attr = nodedata.attribute(XTEXT("corner"));
			CDuiRect rc(XML2T(attr.value()));
			GetDlgItem(IDC_EDIT_CORNER_WIDTH)->SetWindowText(NumberToString(rc.GetWidth()));
			GetDlgItem(IDC_EDIT_CORNER_HEIGHT)->SetWindowText(NumberToString(rc.GetHeight()));
		}
	}
	else
	{
		m_bDoInitData = TRUE;
	}
	m_bDoEditChanged = TRUE;
}

void CImageEditorAdjust::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}

void CImageEditorAdjust::UpdatePara(RECT rc, LPCTSTR attrName)
{
	SetAttributeRect(attrName, rc);

	g_pEditorImage->m_pFrame->m_wndView.InitData();
	g_pEditorImage->m_pFrame->m_wndImage.m_pView->InitData();

	g_pEditorImage->m_pFrame->m_pPropList->InitProp(g_pEditorImage->m_nodedata);
	g_pEditorImage->m_pFrame->m_wndProperty.OnExpandAllProperties();

	g_pEditorImage->m_pFrame->ParentPreview();
}

RECT CImageEditorAdjust::GetAttributeRect(LPCTSTR attrName)
{
	xml_attribute attr = g_pEditorImage->m_nodedata.attribute(T2XML(attrName));
	CDuiRect rc(XML2T(attr.value()));
	return rc;
}

void CImageEditorAdjust::SetAttributeRect(LPCTSTR attrName, RECT &rc)
{
	g_duiProp.AddAttribute(g_pEditorImage->m_nodedata, attrName, RectToString(&rc), NULL);
}

void CImageEditorAdjust::OnChangeEditDestWidth()
{
	if(!m_bDoEditChanged)
		return;

	CString strName = _T("dest");
	RECT rc = GetAttributeRect(strName);
	rc.right = rc.left + GetIntPara(IDC_EDIT_DEST_WIDTH);
	UpdatePara(rc, strName);
}

void CImageEditorAdjust::OnChangeEditDestHeight()
{
	if(!m_bDoEditChanged)
		return;

	CString strName = _T("dest");
	RECT rc = GetAttributeRect(strName);
	rc.bottom = rc.top + GetIntPara(IDC_EDIT_DEST_HEIGHT);
	UpdatePara(rc, strName);
}

void CImageEditorAdjust::OnChangeEditSourceWidth()
{
	if(!m_bDoEditChanged)
		return;

	CString strName = _T("source");
	RECT rc = GetAttributeRect(strName);
	rc.right = rc.left + GetIntPara(IDC_EDIT_SOURCE_WIDTH);
	UpdatePara(rc, strName);
}

void CImageEditorAdjust::OnChangeEditSourceHeight()
{
	if(!m_bDoEditChanged)
		return;

	CString strName = _T("source");
	RECT rc = GetAttributeRect(strName);
	rc.bottom = rc.top + GetIntPara(IDC_EDIT_SOURCE_HEIGHT);
	UpdatePara(rc, strName);
}

void CImageEditorAdjust::OnChangeEditCornerWidth()
{
	if(!m_bDoEditChanged)
		return;

	CString strName = _T("corner");
	RECT rc = GetAttributeRect(strName);
	rc.right = rc.left + GetIntPara(IDC_EDIT_CORNER_WIDTH);
	UpdatePara(rc, strName);
}

void CImageEditorAdjust::OnChangeEditCornerHeight()
{
	if(!m_bDoEditChanged)
		return;

	CString strName = _T("corner");
	RECT rc = GetAttributeRect(strName);
	rc.bottom = rc.top + GetIntPara(IDC_EDIT_CORNER_HEIGHT);
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonDestLeft()
{
	CString strName = _T("dest");
	RECT rc = GetAttributeRect(strName);
	rc.left -= 1;
	rc.right -= 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonDestRight()
{
	CString strName = _T("dest");
	RECT rc = GetAttributeRect(strName);
	rc.left += 1;
	rc.right += 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonDestTop()
{
	CString strName = _T("dest");
	RECT rc = GetAttributeRect(strName);
	rc.top -= 1;
	rc.bottom -= 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonDestBottom()
{
	CString strName = _T("dest");
	RECT rc = GetAttributeRect(strName);
	rc.top += 1;
	rc.bottom += 1;
	UpdatePara(rc, strName);
}

//////////////////////////////////////////////////////////////////////////

void CImageEditorAdjust::OnBnClickedButtonSourceLeft()
{
	CString strName = _T("source");
	RECT rc = GetAttributeRect(strName);
	rc.left -= 1;
	rc.right -= 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonSourceRight()
{
	CString strName = _T("source");
	RECT rc = GetAttributeRect(strName);
	rc.left += 1;
	rc.right += 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonSourceTop()
{
	CString strName = _T("source");
	RECT rc = GetAttributeRect(strName);
	rc.top -= 1;
	rc.bottom -= 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonSourceBottom()
{
	CString strName = _T("source");
	RECT rc = GetAttributeRect(strName);
	rc.top += 1;
	rc.bottom += 1;
	UpdatePara(rc, strName);
}
//////////////////////////////////////////////////////////////////////////

void CImageEditorAdjust::OnBnClickedButtonCornerLeft()
{
	CString strName = _T("corner");
	RECT rc = GetAttributeRect(strName);
	rc.left -= 1;
	rc.right -= 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonCornerRight()
{
	CString strName = _T("corner");
	RECT rc = GetAttributeRect(strName);
	rc.left += 1;
	rc.right += 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonCornerTop()
{
	CString strName = _T("corner");
	RECT rc = GetAttributeRect(strName);
	rc.top -= 1;
	rc.bottom -= 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonCornerBottom()
{
	CString strName = _T("corner");
	RECT rc = GetAttributeRect(strName);
	rc.top += 1;
	rc.bottom += 1;
	UpdatePara(rc, strName);
}


void CImageEditorAdjust::OnBnClickedButtonDestReset()
{
	CString strName = _T("dest");
	RECT rc = GetAttributeRect(strName);
	rc.left = 0;
	rc.top = 0;
	rc.right = 0;
	rc.bottom = 0;
	UpdatePara(rc, strName);
	InitData();
}


void CImageEditorAdjust::OnBnClickedButtonSourceReset()
{
	CString strName = _T("source");
	RECT rc = GetAttributeRect(strName);
	rc.left = 0;
	rc.top = 0;
	rc.right = 0;
	rc.bottom = 0;
	UpdatePara(rc, strName);
	InitData();
}


void CImageEditorAdjust::OnBnClickedButtonCornerReset()
{
	CString strName = _T("corner");
	RECT rc = GetAttributeRect(strName);
	rc.left = 0;
	rc.top = 0;
	rc.right = 0;
	rc.bottom = 0;
	UpdatePara(rc, strName);
	InitData();
}
