# Microsoft Developer Studio Project File - Name="Console" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Console - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Console.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Console.mak" CFG="Console - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Console - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Console - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Console - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Console - Win32 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Console - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /I "..\j2000" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_CRT_SECURE_NO_DEPRECATE" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../png/Debug/png.lib ../jpeg/Debug/jpeg.lib ../zlib/Debug/zlib.lib ../tiff/Debug/tiff.lib ../jbig/Debug/jbig.lib ../raw/Debug/libdcr.lib ../libpsd/Debug/libpsd.lib ../cximage/Debug/cximage.lib /nologo /subsystem:console /debug /machine:I386 /out:"../bin/consoled.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Console - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /I "..\j2000" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_CRT_SECURE_NO_DEPRECATE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ../png/Release/png.lib ../jpeg/Release/jpeg.lib ../zlib/Release/zlib.lib ../tiff/Release/tiff.lib ../jbig/Release/jbig.lib ../raw/Release/libdcr.lib ../libpsd/Release/libpsd.lib ../cximage/Release/cximage.lib /nologo /subsystem:console /machine:I386 /out:"../bin/console.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Console - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Console___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Console___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /I "..\j2000" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /I "..\j2000" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../png/Debug/png.lib ../jpeg/Debug/jpeg.lib ../zlib/Debug/zlib.lib ../tiff/Debug/tiff.lib ../jbig/Debug/jbig.lib ../cximage/Debug/cximage.lib /nologo /subsystem:console /debug /machine:I386 /out:"../bin/consoled.exe" /pdbtype:sept
# ADD LINK32 ../png/Unicode_Debug/png.lib ../jpeg/Unicode_Debug/jpeg.lib ../zlib/Unicode_Debug/zlib.lib ../tiff/Unicode_Debug/tiff.lib ../jbig/Unicode_Debug/jbig.lib ../raw/Unicode_Debug/libdcr.lib ../libpsd/Unicode_Debug/libpsd.lib ../cximage/Unicode_Debug/cximage.lib /nologo /subsystem:console /debug /machine:I386 /out:"../bin/consoledu.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Console - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Console___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "Console___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /I "..\j2000" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\cximage" /I "..\jpeg" /I "..\zlib" /I "..\png" /I "..\mng" /I "..\tiff" /I "..\j2000" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../png/Release/png.lib ../jpeg/Release/jpeg.lib ../zlib/Release/zlib.lib ../tiff/Release/tiff.lib ../jbig/Release/jbig.lib ../cximage/Release/cximage.lib /nologo /subsystem:console /machine:I386 /out:"../bin/console.exe"
# ADD LINK32 ../png/Unicode_Release/png.lib ../jpeg/Unicode_Release/jpeg.lib ../zlib/Unicode_Release/zlib.lib ../tiff/Unicode_Release/tiff.lib ../jbig/Unicode_Release/jbig.lib ../raw/Unicode_Release/libdcr.lib ../libpsd/Unicode_Release/libpsd.lib ../cximage/Unicode_Release/cximage.lib /nologo /subsystem:console /machine:I386 /out:"../bin/consoleu.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Console - Win32 Debug"
# Name "Console - Win32 Release"
# Name "Console - Win32 Unicode Debug"
# Name "Console - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Console.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
