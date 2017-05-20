/************************************************
 * T9PY.C至少使用了20K全局变量空间
 * 其中拼音索引表使用16K全局变量空间
 *************************************************/
#ifndef __T9PY_H__
#define __T9PY_H__

#define T9PY_NORMAL_MODE		0	//正常输入状态，即拼音输入状态
#define T9PY_PY_MODE			1	//拼音输入状态
#define T9PY_ABC_MODE			2	//大小写字母输入状态
#define	T9PY_NUMBER_MODE		3	//数字输入状态
#define	T9PY_SYMBOL_MODE		4	//符号输入状态

#define T9PY_MAX_PYLEN			6	//最大拼音长度zhuang
#define T9PY_MAX_NOMINEES		10	//最大拼音组合数如(426=gao,hao...)

#define T9PY_SYMBOL_FLG1		1	//*号#号键不存在
#define T9PY_SYMBOL_FLG2		2	//不支持符号输入
#define T9PY_ABC_FLG			4	//不支持字母输入
#define T9PY_NUMBER_FLG			8	//不支持数字输入

#ifndef T9PY_HELP_FONT
#define T9PY_HELP_FONT		   "F:/6X8.vft"
#endif

#ifndef PYLIB
#define PYLIB			"F:/PYLIB"
#endif

#ifndef PYINDEX
#define PYINDEX			"F:/PYINDEX"
#endif

/*
 * void T9PY_Init(int disp, int select, int _ALPHAKEY, 
			 int _KEYA, int _KEYB, int _KEYC, int _KEYD)
 * 初始化T9输入法，配置功能键和显示位置
 * disp -- 输入的字符所要显示的行位置。为0则使用默认2, 汉字方式的行数
 * select -- 输入法提示的显示的起始行位置.为0则使用默认3,汉字方式的行数
 * _ALPHAKEY -- 输入法拼音切换键和输入法切换，为0则使用默认ALPHA_KEY,
 * _KEYA -- 输入法汉字选择键，为0则使用默认KEY_a
 * _KEYB -- 输入法汉字选择键，为0则使用默认KEY_b
 * _KEYC -- 输入法汉字翻页键，为0则使用默认KEY_c
 * _KEYD -- 输入法汉字翻页键，为0则使用默认KEY_d
 * _DISABLE -- 输入法支持配置，
              为0默认配置全支持，即支持汉字，支持字母，支持数字，支持字符，支持*号和#号
			  &1则支持符号输入但不存在*号和#号键
			  &2则不支持符号输入
			  &4则不支持字母输入
			  &8则不支持数字输入
*/
void T9PY_Init(int disp, int select, int _ALPHAKEY, 
			 int _KEYA, int _KEYB, int _KEYC, int _KEYD, int _DISABLE);

/* --------------------------------------------------------------------------
* FUNCTION NAME: T9PY_Input.
* DESCRIPTION:   T9拼音汉字输入法
* PARAMETERS:
*		szOutPut	(out) -- 输入的字符串
*		iLen		(in)  -- 输入字符串的最大长度
*		usTimeout	(in)  -- 超时时间(秒)
* RETURN:
*		非0 -- 输入成功，返还字符串长度
*		INPUT_TIMEOUT/INPUT_USERABORT
* NOTES:
* ------------------------------------------------------------------------ */
int T9PY_Input(char * szOutPut, int iLen, usint usTimeout);

#ifdef _SHARED_LIB
void T9PY_InitPrm(void);
#endif

#endif
