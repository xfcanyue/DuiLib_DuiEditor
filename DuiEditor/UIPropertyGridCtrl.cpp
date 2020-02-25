// UIPropertyGridCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "UIPropertyGridCtrl.h"

#include "MainFrm.h"
#include "ImageEditor.h"
#include "DefaultEditor.h"
#include "DlgStringEditor.h"

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

	CString strOldValue = GetValue().bstrVal;
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

	CString strOldValue = GetValue().bstrVal;
	CImageEditor dlg;
	dlg.m_pParentGrid = m_pWndList;
	dlg.m_pParentProp = this;
	dlg.SetAttributeValue(strOldValue);
	if(dlg.DoModal() == IDOK)
	{
		SetValue(_variant_t(dlg.GetAttributeValue()));
	}
	else
	{
		SetValue((_variant_t)strOldValue);
		m_pWndList->OnPropertyChanged(this);
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
		m_strClassName = pGridCtrl->m_TreeNode.attribute(_T("name")).value();
	else if(g_duiProp.IsStyleNode(pGridCtrl->m_TreeNode))
		m_strClassName = pGridCtrl->m_TreeNode.attribute(_T("class")).value();

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


	if(m_strClassName.IsEmpty())
		return;

	CDefaultEditor dlg;
	dlg.SetAttributeValue(m_strClassName, GetValue().bstrVal);
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
	m_bMainFrame = FALSE;
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
	if(!m_bMainFrame)	return NULL;

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CDuiEditorViewDesign *pView = (CDuiEditorViewDesign *)pMain->GetActiveUIView();
	return pView;
}

void CUIPropertyGridCtrl::InitProp(xml_node TreeNode)
{
	m_TreeNode = TreeNode;

	pugi::xml_node root = g_duiProp.GetRoot();
	if(!root)
	{
		AfxMessageBox(_T("读取属性列表失败"));
		return;
	}

	//当前控件属性
	xml_node nodePropertyControl = g_duiProp.FindControl(m_TreeNode.name());
	if(nodePropertyControl)
	{
		_initProp(nodePropertyControl);
	}
	else //在duiprop找不到, 是不是自定义控件?
	{
		xml_attribute attrCustom = m_TreeNode.attribute(_T("custombasedfrom"));
		if(attrCustom)
		{
			nodePropertyControl = g_duiProp.FindControl(attrCustom.value());
			_initProp(nodePropertyControl);
		}
	}

	//载入父控件属性
	LPCTSTR ParentClassName = nodePropertyControl.attribute(_T("parent")).value();
	for(;;)
	{
		if(ParentClassName == NULL || ParentClassName[0] == '\0')
			break;

		xml_node nodeParent = g_duiProp.FindControl(ParentClassName);
		if(!nodeParent)	break;

		_initProp(nodeParent);

		ParentClassName = nodeParent.attribute(_T("parent")).value();
	}
		
	return;
}

void CUIPropertyGridCtrl::_initProp(xml_node nodeProperty)
{
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(nodeProperty.name());
	AddProperty(pGroup1);

	for (pugi::xml_node nodeChild = nodeProperty.first_child(); nodeChild; nodeChild = nodeChild.next_sibling())
	{
		if(_tcscmp(nodeChild.name(), _T("Attribute")) == 0)
		{
			//过滤已经添加的属性
			if(_isRepeatProperty(nodeChild.attribute(_T("name")).value()))
				continue;

			CString strAttr = nodeChild.attribute(_T("name")).value();
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

void CUIPropertyGridCtrl::InsertDuiLibProperty(xml_node TreeNode, xml_node attrNode, CMFCPropertyGridProperty* pGroupParent)
{
	xml_attribute attrName = attrNode.attribute(_T("name"));
	xml_attribute attrDefValue = attrNode.attribute(_T("default"));	
	xml_attribute attrComment = attrNode.attribute(_T("comment"));
	xml_attribute attrType = attrNode.attribute(_T("type"));
	xml_attribute attrSpanValue = attrNode.attribute(_T("value"));

	CMFCPropertyGridProperty *pProperty = NULL;

	//Font定义的name字段, 特殊处理
	if(g_duiProp.IsFontNode(TreeNode) && CompareString(attrName.value(), _T("name")) && CompareString(attrType.value(), _T("STRING")))
	{
		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf); 
		CUIPropertyGridFontProperty *pfntProp;
		pfntProp = new CUIPropertyGridFontProperty(attrName.value(), lf, CF_EFFECTS | CF_SCREENFONTS, attrComment.value());
		pfntProp->SetData((DWORD)attrNode.internal_object());
		pfntProp->AllowEdit(TRUE);
		pGroupParent->AddSubItem(pfntProp);	

		xml_attribute attrName = TreeNode.attribute(_T("name"));
		if(!attrName)
		{
			attrName = g_duiProp.AddAttribute(TreeNode, _T("name"), _T("宋体"), GetView());
		}
		pfntProp->SetFontName(attrName.value());
		return;
	}

	//style字段,列出CPaintManager里面有的 
	if(CompareString(attrName.value(), _T("style")))
	{
		pProperty = new CMFCPropertyGridProperty(attrName.value(), (_variant_t)attrDefValue.value(),  attrComment.value());
		pProperty->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pProperty);

		if(m_bMainFrame)
		{
			CPaintManagerUI *pManager = GetView()->m_Manager.GetManager();
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
			pProperty->SetValue((_variant_t)attr.value());
		else
			pProperty->SetValue((_variant_t)attrDefValue.value());

		pProperty->SetOriginalValue(pProperty->GetValue());	
		return;
	}

	if(CompareString(attrType.value(), _T("INT")))
	{
		pProperty = new CMFCPropertyGridProperty(attrName.value(), (_variant_t)(INT)attrDefValue.as_int(),  attrComment.value());
		pProperty->SetData((DWORD)attrNode.internal_object());
		pProperty->AllowEdit(TRUE);
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pGroupParent->AddSubItem(pProperty);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			pProperty->SetValue((_variant_t)attr.as_int());
		}
	}
	else if(CompareString(attrType.value(), _T("DWORD")))
	{
		CUIPropertyGridColorProperty* pColorProp = new CUIPropertyGridColorProperty(attrName.value(), (_variant_t)0,  NULL, attrComment.value());
		pColorProp->SetData((DWORD)attrNode.internal_object());
		pColorProp->EnableOtherButton(_T("其他..."));
		pColorProp->EnableAutomaticButton(_T("transparent"), RGB(255,255,255), TRUE);
		pColorProp->SetUIColor(attrDefValue.value());
		pGroupParent->AddSubItem(pColorProp);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			pColorProp->SetUIColor(attr.value());
		}
	}
	else if(CompareString(attrType.value(), _T("STRING")))
	{
		if(attrSpanValue)	//STRING类型是否有value取值属性
			pProperty = new CMFCPropertyGridProperty(attrName.value(), (_variant_t)attrDefValue.value(),  attrComment.value());
		else
			pProperty = new CUIPropertyGridStringProperty(attrName.value(), (_variant_t)attrDefValue.value(),  attrComment.value());
		pProperty->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pProperty);

		if(attrSpanValue)	//STRING类型是否有value取值属性
		{
			CStringArray arrString;
			SplitCString(arrString, attrSpanValue.value());
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
			pProperty->SetValue((_variant_t)attr.value());
		else
			pProperty->SetValue((_variant_t)attrDefValue.value());
		pProperty->SetOriginalValue(pProperty->GetValue());		
	}
	else if(CompareString(attrType.value(), _T("BOOL")))
	{
		bool bValue = attrDefValue.as_bool();
		pProperty = new CMFCPropertyGridProperty(attrName.value(), (_variant_t)bValue,  attrComment.value());
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
		CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(attrName.value(), (_variant_t)0, TRUE);
		pSize->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pSize);

		CStringArray strArrDef;
		SplitCString(strArrDef, attrDefValue.value());

		int nWidth = 0;
		int nHeight = 0;
		if(strArrDef.GetSize() > 0) nWidth = _tstoi(strArrDef[0]);
		if(strArrDef.GetSize() > 1) nHeight = _tstoi(strArrDef[1]);

		xml_attribute attr = TreeNode.attribute(attrName.value());
		if(attr)
		{
			CStringArray strArray;
			SplitCString(strArray, attr.value());
			if(strArray.GetSize() > 0) nWidth = _tstoi(strArray[0]);
			if(strArray.GetSize() > 1) nHeight = _tstoi(strArray[1]);
		}

		pProperty = new CMFCPropertyGridProperty(_T("width"), (_variant_t)(INT) nWidth, attrComment.value());
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pSize->AddSubItem(pProperty);
 
		pProperty = new CMFCPropertyGridProperty( _T("height"), (_variant_t)(INT) nHeight, attrComment.value());
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pSize->AddSubItem(pProperty);
	}
	else if(CompareString(attrType.value(), _T("RECT")))
	{
		CMFCPropertyGridProperty* pRect = new CMFCPropertyGridProperty(attrName.value(), attrDefValue.as_int(), TRUE);
		pRect->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pRect);

		CStringArray strArrDef;
		SplitCString(strArrDef, attrDefValue.value());

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
			SplitCString(strArray, attr.value());
			if(strArray.GetSize() > 0) nLeft = _tstoi(strArray[0]);
			if(strArray.GetSize() > 1) nTop = _tstoi(strArray[1]);
			if(strArray.GetSize() > 2) nRight = _tstoi(strArray[2]);
			if(strArray.GetSize() > 3) nBottom = _tstoi(strArray[3]);
		}

		pProperty = new CMFCPropertyGridProperty(_T("left"), (_variant_t)(INT) nLeft, attrComment.value());
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

		pProperty = new CMFCPropertyGridProperty( _T("top"), (_variant_t)(INT) nTop, attrComment.value());
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

		pProperty = new CMFCPropertyGridProperty( _T("right"), (_variant_t)(INT) nRight, attrComment.value());
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

		pProperty = new CMFCPropertyGridProperty( _T("bottom"), (_variant_t)(INT) nBottom, attrComment.value());
		pProperty->EnableSpinControl(TRUE, 0, 9999);
		pRect->AddSubItem(pProperty);

	}
	else if(CompareString(attrType.value(), _T("CHAR")))
	{
		pProperty = new CMFCPropertyGridProperty(attrName.value(), (_variant_t)attrDefValue.value(),  attrComment.value());
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
		CUIPropertyGridImageProperty* pImage = new CUIPropertyGridImageProperty(attrName.value(), attr.value());
		pImage->SetData((DWORD)attrNode.internal_object());
		pGroupParent->AddSubItem(pImage);
	}
	else if(CompareString(attrType.value(), _T("ATTRIBUTELIST")))
	{
		xml_attribute attr = TreeNode.attribute(attrName.value());
		CUIPropertyGridAttributeListProperty* pImage = new CUIPropertyGridAttributeListProperty(attrName.value(), attr.value());
		pImage->SetDescription(attrComment.value());
		pImage->SetData((DWORD)attrNode.internal_object());

		if(g_duiProp.IsDefaultNode(TreeNode))
			pImage->m_strClassName = TreeNode.attribute(_T("name")).value();
		else if(g_duiProp.IsStyleNode(TreeNode))
			pImage->m_strClassName = TreeNode.attribute(_T("class")).value();
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

	pfntProp->SetFontName(m_TreeNode.attribute(_T("name")).as_string(_T("宋体")));
	pfntProp->SetBold(m_TreeNode.attribute(_T("bold")).as_bool());
	pfntProp->SetUnderline(m_TreeNode.attribute(_T("underline")).as_bool());
	pfntProp->SetItalic(m_TreeNode.attribute(_T("italic")).as_bool());
}

void CUIPropertyGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
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
	xml_attribute attrName		= duiNode.attribute(_T("name"));
	xml_attribute attrDefValue	= duiNode.attribute(_T("default"));	
	xml_attribute attrComment	= duiNode.attribute(_T("comment"));
	xml_attribute attrType		= duiNode.attribute(_T("type"));
	xml_attribute attrSpanValue = duiNode.attribute(_T("value"));

	xml_attribute attrTree;
	
	if(pProp->GetValue().vt == VT_EMPTY || pProp->GetValue().vt == VT_NULL)
		return;

	if(CompareString(attrType.value(), _T("INT")))
	{
		//attrTree.set_value(pProp->GetValue().intVal);
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), pProp->GetValue().intVal, GetView());
	}
	else if(CompareString(attrType.value(), _T("DWORD")))
	{
		CUIPropertyGridColorProperty *pPropColor = (CUIPropertyGridColorProperty *)pProp;
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), pPropColor->GetUIColor(), GetView());
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
		if(m_bMainFrame)
		{
			if(CompareString(attrName.value(), _T("name")))
				if(g_duiProp.IsBaseFromControlUI(m_TreeNode.name()))
				{
					//LPCTSTR lpszValue = pProp->GetValue().bstrVal;
					CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
					CDuiEditorViewDesign *pView = (CDuiEditorViewDesign *)pMain->GetActiveUIView();
					xml_node nodeDoc = pView->GetDocument()->m_doc.root().child(_T("Window"));
					if(IsRepeatName(nodeDoc, (LPCTSTR)pProp->GetValue().bstrVal))
					{
						AfxMessageBox(_T("已存在相同的name属性控件!"));
						pProp->SetValue(pProp->GetOriginalValue());
						return;
					}
				}
		}
		
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), (LPCTSTR)pProp->GetValue().bstrVal, GetView());
		pProp->SetOriginalValue(pProp->GetValue());
	}
	else if(CompareString(attrType.value(), _T("BOOL")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), pProp->GetValue().boolVal != 0, GetView());
	}
	else if(CompareString(attrType.value(), _T("SIZE")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), (LPCTSTR)pProp->GetValue().bstrVal, GetView());
	}
	else if(CompareString(attrType.value(), _T("RECT")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), (LPCTSTR)pProp->GetValue().bstrVal, GetView());
	}
	else if(CompareString(attrType.value(), _T("CHAR")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), (LPCTSTR)pProp->GetValue().bstrVal, GetView());
	}
	else if(CompareString(attrType.value(), _T("IMAGE")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), (LPCTSTR)pProp->GetValue().bstrVal, GetView());
	}
	else if(CompareString(attrType.value(), _T("ATTRIBUTELIST")))
	{
		attrTree = g_duiProp.AddAttribute(m_TreeNode, attrName.value(), (LPCTSTR)pProp->GetValue().bstrVal, GetView());
	}

	CDuiEditorViewDesign *pView = GetView();
	if(pView)	pView->m_Manager.UpdateControlUI(m_TreeNode, attrTree);

	g_duiProp.FilterDefaultValue(m_TreeNode);

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
				pProp2->SetValue((_variant_t)(INT)size);
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
	xml_attribute attr = nodeDoc.attribute(_T("name"));
	if(attr)
	{
		LPCTSTR lpszName2 = attr.value();
		if(lpszName2 != '\0' && CompareString(lpszName, lpszName2))
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
	xml_attribute attrType		= duiNode.attribute(_T("type"));


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
	xml_attribute attrType		= duiNode.attribute(_T("type"));


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
	
}


void CUIPropertyGridCtrl::OnUpdateGridpropertySetDefaultValue(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);//(m_pPropFocused!=NULL);
}
