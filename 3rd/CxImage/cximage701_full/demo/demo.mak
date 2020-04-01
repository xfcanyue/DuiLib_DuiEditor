# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=demo - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to demo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "demo - Win32 Release" && "$(CFG)" != "demo - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "demo.mak" CFG="demo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "demo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "demo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "demo - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "demo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\demo.exe"

CLEAN : 
	-@erase ".\Release\demo.exe"
	-@erase ".\Release\ChildFrm.obj"
	-@erase ".\Release\demo.pch"
	-@erase ".\Release\demoView.obj"
	-@erase ".\Release\StdAfx.obj"
	-@erase ".\Release\demoDoc.obj"
	-@erase ".\Release\MainFrm.obj"
	-@erase ".\Release\demo.obj"
	-@erase ".\Release\demo.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\cimage" /I "..\jpeg" /I "..\zlib" /I "..\png" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\cimage" /I "..\jpeg" /I "..\zlib" /I\
 "..\png" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS"\
 /Fp"$(INTDIR)/demo.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/demo.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/demo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib ../png/Release/png.lib ../jpeg/Release/jpeg.lib ../zlib/Release/zlib.lib ../cimage/Release/cimage.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib\
 ../png/Release/png.lib ../jpeg/Release/jpeg.lib ../zlib/Release/zlib.lib\
 ../cimage/Release/cimage.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/demo.pdb" /machine:I386 /out:"$(OUTDIR)/demo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/ChildFrm.obj" \
	"$(INTDIR)/demoView.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/demoDoc.obj" \
	"$(INTDIR)/MainFrm.obj" \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/demo.res"

"$(OUTDIR)\demo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\demo.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\demo.pch"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\demo.exe"
	-@erase ".\Debug\ChildFrm.obj"
	-@erase ".\Debug\MainFrm.obj"
	-@erase ".\Debug\demo.obj"
	-@erase ".\Debug\demoDoc.obj"
	-@erase ".\Debug\demoView.obj"
	-@erase ".\Debug\StdAfx.obj"
	-@erase ".\Debug\demo.res"
	-@erase ".\Debug\demo.ilk"
	-@erase ".\Debug\demo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\cimage" /I "..\jpeg" /I "..\zlib" /I "..\png" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\cimage" /I "..\jpeg" /I\
 "..\zlib" /I "..\png" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D\
 "_MBCS" /Fp"$(INTDIR)/demo.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/demo.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/demo.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib ../png/Debug/png.lib ../jpeg/Debug/jpeg.lib ../zlib/Debug/zlib.lib ../cimage/Debug/cimage.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib\
 ../png/Debug/png.lib ../jpeg/Debug/jpeg.lib ../zlib/Debug/zlib.lib\
 ../cimage/Debug/cimage.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/demo.pdb" /debug /machine:I386 /out:"$(OUTDIR)/demo.exe" 
LINK32_OBJS= \
	"$(INTDIR)/ChildFrm.obj" \
	"$(INTDIR)/MainFrm.obj" \
	"$(INTDIR)/demo.obj" \
	"$(INTDIR)/demoDoc.obj" \
	"$(INTDIR)/demoView.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/demo.res"

"$(OUTDIR)\demo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "demo - Win32 Release"
# Name "demo - Win32 Debug"

!IF  "$(CFG)" == "demo - Win32 Release"

!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "demo - Win32 Release"

!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\demo.cpp

!IF  "$(CFG)" == "demo - Win32 Release"

DEP_CPP_DEMO_=\
	".\StdAfx.h"\
	".\demo.h"\
	".\MainFrm.h"\
	".\ChildFrm.h"\
	".\demoDoc.h"\
	".\demoView.h"\
	".\..\cimage\cimage.h"\
	".\..\cimage\cimageb.h"\
	".\..\cimage\stdafx.h"\
	".\..\cimage\cmap.h"\
	
NODEP_CPP_DEMO_=\
	".\nIndex"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"\
 "$(INTDIR)\demo.pch"


!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

DEP_CPP_DEMO_=\
	".\StdAfx.h"\
	".\demo.h"\
	".\MainFrm.h"\
	".\ChildFrm.h"\
	".\demoDoc.h"\
	".\demoView.h"\
	".\..\cimage\cimage.h"\
	".\..\cimage\cimageb.h"\
	".\..\cimage\stdafx.h"\
	".\..\cimage\cmap.h"\
	

"$(INTDIR)\demo.obj" : $(SOURCE) $(DEP_CPP_DEMO_) "$(INTDIR)"\
 "$(INTDIR)\demo.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "demo - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /I "..\cimage" /I "..\jpeg" /I "..\zlib" /I\
 "..\png" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS"\
 /Fp"$(INTDIR)/demo.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\demo.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\cimage" /I "..\jpeg" /I\
 "..\zlib" /I "..\png" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D\
 "_MBCS" /Fp"$(INTDIR)/demo.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\demo.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\StdAfx.h"\
	".\demo.h"\
	".\MainFrm.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\demo.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\StdAfx.h"\
	".\demo.h"\
	".\ChildFrm.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\demo.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\demoDoc.cpp

!IF  "$(CFG)" == "demo - Win32 Release"

DEP_CPP_DEMOD=\
	".\StdAfx.h"\
	".\demo.h"\
	".\demoDoc.h"\
	".\..\cimage\cimage.h"\
	".\..\cimage\cimageb.h"\
	".\..\cimage\stdafx.h"\
	".\..\cimage\cmap.h"\
	
NODEP_CPP_DEMOD=\
	".\return"\
	

"$(INTDIR)\demoDoc.obj" : $(SOURCE) $(DEP_CPP_DEMOD) "$(INTDIR)"\
 "$(INTDIR)\demo.pch"


!ELSEIF  "$(CFG)" == "demo - Win32 Debug"

DEP_CPP_DEMOD=\
	".\StdAfx.h"\
	".\demo.h"\
	".\demoDoc.h"\
	".\..\cimage\cimage.h"\
	".\..\cimage\cimageb.h"\
	".\..\cimage\stdafx.h"\
	".\..\cimage\cmap.h"\
	

"$(INTDIR)\demoDoc.obj" : $(SOURCE) $(DEP_CPP_DEMOD) "$(INTDIR)"\
 "$(INTDIR)\demo.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\demoView.cpp
DEP_CPP_DEMOV=\
	".\StdAfx.h"\
	".\..\cimage\cimage.h"\
	".\MainFrm.h"\
	".\demo.h"\
	".\demoDoc.h"\
	".\demoView.h"\
	".\..\cimage\cimageb.h"\
	".\..\cimage\stdafx.h"\
	".\..\cimage\cmap.h"\
	

"$(INTDIR)\demoView.obj" : $(SOURCE) $(DEP_CPP_DEMOV) "$(INTDIR)"\
 "$(INTDIR)\demo.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\demo.rc
DEP_RSC_DEMO_R=\
	".\res\demo.ico"\
	".\res\demoDoc.ico"\
	".\res\Toolbar.bmp"\
	".\res\demo.rc2"\
	

"$(INTDIR)\demo.res" : $(SOURCE) $(DEP_RSC_DEMO_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
