# Microsoft Developer Studio Project File - Name="xmlwrapp_xsltwrapp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xsltwrapp - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xmlwrapp_xsltwrapp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xmlwrapp_xsltwrapp.mak" CFG="xsltwrapp - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xsltwrapp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "xsltwrapp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xsltwrapp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug\xsltwrapp"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug\xsltwrapp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Gm /Zi /FdDebug\xsltwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Gm /Zi /FdDebug\xsltwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\xsltwrapp.lib"
# ADD LIB32 /nologo /out:"Debug\xsltwrapp.lib"

!ELSEIF  "$(CFG)" == "xsltwrapp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release\xsltwrapp"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\xsltwrapp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /FdRelease\xsltwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /FdRelease\xsltwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "NDEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\xsltwrapp.lib"
# ADD LIB32 /nologo /out:"Release\xsltwrapp.lib"

!ENDIF

# Begin Target

# Name "xsltwrapp - Win32 Debug"
# Name "xsltwrapp - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\libxslt\init.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxslt\stylesheet.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\xsltwrapp\init.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxslt\result.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xsltwrapp\stylesheet.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xsltwrapp\xsltwrapp.h
# End Source File
# End Group
# End Target
# End Project

