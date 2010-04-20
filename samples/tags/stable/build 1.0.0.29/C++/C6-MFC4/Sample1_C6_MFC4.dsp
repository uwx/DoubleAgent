# Microsoft Developer Studio Project File - Name="Sample1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Sample1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sample1_C6_MFC4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sample1_C6_MFC4.mak" CFG="Sample1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sample1 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Bin"
# PROP BASE Intermediate_Dir ".\Lib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Bin"
# PROP Intermediate_Dir ".\Lib"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /WX /Gm /GR /GX /ZI /Od /Op /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /WX /Gm /GR /GX /ZI /Od /Op /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /i ".\Resources" /d "_DEBUG" /d "UNICODE" /d "_UNICODE"
# ADD RSC /l 0x1009 /i ".\Resources" /d "_DEBUG" /d "UNICODE" /d "_UNICODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Target

# Name "Sample1 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DoubleAgentCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\DoubleAgentSvr.cpp
# End Source File
# Begin Source File

SOURCE=.\MsAgentCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\MsAgentSvr.cpp
# End Source File
# Begin Source File

SOURCE=.\Sample1_C6_MFC4.cpp
# End Source File
# Begin Source File

SOURCE=.\Sample1Dlg_C6_MFC4.cpp
# End Source File
# Begin Source File

SOURCE=StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutBox.h
# End Source File
# Begin Source File

SOURCE=.\DoubleAgentCtl.h
# End Source File
# Begin Source File

SOURCE=.\DoubleAgentSvr.h
# End Source File
# Begin Source File

SOURCE=.\MsAgentCtl.h
# End Source File
# Begin Source File

SOURCE=.\MsAgentSvr.h
# End Source File
# Begin Source File

SOURCE=.\Sample1_C6_MFC4.h
# End Source File
# Begin Source File

SOURCE=.\Sample1Dlg_C6_MFC4.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Sample1.rc
# End Source File
# Begin Source File

SOURCE=.\Sample1.rc2
# End Source File
# Begin Source File

SOURCE=.\Sample1Res.h
# End Source File
# End Group
# End Target
# End Project
