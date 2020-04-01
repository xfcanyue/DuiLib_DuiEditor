@echo off
setlocal EnableDelayedExpansion

REM 编译命令
set devenvcmd=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com


REM 编译的解决方案
set slnFile1=GridControl.sln


"%devenvcmd%" "%slnFile1%" /Clean "Debug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "Debug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "Release|Win32"
"%devenvcmd%" "%slnFile1%" /Build "Release|Win32"

"%devenvcmd%" "%slnFile1%" /Clean "UnicodeDebug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "UnicodeDebug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "UnicodeRelease|Win32"
"%devenvcmd%" "%slnFile1%" /Build "UnicodeRelease|Win32"

pause