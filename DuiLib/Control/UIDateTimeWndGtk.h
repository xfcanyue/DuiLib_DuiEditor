#ifndef __UIDATETIME_WND_GTK_H__
#define __UIDATETIME_WND_GTK_H__

#pragma once

#ifdef DUILIB_GTK
namespace DuiLib
{
	class CDateTimeWndGtk : public CWindowWnd
	{
	public:
		CDateTimeWndGtk();

		void Init(CDateTimeUI* pOwner);
		RECT CalPos();
	};
}
#endif //#ifdef DUILIB_GTK
#endif // __UIDATETIME_H__