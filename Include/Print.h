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

/*����ʽ��ӡ*/
typedef enum
{
	FORMAT_PRINT=0, /* TOTAL:         123456.99?*/
	FORMAT_BOLD,    /* TOTAL:123456.99\n       ?*/
	FORMAT_LBOLD,   /* *C*A*S*H*I*E*R       003?where *X*X*X - BOLD font */
	FORMAT_RBOLD    /* Cashier           *0*0*3?where *X*X*X - BOLD font */
} TFormat;

/*��ӡ����*/
typedef enum
{
	PRINT_OK		= 0,		/*��ӡ������*/
	PRINT_FAIL		= -1,		/*��ӡ������*/
	PRINT_NOPAPER   = -2,       /*��ӡ��ȱֽ*/
	PRINT_NOHANDLE  = -3,       /*δ��ȡ��ӡ�����*/
	PRINT_OPENFAIL  = -4,		/*open��ӡ�ļ�ʧ��*/
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
* DESCRIPTION:   ��ʼ����ӡ��
* PARAMETERS:    
* RETURN:  
*		PRINT_NOHANDLE/PRINT_OK
* NOTES:        
* ------------------------------------------------------------------------ */
int Print_Init(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_DelCache.
* DESCRIPTION:   ɾ����ӡ��ʱ�ļ�
* PARAMETERS:	None
* RETURN:		Return value of _remove()
* NOTES:		Should be called once in MainApp
* ------------------------------------------------------------------------ */
int Print_DelCache(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Str.
* DESCRIPTION:   ��ӡ������Ӣ���ִ��ĺ���
* PARAMETERS:  
*		  str (in) -- Ҫ��ӡ���ַ���
*		  attrib (in) -- ��ӡ����
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES:        
* ------------------------------------------------------------------------ */
int Print_Str(char *str, unsigned char attrib);

/* --------------------------------------------------------------------------
* FUNCTION NAME:  Print_StrStr
* DESCRIPTION:    ��ͬһ�д�ӡ2��buffer with/without justification
* PARAMETERS:  
*		cLStr (in) -- �����ַ���
*		cRStr (in) -- �Ҳ���ַ���
*		form (in)  -- ��ӡ��ʽ���ο�TFormat���壩 
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
* DESCRIPTION:   ��ӡӢ���ַ���
* PARAMETERS:
*		english_str (in) -- Ӣ���ַ���
*		font_size	(in) -- 0x02, 0x03, 0x04
* RETURN:
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
*		����������ӡ��
*		0x02���壬һ�пɴ�ӡ24���ַ�
*		0x03���壬һ�пɴ�ӡ32���ַ�
*		0x04���壬һ�пɴ�ӡ42���ַ�
*		������ʽ��ӡ��
*		0x02���壬һ�пɴ�ӡ30���ַ�
*		0x03���壬һ�пɴ�ӡ15���ַ�
*		0x04���壬һ�пɴ�ӡ20���ַ�
* ------------------------------------------------------------------------ */
int Print_English(char *english_str, short font_size);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_EngSmall.
* DESCRIPTION:   ��ӡӢ���ַ�����С���壩
* PARAMETERS:  
*		english_str ��in��-- Ӣ���ַ���
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES:        
*		����������ӡ����һ�пɴ�ӡ42���ַ�
*		������ʽ��ӡ����һ�пɴ�ӡ30���ַ�
* ------------------------------------------------------------------------ */
int Print_EngSmall(char *english_str);

/* --------------------------------------------------------------------------
 * FUNCTION NAME: Print_Hex
 * DESCRIPTION:   ��ӡ16��������.
 * PARAMETERS:    
 *		msgPrompt (in) - ��ʾ��Ϣ
 *		Buf       (in) - ����
 *		BufLen    (in) - ���ݳ���.
 * RETURN:  
 * NOTES: 
 * ------------------------------------------------------------------------ */
void Print_Hex(char *msgPrompt, byte *Buf, int BufLen);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_logo.
* DESCRIPTION:   ��ӡlgoͼƬ
* PARAMETERS:  
*		Offset ��in��-- ͼƬ��ߵĿհ�
*		LogoFile ��in��-- ͼƬ�ļ����ƣ���׺.lgo��
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES:        
* ------------------------------------------------------------------------ */
int Print_logo(short offset, char *LogoFile);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_bitmap.
* DESCRIPTION:   ��ӡbitmap�ļ�
* PARAMETERS:  
*		Offset ��in��-- ͼƬ��ߵĿհ�
*		BmpFile ��in��-- ͼƬ�ļ����ƣ���׺.bmp��
* RETURN:  
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL     
* NOTES: ��ӡ��bmp֮�󣬴�ӡ����handle���ر��ˡ���Ҫ���´򿪴�ӡ���豸����ʼ����       
* ------------------------------------------------------------------------ */
int Print_bitmap(short offset, char *BmpFile);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_FormFeed
* DESCRIPTION:    ��ʽ��ӡ����ҳָ��
* PARAMETERS :    
* RETURN:         
* NOTES:
 * ----------------------------------------------------------------------*/
int Print_FormFeed(void);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_SetLineHeight
* DESCRIPTION:    �����и߶� 
* PARAMETERS :
*			height(in) -- �и߶�(default is 22, min is 16, max is 48)
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
void Print_SetLineHeight(int height);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_SetFormLength
* DESCRIPTION:    �������ֽҳ�泤��
* PARAMETERS :
*			length(in) -- ҳ�泤��(default is 140mm, max is 300mm)
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
void Print_SetFormLength(int length);

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_Feed
* DESCRIPTION:    ��ʽ��ӡ����ֽ/��ָֽ��
* PARAMETERS :
*		flag(in)   - FORWARD_PAPER:��ֽ/ REVERSE_PAPER:��ֽ
*		length(in) - ����
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
int Print_Feed(int flag, int length);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Str_12.
* DESCRIPTION:   ��ͼ�η�ʽ��ӡ�����У�12x12��Ӣ�ģ�5x8���ִ��ĺ���
* PARAMETERS:
*		  str (in) -- Ҫ��ӡ���ַ���
*		  attrib (in) -- ��ӡ����
* RETURN:
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Print_Str_12(char *str, unsigned char attrib);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_SetDark.
* DESCRIPTION:   ���ô�ӡ�ڶ�*DARK
* PARAMETERS:
*    darkValue(in) ----- (ȡֵ��Χ1-29)
* RETURN:
*		PRINT_NOHANDLE/PRINT_OK
* NOTES:
* ------------------------------------------------------------------------ */
void Print_SetDark(int darkValue);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_PreLoadFont.
* DESCRIPTION:   ����16x16�����ֿ⣬��Print_init��ִ��
* PARAMETERS:    
* RETURN:  
* NOTES:        
* ------------------------------------------------------------------------ */
void Print_PreLoadFont(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_SetSprocketDotMode.
* DESCRIPTION:   ����520���������ʽ����Ҫos֧�� ��
* PARAMETERS:
     dotmode   -- ȡֵ MODE_180_DOTS ,MODE_360_DOTS , 
	
* RETURN:  
      �Ƿ����óɹ������鷵��ֵ�Ƿ������һ�� 
			MODE_360_DOTS  ,��������ΪС�����ʽ
			MODE_180_DOTS  ,��� dotmode Ϊ MODE_360_DOTS�������� MODE_180_DOTS �������������os��֧�ֵ�ԭ��. 
* NOTES:        
* ------------------------------------------------------------------------ */

int Print_SetSprocketDotMode(int dotmode);
#endif

