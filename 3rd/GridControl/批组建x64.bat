@echo off
setlocal EnableDelayedExpansion

REM 编译命令
set devenvcmd=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com


REM 编译的解决方案
set slnFile1=GridControl.sln

"%devenvcmd%" "%slnFile1%" /Clean "Debug|x64"  
"%devenvcmd%" "%slnFile1%" /Build "Debug|x64"  

"%devenvcmd%" "%slnFile1%" /Clean "Release|x64"
"%devenvcmd%" "%slnFile1%" /Build "Release|x64"

"%devenvcmd%" "%slnFile1%" /Clean "UnicodeDebug|x64"  
"%devenvcmd%" "%slnFile1%" /Build "UnicodeDebug|x64"  

"%devenvcmd%" "%slnFile1%" /Clean "UnicodeRelease|x64"
"%devenvcmd%" "%slnFile1%" /Build "UnicodeRelease|x64"

pause