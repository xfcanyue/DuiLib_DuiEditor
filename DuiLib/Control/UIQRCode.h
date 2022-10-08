#pragma once

namespace DuiLib
{

class CQRCodeUI : public CDynamicLayoutUI
{
	DECLARE_DUICONTROL(CQRCodeUI)
public:
	CQRCodeUI(void);
	virtual ~CQRCodeUI(void);

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	//纠错等级
	void SetQrLevel(int nValue);
	int GetQrLevel() const;

	//版本
	void SetQrVersion(int nValue);
	int GetQrVersion() const;

	//掩码等级
	void SetQrMarkNo(int nValue);
	int GetQrMarkNo() const;

	//图像大小
	void SetQrSize(int nValue);
	int GetQrSize() const;

	//边距
	void SetQrMargin(int nValue);
	int GetQrMargin() const;

	//设置二维码内容，当GetText()为空时有效。
	void SetQrCodeText(LPVOID pData, int len);

	//二维码文本字符串编码，当GetText()不为空时有效。
	void SetQrCodeEncoding(LPCTSTR sEncoding);

	virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
	virtual void PaintText(UIRender *pRender) override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

protected:
	void MakeQrImage();
protected:
	//纠错等级
	int m_qrLevel;

	//二维码版本
	int m_qrVersion;

	//掩码等级
	int m_qrMarkNo;

	//图形大小
	int m_qrSize;

	//边缘大小
	int m_qrMargin;

	CDuiString m_sEncoding;

	CBufferUI m_qrText;
	CDuiRect m_rcCode;
	CStdRefPtr<UIRender> m_qrRender;
};

}
