/***************************
 * MMIUTIL.Cʹ��������3K��ȫ�ֱ����ռ�
 ***************************************/
#ifndef __MMIUTIL_H__
#define __MMIUTIL_H__

//����ۺ�ɫ���ܼ�
#define bKEYA			KEY_a
#define bKEYB			KEY_b
#define bKEYC			KEY_c
#define bKEYD			KEY_d

//����ATM��
#define bF1				KEY_e
#define bF2				KEY_f
#define bF3				KEY_g
#define bF4				KEY_h
//��ĸ��
#define bALPHAKEY		ALPHA_KEY
//���ܼ�
#define bESC			KEY_CANCEL
#define bENTER			KEY_CR
#define bBKSP			KEY_BS
#define bCLR			KEY_CLR

#define bSTR			KEY_STR
#define bPND			KEY_PND
//������
#define bMENU			ALPHA_KEY
#define bUP				KEY_UP
#define bDOWN			KEY_DOWN
//���ּ�
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

#define DEFAULT_CHAR_WIDTH	16	//Ĭ����Ļ���  8*16 = 128
#define DEFAULT_CHAR_HEIGHT 4   //Ĭ����Ļ���  16*4 = 64
#define HEADER_HEIGHT 32      

#define MAX_KEY_WAIT	30	//��

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

//�ն��ͺ�
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

//��ӡ������
typedef enum
{
	THERMAL_PRINTER  = 0,
	SPROCKET_PRINTER
}enumPrinterType;

/*ˢ������*/
typedef enum
{
	TRACK_NOEVENT   = -5,	   /* ���¼� */
	TRACK_ESC       = -3,      /* �û��ж� bESC */
	TRACK_TIMEOUT   = -2,      /* ˢ����ʱ*/
	TRACK_FAIL      = -1,      /* ˢ��ʧ��*/
	TRACK_OK        = 0,       /* ��ѭ��ʱ,�޿��޼�����*/
	TRACK_NUM       = 1,       /* ���俨�� */
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
* DESCRIPTION:   ��ȡ�ն�����
* PARAMETERS:
* RETURN:
*		TerminalType -- �ն����ͣ��ο�enumTerminalType��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetTerminalType(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetSerialNO.
* DESCRIPTION:   ��ȡ�ն����к�
* PARAMETERS:
*		seri_no��out�� -- ���кţ�11 bytes��
* RETURN:
*		0���ɹ��� / -1 ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetSerialNO(char *seri_no);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetBatteryCharge.
* DESCRIPTION:   ��ȡ��ص���
* PARAMETERS:
* RETURN:
*		battery charge -- ��ص������ٷֱ� 0-100��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetBatteryCharge(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetBatteryStatus.
* DESCRIPTION:   ��ȡ���״̬
* PARAMETERS:
* RETURN:
*		�ɹ� -- NORMAL_LEVEL   (1)
*		ʧ�� -- CRITICAL_LEVEL (0)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetBatteryStatus(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetDockStatus.
* DESCRIPTION:   ��ȡ���״̬
* PARAMETERS:
* RETURN:
*		�ɹ� -- HANDHELD_DOCKED   (0)
*		ʧ�� -- HANDHELD_UNDOCKED (-1)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetDockStatus(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetGroup.
* DESCRIPTION:   �ı�Group ID
* PARAMETERS:
*		group_id (in) -- ��Group ID
* RETURN:
*		�ɹ� -- group id   ( >=1 )
*		ʧ�� -- (-1)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_SetGroup(int group_id);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetGroup.
* DESCRIPTION:   ��ȡGroup ID
* PARAMETERS:
* RETURN:
*		Group ID
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetGroup(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Restart.
* DESCRIPTION:   �ն�����
* PARAMETERS:
*		filename (in) -- ��ִ���ļ�����""
* RETURN:
*		�ɹ� -- 
*		ʧ�� -- (<0)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_Restart(const char *filename);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontSize.
* DESCRIPTION:   ������Ļ��ʾ�����С
* PARAMETERS:    
*		font_size (in) -- �����С���ο�FONT_SIZE���壩
* RETURN:        
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_SetFontSize(int font_size);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontSize.
* DESCRIPTION:   ��ȡ��Ļ��ʾ�����С
* PARAMETERS:    	
* RETURN:  
*	font_size -- �����С���ο�FONT_SIZE���壩      
* NOTES:         
* ------------------------------------------------------------------------ */
int MmiUtil_GetFontSize(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFont.
* DESCRIPTION:   ������Ļ��ʾ�����ļ�
* PARAMETERS:    
*	szFont (in) -- ����·��������ó���20���ֽ�,Ϊ��������ΪĬ������
* RETURN:       
* NOTES:        
* ------------------------------------------------------------------------ */
void MmiUtil_SetFont(const char * szFont);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetFont.
* DESCRIPTION:   ��ȡ��Ļ��ʾ�����ļ�
* PARAMETERS:    
*	szFont (out) -- ����·��������ó���20���ֽ�,Ϊ��������ΪĬ������
* RETURN:       
* NOTES:        
* ------------------------------------------------------------------------ */
void MmiUtil_GetFont(char * szFont);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontColor.
* DESCRIPTION:  ����������ɫ����680/820��Ч
* PARAMETERS:
*	FontColor (in) -- ������ɫRGB����3�ֽڷֱ��ʾRED,GREEN,BLUE��
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_SetFontColor(unsigned long RGBFontColor);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Window
* DESCRIPTION:  ������ʾ��Ļ
* PARAMETERS:    
*			startX (in) -- ���ϽǺ�����
*			startY (in) -- ���Ͻ�������
*			endX   (in) -- ���½Ǻ�����
*			endY   (in) -- ���½�������
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_Window(int startX, int startY, int endX, int endY);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Clr
* DESCRIPTION:  ���ָ����Ļ
* PARAMETERS:
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_Clr(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WriteAt
* DESCRIPTION:  ��Ļ��ʾ
* PARAMETERS:
*		buf(in) -- ��ʾ������
*		len(in) -- ��ʾ�����ݵĳ���
*		x(in)   -- ��ʾ��ʼ����(column)
*		y(in)	-- ��ʾ��ʼ����(row) 
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_WriteAt(char *buf, int len, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetColumms.
* DESCRIPTION:   ��ȡ��ʾ��Ļ�������
* PARAMETERS:    
* RETURN:       
*	Columms - ��Ļ������		
* NOTES:        
* ------------------------------------------------------------------------ */
usint MmiUtil_GetColumms(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetRows.
* DESCRIPTION:   ��ȡ��ʾ��Ļ�������
* PARAMETERS:    
* RETURN:       
*	Rows - ��Ļ������		
* NOTES:        
* ------------------------------------------------------------------------ */
usint MmiUtil_GetRows(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Reset
* DESCRIPTION:  ��ʼ����ʾ��Ļ,������Ļ��ʾ����
* PARAMETERS:    
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_Reset(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearLine
* DESCRIPTION:  ���ָ���У�1�У�
* PARAMETERS: 
*		disp_line (in) -- Ҫ�������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearLine(int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearLines
* DESCRIPTION:  ���ָ���У����У�
* PARAMETERS: 
*		start_line (in) -- ��ʼ��
*		end_line   (in) -- ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearLines(int start_line, int end_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_UserClr
* DESCRIPTION:  �����Ļ�·���ʾ���򣨴ӵڼ��п�ʼ��
* PARAMETERS: 
*		start_line (in) -- ��ʼ��
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_UserClr(int start_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispStr.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ��Ӣ��. Vx680ר��
* PARAMETERS:
*		str (in) -- Ҫ��ʾ��������λ��
*		startX   (in) -- ��ʼ�к�, ��0��ʼ, ���239
*		startY   (in) -- ��ʼ�к�, ��0��ʼ, ���319
*   fontSize (in) -- �����С
* RETURN:
* NOTES:         
* ------------------------------------------------------------------------ */
void MmiUtil_DispStr(char *str, int startX, int startY, int fontSize);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispStrByColor.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ��Ӣ��/����,������ɫ�ͱ������Զ���. Vx680/820ר��
* PARAMETERS:
*		str		 (in) -- Ҫ��ʾ���ַ�
*		startX   (in) -- ��ʼ�к�, ��0��ʼ, ���239
*		startY   (in) -- ��ʼ�к�, ��0��ʼ, ���319
*		fontSize (in) -- �����С
*		FontColor(in) -- ������ɫRGB����3�ֽڷֱ��ʾRED,GREEN,BLUE��
*		BGColor	 (in) -- ������ɫRGB����3�ֽڷֱ��ʾRED,GREEN,BLUE��
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DispStrByColor(char *str, int startX, int startY, int fontSize,unsigned long  RGBFontColor, unsigned long  RGBBGColor);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispChinese.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ������.
* PARAMETERS:    
*		str (in) -- Ҫ��ʾ��������λ��
*		x   (in) -- �к�, ��1��ʼ
*		y   (in) -- �к�, ��1��ʼ
* RETURN:        
* NOTES:         �����ļ��ɳ�����ǰָ��
* ------------------------------------------------------------------------ */
void MmiUtil_DispChinese(char *str, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispChineseInvert.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ����һ������.
* PARAMETERS:    
*		str (in) -- Ҫ��ʾ��������λ��
*		x   (in) -- �к�, ��1��ʼ
*		y   (in) -- �к�, ��1��ʼ
* RETURN: 
* NOTES:		�����ļ��ɳ�����ǰָ��
* ------------------------------------------------------------------------ */
void MmiUtil_DispChineseInvert(char *str, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispEnglish.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ��Ӣ��.
* PARAMETERS: 
*		str (in) -- Ҫ��ʾ��Ӣ��
*		x   (in) -- �к�, ��1��ʼ
*		y   (in) -- �к�, ��1��ʼ
* RETURN:        
* NOTES:         Ӣ������ʹ��ϵͳĬ�����壨Vx520��6X8��Vx680��9X12��
* ------------------------------------------------------------------------ */
void MmiUtil_DispEnglish(char *str, int x, int y);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayLine
* DESCRIPTION:   ��ʾ��������ָ����(�����)
* PARAMETERS:    
*		msg       (in)	-- Ҫ��ʾ��������λ��
*		disp_line (in)	-- ��ʾ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayLine(char *msg, int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayRight
* DESCRIPTION:   ��ʾ��������ָ����(�Ҷ���)������
* PARAMETERS:    
*		msg		   (in)	-- Ҫ��ʾ��������λ��
*		disp_line  (in)	-- ��ʾ������
* RETURN: 
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayRight(char *msg, int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayCenter
* DESCRIPTION:   ������ʾ������ָ����
* PARAMETERS:    
*		msg       (in) -- Ҫ��ʾ��������λ��
*		disp_line (in) -- ��ʾ������
*		invert    (in) -- �Ƿ���TRUE/FALSE
* RETURN:
* NOTES: 
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayCenter(char *msg, int disp_line, boolean invert);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetTimeout.
* DESCRIPTION:   ����MmiUtilϵ�����뺯���ĳ�ʱʱ��
* PARAMETERS:    
*		usTimeout (in) -- ��ʱʱ��,��λ��,Ϊ0������ʽ�ĵȴ�����
* RETURN:        
* NOTES:         ��ʱʱ��Ĭ��30��
* ------------------------------------------------------------------------ */
void MmiUtil_SetTimeOut(usint usTimeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetTimeout.
* DESCRIPTION:   ��ȡMmiUtilϵ�����뺯���ĳ�ʱʱ��
* PARAMETERS:    
* RETURN:
*	usTimeout -- ��ʱʱ��,��λ��,Ϊ0������ʽ�ĵȴ�����
* NOTES:	��ʱʱ��Ĭ��30��
* ------------------------------------------------------------------------ */
usint MmiUtil_GetTimeOut(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_CheckKey.
* DESCRIPTION:   ����Ƿ��а���
* PARAMETERS:  
* RETURN:        ���û�а�������0�����򷵻ذ����ļ�ֵ
* NOTES:
* ------------------------------------------------------------------------ */
byte MmiUtil_CheckKey(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyMenu
* DESCRIPTION:   �ȴ���ʱ���룩�򰴼���680�������˵�
* PARAMETERS:
*		Timeout (in)     - ��ʱ���룩��Ϊ0ʱ�޳�ʱ
*		Count   (in)     - �ܲ˵�����
*		Head    (in)     - TRUE���в˵�ͷ��FALSE���޲˵�ͷ
*		OneMenu (in)     - TRUE��һ��һ���˵���FALSE��һ�ж����˵�
*		Base    (in)     - �˵���'0'��'1'��ʼ��
* RETURN:        ��ʱ����0�����򷵻ذ���ֵ
* NOTES:         ����Ϊ��λ.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyMenu(usint Timeout, usint Count, usint Head, usint OneMenu, byte Base);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyButton
* DESCRIPTION:   �ȴ���ʱ���룩�򰴼�������680��������Ŧ
* PARAMETERS:
*		Timeout (in)     - ��ʱ���룩��Ϊ0ʱ�޳�ʱ
* RETURN:        ��ʱ����0�����򷵻ذ���ֵ
* NOTES:         ����Ϊ��λ.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyButton(usint Timeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKey
* DESCRIPTION:   �ȴ���ʱ���룩�򰴼�
* PARAMETERS: 
*		Timeout (in) - ��ʱ���룩��Ϊ0ʱ�޳�ʱ
* RETURN:        ��ʱ����0�����򷵻ذ���ֵ
* NOTES:         ����Ϊ��λ.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKey(usint Timeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyMill
* DESCRIPTION:   �ȴ���ʱ�����룩�򰴼�
* PARAMETERS: 
*		Timeout (in) - ��ʱ�����룩��Ϊ0ʱ�޳�ʱ
* RETURN:        ��ʱ����0�����򷵻ذ���ֵ
* NOTES:         �Ժ���Ϊ��λ.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyMill(ulint Timeout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Beep
* DESCRIPTION:   ����300ms�ȴ�200ms
* PARAMETERS: 
* RETURN: 
* ------------------------------------------------------------------------ */
void MmiUtil_Beep(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ErrorSound
* DESCRIPTION:   ����300ms�ȴ�200ms������300ms
* PARAMETERS: 
* RETURN:        none
* ------------------------------------------------------------------------ */
void MmiUtil_ErrorSound(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning.
* DESCRIPTION:   ��ʾ������Ϣ���ȴ���������ʱ�˳���
* PARAMETERS:
*		text (in) -- ������Ϣ	
* RETURN:         
*		0		  -- ��ʱ
*		key_value -- ����ֵ
* NOTES:  
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning(char *text);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning2.
* DESCRIPTION:   ��ʾ������Ϣ���ȴ��������޳�ʱ��
* PARAMETERS:
*		text (in) -- ������Ϣ	
* RETURN:         
*		key_value -- ����ֵ
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning2(char *text);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning3.
* DESCRIPTION:   ��ʾ������Ϣ���ȴ���������ʱ�˳���
* PARAMETERS:
*   text (in) -- ������Ϣ
*	iTimeout(in) --��ʱʱ��
* RETURN:
*   0     -- ��ʱ
*   key_value -- ����ֵ
* NOTES:
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning3(char *text, usint iTiemout );


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputText
* DESCRIPTION:   �����ַ���(��ĸ������)
* PARAMETERS:   
*		disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*		msgPrompt (in)  -- ��ʾ��Ϣ
*		str       (out) -- ����ַ���
*		min       (in)  -- ����������С����
*		max       (in)  -- ����������󳤶�
*		disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputLongText
* DESCRIPTION:   �����ַ���(��ĸ������)
* PARAMETERS:   
*		disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*		msgPrompt (in)  -- ��ʾ��Ϣ
*		str       (out) -- ����ַ���
*		min       (in)  -- ����������С����
*		max       (in)  -- ����������󳤶�
*		disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputLongText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputNum
* DESCRIPTION:   �������ִ�
* PARAMETERS:   
*		disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*		msgPrompt (in)  -- ��ʾ��Ϣ
*		str       (out) -- ������ִ�
*		min       (in)  -- ����������С����
*		max       (in)  -- ����������󳤶�
*		disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputNum(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputLongNum
* DESCRIPTION:   �������ִ�
* PARAMETERS:   
*		disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*		msgPrompt (in)  -- ��ʾ��Ϣ
*		str       (out) -- ������ִ�
*		min       (in)  -- ����������С����
*		max       (in)  -- ����������󳤶�
*		disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputLongNum(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputPWD
* DESCRIPTION:   ��������
* PARAMETERS:   
*		disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*		msgPrompt (in)  -- ��ʾ��Ϣ
*		pwd       (out) -- �������
*		min       (in)  -- ����������С����
*		max       (in)  -- ����������󳤶�
*		disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN: 
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputPWD(int disp_line, char * msgPrompt, char *pwd, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputAmount
* DESCRIPTION:   ������
* PARAMETERS:   
*		disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*		msgPrompt (in)  -- ��ʾ��Ϣ
*		amount    (out) -- ������
*		min       (in)  -- ����������С����
*		max       (in)  -- ����������󳤶�
*		disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN: 
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputAmount(int disp_line, char * msgPrompt, char *amount, usint min, usint max, int disp_mode);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputBcdCode
* DESCRIPTION:   ��ʾ��ʾ������BCD���ַ���.
* PARAMETERS:     
*		DispMsg   (in)   -- ��ʾ,����ΪNULL.
*       BcdBuf    (out)  -- �������in BCD
*       BcdBytes  (in)   -- Ҫ������ֽ���(in BCD)���100
*       IsDispOrg (in)   --�Ƿ���ʾԭ����Ϣ
* RETURN:         >=0 ������ֽڸ���
*				INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTES:
* ------------------------------------------------------------------------ */
sint MmiUtil_InputBcdCode(int disp_line, char *DispMsg, byte *BcdBuf, 
						   usint BcdBytes, boolean IsDispOrg);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MMiUtil_GetTrack
* DESCRIPTION:		�������������俨�ţ�ѭ���ȴ�����ʱ�˳���
* PARAMETERS:
*		CardBuf1 (out) -- 1�ŵ�����
*		CardBuf2 (out) -- 2�ŵ�����
*		CardBuf3 (out) -- 3�ŵ�����
*		PANnum   (out) -- �ֹ�����Ŀ���
*		manflag  (in)  -- �Ƿ�֧�����俨��
* RETURN:
*		TRACK_OK/TRACK_NUM/TRACK_ESC/TRACK_TIMEOUT
* NOTES:
* --------------------------------------------------------------------------*/
int MmiUtil_GetTrack(byte *CardBuf1, byte *CardBuf2, byte *CardBuf3, byte *PANnum, int manflag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MMiUtil_GetTrack2
* DESCRIPTION:		�������������俨�ţ�ֻ��һ�Σ�û��ѭ����
* PARAMETERS:
*		CardBuf1 (out) -- 1�ŵ�����
*		CardBuf2 (out) -- 2�ŵ�����
*		CardBuf3 (out) -- 3�ŵ�����
*		PANnum   (out) -- �ֹ�����Ŀ���
*		manflag  (in)  -- �Ƿ�֧�����俨��
* RETURN:
*		TRACK_OK/TRACK_NUM/TRACK_ESC/TRACK_NOEVENT
* NOTES:
* --------------------------------------------------------------------------*/
int MmiUtil_GetTrack2(byte *CardBuf1, byte *CardBuf2, byte *CardBuf3, byte *PANnum, int manflag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearEngLines
* DESCRIPTION:  ���ָ���У����У�Ӣ��С���壩
* PARAMETERS:
*		start_line (in) -- ��ʼ��
*		end_line   (in) -- ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearEngLines(int start_line, int end_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearEngLines
* DESCRIPTION:  ���ָ���У�1�У�Ӣ��С���壩
* PARAMETERS:
*		disp_line (in) -- Ҫ�������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearEngLine(int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispBMP
* DESCRIPTION:   ��ʾbitmap�ļ�
* PARAMETERS:
*		BmpFile  (in) -- ͼƬ�ļ����ƣ���׺.bmp��
*		x        (in) -- ͼƬ��ʾ���ϽǺ����꣨���أ�
*		y        (in) -- ͼƬ��ʾ���Ͻ������꣨���أ�
*		width    (in) -- ͼƬ��ȣ����أ�
*		height   (in) -- ͼƬ�߶ȣ����أ�
* RETURN:
*		0(success)/ -1(fail)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_DispBMP(char *BmpFile, int x, int y, int width, int height);

/* ------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_flushkbd
* DESCRIPTION:   ��հ���buffer
* PARAMETERS:
* RETURN:
* NOTES:
* -----------------------------------------------------------------------*/
void MmiUtil_flushkbd(void);

/* ------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_flushcard
* DESCRIPTION:  ��մ�����������buffer
* PARAMETERS:
* RETURN:
* NOTES:
* -----------------------------------------------------------------------*/
void MmiUtil_flushcard(void);

#ifdef _SHARED_LIB
void MmiUtil_InitPrm(void);
#endif

/*=====================================================================================*
	 								BUTTON������ģ��˵��								   

  1.BUTTONֻ��VX680/820��320*240��������ʹ�á�

  2.BUTTON���ȸ�����ʾ�ı��ĳ����Զ�������
  
  3.BUTTONѡ�к�ķ���ֵ�������á�
	
  4.ʹ��G15 FLASH�е�BTN1_L.BMP,BTN1_R.BMP,BTN1_C.BMP��Ϊ��ʾBUTTON�����
		BTN2_L.BMP,BTN2_R.BMP,BTN2_C.BMP��Ϊѡ��ʱ��ʾ��BUTTON�����

  5.����ͨ�����������ļ����ﵽ�Լ�������Ч������Ҫ��ѭ����Ҫ��
	a.BUTTONͨ�����С���3��ͼƬ�ļ���϶���
	b.�м䲿�ֵĳ���Ϊ24���أ�����Ҳ��ֵ�ͼƬ���Ϊ10������
	c.BUTTON�ĸ߶ȿɸ���BMP�ĸ߶��Զ��壬ͨ��MmiUtil_SetTouchButton������(Ĭ�ϵ�BMP�ļ��߶�Ϊ32����)
	d.BMP�ļ�Ϊ24λ��ɫ��������32λ��ɫ
*======================================================================================*/

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetTouchButton
* DESCRIPTION:   ��ʼ����ʾbutton��ť����
* PARAMETERS:
*		iButtonHeight		 (in) -- button�ĸ߶�				��0-Ĭ��32���أ�
*		iTextFontSize        (in) -- button����ʾ���ı��Ĵ�С	��0-Ĭ��24����
*		ulTextRGBColor       (in) -- button��ʾ���ı�����ɫ		��0-Ĭ�Ϻ�ɫ��
* RETURN:
* NOTES:		����������ֻ�ǳ�ʼ��BUTTON��ʾ�ʹ���ʱ����ز���
* ------------------------------------------------------------------------ */
void MmiUtil_SetTouchButton(int iButtonHeight,int iTextFontSize,unsigned long ulTextRGBColor);


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ShowTouchButton
* DESCRIPTION:   ��ʾbutton��ť,�����MmiUtil_CheckTouchButtonʹ��
* PARAMETERS:
*		Str		 (in)	  -- button����ʾ���֣���\x00��������
*		x        (in/out) -- button��ʾ���ϽǺ����꣨���أ�,����button�����½Ǻ�����
*		y        (in/out) -- button��ʾ���Ͻ������꣨���أ�,����button�����½�������
*		RetVal	 (in)  	  -- ����buttonʱ����ֵ�趨
* RETURN:
* NOTES:		����֧��20��BUTTON��ʹ��ǰ����MmiUtil_SetTouchButton��������
*				������BUTTON�ص����������һ����ʾ��BUTTONΪ׼
*				��ͨ�����ص�(x,y)���������һ��BUTTON��ʾ��λ��
* ------------------------------------------------------------------------ */
void MmiUtil_ShowTouchButton(char *Str, int *x, int *y,int RetVal);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_CheckTouchButton
* DESCRIPTION:   ����Ƿ���BUTTON������
* PARAMETERS:
*		RetVal (out)     - �����µ�BUTTON�趨�ķ���ֵ
*
* RETURN:        0 - �ް�ť������
*				 1 - �а�ť������
*
* NOTES:         ����������������
*				 ������BUTTON�ص����������һ����ʾ��BUTTONΪ׼
*				 ���û����MmiUtil_SetTouchButton�����޷���⵽BUTTON
* ------------------------------------------------------------------------ */
int MmiUtil_CheckTouchButton(int *RetVal);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetPrinterType.
* DESCRIPTION:   ��ȡ��ӡ������
* PARAMETERS:
* RETURN:
*   PrinterType -- ��ӡ�����ͣ��ο�enumPrinterType��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetPrinterType(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayRightNoClear
* DESCRIPTION:   ��ʾ��������ָ����(�Ҷ���),������
* PARAMETERS:
*		msg       (in)	-- Ҫ��ʾ��������λ��
*		disp_line (in)	-- ��ʾ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayRightNoClear(char *msg, int disp_line);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayCenter_ICBC
* DESCRIPTION:   ������ʾ������ָ����(���а棬��Ļ��һ����ʾͼƬ)
* PARAMETERS:
*   msg       (in) -- Ҫ��ʾ��������λ��
*   disp_line (in) -- ��ʾ������
*   invert    (in) -- �Ƿ���TRUE/FALSE
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayCenter_ICBC(char *msg, int disp_line, boolean invert);

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetKeypadState.
* DESCRIPTION:   ����keypad˯��״̬
* PARAMETERS:  
			sleep �C 1 to put the keypad to sleep, 
					 0 to wake up the keypad
* RETURN:
*   zero if successful, nonzero if not.
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_SetKeypadState(int sleep);

int IsTTFSupport(void);

int isGbkFont16(void);

#endif
