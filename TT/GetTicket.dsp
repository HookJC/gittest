# Microsoft Developer Studio Project File - Name="GetTicket" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GetTicket - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GetTicket.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GetTicket.mak" CFG="GetTicket - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GetTicket - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GetTicket - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GetTicket - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /out:"./bin/GetTicket.exe"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"./bin/GetTicketd.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GetTicket - Win32 Release"
# Name "GetTicket - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GetTicket.cpp
# End Source File
# Begin Source File

SOURCE=.\GetTicket.rc
# End Source File
# Begin Source File

SOURCE=.\GetTicketDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RCodeBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GetTicket.h
# End Source File
# Begin Source File

SOURCE=.\GetTicketDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\RCodeBtn.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GetTicket.ico
# End Source File
# Begin Source File

SOURCE=.\res\GetTicket.rc2
# End Source File
# End Group
# Begin Group "_12306"

# PROP Default_Filter ""
# Begin Group "api"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_12306\api\encrypt_data.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\api\encrypt_data.h
# End Source File
# Begin Source File

SOURCE=.\_12306\api\HandleCode.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\api\HandleCode.h
# End Source File
# Begin Source File

SOURCE=.\_12306\api\memory_conf.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\api\memory_conf.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\_12306\function.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\function.h
# End Source File
# Begin Source File

SOURCE=.\_12306\HttpSocket.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\HttpSocket.h
# End Source File
# Begin Source File

SOURCE=.\_12306\LoginRequest.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\LoginRequest.h
# End Source File
# Begin Source File

SOURCE=.\_12306\md5.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\md5.h
# End Source File
# Begin Source File

SOURCE=.\_12306\OpensslHttpsClt.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\OpensslHttpsClt.h
# End Source File
# Begin Source File

SOURCE=.\_12306\petcmd.h
# End Source File
# Begin Source File

SOURCE=.\_12306\qmap.h
# End Source File
# Begin Source File

SOURCE=.\_12306\SocketHTTPRequest.cpp

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_12306\SocketHTTPRequest.h
# End Source File
# Begin Source File

SOURCE=.\_12306\typedef.h
# End Source File
# Begin Source File

SOURCE=".\_12306\unicode_utf-8.c"

!IF  "$(CFG)" == "GetTicket - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "GetTicket - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\_12306\unicode_utf-8.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project