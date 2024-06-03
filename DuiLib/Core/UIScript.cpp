#include "StdAfx.h"
#include "UIScript.h"


///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	//////////////////////////////////////////////////////////////////////////
	//
	//
	CAutoScriptContext::CAutoScriptContext(IScriptManager *mgr)
	{
		this->mgr = mgr;
		ctx = mgr->CreateContext();
	}
	CAutoScriptContext::~CAutoScriptContext()
	{
		if(ctx)
		{
			mgr->ReleaseContext(ctx);
			ctx = NULL;
		}
	}

	IScriptContext* CAutoScriptContext::operator->() const throw()
	{
		return ctx;
	}

	bool CAutoScriptContext::operator!() const throw()
	{
		return (ctx == NULL);
	}

} // namespace DuiLib
