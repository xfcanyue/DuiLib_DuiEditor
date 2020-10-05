// ImageEditorFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorFrame.h"

#include "ImageEditor.h"
#include "MainFrm.h"
#include "UIManager.h"
//////////////////////////////////////////////////////////////////////////
// CImageEditorFrame

IMPLEMENT_DYNCREATE(CImageEditorFrame, CFrameWndEx)

CImageEditorFrame::CImageEditorFrame()
{
	m_bPreview = TRUE;	
}

CImageEditorFrame::~CImageEditorFrame()
{
}


BEGIN_MESSAGE_MAP(CImageEditorFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, &CImageEditorFrame::OnIdleUpdateCmdUI)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_TB_EXIT, &CImageEditorFrame::OnTbExit)
	ON_UPDATE_COMMAND_UI(ID_TB_EXIT, &CImageEditorFrame::OnUpdateTbExit)
	ON_COMMAND(ID_TB_SAVE, &CImageEditorFrame::OnTbSave)
	ON_UPDATE_COMMAND_UI(ID_TB_SAVE, &CImageEditorFrame::OnUpdateTbSave)
	ON_COMMAND(ID_TB_UI_PREVIEW, &CImageEditorFrame::OnTbUiPreview)
	ON_UPDATE_COMMAND_UI(ID_TB_UI_PREVIEW, &CImageEditorFrame::OnUpdateTbUiPreview)
END_MESSAGE_MAP()


// CImageEditorFrame 消息处理程序


int CImageEditorFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CImageEditor *pDlgMain = (CImageEditor *)GetParent();

	// 创建视图
	//////////////////////////////////////////////////////////////////////////
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar.LoadToolBar(IDR_TB_IMAGE_EDITOR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	m_wndToolBar.EnableTextLabels(TRUE);

	if (!m_wndList.Create(_T("文件"), this, 
		CRect(0, 0, 300, 200), TRUE, ID_VIEW_CONTROL, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // 未能创建
	}

	if (!m_wndProperty.Create(_T("属性"), this, 
		CRect(0, 0, 300, 200), TRUE, ID_PROPERTY_WND, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT, 
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // 未能创建
	}
	CUIPropertyGridCtrl *pPropList = m_wndProperty.CreatePropList();
	pPropList->SetOwner(this);

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndList.EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperty.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndList);
	DockPane(&m_wndProperty);

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CDuiEditorViewDesign *pView = (CDuiEditorViewDesign *)pMain->GetActiveUIView();
	m_wndView.m_pManager = pView->GetUIManager()->GetManager();

	pPropList->InitProp(pDlgMain->m_nodeImage.child(XTEXT("IMAGE")));
	m_wndProperty.OnExpandAllProperties();
	//////////////////////////////////////////////////////////////////////////
	//m_wndList.m_wndList.DisplayFolder(CPaintManagerUI::GetResourcePath().GetData());
	m_wndList.m_wndList.DisplayFolder(pDlgMain->m_pDoc->GetSkinPath());

	SetViewImage();
	m_wndView.SetFocus();
	return 0;
}

void CImageEditorFrame::SetViewImage()
{
	CImageEditor *pDlgMain = (CImageEditor *)GetParent();

	xml_node node = pDlgMain->m_nodeImage.child(XTEXT("IMAGE"));
	CString strImage, temp;
	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		temp.Format(_T("%s='%s' "), XML2T(attr.name()), XML2T(attr.value()));
		strImage += temp;
	}

	//获取图片大小
	xml_attribute attr2 = node.attribute(XTEXT("file"));
	CImage img;
	CRect  rcImg;
	CString strSkinDir = pDlgMain->m_pDoc->GetSkinPath();//CPaintManagerUI::GetResourcePath();
	HRESULT hRet = img.Load(strSkinDir + attr2.value());
	if(SUCCEEDED(hRet))
	{
		rcImg.SetRect(0, 0, img.GetWidth(), img.GetHeight());
	}

	pDlgMain->m_rcImage = rcImg;
	m_wndView.SetImage(strImage, rcImg);
}

BOOL CImageEditorFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CFrameWndEx::OnEraseBkgnd(pDC);
}

BOOL CImageEditorFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;


	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LRESULT CImageEditorFrame::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	if(m_wndToolBar.GetSafeHwnd())
	{
		m_wndToolBar.SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
	}
	if(m_wndList.GetSafeHwnd())
	{
		m_wndList.SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
		m_wndList.m_wndToolBar.SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
	}
	CFrameWnd::OnIdleUpdateCmdUI();
	return 0;
}

void CImageEditorFrame::OnSelectedFile(LPCTSTR lpstrPathName)
{
	CImageEditor *pDlgMain = (CImageEditor *)GetParent();

	CString strDocPath = pDlgMain->m_pDoc->GetSkinPath(); //CPaintManagerUI::GetResourcePath();
	CString strFileName = lpstrPathName;
	
	//只能取子目录的文件
	if(strFileName.Find(strDocPath) == 0)
	{
		strFileName = strFileName.Right(strFileName.GetLength() - strDocPath.GetLength());
	}
	else
	{
		::MessageBoxA(NULL, "只能使用当前目录或者子目录的文件", "ERROR", MB_OK);
		return;
	}

	CUIPropertyGridCtrl *pPropList = m_wndProperty.GetPropList(0);
	if(pPropList->GetPropertyCount() > 0)
	{
		if(pPropList->GetProperty(0)->GetSubItemsCount() > 0)
		{
			CMFCPropertyGridProperty *pProp = pPropList->GetProperty(0)->GetSubItem(0);

			pProp->SetValue((_variant_t)strFileName);
			pPropList->OnPropertyChanged(pProp);
			m_wndView.SetFocus();
		}
	}
}

void CImageEditorFrame::OnSetSourcePos(CRect &rc)
{
	CUIPropertyGridCtrl *pPropList = m_wndProperty.GetPropList(0);
	for (int i=0; i<pPropList->GetPropertyCount(); i++)
	{
		CMFCPropertyGridProperty *pProp1 = pPropList->GetProperty(i);
		for (int j=0; j<pProp1->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty *pProp2 = pProp1->GetSubItem(j);
			CString strName = pProp2->GetName();

			if( CompareString(strName, _T("source")) ) 
			{
				CString temp;
				temp.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
				//pProp2->SetValue((_variant_t)temp);

				for(int x=0; x<pProp2->GetSubItemsCount(); x++)
				{
					CMFCPropertyGridProperty *pProp3 = pProp2->GetSubItem(x);
					if(CompareString(pProp3->GetName(), _T("left")))
					{
						pProp3->SetValue((_variant_t)(INT)rc.left);
					}
					else if(CompareString(pProp3->GetName(), _T("top")))
					{
						pProp3->SetValue((_variant_t)(INT)rc.top);
					}
					else if(CompareString(pProp3->GetName(), _T("right")))
					{
						pProp3->SetValue((_variant_t)(INT)rc.right);
					}
					else if(CompareString(pProp3->GetName(), _T("bottom")))
					{
						pProp3->SetValue((_variant_t)(INT)rc.bottom);
					}
				}

				pPropList->OnPropertyChanged(pProp2);
				break;
			}
		}
	}
}

LRESULT CImageEditorFrame::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	
	SetViewImage();

	if(m_bPreview)
	{
		CImageEditor *pDlgMain = (CImageEditor *)GetParent();

		if(pDlgMain->m_pParentGrid && pDlgMain->m_pParentProp)
		{
			pDlgMain->m_pParentProp->SetValue(_variant_t(pDlgMain->GetAttributeValue()));
			pDlgMain->m_pParentProp->SetOriginalValue(pProp->GetValue());
			pDlgMain->m_pParentGrid->OnPropertyChanged(pDlgMain->m_pParentProp);
		}
	}

	return 0;
}


void CImageEditorFrame::OnTbExit()
{
	CImageEditor *pDlgMain = (CImageEditor *)GetParent();
	pDlgMain->SendMessage(WM_COMMAND, IDCANCEL, 0);
}


void CImageEditorFrame::OnUpdateTbExit(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CImageEditorFrame::OnTbSave()
{
	CImageEditor *pDlgMain = (CImageEditor *)GetParent();
	pDlgMain->SendMessage(WM_COMMAND, IDOK, 0);
}


void CImageEditorFrame::OnUpdateTbSave(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CImageEditorFrame::OnTbUiPreview()
{
	m_bPreview = !m_bPreview;
}


void CImageEditorFrame::OnUpdateTbUiPreview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bPreview);
}
