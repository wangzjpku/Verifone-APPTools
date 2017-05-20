#---------------------------------------------------------------------------
#
#	Filename:	ATOOL.lid
#
#	Product:	ATOOL
#	Version:	1.3.1
#
#	Author:		Administrator
#	Created:	2010-8-18 10:36:47
#
#	Description:  Shared library description file for ATOOL
#				  shared library.
#
#	Notes:
#	CONSECUTIVE NUMBERING (MORE OR LESS) IS USED FOR ALL FUNCTIONS.
#   	NUMBERS CANNOT BE REASSIGNED WITHOUT REQUIRING APPS TO RE-LINK,
#	BUT NUMBERS CAN BE ADDED FOR NEW FUNCTIONS.
#   	WHEREVER POSSIBLE, ATTEMPTS ARE MADE TO GROUP FUNCTIONS BASED
#   	ON SOURCE FILE AND FUNCTIONAL AREA.  BUT IN THE LONG TERM, NO
#   	GUARANTEES.  SO MODIFY CAREFULLY!!!
#
#	Modification history:
#		#		Date		Who			Comments
#		1.0             2010-8-18 10:36:47	Administrator		Initial Draft
#---------------------------------------------------------------------------
#/*****************************************************************************************
#   Copyright (C) 2008-2010 by VeriFone Inc. All rights reserved.
#
# No part of this software may be used, stored, compiled, reproduced,
# modified, transcribed, translated, transmitted, or transferred, in
# any form or by any means  whether electronic, mechanical,  magnetic,
# optical, or otherwise, without the express prior written permission
#                         of VeriFone, Inc.
#******************************************************************************************/
ATOOL	id=16	addr=0	ver=1.0		thumb

#1		get_ATOOL_version

1	File_DirGetFirst
2	File_DirGetNext
3	File_Open
4	File_Read
5	File_Write
6	File_Lseek
7	File_Delete
8	File_Close
9	File_Remove
10	File_Rename
11	File_Clear
12	File_GetFreeSpace
13	File_GetNumberOfRecords
14	File_AppendRecord
15	File_GetRecordByIndex
16	File_UpdateRecordByIndex
17	File_DeleteRecordByIndex
18	File_GetRecord
19	File_UpdateRecord
20	File_DeleteRecord
21	File_ReadTLV
22	File_WriteTLV
23	File_DeleteTLV
24	File_GetKey
25	File_PutKey
26	File_ReadLine

32	MmiUtil_flushcard
33	MmiUtil_flushkbd
34	MmiUtil_GetGroup
35	MmiUtil_GetTerminalType
36	MmiUtil_GetSerialNO
37	MmiUtil_GetBatteryCharge
38	MmiUtil_GetBatteryStatus
39	MmiUtil_GetDockStatus
40	MmiUtil_SetGroup
41	MmiUtil_Restart
42	MmiUtil_SetFontSize
43	MmiUtil_GetFontSize
44	MmiUtil_SetFont
45	MmiUtil_GetFont
46	MmiUtil_Window
47	MmiUtil_Clr
48	MmiUtil_WriteAt
49	MmiUtil_GetColumms
50	MmiUtil_GetRows
51	MmiUtil_Reset
52	MmiUtil_ClearLine
53	MmiUtil_ClearLines
54	MmiUtil_UserClr
55	MmiUtil_DispChinese
56	MmiUtil_DispChineseInvert
57	MmiUtil_DispEnglish
58	MmiUtil_DisplayLine
59	MmiUtil_DisplayRight
60	MmiUtil_DisplayCenter
61	MmiUtil_SetTimeOut
62	MmiUtil_GetTimeOut
63	MmiUtil_CheckKey
64	MmiUtil_WaitKey
65	MmiUtil_WaitKeyMill
66	MmiUtil_Beep
67	MmiUtil_ErrorSound
68	MmiUtil_Warning
69	MmiUtil_Warning2
70	MmiUtil_InputText
71	MmiUtil_InputNum
72	MmiUtil_InputPWD
73	MmiUtil_InputAmount
74	MmiUtil_InputBcdCode
75	MmiUtil_GetTrack
76	MmiUtil_GetTrack2
77	MmiUtil_ClearEngLines
78	MmiUtil_ClearEngLine
79	MmiUtil_DispBMP
80	MmiUtil_DispStrByColor
81	MmiUtil_SetFontColor
82	MmiUtil_SetTouchButton
83	MmiUtil_ShowTouchButton
84	MmiUtil_CheckTouchButton
85	MmiUtil_DisplayLeftRight
86	MmiUtil_GetPrinterType
87	MmiUtil_Warning3

90	Print_Init
91	Print_Str
92	Print_StrStr
93	Print_English
94	Print_EngSmall
95	Print_Hex
96	Print_logo
97	Print_bitmap
98	Print_FormFeed
99	Print_SetLineHeight
100	Print_SetFormLength
101	Print_Feed
102	Print_DelCache
103	Print_Str_12
104	Print_SetDark

105	BankUtil_InputDate
106	BankUtil_Bcd2Asc
107	BankUtil_Asc2Bcd
108	BankUtil_ZipNumber
109	BankUtil_UnzipNumber
110	BankUtil_Amt2Asc
111	BankUtil_Asc2Amt
112	BankUtil_Len2Bcd
113	BankUtil_Bcd2Len
114	BankUtil_GenLrc
115	BankUtil_DES
116	BankUtil_CStrlcpy

120	TimeUtil_CheckDate
121	TimeUtil_GetDate
122	TimeUtil_GetTime
123	TimeUtil_SaveToNos
124	TimeUtil_DateAndTimeAsStr
125	TimeUtil_SetYear
126	TimeUtil_MillSec

130	Device_GetHandle
131	ComDev_Init
132	ComDev_Read
133	ComDev_Write
134 BlueTooth_Open                          #for VX600 BT

138	T9PY_Init
139	T9PY_Input

140	MmiUtil_DispStr                         # ICBC
141	MmiUtil_WaitKeyMenu                     # ICBC
142	MmiUtil_WaitKeyButton                   # ICBC
143	MmiUtil_DisplayCenter_ICBC              # MmiUtil_DisplayCenter(ICBC)
144	MmiUtil_DisplayRightNoClear             # MmiUtil_DisplayRight (CCB)
145	MmiUtil_InputLongText
146	MmiUtil_InputLongNum
147 MmiUtil_SetKeypadState					# for Vx600

150 Barcode_Activate
151 Barcode_Deactivate
152 Barcode_Scan

153 Print_Open
154 Print_Close
155 Print_PreLoadFont

156 eSign_Signature
157 eSign_PrintPBMImage
158 eSign_CompressImage
159 eSign_UnCompressImage
160 eSign_Check

161 Print_SetSprocketDotMode
162 eSign_SetComPort
