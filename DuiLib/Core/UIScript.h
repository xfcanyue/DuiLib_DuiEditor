#ifndef __UISCRIPT_H__
#define __UISCRIPT_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//脚本执行上下文，脚本运行实例
	class UILIB_API IScriptContext
	{
	public:
		virtual ~IScriptContext(void) {}

		//设置入口函数, 按函数名。
		virtual int SetFunByName(LPCTSTR lpszFunName) = 0;
		//设置入口函数, 按函数声明。
		virtual int SetFunByDecl(LPCTSTR lpszFunDecl) = 0;

		//传入参数
		virtual int	SetArgByte(UINT arg, BYTE value) = 0;
		virtual int	SetArgWord(UINT arg, WORD value) = 0;
		virtual int	SetArgDWord(UINT arg, DWORD value) = 0;
		virtual int	SetArgFloat(UINT arg, float value) = 0;
		virtual int	SetArgDouble(UINT arg, double value) = 0;
		virtual int	SetArgAddress(UINT arg, void *addr) = 0;
		virtual int	SetArgObject(UINT arg, void *obj) = 0;
		virtual void *GetAddressOfArg(UINT arg) = 0;

		//脚本执行超时时间，毫秒, dwTimeOut < 0，不判断超时。默认值：5秒。
		virtual void SetTimeOut(int dwTimeOut) = 0;

		//执行脚本
		virtual int Execute() = 0;

		//获取返回值
		virtual BYTE	GetReturnByte() = 0;
		virtual WORD	GetReturnWord() = 0;
		virtual DWORD	GetReturnDWord() = 0;
		virtual float	GetReturnFloat() = 0;
		virtual double	GetReturnDouble() = 0;
		virtual void *	GetReturnAddress() = 0;
		virtual void *	GetReturnObject() = 0;
		virtual void *	GetAddressOfReturnValue() = 0;
	};

	class UILIB_API IScriptManager
	{
	public:
		//加入脚本文件
		virtual bool AddScriptFile(LPCTSTR pstrFileName) = 0;

		//加入脚本代码
		virtual bool AddScriptCode(LPCTSTR pstrCode) = 0;

		//编译脚本。 一次性加入脚本文件完毕，然后只能编译一次。
		virtual bool CompileScript() = 0;

		//创建执行上下文
		virtual IScriptContext *CreateContext() = 0;

		//删除执行上下文
		virtual void ReleaseContext(IScriptContext *ctx) = 0;
	};
	typedef IScriptManager* (__stdcall *CREATE_SCRIPT_ENGINE_INSTANCE)();
	typedef void (__stdcall *DELETE_SCRIPT_ENGINE_INSTANCE)(IScriptManager *pEngine);

	class UILIB_API CAutoScriptContext
	{
	public:
		CAutoScriptContext(IScriptManager *mgr);
		~CAutoScriptContext();

		IScriptContext* operator->() const throw();
		bool operator!() const throw();
	protected:
		IScriptManager *mgr;
		IScriptContext *ctx;
	private:
	};
} // namespace DuiLib

#endif // __UISCRIPT_H__
