#ifndef __UIIPADDRESSEX_H__
#define __UIIPADDRESSEX_H__

#pragma once

//给该控件添加一个属性dtstyle

#ifdef DUILIB_WIN32
namespace DuiLib
{
	
    /// IP输入控件
    class UILIB_API CIPAddressExUI : public CEditUI
    {
		DECLARE_DUICONTROL(CIPAddressExUI)
    public:
        CIPAddressExUI();
        virtual LPCTSTR GetClass() const override;
        virtual LPVOID GetInterface(LPCTSTR pstrName) override;
        virtual UINT GetControlFlags() const override;
        virtual void DoEvent(TEventUI& event) override;
        virtual void PaintText(UIRender *pRender) override;

        void SetIP(LPCWSTR lpIP);
        CDuiString GetIP();

    private:
        void CharToInt();
        void GetNumInput(TCHAR chKey);
        void UpdateText();
        void IncNum();
        void DecNum();

    protected:
        int m_nFirst;
        int m_nSecond;
        int m_nThird;
        int m_nFourth;
        int m_nActiveSection;

        TCHAR m_chNum;
        CDuiString m_strNum;
    };
}
#endif //#ifdef DUILIB_WIN32
#endif // __UIIPADDRESSEX_H__
