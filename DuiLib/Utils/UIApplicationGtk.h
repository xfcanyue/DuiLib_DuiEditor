#pragma once

#ifdef DUILIB_GTK
namespace DuiLib {



class UILIB_API CUIApplicationGtk : public CUIApplicationBase
{
public:
	CUIApplicationGtk(void);
	virtual ~CUIApplicationGtk(void);

	virtual bool InitInstance(int argc, char* argv[])  override;
	virtual void Run() override;

protected:
	virtual int ExitInstance() override;
};
}  //namespace DuiLib {
#endif //#ifdef DUILIB_GTK