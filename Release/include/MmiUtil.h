/***************************
 * MMIUTIL.C使用了至少3K的全局变量空间
 ***************************************/
#ifndef __MMIUTIL_H__
#define __MMIUTIL_H__

//横向粉红色功能键
#define bKEYA			KEY_a
#define bKEYB			KEY_b
#define bKEYC			KEY_c
#define bKEYD			KEY_d

//纵向ATM键
#define bF1				KEY_e
#define bF2				KEY_f
#define bF3				KEY_g
#define bF4				KEY_h
//字母键
#define bALPHAKEY		ALPHA_KEY
//功能键
#define bESC			KEY_CANCEL
#define bENTER			KEY_CR
#define bBKSP			KEY_BS
#define bCLR			KEY_CLR

#define bSTR			KEY_STR
#define bPND			KEY_PND
//其他键
#define bMENU			ALPHA_KEY
#define bUP				KEY_UP
#define bDOWN			KEY_DOWN
//数字键
#define bKEY0			KEY0
#define bKEY1			KEY1
#define bKEY2			KEY2
#define bKEY3			KEY3
#define bKEY4			KEY4
#define bKEY5			KEY5
#define bKEY6			KEY6
#define bKEY7			KEY7
#define bKEY8			KEY8
#define bKEY9			KEY9

#define DEFAULT_CHAR_WIDTH	16	//默认屏幕宽度  8*16 = 128
#define DEFAULT_CHAR_HEIGHT 4   //默认屏幕宽度  16*4 = 64
#define HEADER_HEIGHT 32      

#define MAX_KEY_WAIT	30	//秒

#define CARD_LEN_MAX	 	300
#define CARD_LEN_MIN	 	20

typedef enum _DISP_LINE{
	LINE1 = 1,
	LINE2,
	LINE3,
	LINE4,
	LINE5,
	LINE6,
	LINE7,
	LINE8,
	LINE9,
	LINE10,
	LINE11,
	LINE12
}DISP_LINE;

typedef enum _DISP_FORMAT
{
    LEFT_JST=0,   
	RIGHT_JST,
	CENTER_JST
}DISP_FORMAT;

typedef enum _FONT_SIZE{
	FONT_16X16 = 16,
	FONT_24X24 = 24,
	FONT_28X28 = 28
}FONT_SIZE;

//终端型号
typedef enum
{
	_VX510 = 0,
	_VX510G ,
	_VX610 ,
	_VX670 ,
	_VX810 ,
	_VX700 ,
	_VX520 ,
	_VX520S ,
	_VX680 ,
	_VX820 ,
	_VX680C ,
	_VX805 ,
	_VX825 ,
	_VX675 ,
	_VX600
}enumTerminalType;

//打印机类型
typedef enum
{
	THERMAL_PRINTER  = 0,
	SPROCKET_PRINTER
}enumPrinterType;

/*刷卡返回*/
typedef enum
{
	TRACK_NOEVENT   = -5,	   /* 无事件 */
	TRACK_ESC       = -3,      /* 用户中断 bESC */
	TRACK_TIMEOUT   = -2,      /* 刷卡超时*/
	TRACK_FAIL      = -1,      /* 刷卡失败*/
	TRACK_OK        = 0,       /* 不循环时,无卡无键发生*/
	TRACK_NUM       = 1,       /* 手输卡号 */
}enumSwipCardRet;


#define DISP_FIRSTZERO	0x40

#define INPUT_OK			0
#define INPUT_USERABORT		-1
#define INPUT_TIMEOUT		-2
#define INPUT_FAILED		-3
#define INPUT_CANCEL		INPUT_USERABORT

#ifndef CHNFONT
#define CHNFONT		"F:/chn.fon"
#endif

#ifndef TTFFONT
#define TTFFONT		"F:/#wqydkzh.ttf"
#endif

#ifndef GBKFONT_16
#define GBKFONT_16		"F:/#gbk16.fon"
#endif

#ifndef GBKFONT_12
#define GBKFONT_12		"F:/#gbk12.fon"
#endif

/*=========================================*
 *   P U B L I C     F U N C T I O N S     *
 *=========================================*/ 
/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetTerminalType.
* DESCRIPTION:   获取终端类型
* PARAMETERS:
* RETURN:
*		TerminalType -- 终端类型（参考enumTerminalType）
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetTerminalType(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetSerialNO.
* DESCRIPTION:   获取终端序列号
* PARAMETERS:
*		seri_no（out） -- 序列号（11 bytes）
* RETURN:
*		0（成功） / -1 失败
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetSerialNO(char *seri_no);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetBatteryCharge.
* DESCRIPTION:   获取电池电量
* PARAMETERS:
* RETURN:
*		battery charge -- 电池电量（百分比 0-100）
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetBatteryCharge(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetBatteryStatus.
* DESCRIPTION:   获取电池状态
* PARAMETERS:
* RETURN:
*		成功 -- NORMAL_LEVEL   (1)
*		失败 -- CRITICAL_LEVEL (0)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetBatteryStatus(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetDockStatus.
* DESCRIPTION:   获取充电状态
* PARAMETERS:
* RETURN:
*		成功 -- HANDHELD_DOCKED   (0)
*		失败 -- HANDHELD_UNDOCKED (-1)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetDockStatus(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetGroup.
* DESCRIPTION:   改变Group ID
* PARAMETERS:
*		group_id (in) -- 新Group ID
* RETURN:
*		成功 -- group id   ( >=1 )
*		失败 -- (-1)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_SetGroup(int group_id);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetGroup.
* DESCRIPTION:   获取Group ID
* PARAMETERS:
* RETURN:
*		Group ID
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetGroup(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Restart.
* DESCRIPTION:   终端重启
* PARAMETERS:
*		filename (in) -- 可执行文件名或""
* RETURN:
*		成功 -- 
*		失败 -- (<0)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_Restart(const char *filename);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontSize.
* DESCRIPTION:   设置屏幕显示字体大小
* PARAMETERS:    
*		font_size (in) -- 字体大小（参考FONT_SIZE定义）
* RETURN:        
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_SetFontSize(int font_size);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontSize.
* DESCRIPTION:   获取屏幕显示字体大小
* PARAMETERS:    	
* RETURN:  
*	font_size -- 字体大小（参考FONT_SIZE定义）      
* NOTES:         
* ------------------------------------------------------------------------ */
int MmiUtil_GetFontSize(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFont.
* DESCRIPTION:   设置屏幕显示字体文件
* PARAMETERS:    
*	szFont (in) -- 字体路径，最长不得超过20个字节,为空则设置为默认字体
* RETURN:       
* NOTES:        
* ------------------------------------------------------------------------ */
void MmiUtil_SetFont(const char * szFont);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetFont.
* DESCRIPTION:   获取屏幕显示字体文件
* PARAMETERS:    
*	szFont (out) -- 字体路径，最长不得超过20个字节,为空则设置为默认字体
* RETURN:       
* NOTES:        
* ------------------------------------------------------------------------ */
void MmiUtil_GetFont(char * szFont);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontColor.
* DESCRIPTION:  设置字体颜色，对680/820有效
* PARAMETERS:
*	FontColor (in) -- 字体颜色RGB（后3字节分别表示RED,GREEN,BLUE）
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_SetFontColor(unsigned long RGBFontColor);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Window
* DESCRIPTION:  设置显示屏幕
* PARAMETERS:    
*			startX (in) -- 左上角横坐标
*			startY (in) -- 左上角纵坐标
*			endX   (in) -- 右下角横坐标
*			endY   (in) -- 右下角纵坐标
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_Window(int startX, int startY, int endX, int endY);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Clr
* DESCRIPTION:  清除指定屏幕
* PARAMETERS:
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_Clr(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WriteAt
* DESCRIPTION:  屏幕显示
* PARAMETERS:
*		buf(in) -- 显示的数据
*		len(in) -- 显示的数据的长度
*		x(in)   -- 显示开始的列(column)
*		y(in)	-- 显示开始的行(row) 
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_WriteAt(char *buf, int len, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetColumms.
* DESCRIPTION:   获取显示屏幕最大列数
* PARAMETERS:    
* RETURN:       
*	Columms - 屏幕的列数		
* NOTES:        
* ------------------------------------------------------------------------ */
usint MmiUtil_GetColumms(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetRows.
* DESCRIPTION:   获取显示屏幕最大行数
* PARAMETERS:    
* RETURN:       
*	Rows - 屏幕的行数		
* NOTES:        
* ------------------------------------------------------------------------ */
usint MmiUtil_GetRows(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Reset
* DESCRIPTION:  初始化显示屏幕,设置屏幕显示字体
* PARAMETERS:    
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_Reset(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearLine
* DESCRIPTION:  清除指定行（1行）
* PARAMETERS: 
*		disp_line (in) -- 要清除的行
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearLine(int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearLines
* DESCRIPTION:  清除指定行（多行）
* PARAMETERS: 
*		start_line (in) -- 开始行
*		end_line   (in) -- 结束行
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearLines(int start_line, int end_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_UserClr
* DESCRIPTION:  清除屏幕下方显示区域（从第几行开始）
* PARAMETERS: 
*		start_line (in) -- 开始行
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_UserClr(int start_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispStr.
* DESCRIPTION:   在坐标起始位置开始显示一行英文. Vx680专用
* PARAMETERS:
*		str (in) -- 要显示的中文区位码
*		startX   (in) -- 起始列号, 从0开始, 最大239
*		startY   (in) -- 起始行号, 从0开始, 最大319
*   fontSize (in) -- 字体大小
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_DispStr(char *str, int startX, int startY, int fontSize);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispStrByColor.
* DESCRIPTION:   在坐标起始位置开始显示一行英文/中文,字体颜色和背景可自定义. Vx680/820专用
* PARAMETERS:
*		str		 (in) -- 要显示的字符
*		startX   (in) -- 起始列号, 从0开始, 最大239
*		startY   (in) -- 起始行号, 从0开始, 最大319
*		fontSize (in) -- 字体大小
*		FontColor(in) -- 字体颜色RGB（后3字节分别表示RED,GREEN,BLUE）
*		BGColor	 (in) -- 背景颜色RGB（后3字节分别表示RED,GREEN,BLUE）
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DispStrByColor(char *str, int startX, int startY, int fontSize,unsigned long  RGBFontColor, unsigned long  RGBBGColor);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispChinese.
* DESCRIPTION:   在坐标起始位置开始显示一行中文.
* PARAMETERS:    
*		str (in) -- 要显示的中文区位码
*		x   (in) -- 列号, 从1开始
*		y   (in) -- 行号, 从1开始
* RETURN:        
* NOTES:         字体文件由程序提前指定
* ------------------------------------------------------------------------ */
void MmiUtil_DispChinese(char *str, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispChineseInvert.
* DESCRIPTION:   在坐标起始位置开始反显一行中文.
* PARAMETERS:    
*		str (in) -- 要显示的中文区位码
*		x   (in) -- 列号, 从1开始
*		y   (in) -- 行号, 从1开始
* RETURN: 
* NOTES:		字体文件由程序提前指定
* ------------------------------------------------------------------------ */
void MmiUtil_DispChineseInvert(char *str, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispEnglish.
* DESCRIPTION:   在坐标起始位置开始显示一行英文.
* PARAMETERS: 
*		str (in) -- 要显示的英文
*		x   (in) -- 列号, 从1开始
*		y   (in) -- 行号, 从1开始
* RETURN:        
* NOTES:         英文字体使用系统默认字体（Vx520：6X8；Vx680：9X12）
* ------------------------------------------------------------------------ */
void MmiUtil_DispEnglish(char *str, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayLine
* DESCRIPTION:   显示内容致至指定行(左对齐)
* PARAMETERS:    
*		msg       (in)	-- 要显示的中文区位码
*		disp_line (in)	-- 显示的行数
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayLine(char *msg, int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayRight
* DESCRIPTION:   显示内容致至指定行(右对齐)，清行
* PARAMETERS:    
*		msg		   (in)	-- 要显示的中文区位码
*		disp_line  (in)	-- 显示的行数
* RETURN: 
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayRight(char *msg, int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayCenter
* DESCRIPTION:   居中显示内容至指定行
* PARAMETERS:    
*		msg       (in) -- 要显示的中文区位码
*		disp_line (in) -- 显示的行数
*		invert    (in) -- 是否反显TRUE/FALSE
* RETURN:
* NOTES: 
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayCenter(char *msg, int disp_line, boolean invert);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetTimeout.
* DESCRIPTION:   设置MmiUtil系列输入函数的超时时间
* PARAMETERS:    
*		usTimeout (in) -- 超时时间,单位秒,为0则阻塞式的等待按键
* RETURN:        
* NOTES:         超时时间默认30秒
* ------------------------------------------------------------------------ */
void MmiUtil_SetTimeOut(usint usTimeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetTimeout.
* DESCRIPTION:   获取MmiUtil系列输入函数的超时时间
* PARAMETERS:    
* RETURN:
*	usTimeout -- 超时时间,单位秒,为0则阻塞式的等待按键
* NOTES:	超时时间默认30秒
* ------------------------------------------------------------------------ */
usint MmiUtil_GetTimeOut(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_CheckKey.
* DESCRIPTION:   检测是否有按键
* PARAMETERS:  
* RETURN:        如果没有按键返回0，否则返回按键的键值
* NOTES:
* ------------------------------------------------------------------------ */
byte MmiUtil_CheckKey(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyMenu
* DESCRIPTION:   等待超时（秒）或按键，680触摸屏菜单
* PARAMETERS:
*		Timeout (in)     - 超时（秒），为0时无超时
*		Count   (in)     - 总菜单项数
*		Head    (in)     - TRUE：有菜单头；FALSE：无菜单头
*		OneMenu (in)     - TRUE：一行一个菜单；FALSE：一行二个菜单
*		Base    (in)     - 菜单从'0'或'1'开始？
* RETURN:        超时返回0，否则返回按键值
* NOTES:         以秒为单位.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyMenu(usint Timeout, usint Count, usint Head, usint OneMenu, byte Base);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyButton
* DESCRIPTION:   等待超时（秒）或按键，增加680触摸屏按纽
* PARAMETERS:
*		Timeout (in)     - 超时（秒），为0时无超时
* RETURN:        超时返回0，否则返回按键值
* NOTES:         以秒为单位.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyButton(usint Timeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKey
* DESCRIPTION:   等待超时（秒）或按键
* PARAMETERS: 
*		Timeout (in) - 超时（秒），为0时无超时
* RETURN:        超时返回0，否则返回按键值
* NOTES:         以秒为单位.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKey(usint Timeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyMill
* DESCRIPTION:   等待超时（毫秒）或按键
* PARAMETERS: 
*		Timeout (in) - 超时（毫秒），为0时无超时
* RETURN:        超时返回0，否则返回按键值
* NOTES:         以毫秒为单位.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyMill(ulint Timeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Beep
* DESCRIPTION:   鸣叫300ms等待200ms
* PARAMETERS: 
* RETURN: 
* ------------------------------------------------------------------------ */
void MmiUtil_Beep(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ErrorSound
* DESCRIPTION:   鸣叫300ms等待200ms再鸣叫300ms
* PARAMETERS: 
* RETURN:        none
* ------------------------------------------------------------------------ */
void MmiUtil_ErrorSound(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning.
* DESCRIPTION:   显示警告信息，等待按键（超时退出）
* PARAMETERS:
*		text (in) -- 警告信息	
* RETURN:         
*		0		  -- 超时
*		key_value -- 按键值
* NOTES:  
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning(char *text);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning2.
* DESCRIPTION:   显示警告信息，等待按键（无超时）
* PARAMETERS:
*		text (in) -- 警告信息	
* RETURN:         
*		key_value -- 按键值
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning2(char *text);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning3.
* DESCRIPTION:   显示警告信息，等待按键（超时退出）
* PARAMETERS:
*   text (in) -- 警告信息
*	iTimeout(in) --超时时间
* RETURN:
*   0     -- 超时
*   key_value -- 按键值
* NOTES:
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning3(char *text, usint iTiemout );


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputText
* DESCRIPTION:   输入字符串(字母和数字)
* PARAMETERS:   
*		disp_line (in)  -- 提示信息显示行数
*		msgPrompt (in)  -- 提示信息
*		str       (out) -- 输出字符串
*		min       (in)  -- 输入数据最小长度
*		max       (in)  -- 输入数据最大长度
*		disp_mode (in)  -- 输入数据显示模式(参考DISP_FORMAT定义)
* RETURN:
*			返回值大于0:输入数据长度
*			返回值小于0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputLongText
* DESCRIPTION:   输入字符串(字母和数字)
* PARAMETERS:   
*		disp_line (in)  -- 提示信息显示行数
*		msgPrompt (in)  -- 提示信息
*		str       (out) -- 输出字符串
*		min       (in)  -- 输入数据最小长度
*		max       (in)  -- 输入数据最大长度
*		disp_mode (in)  -- 输入数据显示模式(参考DISP_FORMAT定义)
* RETURN:
*			返回值大于0:输入数据长度
*			返回值小于0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputLongText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputNum
* DESCRIPTION:   输入数字串
* PARAMETERS:   
*		disp_line (in)  -- 提示信息显示行数
*		msgPrompt (in)  -- 提示信息
*		str       (out) -- 输出数字串
*		min       (in)  -- 输入数据最小长度
*		max       (in)  -- 输入数据最大长度
*		disp_mode (in)  -- 输入数据显示模式(参考DISP_FORMAT定义)
* RETURN:
*			返回值大于0:输入数据长度
*			返回值小于0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputNum(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputLongNum
* DESCRIPTION:   输入数字串
* PARAMETERS:   
*		disp_line (in)  -- 提示信息显示行数
*		msgPrompt (in)  -- 提示信息
*		str       (out) -- 输出数字串
*		min       (in)  -- 输入数据最小长度
*		max       (in)  -- 输入数据最大长度
*		disp_mode (in)  -- 输入数据显示模式(参考DISP_FORMAT定义)
* RETURN:
*			返回值大于0:输入数据长度
*			返回值小于0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputLongNum(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputPWD
* DESCRIPTION:   输入密码
* PARAMETERS:   
*		disp_line (in)  -- 提示信息显示行数
*		msgPrompt (in)  -- 提示信息
*		pwd       (out) -- 输出密码
*		min       (in)  -- 输入数据最小长度
*		max       (in)  -- 输入数据最大长度
*		disp_mode (in)  -- 输入数据显示模式(参考DISP_FORMAT定义)
* RETURN: 
*			返回值大于0:输入数据长度
*			返回值小于0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputPWD(int disp_line, char * msgPrompt, char *pwd, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputAmount
* DESCRIPTION:   输入金额
* PARAMETERS:   
*		disp_line (in)  -- 提示信息显示行数
*		msgPrompt (in)  -- 提示信息
*		amount    (out) -- 输出金额
*		min       (in)  -- 输入数据最小长度
*		max       (in)  -- 输入数据最大长度
*		disp_mode (in)  -- 输入数据显示模式(参考DISP_FORMAT定义)
* RETURN: 
*			返回值大于0:输入数据长度
*			返回值小于0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputAmount(int disp_line, char * msgPrompt, char *amount, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputBcdCode
* DESCRIPTION:   显示提示并输入BCD的字符串.
* PARAMETERS:     
*		DispMsg   (in)   -- 提示,可以为NULL.
*       BcdBuf    (out)  -- 输出缓冲in BCD
*       BcdBytes  (in)   -- 要输入的字节数(in BCD)最大100
*       IsDispOrg (in)   --是否显示原有信息
* RETURN:         >=0 输入的字节个数
*				INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTES:
* ------------------------------------------------------------------------ */
sint MmiUtil_InputBcdCode(int disp_line, char *DispMsg, byte *BcdBuf, 
						   usint BcdBytes, boolean IsDispOrg);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MMiUtil_GetTrack
* DESCRIPTION:		读磁条卡或手输卡号（循环等待，超时退出）
* PARAMETERS:
*		CardBuf1 (out) -- 1磁道数据
*		CardBuf2 (out) -- 2磁道数据
*		CardBuf3 (out) -- 3磁道数据
*		PANnum   (out) -- 手工输入的卡号
*		manflag  (in)  -- 是否支持手输卡号
* RETURN:
*		TRACK_OK/TRACK_NUM/TRACK_ESC/TRACK_TIMEOUT
* NOTES:
* --------------------------------------------------------------------------*/
int MmiUtil_GetTrack(byte *CardBuf1, byte *CardBuf2, byte *CardBuf3, byte *PANnum, int manflag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MMiUtil_GetTrack2
* DESCRIPTION:		读磁条卡或手输卡号（只读一次，没有循环）
* PARAMETERS:
*		CardBuf1 (out) -- 1磁道数据
*		CardBuf2 (out) -- 2磁道数据
*		CardBuf3 (out) -- 3磁道数据
*		PANnum   (out) -- 手工输入的卡号
*		manflag  (in)  -- 是否支持手输卡号
* RETURN:
*		TRACK_OK/TRACK_NUM/TRACK_ESC/TRACK_NOEVENT
* NOTES:
* --------------------------------------------------------------------------*/
int MmiUtil_GetTrack2(byte *CardBuf1, byte *CardBuf2, byte *CardBuf3, byte *PANnum, int manflag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearEngLines
* DESCRIPTION:  清除指定行（多行，英文小字体）
* PARAMETERS:
*		start_line (in) -- 开始行
*		end_line   (in) -- 结束行
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearEngLines(int start_line, int end_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearEngLines
* DESCRIPTION:  清除指定行（1行，英文小字体）
* PARAMETERS:
*		disp_line (in) -- 要清除的行
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearEngLine(int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispBMP
* DESCRIPTION:   显示bitmap文件
* PARAMETERS:
*		BmpFile  (in) -- 图片文件名称（后缀.bmp）
*		x        (in) -- 图片显示左上角横坐标（像素）
*		y        (in) -- 图片显示左上角纵坐标（像素）
*		width    (in) -- 图片宽度（像素）
*		height   (in) -- 图片高度（像素）
* RETURN:
*		0(success)/ -1(fail)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_DispBMP(char *BmpFile, int x, int y, int width, int height);

/* ------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_flushkbd
* DESCRIPTION:   清空按键buffer
* PARAMETERS:
* RETURN:
* NOTES:
* -----------------------------------------------------------------------*/
void MmiUtil_flushkbd(void);

/* ------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_flushcard
* DESCRIPTION:  清空磁条卡读卡器buffer
* PARAMETERS:
* RETURN:
* NOTES:
* -----------------------------------------------------------------------*/
void MmiUtil_flushcard(void);

#ifdef _SHARED_LIB
void MmiUtil_InitPrm(void);
#endif

/*=====================================================================================*
	 								BUTTON处理子模块说明								   

  1.BUTTON只在VX680/820的320*240触摸屏上使用。

  2.BUTTON长度跟随显示文本的长度自动调整。
  
  3.BUTTON选中后的返回值自行设置。
	
  4.使用G15 FLASH中的BTN1_L.BMP,BTN1_R.BMP,BTN1_C.BMP作为显示BUTTON组件，
		BTN2_L.BMP,BTN2_R.BMP,BTN2_C.BMP作为选中时显示的BUTTON组件。

  5.可以通过更换上述文件来达到自己期望的效果，但要遵循如下要求：
	a.BUTTON通过左、中、右3个图片文件组合而成
	b.中间部分的长度为24像素，左和右部分的图片宽度为10个像素
	c.BUTTON的高度可根据BMP的高度自定义，通过MmiUtil_SetTouchButton来设置(默认的BMP文件高度为32像素)
	d.BMP文件为24位彩色，不可用32位彩色
*======================================================================================*/

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetTouchButton
* DESCRIPTION:   初始化显示button按钮参数
* PARAMETERS:
*		iButtonHeight		 (in) -- button的高度				（0-默认32像素）
*		iTextFontSize        (in) -- button上显示的文本的大小	（0-默认24点阵）
*		ulTextRGBColor       (in) -- button显示的文本的颜色		（0-默认黑色）
* RETURN:
* NOTES:		不会清屏，只是初始化BUTTON显示和触发时的相关参数
* ------------------------------------------------------------------------ */
void MmiUtil_SetTouchButton(int iButtonHeight,int iTextFontSize,unsigned long ulTextRGBColor);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ShowTouchButton
* DESCRIPTION:   显示button按钮,可配合MmiUtil_CheckTouchButton使用
* PARAMETERS:
*		Str		 (in)	  -- button上显示的字（带\x00结束符）
*		x        (in/out) -- button显示左上角横坐标（像素）,返回button的右下角横坐标
*		y        (in/out) -- button显示左上角纵坐标（像素）,返回button的右下角纵坐标
*		RetVal	 (in)  	  -- 按下button时返回值设定
* RETURN:
* NOTES:		最多可支持20个BUTTON，使用前须用MmiUtil_SetTouchButton进行设置
*				若出现BUTTON重叠，则以最后一个显示的BUTTON为准
*				可通过返回的(x,y)坐标决定下一个BUTTON显示的位置
* ------------------------------------------------------------------------ */
void MmiUtil_ShowTouchButton(char *Str, int *x, int *y,int RetVal);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_CheckTouchButton
* DESCRIPTION:   检查是否有BUTTON被按下
* PARAMETERS:
*		RetVal (out)     - 被按下的BUTTON设定的返回值
*
* RETURN:        0 - 无按钮被按下
*				 1 - 有按钮被按下
*
* NOTES:         非阻塞，立即返回
*				 若出现BUTTON重叠，则以最后一个显示的BUTTON为准
*				 如果没有做MmiUtil_SetTouchButton，则无法检测到BUTTON
* ------------------------------------------------------------------------ */
int MmiUtil_CheckTouchButton(int *RetVal);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetPrinterType.
* DESCRIPTION:   获取打印机类型
* PARAMETERS:
* RETURN:
*   PrinterType -- 打印机类型（参考enumPrinterType）
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetPrinterType(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayRightNoClear
* DESCRIPTION:   显示内容致至指定行(右对齐),不清行
* PARAMETERS:
*		msg       (in)	-- 要显示的中文区位码
*		disp_line (in)	-- 显示的行数
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayRightNoClear(char *msg, int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayCenter_ICBC
* DESCRIPTION:   居中显示内容至指定行(工行版，屏幕第一行显示图片)
* PARAMETERS:
*   msg       (in) -- 要显示的中文区位码
*   disp_line (in) -- 显示的行数
*   invert    (in) -- 是否反显TRUE/FALSE
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayCenter_ICBC(char *msg, int disp_line, boolean invert);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetKeypadState.
* DESCRIPTION:   设置keypad睡眠状态
* PARAMETERS:  
			sleep – 1 to put the keypad to sleep, 
					 0 to wake up the keypad
* RETURN:
*   zero if successful, nonzero if not.
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_SetKeypadState(int sleep);

int IsTTFSupport(void);

int isGbkFont16(void);

#endif
