#pragma once
#include <ExDisp.h>
#include <ExDispid.h>
#include <mshtmhst.h>

namespace DuiLib
{
	class CWebBrowserEventHandler
	{
	public:
		CWebBrowserEventHandler() {}
		~CWebBrowserEventHandler() {}

		virtual void BeforeNavigate2(CControlUI* pWeb, IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel ) {}
		virtual void NavigateError(CControlUI* pWeb, IDispatch *pDisp,VARIANT * &url,VARIANT *&TargetFrameName,VARIANT *&StatusCode,VARIANT_BOOL *&Cancel) {}
		virtual void NavigateComplete2(CControlUI* pWeb, IDispatch *pDisp,VARIANT *&url){}
		virtual void ProgressChange(CControlUI* pWeb, LONG nProgress, LONG nProgressMax){}
		virtual void NewWindow3(CControlUI* pWeb, IDispatch **pDisp, VARIANT_BOOL *&Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl){}
		virtual void CommandStateChange(CControlUI* pWeb, long Command,VARIANT_BOOL Enable){};
		virtual void TitleChange(CControlUI* pWeb, BSTR bstrTitle){};
		virtual void DocumentComplete(CControlUI* pWeb, IDispatch *pDisp,VARIANT *&url){}

		// interface IDocHostUIHandler
		virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(CControlUI* pWeb, 
			/* [in] */ DWORD dwID,
			/* [in] */ POINT __RPC_FAR *ppt,
			/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
			/* [in] */ IDispatch __RPC_FAR *pdispReserved)
		{
			//return E_NOTIMPL;
			//返回 E_NOTIMPL 正常弹出系统右键菜单
			return S_OK;
			//返回S_OK 则可屏蔽系统右键菜单
		}

		virtual HRESULT STDMETHODCALLTYPE GetHostInfo(CControlUI* pWeb, 
			/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE ShowUI(CControlUI* pWeb, 
			/* [in] */ DWORD dwID,
			/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
			/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
			/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
			/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc)
		{
			return S_FALSE;
		}

		virtual HRESULT STDMETHODCALLTYPE HideUI( CControlUI* pWeb)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE UpdateUI( CControlUI* pWeb)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE EnableModeless(CControlUI* pWeb, 
			/* [in] */ BOOL fEnable)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate(CControlUI* pWeb, 
			/* [in] */ BOOL fActivate)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(CControlUI* pWeb, 
			/* [in] */ BOOL fActivate)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE ResizeBorder(CControlUI* pWeb, 
			/* [in] */ LPCRECT prcBorder,
			/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
			/* [in] */ BOOL fRameWindow)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(CControlUI* pWeb, 
			/* [in] */ LPMSG lpMsg,
			/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
			/* [in] */ DWORD nCmdID)
		{
			return S_FALSE;
		}

		virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath(CControlUI* pWeb, 
			/* [out] */ LPOLESTR __RPC_FAR *pchKey,
			/* [in] */ DWORD dw)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE GetDropTarget(CControlUI* pWeb, 
			/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
			/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE GetExternal(CControlUI* pWeb, 
			/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE TranslateUrl(CControlUI* pWeb, 
			/* [in] */ DWORD dwTranslate,
			/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
			/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE FilterDataObject(CControlUI* pWeb, 
			/* [in] */ IDataObject __RPC_FAR *pDO,
			/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
		{
			return S_OK;
		}

		// 	virtual HRESULT STDMETHODCALLTYPE GetOverrideKeyPath( 
		// 		/* [annotation][out] */ 
		// 		__deref_out  LPOLESTR *pchKey,
		// 		/* [in] */ DWORD dw)
		// 	{
		// 		return E_NOTIMPL;
		// 	}

		// IDownloadManager
		virtual HRESULT STDMETHODCALLTYPE Download( CControlUI* pWeb, 
			/* [in] */ IMoniker *pmk,
			/* [in] */ IBindCtx *pbc,
			/* [in] */ DWORD dwBindVerb,
			/* [in] */ LONG grfBINDF,
			/* [in] */ BINDINFO *pBindInfo,
			/* [in] */ LPCOLESTR pszHeaders,
			/* [in] */ LPCOLESTR pszRedir,
			/* [in] */ UINT uiCP)
		{
			return S_OK;
		}
	};
}
