#pragma once

//×¢²á¸÷ÖÖ¿Ø¼þ
class CScriptRegister
{
public:
	CScriptRegister(asIScriptEngine *engine);
	~CScriptRegister();

	void RegisterAll();

private:
	asIScriptEngine *engine;
};

