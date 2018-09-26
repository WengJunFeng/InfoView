# Microsoft Developer Studio Project File - Name="InfoView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=InfoView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "InfoView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "InfoView.mak" CFG="InfoView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "InfoView - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "InfoView - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "InfoView - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "InfoView - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "InfoView - Win32 Release"
# Name "InfoView - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\95Processes.cpp
# End Source File
# Begin Source File

SOURCE=.\95ProcessPage.cpp
# End Source File
# Begin Source File

SOURCE=.\AboutInfoViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\ButtonSubClass.cpp
# End Source File
# Begin Source File

SOURCE=.\CFlatBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DLLListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DllListSizer.cpp
# End Source File
# Begin Source File

SOURCE=.\FindTxtDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FindWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoView.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoViewDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MessagesView.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NTProcesses.cpp
# End Source File
# Begin Source File

SOURCE=.\NTProcessPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessesView.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessMemInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessThreadsView.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Sizer.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StylesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\95Processes.h
# End Source File
# Begin Source File

SOURCE=.\95ProcessPage.h
# End Source File
# Begin Source File

SOURCE=.\AboutInfoViewer.h
# End Source File
# Begin Source File

SOURCE=.\ButtonSubClass.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DLLListDlg.h
# End Source File
# Begin Source File

SOURCE=.\DllListSizer.h
# End Source File
# Begin Source File

SOURCE=.\EnumProcess.h
# End Source File
# Begin Source File

SOURCE=.\FindWindow.h
# End Source File
# Begin Source File

SOURCE=..\InfoHook\HHook.h
# End Source File
# Begin Source File

SOURCE=.\InfoUtil.h
# End Source File
# Begin Source File

SOURCE=.\InfoView.h
# End Source File
# Begin Source File

SOURCE=.\InfoViewDoc.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NTProcesses.h
# End Source File
# Begin Source File

SOURCE=.\NTProcessPage.h
# End Source File
# Begin Source File

SOURCE=.\ProcessesView.h
# End Source File
# Begin Source File

SOURCE=.\ProcessMemInfo.h
# End Source File
# Begin Source File

SOURCE=.\ProcessThreadsView.h
# End Source File
# Begin Source File

SOURCE=.\Psapi.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Sizer.h
# End Source File
# Begin Source File

SOURCE=.\SplashDialog.h
# End Source File
# Begin Source File

SOURCE=.\StaticEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Tree.h
# End Source File
# Begin Source File

SOURCE=.\WindowsView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Circle.cur
# End Source File
# Begin Source File

SOURCE=.\Res\classes.ico
# End Source File
# Begin Source File

SOURCE=.\res\Clsdfold.ico
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Res\dllicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\Frame.ico
# End Source File
# Begin Source File

SOURCE=.\res\FrameCircle.ico
# End Source File
# Begin Source File

SOURCE=.\Res\general.ico
# End Source File
# Begin Source File

SOURCE=.\res\general1.ico
# End Source File
# Begin Source File

SOURCE=.\res\general2.ico
# End Source File
# Begin Source File

SOURCE=.\res\general3.ico
# End Source File
# Begin Source File

SOURCE=.\Res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\Res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\Res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\InfoView.ico
# End Source File
# Begin Source File

SOURCE=.\InfoView.rc
# End Source File
# Begin Source File

SOURCE=.\res\InfoView.rc2
# End Source File
# Begin Source File

SOURCE=.\res\InfoViewDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Openfold.ico
# End Source File
# Begin Source File

SOURCE=.\Res\processes.ico
# End Source File
# Begin Source File

SOURCE=.\Res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\styles.ico
# End Source File
# Begin Source File

SOURCE=.\res\Surya256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\windows.ico
# End Source File
# Begin Source File

SOURCE=.\res\windows1.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\InfoHook\Debug\InfoHook.lib
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
