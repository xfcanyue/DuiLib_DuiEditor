#ifndef __UITEXT_H__
#define __UITEXT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CTextUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CTextUI)
	public:
		CTextUI();
		~CTextUI();

		virtual LPCTSTR GetClass() const override;
		virtual UINT GetControlFlags() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void DoEvent(TEventUI& event) override;
	};

} // namespace DuiLib

#endif //__UITEXT_H__