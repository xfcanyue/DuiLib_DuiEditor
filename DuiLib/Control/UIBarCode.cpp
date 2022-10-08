#include "StdAfx.h"
#include "UIBarCode.h"

#include "../Render/UIRender_gdi.h"
#include "../Utils/barcode.h"
namespace DuiLib
{

struct CBarCodeUI::Imp
{
	Imp()
	{
		m_pBarCodeImpl = NULL;
	}

	~Imp()
	{
		delete m_pBarCodeImpl; m_pBarCodeImpl = NULL;
	}

	BarcodeBase *GetImpl(LPCTSTR codetype)
	{
		if((m_sType.CompareNoCase(codetype) == 0) && (m_pBarCodeImpl != NULL))
			return m_pBarCodeImpl;

		m_sType = codetype;
		if(m_pBarCodeImpl != NULL) { delete m_pBarCodeImpl; m_pBarCodeImpl = NULL; }

		if(m_sType.CompareNoCase(_T("code39")) == 0)
		{
			m_pBarCodeImpl = new Barcode39;
		}
		else if(m_sType.CompareNoCase(_T("code93")) == 0)
		{
			m_pBarCodeImpl = new Barcode93;
		}
		else if(m_sType.CompareNoCase(_T("code128")) == 0)
		{
			m_pBarCodeImpl = new Barcode128;
		}
		return m_pBarCodeImpl;
	}
protected:
	BarcodeBase *m_pBarCodeImpl;
	CDuiString m_sType;
};

IMPLEMENT_DUICONTROL(CBarCodeUI)
CBarCodeUI::CBarCodeUI(void)
{
	m_sBarCodeType = _T("code39");
	m_nBarCodeSize = 1;
	m_pImpl = new CBarCodeUI::Imp;
}


CBarCodeUI::~CBarCodeUI(void)
{
	delete m_pImpl; m_pImpl = NULL;
}


LPCTSTR CBarCodeUI::GetClass() const
{
	return _T("BarCodeUI");
}

LPVOID	CBarCodeUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_BARCODE) == 0 ) return static_cast<CBarCodeUI*>(this);
	return __super::GetInterface(pstrName);
}

void CBarCodeUI::SetBarCodeType(LPCTSTR sValue)
{
	if(m_sBarCodeType.CompareNoCase(sValue) == 0)
		return;

	m_sBarCodeType = sValue;
	MakeBarcodeImage();
	Invalidate();
}

CString CBarCodeUI::GetBarCodeType() const
{
	return m_sBarCodeType;
}

void CBarCodeUI::SetBarCodeSize(int nValue)
{
	if(m_nBarCodeSize == nValue)
		return;
	m_nBarCodeSize = nValue;
	MakeBarcodeImage();
	Invalidate();
}

int CBarCodeUI::GetBarCodeSize() const
{
	return m_nBarCodeSize;
}

void CBarCodeUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	__super::SetPos(rc, bNeedInvalidate);
	MakeBarcodeImage();
}

void CBarCodeUI::PaintText(UIRender *pRender)
{
	if(m_sText.IsEmpty())
		return;

	if(m_qrRender)
	{
		CDuiRect rc = GetPos();
		pRender->StretchBlt(rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), 
			m_qrRender, 0, 0, m_rcCode.GetWidth(), m_rcCode.GetHeight());
	}
}

void CBarCodeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("barcodetype")) == 0 ) 
	{
		SetBarCodeType(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("barcodesize")) == 0 ) 
	{
		SetBarCodeSize(_ttoi(pstrValue));
	}
	else
		__super::SetAttribute(pstrName, pstrValue);
}

void CBarCodeUI::MakeBarcodeImage()
{
	if(!m_qrRender)
	{
		m_qrRender = MakeRefPtr<UIRender>(new UIRender_gdi);
		m_qrRender->Init(NULL,NULL);
	}

	CDuiRect rc = GetPos();
	BarcodeBase *pbarcode = m_pImpl->GetImpl(GetBarCodeType());
	if(pbarcode)
	{
		UISTRING_CONVERSION;
		pbarcode->Clear();
		pbarcode->Encode(UIT2A(GetText()));

		m_rcCode.left = 0;
		m_rcCode.right = rc.GetWidth();
		m_rcCode.top = 0;
		m_rcCode.bottom = rc.GetHeight();

		m_qrRender->Resize(m_rcCode.GetWidth(), m_rcCode.GetHeight());
		m_qrRender->DrawBackColor(m_rcCode, CDuiSize(0,0), UIRGB(255,255,255));

		m_rcCode.right = 0;

		int i_Ratio = pbarcode->GetRatio();
		BYTE*pb = pbarcode->GetBuffer();
		int i0, iNum0 = pbarcode->GetBufferLength();

		BYTE bBar;
		int i1, iNum1;
		int iY;
		int iX = m_rcCode.left;
		for (i0 = 0; i0 < iNum0; i0++) {
			bBar = *pb & 0x01;
			iNum1 = (*pb & 0x02) ? i_Ratio : 1;
			iY = m_rcCode.bottom;//(*pb & 0x04) ? m_rcCode.bottom : m_rcCode.top;
			for (i1 = 0; i1 < iNum1; i1++) {
				if (bBar)	
					m_qrRender->DrawLine(iX, m_rcCode.top, iX, iY, GetBarCodeSize(), UIRGB(0,0,0));
				else
					m_qrRender->DrawLine(iX, m_rcCode.top, iX, iY, GetBarCodeSize(), UIRGB(255,255,255));
				iX += GetBarCodeSize();
				if(iX > m_rcCode.right) 
					m_rcCode.right = iX;
			}
			pb++;
		}
	}
}

}
