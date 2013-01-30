# Microsoft Developer Studio Project File - Name="xmlwrapp_xmlwrapp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=xmlwrapp - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xmlwrapp_xmlwrapp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xmlwrapp_xmlwrapp.mak" CFG="xmlwrapp - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xmlwrapp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "xmlwrapp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xmlwrapp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug\xmlwrapp"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug\xmlwrapp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Gm /Zi /FdDebug\xmlwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /W1 /GR /EHsc /Od /Gm /Zi /FdDebug\xmlwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\xmlwrapp.lib"
# ADD LIB32 /nologo /out:"Debug\xmlwrapp.lib"

!ELSEIF  "$(CFG)" == "xmlwrapp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release\xmlwrapp"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release\xmlwrapp"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /FdRelease\xmlwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "NDEBUG" /c
# ADD CPP /nologo /FD /MD /W1 /GR /EHsc /O2 /FdRelease\xmlwrapp.pdb /I "..\..\include" /I "include" /D "WIN32" /D "_LIB" /D "NDEBUG" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\xmlwrapp.lib"
# ADD LIB32 /nologo /out:"Release\xmlwrapp.lib"

!ENDIF

# Begin Target

# Name "xmlwrapp - Win32 Debug"
# Name "xmlwrapp - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\libxml\ait_impl.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\attributes.cxx
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

SOURCE=..\..\src\libxml\init.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_iterator.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_manip.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\nodes_view.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\tree_parser.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\utility.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\xmlwrapp\_cbfo.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\ait_impl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\attributes.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\document.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\dtd_impl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\event_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\errors.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\init.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\node.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\node_manip.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\nodes_view.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\pimpl_base.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\tree_parser.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxml\utility.h
# End Source File
# Begin Source File

SOURCE=..\..\include\xmlwrapp\xmlwrapp.h
# End Source File
# End Group
# End Target
# End Project

