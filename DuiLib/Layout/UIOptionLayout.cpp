#include "StdAfx.h"
#include "UIOptionLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(COptionLayoutUI)
	COptionLayoutUI::COptionLayoutUI() : m_bSelected(false) ,m_iSelectedFont(-1), m_dwSelectedTextColor(0), m_dwSelectedBkColor(0), m_nSelectedStateCount(0)
	{
	}

	COptionLayoutUI::~COptionLayoutUI()
	{
		if( !m_sGroupName.IsEmpty() && m_pManager ) m_pManager->RemoveOptionGroup(m_sGroupName, this);
	}
	 
	LPCTSTR COptionLayoutUI::GetClass() const
	{
		return _T("OptionLayoutUI");
	}

	LPVOID COptionLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_OPTION_LAYOUT) == 0 ) return static_cast<COptionLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void COptionLayoutUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
	{
		__super::SetManager(pManager, pParent, bInit);
		if( bInit && !m_sGroupName.IsEmpty() ) {
			if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
		}
	}

	LPCTSTR COptionLayoutUI::GetGroup() const
	{
		return m_sGroupName;
	}

	void COptionLayoutUI::SetGroup(LPCTSTR pStrGroupName)
	{
		if( pStrGroupName == NULL ) {
			if( m_sGroupName.IsEmpty() ) return;
			m_sGroupName.Empty();
		}
		else {
			if( m_sGroupName == pStrGroupName ) return;
			if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
			m_sGroupName = pStrGroupName;
		}

		if( !m_sGroupName.IsEmpty() ) {
			if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
		}
		else {
			if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
		}

		Selected(m_bSelected);
	}

	bool COptionLayoutUI::IsSelected() const
	{
		return m_bSelected;
	}

	void COptionLayoutUI::Selected(bool bSelected, bool bTriggerEvent)
	{
		if(m_bSelected == bSelected) return;

		m_bSelected = bSelected;
		if( m_bSelected ) m_uButtonState |= UISTATE_SELECTED;
		else m_uButtonState &= ~UISTATE_SELECTED;

		if( m_pManager != NULL ) {
			if( !m_sGroupName.IsEmpty() ) {
				if( m_bSelected ) {
					CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
					for( int i = 0; i < aOptionGroup->GetSize(); i++ ) 
					{
						CControlUI* pControlx = static_cast<CControlUI*>(aOptionGroup->GetAt(i));
						if(pControlx->GetInterface(DUI_CTR_OPTION_LAYOUT))
						{
							COptionLayoutUI *pControl = static_cast<COptionLayoutUI*>(aOptionGroup->GetAt(i));
							if( pControl != this ) {
								pControl->Selected(false);
								pControl->SwitchControlVisible();
							}
						}
					}
					if (bTriggerEvent) m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
				}
			}
			else {
				if (bTriggerEvent) m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
			}
		}

		Invalidate();
	}

	bool COptionLayoutUI::Activate()
	{
		//if( !CButtonUI::Activate() ) return false;
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL )
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
			BindTriggerTabSel();
		}
		if( !m_sGroupName.IsEmpty() ) Selected(true);
		else Selected(!IsSelected());

		SwitchControlVisible();
		return true;
	}

	void COptionLayoutUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			if( IsSelected() ) m_uButtonState = UISTATE_SELECTED;
			else m_uButtonState = 0;
		}
	}

	void COptionLayoutUI::SwitchControlVisible()
	{
		if(m_sSwitchControlVisible.IsEmpty()) 
			return;

		CControlUI* pControl = GetManager()->FindControl(m_sSwitchControlVisible);
		if(!pControl) return;
		bool bVisible = pControl->IsVisible();
		if(IsSelected() && !bVisible)
		{
			pControl->SetVisible(true);
		}
		else if(!IsSelected() && bVisible)
		{
			pControl->SetVisible(false);
		}
	}

	LPCTSTR COptionLayoutUI::GetSelectedImage()
	{
		return m_sSelectedImage;
	}

	void COptionLayoutUI::SetSelectedImage(LPCTSTR pStrImage)
	{
		m_sSelectedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR COptionLayoutUI::GetSelectedHotImage()
	{
		return m_sSelectedHotImage;
	}

	void COptionLayoutUI::SetSelectedHotImage( LPCTSTR pStrImage )
	{
		m_sSelectedHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR COptionLayoutUI::GetSelectedPushedImage()
	{
		return m_sSelectedPushedImage;
	}

	void COptionLayoutUI::SetSelectedPushedImage(LPCTSTR pStrImage)
	{
		m_sSelectedPushedImage = pStrImage;
		Invalidate();
	}

	void COptionLayoutUI::SetSelectedTextColor(DWORD dwTextColor)
	{
		m_dwSelectedTextColor = dwTextColor;
	}

	DWORD COptionLayoutUI::GetSelectedTextColor()
	{
		if (m_dwSelectedTextColor == 0) m_dwSelectedTextColor = m_pManager->GetDefaultFontColor();
		return m_dwSelectedTextColor;
	}

	void COptionLayoutUI::SetSelectedBkColor( DWORD dwBkColor )
	{
		m_dwSelectedBkColor = dwBkColor;
	}

	DWORD COptionLayoutUI::GetSelectBkColor()
	{
		return m_dwSelectedBkColor;
	}

	LPCTSTR COptionLayoutUI::GetSelectedForedImage()
	{
		return m_sSelectedForeImage;
	}

	void COptionLayoutUI::SetSelectedForedImage(LPCTSTR pStrImage)
	{
		m_sSelectedForeImage = pStrImage;
		Invalidate();
	}

	void COptionLayoutUI::SetSelectedStateCount(int nCount)
	{
		m_nSelectedStateCount = nCount;
		Invalidate();
	}

	int COptionLayoutUI::GetSelectedStateCount() const
	{
		return m_nSelectedStateCount;
	}

	LPCTSTR COptionLayoutUI::GetSelectedStateImage()
	{
		return m_sSelectedStateImage;
	}

	void COptionLayoutUI::SetSelectedStateImage( LPCTSTR pStrImage )
	{
		m_sSelectedStateImage = pStrImage;
		Invalidate();
	}
	void COptionLayoutUI::SetSelectedFont(int index)
	{
		m_iSelectedFont = index;
		Invalidate();
	}

	int COptionLayoutUI::GetSelectedFont() const
	{
		return m_iSelectedFont;
	}
	void COptionLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("group")) == 0 ) SetGroup(pstrValue);
		else if( _tcsicmp(pstrName, _T("selected")) == 0 ) Selected(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("selectedimage")) == 0 ) SetSelectedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedhotimage")) == 0 ) SetSelectedHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedpushedimage")) == 0 ) SetSelectedPushedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedforeimage")) == 0 ) SetSelectedForedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedstateimage")) == 0 ) SetSelectedStateImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("selectedstatecount")) == 0 ) SetSelectedStateCount(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("selectedbkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("selectedtextcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("selectedfont")) == 0 ) SetSelectedFont(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("selectedbordercolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedBorderColor(clrColor);
		}
		else __super::SetAttribute(pstrName, pstrValue);
	}

	void COptionLayoutUI::PaintBkColor(HDC hDC)
	{
		if(IsSelected()) {
			if(m_dwSelectedBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwSelectedBkColor));
			}
		}
		else {
			return __super::PaintBkColor(hDC);
		}
	}

	void COptionLayoutUI::PaintStatusImage(HDC hDC)
	{
		if(IsSelected()) {
			if(!m_sSelectedStateImage.IsEmpty() && m_nSelectedStateCount > 0)
			{
				TDrawInfo info;
				info.Parse(m_sSelectedStateImage, _T(""), m_pManager);
				const TImageInfo* pImage = m_pManager->GetImageEx(info.sImageName, info.sResType, info.dwMask, info.width, info.height, info.fillcolor, info.bHSL);
				if(m_sSelectedImage.IsEmpty() && pImage != NULL)
				{
					SIZE szImage = {pImage->nX, pImage->nY};
					SIZE szStatus = {pImage->nX / m_nSelectedStateCount, pImage->nY};
					if( szImage.cx > 0 && szImage.cy > 0 )
					{
						RECT rcSrc = {0, 0, szImage.cx, szImage.cy};
						if(m_nSelectedStateCount > 0) {
							int iLeft = rcSrc.left + 0 * szStatus.cx;
							int iRight = iLeft + szStatus.cx;
							int iTop = rcSrc.top;
							int iBottom = iTop + szStatus.cy;
							m_sSelectedImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
						}
						if(m_nSelectedStateCount > 1) {
							int iLeft = rcSrc.left + 1 * szStatus.cx;
							int iRight = iLeft + szStatus.cx;
							int iTop = rcSrc.top;
							int iBottom = iTop + szStatus.cy;
							m_sSelectedHotImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
							m_sSelectedPushedImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
						}
						if(m_nSelectedStateCount > 2) {
							int iLeft = rcSrc.left + 2 * szStatus.cx;
							int iRight = iLeft + szStatus.cx;
							int iTop = rcSrc.top;
							int iBottom = iTop + szStatus.cy;
							m_sSelectedPushedImage.Format(_T("res='%s' restype='%s' dest='%d,%d,%d,%d' source='%d,%d,%d,%d'"), info.sImageName.GetData(), info.sResType.GetData(), info.rcDest.left, info.rcDest.top, info.rcDest.right, info.rcDest.bottom, iLeft, iTop, iRight, iBottom);
						}
					}
				}
			}


			if( (m_uButtonState & UISTATE_PUSHED) != 0 && !m_sSelectedPushedImage.IsEmpty()) {
				if( !DrawImage(hDC, (LPCTSTR)m_sSelectedPushedImage) ) {}
				else return;
			}
			else if( (m_uButtonState & UISTATE_HOT) != 0 && !m_sSelectedHotImage.IsEmpty()) {
				if( !DrawImage(hDC, (LPCTSTR)m_sSelectedHotImage) ) {}
				else return;
			}

			if( !m_sSelectedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sSelectedImage) ) {}
			}
		}
		else {
			__super::PaintStatusImage(hDC);
		}
	}

	void COptionLayoutUI::PaintForeImage(HDC hDC)
	{
		if(IsSelected()) {
			if( !m_sSelectedForeImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sSelectedForeImage) ) {}
				else return;
			}
		}

		return __super::PaintForeImage(hDC);
	}

	void COptionLayoutUI::PaintText(HDC hDC)
	{
		if( (m_uButtonState & UISTATE_SELECTED) != 0 )
		{
			DWORD oldTextColor = m_dwTextColor;
			if( m_dwSelectedTextColor != 0 ) m_dwTextColor = m_dwSelectedTextColor;

			if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
			if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

			int iFont = GetFont();
			if(GetSelectedFont() != -1) {
				iFont = GetSelectedFont();
			}
			CDuiString sText = GetText();
			if( sText.IsEmpty() ) return;
			int nLinks = 0;
			RECT rc = m_rcItem;
			RECT rcTextPadding = GetTextPadding();
			GetManager()->GetDPIObj()->Scale(&rcTextPadding);
			rc.left += rcTextPadding.left;
			rc.right -= rcTextPadding.right;
			rc.top += rcTextPadding.top;
			rc.bottom -= rcTextPadding.bottom;
			
			if( m_bShowHtml )
				CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
				NULL, NULL, nLinks, iFont, m_uTextStyle);
			else
				CRenderEngine::DrawText(hDC, m_pManager, rc, sText, IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, \
				iFont, m_uTextStyle);

			m_dwTextColor = oldTextColor;
		}
		else
			__super::PaintText(hDC);
	}

	void COptionLayoutUI::PaintBorder(HDC hDC) //add by liqs99
	{
		DWORD dwColor = 0;

		if( IsSelected() ) 
			dwColor = GetSelectedBorderColor();

		if(dwColor == 0)	
		{
			__super::PaintBorder(hDC);
			return;
		}

		int nBorderSize;
		SIZE cxyBorderRound;
		RECT rcBorderSize;
		if (m_pManager) {
			nBorderSize = GetManager()->GetDPIObj()->Scale(m_nBorderSize);
			cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
			rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
		}
		else {
			nBorderSize = m_nBorderSize;
			cxyBorderRound = m_cxyBorderRound;
			rcBorderSize = m_rcBorderSize;
		}

		if(nBorderSize > 0)
		{
			if(cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 ) 
			{
				CRenderEngine::DrawRoundRect(hDC, m_rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(dwColor), m_nBorderStyle);
			}
			else
			{
				CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, GetAdjustColor(dwColor), m_nBorderStyle);
			}
		}
		else
		{
			RECT rcBorder;

			if(rcBorderSize.left > 0){
				rcBorder		= m_rcItem;
				rcBorder.right	= rcBorder.left;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.left,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.top > 0){
				rcBorder		= m_rcItem;
				rcBorder.bottom	= rcBorder.top;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.top,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.right > 0){
				rcBorder		= m_rcItem;
				rcBorder.right -= 1;
				rcBorder.left	= rcBorder.right;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.right,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.bottom > 0){
				rcBorder		= m_rcItem;
				rcBorder.bottom -= 1;
				rcBorder.top	= rcBorder.bottom;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.bottom,GetAdjustColor(dwColor),m_nBorderStyle);
			}
		}
	}
}