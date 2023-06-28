#ifndef __UIACTIVEX_H__
#define __UIACTIVEX_H__

#pragma once

//struct IOleObject;


#ifdef DUILIB_WIN32
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	//class CActiveXCtrl;

	template< class T >
	class CSafeRelease
	{
	public:
		CSafeRelease(T* p) : m_p(p) { };
		~CSafeRelease() { if( m_p != NULL ) m_p->Release(); };
		T* Detach() { T* t = m_p; m_p = NULL; return t; };
		T* m_p;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CActiveXUI : public CControlUI, public IMessageFilterUI
	{
		DECLARE_DUICONTROL(CActiveXUI)

		friend class CActiveXCtrl;
	public:
		CActiveXUI();
		virtual ~CActiveXUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		HWND GetHostWindow() const;

		virtual bool IsDelayCreate() const;
		virtual void SetDelayCreate(bool bDelayCreate = true);
		virtual bool IsMFC() const;
		virtual void SetMFC(bool bMFC = false);

		bool CreateControl(const CLSID clsid);
		bool CreateControl(LPCTSTR pstrCLSID);
		HRESULT GetControl(const IID iid, LPVOID* ppRet);
		CLSID GetClisd() const;
		CDuiString GetModuleName() const;
		void SetModuleName(LPCTSTR pstrText);

		virtual void SetVisible(bool bVisible = true) override;
		virtual void SetInternVisible(bool bVisible = true) override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true) override;
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

	protected:
		virtual void ReleaseControl();
		virtual bool DoCreateControl();

	protected:
		CLSID m_clsid;
		CDuiString m_sModuleName;
		bool m_bCreated;
		bool m_bDelayCreate;
		bool m_bMFC;
		IOleObject* m_pUnk;
		CActiveXCtrl* m_pControl;
		HWND m_hwndHost;
	};

} // namespace DuiLib

#endif //#ifdef DUILIB_WIN32
#endif // __UIACTIVEX_H__

