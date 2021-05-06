// UIPropertyGridCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "UIPropertyGridCtrl.h"

#include "MainFrm.h"
#include "ImageEditor.h"
#include "DefaultEditor.h"
#include "DlgStringEditor.h"
#include "UIManager.h"

//////////////////////////////////////////////////////////////////////////
// CMFCPropertyGridImageProperty
IMPLEMENT_DYNAMIC(CUIPropertyGridStringProperty, CMFCPropertyGridProperty)
	CUIPropertyGridStringProperty::CUIPropertyGridStringProperty(const CString& strName, const CString &strValue, LPCTSTR lpszDescr, DWORD_PTR dwData)
	: CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)strValue), lpszDescr, dwData)
{
	m_dwFlags = 0x0002;
}

void CUIPropertyGridStringProperty::OnClickButton(CPoint point)
{	
	m_bButtonIsDown = TRUE;
	Redraw();

	CString strOldValue = COLE2CT(GetValue().bstrVal);
	CDlgStringEditor dlg;
//	dlg.m_pMainProp = this;
	dlg.m_string = strOldValue;
	if(dlg.DoModal() == IDOK)
	{
		SetValue(_variant_t(dlg.m_string));
	}
	else
	{
		//SetValue((_variant_t)strOldValue);
		//m_pWndList->OnPropertyChanged(this);
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL) m_pWndInPlace->SetFocus();
	else m_pWndList->SetFocus();
	
}

//////////////////////////////////////////////////////////////////////////
CUIPropertyGridColorProperty::CUIPropertyGridColorProperty(const CString& strName, const COLORREF& color,CPalette* pPalette/*=NULL*/,LPCTSTR lpszDescr/*=NULL*/,DWORD_PTR dwData/*=0*/)
	:CMFCPropertyGridColorProperty(strName,color,pPalette,lpszDescr,dwData)
{
}

void CUIPropertyGridColorProperty::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height();
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height();
	rectColor.DeflateRect(1, 1);
	rectColor.top++;
	rectColor.left++;

	CBrush br(m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color);
	pDC->FillRect(rectColor, &br);
	pDC->Draw3dRect(rectColor, 0, 0);
}

BOOL CUIPropertyGridColorProperty::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);

	COLORREF colorCurr = m_Color;

	if (!strText.IsEmpty())
	{
		int nR = 0, nG = 0, nB = 0;
		_stscanf_s(strText, _T("%2x%2x%2x"), &nR, &nG, &nB);
		m_Color = RGB(nR, nG, nB);
	}

	if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged(this);
	}

	return TRUE;
}

void CUIPropertyGridColorProperty::SetUIColor(LPCTSTR szrColor)
{
	if(CompareString(_T("0x00000000"), szrColor))
	{
		m_Color = (COLORREF)-1;
		return;
	}

	CString strColor = szrColor;
	if(strColor.Find(_T("#")) >= 0)
	{
		strColor.Delete(0,1);
	}
	else if(strColor.Find(_T("0x")) >= 0)
	{
		strColor.Delete(0, 2);
	}

	CString strColor2 = strColor.Mid(6,2) + strColor.Mid(4,2) + strColor.Mid(2, 2);
	LPTSTR pstr = NULL;
	m_Color = _tcstoul(strColor2, &pstr, 16);
}

CString CUIPropertyGridColorProperty::GetUIColor()
{
	CString strRet;

	if(m_Color == (COLORREF)-1)
	{
		strRet =_T("0x00000000");
	}
	else
	{
		strRet.Format(_T("0xFF%02X%02X%02X"), (m_Color) & 0x00FF, (m_Color>>8) & 0x00FF, (m_Color>>16) & 0x00FF);
	}

	return strRet;
}

// CMFCPropertyGridImageProperty
IMPLEMENT_DYNAMIC(CUIPropertyGridImageProperty, CMFCPropertyGridProperty)
CUIPropertyGridImageProperty::CUIPropertyGridImageProperty(const CString& strName, const CString &strValue, LPCTSTR lpszDescr, DWORD_PTR dwData)
	: CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)strValue), lpszDescr, dwData)
{
	m_dwFlags = 0x0002;
}

void CUIPropertyGridImageProperty::OnClickButton(CPoint point)
{
	m_bButtonIsDown = TRUE;
	Redraw();

	CString strOldValue = COLE2CT(GetValue().bstrVal);
	CImageEditor dlg;
	dlg.SetUIManager(((CMainFrame *)AfxGetMainWnd())->GetActiveUIManager());
	dlg.m_pParentGrid = m_pWndList;
	dlg.m_pParentProp = this;
	dlg.SetAttributeValue(strOldValue);

	//获取一张图片，作为图片编辑的背景层
	xml_node node = ((CUIPropertyGridCtrl *)m_pWndList)->GetXmlNode();
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(pControl)
	{
		//获取图片之前，先给控件设置空图片再拍照
		pControl->SetAttribute(GetName(), _T(""));

		CControlUI *pRoot = pControl->GetManager()->GetRoot();
		CSize szForm = pControl->GetManager()->GetInitSize();
		CRect rcControl = pControl->GetPos();

		//给整个设计视图拍照
		CImage img;
		img.Create(szForm.cx, szForm.cy, 32);
		CRect rcPaint(0,0,szForm.cx,szForm.cy);
		pRoot->DoPaint(img.GetDC(), rcPaint, NULL);
		img.ReleaseDC();

		//裁剪出pControl那部分的区域
		CImage img2;
		img2.Create(rcControl.Width(), rcControl.Height(), img.GetBPP());
		img.BitBlt(img2.GetDC(), 0, 0, rcControl.Width(), rcControl.Height(), rcControl.left , rcControl.top, SRCCOPY);
		img2.ReleaseDC();

		//裁剪结果给图像编辑器使用
		dlg.SetControlImage(img2.Detach(), rcControl);

		//获取图片之后，先给控件的图片设置回去
		pControl->SetAttribute(GetName(), (LPCTSTR)GetValue().bstrVal);
	}

	if(dlg.DoModal() == IDOK)
	{
		SetValue(_variant_t(dlg.GetAttributeValue()));
	}
	else
	{
		if(dlg.GetAttributeValue() != strOldValue)
		{
			SetValue((_variant_t)strOldValue);
			m_pWndList->OnPropertyChanged(this);
		}
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL) m_pWndInPlace->SetFocus();
	else m_pWndList->SetFocus();
}

//////////////////////////////////////////////////////////////////////////
// CUIPropertyGridAttributeListProperty
IMPLEMENT_DYNAMIC(CUIPropertyGridAttributeListProperty, CMFCPropertyGridProperty)
CUIPropertyGridAttributeListProperty::CUIPropertyGridAttributeListProperty(const CString& strName, const CString &strValue, LPCTSTR lpszDescr, DWORD_PTR dwData)
	: CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)strValue), lpszDescr, dwData)
{
	m_dwFlags = 0x0002;
}

void CUIPropertyGridAttributeListProperty::OnClickButton(CPoint point)
{
	m_bButtonIsDown = TRUE;
	Redraw();

	m_strClassName.Empty();
	CUIPropertyGridCtrl *pGridCtrl = (CUIPropertyGridCtrl *)m_pWndList;
	if(g_duiProp.IsDefaultNode(pGridCtrl->m_TreeNode))
		m_strClassName = pGridCtrl->m_TreeNode.attribute(XTEXT("name")).value();
	else if(g_duiProp.IsStyleNode(pGridCtrl->m_TreeNode))
		m_strClassName = pGridCtrl->m_TreeNode.attribute(XTEXT("class")).value();
	else
	{
		xml_node nodeAttr = g_duiProp.FindAttribute(XML2T(pGridCtrl->m_TreeNode.name()), GetName());
		if(nodeAttr)
		{
			m_strClassName = nodeAttr.attribute(XTEXT("value")).value();
		}
	}
	/*
	//这里要进行单独判断, TreeViewUI的几个属性
	else if(CompareString(GetName(), _T("checkboxattr")))
	{
		m_strClassName = _T("Option");
	}
	else if(CompareString(GetName(), _T("folderattr")))
	{
		m_strClassName = _T("Option");
	}
	else if(CompareString(GetName(), _T("dotlineattr")))
	{
		m_strClassName = _T("Label");
	}
	else if(CompareString(GetName(), _T("horizattr")))
	{
		m_strClassName = _T("HorizontalLayout");
	}
	else if(CompareString(GetName(), _T("dropbox"))) //combo的dropbox属性
	{
		m_strClassName = _T("VerticalLayout");
	}
	else if(CompareString(GetName(), _T("vscrollbarstyle")))
	{
		m_strClassName = _T("VScrollBar");
	}
	else if(CompareString(GetName(), _T("hscrollbarstyle")))
	{
		m_strClassName = _T("HScrollBar");
	}
	else if(CompareString(GetName(), _T("datelabelstyle")))
	{
		m_strClassName = _T("Label");
	}
	else if(CompareString(GetName(), _T("timelabelstyle")))
	{
		m_strClassName = _T("Label");
	}
	*/
	if(m_strClassName.IsEmpty())
		return;

	CDefaultEditor dlg;
	dlg.SetAttributeValue(m_strClassName, COLE2CT(GetValue().bstrVal));
	if(dlg.DoModal() == IDOK)
	{
		SetValue((_variant_t)(LPCTSTR)dlg.GetAttributeValue());
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL) m_pWndInPlace->SetFocus();
	else m_pWndList->SetFocus();
}
//////////////////////////////////////////////////////////////////////////
//CUIPropertyGridFontProperty
IMPLEMENT_DYNAMIC(CUIPropertyGridFontProperty, CMFCPropertyGridFontProperty)

CUIPropertyGridFontProperty::CUIPropertyGridFontProperty(const CString& strName, LOGFONT& lf, DWORD dwFontDialogFlags, LPCTSTR lpszDescr, DWORD_PTR dwData, COLORREF color) 
	: CMFCPropertyGridFontProperty(strName, lf, dwFontDialogFlags, lpszDescr, dwData)
{
	m_bOpenDialog = FALSE;
}

void CUIPropertyGridFontProperty::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	LOGFONT lfPrev = m_lf;
	COLORREF nColorPrev = m_Color;

	m_bButtonIsDown = TRUE;
	Redraw();
	
	static_cast<CUIPropertyGridCtrl* >(m_pWndList)->OnOpenFontDialog(this);

#pragma warning(disable : 4244)

	CFontDialog dlg(&m_lf, m_dwFontDialogFlags, NULL, m_pWndList);

#pragma warning(default : 4244)

	if (m_Color != (COLORREF)-1)
	{
		dlg.m_cf.rgbColors = m_Color;
	}

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&m_lf);
		m_Color = dlg.GetColor();

		if (memcmp(&lfPrev, &m_lf, sizeof(lfPrev) - sizeof(lfPrev.lfFaceName)) || _tcscmp( lfPrev.lfFaceName, m_lf.lfFaceName) || nColorPrev != m_Color)
		{
			m_bOpenDialog = TRUE;
			m_pWndList->OnPropertyChanged(this);
			m_bOpenDialog = FALSE;
		}

		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText((LPCTSTR)m_lf.lfFaceName);
		}
		else
		{
			m_varValue = (LPCTSTR) m_lf.lfFaceName;
		}
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}

	m_bButtonIsDown = FALSE;
	Redraw();
}

CString CUIPropertyGridFontProperty::FormatProperty()
{
	return CMFCPropertyGridProperty::FormatProperty();
}

void CUIPropertyGridFontProperty::SetFontName(LPCTSTR lfFaceName)
{
	_tcscpy_s(m_lf.lfFaceName, LF_FACESIZE, lfFaceName);
	SetValue(_variant_t((LPCTSTR)m_lf.lfFaceName));
}

LPCTSTR CUIPropertyGridFontProperty::GetFontName()
{
	return m_lf.lfFaceName;
}

void CUIPropertyGridFontProperty::SetFontSize(int size)
{
	CWindowDC dc(m_pWndList);

	int nLogY = dc.GetDeviceCaps(LOGPIXELSY);
	if (nLogY != 0)
	{
		m_lf.lfHeight = nLogY*size/72;
	}
}

int CUIPropertyGridFontProperty::GetFontSize()
{
	CWindowDC dc(m_pWndList);

	int nLogY = dc.GetDeviceCaps(LOGPIXELSY);
	if (nLogY != 0)
	{
		return MulDiv(72, -m_lf.lfHeight, nLogY);
	}

	return 9;
}

void CUIPropertyGridFontProperty::SetBold(BOOL bTrue)
{	
	m_lf.lfWeight		= bTrue	?	700 : 0;
}

BOOL CUIPropertyGridFontProperty::GetBold()
{
	return m_lf.lfWeight >= 700;
}

void CUIPropertyGridFontProperty::SetUnderline(BOOL bTrue)
{
	m_lf.lfUnderline	= bTrue	?	1	: 0;
}

BOOL CUIPropertyGridFontProperty::GetUnderline()
{
	return m_lf.lfUnderline == 1;
}

void CUIPropertyGridFontProperty::SetItalic(BOOL bTrue)
{
	m_lf.lfItalic	= bTrue	?	255	: 0;
}

BOOL CUIPropertyGridFontProperty::GetItalic()
{
	return m_lf.lfItalic == 255;
}

//////////////////////////////////////////////////////////////////////////

// CUIPropertyGridCtrl

IMPLEMENT_DYNAMIC(CUIPropertyGridCtrl, CMFCPropertyGridCtrl)

CUIPropertyGridCtrl::CUIPropertyGridCtrl()
{
	m_pUIManager = NULL;
	m_pPropertyWnd = NULL;
	m_pPropFocused = NULL;
}

CUIPropertyGridCtrl::~CUIPropertyGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CUIPropertyGridCtrl, CMFCPropertyGridCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GRIDPROPERTY_COPY_VALUE, &CUIPropertyGridCtrl::OnGridpropertyCopyValue)
	ON_COMMAND(ID_GRIDPROPERTY_COPY_VALUE_EX, &CUIPropertyGridCtrl::OnGridpropertyCopyValueEx)
	ON_UPDATE_COMMAND_UI(ID_GRIDPROPERTY_COPY_VALUE, &CUIPropertyGridCtrl::OnUpdateGridpropertyCopyValue)
	ON_UPDATE_COMMAND_UI(ID_GRIDPROPERTY_COPY_VALUE_EX, &CUIPropertyGridCtrl::OnUpdateGridpropertyCopyValueEx)
	ON_COMMAND(ID_GRIDPROPERTY_SET_DEFAULT_VALUE, &CUIPropertyGridCtrl::OnGridpropertySetDefaultValue)
	ON_UPDATE_COMMAND_UI(ID_GRIDPROPERTY_SET_DEFAULT_VALUE, &CUIPropertyGridCtrl::OnUpdateGridpropertySetDefaultValue)
END_MESSAGE_MAP()



// CUIPropertyGridCtrl 消息处理程序
CDuiEditorViewDesign *CUIPropertyGridCtrl::GetView() const
{
	if(!GetUIManager()) return NULL;
	return GetUIManager()->GetDesignView();
}

void CUIPropertyGridCtrl::InitProp(xml_node TreeNode)
{
	//InsertMsg(_T("CUIPropertyGridCtrl::InitProp(xml_node TreeNode)"));
	CLockWindowUpdate lockUpdate(this);
	RemoveAll();

	CString strText = _T("属性 -- ");
	strText += XML2T(TreeNode.name());
	m_pPropertyWnd->SetWindowText(strText);

	m_pPropertyWnd->m_wndFindText.GetWindowText(m_strFilter);

	m_TreeNode = TreeNode;
	pugi::xml_node root = g_duiProp.GetRoot();
	if(!root)
	{
		AfxMessageBox(_T("读取属性列表失败"));
		return;
	}

	//当前控件属性
	xml_node nodePropertyControl = g_duiProp.FindControl(XML2T(m_TreeNode.name()));
	if(nodePropertyControl)
	{
		_initProp(nodePropertyControl);
	}
	else //在duiprop找不到, 是不是自定义控件?
	{
		xml_attribute attrCustom = m_TreeNode.attribute(XTEXT("custombasedfrom"));
		if(attrCustom)
		{
			nodePropertyControl = g_duiProp.FindControl(XML2T(attrCustom.value()));
			_initProp(nodePropertyControl);
		}
	}

	//载入父控件属性
	CString ParentClassName = XML2T(nodePropertyControl.attribute(XTEXT("parent")).value());
	for(;;)
	{
		if(ParentClassName.IsEmpty())
			break;

		xml_node nodeParent = g_duiProp.FindControl(ParentClassName);
		if(!nodeParent)	break;

		_initProp(nodeParent);

		ParentClassName = XML2T(nodeParent.attribute(XTEXT("parent")).value());
	}
		
	return;
}

void CUIPropertyGridCtrl::_initProp(xml_node nodeProperty)
{
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(XML2T(nodeProperty.name()));
	AddProperty(pGroup1);

	for (pugi::xml_node nodeChild = nodeProperty.first_child(); nodeChild; nodeChild = nodeChild.next_sibling())
	{
		if(CompareString(nodeChild.name(), _T("Attribute")))
		{
			//过滤已经添加的属性
			if(_isRepeatProperty(XML2T(nodeChild.attribute(XTEXT("name")).value())))
				continue;

			CString strAttr = XML2T(nodeChild.attribute(XTEXT("name")).value());
			if(strAttr.Find(m_strFilter) >= 0)
			{
				//CString str = nodeChild.attribute(_T("name")).value();
				//OutputDebugString(str);
				InsertDuiLibProperty(m_TreeNode, nodeChild, pGroup1);
			}
		}
	}
	pGroup1->Expand(FALSE);
	pGroup1->Expand(TRUE);		
}

BOOL CUIPropertyGridCtrl::_isRepeatProperty(LPCTSTR propName)
{
	for (int i=0; i<GetPropertyCount(); i++)
	{
		CMFCPropertyGridProperty *pProp1 = GetProperty(i);
		for (int j=0; j<pProp1->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty *pProp2 = pProp1->GetSubItem(j);
			if(CompareString(pProp2->GetName(), propName))
				return TRUE;
		}
	}
	return FALSE;
}

CMFCPropertyGridProperty *CUIPropertyGridCtrl::GetAttributeCell(LPCTSTR attrName)
{
	for (int i=0; i<GetPropertyCount(); i++)
	{
		CMFCPropertyGridProperty *pProp1 = GetProperty(i);
		for (int j=0; j<pProp1->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty *pProp2 = pProp1->GetSubItem(j);
			if(CompareString(pProp2->GetName(), attrName))
				return pProp2;
		}
	}
	return NULL;
}

void CUIPropertyGridCtrl::InsertDuiLibProperty(xml_node TreeNode, xml_node attrNode, CMFCPropertyGridProperty* pGroupParent)
{
	xml_attribute attrName = attrNode.attribute(XTEXT("name"));
	xml_attribute attrDefValue = attrNode.attribute(XTEXT("default"));	
	xml_attribute attrComment = attrNode.attribute(XTEXT("comment"));
	xml_attribute attrType = attrNode.attribute(XTEXT("type"));
	xml_attribute attrSpanValue = attrNode.attribute(XTEXT("value"));

	CMFCPropertyGridProperty *pProperty = NULL;

	//Font定义的name字段, 特殊处理
	if(g_duiProp.IsFontNode(TreeNode) && CompareString(attrName.value(), _T("name")) && CompareString(attrType.value(), _T("STRING")))
	{
		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf); 
		CUIPropertyGridFontProperty *pfntProp;
		pfntProp = new CUIPropertyGridFontProperty(XML2T(attrName.value()), lf, CF_EFFECTS | CF_SCREENFONTS, XML2T(attrComment.value()));
		pfntProp->SetData((DWORD)attrNode.internal_object());
		pfntProp->AllowEdit(TRUE);
		pGroupParent->AddSubItem(pfntProp);	

		xml_attribute attrName = TreeNode.attribute(XTEXT("name"));
		if(!attrName)
		{
			attrName = g_duiProp.AddAttribute(TreeNode, _T("name"), _T("宋体"), GetUIManager());
		}
		pfntProp->SetFontName(XML2T(attrName.value()));
		return;
	}

	//style字段,列出CPaintManager里面有的 
	if(CompareString(attrName.value(), _T("style")))
	{
		pProperty = new CMFCPropertyGridProperty(XML2T(attrName.value()), (_variant_t)XML2T(attrDefValue.value()),  XML2T(attrComment.value()));
		pProperty->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pProperty);

		if(GetUIManager())
		{
			CPaintManagerUI *pManager = GetUIManager()->GetManager();
			for (int i=0; i<pManager->GetStyles(false).GetSize(); i++)
			{
				pProperty->AddOption(pManager->GetStyles(false).GetAt(i));
			}
			for (int i=0; i<pManager->GetStyles(true).GetSize(); i++)
			{
				pProperty->AddOption(pManager->GetStyles(true).GetAt(i));
			}
		}
		
		pProperty->AllowEdit(TRUE);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
			pProperty->SetValue((_variant_t)XML2T(attr.value()));
		else
			pProperty->SetValue((_variant_t)XML2T(attrDefValue.value()));

		pProperty->SetOriginalValue(pProperty->GetValue());	
		return;
	}

	if(CompareString(attrType.value(), _T("INT")))
	{
		pProperty = new CMFCPropertyGridProperty(XML2T(attrName.value()), _variant_t((long)attrDefValue.as_int(), VT_I4),  XML2T(attrComment.value()));
		pProperty->SetData((DWORD)attrNode.internal_object());
		pProperty->AllowEdit(TRUE);
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pGroupParent->AddSubItem(pProperty);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			pProperty->SetValue(_variant_t((long)attr.as_int(), VT_I4));
		}
	}
	else if(CompareString(attrType.value(), _T("DWORD")))
	{
		CUIPropertyGridColorProperty* pColorProp = new CUIPropertyGridColorProperty(XML2T(attrName.value()), (_variant_t)0,  NULL, XML2T(attrComment.value()));
		pColorProp->SetData((DWORD)attrNode.internal_object());
		pColorProp->EnableOtherButton(_T("其他..."));
		pColorProp->EnableAutomaticButton(_T("transparent"), RGB(255,255,255), TRUE);
		pColorProp->SetUIColor(XML2T(attrDefValue.value()));
		pGroupParent->AddSubItem(pColorProp);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			pColorProp->SetUIColor(XML2T(attr.value()));
		}
	}
	else if(CompareString(attrType.value(), _T("STRING")))
	{
		if(attrSpanValue)	//STRING类型是否有value取值属性
			pProperty = new CMFCPropertyGridProperty(XML2T(attrName.value()), (_variant_t)XML2T(attrDefValue.value()),  XML2T(attrComment.value()));
		else
			pProperty = new CUIPropertyGridStringProperty(XML2T(attrName.value()), (_variant_t)XML2T(attrDefValue.value()),  XML2T(attrComment.value()));
		pProperty->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pProperty);

		if(attrSpanValue)	//STRING类型是否有value取值属性
		{
			CStringArray arrString;
			SplitCString(arrString, XML2T(attrSpanValue.value()));
			for (int i=0; i<arrString.GetSize(); i++)
			{
				pProperty->AddOption(arrString[i]);
			}
			pProperty->AllowEdit(FALSE);
		}
		else
		{
			pProperty->AllowEdit(TRUE);
		}

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
			pProperty->SetValue((_variant_t)XML2T(attr.value()));
		else
			pProperty->SetValue((_variant_t)attrDefValue.value());
		pProperty->SetOriginalValue(pProperty->GetValue());		
	}
	else if(CompareString(attrType.value(), _T("BOOL")))
	{
		bool bValue = attrDefValue.as_bool();
		pProperty = new CMFCPropertyGridProperty(XML2T(attrName.value()), (_variant_t)bValue,  XML2T(attrComment.value()));
		pProperty->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pProperty);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			pProperty->SetValue((_variant_t)attr.as_bool());
		}
	}
	else if(CompareString(attrType.value(), _T("SIZE")))
	{
		CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(XML2T(attrName.value()), (_variant_t)0, TRUE);
		pSize->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pSize);

		CStringArray strArrDef;
		SplitCString(strArrDef, XML2T(attrDefValue.value()));

		int nWidth = 0;
		int nHeight = 0;
		if(strArrDef.GetSize() > 0) nWidth = _tstoi(strArrDef[0]);
		if(strArrDef.GetSize() > 1) nHeight = _tstoi(strArrDef[1]);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			CStringArray strArray;
			SplitCString(strArray, XML2T(attr.value()));
			if(strArray.GetSize() > 0) nWidth = _tstoi(strArray[0]);
			if(strArray.GetSize() > 1) nHeight = _tstoi(strArray[1]);
		}

		pProperty = new CMFCPropertyGridProperty(_T("width"), _variant_t((long)nWidth, VT_I4), XML2T(attrComment.value()));
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pSize->AddSubItem(pProperty);
 
		pProperty = new CMFCPropertyGridProperty( _T("height"), _variant_t((long)nHeight, VT_I4), XML2T(attrComment.value()));
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pSize->AddSubItem(pProperty);
	}
	else if(CompareString(attrType.value(), _T("RECT")))
	{
		CMFCPropertyGridProperty* pRect = new CMFCPropertyGridProperty(XML2T(attrName.value()), attrDefValue.as_int(), TRUE);
		pRect->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pRect);

		CStringArray strArrDef;
		SplitCString(strArrDef, XML2T(attrDefValue.value()));

		int nLeft = 0;
		int nTop = 0;
		int nRight = 0;
		int nBottom = 0;
		if(strArrDef.GetSize() > 0) nLeft = _tstoi(strArrDef[0]);
		if(strArrDef.GetSize() > 1) nTop = _tstoi(strArrDef[1]);
		if(strArrDef.GetSize() > 2) nRight = _tstoi(strArrDef[2]);
		if(strArrDef.GetSize() > 3) nBottom = _tstoi(strArrDef[3]);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			CStringArray strArray;
			SplitCString(strArray, XML2T(attr.value()));
			if(strArray.GetSize() > 0) nLeft = _tstoi(strArray[0]);
			if(strArray.GetSize() > 1) nTop = _tstoi(strArray[1]);
			if(strArray.GetSize() > 2) nRight = _tstoi(strArray[2]);
			if(strArray.GetSize() > 3) nBottom = _tstoi(strArray[3]);
		}

		pProperty = new CMFCPropertyGridProperty(_T("left"), _variant_t((long)nLeft, VT_I4), XML2T(attrComment.value()));
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

		pProperty = new CMFCPropertyGridProperty( _T("top"), _variant_t((long)nTop, VT_I4), XML2T(attrComment.value()));
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

		pProperty = new CMFCPropertyGridProperty( _T("right"), _variant_t((long)nRight, VT_I4), XML2T(attrComment.value()));
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

		pProperty = new CMFCPropertyGridProperty( _T("bottom"), _variant_t((long)nBottom, VT_I4), XML2T(attrComment.value()));
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

	}
	else if(CompareString(attrType.value(), _T("CHAR")))
	{
		pProperty = new CMFCPropertyGridProperty(XML2T(attrName.value()), (_variant_t)attrDefValue.value(),  XML2T(attrComment.value()));
		pProperty->SetData((DWORD)attrNode.internal_object());
		pProperty->AllowEdit(TRUE);
		pGroupParent->AddSubItem(pProperty);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			pProperty->SetValue((_variant_t)attr.value());
		}
	}
	else if(CompareString(attrType.value(), _T("IMAGE")))
	{
		xml_attribute attr = TreeNode.attribute(attrName.value());
		CUIPropertyGridImageProperty* pImage = new CUIPropertyGridImageProperty(XML2T(attrName.value()), XML2T(attr.value()));
		pImage->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pImage);
	}
	else if(CompareString(attrType.value(), _T("ATTRIBUTELIST")))
	{
		xml_attribute attr = TreeNode.attribute(attrName.value());
		CUIPropertyGridAttributeListProperty* pImage = new CUIPropertyGridAttributeListProperty(XML2T(attrName.value()), XML2T(attr.value()));
		pImage->SetDescription(XML2T(attrComment.value()));
		pImage->SetData((DWORD)attrNode.internal_object());

		if(g_duiProp.IsDefaultNode(TreeNode))
			pImage->m_strClassName = TreeNode.attribute(XTEXT("name")).value();
		else if(g_duiProp.IsStyleNode(TreeNode))
			pImage->m_strClassName = TreeNode.attribute(XTEXT("class")).value();
		else if(attrSpanValue)
		{
			pImage->m_strClassName = attrSpanValue.value();
		}

		pGroupParent->AddSubItem(pImage);
	}
}

void CUIPropertyGridCtrl::OnOpenFontDialog(CUIPropertyGridFontProperty *pfntProp)
{
	ASSERT(g_duiProp.IsFontNode(m_TreeNode));

	pfntProp->SetFontName(XML2T(m_TreeNode.attribute(XTEXT("name")).as_string(XTEXT("宋体"))));
	pfntProp->SetBold(m_TreeNode.attribute(XTEXT("bold")).as_bool());
	pfntProp->SetUnderline(m_TreeNode.attribute(XTEXT("underline")).as_bool());
	pfntProp->SetItalic(m_TreeNode.attribute(XTEXT("italic")).as_bool());
}

void CUIPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	if(GetUIManager())
		CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());

	int nLevel = pProp->GetHierarchyLevel();

	if(nLevel > 1) //修改子项 第2层, size rect 等参数
	{
		pProp = pProp->GetParent();
	}

	if(pProp->GetSubItemsCount() > 0) //有子项, 直接修改父项
	{
		CString str, temp;
		for (int i=0; i<pProp->GetSubItemsCount(); i++)
		{
			if(!str.IsEmpty())
				str += _T(",");
			temp.Format(_T("%d"), pProp->GetSubItem(i)->GetValue().iVal);
			str += temp;
		}

		pProp->SetValue((_variant_t)str);
	}

	xml_node duiNode((xml_node_struct *)pProp->GetData()); //属性文件
	xml_attribute attrName		= duiNode.attribute(XTEXT("name"));
	xml_attribute attrDefValue	= duiNode.attribute(XTEXT("default"));	
	xml_attribute attrComment	= duiNode.attribute(XTEXT("comment"));
	xml_attribute attrType		= duiNode.attribute(XTEXT("type"));
	xml_attribute attrSpanValue = duiNode.attribute(XTEXT("value"));

	xml_attribute attrTree;
	
	if(pProp->GetValue().vt == VT_EMPTY || pProp->GetValue().vt == VT_NULL)
		return;

	//特别处理Style的修改
	if( CompareString(m_TreeNode.name(), _T("Style")) )
	{
		if( CompareString(attrName.value(), _T("shared")) )
		{
			//修改Style的shared属性, 应该把原来的remove掉
			BOOL bShared = m_TreeNode.attribute(XTEXT("shared")).as_bool();
			BOOL bSharedNew = pProp->GetValue().boolVal != 0;
			CDuiEditorViewDesign *pView = GetView();
			if(pView)
			{
				CString strStyleName = XML2T(m_TreeNode.attribute(XTEXT("name")).as_string());
				bool bShared = m_TreeNode.attribute(XTEXT("shared")).as_bool();
				pView->GetUIManager()->GetManager()->RemoveStyle(strStyleName, bShared);
			}
		}
	}

	if(CompareString(attrType.value(), _T("INT")))
	{
		//是否设置resourceid
		if(GetUIManager())
		{
			if(CompareString(attrName.value(), _T("resourceid")) && g_duiProp.IsControlUI(m_TreeNode))
			{
				xml_node nodeDoc = GetUIManager()->GetDocument()->m_doc.root().child(XTEXT("Window"));
				if(IsRepeatResourceID(nodeDoc, pProp->GetValue().intVal))
				{
					AfxMessageBox(_T("已存在相同的resourceid!"));
					pProp->SetValue(pProp->GetOriginalValue());
					return;
				}
			}
		}

		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), pProp->GetValue().intVal, GetUIManager());
	}
	else if(CompareString(attrType.value(), _T("DWORD")))
	{
		CUIPropertyGridColorProperty *pPropColor = (CUIPropertyGridColorProperty *)pProp;
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), pPropColor->GetUIColor(), GetUIManager());
	}
	else if(CompareString(attrType.value(), _T("STRING")))
	{
		//Font定义的name字段, 特殊处理
		if(g_duiProp.IsFontNode(m_TreeNode) && CompareString(attrName.value(), _T("name")))
		{
			CUIPropertyGridFontProperty *pFontProp = static_cast<CUIPropertyGridFontProperty *>(pProp);
			ASSERT(pFontProp);
			if(pFontProp->m_bOpenDialog)
				OnPropertyFontChangedFromFontDialog(pFontProp);
		}

		//是否设置Control的name?
		if(GetUIManager())
		{
			if(CompareString(attrName.value(), _T("name")) && g_duiProp.IsControlUI(m_TreeNode))
			{
				CDuiEditorViewDesign *pView = GetUIManager()->GetDesignView();
				xml_node nodeDoc = pView->GetDocument()->m_doc.root().child(XTEXT("Window"));
				if(IsRepeatName(nodeDoc, (LPCTSTR)pProp->GetValue().bstrVal))
				{
					AfxMessageBox(_T("已存在相同的name属性控件!"));
					pProp->SetValue(pProp->GetOriginalValue());
					return;
				}
			}
		}
		
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), (LPCTSTR)pProp->GetValue().bstrVal, GetUIManager());
		pProp->SetOriginalValue(pProp->GetValue());
	}
	else if(CompareString(attrType.value(), _T("BOOL")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), pProp->GetValue().boolVal != 0, GetUIManager());
	}
	else if(CompareString(attrType.value(), _T("SIZE")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), (LPCTSTR)pProp->GetValue().bstrVal, GetUIManager());
	}
	else if(CompareString(attrType.value(), _T("RECT")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), (LPCTSTR)pProp->GetValue().bstrVal, GetUIManager());
	}
	else if(CompareString(attrType.value(), _T("CHAR")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), (LPCTSTR)pProp->GetValue().bstrVal, GetUIManager());
	}
	else if(CompareString(attrType.value(), _T("IMAGE")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), (LPCTSTR)pProp->GetValue().bstrVal, GetUIManager());
	}
	else if(CompareString(attrType.value(), _T("ATTRIBUTELIST")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, XML2T(attrName.value()), (LPCTSTR)pProp->GetValue().bstrVal, GetUIManager());
	}

 	CDuiEditorViewDesign *pView = GetView();
	if(pView)
	{
		pView->GetUIManager()->UpdateControlUI(m_TreeNode, attrTree);
		pView->Invalidate();
	}

	if(GetUIManager() && CompareString(_T("name"), attrTree.name()) )
	{
		GetUIManager()->GetTreeView()->UpdateXmlNode(m_TreeNode);
	}

	g_duiProp.FilterDefaultValue(m_TreeNode, GetUIManager());
	g_duiProp.FilterPosWidthHeight(m_TreeNode, GetUIManager());

	CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
	return;
}

void CUIPropertyGridCtrl::OnPropertyFontChangedFromFontDialog(CUIPropertyGridFontProperty* pProp) const
{
	ASSERT(pProp->m_bOpenDialog);

	for (int i=0; i<GetPropertyCount(); i++)
	{
		CMFCPropertyGridProperty *pProp1 = GetProperty(i);
		for (int j=0; j<pProp1->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty *pProp2 = pProp1->GetSubItem(j);
			CString strName = pProp2->GetName();

			if( CompareString(strName, _T("size")) ) 
			{
				int size = pProp->GetFontSize();
				pProp2->SetValue(_variant_t((long)size, VT_I4));
			}
			else if( CompareString(strName, _T("bold")) )
			{
				bool bold = pProp->GetBold() == TRUE;
				pProp2->SetValue((_variant_t)bold);
			}
			else if( CompareString(strName, _T("underline")) )
			{
				bool underline = pProp->GetUnderline() == TRUE;
				pProp2->SetValue((_variant_t)underline);
			}
			else if( CompareString(strName, _T("italic")) ) 
			{
				bool italic = pProp->GetItalic() == TRUE;
				pProp2->SetValue((_variant_t)italic);
			}
		}
	}
}

BOOL CUIPropertyGridCtrl::IsRepeatName(xml_node nodeDoc, LPCTSTR lpszName) const
{
	xml_attribute attr = nodeDoc.attribute(XTEXT("name"));
	if(attr)
	{
		CString lpszName2 = XML2T(attr.value());
		if(lpszName == lpszName2)
		{
			return TRUE;
		}
	}

	for (pugi::xml_node node = nodeDoc.first_child(); node; node = node.next_sibling())
	{
		if(IsRepeatName(node, lpszName))
			return TRUE;
	}

	return FALSE;
}

BOOL CUIPropertyGridCtrl::IsRepeatResourceID(xml_node nodeDoc, int resID) const
{
	xml_attribute attr = nodeDoc.attribute(XTEXT("resourceid"));
	if(attr)
	{
		if(resID == attr.as_int())
		{
			return TRUE;
		}
	}

	for (pugi::xml_node node = nodeDoc.first_child(); node; node = node.next_sibling())
	{
		if(IsRepeatResourceID(node, resID))
			return TRUE;
	}

	return FALSE;
}

void CUIPropertyGridCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&pt);
	m_pPropFocused = HitTest(pt);//, NULL, TRUE);

	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_GRIDPROPERTY);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, TRUE);
}


void CUIPropertyGridCtrl::OnGridpropertyCopyValue()
{
	CMFCPropertyGridProperty *pProp = m_pPropFocused;

	xml_node duiNode((xml_node_struct *)pProp->GetData()); //属性文件
	xml_attribute attrType		= duiNode.attribute(XTEXT("type"));


	CString strText;
	if(pProp->GetSubItemsCount() > 0) //有子项
	{
		CString str, temp;
		for (int i=0; i<pProp->GetSubItemsCount(); i++)
		{
			if(!str.IsEmpty())
				str += _T(",");
			temp.Format(_T("%d"), pProp->GetSubItem(i)->GetValue().iVal);
			str += temp;
		}
		strText = str;
	}
	else if(CompareString(attrType.value(), _T("INT")))
	{
		strText.Format(_T("%d"), pProp->GetValue().intVal);
	}
	else if(CompareString(attrType.value(), _T("DWORD")))
	{
		CUIPropertyGridColorProperty *pPropColor = (CUIPropertyGridColorProperty *)pProp;
		strText = pPropColor->GetUIColor();
	}
	else if(CompareString(attrType.value(), _T("STRING")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("BOOL")))
	{
		strText = pProp->GetValue().boolVal ? _T("true") : _T("false");
	}
	else if(CompareString(attrType.value(), _T("SIZE")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("RECT")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("CHAR")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("IMAGE")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("ATTRIBUTELIST")))
	{
		strText = pProp->GetValue().bstrVal;
	}

	if(OpenClipboard())
	{
		EmptyClipboard();

		HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE,   (strText.GetLength()+1) * sizeof(TCHAR));
		TCHAR *buffer = (TCHAR *)GlobalLock(clipbuffer);
		_tcscpy(buffer, strText);
		GlobalUnlock(clipbuffer);
#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT, clipbuffer);
#else
		SetClipboardData(CF_TEXT, clipbuffer);
#endif
		CloseClipboard();
	}
}


void CUIPropertyGridCtrl::OnUpdateGridpropertyCopyValue(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pPropFocused!=NULL);
}

void CUIPropertyGridCtrl::OnGridpropertyCopyValueEx()
{
	CMFCPropertyGridProperty *pProp = m_pPropFocused;

	xml_node duiNode((xml_node_struct *)pProp->GetData()); //属性文件
	xml_attribute attrType		= duiNode.attribute(XTEXT("type"));


	CString strText;
	// 	int nLevel = pProp->GetHierarchyLevel();
	// 	if(nLevel > 1)
	// 	{
	// 		pProp = pProp->GetParent();
	// 	}
	if(pProp->GetSubItemsCount() > 0) //有子项, 直接修改父项
	{
		CString str, temp;
		for (int i=0; i<pProp->GetSubItemsCount(); i++)
		{
			if(!str.IsEmpty())
				str += _T(",");
			temp.Format(_T("%d"), pProp->GetSubItem(i)->GetValue().iVal);
			str += temp;
		}
		strText = str;
	}
	else if(CompareString(attrType.value(), _T("INT")))
	{
		strText.Format(_T("%d"), pProp->GetValue().intVal);
	}
	else if(CompareString(attrType.value(), _T("DWORD")))
	{
		CUIPropertyGridColorProperty *pPropColor = (CUIPropertyGridColorProperty *)pProp;
		strText = pPropColor->GetUIColor();
	}
	else if(CompareString(attrType.value(), _T("STRING")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("BOOL")))
	{
		strText = pProp->GetValue().boolVal ? _T("true") : _T("false");
	}
	else if(CompareString(attrType.value(), _T("SIZE")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("RECT")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("CHAR")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("IMAGE")))
	{
		strText = pProp->GetValue().bstrVal;
	}
	else if(CompareString(attrType.value(), _T("ATTRIBUTELIST")))
	{
		strText = pProp->GetValue().bstrVal;
	}

	CString strTextToCopy;
	strTextToCopy += _T("_T(\"");
	strTextToCopy += strText;
	strTextToCopy += _T("\")");

	if(OpenClipboard())
	{
		EmptyClipboard();

		HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE,   (strTextToCopy.GetLength()+1) * sizeof(TCHAR));
		TCHAR *buffer = (TCHAR *)GlobalLock(clipbuffer);
		_tcscpy(buffer, strTextToCopy);
		GlobalUnlock(clipbuffer);
#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT, clipbuffer);
#else
		SetClipboardData(CF_TEXT, clipbuffer);
#endif
		CloseClipboard();
	}
}


void CUIPropertyGridCtrl::OnUpdateGridpropertyCopyValueEx(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pPropFocused!=NULL);
}


void CUIPropertyGridCtrl::OnGridpropertySetDefaultValue()
{
	CMFCPropertyGridProperty *pProp = m_pPropFocused;

	xml_node duiNode((xml_node_struct *)pProp->GetData()); //属性文件
	xml_attribute attrType		= duiNode.attribute(XTEXT("type"));
	xml_attribute attrDefault	= duiNode.attribute(XTEXT("default"));

	int nLevel = pProp->GetHierarchyLevel();
	if(nLevel > 1) //重置子项，比如 RECT SIZE
	{
		CMFCPropertyGridProperty *pPropParent = pProp->GetParent();
		duiNode = xml_node((xml_node_struct *)pPropParent->GetData());
		attrType = duiNode.attribute(XTEXT("type"));
		attrDefault	= duiNode.attribute(XTEXT("default"));

		if(CompareString(attrType.value(), _T("SIZE")))
		{
			CDuiSize szDefault(XML2T(attrDefault.as_string(XTEXT("0,0"))));

			CString strName = pProp->GetName();
			if(CompareString(strName, _T("width")))
			{
				pProp->SetValue(_variant_t((long)szDefault.cx, VT_I4));
			}
			else if(CompareString(strName, _T("height")))
			{
				pProp->SetValue(_variant_t((long)szDefault.cy, VT_I4));
			}
		}
		else if(CompareString(attrType.value(), _T("RECT")))
		{
			CDuiRect rcDefault(XML2T(attrDefault.as_string(XTEXT("0,0,0,0"))));
			CString strName = pProp->GetName();
			if(CompareString(strName, _T("left")))
			{
				pProp->SetValue(_variant_t((long)rcDefault.left, VT_I4));
			}
			else if(CompareString(strName, _T("top")))
			{
				pProp->SetValue(_variant_t((long)rcDefault.top, VT_I4));
			}
			else if(CompareString(strName, _T("right")))
			{
				pProp->SetValue(_variant_t((long)rcDefault.top, VT_I4));
			}
			else if(CompareString(strName, _T("bottom")))
			{
				pProp->SetValue(_variant_t((long)rcDefault.top, VT_I4));
			}
		}
	}
	else if(CompareString(attrType.value(), _T("INT")))
	{
		pProp->SetValue(_variant_t((long)attrDefault.as_int(), VT_I4));
	}
	else if(CompareString(attrType.value(), _T("DWORD")))
	{
		CUIPropertyGridColorProperty *pPropColor = (CUIPropertyGridColorProperty *)pProp;
		pPropColor->SetUIColor(XML2T(attrDefault.as_string()));
	}
	else if(CompareString(attrType.value(), _T("STRING")))
	{
		pProp->SetValue((_variant_t)attrDefault.as_string());
	}
	else if(CompareString(attrType.value(), _T("BOOL")))
	{
		pProp->SetValue((_variant_t)attrDefault.as_bool());
	}
	else if(CompareString(attrType.value(), _T("SIZE")))
	{
		CDuiSize szDefault(XML2T(attrDefault.as_string(XTEXT("0,0"))));
		if(pProp->GetSubItem(0)) pProp->GetSubItem(0)->SetValue(_variant_t((long)szDefault.cx, VT_I4));
		if(pProp->GetSubItem(1)) pProp->GetSubItem(1)->SetValue(_variant_t((long)szDefault.cy, VT_I4));
	}
	else if(CompareString(attrType.value(), _T("RECT")))
	{
		CDuiRect rcDefault(XML2T(attrDefault.as_string(XTEXT("0,0,0,0"))));	
		if(pProp->GetSubItem(0)) pProp->GetSubItem(0)->SetValue(_variant_t((long)rcDefault.left, VT_I4));
		if(pProp->GetSubItem(1)) pProp->GetSubItem(1)->SetValue(_variant_t((long)rcDefault.top, VT_I4));
		if(pProp->GetSubItem(2)) pProp->GetSubItem(2)->SetValue(_variant_t((long)rcDefault.right, VT_I4));
		if(pProp->GetSubItem(3)) pProp->GetSubItem(3)->SetValue(_variant_t((long)rcDefault.bottom, VT_I4));

	}
	else if(CompareString(attrType.value(), _T("CHAR")))
	{
		pProp->SetValue((_variant_t)attrDefault.as_string());
	}
	else if(CompareString(attrType.value(), _T("IMAGE")))
	{
		pProp->SetValue((_variant_t)attrDefault.as_string());
	}
	else if(CompareString(attrType.value(), _T("ATTRIBUTELIST")))
	{
		pProp->SetValue((_variant_t)attrDefault.as_string());
	}

	OnPropertyChanged(pProp);
}


void CUIPropertyGridCtrl::OnUpdateGridpropertySetDefaultValue(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pPropFocused!=NULL);
}
