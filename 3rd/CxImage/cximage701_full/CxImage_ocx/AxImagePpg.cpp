// AxImagePpg.cpp : Implementation of the AxImagePropPage property page class.

#include "stdafx.h"
#include "AxImage.h"
#include "AxImagePpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(AxImagePropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(AxImagePropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(AxImagePropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(AxImagePropPage, "AXIMAGE.AxImagePropPage.1",
	0x4fed1ac7, 0x6cf3, 0x11d6, 0xbb, 0x83, 0x9f, 0xdd, 0x92, 0x23, 0x64, 0x7d)


/////////////////////////////////////////////////////////////////////////////
// AxImagePropPage::AxImagePropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for AxImagePropPage

BOOL AxImagePropPage::AxImagePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_AXIMAGE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// AxImagePropPage::AxImagePropPage - Constructor

AxImagePropPage::AxImagePropPage() :
	COlePropertyPage(IDD, IDS_AXIMAGE_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(AxImagePropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// AxImagePropPage::DoDataExchange - Moves data between page and properties

void AxImagePropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(AxImagePropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// AxImagePropPage message handlers
