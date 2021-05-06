#include "StdAfx.h"
#include "DuiEditor.h"
#include "UIView.h"

namespace DuiLib {

CUIFormView::CUIFormView(void)
{

}

CUIFormView::~CUIFormView(void)
{
}

LPCTSTR CUIFormView::GetClass() const
{
	return _T("FormUI");
}

LPVOID CUIFormView::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("Form")) == 0 )
		return static_cast<CUIFormView*>(this);

	return CContainerUI::GetInterface(pstrName);
}

void CUIFormView::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CPaintManagerUI *pManager = GetManager();

	if( _tcsicmp(pstrName, _T("size")) == 0 ) {
		LPTSTR pstr = NULL;
		int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 

		SetInitSize(cx, cy);
	} 
	else if( _tcsicmp(pstrName, _T("sizebox")) == 0 ) {
		RECT rcSizeBox = { 0 };
		LPTSTR pstr = NULL;
		rcSizeBox.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		pManager->SetSizeBox(rcSizeBox);
	}
	else if( _tcsicmp(pstrName, _T("caption")) == 0 ) {
		RECT rcCaption = { 0 };
		LPTSTR pstr = NULL;
		rcCaption.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		pManager->SetCaptionRect(rcCaption);
	}
	else if( _tcsicmp(pstrName, _T("roundcorner")) == 0 ) {
		LPTSTR pstr = NULL;
		int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
		pManager->SetRoundCorner(cx, cy);
	} 
	else if( _tcsicmp(pstrName, _T("mininfo")) == 0 ) {
		LPTSTR pstr = NULL;
		int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
		pManager->SetMinInfo(cx, cy);
	}
	else if( _tcsicmp(pstrName, _T("maxinfo")) == 0 ) {
		LPTSTR pstr = NULL;
		int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
		pManager->SetMaxInfo(cx, cy);
	}
	else if( _tcsicmp(pstrName, _T("showdirty")) == 0 ) {
		pManager->SetShowUpdateRect(_tcsicmp(pstrValue, _T("true")) == 0);
	} 
	else if( _tcsicmp(pstrName, _T("opacity")) == 0 || _tcsicmp(pstrName, _T("alpha")) == 0 ) {
		pManager->SetOpacity(_ttoi(pstrValue));
	} 
	else if( _tcscmp(pstrName, _T("layeredopacity")) == 0 ) {
		pManager->SetLayeredOpacity(_ttoi(pstrValue));
	} 
	else if( _tcscmp(pstrName, _T("layered")) == 0 || _tcscmp(pstrName, _T("bktrans")) == 0) {
		pManager->SetLayered(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcscmp(pstrName, _T("layeredimage")) == 0 ) {
		pManager->SetLayered(true);
		pManager->SetLayeredImage(pstrValue);
	} 
	else if( _tcscmp(pstrName, _T("noactivate")) == 0 ) {
		pManager->SetNoActivate(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("disabledfontcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		pManager->SetDefaultDisabledColor(clrColor);
	} 
	else if( _tcsicmp(pstrName, _T("defaultfontcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		pManager->SetDefaultFontColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("linkfontcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		pManager->SetDefaultLinkFontColor(clrColor);
	} 
	else if( _tcsicmp(pstrName, _T("linkhoverfontcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		pManager->SetDefaultLinkHoverFontColor(clrColor);
	} 
	else if( _tcsicmp(pstrName, _T("selectedcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		pManager->SetDefaultSelectedBkColor(clrColor);
	} 

	else if( _tcsicmp(pstrName, _T("shadowsize")) == 0 ) {
		pManager->GetShadow()->SetSize(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("shadowsharpness")) == 0 ) {
		pManager->GetShadow()->SetSharpness(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("shadowdarkness")) == 0 ) {
		pManager->GetShadow()->SetDarkness(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("shadowposition")) == 0 ) {
		LPTSTR pstr = NULL;
		int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
		pManager->GetShadow()->SetPosition(cx, cy);
	}
	else if( _tcsicmp(pstrName, _T("shadowcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		pManager->GetShadow()->SetColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("shadowcorner")) == 0 ) {
		RECT rcCorner = { 0 };
		LPTSTR pstr = NULL;
		rcCorner.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcCorner.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcCorner.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcCorner.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		pManager->GetShadow()->SetShadowCorner(rcCorner);
	}
	else if( _tcsicmp(pstrName, _T("shadowimage")) == 0 ) {
		pManager->GetShadow()->SetImage(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("showshadow")) == 0 ) {
		pManager->GetShadow()->ShowShadow(_tcsicmp(pstrValue, _T("true")) == 0);
	} 
	else if( _tcsicmp(pstrName, _T("gdiplustext")) == 0 ) {
		pManager->SetUseGdiplusText(_tcsicmp(pstrValue, _T("true")) == 0);
	} 
	else if( _tcsicmp(pstrName, _T("textrenderinghint")) == 0 ) {
		pManager->SetGdiplusTextRenderingHint(_ttoi(pstrValue));
	} 
	else if( _tcsicmp(pstrName, _T("tooltiphovertime")) == 0 ) {
		pManager->SetHoverTime(_ttoi(pstrValue));
	} 
}

SIZE CUIFormView::GetInitSize()
{
	return m_pManager->GetInitSize();
}

void CUIFormView::SetInitSize(int cx, int cy)
{
	GetManager()->SetInitSize(GetManager()->GetDPIObj()->Scale(cx), GetManager()->GetDPIObj()->Scale(cy));

	if(GetManager()->GetPaintWindow())
	{
		::SetWindowPos(m_pManager->GetPaintWindow(), NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		Refresh();
	}
}

void CUIFormView::Refresh()
{
	m_pManager->GetRoot()->NeedUpdate();
}


}