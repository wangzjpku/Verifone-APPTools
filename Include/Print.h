/*-------------------------------------------------------------------
*
*	Filename	: CHNFONT.H
*
*	Description	: This file contains prototypes for the functions
*
*  Modification History:
*   #		Date        Who				Comments
*	1.0		DD-MMM-YYYY	VerixV Wizard		Initial Implementation
*
*-------------------------------------------------------------------*/
#ifndef PRINT_H
#define PRINT_H

#include "apptools.h"

#ifndef CHNFONT
#define CHNFONT		"F:/chn.fon"
#endif

#ifndef PRNCHN
#define PRNCHN		"/prnhz.txt"
#endif

#ifndef CHNFONT_12
#define CHNFONT_12		"f:/cn12x12.dat"
#endif

#ifndef ENGFONT_5x8
#define ENGFONT_5x8		"f:/en5x8.dat"
#endif

#ifndef ENGFONT_8x14
#define ENGFONT_8x14	"/Asc8x14.pft"
#endif

//define print attribution
#define NORMAL		0
#define DH          1
#define DW          2
#define INV         4

/*按格式打印*/
typedef enum
{
	FORMAT_PRINT=0, /* TOTAL:         123456.99?*/
	FORMAT_BOLD,    /* TOTAL:123456.99\n       ?*/
	FORMAT_LBOLD,   /* *C*A*S*H*I*E*R       003?where *X*X*X - BOLD font */
	FORMAT_RBOLD    /* Cashier           *0*0*3?where *X*X*X - BOLD font */
} TFormat;

/*打印返回*/
typedef enum
{
	PRINT_OK		= 0,		/*打印机正常*/
	PRINT_FAIL		= -1,		/*打印机故障*/
	PRINT_NOPAPER   = -2,       /*打印机缺纸*/
	PRINT_NOHANDLE  = -3,       /*未获取打印机句柄*/
	PRINT_OPENFAIL  = -4,		/*open打印文件失败*/
}enumPrintRet;

typedef enum
{
	FORWARD_PAPER=1,
	REVERSE_PAPER, 
} PaperFeed;

typedef enum
{
	MODE_180_DOTS = 0,
	MODE_360_DOTS = 1, 
} DotModeType;

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Init.
* DESCRIPTION:   初始化打印机
* PARAMETERS:    
* RETURN:  
*		PRINT_NOHANDLE/PRINT_OK
* NOTES:        
* ------------------------------------------------------------------------ */
int Print_Init(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_DelCache.
* DESCRIPTION:   删除打印临时文件
* PARAMETERS:	None
* RETURN:		Return value of _remove()
* NOTES:		Should be called once in MainApp
* ------------------------------------------------------------------------ */
int Print_DelCache(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Str.
* DESCRIPTION:   打印任意中英文字串的函数
* PARAMETERS:  
*		  str (in) -- 要打印的字符串
*		  attrib (in) -- 打印属性
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES:        
* ------------------------------------------------------------------------ */
int Print_Str(char *str, unsigned char attrib);

/* --------------------------------------------------------------------------
* FUNCTION NAME:  Print_StrStr
* DESCRIPTION:    在同一行打印2个buffer with/without justification
* PARAMETERS:  
*		cLStr (in) -- 左侧的字符串
*		cRStr (in) -- 右侧的字符串
*		form (in)  -- 打印格式（参考TFormat定义） 
* RETURN:         none
* NOTES:
*  1.left string can be overwritten by right one, if the strings too long
*  2.form=FORMAT_PRINT   causes left/right-justification of the strings ;
*    form=WITHOUT_FORMAT cause string's concatenation and printing as
*                        single string.
*    form=LEFT_BOLD      causes left/right justification like in the case
*                        of FORMAT_PRINT, left-justified part will be
*                        printed by Bold font.
* ------------------------------------------------------------------------ */
void Print_StrStr(char *cLStr, char *cRStr, TFormat form);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_English.
* DESCRIPTION:   打印英文字符串
* PARAMETERS:
*		english_str (in) -- 英文字符串
*		font_size	(in) -- 0x02, 0x03, 0x04
* RETURN:
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
*		对于热敏打印机
*		0x02字体，一行可打印24个字符
*		0x03字体，一行可打印32个字符
*		0x04字体，一行可打印42个字符
*		对于针式打印机
*		0x02字体，一行可打印30个字符
*		0x03字体，一行可打印15个字符
*		0x04字体，一行可打印20个字符
* ------------------------------------------------------------------------ */
int Print_English(char *english_str, short font_size);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_EngSmall.
* DESCRIPTION:   打印英文字符串（小字体）
* PARAMETERS:  
*		english_str （in）-- 英文字符串
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES:        
*		对于热敏打印机，一行可打印42个字符
*		对于针式打印机，一行可打印30个字符
* ------------------------------------------------------------------------ */
int Print_EngSmall(char *english_str);

/* --------------------------------------------------------------------------
 * FUNCTION NAME: Print_Hex
 * DESCRIPTION:   打印16进制数据.
 * PARAMETERS:    
 *		msgPrompt (in) - 提示信息
 *		Buf       (in) - 数据
 *		BufLen    (in) - 数据长度.
 * RETURN:  
 * NOTES: 
 * ------------------------------------------------------------------------ */
void Print_Hex(char *msgPrompt, byte *Buf, int BufLen);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_logo.
* DESCRIPTION:   打印lgo图片
* PARAMETERS:  
*		Offset （in）-- 图片左边的空白
*		LogoFile （in）-- 图片文件名称（后缀.lgo）
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES:        
* ------------------------------------------------------------------------ */
int Print_logo(short offset, char *LogoFile);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_bitmap.
* DESCRIPTION:   打印bitmap文件
* PARAMETERS:  
*		Offset （in）-- 图片左边的空白
*		BmpFile （in）-- 图片文件名称（后缀.bmp）
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES: 打印完bmp之后，打印机的handle被关闭了。需要重新打开打印机设备并初始化。       
* ------------------------------------------------------------------------ */
int Print_bitmap(short offset, char *BmpFile);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_FormFeed
* DESCRIPTION:    针式打印机换页指令
* PARAMETERS :    
* RETURN:         
* NOTES:
 * ----------------------------------------------------------------------*/
int Print_FormFeed(void);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_SetLineHeight
* DESCRIPTION:    设置行高度 
* PARAMETERS :
*			height(in) -- 行高度(default is 22, min is 16, max is 48)
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
void Print_SetLineHeight(int height);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_SetFormLength
* DESCRIPTION:    设置针打纸页面长度
* PARAMETERS :
*			length(in) -- 页面长度(default is 140mm, max is 300mm)
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
void Print_SetFormLength(int length);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_Feed
* DESCRIPTION:    针式打印机进纸/退纸指令
* PARAMETERS :
*		flag(in)   - FORWARD_PAPER:进纸/ REVERSE_PAPER:退纸
*		length(in) - 毫米
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
int Print_Feed(int flag, int length);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Str_12.
* DESCRIPTION:   以图形方式打印任意中（12x12）英文（5x8）字串的函数
* PARAMETERS:
*		  str (in) -- 要打印的字符串
*		  attrib (in) -- 打印属性
* RETURN:
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Print_Str_12(char *str, unsigned char attrib);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_SetDark.
* DESCRIPTION:   设置打印黑度*DARK
* PARAMETERS:
*    darkValue(in) ----- (取值范围1-29)
* RETURN:
*		PRINT_NOHANDLE/PRINT_OK
* NOTES:
* ------------------------------------------------------------------------ */
void Print_SetDark(int darkValue);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_PreLoadFont.
* DESCRIPTION:   下载16x16点阵字库，在Print_init后执行
* PARAMETERS:    
* RETURN:  
* NOTES:        
* ------------------------------------------------------------------------ */
void Print_PreLoadFont(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_SetSprocketDotMode.
* DESCRIPTION:   设置520针打的字体格式，需要os支持 ，
* PARAMETERS:
     dotmode   -- 取值 MODE_180_DOTS ,MODE_360_DOTS , 
	
* RETURN:  
      是否设置成功，请检查返回值是否与参数一致 
			MODE_360_DOTS  ,表明设置为小字体格式
			MODE_180_DOTS  ,如果 dotmode 为 MODE_360_DOTS，而返回 MODE_180_DOTS ，则表明可能是os不支持等原因. 
* NOTES:        
* ------------------------------------------------------------------------ */

int Print_SetSprocketDotMode(int dotmode);
#endif

