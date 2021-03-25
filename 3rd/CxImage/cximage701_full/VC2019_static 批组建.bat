@echo off
setlocal EnableDelayedExpansion

REM 编译命令
set devenvcmd=C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\devenv.exe


REM 编译的解决方案
set slnFile1=CxImgLibVC2019_static.sln


"%devenvcmd%" "%slnFile1%" /Clean "Debug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "Debug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "Release|Win32"
"%devenvcmd%" "%slnFile1%" /Build "Release|Win32"

"%devenvcmd%" "%slnFile1%" /Clean "Unicode Debug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "Unicode Debug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "Unicode Release|Win32"
"%devenvcmd%" "%slnFile1%" /Build "Unicode Release|Win32"

pause