# Microsoft Developer Studio Project File - Name="ATOOL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=ATOOL - Win32 Vx_Sh_R
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ATOOL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ATOOL.mak" CFG="ATOOL - Win32 Vx_Sh_R"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ATOOL - Win32 Tri_Sh_R" (based on "Win32 (x86) External Target")
!MESSAGE "ATOOL - Win32 Vx_Sh_R" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "ATOOL - Win32 Tri_Sh_R"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /i /f ATOOL.smk TRIDENT=1 SHARELIB=1 DEBUG=1 TTF=0"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "ATOOL.lib"
# PROP BASE Bsc_Name "ATOOL.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "NMAKE /i /f ATOOL.smk TRIDENT=1 SHARELIB=1 DEBUGOPT=0 PRODUCT_SIGN=1 TTF=0"
# PROP Rebuild_Opt "/a"
# PROP Target_File "ATOOL.lib"
# PROP Bsc_Name "ATOOL.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "ATOOL - Win32 Vx_Sh_R"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /i /f ATOOL.smk TRIDENT=1 SHARELIB=1 DEBUG=1 TTF=0"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "ATOOL.lib"
# PROP BASE Bsc_Name "ATOOL.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "NMAKE /i /f ATOOL.smk TRIDENT=0 SHARELIB=1 DEBUGOPT=1 "
# PROP Rebuild_Opt "/a"
# PROP Target_File "ATOOL.lib"
# PROP Bsc_Name "ATOOL.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "ATOOL - Win32 Tri_Sh_R"
# Name "ATOOL - Win32 Vx_Sh_R"

!IF  "$(CFG)" == "ATOOL - Win32 Tri_Sh_R"

!ELSEIF  "$(CFG)" == "ATOOL - Win32 Vx_Sh_R"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Source\BankUtil.c
# End Source File
# Begin Source File

SOURCE=..\Source\barcode.c
# End Source File
# Begin Source File

SOURCE=..\Source\Bluetooth.c
# End Source File
# Begin Source File

SOURCE=..\Source\ComDev.c
# End Source File
# Begin Source File

SOURCE=..\Source\FILE.c
# End Source File
# Begin Source File

SOURCE=..\Source\MmiUtil.c
# End Source File
# Begin Source File

SOURCE=..\Source\Print.c
# End Source File
# Begin Source File

SOURCE=..\Source\T9PY.c
# End Source File
# Begin Source File

SOURCE=..\Source\TimeUtil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\Include\AppTools.h
# End Source File
# Begin Source File

SOURCE=..\Include\BankUtil.h
# End Source File
# Begin Source File

SOURCE=..\Include\barcode.h
# End Source File
# Begin Source File

SOURCE=..\Include\bluetooth.h
# End Source File
# Begin Source File

SOURCE=..\Include\ComDev.h
# End Source File
# Begin Source File

SOURCE=..\Include\File.h
# End Source File
# Begin Source File

SOURCE=..\Include\FreeType.h
# End Source File
# Begin Source File

SOURCE=..\Include\ft2build.h
# End Source File
# Begin Source File

SOURCE=..\Include\MmiUtil.h
# End Source File
# Begin Source File

SOURCE=..\Include\Print.h
# End Source File
# Begin Source File

SOURCE=..\Include\T9PY.h
# End Source File
# Begin Source File

SOURCE=..\Include\TimeUtil.h
# End Source File
# Begin Source File

SOURCE=..\Include\UserType.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ATOOL.lid
# End Source File
# Begin Source File

SOURCE=.\ATOOL.smk
# End Source File
# Begin Source File

SOURCE=.\atool_Trident.fst
# End Source File
# Begin Source File

SOURCE=.\atool_vx.fst
# End Source File
# Begin Source File

SOURCE=..\revision.txt
# End Source File
# End Target
# End Project
