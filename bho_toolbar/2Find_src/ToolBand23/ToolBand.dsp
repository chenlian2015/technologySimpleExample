# Microsoft Developer Studio Project File - Name="ToolBand" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ToolBand - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ToolBand.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ToolBand.mak" CFG="ToolBand - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ToolBand - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ToolBand - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ToolBand - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\toolband" /D "_ATL_DEBUG_INTERFACES" /D "APOBASE_LIB" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wininet.lib Ws2_32.lib Crypt32.lib shlwapi.lib Urlmon.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/toolbar_sample.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\toolbar_sample.dll
InputPath=.\Debug\toolbar_sample.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ToolBand - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "..\toolband" /D "APOBASE_LIB" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 wininet.lib Ws2_32.lib Crypt32.lib Urlmon.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"ReleaseMinDependency/toolbar_sample.dll"
# SUBTRACT LINK32 /debug
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\toolbar_sample.dll
InputPath=.\ReleaseMinDependency\toolbar_sample.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "ToolBand - Win32 Debug"
# Name "ToolBand - Win32 Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\NamesInc.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ToolBand - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "ToolBand - Win32 Release MinDependency"

# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ToolBand.cpp

!IF  "$(CFG)" == "ToolBand - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ToolBand - Win32 Release MinDependency"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ToolBand.def
# End Source File
# Begin Source File

SOURCE=.\ToolBand.idl
# ADD MTL /tlb "ToolBand.tlb"
# End Source File
# Begin Source File

SOURCE=.\ToolBand.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolBand.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ContextItem.rgs
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\GoRSDN.dll.htm
# End Source File
# Begin Source File

SOURCE=.\web\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\web\logo2.bmp
# End Source File
# Begin Source File

SOURCE=.\web\nav.bmp
# End Source File
# Begin Source File

SOURCE=.\web\nav_hot.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBandObj.rgs
# End Source File
# End Group
# Begin Group "Common Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ApoBaseMembers.h
# End Source File
# Begin Source File

SOURCE=..\BandEditCtrl.inl
# End Source File
# Begin Source File

SOURCE=..\BandToolBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\BandToolBarCtrl.h
# End Source File
# Begin Source File

SOURCE=..\BandToolBarCtrlCRCImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\BandToolBarCtrlFindImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\dhtmlEvent.h
# End Source File
# Begin Source File

SOURCE=..\Exceptions.h
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\ToolBand.h
# End Source File
# Begin Source File

SOURCE=..\ToolBandObj.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBandObj.h
# End Source File
# Begin Source File

SOURCE=..\UrlEncodeImpl.cpp

!IF  "$(CFG)" == "ToolBand - Win32 Debug"

!ELSEIF  "$(CFG)" == "ToolBand - Win32 Release MinDependency"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\utils.cpp
# End Source File
# Begin Source File

SOURCE=..\utils.h
# End Source File
# Begin Source File

SOURCE=..\XmlParser.cpp
# End Source File
# Begin Source File

SOURCE=..\XmlParser.h
# End Source File
# Begin Source File

SOURCE=..\XmlParserImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\XmlParserImpl.h
# End Source File
# End Group
# End Target
# End Project
# Section ToolBand : {006936A4-632E-6C74-0000-000000000000}
# 	1:13:IDD_CONFIGDLG:103
# End Section
