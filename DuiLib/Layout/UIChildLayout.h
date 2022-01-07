#ifndef __UICHILDLAYOUT_H__
#define __UICHILDLAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CChildLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CChildLayoutUI)
	public:
		CChildLayoutUI();

		virtual void Init() override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		void SetChildLayoutXML(CDuiString pXML);
		CDuiString GetChildLayoutXML();
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual LPCTSTR GetClass() const override;

	private:
		CDuiString m_pstrXMLFile;
	};
} // namespace DuiLib
#endif // __UICHILDLAYOUT_H__
