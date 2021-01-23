#include "StdAfx.h"
#include "DuiEditor.h"
#include "ScriptLexer.h"

#include "../DuiScript/angelscript/angelscript/source/as_tokenizer.h"

CScriptLexer::CScriptLexer(void)
{
	engine = NULL;
}


CScriptLexer::~CScriptLexer(void)
{
	std::list<tagMyIdentifier *>::iterator it;
	for (it=m_arrToken.begin(); it!=m_arrToken.end(); it++)
		delete (tagMyIdentifier *)(*it);
	m_arrToken.clear();

	for (it=m_arrUser.begin(); it!=m_arrUser.end(); it++)
		delete (tagMyIdentifier *)(*it);
	m_arrUser.clear();
}

void CScriptLexer::SetEngine(asIScriptEngine *eng)
{
	engine = eng;
}

//初始化语法提示，载入系统类，全局函数，全局变量
BOOL CScriptLexer::InitLexer(std::string &strKeyWord, std::string &strKeyWord2, std::string &strKeyWord3, std::string &strKeyWord4)
{
	if(!engine)	return FALSE;

	int n = 0;

	//关键字
	for( asUINT n = 0; n < numTokenWords; n++ )
	{
		char ch = tokenWords[n].word[0];
		if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))
		{
			strKeyWord += tokenWords[n].word;
			strKeyWord += " ";

			tagMyIdentifier *p = new tagMyIdentifier;
			p->Type = 1;
			p->strName = tokenWords[n].word;
			p->strNameNoCase = tokenWords[n].word; p->strNameNoCase.MakeLower();
			m_arrToken.push_back(p);
		}
	}

	//全局函数
	n = engine->GetGlobalFunctionCount();	
	for (int i=0; i<n; i++)
	{
		asIScriptFunction *pFun = engine->GetGlobalFunctionByIndex(i);
		if(pFun)
		{
			strKeyWord2 += pFun->GetName();
			strKeyWord2 += " ";

			tagMyIdentifier *p = new tagMyIdentifier;
			p->Type = 2;
			p->strName = pFun->GetName();
			p->strNameNoCase = pFun->GetName(); p->strNameNoCase.MakeLower();
			m_arrToken.push_back(p);
		}	
	}

	//全局变量
	n = engine->GetGlobalPropertyCount();
	for (int i=0; i<n; i++)
	{
		const char *name;
		int nRet = engine->GetGlobalPropertyByIndex(i, &name); 
		if(nRet >= 0)
		{
			strKeyWord2 += name;
			strKeyWord2 += " ";

			tagMyIdentifier *p = new tagMyIdentifier;
			p->Type = 3;
			p->strName = name;
			p->strNameNoCase = name; p->strNameNoCase.MakeLower();
			m_arrToken.push_back(p);
		}
	}

	//系统类
	n = engine->GetObjectTypeCount();
	for (int i=0; i<n; i++)
	{
		asITypeInfo *pObject = engine->GetObjectTypeByIndex(i);
		if(pObject)
		{
			strKeyWord += pObject->GetName();
			strKeyWord += " ";

			tagMyIdentifier *p = new tagMyIdentifier;
			p->Type =  4;
			p->strName = pObject->GetName();
			p->strNameNoCase = pObject->GetName(); p->strNameNoCase.MakeLower();
			m_arrToken.push_back(p);
		}
	}

	return TRUE;
}

//获取语法提示字符串
std::string CScriptLexer::GetLexerText(const char *szCode, int parselength, const char *szWord)
{
	EmptyLexerText();

	int nCodeLen = strlen(szCode); //parselength + 1;
	int nWordLen = strlen(szWord);
	CStringA strWord = szWord;
	strWord.MakeLower();

	CStringA sClassVar, sClassMethod, temp;
	const char *p = szCode + parselength;
	int flag = -1;
	while(p)
	{
		if( (*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9') )  
			break;
		if(*p == '.') break;
		if(*p == ' ' || *p == '\n' || *p == ';') 
			return MakeLexerText();
		--p;
	}

	while (p && p != szCode)
	{
		if( (*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9') )
		{
			temp.Insert(0, *p);
		}
		else if(*p == '.')
		{
			flag = 0;
			sClassMethod = temp;
			temp.Empty();
		}
		else
		{
			break;
		}
  		p--;
	}
	if(flag == 0)
	{
		sClassVar = temp;
	}

	//扫描当前源代码，不能每次都重新扫描，太慢了
	//ParseToken(szCode, nCodeLen);

	if(sClassVar.IsEmpty())
	{
		std::list<tagMyIdentifier *>::iterator it;
		for (it=m_arrUser.begin(); it!=m_arrUser.end(); it++)
		{
			tagMyIdentifier *p = *it;
			if(p->strNameNoCase.Find(strWord) == 0)
				AddLexerText(p->strName);
		}
		for (it=m_arrToken.begin(); it!=m_arrToken.end(); it++)
		{
			tagMyIdentifier *p = *it;
			if(p->strNameNoCase.Find(strWord) == 0)
				AddLexerText(p->strName);
		}
	}
	else	//匹配类的成员函数
	{
		std::list<tagMyIdentifier *>::iterator it;
		for (it=m_arrUser.begin(); it!=m_arrUser.end(); it++)
		{
			tagMyIdentifier *p = *it;
			if(sClassVar == p->strName)
			{
				sClassMethod.MakeLower();
				GetLexerTextMethod(p->strDataTypeName, sClassVar, sClassMethod);
			}
		}
	}


	return MakeLexerText();
}


void CScriptLexer::GetLexerTextMethod(const char *szClass, const char *var, const char *szMethod)
{
	for (asUINT x=0; x<engine->GetObjectTypeCount(); x++)
	{
		asITypeInfo *pTypeInfo = engine->GetObjectTypeByIndex(x);
		if(strcmp(szClass, pTypeInfo->GetName()) == 0)
		{
			for (asUINT i=0; i<pTypeInfo->GetMethodCount(); i++)
			{
				asIScriptFunction *pFun = pTypeInfo->GetMethodByIndex(i, true);
				if(pFun)
				{
					CStringA strClassMethod = pFun->GetName(); strClassMethod.MakeLower();
					if(strClassMethod.Find(szMethod) >= 0)
					{
						const char *fname = pFun->GetName();
						if(fname)
						{
							if(fname[0] == 'o' && fname[1] == 'p')
							{
								continue; //op开头的函数不要列出来，可能是运算符重载的函数
							}
						}
						AddLexerText(pFun->GetName());
					}
				}
			}

			for (asUINT i=0; i<pTypeInfo->GetPropertyCount(); i++)
			{
				const char *name;
				if(pTypeInfo->GetProperty(i, &name) >= 0)
				{
					CStringA strClassProperty = name; strClassProperty.MakeLower();
					if(strClassProperty.Find(szMethod) >= 0)
					{
						AddLexerText(name);
					}
				}

			}

			break;
		}
	}
}

std::string CScriptLexer::GetFunctionTips(const char *szWord)
{
	std::string strTips;
	if(!szWord) return strTips;

	//全局函数
	int n = engine->GetGlobalFunctionCount();	
	for (int i=0; i<n; i++)
	{
		asIScriptFunction *pFun = engine->GetGlobalFunctionByIndex(i);
		if(pFun)
		{
			if(strcmp(pFun->GetName(), szWord) == 0)
			{
				if(!strTips.empty()) strTips += "\n";
				strTips += pFun->GetDeclaration();
			}
		}	
	}
	
	return strTips;
}

std::string CScriptLexer::GetMethodTips(const char *szClassVar, const char *szMethod)
{
	std::string strTips;
	if(!szClassVar || !szMethod) return strTips;

	std::list<tagMyIdentifier *>::iterator it;
	for (it=m_arrUser.begin(); it!=m_arrUser.end(); it++)
	{
		tagMyIdentifier *p = *it;
		if(strcmp(szClassVar, p->strName) != 0)
			continue;
		
		for (asUINT x=0; x<engine->GetObjectTypeCount(); x++)
		{
			asITypeInfo *pTypeInfo = engine->GetObjectTypeByIndex(x);
			if(strcmp(p->strDataTypeName, pTypeInfo->GetName()) != 0)
				continue;

			for (asUINT i=0; i<pTypeInfo->GetMethodCount(); i++)
			{
				asIScriptFunction *pFun = pTypeInfo->GetMethodByIndex(i, true);
				if(pFun)
				{
					if(pFun)
					{
						if(strcmp(pFun->GetName(), szMethod) == 0)
						{
							if(!strTips.empty()) strTips += "\n";
							strTips += pFun->GetDeclaration();
						}
					}	
				}
			}
			break;	
		}	
		break;
	}

	return strTips;
}

std::string CScriptLexer::MakeLexerText() 
{ 
	return std::string(m_sLexerText);
}

void CScriptLexer::EmptyLexerText()
{
	m_sLexerText.Empty();
}

BOOL CScriptLexer::AddLexerText(const char *text)
{
	int len = strlen(text);
	int iStart = 0;
	for (;;)
	{
		int pos = m_sLexerText.Find(text, iStart);
		if(pos < 0) break;

		if(pos == 0)
		{
			if(m_sLexerText[pos+len] == ' ')
				return FALSE;
		}
		else
		{
			if(m_sLexerText[pos-1] == ' ' && m_sLexerText[pos+len] == ' ')
				return FALSE;
		}
		iStart += pos + len;
	}

	m_sLexerText += text;
	m_sLexerText += " ";
	return TRUE;
}

void CScriptLexer::ParseToken(const char *szCode, int nCodeLen)
{
	//先清理掉上次分析的结果
	std::list<tagMyIdentifier *>::iterator it;
	for (it=m_arrUser.begin(); it!=m_arrUser.end(); it++)
		delete (tagMyIdentifier *)(*it);
	m_arrUser.clear();

	int pos = 0;
	size_t len = 0;
	asETokenClass lastTC = asTC_UNKNOWN;
	CStringA lastTcName;
	asETokenClass tc = asTC_UNKNOWN;
	CStringA tcName;
	while (pos <= nCodeLen)
	{
		asETokenClass tc = engine->ParseToken(szCode+pos, nCodeLen-pos, &len);
		CStringA tcName = CStringA(szCode+pos, len);
		switch (tc)
		{
		case asTC_UNKNOWN:
			{
				//InsertMsg(LSUTF82T(tcName) + _T(" asTC_UNKNOWN"));
				lastTC = asTC_UNKNOWN;
				lastTcName.Empty();
			}
			break;
		case asTC_KEYWORD:
			{
				//InsertMsg(LSUTF82T(tcName) + _T(" asTC_KEYWORD"));
				
				if(IsDataTypeIdentifier(tcName))
				{
					lastTC = asTC_KEYWORD;
					lastTcName = tcName;
				}
				else if(IsDataTypeIdentifier(lastTcName) || IsClass(lastTcName))
				{
					if(tcName != ',' && tcName != '&' && tcName != '@')
					{
						lastTC = asTC_UNKNOWN;
						lastTcName.Empty();
					}
				}
				else
				{
					lastTC = asTC_UNKNOWN;
					lastTcName.Empty();
				}
			}
			break;
		case asTC_VALUE:
			{
				//InsertMsg(LSUTF82T(tcName) + _T(" asTC_VALUE"));
				lastTC = asTC_UNKNOWN;
				lastTcName.Empty();
			}
			break;
		case asTC_IDENTIFIER:
			{
				//InsertMsg(LSUTF82T(tcName) + _T(" asTC_IDENTIFIER"));

				if(IsDataTypeIdentifier(lastTcName))
				{
					//InsertMsgV(_T("定义变量 %s"), LSUTF82T(tcName));
					tagMyIdentifier *p = new tagMyIdentifier;
					p->Type = 3;
					p->strName = tcName;
					p->strNameNoCase = tcName; p->strNameNoCase.MakeLower();
					p->strDataTypeName = lastTcName;
					m_arrUser.push_back(p);
				}
				else if(IsClass(lastTcName))
				{
					//InsertMsgV(_T("定义变量 %s"), LSUTF82T(tcName));
					tagMyIdentifier *p = new tagMyIdentifier;
					p->Type = 5;
					p->strName = tcName;
					p->strNameNoCase = tcName; p->strNameNoCase.MakeLower();
					p->strDataTypeName = lastTcName;
					m_arrUser.push_back(p);
				}
				else
				{
					lastTC = tc;
					lastTcName = tcName;
				}
			}
			break;
		case asTC_COMMENT:
			{
				//InsertMsg(LSUTF82T(tcName) + _T(" asTC_COMMENT"));
				lastTC = asTC_UNKNOWN;
				lastTcName.Empty();
			}
			break;
		case asTC_WHITESPACE:
			{
				//InsertMsg(LSUTF82T(tcName) + _T(" asTC_WHITESPACE"));
			}
			break;
		}

		pos += len;
	}
}

BOOL CScriptLexer::IsClass(const char *szWord)
{
	std::list<tagMyIdentifier *>::iterator it;
	for (it=m_arrToken.begin(); it!=m_arrToken.end(); it++)
	{
		tagMyIdentifier *p = *it;
		if(p->Type == 4 && p->strName == szWord)
			return TRUE;
	}
	return FALSE;
}

BOOL CScriptLexer::IsClassMember(const char *szWord)
{
	return FALSE;
}

BOOL CScriptLexer::IsDataTypeIdentifier(const char *szWord)
{
	if(strcmp(szWord, "void") == 0		||
		strcmp(szWord, "int8") == 0		||
		strcmp(szWord, "int16") == 0	||
		strcmp(szWord, "int") == 0		||
		strcmp(szWord, "int64") == 0	||
		strcmp(szWord, "uint8") == 0	||
		strcmp(szWord, "uint16") == 0	||
		strcmp(szWord, "uint") == 0		||
		strcmp(szWord, "uint64") == 0	||
		strcmp(szWord, "float") == 0	||
		strcmp(szWord, "double") == 0	||
		strcmp(szWord, "bool") == 0		)
		return TRUE;
	return FALSE;
}