#include "StdAfx.h"
#include "UIScript.h"


///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	//////////////////////////////////////////////////////////////////////////
	//
	//
	IScriptFunction::IScriptFunction()
	{

	}

	IScriptFunction::~IScriptFunction()
	{
		for (int i=0; i<m_arrArgs.GetSize(); i++)
		{
			TArgItem *pItem = (TArgItem *)m_arrArgs.GetAt(i);
			delete pItem;
		}
		m_arrArgs.Empty();
	}
	
	void IScriptFunction::SetReturnType(emUIArgTypes argType)
	{
		m_emReturnArgTypes = argType;
	}

	//设置函数名
	void IScriptFunction::SetFuctionName(LPCTSTR sFunName)
	{
		m_sFunName = sFunName;
	}

	void IScriptFunction::PushArgBool(bool value)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_bool;
		pItem->_byte = value;
		m_arrArgs.Add(pItem);
	}

	//从左到右入栈参数
	void IScriptFunction::PushArgByte(BYTE value)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_BYTE;
		pItem->_byte = value;
		m_arrArgs.Add(pItem);
	}

	void IScriptFunction::PushArgWord(WORD value)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_WORD;
		pItem->_word = value;
		m_arrArgs.Add(pItem);
	}

	void IScriptFunction::PushArgDWord(DWORD value)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_DWORD;
		pItem->_dword = value;
		m_arrArgs.Add(pItem);
	}

	void IScriptFunction::PushArgFloat(float value)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_float;
		pItem->_float = value;
		m_arrArgs.Add(pItem);
	}

	void IScriptFunction::PushArgDouble(double value)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_double;
		pItem->_double = value;
		m_arrArgs.Add(pItem);
	}

	void IScriptFunction::PushArgAddress(void *addr)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_address;
		pItem->_addr = addr;
		m_arrArgs.Add(pItem);
	}

	void IScriptFunction::PushArgObject(void *obj)
	{
		TArgItem *pItem = new TArgItem;
		memset(pItem, 0, sizeof(TArgItem));
		pItem->_type = UIArg_object;
		pItem->_obj = obj;
		m_arrArgs.Add(pItem);
	}

	bool IScriptFunction::GetReturnBool()
	{
		return m_result._bool;
	}

	BYTE IScriptFunction::GetReturnByte()
	{
		return m_result._byte;
	}

	WORD IScriptFunction::GetReturnWord()
	{
		return m_result._word;
	}

	DWORD IScriptFunction::GetReturnDWord()
	{
		return m_result._dword;
	}

	float IScriptFunction::GetReturnFloat()
	{
		return m_result._float;
	}

	double IScriptFunction::GetReturnDouble()
	{
		return m_result._double;
	}

	void *IScriptFunction::GetReturnAddress()
	{
		return m_result._addr;
	}

	void *IScriptFunction::GetReturnObject()
	{
		return m_result._obj;
	}

} // namespace DuiLib
