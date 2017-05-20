#------------------------------------------------------------ 
#
#       ATOOL.SMK - ARM make file to build the ATOOL	application library
#
#-------------------------------------------------------------
#*********************************************************************
#   Copyright (C) 2008-2011 by VeriFone Inc. All rights reserved.
#
# No part of this software may be used, stored, compiled, reproduced,
# modified, transcribed, translated, transmitted, or transferred, in
# any form or by any means  whether electronic, mechanical,  magnetic,
# optical, or otherwise, without the express prior written permission
#                           of VeriFone, Inc.
#*********************************************************************
#
# 编译环境(RVDS4.0)
#
# !!! these MACROs needs to be set in project->settings
#
# TRIDENT, DEBUGOPT, SHARELIB, PRODUCT_SIGN, TTF
#
#################################################
# 基本库信息
#################################################
LIBNAME=ATOOL
GID=15
LIBVER=02.04.06

#################
# SDK Path define
#################################################
VRXSDK		= D:\eVoAps\VRXSDK\3.7.2
VACT2000	= D:\eVoAps\ACT2000\1.5.0
DTKTOOLS	= D:\eVoAps\tools\1.0.1.2
AppLog		= D:\eVoAps\Applib\AppLog\01.02.00

VSFSTOOL	= D:\evoaps\FST

#################
# Trident/Vx folder and COption flag
#################################################

!if $(TRIDENT) == 1
!if $(TTF) == 1
!if $(PRODUCT_SIGN) == 1
PLATFORMDIR = Trident\Color\product
!else
PLATFORMDIR = Trident\Color\develop
!endif
!else
!if $(PRODUCT_SIGN) == 1
PLATFORMDIR = Trident\Mono\product
!else
PLATFORMDIR = Trident\Mono\develop
!endif
!endif
!else
PLATFORMDIR = Vx
!endif

#################
# for release/debug folder and COption flag
#################################################
!if $(DEBUGOPT) == 1
DEBUGFLAG = -DLOGSYS_FLAG
!else
DEBUGFLAG = 
!endif

#################
# for COption, shared/Static
#################################################
!if $(SHARELIB) == 1
SHAREDFLAG = -shlpic -D_SHARED_LIB
SHAREDDIR = Shared
!else
SHAREDFLAG = 
SHAREDDIR = Static
!endif
#################################################

COptions = -b $(SHAREDFLAG) $(DEBUGFLAG) -armcc,"--diag_suppress=870" -o $@ 

#################################################
# Project path
#################################################
ROOT=..
SrcDir = $(ROOT)\source
IncDir = $(ROOT)\include
ObjDir = $(ROOT)\obj
OutDir = $(ROOT)\output

TempDir = $(ROOT)\output\$(PLATFORMDIR)
RelDir = $(ROOT)\release
AppLibDir = D:\eVoAps\AppLib\AppTools\$(LIBVER)

SDSIncludes = $(VRXSDK)\include
ACTIncludes = $(VACT2000)\include
AppLogInc	= $(AppLog)\Include

ACTLibraries = $(VACT2000)\Output\RV\Files\Shlib1\Release

Includes = -I$(SDSIncludes) -I$(ACTIncludes) -I$(SrcDir) -I$(IncDir) -I$(AppLogInc)

ACTLibs = \
		$(ACTLibraries)\act.o	\
		$(VRXSDK)\lib\clibPI.a	\
#		$(AppLog)\shared\ALog.o \

AppObjects = \
		$(ObjDir)\FILE.o	\
		$(ObjDir)\MmiUtil.o	\
		$(ObjDir)\Print.o	\
		$(ObjDir)\BankUtil.o \
		$(ObjDir)\TimeUtil.o \
		$(ObjDir)\ComDev.o \
		$(ObjDir)\T9PY.o	\
		$(ObjDir)\barcode.o	\
		$(ObjDir)\bluetooth.o \
		$(ObjDir)\eSignature.o \
		$(ObjDir)\jbig.o \

pseudoOut :$(OutDir)\$(LIBNAME).a
	$(VRXSDK)\bin\vrxlib -b -u $(LIBNAME)
	move $(LIBNAME).o $(OutDir)
	move $(LIBNAME).lib $(OutDir)

#########################################################
#   签名注意事项  
#FST(File Signing Tool)版本不低于3.03.05
#在"setting selections"时选择"used saved setting"
#对Vx平台，签名时选择工程目录下的$(LIBNAME)_vx.fst
#对Trident平台，签名时选择工程目录下的$(LIBNAME)_Trident.fst
#########################################################
!if $(TRIDENT) == 1
!if $(TTF) == 1
	"$(VSFSTOOL)\FileSignature" -C $(TempDir)\F$(GID)\TTF.crt -F $(TempDir)\F$(GID)\TTF.LIB -L
	move $(TempDir)\F$(GID)\TTF.lib.P7S	$(TempDir)\$(GID)\TTF.lib.P7S
	move $(TempDir)\F$(GID)\TTF.crt	$(TempDir)\$(GID)\TTF.crt
!endif

	"$(VSFSTOOL)\FileSignature" -C $(OutDir)\ATOOL.crt -F $(OutDir)\ATOOL.LIB -L
!else
#	"D:\VerixVAps\VeriShield File Signing Tool\FileSignature.exe" $(LIBNAME)_vx.fst -nogui
	"$(VSFSTOOL)\FileSignature" -C $(OutDir)\ATOOL.crt -F $(OutDir)\ATOOL.LIB -L --defcert
!endif

	copy $(IncDir)\*.h					$(ROOT)\release\Include
	move $(OutDir)\$(LIBNAME).lib		$(TempDir)\F$(GID)\$(LIBNAME).lib
	move $(OutDir)\$(LIBNAME).lib.P7S	$(TempDir)\$(GID)\$(LIBNAME).lib.P7S
	move $(OutDir)\$(LIBNAME).o			$(RelDir)\Shared\$(LIBNAME).o
	del  $(LIBNAME).a
!if $(TRIDENT) == 1
#	move $(OutDir)\SponsorCert.crt		$(TempDir)\$(GID)\SponsorCert.crt
	move .\SponsorCert.crt				$(TempDir)\$(GID)\SponsorCert.crt
	move $(OutDir)\$(LIBNAME).crt		$(TempDir)\$(GID)\$(LIBNAME).crt
!endif

#PKZIP
	del $(TempDir)\15\*.ini
	date/t > $(TempDir)\15\D_$(LIBNAME)_$(LIBVER)_$(GID).ini

	del $(RelDir)\download\$(PLATFORMDIR)\d_ATOOL.bat
	echo >>$(RelDir)\download\$(PLATFORMDIR)\d_ATOOL.bat ddl -p9 -iD_$(LIBNAME)_$(LIBVER)_$(GID).ZIP *unzip=D_$(LIBNAME)_$(LIBVER)_$(GID).ZIP 

	del $(RelDir)\download\$(PLATFORMDIR)\D_$(LIBNAME)_$(LIBVER)_$(GID).ZIP

	$(DTKTOOLS)\pkzip -add $(RelDir)\download\$(PLATFORMDIR)\D_$(LIBNAME)_$(LIBVER)_$(GID).ZIP $(TempDir)\*.* -dir=rel

# copy to AppLib\AppTools
	del $(AppLibDir)\download\$(PLATFORMDIR)\*.zip
	copy $(RelDir)\download\$(PLATFORMDIR)\D_$(LIBNAME)_$(LIBVER)_$(GID).ZIP	$(AppLibDir)\download\$(PLATFORMDIR)
	copy $(RelDir)\download\$(PLATFORMDIR)\d_ATOOL.bat							$(AppLibDir)\download\$(PLATFORMDIR)
	copy $(RelDir)\Shared\$(LIBNAME).o											$(AppLibDir)\Shared
	copy $(IncDir)\*.h															$(AppLibDir)\Include

	copy $(ROOT)\revision.txt $(AppLibDir)
	del $(ObjDir)\*.o

$(OutDir)\$(LIBNAME).a : $(AppObjects)
#	$(VRXSDK)\bin\vrxcc -shlpic $(AppObjects) $(ACTLibs) -o $(LIBNAME).a 
	armar --create $(LIBNAME).a $(AppObjects) $(ACTLibs) 

######  Compile #######
{$(SrcDir)}.c{$(ObjDir)}.o:
	$(DTKTOOLS)\astyle --style=ansi $<
	$(VRXSDK)\bin\vrxcc -c $(Includes) $(COptions)  $<
#-e"-" | "$(DTKTOOLS)\fmterrorARM.exe"

