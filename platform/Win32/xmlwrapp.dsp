# Microsoft Developer Studio Project File - Name="xmlwrapp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xmlwrapp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xmlwrapp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xmlwrapp.mak" CFG="xmlwrapp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xmlwrapp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xmlwrapp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xmlwrapp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /I../../include /I..\..\..\libxml2-2.4.30.win32\include /I..\..\..\iconv-1.8.win32\include /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "xmlwrapp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /I../../include /I../../../libxml2-2.4.30.win32\include /I../../../iconv-1.8.win32\include /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "xmlwrapp - Win32 Release"
# Name "xmlwrapp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\libxml\ait_impl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\attributes.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\context.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\document.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\dtd_impl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\event_parser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\expression.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\init.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_iterator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_ref.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_set_iterator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\object.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\tree_parser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\utility.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\xmlwrapp\attributes.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\document.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\event_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\init.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\node.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\tree_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\xmlwrapp.h
# End Source File
# End Group
# End Target
# End Project
