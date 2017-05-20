/****************************************************************************
* FILE NAME:   mmiutil.c                                                   *
* MODULE NAME: mmiutil                                                     *
* PROGRAMMER:  luqr                            *
* DESCRIPTION: �˻��ӿ�ģ�飨��ʾ�����룩                                  *
* REVISION:    01.00 08/18/10                                              *
****************************************************************************/

/*==========================================*
*         I N T R O D U C T I O N          *
*==========================================*/

/* void */

/*==========================================*
*             I N C L U D E S              *
*==========================================*/

#include "AppTools.h"

/*==========================================*
*        P R I V A T E  DEFINE             *
*==========================================*/
#define MAX_INPUT_BUF 100
#define DEFAULT_COLOR 0x0000    /*BLACK*/
#define MENU_HEADER_BMP	"F:menu_header.bmp" /* �˵�ͷ */

#define RGBtoRGB565(rgb)       ((unsigned short)(((((unsigned short)((rgb)>>3))&(0x1F)))|((((unsigned short)((rgb)>>10))&(0x3F))<<5)|(((unsigned short)((rgb)>>19))&(0x1F))<<11))
#define MMI_INPUTTEXT_MAX 256	// max input text/number size,Simon


/*==========================================*
*        P R I V A T E  D A T A            *
*==========================================*/
int m_font_size;      //�����С
int font_size, line_space, line_height;
char m_MmiUtil_Tmp[2048];         //ʹ�õ���ʱ����
int screen_height, screen_width;  //��ʾ��Ļ�߶�/��ȣ�in pixels��
typedef int (*tf_init)(char *filename);
typedef int (*tf_displine)(char *str, int x, int y);
typedef int (*tf_fontsize)(unsigned long size);
typedef void (*tf_setcolor)(int rgb);

#ifdef _SHARED_LIB

char  m_MmiUtil_Font[21]; //���ֵ����ֿ⣨16*16��
usint m_MmiUtil_InputTimeout;   //���볬ʱʱ��,��λ��
usint m_MmiUtil_Columns;  //��Ļ��ȣ���λ�ֽ�,���������������ַ����
usint m_MmiUtil_Rows;     //��Ļ�߶ȣ���λ�ֽ�,�������������������ַ��߶�
usint m_MmiUtil_FontColor;      //������ɫ

char  m_ttf_Font[21];   //TTF�ֿ�
int ttf_init;          /*�Ƿ��ѳ�ʼ��ttf*/
int dll_handle;      /*TTF handle*/

#else

static char  m_MmiUtil_Font[21] = {CHNFONT};  //���ֵ����ֿ⣨16*16��
static usint m_MmiUtil_InputTimeout = MAX_KEY_WAIT;   //���볬ʱʱ��,��λ��
static usint m_MmiUtil_Columns = DEFAULT_CHAR_WIDTH;  //��Ļ��ȣ���λ�ֽ�,���������������ַ����
static usint m_MmiUtil_Rows = DEFAULT_CHAR_HEIGHT;      //��Ļ�߶ȣ���λ�ֽ�,�������������������ַ��߶�
static usint m_MmiUtil_FontColor = DEFAULT_COLOR;     //������ɫ

static char  m_ttf_Font[21] = {TTFFONT};
//static char  m_ttf_Font[21] = {"F:/MHeiGb18030.ttf"}; //����TTF����
static int ttf_init = 0;         /*�Ƿ��ѳ�ʼ��ttf*/
static int dll_handle = -1;

#endif

static usint m_MmiUtil_Rows_real=0;	// ���������������ĵ���, ֵ=0��1 // Added by @laikey_20130326

/*=====================================================*
*   I N C L U D E  P R I V A T E  F U N C T I O N S   *
*=====================================================*/
/* ˽�к������� */
static void ins_chr(char *str, char offset, char chr);
static void disp_set_font(int fsize, unsigned short color);
static void disp_line(char *str, int x, int y, int invert);
static void clear_screen(void);
static void clear_lines(int start_line, int end_line);
static sint Vx680_InputText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);
static sint Vx680_InputLongText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode);
static sint MmiUtil_DisplayBuffer( char* szBuffer, sint line_start, sint line_end, int disp_mode );	// simon
int IsTouchScreen(void);

/*=========================================*
*   P U B L I C     F U N C T I O N S     *
*=========================================*/
#ifdef _SHARED_LIB
int main(int argc, char **argv)
{
    // initialize your ATOOL Library initialized static data here

    MmiUtil_InitPrm();
    T9PY_InitPrm();
}
#endif

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Init.
* DESCRIPTION:   ��ʼ��MmiUtilģ�����
* PARAMETERS:
* RETURN:
*   TerminalType -- �ն����ͣ��ο�enumTerminalType��
* NOTES:
* ------------------------------------------------------------------------ */
#ifdef _SHARED_LIB
void MmiUtil_InitPrm(void)
{
    //���ֵ����ֿ⣨16*16��
    memset(m_MmiUtil_Font, 0, sizeof(m_MmiUtil_Font));
    if (isGbkFont16())		//added by baijz 20130515 for GBK
    {
        strcpy(m_MmiUtil_Font, GBKFONT_16);
    }
    else
    {
        strcpy(m_MmiUtil_Font, CHNFONT);
    }

    m_MmiUtil_InputTimeout = MAX_KEY_WAIT;    //���볬ʱʱ��,��λ��
    m_MmiUtil_Columns = DEFAULT_CHAR_WIDTH;   //��Ļ��ȣ���λ�ֽ�,���������������ַ����
    m_MmiUtil_Rows = DEFAULT_CHAR_HEIGHT;     //��Ļ�߶ȣ���λ�ֽ�,�������������������ַ��߶�
    m_MmiUtil_FontColor = DEFAULT_COLOR;    //������ɫ

    //����TTF����
    memset(m_ttf_Font, 0, sizeof(m_ttf_Font));
    strcpy(m_ttf_Font, TTFFONT);
    //strcpy(m_ttf_Font,  "F:/MHeiGb18030.ttf");
    ttf_init = 0;         /*�Ƿ��ѳ�ʼ��ttf*/
    dll_handle = -1;

}
#endif

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetTerminalType.
* DESCRIPTION:   ��ȡ�ն�����
* PARAMETERS:
* RETURN:
*   TerminalType -- �ն����ͣ��ο�enumTerminalType��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetTerminalType(void)
{
    char  buf[64];
    int   TermType = 0;

    memset(buf, 0 , sizeof(buf));
    SVC_INFO_MODELNO(buf);
    //ApplLog_Debug("ATOOL","Model Type[%s]",buf);

    if (!memcmp(buf, "VX670", 5))
        TermType = _VX670;
    else if (!memcmp(buf, "VX610", 5))
        TermType = _VX610;
    else if (!memcmp(buf, "VX810", 5))
        TermType = _VX810;
    else if (!memcmp(buf, "VX700", 5))
        TermType = _VX700;
    else if (!memcmp(buf, "V510G", 5))
        TermType = _VX510G;
    else if (!memcmp(buf, "VX680", 5))
    {
        SVC_INFO_EPROM(buf);
        if (!memcmp(buf+1,"QT6C",4))
            TermType = _VX680C;
        else
            TermType = _VX680;
    }
    else if (!memcmp(buf, "VX520S", 6))
        TermType = _VX520S;
    else if (!memcmp(buf, "VX520", 5))
        TermType = _VX520;
    else if (!memcmp(buf, "VX820", 5))
        TermType = _VX820;
    else if (!memcmp(buf, "VX805", 5))
        TermType = _VX805;
    else if (!memcmp(buf, "VX675", 5))
        TermType = _VX675;
    else if (!memcmp(buf, "VX600", 5))
        TermType = _VX600;
    else
        TermType = _VX510;

    return TermType ;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetSerialNO.
* DESCRIPTION:   ��ȡ�ն����к�
* PARAMETERS:
*   seri_no��out�� -- ���кţ�11 bytes��
* RETURN:
*   0���ɹ��� / -1 ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetSerialNO(char *seri_no)
{
    return SVC_INFO_SERLNO(seri_no);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetBatteryCharge.
* DESCRIPTION:   ��ȡ��ص���
* PARAMETERS:
* RETURN:
*   battery charge -- ��ص������ٷֱ� 0-100��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetBatteryCharge(void)
{
    return get_battery_charge();
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetBatteryStatus.
* DESCRIPTION:   ��ȡ���״̬
* PARAMETERS:
* RETURN:
*   �ɹ� -- NORMAL_LEVEL   (1)
*   ʧ�� -- CRITICAL_LEVEL (0)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetBatteryStatus(void)
{
    return get_battery_status();
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetDockStatus.
* DESCRIPTION:   ��ȡ���״̬
* PARAMETERS:
* RETURN:
*   �ɹ� -- HANDHELD_DOCKED   (0)
*   ʧ�� -- HANDHELD_UNDOCKED (-1)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetDockStatus(void)
{
    return get_dock_status();
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetGroup.
* DESCRIPTION:   �ı�Group ID
* PARAMETERS:
*   group_id (in) -- ��Group ID
* RETURN:
*   �ɹ� -- group id   ( >=1 )
*   ʧ�� -- (-1)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_SetGroup(int group_id)
{
    return set_group(group_id);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetGroup.
* DESCRIPTION:   ��ȡGroup ID
* PARAMETERS:
* RETURN:
*   Group ID
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetGroup(void)
{
    return get_group();
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Restart.
* DESCRIPTION:   �ն�����
* PARAMETERS:
*   filename (in) -- ��ִ���ļ�����""
* RETURN:
*   �ɹ� --
*   ʧ�� -- (<0)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_Restart(const char *filename)
{
    return SVC_RESTART(filename);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFont.
* DESCRIPTION:   ����MmiUtilϵ�����뺯��������
* PARAMETERS:
* szFont (in) -- ����·��������ó���20���ֽ�,Ϊ��������ΪĬ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_SetFont(const char * szFont)
{
    char buf[32];

    if (szFont == NULL || strlen(szFont) == 0)
    {
        set_font("");

        memset(buf, 0, sizeof(buf));
        screen_size(buf);         /*Tridentƽ̨�£����и�������仯����Vx�²���*/
        //ApplLog_Debug("ATOOL","screen size [%d][%d]", buf[0], buf[1]);

        m_MmiUtil_Rows = buf[0];
        m_MmiUtil_Columns = buf[1];
    }
    else
    {
        if (IsTTFSupport())
        {
            int i ;
            strcpy(buf, szFont);

            for (i = 0 ; i < strlen(szFont) ; i++)
            {
                if (buf[i] >= 'a' && buf[i] <= 'z')
                    buf[i] -= 0x20;
            }

            if (!memcmp(buf + strlen(buf) - 3, "TTF", 3))
            {
                strcpy(m_ttf_Font, buf);
                return ;
            }
        }

        set_font(szFont);
    }

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetFont.
* DESCRIPTION:   ��ȡMmiUtilϵ�����뺯��������
* PARAMETERS:
* szFont (out) -- ����·��������ó���20���ֽ�,Ϊ��������ΪĬ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_GetFont(char * szFont)
{
    //strcpy(szFont, m_MmiUtil_Font);
    get_font(szFont);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontColor.
* DESCRIPTION:  ����������ɫ����680/820��Ч
* PARAMETERS:
* FontColor (in) -- ������ɫ����3�ֽڷֱ��ʾRED,GREEN,BLUE��
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_SetFontColor(unsigned long RGBFontColor)
{
    m_MmiUtil_FontColor = RGBtoRGB565(RGBFontColor);
    disp_set_font(m_font_size, m_MmiUtil_FontColor);

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Window
* DESCRIPTION:  ������ʾ��Ļ
* PARAMETERS:
*     startX (in) -- ���ϽǺ�����
*     startY (in) -- ���Ͻ�������
*     endX   (in) -- ���½Ǻ�����
*     endY   (in) -- ���½�������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_Window(int startX, int startY, int endX, int endY)
{
    window(startX, startY, endX, endY);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Clr
* DESCRIPTION:  ���ָ����Ļ
* PARAMETERS:
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_Clr(void)
{
    clrscr();
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WriteAt
* DESCRIPTION:  ��Ļ��ʾ
* PARAMETERS:
*   buf(in) -- ��ʾ������
*   len(in) -- ��ʾ�����ݵĳ���
*   x(in)   -- ��ʾ��ʼ����(column)
*   y(in) -- ��ʾ��ʼ����(row)
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_WriteAt(char *buf, int len, int x, int y)
{
    write_at(buf, len, x, y);
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetFontSize.
* DESCRIPTION:   ����MmiUtilϵ�����뺯��������
* PARAMETERS:
*   font_size (in) -- �����С���ο�FONT_SIZE���壩
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_SetFontSize(int font_size)
{

    tf_init fptr;
    displayInfo_t dispInfo;

    if (font_size)
        m_font_size = font_size;

    if (IsTTFSupport())
    {
        if ( !ttf_init )
        {

            //ApplLog_Debug("ATOOL", "start mmi [%d] [%s]", ttf_init, m_ttf_Font);

            dll_handle = load_named_DLL( "F:/TTF.LIB" );
            //ApplLog_Debug("ATOOL", "load TTF.LIB ret=[%d] errno=[%d]", dll_handle, errno);

            if (dll_handle < 0)
                return;

            fptr = (tf_init)DLL_function_address(dll_handle, 1);

            if (fptr == NULL)
                return;

            if (fptr(m_ttf_Font) == 0)
            {
                ttf_init = 1;
            }

        }

        disp_set_font( m_font_size, m_MmiUtil_FontColor);

        return;
    }

    memset(&dispInfo, 0, sizeof(displayInfo_t));
    SVC_INFO_DISPLAY_EXT(&dispInfo);
    //LOG_PRINTF(("dispInfo.width=[%d] dispInfo.height=[%d]", dispInfo.width, dispInfo.height));

    if (font_size)
    {
        //ApplLog_Debug("ATOOL","font file: [%s]", m_MmiUtil_Font);
        set_font(m_MmiUtil_Font);

        m_MmiUtil_Columns = DEFAULT_CHAR_WIDTH;
        if	(MmiUtil_GetTerminalType() == _VX670)
            m_MmiUtil_Rows = 8;
        else
            m_MmiUtil_Rows = DEFAULT_CHAR_HEIGHT;

        if (dispInfo.height == 32)
        {
            m_MmiUtil_Rows = dispInfo.height/16;
        }

    }
    else
    {
        set_font("");

        m_MmiUtil_Columns = 21;
        if	(MmiUtil_GetTerminalType() == _VX670)
            m_MmiUtil_Rows = 16;
        else
            m_MmiUtil_Rows = 8;

        if (dispInfo.height == 32)
            m_MmiUtil_Rows = dispInfo.height/8;

    }

    //LOG_PRINTF(("row=[%d] col=[%d]", m_MmiUtil_Rows, m_MmiUtil_Columns));

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetFontSize.
* DESCRIPTION:   ����MmiUtilϵ�����뺯��������
* PARAMETERS:
* RETURN:
* font_size -- �����С���ο�FONT_SIZE���壩
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetFontSize(void)
{
    return m_font_size;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetColumms.
* DESCRIPTION:   ��ȡ��ʾ��Ļ�������
* PARAMETERS:
* RETURN:
* Columms - ��Ļ������
* NOTES:
* ------------------------------------------------------------------------ */
usint MmiUtil_GetColumms(void)
{
    return m_MmiUtil_Columns;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetRows.
* DESCRIPTION:   ��ȡ��ʾ��Ļ�������
* PARAMETERS:
* RETURN:
* Rows - ��Ļ������
* NOTES:
* ------------------------------------------------------------------------ */
usint MmiUtil_GetRows(void)
{
    return m_MmiUtil_Rows;
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Reset
* DESCRIPTION:  ��ʼ����ʾ��Ļ,������Ļ��ʾ����
* PARAMETERS:
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_Reset(void)
{

    if (IsTTFSupport())
    {
        clear_screen();
    }
    else
    {
        set_font(m_MmiUtil_Font);
        window(1, 1, m_MmiUtil_Columns, m_MmiUtil_Rows);
        clrscr();
    }

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearLines
* DESCRIPTION:  ���ָ���У����У�
* PARAMETERS:
*   start_line (in) -- ��ʼ��
*   end_line   (in) -- ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearLines(int start_line, int end_line)
{
    if (start_line > end_line)
        return;

    if (start_line > m_MmiUtil_Rows)
        start_line = m_MmiUtil_Rows;

    if (end_line > m_MmiUtil_Rows)
        end_line = m_MmiUtil_Rows;

    if (IsTTFSupport())
    {
        clear_lines(start_line, end_line);
    }
    else
    {
        set_font(m_MmiUtil_Font);
        window(1, start_line, m_MmiUtil_Columns, end_line);
        clrscr();
        window(1, 1, m_MmiUtil_Columns, m_MmiUtil_Rows);
    }

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearLine
* DESCRIPTION:  ���ָ���У�1�У�
* PARAMETERS:
*   disp_line (in) -- Ҫ�������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearLine(int disp_line)
{
    MmiUtil_ClearLines(disp_line, disp_line);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_UserClr
* DESCRIPTION:  �����Ļ�·���ʾ���򣨴ӵڼ��п�ʼ��
* PARAMETERS:
*   start_line (in) -- ��ʼ��
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_UserClr(int start_line)
{
    MmiUtil_ClearLines(start_line, m_MmiUtil_Rows );
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispStr.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ��Ӣ��/����. Vx680/820ר��
* PARAMETERS:
*   str (in) -- Ҫ��ʾ��������λ��
*   startX   (in) -- ��ʼ�к�, ��0��ʼ, ���239
*   startY   (in) -- ��ʼ�к�, ��0��ʼ, ���319
*   fontSize (in) -- �����С
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DispStr(char *str, int startX, int startY, int fontSize)
{
    tf_displine fptr4;

    if (dll_handle < 0)
        return;

    if (IsTTFSupport())
    {

        /*��ΪĬ����ɫ*/
        disp_set_font(fontSize, m_MmiUtil_FontColor);

        /*��ʾ*/
        write_pixels(startX , startY, startX + strlen(str)*fontSize / 2, startY + fontSize, 0xFFFF); //����ɫ, fontSize�������оࣿ

        //ft_disp_line(str, startx, starty);
        fptr4 = (tf_displine)DLL_function_address(dll_handle, 4);

        if (fptr4 == NULL)
            return;

        fptr4(str, startX, startY);

        /*���Ĭ����ɫ*/
        disp_set_font(m_font_size, m_MmiUtil_FontColor);
    }
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispStrByColor.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ��Ӣ��/����,������ɫ�ͱ������Զ���. Vx680/820ר��
* PARAMETERS:
*   str    (in) -- Ҫ��ʾ���ַ�
*   startX   (in) -- ��ʼ�к�, ��0��ʼ, ���239
*   startY   (in) -- ��ʼ�к�, ��0��ʼ, ���319
*   fontSize (in) -- �����С
*   RGBFontColor(in) -- ������ɫ��RGB��ʽ����3�ֽڷֱ��ʾRED,GREEN,BLUE��
*   RGBBGColor   (in) -- ������ɫ��RGB��ʽ����3�ֽڷֱ��ʾRED,GREEN,BLUE��
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DispStrByColor(char *str, int startX, int startY, int fontSize, unsigned long RGBFontColor, unsigned long RGBBGColor)
{
    tf_displine fptr4;

    if (dll_handle < 0)
        return;

    if (IsTTFSupport())
    {

        /*��Ϊָ����ɫ*/
        disp_set_font(fontSize, RGBtoRGB565(RGBFontColor));

        /*��ʾ*/
        write_pixels(startX , startY, startX + strlen(str)*fontSize / 2, startY + fontSize, RGBtoRGB565(RGBBGColor)); //����ɫ, fontSize�������оࣿ

        //ft_disp_line(str, startx, starty);
        fptr4 = (tf_displine)DLL_function_address(dll_handle, 4);

        if (fptr4 == NULL)
            return;

        fptr4(str, startX, startY);

        /*���Ĭ����ɫ*/
        disp_set_font(m_font_size, m_MmiUtil_FontColor);
    }
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispChinese.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ������.
* PARAMETERS:
*   str (in) -- Ҫ��ʾ��������λ��
*   x   (in) -- �к�, ��1��ʼ
*   y   (in) -- �к�, ��1��ʼ
* RETURN:
* NOTES:         �����ļ��ɳ�����ǰָ��
* ------------------------------------------------------------------------ */
void MmiUtil_DispChinese(char *str, int x, int y)
{

    long i, j, qu, wei;
    long offset;
    char buf[100 + 1];
    int  fp = 0;
    BOOL bIsGbkFont16;

    j = strlen(str);
    j = j > m_MmiUtil_Columns ? m_MmiUtil_Columns : j;
    memset(buf, 0, sizeof(buf));
    strncpy(buf, str, j);

    if (IsTTFSupport())
    {
        disp_line(buf, x, y, FALSE);
        return;
    }

    bIsGbkFont16 = isGbkFont16();	//added by baijz 20130515 for GBK

    memset(m_MmiUtil_Tmp, 0, sizeof(m_MmiUtil_Tmp));

    fp = open(m_MmiUtil_Font, O_RDONLY);

    for (i = 0; i < j;)
    {
        if (bIsGbkFont16)	//added by baijz 20130515 for GBK
        {
            if (buf[i] < 0x81)
            {
                //�����ַ�
                offset = buf[i];
                lseek(fp, (offset << 4) + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���
                read(fp, m_MmiUtil_Tmp + (i << 4), 16);
                i ++;
                continue;
            }
            qu = buf[i] - 0x81;
            wei = buf[i+1] - 0x40;
        }
        else
        {
            if (buf[i] < 0xA1)
            {
                //�����ַ�
                offset = buf[i];
                lseek(fp, (offset << 4) + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���
                read(fp, m_MmiUtil_Tmp + (i << 4), 16);
                i ++;
                continue;
            }
            qu = buf[i] - 0xA1;
            wei = buf[i+1] - 0xA1;
        }

        if (qu < 0 || wei < 0)
        {
            //����
            i += 2;
            continue;
        }

        //������λ����㺺��ƫ��
        //һ��������0xFF-0XA1=94������
        //ÿ������ռ��2���ֽ�
        //�ֿ�ǰ128���ֽ�Ϊ�����ַ�ռ��1���ֽ�
        if (bIsGbkFont16)	//added by baijz 20130515 for GBK
        {
            offset = (((qu << 7) + (qu << 6) - qu + wei) << 1) + 128;
        }
        else
        {
            offset = (((qu << 6) + (qu << 5) - (qu << 1) + wei) << 1) + 128;//�˷���λ��Ϊ��������ٶ�
        }
        lseek(fp, (offset << 4) + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���,ƫ�ư�������
        read(fp, m_MmiUtil_Tmp + (i << 4), 32);//һ�������ַ�32���ֽڵ���
        i += 2;
    }

    close(fp);

    gotoxy(x, y);
    putpixelcol(m_MmiUtil_Tmp, j << 4);

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearEngLines
* DESCRIPTION:  ���ָ���У����У�Ӣ��С���壩
* PARAMETERS:
*   start_line (in) -- ��ʼ��
*   end_line   (in) -- ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearEngLines(int start_line, int end_line)
{
    if (start_line > end_line)
        return;

    if (IsTTFSupport())
    {
        /*��ΪӢ��С����*/
        disp_set_font(20, m_MmiUtil_FontColor);

        if (start_line > m_MmiUtil_Rows)
            start_line = m_MmiUtil_Rows;

        if (end_line > m_MmiUtil_Rows)
            end_line = m_MmiUtil_Rows;

        clear_lines(start_line, end_line);

        /*��غ�������*/
        disp_set_font(m_font_size, m_MmiUtil_FontColor);
    }
    else
    {
        /*��ΪӢ��С����*/
        MmiUtil_SetFontSize(0);

        if (start_line > m_MmiUtil_Rows)
            start_line = m_MmiUtil_Rows;

        if (end_line > m_MmiUtil_Rows)
            end_line = m_MmiUtil_Rows;

        window(1, start_line, m_MmiUtil_Columns, end_line);
        clrscr();
        window(1, 1, m_MmiUtil_Columns, m_MmiUtil_Rows);

        /*��غ�������*/
        MmiUtil_SetFontSize(m_font_size);

    }

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ClearEngLines
* DESCRIPTION:  ���ָ���У�1�У�Ӣ��С���壩
* PARAMETERS:
*   disp_line (in) -- Ҫ�������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_ClearEngLine(int disp_line)
{
    MmiUtil_ClearEngLines(disp_line, disp_line);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispEnglish.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ��ʾһ��Ӣ��.
* PARAMETERS:
*   str (in) -- Ҫ��ʾ��Ӣ��
*   x   (in) -- �к�, ��1��ʼ
*   y   (in) -- �к�, ��1��ʼ
* RETURN:
* NOTES:         Ӣ������ʹ��ϵͳĬ�����壨Vx520��6X8��Vx680��9X12��
* ------------------------------------------------------------------------ */
void MmiUtil_DispEnglish(char *str, int x, int y)
{
    long j;
    char buf[100 + 1];

    if (IsTTFSupport())
    {

        /*��ΪӢ��С����*/
        disp_set_font(20, m_MmiUtil_FontColor);

        j = strlen(str);
        j = j > m_MmiUtil_Columns ? m_MmiUtil_Columns : j;
        memset(buf, 0, sizeof(buf));
        strncpy(buf, str, j);

        /*��ʾ*/
        disp_line(buf, x, y, FALSE);

        /*��غ�������*/
        disp_set_font(m_font_size, m_MmiUtil_FontColor);
    }

    else
    {
        /*��ΪӢ��С����*/
        MmiUtil_SetFontSize(0);

        j = strlen(str);
        j = j > m_MmiUtil_Columns ? m_MmiUtil_Columns : j;
        memset(buf, 0, sizeof(buf));
        strncpy(buf, str, j);

        window(1, 1, m_MmiUtil_Columns, m_MmiUtil_Rows);
        display_at( x, y, buf, 0);

        /*��غ�������*/
        MmiUtil_SetFontSize(m_font_size);
    }

}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispChineseInvert.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ����ʾһ������.
* PARAMETERS:
*   str (in) -- Ҫ��ʾ��������λ��
*   x   (in) -- �к�, ��1��ʼ
*   y   (in) -- �к�, ��1��ʼ
* RETURN:        none.
* NOTES:         none.�����ļ��ɳ�����ǰָ��
* ------------------------------------------------------------------------ */
void MmiUtil_DispChineseInvert(char *str, int x, int y)
{

    long i, j, k, qu, wei;
    long offset;
    char buf[100 + 1];
    int  fp = 0;
    BOOL bIsGbkFont16;

    if (IsTTFSupport())
    {
        disp_line(str, x, y, TRUE);
        return;
    }

    bIsGbkFont16 = isGbkFont16();	//added by baijz 20130515 for GBK

    j = strlen(str);
    j = j > m_MmiUtil_Columns ? m_MmiUtil_Columns : j;
    memset(buf, 0, sizeof(buf));
    strncpy(buf, str, j);

    memset(m_MmiUtil_Tmp, 0, sizeof(m_MmiUtil_Tmp));

    fp = open(m_MmiUtil_Font, O_RDONLY);

    for (i = 0; i < j;)
    {
        if (bIsGbkFont16)	//added by baijz 20130515 for GBK
        {
            if (buf[i] < 0x81)
            {
                //�����ַ�
                offset = buf[i];
                lseek(fp, (offset << 4) + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���
                read(fp, m_MmiUtil_Tmp + (i << 4), 16);
                i ++;
                continue;
            }
            qu = buf[i] - 0x81;
            wei = buf[i+1] - 0x40;
        }
        else
        {
            if (buf[i] < 0xA1)
            {
                //�����ַ�
                offset = buf[i];
                lseek(fp, (offset << 4) + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���
                read(fp, m_MmiUtil_Tmp + (i << 4), 16);
                i ++;
                continue;
            }
            //�����ַ�
            qu = buf[i] - 0xA1;
            wei = buf[i+1] - 0xA1;
        }

        if (qu < 0 || wei < 0)
        {
            //����
            i += 2;
            continue;
        }

        //������λ����㺺��ƫ��
        //һ��������0xFF-0XA1=94������
        //ÿ������ռ��2���ֽ�
        //�ֿ�ǰ128���ֽ�Ϊ�����ַ�ռ��1���ֽ�
        if (bIsGbkFont16)	//added by baijz 20130515 for GBK
        {
            offset = (((qu << 7) + (qu << 6) - qu + wei) << 1) + 128;
        }
        else
        {
            offset = (((qu << 6) + (qu << 5) - (qu << 1) + wei) << 1) + 128;
        }
        lseek(fp, (offset << 4) + 16, SEEK_SET);//һ�������ַ�16���ֽڵ���,ƫ�ư�������
        read(fp, m_MmiUtil_Tmp + (i << 4), 32);//һ�������ַ�32���ֽڵ���
        i += 2;
    }

    close(fp);

    for (k = 0, i = (j << 4); k < i; k++)
    {
        m_MmiUtil_Tmp[k] = ~m_MmiUtil_Tmp[k];
    }

    gotoxy(x, y);
    putpixelcol(m_MmiUtil_Tmp, j << 4);


}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispChineseInvert_ICBC.
* DESCRIPTION:   ��������ʼλ�ÿ�ʼ����ʾһ������.(���а棬��Ļ��һ����ʾͼƬ)
* PARAMETERS:
*		str (in) -- Ҫ��ʾ��������λ��
*		x   (in) -- �к�, ��1��ʼ
*		y   (in) -- �к�, ��1��ʼ
* RETURN:        none.
* NOTES:         none.�����ļ��ɳ�����ǰָ��
* ------------------------------------------------------------------------ */
void MmiUtil_DispChineseInvert_ICBC(char *str, int x, int y)
{

    long i, j, k, qu, wei;
    long offset;
    char buf[100 + 1];
    int  fp=0;
    BOOL bIsGbkFont16;

    if (IsTTFSupport())
    {
        if (y == LINE1)
            MmiUtil_DispBMP(MENU_HEADER_BMP, 0, 0, 240, 32);

        disp_line(str, x, y, TRUE);
        return;
    }

    bIsGbkFont16 = isGbkFont16();	//added by baijz 20130515 for GBK

    j = strlen(str);
    j = j>m_MmiUtil_Columns?m_MmiUtil_Columns:j;
    memset(buf, 0, sizeof(buf));
    strncpy(buf, str, j);

    memset(m_MmiUtil_Tmp, 0, sizeof(m_MmiUtil_Tmp));

    fp = open(m_MmiUtil_Font, O_RDONLY);
    for (i = 0; i < j; )
    {
        if (bIsGbkFont16)	//added by baijz 20130515 for GBK
        {
            if (buf[i] < 0x81)
            {
                //�����ַ�
                offset = buf[i];
                lseek(fp, (offset << 4)+16, SEEK_SET);//һ�������ַ�16���ֽڵ���
                read(fp, m_MmiUtil_Tmp + (i <<4), 16);
                i ++;
                continue;
            }
            qu = buf[i] - 0x81;
            wei = buf[i+1] - 0x40;
        }
        else
        {
            if (buf[i] < 0xA1)
            {
                //�����ַ�
                offset = buf[i];
                lseek(fp, (offset << 4)+16, SEEK_SET);//һ�������ַ�16���ֽڵ���
                read(fp, m_MmiUtil_Tmp + (i <<4), 16);
                i ++;
                continue;
            }
            qu = buf[i] - 0xA1;
            wei = buf[i+1] - 0xA1;
        }

        if (qu < 0 || wei < 0)
        {
            //����
            i += 2;
            continue;
        }
        //������λ����㺺��ƫ��
        //һ��������0xFF-0XA1=94������
        //ÿ������ռ��2���ֽ�
        //�ֿ�ǰ128���ֽ�Ϊ�����ַ�ռ��1���ֽ�
        if (bIsGbkFont16)	//added by baijz 20130515 for GBK
        {
            offset = (((qu << 7) + (qu << 6) - qu + wei) << 1) + 128;
        }
        else
        {
            offset = (((qu<<6)+(qu<<5)-(qu<<1)+wei)<<1)+128;
        }
        lseek(fp, (offset << 4)+16, SEEK_SET);//һ�������ַ�16���ֽڵ���,ƫ�ư�������
        read(fp, m_MmiUtil_Tmp + (i << 4), 32);//һ�������ַ�32���ֽڵ���
        i += 2;
    }
    close(fp);

    for (k = 0, i = (j << 4); k < i; k++)
    {
        m_MmiUtil_Tmp[k] = ~m_MmiUtil_Tmp[k];
    }

    gotoxy(x, y);
    putpixelcol(m_MmiUtil_Tmp, j<<4);


}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayLine
* DESCRIPTION:   ��ʾ��������ָ����(�����)
* PARAMETERS:
*   msg       (in)  -- Ҫ��ʾ��������λ��
*   disp_line (in)  -- ��ʾ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayLine(char *msg, int disp_line)
{
    MmiUtil_ClearLine(disp_line);
    MmiUtil_DispChinese(msg, 1, disp_line);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayRight
* DESCRIPTION:   ��ʾ��������ָ����(�Ҷ���),������
* PARAMETERS:
*   msg       (in)  -- Ҫ��ʾ��������λ��
*   disp_line (in)  -- ��ʾ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayRight(char *msg, int disp_line)
{
    int  len = 0;

    len = strlen(msg);
    len = len > m_MmiUtil_Columns ? m_MmiUtil_Columns : len;

    //ApplLog_Debug("ATOOL","max_col=[%d] len=[%d]", m_MmiUtil_Columns, len);

    MmiUtil_ClearLine(disp_line);
    MmiUtil_DispChinese(msg, m_MmiUtil_Columns - len + 1, disp_line);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayRightNoClear
* DESCRIPTION:   ��ʾ��������ָ����(�Ҷ���),������
* PARAMETERS:
*		msg       (in)	-- Ҫ��ʾ��������λ��
*		disp_line (in)	-- ��ʾ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayRightNoClear(char *msg, int disp_line)
{
    int  len = 0;

    len = strlen(msg);
    len = len>m_MmiUtil_Columns?m_MmiUtil_Columns:len;

    //ApplLog_Debug("ATOOL","max_col=[%d] len=[%d]", m_MmiUtil_Columns, len);

    //MmiUtil_ClearLine(disp_line);
    MmiUtil_DispChinese(msg, m_MmiUtil_Columns - len + 1, disp_line);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayLeftRight
* DESCRIPTION:   ��ʾ2����������ָ����(�������Ҷ���)
* PARAMETERS:
*   Leftmsg   (in)  -- Ҫ�������ʾ��������λ��
*   Rightmsg  (in)  -- Ҫ���ұ���ʾ��������λ��
*   disp_line (in)  -- ��ʾ������
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayLeftRight(char *LeftMsg, char *RightMsg, int disp_line)
{
    int  len = 0;

    len = strlen(RightMsg);
    len = len > m_MmiUtil_Columns ? m_MmiUtil_Columns : len;

    MmiUtil_ClearLine(disp_line);
    MmiUtil_DispChinese(LeftMsg, 1, disp_line);
    MmiUtil_DispChinese(RightMsg, m_MmiUtil_Columns - len + 1, disp_line);
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DisplayCenter
* DESCRIPTION:   ������ʾ������ָ����
* PARAMETERS:
*   msg       (in) -- Ҫ��ʾ��������λ��
*   disp_line (in) -- ��ʾ������
*   invert    (in) -- �Ƿ���TRUE/FALSE
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void MmiUtil_DisplayCenter(char *msg, int disp_line, boolean invert)
{
    char tmp[100+2];
    int len = 0;
    int left = 0;

    len = strlen(msg);
    len = len > m_MmiUtil_Columns ? m_MmiUtil_Columns : len;
    left = (m_MmiUtil_Columns - len) / 2;

    //ApplLog_Debug("ATOOL","max_col=[%d] len=[%d] left=[%d]", m_MmiUtil_Columns, len, left);

    memset(tmp, ' ',  left);
    memcpy(tmp + left, msg, len);
    memset(tmp + left + len, ' ', m_MmiUtil_Columns - len - left);
    tmp[m_MmiUtil_Columns] = 0;

    MmiUtil_ClearLine(disp_line);

    if (invert)
        MmiUtil_DispChineseInvert((char *)tmp, 1, disp_line);
    else
        MmiUtil_DispChinese((char *)tmp, 1, disp_line);

}

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
void MmiUtil_DisplayCenter_ICBC(char *msg, int disp_line, boolean invert)
{
    char tmp[100+2];
    int len = 0;
    int left = 0;

    len = strlen(msg);
    len = len > m_MmiUtil_Columns ? m_MmiUtil_Columns : len;
    left = (m_MmiUtil_Columns - len) / 2;

    //ApplLog_Debug("ATOOL","max_col=[%d] len=[%d] left=[%d]", m_MmiUtil_Columns, len, left);

    memset(tmp, ' ',  left);
    memcpy(tmp + left, msg, len);
    memset(tmp + left + len, ' ', m_MmiUtil_Columns - len - left);
    tmp[m_MmiUtil_Columns] = 0;

    MmiUtil_ClearLine(disp_line);

    if (invert)
        MmiUtil_DispChineseInvert_ICBC((char *)tmp, 1, disp_line);
    else
        MmiUtil_DispChinese((char *)tmp, 1, disp_line);

}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetTimeOut.
* DESCRIPTION:   ����MmiUtilϵ�����뺯���ĳ�ʱʱ��
* PARAMETERS:
*   usTimeout (in) -- ��ʱʱ��,��λ��,Ϊ0������ʽ�ĵȴ�����
* RETURN:
* NOTES:         ��ʱʱ��Ĭ��30��
* ------------------------------------------------------------------------ */
void MmiUtil_SetTimeOut(usint usTimeout)
{
    m_MmiUtil_InputTimeout = usTimeout;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetTimeout.
* DESCRIPTION:   ��ȡMmiUtilϵ�����뺯���ĳ�ʱʱ��
* PARAMETERS:
* RETURN:
* usTimeout -- ��ʱʱ��,��λ��,Ϊ0������ʽ�ĵȴ�����
* NOTES:  ��ʱʱ��Ĭ��30��
* ------------------------------------------------------------------------ */
usint MmiUtil_GetTimeOut(void)
{
    return m_MmiUtil_InputTimeout;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_CheckKey.
* DESCRIPTION:   ����Ƿ��а���
* PARAMETERS:
* RETURN:        ���û�а�������0�����򷵻ذ����ļ�ֵ
* NOTES:
* ------------------------------------------------------------------------ */
byte MmiUtil_CheckKey(void)
{
    byte key = 0;

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return key;

    if (KBHIT())
    {
        key = (byte)get_char();
    }

    return key;
}

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
byte MmiUtil_WaitKeyMenu(usint Timeout, usint Count, usint Head, usint OneMenu, byte Base)
{
    long end = 0;
    byte key = 0;
    int  starty = 0;
    int  x = 0, y = 0, line = 0, col = 0;

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return key;

    if (Timeout == 0)
    {
        Timeout = 100;
    }

    if (Head)
        starty = 64;
    else
        starty = 32;

    end = set_itimeout(-1, Timeout, TM_SECONDS);
    while (CHK_TIMEOUT(-1, end))
    {
        if (KBHIT())
        {
            key = (byte)get_char();
            break;
        }
        if (get_touchscreen(&x, &y))
        {
            // char bb[30];

            if (OneMenu) //ÿ��1��˵�
            {
                if (y > starty && (y < (32*Count + starty)))
                {
                    line = (y - starty) / 32; //������Ļͷ�Ͳ˵�ͷ
                    key = line + bKEY1;
                    if (Base == '0')
                        key -= 1;
                    //sprintf(bb, "x=[%d]y=[%d][%d]", x, y, key);
                    //MmiUtil_DisplayLine((char*)bb, LINE6);
                    break;
                }
            }
            else //ÿ��2��˵�
            {
                if (y > starty && (y < (32*(Count + 1) / 2 + starty)))
                {
                    line = (y - starty) / 32; //������Ļͷ�Ͳ˵�ͷ
                    col = x / 120; //0��߲˵���1�ұ߲˵�
                    key = line * 2 + col + bKEY1;
                    if (Base == '0')
                        key -= 1;
                    //sprintf(bb, "x=[%d]y=[%d][%d]", x, y, key);
                    //MmiUtil_DisplayLine((char*)bb, LINE6);
                    break;
                }
            }
        }
    }

    return key;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyButton
* DESCRIPTION:   �ȴ���ʱ���룩�򰴼�������680��������Ŧ
* PARAMETERS:
*		Timeout (in)     - ��ʱ���룩��Ϊ0ʱ�޳�ʱ
* RETURN:        ��ʱ����0�����򷵻ذ���ֵ
* NOTES:         ����Ϊ��λ.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyButton(usint Timeout)
{
    long end = 0;
    byte key = 0;
    int  x = 0, y = 0;

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return key;

    if (Timeout == 0)
    {
        Timeout = 100;
    }

    end = set_itimeout(-1, Timeout, TM_SECONDS);
    while (CHK_TIMEOUT(-1, end))
    {
        if (KBHIT())
        {
            key = (byte)get_char();
            break;
        }
        if (get_touchscreen(&x, &y))
        {
            /*
                        char bb[30];
                        sprintf(bb, "x=[%d]y=%d", x, y);
                        MmiUtil_DisplayLine((char*)bb, LINE6);
                        */
            if (x > 0 && x < 46 && y > 290 && y < 320)
            {
                key = bKEYA;
                break;
            }
            else if (x > 48 && x < 94 && y > 290 && y < 320)
            {
                key = bKEYB;
                break;
            }
            else if (x > 96 && x < 142 && y > 290 && y < 320)
            {
                key = bALPHAKEY;
                break;
            }
            else if (x > 144 && x < 190 && y > 290 && y < 320)
            {
                key = bKEYC;
                break;
            }
            else if (x > 192 && x < 240 && y > 290 && y < 320)
            {
                key = bKEYD;
                break;
            }
        }
    }

    return key;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKey
* DESCRIPTION:   �ȴ���ʱ���룩�򰴼�
* PARAMETERS:
*   Timeout (in) - ��ʱ���룩��Ϊ0ʱ�޳�ʱ
* RETURN:        ��ʱ����0�����򷵻ذ���ֵ
* NOTES:         ����Ϊ��λ.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKey(usint Timeout)
{
    long end = 0;
    byte key = 0;

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return key;

    if (Timeout == 0)
    {
        key = (byte)get_char();
        return key;
    }

    end = set_itimeout(-1, Timeout, TM_SECONDS);

    while (CHK_TIMEOUT(-1, end))
    {
        if (KBHIT())
        {
            key = (byte)get_char();
            break;
        }
    }

    return key;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_WaitKeyMill
* DESCRIPTION:   �ȴ���ʱ�����룩�򰴼�
* PARAMETERS:
*   Timeout (in) - ��ʱ�����룩��Ϊ0ʱ�޳�ʱ
* RETURN:        ��ʱ����0�����򷵻ذ���ֵ
* NOTES:         �Ժ���Ϊ��λ.
* ------------------------------------------------------------------------ */
byte MmiUtil_WaitKeyMill(ulint Timeout)
{
    long end = 0;
    byte key = 0;

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return key;

    if (Timeout == 0)
    {
        key = (byte)get_char();
        return key;
    }

    end = set_itimeout(-1, Timeout, TM_TICKS);

    while (CHK_TIMEOUT(-1, end))
    {
        if (KBHIT())
        {
            key = (byte)get_char();
            break;
        }
    }

    return key;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Beep
* DESCRIPTION:   ����300ms�ȴ�200ms
* PARAMETERS:
* RETURN:
* ------------------------------------------------------------------------ */
void MmiUtil_Beep(void)
{
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    SVC_WAIT(200);
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
    normal_tone();//Produces a 50-ms tone at 1245 Hz
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ErrorSound
* DESCRIPTION:   ����300ms�ȴ�200ms������300ms
* PARAMETERS:
* RETURN:
* ------------------------------------------------------------------------ */
void MmiUtil_ErrorSound(void)
{
    error_tone();//Produces a 100-ms tone at 880 Hz
    error_tone();//Produces a 100-ms tone at 880 Hz
    error_tone();//Produces a 100-ms tone at 880 Hz
    SVC_WAIT(200);
    error_tone();//Produces a 100-ms tone at 880 Hz
    error_tone();//Produces a 100-ms tone at 880 Hz
    error_tone();//Produces a 100-ms tone at 880 Hz
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning.
* DESCRIPTION:   ��ʾ������Ϣ���ȴ���������ʱ�˳���
* PARAMETERS:
*   text (in) -- ������Ϣ
* RETURN:
*   0     -- ��ʱ
*   key_value -- ����ֵ
* NOTES:
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning(char *text)
{
    int  ilen = 0;
    char tmp[100+1];
    int  i = 0;
    int  isize = 0;

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return 0;

    ilen = strlen(text);

    if (ilen <= m_MmiUtil_Columns)
    {
        MmiUtil_UserClr(LINE2);
        MmiUtil_DisplayCenter(text, LINE2, FALSE);
        MmiUtil_Beep();

        return (sint)MmiUtil_WaitKey(m_MmiUtil_InputTimeout);
    }

    MmiUtil_UserClr(LINE2);

    for (i = 0; isize < ilen; i++)
    {
        memset(tmp, 0, sizeof(tmp));
        isize += BankUtil_CStrlcpy(tmp, text + isize, m_MmiUtil_Columns + 1);
        MmiUtil_DisplayCenter(tmp, LINE2 + i, FALSE);
    }

    MmiUtil_Beep();

    return (sint)MmiUtil_WaitKey(m_MmiUtil_InputTimeout);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_Warning2.
* DESCRIPTION:   ��ʾ������Ϣ���ȴ��������޳�ʱ��
* PARAMETERS:
*   text (in) -- ������Ϣ
* RETURN:
*   key_value -- ����ֵ
* ------------------------------------------------------------------------ */
sint MmiUtil_Warning2(char *text)
{
    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return 0;

    MmiUtil_UserClr(LINE2);
    MmiUtil_DisplayLine(text, LINE2);
    MmiUtil_ErrorSound();

    return (sint)MmiUtil_WaitKey(0);
}

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
sint MmiUtil_Warning3(char *text, usint iTiemout )
{
    int  ilen = 0;
    char tmp[100+1];
    int  i = 0;
    int  isize = 0;

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return 0;

    ilen = strlen(text);

    if (ilen <= m_MmiUtil_Columns)
    {
        MmiUtil_UserClr(LINE2);
        MmiUtil_DisplayCenter(text, LINE2, FALSE);
        MmiUtil_Beep();

        return (sint)MmiUtil_WaitKey(iTiemout);
    }

    MmiUtil_UserClr(LINE2);

    for (i = 0; isize < ilen; i++)
    {
        memset(tmp, 0, sizeof(tmp));
        isize += BankUtil_CStrlcpy(tmp, text + isize, m_MmiUtil_Columns + 1);
        MmiUtil_DisplayCenter(tmp, LINE2 + i, FALSE);
    }

    MmiUtil_Beep();

    return (sint)MmiUtil_WaitKey(iTiemout);
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputText
* DESCRIPTION:   �����ַ���(��ĸ������)
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   msgPrompt (in)  -- ��ʾ��Ϣ
*   str       (out) -- ����ַ���
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode)
{

    char szKeyMask[MAX_ALPNUM_KEYS][9] =
    {
        "0- +%_", "1QZqz.\\", "2ABCabc&", "3DEFdef%",
        "4GHIghi*", "5JKLjkl/", "6MNOmno~", "7PRSprs^",
        "8TUVtuv[", "9WXYwxy]", "*,'\":", "#=:$?"
    };

    char  ch;
    int   idx = 0;
    byte  key = 0;
    int   offset = 0;
    char  szBuffer[101] = {0};
    int   keyCounter = 0;
    int   keyoffset = 0;
    int   input_line, total_lines, i;

    if (IsTouchScreen())
    {
        return Vx680_InputText(disp_line, msgPrompt, str, min, max, disp_mode);
    }

    if ((max > 100) || (min > max))
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    sprintf(szBuffer, "%s", str);

    while (1)
    {
        idx = strlen(szBuffer);
        idx = idx > max ? max : idx;
        szBuffer[idx] = 0;

        if (idx == 0 || idx % m_MmiUtil_Columns)
            total_lines = idx / m_MmiUtil_Columns + 1;
        else
            total_lines = idx / m_MmiUtil_Columns;

        for (i = 1; i < total_lines; i++)
        {
            offset = m_MmiUtil_Columns * (i - 1);
            input_line = disp_line + (i - 1);
            MmiUtil_ClearLine(input_line);
            MmiUtil_DisplayLine(szBuffer + offset, input_line);
        }

        offset = m_MmiUtil_Columns * (total_lines - 1);
        input_line = disp_line + total_lines - 1;
        MmiUtil_ClearLine(input_line);

        switch (disp_mode)
        {
        case LEFT_JST:
            MmiUtil_DisplayLine(szBuffer + offset, input_line);
            break;
        case RIGHT_JST:
            MmiUtil_DisplayRight(szBuffer + offset, input_line);
            break;
        case CENTER_JST:
        default:
            MmiUtil_DisplayCenter(szBuffer + offset, input_line, FALSE);
            break;
        }

        key = MmiUtil_WaitKey(m_MmiUtil_InputTimeout);

        switch (key)
        {
        case 0:
            return INPUT_TIMEOUT;

        case bESC:
            return INPUT_CANCEL;

        case bENTER:

            if (idx < min)
            {
                continue;
            }

            strcpy(str, szBuffer);
            return idx;
        case bCLR:

            if (idx > 0)
            {
                if (idx > m_MmiUtil_Columns)
                {
                    MmiUtil_ClearLines(disp_line, input_line);
                }
                else
                {
                    MmiUtil_ClearLine(input_line);
                }

                idx = 0;
                memset(szBuffer, 0, sizeof(szBuffer));
            }

            break;

        case bBKSP:

            if (idx)
            {
                szBuffer[--idx] = 0;

                if (idx % m_MmiUtil_Columns == 0)
                {
                    MmiUtil_ClearLine(input_line);
                }
            }
            else
                szBuffer[idx] = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2:
        case bKEY3:
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:
        case bSTR:
        case bPND:

            if (idx < max)
            {
                szBuffer[idx++] = key;
                keyCounter = 0;
            }

            break;
        case bALPHAKEY:

            if (keyCounter == 0)
            {
                ch = szBuffer[idx-1];

                if (ch >= bKEY0 && ch <= bKEY9)
                    keyoffset = ch - '0';
                else if (ch == bSTR)
                    keyoffset = 10;
                else
                    keyoffset = 11;
            }

            //�л���ǰ����ֵ
            keyCounter += 1;
            keyCounter = keyCounter % strlen(szKeyMask[keyoffset]);
            szBuffer[idx-1] = szKeyMask[keyoffset] \
                              [(keyCounter % strlen(szKeyMask[keyoffset]))];
            break;
        }//end switch
    }//end while


}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputLongText
* DESCRIPTION:   �����ַ���(��ĸ������)����󳤶�#define MMI_INPUTTEXT_MAX 256
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   msgPrompt (in)  -- ��ʾ��Ϣ
*   str       (out) -- ����ַ���
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputLongText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode)
{

    char szKeyMask[MAX_ALPNUM_KEYS][9] =
    {
        "0- +%_", "1QZqz.\\", "2ABCabc&", "3DEFdef%",
        "4GHIghi*", "5JKLjkl/", "6MNOmno~", "7PRSprs^",
        "8TUVtuv[", "9WXYwxy]", "*,'\":", "#=:$?"
    };

    char  ch;
    int   idx = 0;
    byte  key = 0;
    char  szBuffer[MMI_INPUTTEXT_MAX+4] = {0};
    int   keyCounter = 0;
    int   keyoffset = 0;
    int   input_line;

    if (IsTouchScreen())
    {
        return Vx680_InputLongText(disp_line, msgPrompt, str, min, max, disp_mode);
    }

    if ((max > MMI_INPUTTEXT_MAX) || (min > max) || (strlen(str) > max) )
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    sprintf(szBuffer, "%s", str);

    while (1)
    {
        idx = strlen(szBuffer);
        idx = idx > max ? max : idx;
        szBuffer[idx] = 0;
        // ��ʾbuffer�������ص�ǰ���һ�е�λ��
        input_line = MmiUtil_DisplayBuffer(szBuffer,disp_line,0, disp_mode);

        key = MmiUtil_WaitKey(m_MmiUtil_InputTimeout);

        switch (key)
        {
        case 0:
            return INPUT_TIMEOUT;

        case bESC:
            return INPUT_CANCEL;

        case bENTER:

            if (idx < min)
            {
                continue;
            }

            strcpy(str, szBuffer);
            return idx;
        case bCLR:

            if (idx > 0)
            {
                //if (idx > m_MmiUtil_Columns)
                if (input_line > disp_line)
                {
                    MmiUtil_ClearLines(disp_line, input_line);
                }
                else
                {
                    MmiUtil_ClearLine(input_line);
                }

                idx = 0;
                memset(szBuffer, 0, sizeof(szBuffer));
            }

            break;

        case bBKSP:

            if (idx)
            {
                szBuffer[--idx] = 0;

                if (idx % m_MmiUtil_Columns == 0)
                {
                    MmiUtil_ClearLine(input_line);
                }
            }
            else
                szBuffer[idx] = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2:
        case bKEY3:
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:
        case bSTR:
        case bPND:

            if (idx < max)
            {
                szBuffer[idx++] = key;
                keyCounter = 0;
            }

            break;
        case bALPHAKEY:

            if (keyCounter == 0)
            {
                ch = szBuffer[idx-1];

                if (ch >= bKEY0 && ch <= bKEY9)
                    keyoffset = ch - '0';
                else if (ch == bSTR)
                    keyoffset = 10;
                else
                    keyoffset = 11;
            }

            //�л���ǰ����ֵ
            keyCounter += 1;
            keyCounter = keyCounter % strlen(szKeyMask[keyoffset]);
            szBuffer[idx-1] = szKeyMask[keyoffset] \
                              [(keyCounter % strlen(szKeyMask[keyoffset]))];
            break;
        }//end switch
    }//end while


}
/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputNum
* DESCRIPTION:   ���������ַ���
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   msgPrompt (in)  -- ��ʾ��Ϣ
*   str       (out) -- ������ִ�
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputNum(int disp_line, char *msgPrompt, char *str, usint min, usint max, int disp_mode)
{
    int idx = 0;
    byte key = 0;
    int offset = 0;
    char szBuffer[101] = {0};
    int input_line, total_lines, i;

    if ((max > 100) || (min > max))
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    sprintf(szBuffer, "%s", str);

    for (;;)
    {
        idx = strlen(szBuffer);
        idx = idx > max ? max : idx;
        szBuffer[idx] = 0;

        if (idx == 0 || idx % m_MmiUtil_Columns)
            total_lines = idx / m_MmiUtil_Columns + 1;
        else
            total_lines = idx / m_MmiUtil_Columns;

        for (i = 1; i < total_lines; i++)
        {
            offset = m_MmiUtil_Columns * (i - 1);
            input_line = disp_line + (i - 1);
            MmiUtil_ClearLine(input_line);
            MmiUtil_DisplayLine(szBuffer + offset, input_line);
        }

        offset = m_MmiUtil_Columns * (total_lines - 1);
        input_line = disp_line + total_lines - 1;
        MmiUtil_ClearLine(input_line);

        switch (disp_mode)
        {
        case LEFT_JST:
            MmiUtil_DisplayLine(szBuffer + offset, input_line);
            break;
        case RIGHT_JST:
            MmiUtil_DisplayRight(szBuffer + offset, input_line);
            break;
        case CENTER_JST:
        default:
            MmiUtil_DisplayCenter(szBuffer + offset, input_line, FALSE);
            break;
        }

        key = MmiUtil_WaitKey(m_MmiUtil_InputTimeout);

        switch (key)
        {
        case 0:
            return INPUT_TIMEOUT;

        case bESC:
            return INPUT_CANCEL;

        case bENTER:

            if (idx < min)
            {
                continue;
            }

            strcpy(str, szBuffer);
            return idx;
        case bCLR:

            if (idx > 0)
            {
                if (idx > m_MmiUtil_Columns)
                {
                    MmiUtil_ClearLines(disp_line, input_line);
                }
                else
                {
                    MmiUtil_ClearLine(input_line);
                }

                idx = 0;
                memset(szBuffer, 0, sizeof(szBuffer));
            }

            break;

        case bBKSP:

            if (idx)
            {
                szBuffer[--idx] = 0;

                if (idx % m_MmiUtil_Columns == 0)
                {
                    MmiUtil_ClearLine(input_line);
                }
            }
            else
                szBuffer[idx] = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2:
        case bKEY3:
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:

            if (idx < max)
                szBuffer[idx++] = key;

            /*else
              szBuffer[idx-1] = key;*/
            break;
        }
    }
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputLongNum
* DESCRIPTION:   ���������ַ�������󳤶�#define MMI_INPUTTEXT_MAX 256
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   msgPrompt (in)  -- ��ʾ��Ϣ
*   str       (out) -- ������ִ�
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputLongNum(int disp_line, char *msgPrompt, char *str, usint min, usint max, int disp_mode)
{
    int idx = 0;
    byte key = 0;
    char szBuffer[MMI_INPUTTEXT_MAX+4] = {0};
    int input_line;

    if ((max > MMI_INPUTTEXT_MAX) || (min > max) || (strlen(str) > max) )
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    sprintf(szBuffer, "%s", str);

    for (;;)
    {
        idx = strlen(szBuffer);
        idx = idx > max ? max : idx;
        szBuffer[idx] = 0;
        // ��ʾbuffer�������ص�ǰ���һ�е�λ��
        input_line = MmiUtil_DisplayBuffer(szBuffer, disp_line, 0, disp_mode);


        key = MmiUtil_WaitKey(m_MmiUtil_InputTimeout);

        switch (key)
        {
        case 0:
            return INPUT_TIMEOUT;

        case bESC:
            return INPUT_CANCEL;

        case bENTER:

            if (idx < min)
            {
                continue;
            }

            strcpy(str, szBuffer);
            return idx;
        case bCLR:

            if (idx > 0)
            {
                // if (idx > m_MmiUtil_Columns)
                if (input_line > disp_line)
                {
                    MmiUtil_ClearLines(disp_line, input_line);
                }
                else
                {
                    MmiUtil_ClearLine(input_line);
                }

                idx = 0;
                memset(szBuffer, 0, sizeof(szBuffer));
            }

            break;

        case bBKSP:

            if (idx)
            {
                szBuffer[--idx] = 0;

                if (idx % m_MmiUtil_Columns == 0)
                {
                    MmiUtil_ClearLine(input_line);
                }
            }
            else
                szBuffer[idx] = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2:
        case bKEY3:
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:

            if (idx < max)
                szBuffer[idx++] = key;

            /*else
              szBuffer[idx-1] = key;*/
            break;
        }
    }
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputPWD
* DESCRIPTION:   ��������
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   msgPrompt (in)  -- ��ʾ��Ϣ
*   pwd       (out) -- �������
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputPWD(int disp_line, char *msgPrompt, char *pwd, usint min, usint max, int disp_mode)
{
    int idx = 0;
    byte key = 0;
    char str[20+1];
    char szMask[20+1];

    if (max > 16 || min > max)
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    memset(str, 0, sizeof(str));
    memset(szMask, 0, sizeof(szMask));

    for (;;)
    {
        idx = strlen(str);
        idx = idx > max ? max : idx;
        str[idx] = 0;

        memset(szMask, '*', idx);
        szMask[idx] = 0;

        MmiUtil_ClearLine(disp_line);

        switch (disp_mode)
        {
        case LEFT_JST:
            MmiUtil_DisplayLine(szMask, disp_line);
            break;
        case RIGHT_JST:
            MmiUtil_DisplayRight(szMask, disp_line);
            break;
        case CENTER_JST:
        default:
            MmiUtil_DisplayCenter(szMask, disp_line, FALSE);
            break;
        }

        key = MmiUtil_WaitKey(m_MmiUtil_InputTimeout);

        switch (key)
        {
        case 0:
            return INPUT_TIMEOUT;

        case bESC:
            return INPUT_USERABORT;

        case bENTER:

            if (idx < min)
            {
                continue;
            }

            memcpy(pwd, str, idx);
            return idx;

        case bCLR:

            if (idx > 0)
            {
                MmiUtil_ClearLine(disp_line);

                idx = 0;
                memset(str, 0, sizeof(str));
            }

            break;

        case bBKSP:

            if (idx)
                str[--idx] = 0;
            else
                str[idx] = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2:
        case bKEY3:
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:

            if (idx < max)
                str[idx++] = key;

            break;
        }
    }
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputAmount
* DESCRIPTION:   ��������
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   msgPrompt (in)  -- ��ʾ��Ϣ
*   amount    (out) -- ������
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*			����ֵ����0:�������ݳ���
*			����ֵС��0:INPUT_FAILED/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
int MmiUtil_InputAmount(int disp_line, char * msgPrompt, char *amount, usint min, usint max, int disp_mode)
{
    int idx = 0;
    byte key = 0;
    int input_line = disp_line;
    char szBuffer[31] = {0};
    char disp_amount[31] = {0};

    if ((max > 15) || (min > max))
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        input_line++;
    }

    sprintf(szBuffer, "%s", amount);

    for (;;)
    {
        idx = strlen(szBuffer);
        idx = idx > max ? max : idx;
        szBuffer[idx] = 0;

        memset(disp_amount, 0, sizeof(disp_amount));

        if (idx <= 2)
            sprintf(disp_amount, "%03d", atoi(szBuffer));
        else
            strcpy(disp_amount, szBuffer);

        ins_chr(disp_amount, 2, '.');

        MmiUtil_ClearLine(input_line);

        switch (disp_mode)
        {
        case LEFT_JST:
            MmiUtil_DisplayLine(disp_amount, input_line);
            break;
        case RIGHT_JST:
            MmiUtil_DisplayRight(disp_amount, input_line);
            break;
        case CENTER_JST:
        default:
            MmiUtil_DisplayCenter(disp_amount, input_line, FALSE);
            break;
        }

        key = MmiUtil_WaitKey(m_MmiUtil_InputTimeout);

        switch (key)
        {
        case 0:
            return INPUT_TIMEOUT;

        case bESC:
            return INPUT_CANCEL;

        case bENTER:

            if (idx < min)
            {
                continue;
            }

            strcpy(amount, szBuffer);
            return idx;
        case bCLR:

            if (idx > 0)
            {
                MmiUtil_ClearLine(input_line);

                idx = 0;
                memset(szBuffer, 0, sizeof(szBuffer));
            }

            break;

        case bBKSP:

            if (idx)
                szBuffer[--idx] = 0;
            else
                szBuffer[idx] = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2:
        case bKEY3:
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:

            if (idx < max)
                szBuffer[idx++] = key;

            break;
        }
    }
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputBcdCode
* DESCRIPTION:   ��ʾ��ʾ������BCD���ַ���.
* PARAMETERS:
*   msgPrompt   (in)   -- ��ʾ,����ΪNULL.
*       BcdBuf    (out)  -- �������in BCD
*       BcdBytes  (in)   -- Ҫ������ֽ���(in BCD)���100
*       IsDispOrg (in)   --�Ƿ���ʾԭ����Ϣ
* RETURN:         >=0 ������ֽڸ���
* INPUT_OK/INPUT_USERABORT/INPUT_TIMEOUT/INPUT_FAILED
* NOTES:
* ------------------------------------------------------------------------ */
sint MmiUtil_InputBcdCode(int disp_line, char *msgPrompt, byte *BcdBuf,
                          usint BcdBytes, boolean IsDispOrg)
{
    char   szKeyMask[10][5] = {"0", "1", "2ABC", "3DEF", "4", "5", "6", "7", "8", "9"};
    int    curKey = 0;
    int    keyCounter = 0;
    int    Key = 0;
    int    idx = 0;
    int    iDispOffset = 0;
    int    len = 0;
    char   szOutputText[210];
    int    max = BcdBytes * 2;
    char   line1buf[30];
    int    input_line, total_lines, i;

    if ((BcdBytes * 2) > 200)
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    memset(szOutputText, 0, sizeof(szOutputText));

    if (IsDispOrg)
    {
        BankUtil_Bcd2Asc(BcdBuf, (byte *)szOutputText, BcdBytes * 2);
    }

    idx = strlen(szOutputText);

    for (keyCounter = 0, curKey = 0; ;)
    {
        len = strlen(szOutputText);

        if (len == 0 || len % DEFAULT_CHAR_WIDTH)
            total_lines = len / DEFAULT_CHAR_WIDTH + 1;
        else
            total_lines = len / DEFAULT_CHAR_WIDTH;

        for (i = 1; i < total_lines; i++)
        {
            iDispOffset = DEFAULT_CHAR_WIDTH * (i - 1);
            input_line = disp_line + (i - 1);
            memset(line1buf, 0, sizeof(line1buf));
            strncpy(line1buf, szOutputText + iDispOffset, DEFAULT_CHAR_WIDTH);
            MmiUtil_ClearLine(input_line);
            MmiUtil_DisplayLine(line1buf, input_line);
        }

        iDispOffset = DEFAULT_CHAR_WIDTH * (total_lines - 1);
        input_line = disp_line + total_lines - 1;
        MmiUtil_ClearLine(input_line);
        MmiUtil_DisplayLine(szOutputText + iDispOffset, input_line);
        Key = MmiUtil_WaitKey(1);

        switch (Key)
        {
        case 0://��ʱ

            if (szOutputText[idx])
            {
                //�����ǰ�ַ���Ϊ����������һ���ַ�
                if (idx < (max - 1))
                    idx += 1;

                keyCounter = 0;
            }
            else
            {
                //���Ϊ�ռ������뵱ǰ�ַ�
                //return INPUT_TIMEOUT;
            }

            break;

        case bESC:  //ȡ��
            return INPUT_USERABORT;

        case bENTER:
            BankUtil_Asc2Bcd((byte *)szOutputText, BcdBuf, strlen(szOutputText));
            return strlen(szOutputText) / 2;

        case bCLR:  //���

            if (idx > 0)
            {
                if (iDispOffset > 0)
                {
                    MmiUtil_ClearLines(disp_line, input_line);
                }
                else
                {
                    MmiUtil_ClearLine(input_line);
                }
            }

            idx = 0;
            memset(szOutputText, 0, sizeof(szOutputText));
            keyCounter = 0;

            break;

        case bBKSP: //�˸�

            if (szOutputText[idx])    //��ǰ�ַ�����,ɾ����ǰ�ַ�
            {
                szOutputText[idx] = 0;
            }
            else            //��ǰ�ַ�Ϊ��,ɾ��ǰһ���ַ�
            {
                if (idx)
                {
                    idx -= 1;
                    szOutputText[idx] = 0;
                }
            }

            if (idx % DEFAULT_CHAR_WIDTH == 0)
            {
                MmiUtil_ClearLine(input_line);
            }

            keyCounter = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2://abc
        case bKEY3://def
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:

            //����0-9
            if (szOutputText[idx] == 0)
            {
                //��ǰ�ַ�Ϊ��,��Ϊ��ǰ����,��Ϊ��ǰ�����ĵ�һ���ַ�
                szOutputText[idx] = szKeyMask[(Key - '0')][0];
                keyCounter = 1;
            }
            else if (curKey == Key && strlen(szKeyMask[Key - '0']) > 1)
            {
                //����һ��������ͬ,��Ϊ��ǰ����,���л���ǰ����ֵ
                szOutputText[idx] = szKeyMask[(Key - '0')] \
                                    [(keyCounter % strlen(szKeyMask[(Key - '0')]))];
                keyCounter += 1;
                keyCounter = keyCounter % strlen(szKeyMask[(Key - '0')]);
            }
            else
            {
                //��ͬ����,��Ϊ��һ�������ַ�,��Ϊ��ǰ�����ĵ�һ���ַ�
                //�������,��Ҫɾ�������ַ�
                //��������Ҫɾ��1�������ַ���1�������ַ�
                while ((len + 1) > max)
                {
                    {
                        //�����ַ���ɾһ���ֽ�
                        szOutputText[len - 1] = 0;
                        len -= 1;
                    }
                }

                idx = strlen(szOutputText);
                szOutputText[idx] = szKeyMask[(Key - '0')][0];
                keyCounter = 1;
            }

            break;
        }

        curKey = Key;
    }
}


/* ------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_flushkbd
* DESCRIPTION:   ��հ���buffer
* PARAMETERS:
* RETURN:
* NOTES:
* -----------------------------------------------------------------------*/
void MmiUtil_flushkbd(void)
{

    //ApplLog_Debug("ATOOL","flush kbd");

    if (Device_GetHandle(DEV_CONSOLE) < 0)
        return;

    while (KBHIT())
    {
        get_char();
    }
}

/* ------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_flushcard
* DESCRIPTION:  ��մ�����������buffer
* PARAMETERS:
* RETURN:
* NOTES:
* -----------------------------------------------------------------------*/
void MmiUtil_flushcard(void)
{
    char buf[CARD_LEN_MAX];
    int reader_handle;

    //ApplLog_Debug("ATOOL","flush card");

    reader_handle = Device_GetHandle(DEV_CARD);

    if (reader_handle < 0)
    {
        return;
    }

    memset(buf, 0, sizeof(buf));

    while (read(reader_handle, (char *)buf, CARD_LEN_MAX) > 0);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MMiUtil_GetTrack
* DESCRIPTION:    �������������俨�ţ�ѭ���ȴ�����ʱ�˳���
* PARAMETERS:
*   CardBuf1 (out) -- 1�ŵ�����
*   CardBuf2 (out) -- 2�ŵ�����
*   CardBuf3 (out) -- 3�ŵ�����
*   PANnum   (out) -- �ֹ�����Ŀ���
*   manflag  (in)  -- �Ƿ�֧�����俨��
* RETURN:
*   TRAN_OK/TRAN_NUM/TRAN_ESC/TRACK_TIMEOUT
* NOTES:
* --------------------------------------------------------------------------*/
int MmiUtil_GetTrack(byte *CardBuf1, byte *CardBuf2, byte *CardBuf3, byte *PANnum, int manflag)
{
    char  acct[25];
    char  *ptr;
    int   len1 = 0, len2 = 0, len3 = 0;
    unsigned long   tstart, tnow, Tout;
    char  t_card_image[CARD_LEN_MAX];
    char  key;

    int reader_handle;
    int pan_len;
    //char  track1 = 1, track2 = 1, track3 = 1; //1Ϊ����,0Ϊ�ɹ�

    //ApplLog_Debug("ATOOL","MmiUtil_GetTrack");

    reader_handle = Device_GetHandle(DEV_CARD);

    if (reader_handle < 0)
    {
        return TRACK_FAIL;
    }

    CardBuf1[0] = 0;
    CardBuf2[0] = 0;
    CardBuf3[0] = 0;

    //MmiUtil_flushkbd();
    //MmiUtil_flushcard();

    Tout = MmiUtil_GetTimeOut();
    tnow = tstart = read_ticks();
    MmiUtil_flushkbd();
    MmiUtil_flushcard();

    while (1)
    {
        if (Tout > 0)
        {
            tnow = read_ticks();

            if (tnow > (tstart + Tout*1000))
            {
                MmiUtil_flushkbd();
                MmiUtil_flushcard();
                return TRACK_TIMEOUT;
            }
        }

        if (act_kbd_pending_test(KEY_CANCEL))
        {
            MmiUtil_flushkbd();
            MmiUtil_flushcard();
            return TRACK_ESC;
        }

        /*����Ƿ���ڰ���*/
        if (KBHIT())
        {
            //ApplLog_Debug("ATOOL","key input");
            key = get_char();

            if (key == bESC)
                return TRACK_ESC;
            else if (key >= bKEY0 && key <= bKEY9 && manflag)
            {
                memset(acct, 0, sizeof(acct));
                acct[0] = key;

                MmiUtil_ClearLines(LINE2, LINE3);
                pan_len = MmiUtil_InputNum(LINE2, NULL, (char*)acct, 10, 19, LEFT_JST);
                //ApplLog_Debug("ATOOL","input card no ret=[%d] pan=[%s]", pan_len, acct);

                if (pan_len <= 0)
                {
                    MmiUtil_flushkbd();
                    MmiUtil_flushcard();
                    return TRACK_ESC;
                }

                strncpy((char*)PANnum, acct, pan_len);
                MmiUtil_flushkbd();
                MmiUtil_flushcard();
                return TRACK_NUM;
            }
        }

        /*����Ƿ���ڴŵ���Ϣ*/
        if (chk_card_rdr(reader_handle))
        {
            MmiUtil_flushkbd();
            memset(t_card_image, 0, sizeof(t_card_image));
            pan_len = read(reader_handle, t_card_image, CARD_LEN_MAX);

            if (pan_len < CARD_LEN_MIN)
            {
                beep(500);
                continue;
            }

            //ApplLog_DebugHex("ATOOL","card_image:", t_card_image, pan_len);

            beep(500);

            /*�Ѿ���ȡ�ŵ���Ϣ*/
            len1 = *t_card_image;
            len2 = *(t_card_image + len1);
            len3 = *(t_card_image + len1 + len2);

            //��ȡ���ŵ���Ϣ
            if ((len3 > 2) && (len3 <= 106))
            {
                ptr = t_card_image + len1 + len2 + 2;
                memcpy((char*)CardBuf3, ptr, len3 - 2);
            }

            //��ȡ���ŵ���Ϣ
            if ((len2 > 2) && (len2 <= 39))
            {
                ptr = t_card_image + len1 + 2;
                memcpy((char*)CardBuf2, ptr, len2 - 2);
            }

            //��ȡһ�ŵ���Ϣ
            if ((len1 > 2) && (len1 < 79))
            {
                ptr = t_card_image + 2;
                memcpy((char*)CardBuf1, ptr, len1 - 2);
            }

            //ApplLog_Debug("ATOOL","card1=[%d] ->[%s]", len1, CardBuf1);
            //ApplLog_Debug("ATOOL","card2=[%d] ->[%s]", len2, CardBuf2);
            //ApplLog_Debug("ATOOL","card3=[%d] ->[%s]", len3, CardBuf3);

            MmiUtil_flushcard();
            return TRACK_OK;

        }//end if


    }//end while

}


/* --------------------------------------------------------------------------
* FUNCTION NAME: MMiUtil_GetTrack2
* DESCRIPTION:    �������������俨�ţ�ֻ��һ�Σ�û��ѭ����
* PARAMETERS:
*   CardBuf1 (out) -- 1�ŵ�����
*   CardBuf2 (out) -- 2�ŵ�����
*   CardBuf3 (out) -- 3�ŵ�����
*   PANnum   (out) -- �ֹ�����Ŀ���
*   manflag  (in)  -- �Ƿ�֧�����俨��
* RETURN:
*   TRAN_OK/TRAN_NUM/TRAN_ESC/TRACK_NOEVENT
* NOTES:
* --------------------------------------------------------------------------*/
int MmiUtil_GetTrack2(byte *CardBuf1, byte *CardBuf2, byte *CardBuf3, byte *PANnum, int manflag)
{
    char  acct[25];
    char  *ptr;
    int   len1 = 0, len2 = 0, len3 = 0;
    char  t_card_image[CARD_LEN_MAX];
    int reader_handle;
    int pan_len;
    int key;
    //char  track1 = 1, track2 = 1, track3 = 1; //1Ϊ����,0Ϊ�ɹ�

    //ApplLog_Debug("ATOOL","MmiUtil_GetTrack2");

    reader_handle = Device_GetHandle(DEV_CARD);

    if (reader_handle < 0)
    {
        return TRACK_FAIL;
    }

    CardBuf1[0] = 0;
    CardBuf2[0] = 0;
    CardBuf3[0] = 0;

    /*����Ƿ���ڰ���*/
    if (KBHIT())
    {
        //ApplLog_Debug("ATOOL","key input");
        key = get_char();

        if (key == bESC)
            return TRACK_ESC;
        else if (key >= bKEY0 && key <= bKEY9 && manflag)
        {
            memset(acct, 0, sizeof(acct));
            acct[0] = key;

            MmiUtil_ClearLines(LINE2, LINE3);
            pan_len = MmiUtil_InputNum(LINE2, NULL, (char*)acct, 10, 19, LEFT_JST);
            //ApplLog_Debug("ATOOL","input card no ret=[%d] pan=[%s]", pan_len, acct);

            if (pan_len <= 0)
            {
                MmiUtil_flushkbd();
                MmiUtil_flushcard();
                return TRACK_ESC;
            }

            strncpy((char*)PANnum, acct, pan_len);
            MmiUtil_flushkbd();
            MmiUtil_flushcard();
            return TRACK_NUM;
        }
    }

    /*����Ƿ���ڴŵ���Ϣ*/
    if (chk_card_rdr(reader_handle))
    {
        MmiUtil_flushkbd();
        memset(t_card_image, 0, sizeof(t_card_image));
        pan_len = read(reader_handle, t_card_image, CARD_LEN_MAX);

        if (pan_len < CARD_LEN_MIN)
        {
            beep(500);
            return TRACK_FAIL;
        }

        //ApplLog_DebugHex("ATOOL","card_image:", t_card_image, pan_len);

        beep(500);

        /*�Ѿ���ȡ�ŵ���Ϣ*/
        len1 = *t_card_image;
        len2 = *(t_card_image + len1);
        len3 = *(t_card_image + len1 + len2);

        /*track1 = *(t_card_image + 1);
        track2 = *(t_card_image + *t_card_image + 1);
        track3 = *(t_card_image + *t_card_image + len2 + 1);*/

        if ((len3 > 2) && (len3 <= 106))
        {
            ptr = t_card_image + len1 + len2 + 2;
            memcpy((char*)CardBuf3, ptr, len3 - 2);
        }

        //��ȡ���ŵ���Ϣ
        if ((len2 > 2) && (len2 <= 39))
        {
            ptr = t_card_image + len1 + 2;
            memcpy((char*)CardBuf2, ptr, len2 - 2);
        }

        //��ȡһ�ŵ���Ϣ
        if ((len1 > 2) && (len1 < 79))
        {
            ptr = t_card_image + 2;
            memcpy((char*)CardBuf1, ptr, len1 - 2);
        }

        //ApplLog_Debug("ATOOL","card1=[%d] ->[%s]", len1, CardBuf1);
        //ApplLog_Debug("ATOOL","card2=[%d] ->[%s]", len2, CardBuf2);
        //ApplLog_Debug("ATOOL","card3=[%d] ->[%s]", len3, CardBuf3);
        MmiUtil_flushcard();
        return TRACK_OK;

    }//end if

    return TRACK_NOEVENT;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_DispBMP
* DESCRIPTION:   ��ʾbitmap�ļ�
* PARAMETERS:
*   BmpFile  (in) -- ͼƬ�ļ����ƣ���׺.bmp��
*   x        (in) -- ͼƬ��ʾ���ϽǺ����꣨���أ�
*   y        (in) -- ͼƬ��ʾ���Ͻ������꣨���أ�
*   width    (in) -- ͼƬ��ȣ����أ�
*   height   (in) -- ͼƬ�߶ȣ����أ�
* RETURN:
*   0(success)/ -1(fail)
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_DispBMP(char *BmpFile, int x, int y, int width, int height)
{
    set_display_coordinate_mode(PIXEL_MODE);
    window(x, y, x + width, y + height);
    return put_BMP(BmpFile);
}




#define MAX_BUTTON_NUM    20
typedef struct
{
    int startx;
    int starty;
    int endx;
    int endy;
    char Text[36];
    int ReturnValue;
} BUTTONLIST;

static BUTTONLIST ButtonList[MAX_BUTTON_NUM];
static int ButtonHeight;
static int TextFontSize ;
static unsigned long TextRGBColor;

/*=====================================================================================*
                  BUTTON������ģ��˵��

  1.BUTTONֻ��VX680/820��320*240��Ļ��ʹ�á�

  2.BUTTON���ȸ�����ʾ�ı��ĳ����Զ�������

  3.BUTTONѡ�к�ķ���ֵ�������á�

  4.ʹ��G15 FLASH�е�BTN1_L.BMP,BTN1_R.BMP,BTN1_C.BMP��Ϊ��ʾBUTTON�����
    BTN2_L.BMP,BTN2_R.BMP,BTN2_C.BMP��Ϊѡ��ʱ��ʾ��BUTTON�����

  5.����ͨ�����������ļ��������Լ�������Ч������Ҫ��ѭ����Ҫ��
  a.BUTTONͨ�����С���3��ͼƬ�ļ���϶���
  b.�м䲿�ֵĳ���Ϊ240���أ�����Ҳ��ֵ�ͼƬ���Ϊ10������
  c.BUTTON�ĸ߶ȿɸ���BMP�ĸ߶��Զ��壬ͨ��MmiUtil_SetTouchButton������(Ĭ�ϵ�BMP�ļ��߶�Ϊ32����)
  d.BMP�ļ�Ϊ24λ��ɫ��������32λ��ɫ
*======================================================================================*/

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_SetTouchButton
* DESCRIPTION:   ��ʼ����ʾbutton��ť����
* PARAMETERS:
*   iButtonHeight    (in) -- button�ĸ߶�       ��0-Ĭ��32���أ�
*   iTextFontSize        (in) -- button����ʾ���ı��Ĵ�С ��0-Ĭ��24����
*   ulTextRGBColor       (in) -- button��ʾ���ı�����ɫ   ��0-Ĭ�Ϻ�ɫ��
* RETURN:
* NOTES:    ����������ֻ�ǳ�ʼ��BUTTON��ʾ�ʹ���ʱ����ز���
* ------------------------------------------------------------------------ */
void MmiUtil_SetTouchButton(int iButtonHeight, int iTextFontSize, unsigned long ulTextRGBColor)
{
    int i = 0 ;

    if (iButtonHeight <= 0)
        iButtonHeight = 32 ;

    if (iTextFontSize <= 0)
        iTextFontSize = 24 ;

    ButtonHeight = iButtonHeight;
    TextFontSize = iTextFontSize;
    TextRGBColor = ulTextRGBColor;
    memset(ButtonList , 0 , sizeof(BUTTONLIST)*MAX_BUTTON_NUM);

    for (i = 0 ; i < MAX_BUTTON_NUM ; i++)
    {
        ButtonList[i].startx = -1;
        ButtonList[i].starty = -1;
    }
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_ShowTouchButton
* DESCRIPTION:   ��ʾbutton��ť,�����MmiUtil_CheckTouchButtonʹ��
* PARAMETERS:
*   Str    (in) -- button����ʾ���֣���\x00��������
*   x        (in/out) -- button��ʾ���ϽǺ����꣨���أ�,����button�����½Ǻ�����
*   y        (in/out) -- button��ʾ���Ͻ������꣨���أ�,����button�����½�������
*   RetVal   (in) --  ����buttonʱ����ֵ�趨
* RETURN:
* NOTES:    ����֧��20��BUTTON��ʹ��ǰ����MmiUtil_SetTouchButton��������
*       ������BUTTON�ص����������һ����ʾ��BUTTONΪ׼
*       ��ͨ�����ص�(x,y)���������һ��BUTTON��ʾ��λ��
* ------------------------------------------------------------------------ */
void MmiUtil_ShowTouchButton(char *Str, int *x, int *y, int RetVal)
{
    int StartX = 1 , StartY = 1;
    int StrLen = 0;
    int i ;

    displayInfo_t dispInfo;
    memset(&dispInfo, 0, sizeof(displayInfo_t));
    SVC_INFO_DISPLAY_EXT(&dispInfo);

    StrLen = strlen(Str);

    if (StrLen < 0)
        return ;

    if (*x <= 0 || *x >= dispInfo.width)
        *x = 1;

    if (*y <= 0 || *y >= dispInfo.height)
        *y = 1;

    StartX = *x;
    StartY = *y;

    if (ButtonHeight <= 0)
        ButtonHeight = 32 ;

    if (TextFontSize <= 0)
        TextFontSize = 24 ;

    if (dispInfo.height == 240)
    {
        MmiUtil_DispBMP("F:/BTN3_C.bmp", StartX + 10, StartY, TextFontSize*StrLen / 2 , ButtonHeight);
        MmiUtil_DispBMP("F:/BTN3_L.bmp", StartX, StartY, 10 , ButtonHeight);
        MmiUtil_DispBMP("F:/BTN3_R.bmp", StartX + 10 + TextFontSize*StrLen / 2, StartY, 10, ButtonHeight);

        MmiUtil_DispStrByColor(Str, StartX + 10, StartY + 3, TextFontSize, TextRGBColor, 0xffffff);
    }
    else
    {
        MmiUtil_DispBMP("F:/BTN1_C.bmp", StartX + 10, StartY, TextFontSize*StrLen / 2 , ButtonHeight);
        MmiUtil_DispBMP("F:/BTN1_L.bmp", StartX, StartY, 10 , ButtonHeight);
        MmiUtil_DispBMP("F:/BTN1_R.bmp", StartX + 10 + TextFontSize*StrLen / 2, StartY, 10, ButtonHeight);

        MmiUtil_DispStrByColor(Str, StartX + 10, StartY + 4, TextFontSize, TextRGBColor, 0xffffff);
    }

    *x = StartX + 10 + TextFontSize * StrLen / 2 + 10;
    *y = StartY + ButtonHeight;

    //����BUTTON�б�
    for (i = 0 ; i < MAX_BUTTON_NUM ; i++)
    {
        if (ButtonList[i].startx == -1)
        {
            strncpy(ButtonList[i].Text , Str, sizeof(ButtonList[i].Text) - 1);
            ButtonList[i].startx = StartX ;
            ButtonList[i].starty = StartY ;
            ButtonList[i].endx = *x ;
            ButtonList[i].endy = *y ;
            ButtonList[i].ReturnValue = RetVal ;
            break ;
        }
    }
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_CheckTouchButton
* DESCRIPTION:   ����Ƿ���BUTTON������
* PARAMETERS:
*   RetVal (out)     - �����µ�BUTTON�趨�ķ���ֵ
* RETURN:        0 - �ް�ť������
*        1 - �а�ť������
* NOTES:         ����������������
*        ���û����MmiUtil_SetTouchButton�����޷���⵽BUTTON
* ------------------------------------------------------------------------ */
int MmiUtil_CheckTouchButton(int *RetVal)
{
    int  x = 0, y = 0;
    int i, TextLen;

    if (get_touchscreen(&x, &y))
    {

        for (i = MAX_BUTTON_NUM - 1 ; i >= 0 ; i--)
        {

            if (ButtonList[i].startx != -1
                    && (ButtonList[i].startx <= x && ButtonList[i].endx >= x)
                    && (ButtonList[i].starty <= y && ButtonList[i].endy >= y)
               )
            {
                TextLen = strlen(ButtonList[i].Text);

                MmiUtil_DispBMP("F:/BTN2_C.bmp", ButtonList[i].startx + 10, ButtonList[i].starty, TextFontSize*TextLen / 2 , ButtonHeight);
                MmiUtil_DispBMP("F:/BTN2_L.bmp", ButtonList[i].startx, ButtonList[i].starty, 10 , ButtonHeight);
                MmiUtil_DispBMP("F:/BTN2_R.bmp", ButtonList[i].startx + 10 + TextFontSize*TextLen / 2, ButtonList[i].starty, 10, ButtonHeight);
                MmiUtil_DispStrByColor(ButtonList[i].Text, ButtonList[i].startx + 10, ButtonList[i].starty + 4, TextFontSize, TextRGBColor, 0xffffff);

                while (get_touchscreen(&x, &y));

                if ((ButtonList[i].startx <= x && ButtonList[i].endx >= x)
                        && (ButtonList[i].starty <= y && ButtonList[i].endy >= y)
                   )
                {
                    *RetVal = ButtonList[i].ReturnValue;
                    return 1;
                }
                else
                {
                    TextLen = strlen(ButtonList[i].Text);

                    MmiUtil_DispBMP("F:/BTN1_C.bmp", ButtonList[i].startx + 10, ButtonList[i].starty, TextFontSize*TextLen / 2 , ButtonHeight);
                    MmiUtil_DispBMP("F:/BTN1_L.bmp", ButtonList[i].startx, ButtonList[i].starty, 10 , ButtonHeight);
                    MmiUtil_DispBMP("F:/BTN1_R.bmp", ButtonList[i].startx + 10 + TextFontSize*TextLen / 2, ButtonList[i].starty, 10, ButtonHeight);
                    MmiUtil_DispStrByColor(ButtonList[i].Text, ButtonList[i].startx + 10, ButtonList[i].starty + 4, TextFontSize, TextRGBColor, 0xffffff);
                    return 0;
                }

            }
        }
    }

    return 0 ;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_GetPrinterType.
* DESCRIPTION:   ��ȡ��ӡ������
* PARAMETERS:
* RETURN:
*   PrinterType -- ��ӡ�����ͣ��ο�enumPrinterType��
* NOTES:
* ------------------------------------------------------------------------ */
int MmiUtil_GetPrinterType(void)
{
    int   TermType = 0;
    int   printer_type = 0;

    TermType = MmiUtil_GetTerminalType();

    switch (TermType)
    {
    case _VX520S:
    case _VX805:
        printer_type = SPROCKET_PRINTER;
        break;
    default:
        printer_type = THERMAL_PRINTER;
        break;
    }

    return printer_type;
}

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
int MmiUtil_SetKeypadState(int sleep)
{
    return cs_set_sleep_state(sleep);
}

/*=========================================*
*   P R I V A T E     F U N C T I O N S   *
*=========================================*/

int IsTTFSupport(void)
{
    int   TermType = 0;

    TermType = MmiUtil_GetTerminalType();

    if (TermType == _VX680 || TermType == _VX820 || TermType == _VX680C || TermType == _VX675)
        return 1;

    return 0;
}

int IsTouchScreen(void)
{
    int   TermType = 0;

    TermType = MmiUtil_GetTerminalType();

    if (TermType == _VX680 || TermType == _VX820 || TermType == _VX680C || TermType == _VX675 || TermType == _VX600)
        return 1;

    return 0;
}

int isGbkFont16(void)
{
    int   fp;
    static byte gbk16found;

    if( gbk16found == '1' )
        return 1;
    else if( gbk16found == '2' )
        return 0;

    fp = open( GBKFONT_16, O_RDONLY );
    if ( fp < 0 )
    {
        gbk16found = '2';
        return 0;
    }
    close(fp);

    gbk16found = '1';
    return 1;
}

int isGbkFont12(void)
{
    int   fp;
    static byte gbk12found;

    if( gbk12found == '1' )
        return 1;
    else if( gbk12found == '2' )
        return 0;

    fp = open( GBKFONT_12, O_RDONLY );
    if ( fp < 0 )
    {
        gbk12found = '2';
        return 0;
    }
    close(fp);

    gbk12found = '1';
    return 1;
}

/***************************************************************************
** ins_chr() - inserts a char in string
***************************************************************************/
static void ins_chr(char *str, char offset, char chr)
{
    char *dst;

    if (offset && ((int)offset <= strlen(str)))
    {
        /* destination now points to after null */
        dst = str + strlen(str) + 1;
        *(dst + 1) = 0;

        /* shift 'offset+1' characters to one position to the right */
        offset++;

        while (offset--)
        {
            *dst = *(dst - 1);
            dst--;
        }

        *dst = chr;
    }
}


static void disp_set_font(int fsize, unsigned short color)
{
    tf_fontsize fptr2;
    tf_setcolor fptr3;
    displayInfo_t dispInfo;

    if (dll_handle < 0)
        return;

    font_size = fsize;
    //ft_set_font_size(font_size);
    fptr2 = (tf_fontsize)DLL_function_address(dll_handle, 2);

    if (fptr2 == NULL)
        return;

    fptr2(font_size);

    //ft_set_color(color);
    fptr3 = (tf_setcolor)DLL_function_address(dll_handle, 3);

    if (fptr3 == NULL)
        return;

    fptr3(color);

    memset(&dispInfo, 0, sizeof(displayInfo_t));
    SVC_INFO_DISPLAY_EXT(&dispInfo);


    if (dispInfo.height == 240)
    {
        line_space = (fsize / 8) + ((fsize % 8) ? 0 : -1);
    }
    else
    {
        line_space = (fsize / 8) + ((fsize % 8) ? 1 : 0);
    }

    line_height = font_size + line_space;

    m_MmiUtil_Rows = (dispInfo.height - HEADER_HEIGHT) / line_height;
    m_MmiUtil_Columns = (dispInfo.width * 2) / font_size;

    // Added by @laikey_20130326 -->
    m_MmiUtil_Rows_real = ((dispInfo.height - HEADER_HEIGHT) % line_height) ? 1 : 0;
    // <--

#if 0
    {
        char cTmpBuf[ 56 ];

        sprintf( cTmpBuf, "height=%d,width=%d", dispInfo.height, dispInfo.width );
        MmiUtil_DisplayLine( cTmpBuf, 0 );

        sprintf( cTmpBuf, "fsize=%d,font_size=%d", fsize, font_size );
        MmiUtil_DisplayLine( cTmpBuf, 1 );

        sprintf( cTmpBuf, "line_space=%d", line_space );
        MmiUtil_DisplayLine( cTmpBuf, 2 );

        sprintf( cTmpBuf, "line_height=%d", line_height );
        MmiUtil_DisplayLine( cTmpBuf, 3 );

        sprintf( cTmpBuf, "Row=%d, Col=%d", m_MmiUtil_Rows, m_MmiUtil_Columns );
        MmiUtil_DisplayLine( cTmpBuf, 4 );

        MmiUtil_WaitKey( 0 );
    }
#endif

    //LOG_PRINTF(("screen size y=[%d] x=[%d]", dispInfo.height, dispInfo.width));
    //LOG_PRINTF(("font_size=[%d] line_space=[%d] line_height=[%d]", font_size, line_space, line_height));
    //LOG_PRINTF(("row=[%d] column [%d]", m_MmiUtil_Rows, m_MmiUtil_Columns));
}

static void clear_screen(void)
{
    displayInfo_t dispInfo;
    memset(&dispInfo, 0, sizeof(displayInfo_t));
    SVC_INFO_DISPLAY_EXT(&dispInfo);

    window(0, 0, dispInfo.width, dispInfo.height);
    clrscr();
}

static void clear_lines(int start_line, int end_line)
{
    int start_y, end_y;
    displayInfo_t dispInfo;

    memset(&dispInfo, 0, sizeof(displayInfo_t));
    SVC_INFO_DISPLAY_EXT(&dispInfo);

    start_y = (start_line - 1) * line_height + HEADER_HEIGHT;

    // Modity by @laikey_20130326 -->
    if ( end_line >= m_MmiUtil_Rows )
        end_y = (end_line + m_MmiUtil_Rows_real) * line_height + HEADER_HEIGHT;
    else
        // <--
        end_y = end_line * line_height + HEADER_HEIGHT;

    window(0, start_y, dispInfo.width, end_y);
    clrscr();
    window(0, 0, dispInfo.width, dispInfo.height);
}

static void disp_line(char *str, int x, int y, int invert)
{
    int startx, starty;
    tf_displine fptr4;
    displayInfo_t dispInfo;

    if (y > m_MmiUtil_Rows)
        y = m_MmiUtil_Rows;

    memset(&dispInfo, 0, sizeof(displayInfo_t));
    SVC_INFO_DISPLAY_EXT(&dispInfo);

    if (dll_handle < 0)
        return;

    startx = (x - 1) * (font_size / 2);
    starty = HEADER_HEIGHT + (y - 1) * line_height;

    //ft_disp_line(str, startx, starty);
    fptr4 = (tf_displine)DLL_function_address(dll_handle, 4);

    if (fptr4 == NULL)
        return;

    fptr4(str, startx, starty);

    if (invert == TRUE)
        invert_pixel_block(0, starty, dispInfo.width, starty + line_height - 1);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: MmiUtil_InputText
* DESCRIPTION:   �����ַ���(��ĸ������)
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   str       (out) -- ����ַ���
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*     INPUT_OK/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
sint Vx680_InputText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode)
{
    /*for Vx680*/
    char szKeyMask[MAX_ALPNUM_KEYS][9] =
    {
        "0- +%_", "1QZqz.\\", "2ABCabc&", "3DEFdef%",
        "4GHIghi*","5JKLjkl/", "6MNOmno~", "7PRSprs^",
        "8TUVtuv[","9WXYwxy]", "*,'\":","#=:$?@"
    };

    int    curKey = 0;
    int    keyCounter = 0;
    int    Key = 0;
    int    idx = 0;
    int    iDispOffset = 0;
    int    len = 0;
    char   szOutputText[110];
    int   keyoffset = 0;
    int   input_line, total_lines, i;

    if ((max > 100) || (min > max))
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    memset(szOutputText, 0, sizeof(szOutputText));
    sprintf(szOutputText, "%s", str);

    idx = strlen(szOutputText);

    for (keyCounter = 0, curKey = 0; ;)
    {
        len = strlen(szOutputText);

        if (len == 0 || len % m_MmiUtil_Columns)
            total_lines = len / m_MmiUtil_Columns + 1;
        else
            total_lines = len / m_MmiUtil_Columns;

        for (i = 1; i < total_lines; i++)
        {
            iDispOffset = m_MmiUtil_Columns * (i - 1);
            input_line = disp_line + (i - 1);
            MmiUtil_ClearLine(input_line);
            MmiUtil_DisplayLine(szOutputText + iDispOffset, input_line);
        }

        iDispOffset = m_MmiUtil_Columns * (total_lines - 1);
        input_line = disp_line + total_lines - 1;
        MmiUtil_ClearLine(input_line);

        switch (disp_mode)
        {
        case LEFT_JST:
            MmiUtil_DisplayLine(szOutputText + iDispOffset, input_line);
            break;
        case RIGHT_JST:
            MmiUtil_DisplayRight(szOutputText + iDispOffset, input_line);
            break;
        case CENTER_JST:
        default:
            MmiUtil_DisplayCenter(szOutputText + iDispOffset, input_line, FALSE);
            break;
        }

        Key = MmiUtil_WaitKey(1);

        switch (Key)
        {
        case 0://��ʱ

            if (szOutputText[idx])
            {
                //�����ǰ�ַ���Ϊ����������һ���ַ�
                if (idx < (max - 1))
                    idx += 1;

                keyCounter = 0;
            }
            else
            {
                //���Ϊ�ռ������뵱ǰ�ַ�
                //return INPUT_TIMEOUT;
            }

            break;

        case bESC:  //ȡ��
            return INPUT_USERABORT;

        case bENTER:

            if (strlen(szOutputText) < min)
            {
                continue;
            }

            strcpy(str, szOutputText);
            return strlen(szOutputText);

        case bCLR:  //���

            if (idx > 0)
            {
                if (iDispOffset > 0)
                {
                    MmiUtil_ClearLines(disp_line, input_line);
                }
                else
                {
                    MmiUtil_ClearLine(input_line);
                }
            }

            idx = 0;
            memset(szOutputText, 0, sizeof(szOutputText));
            keyCounter = 0;

            break;

        case bBKSP: //�˸�

            if (szOutputText[idx])    //��ǰ�ַ�����,ɾ����ǰ�ַ�
            {
                szOutputText[idx] = 0;
            }
            else            //��ǰ�ַ�Ϊ��,ɾ��ǰһ���ַ�
            {
                if (idx)
                {
                    idx -= 1;
                    szOutputText[idx] = 0;
                }
            }

            if (idx % m_MmiUtil_Columns == 0)
            {
                MmiUtil_ClearLine(input_line);
            }

            keyCounter = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2://abc
        case bKEY3://def
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:
        case bSTR:
        case bPND:

            if (Key >= bKEY0 && Key <= bKEY9)
                keyoffset = Key - '0';
            else if (Key == bSTR)
                keyoffset = 10;
            else
                keyoffset = 11;

            //����0-9
            if (szOutputText[idx] == 0)
            {
                //��ǰ�ַ�Ϊ��,��Ϊ��ǰ����,��Ϊ��ǰ�����ĵ�һ���ַ�
                szOutputText[idx] = szKeyMask[keyoffset][0];
                keyCounter = 1;
            }
            else if (curKey == Key)
            {
                //����һ��������ͬ,��Ϊ��ǰ����,���л���ǰ����ֵ
                szOutputText[idx] = szKeyMask[keyoffset] \
                                    [(keyCounter % strlen(szKeyMask[keyoffset]))];
                keyCounter += 1;
                keyCounter = keyCounter % strlen(szKeyMask[keyoffset]);
            }
            else
            {
                //��ͬ����,��Ϊ��һ�������ַ�,��Ϊ��ǰ�����ĵ�һ���ַ�
                //�������,��Ҫɾ�������ַ�
                //��������Ҫɾ��1�������ַ���1�������ַ�
                while ((len + 1) > max)
                {
                    {
                        //�����ַ���ɾһ���ֽ�
                        szOutputText[len - 1] = 0;
                        len -= 1;
                    }
                }

                idx = strlen(szOutputText);
                szOutputText[idx] = szKeyMask[keyoffset][0];
                keyCounter = 1;
            }

            break;
        }

        curKey = Key;
    }
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: Vx680_InputLongText
* DESCRIPTION:   �����ַ���(��ĸ������)����󳤶�#define MMI_INPUTTEXT_MAX 256
* PARAMETERS:
*   disp_line (in)  -- ��ʾ��Ϣ��ʾ����
*   str       (out) -- ����ַ���
*   min       (in)  -- ����������С����
*   max       (in)  -- ����������󳤶�
*   disp_mode (in)  -- ����������ʾģʽ(�ο�DISP_FORMAT����)
* RETURN:
*     INPUT_OK/INPUT_TIMEOUT/INPUT_USERABORT
* NOTE:
* -------------------------------------------------------------------------*/
sint Vx680_InputLongText(int disp_line, char * msgPrompt, char *str, usint min, usint max, int disp_mode)
{
    /*for Vx680*/
    char szKeyMask[MAX_ALPNUM_KEYS][9] =
    {
        "0- +%_", "1QZqz.\\", "2ABCabc&", "3DEFdef%",
        "4GHIghi*","5JKLjkl/", "6MNOmno~", "7PRSprs^",
        "8TUVtuv[","9WXYwxy]", "*,'\":","#=:$?@"
    };

    int    curKey = 0;
    int    keyCounter = 0;
    int    Key = 0;
    int    idx = 0;
//    int    iDispOffset = 0;
    int    len = 0;
    char   szOutputText[MMI_INPUTTEXT_MAX+4] = {0};
    int   keyoffset = 0;
    int   input_line;
    int   lineMax2show = 0;

    if ((max > MMI_INPUTTEXT_MAX) || (min > max)|| strlen(str)>max)
        return INPUT_FAILED;

    if (msgPrompt != NULL && strlen(msgPrompt) > 0)
    {
        MmiUtil_ClearLine(disp_line);
        MmiUtil_DisplayLine(msgPrompt, disp_line);
        disp_line++;
    }

    memset(szOutputText, 0, sizeof(szOutputText));
    sprintf(szOutputText, "%s", str);

    idx = strlen(szOutputText);
    // ����հ������������ṩ��buffer��ʾ��������
    lineMax2show = m_MmiUtil_Rows - disp_line;
    ++lineMax2show;

    --idx;  // �����л���ʾ�����һ���ַ���simon
    curKey = szOutputText[idx]; //
    if (curKey < bKEY0 || curKey > bKEY9)
    {
        curKey = 0;
        keyCounter = 0;
    }
    else
    {
        keyCounter = 1;
    }
    for (; ;)
    {
        len = strlen(szOutputText);
        // ��ʾbuffer
        input_line = MmiUtil_DisplayBuffer(szOutputText, disp_line,8, disp_mode);

        Key = MmiUtil_WaitKey(1);

        switch (Key)
        {
        case 0://��ʱ

            if (szOutputText[idx])
            {
                //�����ǰ�ַ���Ϊ����������һ���ַ�
                if (idx < (max - 1))
                    idx += 1;

                keyCounter = 0;
            }
            else
            {
                //���Ϊ�ռ������뵱ǰ�ַ�
                //return INPUT_TIMEOUT;
            }

            break;

        case bESC:  //ȡ��
            return INPUT_USERABORT;

        case bENTER:

            if (strlen(szOutputText) < min)
            {
                continue;
            }

            strcpy(str, szOutputText);
            return strlen(szOutputText);

        case bCLR:  //���

            if (idx > 0)
            {
                // if (iDispOffset > 0)
                if (input_line> disp_line)
                {
                    MmiUtil_ClearLines(disp_line, input_line);
                }
                else
                {
                    MmiUtil_ClearLine(input_line);
                }
            }

            idx = 0;
            memset(szOutputText, 0, sizeof(szOutputText));
            keyCounter = 0;

            break;

        case bBKSP: //�˸�

            if (szOutputText[idx])    //��ǰ�ַ�����,ɾ����ǰ�ַ�
            {
                szOutputText[idx] = 0;
            }
            else            //��ǰ�ַ�Ϊ��,ɾ��ǰһ���ַ�
            {
                if (idx)
                {
                    idx -= 1;
                    szOutputText[idx] = 0;
                }
            }

            if (idx % m_MmiUtil_Columns == 0)
            {
                MmiUtil_ClearLine(input_line);
            }

            keyCounter = 0;

            break;

        case bKEY0:
        case bKEY1:
        case bKEY2://abc
        case bKEY3://def
        case bKEY4:
        case bKEY5:
        case bKEY6:
        case bKEY7:
        case bKEY8:
        case bKEY9:
        case bSTR:
        case bPND:

            if (Key >= bKEY0 && Key <= bKEY9)
                keyoffset = Key - '0';
            else if (Key == bSTR)
                keyoffset = 10;
            else
                keyoffset = 11;

            //����0-9
            if (szOutputText[idx] == 0)
            {
                //��ǰ�ַ�Ϊ��,��Ϊ��ǰ����,��Ϊ��ǰ�����ĵ�һ���ַ�
                szOutputText[idx] = szKeyMask[keyoffset][0];
                keyCounter = 1;
            }
            else if (curKey == Key)
            {
                //����һ��������ͬ,��Ϊ��ǰ����,���л���ǰ����ֵ
                szOutputText[idx] = szKeyMask[keyoffset] \
                                    [(keyCounter % strlen(szKeyMask[keyoffset]))];
                keyCounter += 1;
                keyCounter = keyCounter % strlen(szKeyMask[keyoffset]);
            }
            else
            {
                //��ͬ����,��Ϊ��һ�������ַ�,��Ϊ��ǰ�����ĵ�һ���ַ�
                //�������,��Ҫɾ�������ַ�
                //��������Ҫɾ��1�������ַ���1�������ַ�
                while ((len + 1) > max)
                {
                    {
                        //�����ַ���ɾһ���ֽ�
                        szOutputText[len - 1] = 0;
                        len -= 1;
                    }
                }

                idx = strlen(szOutputText);
                szOutputText[idx] = szKeyMask[keyoffset][0];
                keyCounter = 1;
            }

            break;
        }

        curKey = Key;
    }

}

/************************************************************************
* FUNCTION NAME: MmiUtil_DisplayBuffer
* DESCRIPTION:��ָ������Ļ��Χ�ڣ���ʾbuffer�����buffer̫������bufferǰ��Ĳ��ֲ���ʾ
* PARAMETERS:
*   szBuffer   (in) -- Ҫ��ʾ��Buffer
*   line_start (in) -- ��ʾ��ʼ���к�
*   line_end   (in) -- ��ʾ�������к�
*   disp_mode  (in) -- ��ʾģʽ
* RETURN:
*    line_current   -- ���һ�е�λ��
* NOTES:
************************************************************************/
sint MmiUtil_DisplayBuffer( char* szBuffer, sint line_start, sint line_end, int disp_mode )
{
    usint len = 0;          // buffer ����
    usint total_lines = 0;  // һ����ʾ������
    sint offset = 0;        // buffer��offset
    sint line_current = 0; // ��ǰ��ʾ����index

    if (line_end == 0)
    {
        line_end = m_MmiUtil_Rows;
    }
    if ( line_end < line_start )
    {
        return -1;
    }
    len = strlen(szBuffer);
    // ����հ������������ṩ��buffer��ʾ��������
    // ��������bufferһ����Ҫ������ʾ
    if (len == 0 || len % m_MmiUtil_Columns)
    {
        total_lines = len / m_MmiUtil_Columns + 1;
    }
    else
    {
        total_lines = len / m_MmiUtil_Columns;
    }
    // ���������ʾ���к�
    if (line_end < (line_start + total_lines -1))
    {
        // ������Ļ����ʾ��Χ
        // �����趨��ʾ������
        total_lines = line_end-line_start+1;
    }
    else
    {
        // û�г�����Ļ��ʾ��Χ
        // �����趨��ʾ�����һ��
        line_end = line_start + total_lines -1;
    }

    // ��ǰ��Ļ��������ʾ���ַ���Ϊ������Xÿ���ַ���
    // �����еڼ����ַ���ʼ��ʾ
    offset = len - total_lines*m_MmiUtil_Columns;
    if (offset < 0 )
    {
        offset = 0;
    }
    // ��ʾ���һ����ǰ��buffer
    for (line_current = line_start; line_current < line_end; line_current++)
    {
        // ����˴�ʱ��һ����������ʾ�����ÿ�����ʾ��ʽ
        MmiUtil_ClearLine(line_current);
        MmiUtil_DisplayLine(szBuffer + offset, line_current);
        offset+=m_MmiUtil_Columns;
    }
    // ��ʾ���һ��
    MmiUtil_ClearLine(line_current);

    switch (disp_mode)
    {
    case LEFT_JST:
        MmiUtil_DisplayLine(szBuffer + offset, line_current);
        break;
    case RIGHT_JST:
        MmiUtil_DisplayRight(szBuffer + offset, line_current);
        break;
    case CENTER_JST:
    default:
        MmiUtil_DisplayCenter(szBuffer + offset, line_current, FALSE);
        break;
    }

#ifdef WIN32
    dumpbuffer( szBuffer, total_lines );
    printf("input number(X:�� , b:��, B:hold��, E:����, a: alpha):"); // only windows
#endif

    return line_current;

}


