@echo off
setlocal EnableDelayedExpansion

REM 编译命令
set devenvcmd=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\devenv.exe


REM 编译的解决方案
set slnFile1=DuiEditor2019.sln


"%devenvcmd%" "%slnFile1%" /Clean "Debug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "Debug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "Release|Win32"
"%devenvcmd%" "%slnFile1%" /Build "Release|Win32"

"%devenvcmd%" "%slnFile1%" /Clean "UnicodeDebug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "UnicodeDebug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "UnicodeRelease|Win32"
"%devenvcmd%" "%slnFile1%" /Build "UnicodeRelease|Win32"

"%devenvcmd%" "%slnFile1%" /Clean "xsDebug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "xsDebug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "xsRelease|Win32"
"%devenvcmd%" "%slnFile1%" /Build "xsRelease|Win32"

"%devenvcmd%" "%slnFile1%" /Clean "xsUnicodeDebug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "xsUnicodeDebug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "xsUnicodeRelease|Win32"
"%devenvcmd%" "%slnFile1%" /Build "xsUnicodeRelease|Win32"

pause