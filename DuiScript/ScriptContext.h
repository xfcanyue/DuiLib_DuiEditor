#pragma once
#include "Include/DuiScriptInterface.h"

namespace DuiLib
{


class UISCRIPT_API CScriptContext : public IScriptContext
{
public:
	CScriptContext(IScriptManager *pScriptManager);
	virtual ~CScriptContext(void);

	virtual int SetFunByName(LPCTSTR lpszFunName) override;
	virtual int SetFunByDecl(LPCTSTR lpszFunDecl) override;

	virtual int	SetArgByte(UINT arg, BYTE value) override;
	virtual int	SetArgWord(UINT arg, WORD value) override;
	virtual int	SetArgDWord(UINT arg, DWORD value) override;
	virtual int	SetArgFloat(UINT arg, float value) override;
	virtual int	SetArgDouble(UINT arg, double value) override;
	virtual int	SetArgAddress(UINT arg, void *addr) override;
	virtual int	SetArgObject(UINT arg, void *obj) override;
	virtual void *  GetAddressOfArg(UINT arg) override;

	virtual int Execute() override;

	virtual int Abort() override;

	virtual BYTE	GetReturnByte() override;
	virtual WORD	GetReturnWord() override;
	virtual DWORD	GetReturnDWord() override;
	virtual float	GetReturnFloat() override;
	virtual double	GetReturnDouble() override;
	virtual void *	GetReturnAddress() override;
	virtual void *	GetReturnObject() override;
	virtual void *	GetAddressOfReturnValue() override;

	asIScriptContext *CTX() const { return m_ctx; }
protected:
	void ContextLineCallback(asIScriptContext *ctx);
protected:
	IScriptManager *m_pScriptManager;
	asIScriptEngine *m_pEngine;
	asIScriptContext *m_ctx;
	DWORD m_dwStartTime;
};

}