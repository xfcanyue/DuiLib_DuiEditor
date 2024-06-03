#include "StdAfx.h"
#include "UIQRCode.h"
//#include "atlimage.h"

#include "../Render/UIRender_gdi.h"
#include "../Utils/QREncode.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CQRCodeUI)
CQRCodeUI::CQRCodeUI(void)
{
	m_qrLevel = 0;
	m_qrVersion = 0;
	m_qrMarkNo = -1;
	m_qrSize = 0;
	m_qrMargin = 4;
	m_sEncoding = _T("unicode");
}


CQRCodeUI::~CQRCodeUI(void)
{
}


LPCTSTR CQRCodeUI::GetClass() const
{
	return _T("QRCodeUI");
}

LPVOID	CQRCodeUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_QRCODE) == 0 ) return static_cast<CQRCodeUI*>(this);
	return CDynamicLayoutUI::GetInterface(pstrName);
}

void CQRCodeUI::SetQrLevel(int nValue)
{
	if(nValue == m_qrLevel) return;
	m_qrLevel = nValue;
	Invalidate();
}

int CQRCodeUI::GetQrLevel() const
{
	return m_qrLevel;
}

void CQRCodeUI::SetQrVersion(int nValue)
{
	if(nValue == m_qrVersion) return;
	m_qrVersion = nValue;
	Invalidate();
}

int CQRCodeUI::GetQrVersion() const
{
	return m_qrVersion;
}

void CQRCodeUI::SetQrMarkNo(int nValue)
{
	if(nValue == m_qrMarkNo) return;
	m_qrMarkNo = nValue;
	Invalidate();
}

int CQRCodeUI::GetQrMarkNo() const
{
	return m_qrMarkNo;
}

void CQRCodeUI::SetQrSize(int nValue)
{
	if(nValue == m_qrSize) return;
	m_qrSize = nValue;
	Invalidate();
}

int CQRCodeUI::GetQrSize() const
{
	return m_qrSize;
}

void CQRCodeUI::SetQrMargin(int nValue)
{
	if(nValue == m_qrMargin) return;
	m_qrMargin = nValue;
	Invalidate();
}

int CQRCodeUI::GetQrMargin() const
{
	return m_qrMargin;
}

void CQRCodeUI::SetQrCodeText(LPVOID pData, int len)
{
	m_qrText.InitBuffer();
	m_qrText.AddBuffer(pData, len);
	Invalidate();
}

void CQRCodeUI::SetQrCodeEncoding(LPCTSTR sEncoding)
{
	m_sEncoding = sEncoding;
}

void CQRCodeUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CDynamicLayoutUI::SetPos(rc, bNeedInvalidate);
}

void CQRCodeUI::PaintText(UIRender *pRender)
{
	if(m_sText.IsEmpty())
		return;

	MakeQrImage();
	if(m_qrRender)
	{
		pRender->StretchBlt(m_rcCode.left, m_rcCode.top, m_rcCode.GetWidth(), m_rcCode.GetHeight(), 
			m_qrRender, 0, 0, m_qrRender->GetWidth(), m_qrRender->GetHeight());
	}
}


void CQRCodeUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("qrlevel")) == 0 ) 
	{
		SetQrLevel(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("qrversion")) == 0 ) 
	{
		SetQrVersion(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("qrmark")) == 0 ) 
	{
		SetQrMarkNo(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("qrsize")) == 0 ) 
	{
		SetQrSize(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("qrmargin")) == 0 ) 
	{
		SetQrMargin(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("encoding")) == 0 ) 
	{
		SetQrCodeEncoding(pstrValue);
	}
	else
		CDynamicLayoutUI::SetAttribute(pstrName, pstrValue);
}

void CQRCodeUI::MakeQrImage()
{
	if(!m_qrRender)
	{
		m_qrRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		m_qrRender->Init(NULL,NULL);
	}

	if(!GetText().IsEmpty())
	{
		m_qrText.InitBuffer();
		UISTRING_CONVERSION;
		if(m_sEncoding.CompareNoCase(_T("ascii")) == 0)
		{
			LPCSTR s = UIT2A(GetText());
			m_qrText.AddBuffer(s, strlen(s));
		}
		else if(m_sEncoding.CompareNoCase(_T("unicode")) == 0)
		{
			LPCWSTR s = UIT2W(GetText());
			m_qrText.AddBuffer(s, wcslen(s));
		}
		else if(m_sEncoding.CompareNoCase(_T("utf8")) == 0)
		{
			LPCSTR s = UIT2UTF8(GetText());
			m_qrText.AddBuffer(s, strlen(s));
		}
	}

	CDuiRect rcControl = GetPos();

	//产生二维码图片
	uinner::CQR_Encode qr;
	int nLevel = MAX(MIN(m_qrLevel,3),0);
	int nVersion = MAX(MIN(m_qrVersion,40),0);
	BOOL bAutoExtent = nVersion == 0;
	int nMaskingNo = MAX(MIN(m_qrMarkNo,7),-1);
	if(qr.EncodeData(nLevel, nVersion, bAutoExtent, nMaskingNo, m_qrText.GetBuffer(), m_qrText.GetLength()))
	{
		int size = qr.m_nSymbleSize + (m_qrMargin * 2);

		m_qrRender->Resize(size,size);
		m_qrRender->DrawBackColor(CDuiRect(0,0,size,size), CDuiSize(0,0), UIRGB(255,255,255));
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				if (qr.m_byModuleData[i][j])
				{
					m_qrRender->SetPixel(i + m_qrMargin, j + m_qrMargin, RGB(0, 0, 0));
				}
			}
		}

		m_rcCode = CDuiRect();
		if(m_qrSize <= 0)
		{
			//非整数倍放大时,小数取整,可能会导致失真无法扫描，所以不要实际铺满控件
			int qrsize = 1;
			while(TRUE)
			{
				if(rcControl.GetWidth() < (qrsize+1) * size)
					break;
				if(rcControl.GetHeight() < (qrsize+1) * size)
					break;
				qrsize++;
			}
			m_rcCode.right = qrsize * size;
			m_rcCode.bottom = qrsize * size;
		}
		else
		{
			m_rcCode.right = m_qrSize * size;
			m_rcCode.bottom = m_qrSize * size;
		}

		m_rcCode.right = MIN(m_rcCode.right, rcControl.GetWidth());
		m_rcCode.bottom = MIN(m_rcCode.bottom, rcControl.GetHeight());

		//居中显示
		m_rcCode.AlignRect(rcControl, DT_CENTER|DT_VCENTER);
	}
}

} //namespace DuiLib
