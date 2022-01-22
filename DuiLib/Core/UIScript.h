#ifndef __UISCRIPT_H__
#define __UISCRIPT_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	//脚本参数类型
	enum emUIArgTypes
	{
		UIArg_void		= 0,
		UIArg_BYTE,
		UIArg_WORD,
		UIArg_DWORD,
		UIArg_float,
		UIArg_double,
		UIArg_address,
		UIArg_object,
	};

	//通用脚本函数调用
	class UILIB_API IScriptFunction
	{
	public:
		struct TArgItem
		{
			emUIArgTypes _type;
			BYTE _byte;
			WORD _word;
			DWORD _dword;
			float _float;
			double _double;
			void *_addr;
			void *_obj;
		};
	public:
		IScriptFunction();
		virtual ~IScriptFunction();

		//设置函数返回值类型
		virtual void SetReturnType(emUIArgTypes argType);

		//设置函数名
		virtual void SetFuctionName(LPCTSTR sFunName);

		//从左到右入栈参数
		virtual void PushArgByte(BYTE value);
		virtual void PushArgWord(WORD value);
		virtual void PushArgDWord(DWORD value);
		virtual void PushArgFloat(float value);
		virtual void PushArgDouble(double value);
		virtual void PushArgAddress(void *addr);
		virtual void PushArgObject(void *obj);

		//获取返回值
		virtual BYTE	GetReturnByte();
		virtual WORD	GetReturnWord();
		virtual DWORD	GetReturnDWord();
		virtual float	GetReturnFloat();
		virtual double	GetReturnDouble();
		virtual void *	GetReturnAddress();
		virtual void *	GetReturnObject();


		emUIArgTypes m_emReturnArgTypes;
		CDuiString m_sFunName;
		CStdPtrArray m_arrArgs;
		TArgItem m_result;
	};
	

} // namespace DuiLib

#endif // __UISCRIPT_H__
