// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifdef WIN32
#include "targetver.h"
#endif

// TODO: 在此处引用程序需要的其他头文件
#ifdef WIN32
#include "../DuiLib/Duilib.h"
#else
#include "../DuiLib/UIlib.h"
#endif
using namespace DuiLib;

#include "LsStringConverter.h"


#include "angelscript.h"
#include "basedefine.h"
