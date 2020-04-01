#pragma once
#include "IScriptManager.h"
//#include "../../3rd/AngelScript/Include/AngelScript.h"

namespace DuiLib
{
	
class CScriptManager : public IScriptManager
{
public:
	CScriptManager(void);
	virtual ~CScriptManager(void);

	virtual void AddScriptContext(LPCTSTR pScriptCode);
	virtual void AddScriptFile(LPCTSTR pstrFileName);
	virtual bool ExecuteScript(LPCTSTR funName, void *pControl);
	virtual bool ExecuteScript(LPCTSTR funName, void *pControl, void *ev);
};

}