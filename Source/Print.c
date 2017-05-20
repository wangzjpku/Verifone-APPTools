/****************************************************************************
* FILE NAME:   Print.c                            *
* MODULE NAME: Print                            *
* PROGRAMMER:  luqr                             *
* DESCRIPTION: 打印模块                           *
* REVISION:    01.00 08/18/10                       *
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
*        P R I V A T E  DEFINE              *
*===========================================*/
#define PRN_LINE_SIZE  42
#define DNLD_TO		3

extern int isGbkFont12(void);


/*==========================================*
*        P R I V A T E  D A T A            *
*==========================================*/
static open_block_t stprinter_parm;
static int m_sprocket_dot_mode;
static int h_printer;
static int inprinter_init;
static unsigned char inprinter_newprnmode;				// =1:针打新字体打印模式
static char inprinter_prthzcachestring[ 4096 ]= {0};	// 已经下载的汉字缓冲
static char inprinter_notshzstring[ 512 ]= {0};			// 不需要转换直接下载的汉字缓冲

/*=====================================================*
*   I N C L U D E  P R I V A T E  F U N C T I O N S   *
*=====================================================*/
static char inPrint_IsChinese(char *s);
static int inPrint_SearchHZ(char *str, int *totalhz);
static int inPrint_DownloadHZ(int printer_h , unsigned char *str, int totalhz);
static int inPrint_GetPrnCacheString( void );
static void inPrint_WriteStr(int hdl, unsigned char *pcString );
static void inPrint_Compress_16x16_h_only( unsigned char* buf_zip, unsigned char* buf_pft);
static void inPrint_Compress_16x16_hv( unsigned char* buf_zip, unsigned char* buf_pft);
static void inPrint_Print_English( unsigned char attrib, unsigned char *txtenglish );
static int inPrint_Download_EnglishFont_8x14(void);
static int inPrint_CheckNotshzCasheString( char *str );
static int inPrint_GetNNotshzCacheString( void );
static int inPrint_Download_Hz_font(char Tab_id, char Ch_id, unsigned char *buf_pft);
static void inPrint_Print_Hz( char *str, unsigned char attrib );
static int inPrint_p3700_print( short h_comm_port, unsigned char *pPrintString );
static short inPrint_p3700_select_font( short h_comm_port, short font_size, short font_table);
static short inPrint_p3700_dnld_graphic_file(short h_comm_port, short h_graphic_file );
static int inPrint_OpenDevice( void );
static int inPrint_CloseDev( void );
static int inPrint_CheckPrintAutoOpenStatus( void );
static int inPrint_CheckReadlyStatus( int iStatus );
static void inPrint_Feed_Vx805( int flag, int length );
static int inPrint_Print_Str_12_Vx805(char *str, unsigned char attrib);
static int inPrint_GetGB2312Offset(unsigned char h, unsigned char l);
static int inPrint_GetGBKOffset(unsigned char h, unsigned char l);
static char inPrint_IsChinese(char *s);
static int inPrint_SearchHZ(char *str, int *totalhz);
static int inPrint_DownloadHZ(int printer_h , unsigned char *str, int totalhz);
static int inPrint_CheckStatus( int iStatus );


/*=========================================*
*   P U B L I C     F U N C T I O N S     *
*=========================================*/

void Print_Log( char *pString, ... )
{
#ifdef	LOGSYS_FLAG
    static int init_status;
    struct Opn_Blk com_parm;
    char cMsgBuf[ 2048 ];
    char *pArg;
    va_list v_Arg;
    char *pComName = "/DEV/COM1";

    memset( cMsgBuf, 0, sizeof( cMsgBuf ) );

    if( pString != NULL )
    {
        pArg = strchr( pString, '%' );

        if( pArg )
        {
            va_start( v_Arg, pString );
            vsprintf( cMsgBuf, pString, v_Arg );
            va_end( v_Arg );
        }
        else
        {
            strcpy( cMsgBuf, pString );
        }
    }

    if( !strchr( cMsgBuf, '\n') )
        strcat( cMsgBuf, "\n" );

    // Output message by COM
    if( init_status != 99 )
    {
        com_parm.rate = Rt_115200;
        com_parm.format = Fmt_A8N1;
        com_parm.protocol = P_char_mode;
        com_parm.parameter = 0;
        if( ComDev_Init( pComName, &com_parm ) == COMDEV_NOHANDLE )
            return;

        init_status = 99;
    }

    ComDev_Write( pComName, cMsgBuf, strlen( cMsgBuf ) );
#endif
    return;
}


void Print_InitPrm( void )
{
    h_printer = 0;
    inprinter_init = 0;
    inprinter_newprnmode = 0;
    memset( inprinter_prthzcachestring, 0, sizeof( inprinter_prthzcachestring ) );
    memset( inprinter_notshzstring, 0, sizeof( inprinter_notshzstring ) );
    return;
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Init.
* DESCRIPTION:   初始化打印机
* PARAMETERS:
* RETURN:
*   PRINT_NOHANDLE/PRINT_OK
* NOTES:
* ------------------------------------------------------------------------ */
int Print_Init(void)
{
    Print_Log( "Print_Init......" );

    h_printer = Device_GetHandle( DEV_COM4 );
    if ( h_printer < 0 )
    {
        // LOG_PRINTF_LIB( "Print_Init, DEV_COM4 can't be used,handle=[%d]", h_printer );
        return PRINT_NOHANDLE;
    }

    memset(&stprinter_parm, 0, sizeof(stprinter_parm));
    stprinter_parm.rate      = Rt_19200;
    stprinter_parm.format    = Fmt_A8N1 | Fmt_DTR | Fmt_RTS | Fmt_auto;	//Fmt_A8N1 | Fmt_AFC | Fmt_DTR;
    stprinter_parm.protocol  = P_char_mode;
    stprinter_parm.parameter = 0;

    inprinter_newprnmode = 0;

    if ( MmiUtil_GetPrinterType() == SPROCKET_PRINTER)
    {
        if( MmiUtil_GetTerminalType() == _VX520S || MmiUtil_GetTerminalType() == _VX805 )
        {
            inprinter_newprnmode = 1;

            Print_Log( "printer for new fast printing mode!" );
        }

        // cx 141107 start
        if( m_sprocket_dot_mode == MODE_360_DOTS)
        {
            //add by 2014.10.10
            // set 360-dot mode
            stprinter_parm.trailer.bytes.b[0] = 1;

            // do home-position-seek after every 10 left+right passes
            stprinter_parm.trailer.bytes.b[1] = 20;//20141205 Set 20 or even more instead of 10.

            // shift printing area on 3 dot to left
            //stprinter_parm.trailer.bytes.b[2] = (signed char)-3;
            //end
        }
        // cx 141107 end
    }

    set_opn_blk( h_printer, &stprinter_parm );
    SVC_WAIT( 200 );

    inPrint_p3700_print( h_printer, (unsigned char *)"\x18");

    p3700_init( h_printer, 6 );
    SVC_WAIT( 100 );

    //删除打印临时文件
//  _remove(PRNCHN);

    if( inprinter_init < 100 )
    {
        inprinter_init = 100;

        if( inprinter_newprnmode == 1 )
        {
            inPrint_Download_EnglishFont_8x14();

            inPrint_GetNNotshzCacheString( );
        }
    }

    return PRINT_OK;
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Open.
* DESCRIPTION:   打开打印机
* PARAMETERS:
* RETURN:
*   PRINT_NOHANDLE/PRINT_OK
* NOTES:
*	Added by @laikey_20130111
* ------------------------------------------------------------------------ */
int Print_Open( void )
{
    open_block_t parm;

    if ( inPrint_CheckPrintAutoOpenStatus( ) == 1 )
        return PRINT_OK;

    if ( inPrint_OpenDevice( ) != TRUE )
        return PRINT_NOHANDLE;

    h_printer = Device_GetHandle( DEV_COM4 );
    if ( h_printer < 0 )
        return PRINT_NOHANDLE;

    memset(&parm, 0, sizeof(parm));
    parm.rate      = Rt_19200;
    parm.format    = Fmt_A8N1 | Fmt_auto | Fmt_RTS;
    parm.protocol  = P_char_mode;
    parm.parameter = 0;

    set_opn_blk( h_printer, &parm );
    SVC_WAIT( 200 );

    inPrint_p3700_print( h_printer, (unsigned char *)"\x18");	// clear print buffer

    p3700_init( h_printer, 6 );
    SVC_WAIT( 100 );

    return PRINT_OK;
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Close.
* DESCRIPTION:   关闭打印机
* PARAMETERS:
* RETURN:
*   PRINT_NOHANDLE/PRINT_OK
* NOTES:
*	Added by @laikey_20130111
* ------------------------------------------------------------------------ */
int Print_Close( void )
{
    int iRet;

    if ( inPrint_CheckPrintAutoOpenStatus( ) == 1 )
        return PRINT_OK;

    iRet = p3700_close( h_printer );
    if ( iRet < 0 )
    {
        SVC_WAIT( 50 );

        iRet = p3700_close( h_printer );
    }

    h_printer = 0;
    iRet = inPrint_CloseDev( );

    if ( iRet == TRUE )
        return PRINT_OK;
    else
        return PRINT_FAIL;
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_DelCache.
* DESCRIPTION:   删除打印临时文件
* PARAMETERS:	None
* RETURN:		Return value of _remove()
* NOTES:		Should be called once in MainApp
* ------------------------------------------------------------------------ */
int Print_DelCache(void)
{
    Print_Log( "Print_DelCache" );

    memset( inprinter_prthzcachestring, 0, sizeof( inprinter_prthzcachestring ) );
    _remove(PRNCHN);

    return PRINT_OK;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_PreLoadFont.
* DESCRIPTION:   下载16x16点阵字库，在Print_init后执行
* PARAMETERS:
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void Print_PreLoadFont(void)
{
    int fp;
    int i;
    unsigned long lMaxCnt;
    char Ch_id;

    if((fp = open( PRNCHN, O_RDONLY)) <= 0)
        return;

    lMaxCnt = lseek(fp, 0L, SEEK_END);

    if( lMaxCnt > sizeof(inprinter_prthzcachestring))
        lMaxCnt = sizeof(inprinter_prthzcachestring);

    lseek( fp, 0L, SEEK_SET);
    read( fp, inprinter_prthzcachestring, lMaxCnt );
    close( fp );

    Print_Log( "sting cache:%s", inprinter_prthzcachestring );

    for( Ch_id = 0, i = 0; i < lMaxCnt; i += 2 )
    {
        inPrint_DownloadHZ( h_printer, (unsigned char *)&inprinter_prthzcachestring[ i ], Ch_id );

        Print_Log( "download hz:%2.2s-%d", &inprinter_prthzcachestring[ i ], Ch_id );

        Ch_id++;
    }

    return;
}


int Print_Str(char *str, unsigned char attrib)
{
    int i;
    int offset;
    int head;
    int tail;
    int len;
    int hzmode = 0;
    int p_status = 0;
    char buf[1024];

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 0 );
    if ( p_status != PRINT_NOPAPER && p_status != PRINT_OK )
        return p_status;

    inPrint_GetPrnCacheString( );

    Print_Log( "string=%s", str );

    len = strlen( str );

    for( i = 0; i < len; i ++ )
    {
        if( inPrint_IsChinese( &str[ i ] ) )
        {
            head = inPrint_SearchHZ( &str[i], &offset );
            if( head < 0 )
            {
                inPrint_DownloadHZ( h_printer, (unsigned char *)&str[ i ], offset );

                //Print_Log( "download hz:%2.2s-%d", &str[i], offset );
            }
            else
            {
                //Print_Log( "search hz:%2.2s-%d", &str[i], head );
            }

            i++;
        }
    }

    hzmode = inPrint_IsChinese( str );

    for(head = tail = 0; tail < len; tail ++)
    {
        if( hzmode == 0 )
        {
            if( inPrint_IsChinese( str + tail))
            {
                hzmode = 1;
                memcpy( buf, str + head, tail - head);
                buf[tail-head] = 0;

                head = tail++; // Chinese has 2 bytes
                inPrint_Print_English( attrib, (unsigned char *)buf );
            }
        }
        else
        {
            if(inPrint_IsChinese(str + tail))
            {
                tail++; // Chinese has 2 bytes
            }
            else
            {
                hzmode = 0;
                memcpy(buf, str + head, tail - head);
                buf[ tail-head] = 0;

                head = tail;

                inPrint_Print_Hz( buf, attrib );
            }
        }
    }

    if( head < len )
    {
        memcpy(buf, str + head, tail - head);
        buf[tail-head] = 0;

        if( hzmode )
            inPrint_Print_Hz( buf, attrib );
        else
            inPrint_Print_English( attrib, (unsigned char *)buf );
    }

    if( inprinter_newprnmode == 0 )
    {
        if( strchr( str, '\n' ) )
            SVC_WAIT( 10 );
    }

    return 0;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME:  Print_StrStr
* DESCRIPTION:    在同一行打印2个buffer with/without justification
* PARAMETERS:
*   cLStr (in) -- 左侧的字符串
*   cRStr (in) -- 右侧的字符串
*   form (in)  -- 打印格式（参考TFormat定义）
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
void Print_StrStr(char *cLStr, char *cRStr, TFormat form)
{
    BOOL  bIsTotalASCIIFlag;
    sint i, siPrintSize;
    char tmp[64];

    siPrintSize = PRN_LINE_SIZE;
    bIsTotalASCIIFlag = FALSE;

    for (i = 0; i < strlen(cLStr); i++)
    {
        if (!isascii(cLStr[i]))
        {
            bIsTotalASCIIFlag = TRUE;
            break;
        }
    }

    for (i = 0; i < strlen(cRStr); i++)
    {
        if (!isascii(cRStr[i]))
        {
            bIsTotalASCIIFlag = TRUE;
            break;
        }
    }

    if (!bIsTotalASCIIFlag)
    {
        siPrintSize -= 1;
    }

    switch (form)
    {
    case FORMAT_PRINT:
        i = siPrintSize - strlen(cRStr);
        Print_Str(cLStr, NORMAL);

        if ((siPrintSize - strlen(cLStr)) <= strlen(cRStr))
        {
            Print_Str("\n", NORMAL);
        }
        else
        {
            i -= strlen(cLStr);
        }

        if (i > 0)
        {
            memset(tmp, 0, sizeof(tmp));
            memset(tmp, ' ', i);
            Print_Str(tmp, NORMAL);
        }

        Print_Str(cRStr, NORMAL);
        Print_Str("\n", NORMAL);
        break;
    case FORMAT_LBOLD:
        Print_Str(cLStr, DH | DW);

        if ((siPrintSize - strlen(cLStr)*2) < strlen(cRStr))
        {
            Print_Str("\n", NORMAL);
            i = siPrintSize - strlen(cRStr);
        }
        else
        {
            i = siPrintSize - strlen(cLStr) * 2 - strlen(cRStr);
        }

        if (i > 0)
        {
            memset(tmp, 0, sizeof(tmp));
            memset(tmp, ' ', i);
            Print_Str(tmp, NORMAL);
        }

        Print_Str(cRStr, NORMAL);
        Print_Str("\n", NORMAL);
        break;
    case FORMAT_RBOLD:
        Print_Str(cLStr, NORMAL);

        if ((siPrintSize - strlen(cLStr)) < strlen(cRStr)*2)
        {
            Print_Str("\n", NORMAL);
            i = siPrintSize - strlen(cRStr) * 2;
        }
        else
        {
            i = siPrintSize - strlen(cLStr) - strlen(cRStr) * 2;
        }

        if (i > 0)
        {
            memset(tmp, 0, sizeof(tmp));
            memset(tmp, ' ', i);
            Print_Str(tmp, NORMAL);
        }

        Print_Str(cRStr, DW);
        Print_Str("\n", NORMAL);
        break;
    case FORMAT_BOLD:
        Print_Str(cLStr, DH | DW);

        if ((siPrintSize - strlen(cLStr)*2) < strlen(cRStr)*2)
        {
            Print_Str("\n", NORMAL);
            i = siPrintSize - strlen(cRStr) * 2;
        }
        else
        {
            i = siPrintSize - strlen(cLStr) * 2 - strlen(cRStr) * 2;
        }

        if (i > 0)
        {
            memset(tmp, 0, sizeof(tmp));
            memset(tmp, ' ', i);
            Print_Str(tmp, NORMAL);
        }

        Print_Str(cRStr, DH | DW);
        Print_Str("\n", NORMAL);
        break;
    default:
        break;
    }

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_logo.
* DESCRIPTION:   打印lgo图片
* PARAMETERS:
*   Offset （in）-- 图片左边的空白
*   LogoFile （in）-- 图片文件名称（后缀.lgo）
* RETURN:
*   PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Print_logo(short offset, char *LogoFile)
{
    int  font_file_h;
    int  ret;
    int  i;
    int  p_status = 0;
//    static int dnld_graphic_flag;
//    static char cLogoFileName[ 126 ];

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 1 );

    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return p_status;

    if (LogoFile == NULL)
        return PRINT_OK;

//   if( !strstr( cLogoFileName, LogoFile ) )
    {
        // 下载LOGO文件
        if ((font_file_h = File_Open(LogoFile, O_RDONLY)) < 0)
        {
            return PRINT_OPENFAIL;
        }

        i = 3;

        while ( i > 0 )
        {
            ret = inPrint_p3700_dnld_graphic_file(h_printer, font_file_h);
            if (ret == 0)
                break;

            SVC_WAIT( 1000 );
            i--;
        }

        File_Close(font_file_h);

//        sprintf( cLogoFileName, "%s", LogoFile );
    }

    // 打印LOGO文件
    ret = p3700_print_graphic( h_printer, 0, offset);

    if (ret <= 0)
        return PRINT_FAIL;

    return p_status;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_bitmap.
* DESCRIPTION:   打印bitmap文件
* PARAMETERS:
*   Offset （in）-- 图片左边的空白
*   BmpFile （in）-- 图片文件名称（后缀.bmp）
* RETURN:
*   PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:打印完bmp之后，打印机的handle被关闭了。需要重新打开打印机设备并初始化。
* ------------------------------------------------------------------------ */
int Print_bitmap(short offset, char *BmpFile)
{
    int  ret;
    int  p_status = 0;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 1 );

    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return p_status;

    if (BmpFile == NULL)
        return PRINT_OK;

    ret = print_image(offset, BmpFile);
    //ApplLog_Debug("ATOOL","print image ret=[%d]", ret);

    if ( ret != 1 )
        return PRINT_FAIL;

    return p_status;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_English.
* DESCRIPTION:   打印英文字符串
* PARAMETERS:
*   english_str (in) -- 英文字符串
*   font_size (in) -- 0x02, 0x03, 0x04
* RETURN:
*   PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
*   对于热敏打印机
*   0x02字体，一行可打印24个字符
*   0x03字体，一行可打印32个字符
*   0x04字体，一行可打印42个字符
*   对于针式打印机
*   0x02字体，一行可打印30个字符
*   0x03字体，一行可打印15个字符
*   0x04字体，一行可打印20个字符
* ------------------------------------------------------------------------ */
int Print_English(char *english_str, short font_size)
{
    int  p_status = 0;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 0 );
    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return p_status;

    if( inprinter_newprnmode == 1 )
    {
        inPrint_Print_English( NORMAL, (unsigned char *)english_str );
        return PRINT_OK;
    }

    // ESC=0x1B, a22=设置行高度22
    //inPrint_p3700_print( h_printer, (unsigned char *)"\x1B""a22;");

    inPrint_p3700_select_font( h_printer, font_size, 0);
    inPrint_p3700_print( h_printer, (unsigned char *)english_str);

    return p_status;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_EngSmall.
* DESCRIPTION:   打印英文字符串
* PARAMETERS:
*   english_str （in）-- 英文字符串
* RETURN:
*   PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
*   对于热敏打印机，一行可打印42个字符
*   对于针式打印机，一行可打印30个字符
* ------------------------------------------------------------------------ */
int Print_EngSmall(char *english_str)
{
    int  p_status = 0;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 0 );
    if ( p_status != PRINT_NOPAPER && p_status != PRINT_OK )
        return p_status;

    if( inprinter_newprnmode == 1 )
    {
        inPrint_Print_English( NORMAL, (unsigned char *)english_str );
        return PRINT_OK;
    }

    if ( MmiUtil_GetPrinterType() == SPROCKET_PRINTER )
    {
        inPrint_p3700_print(h_printer, (unsigned char *)"\x1B""a22;");
        inPrint_p3700_select_font(h_printer, 0x02, 0);
    }
    else
    {
        //inPrint_p3700_print(h_printer, (unsigned char *)"\x1B""a16;");
        inPrint_p3700_select_font(h_printer, 0x04, 0);
    }

    inPrint_p3700_print(h_printer, (unsigned char *)english_str);

    return p_status;
}

/* --------------------------------------------------------------------------
 * FUNCTION NAME: Print_Hex
 * DESCRIPTION:   打印16进制数据.
 * PARAMETERS:
 *    msgPrompt (in) - 提示信息
 *    Buf       (in) - 数据
 *    BufLen    (in) - 数据长度.
 * RETURN:
 * NOTES:
 * ------------------------------------------------------------------------ */
void Print_Hex(char *msgPrompt, byte *Buf, int BufLen)
{
    char  out[128], tmp[128];
    usint i, j, k;

    Print_Str((char *)msgPrompt, 0);
    Print_Str("\n", 0);

    sprintf(tmp, "%03hu|---------Hex-------+\n", BufLen);
    Print_Str(tmp, 0);

    if ( BufLen > 1000 )
    {
        Print_Str( "BufLen is too large!\n", 0 );
        return;
    }

    j = (BufLen % 8) ? (BufLen / 8 + 1) : (BufLen / 8);

    for (i = 0; i < j; i++)
    {
        sprintf(out, "%03u| ", i*8);

        for (k = 0; k < 8; k++)
        {
            sprintf(tmp, "%02X", Buf[i*8+k]);
            strcat(out, tmp);

            if (k == 3)
            {
                strcat(out, " ");
            }
        }

        strcat(out, "\n");
        Print_Str(out, 0);
    }

    Print_Str("+----------------------+\n", 0);

    return;
}

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_FormFeed
* DESCRIPTION:    针式打印机换页指令
* PARAMETERS :
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
int Print_FormFeed(void)
{
    int p_status = 0;

    if ( MmiUtil_GetPrinterType() != SPROCKET_PRINTER)  // 热敏机型，直接返回 0
        return 0;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 1 );
    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return p_status;

    Print_Log( "form feed start" );

    //inPrint_p3700_print(h_printer, (unsigned char *)"\x18");  // clear print buffer
    inPrint_p3700_print(h_printer, (unsigned char *)"\x0B");

    //inPrint_CheckStatus( 1 );

    Print_Log( "form feed end" );
    return p_status;
}

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_SetLineHeight
* DESCRIPTION:    设置行高度
* PARAMETERS :
*     height(in) -- 行高度(default is 22, min is 16, max is 48)
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
void Print_SetLineHeight(int height)
{
    char buf[10];
    int  p_status = 0;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 1 );
    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return;

    //inPrint_p3700_print( h_printer, (unsigned char *)"\x18");  // clear print buffer

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\x1B""a%d;", height);

    inPrint_p3700_print(h_printer, (unsigned char *)buf);
}

/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_SetFormLength
* DESCRIPTION:    设置针打纸页面长度
* PARAMETERS :
*     length(in) -- 页面长度(default is 140mm, max is 300mm)
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
void Print_SetFormLength(int length)
{
    char buf[10];
    int  p_status = 0;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 1 );
    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return;

    //inPrint_p3700_print(h_printer, (unsigned char *)"\x18");  // clear print buffer

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\x1B""K%d;", length);
    inPrint_p3700_print(h_printer, (unsigned char *)buf);
}


/* -----------------------------------------------------------------------
* FUNCTION NAME:  Print_Feed
* DESCRIPTION:    针式打印机进纸/退纸指令
* PARAMETERS :
*   flag(in)   - FORWARD_PAPER:进纸/ REVERSE_PAPER:退纸
*   length(in) - 毫米
* RETURN:
* NOTES:
 * ----------------------------------------------------------------------*/
int Print_Feed(int flag, int length)
{
    int p_status = 0;
    char buf[20];

    if (flag != FORWARD_PAPER && flag != REVERSE_PAPER)
        return PRINT_OK;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 1 );
    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return p_status;

    // Added by @laikey_20130321 -->
    if ( MmiUtil_GetTerminalType( ) == _VX805 )
    {
        inPrint_Feed_Vx805( flag, length );
        return p_status;
    }
    // <--

    //inPrint_p3700_print(h_printer, (unsigned char *)"\x18");  // clear print buffer

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\x1B""b%d,%d;", length, flag);

    inPrint_p3700_print(h_printer, (unsigned char *)buf);

    if( MmiUtil_GetPrinterType() == SPROCKET_PRINTER &&
            MmiUtil_GetTerminalType() == _VX520S )
    {
        unsigned long lTrytimer;

        lTrytimer = read_ticks();
        while( 1 )
        {
            if( !inPrint_CheckReadlyStatus( 1 ) )
                break;

            if( read_ticks() - lTrytimer > 500 )
                break;
        }
    }

    Print_Log( "Print_Feed(%d,%d) send ok!", flag, length );
    return p_status;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_SetDark.
* DESCRIPTION:   设置打印黑度*DARK
* PARAMETERS:
*    darkValue(in) ----- (取值范围1-29)
* RETURN:
*		PRINT_NOHANDLE/PRINT_OK
* NOTES:
* ------------------------------------------------------------------------ */
void Print_SetDark(int darkValue)
{
    char buf[40];
    int  p_status = 0;

    // 检测打印机状态
    p_status = inPrint_CheckStatus( 1 );

    if (p_status != PRINT_NOPAPER && p_status != PRINT_OK)
        return;

    inPrint_p3700_print(h_printer, (unsigned char *)"\x18");  // clear print buffer

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\x1B""w%d;", darkValue);

    inPrint_p3700_print(h_printer, (unsigned char *)buf);
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: Print_Str_12.
* DESCRIPTION:   以图形方式打印任意中（12x12）英文（5x8）字串的函数
* PARAMETERS:
*		  str (in) -- 要打印的字符串
*		  attrib (in) -- 打印属性, 暂时不用
* RETURN:
*		PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Print_Str_12(char *str, unsigned char attrib)
{
    int len, i, j;
    short fp, fp1, retVal;
    char filename[32], tmp[4];
    char Printmode[4]= {0};
    char ioctl_status[4];
    unsigned char buf[32], linebuf[32*12];	// 12行, 每行15个字，每个字的每行
    int ret;
    long end;
    BOOL bIsGbkFont12;

    if (strlen(str) == 0)
        return PRINT_OK;

    // Added by @laikey_20130321 -->
    if ( MmiUtil_GetTerminalType() == _VX805)
    {
        return ( inPrint_Print_Str_12_Vx805( str, attrib ) );
    }
    // <--

//    strcpy(filename, CHNFONT_12);
    bIsGbkFont12 = isGbkFont12();
    if (bIsGbkFont12)
    {
        strcpy(filename, GBKFONT_12);
    }
    else
    {
        strcpy(filename, CHNFONT_12);
    }
    if ((fp = open(filename,O_RDONLY)) < 0)
        return PRINT_NOHANDLE;

    strcpy(filename, ENGFONT_5x8);
    if ((fp1 = open(filename,O_RDONLY)) < 0)
        return PRINT_NOHANDLE;

    //**************************************************************************************
    // Start the graphics mode of printing the data
    //ESC g is the command for going to the graphicsa mode
    //**************************************************************************************
    memset( Printmode, 0, 4 );
    memset( ioctl_status, 0, 4 );
    Printmode[ 0 ] = 0x1b;
    Printmode[ 1 ] = 0x67;

    while (get_port_status(h_printer,ioctl_status)!=0)
        SVC_WAIT(2);

    end = set_itimeout(-1, 2, TM_SECONDS);
    while (write(h_printer,Printmode,2) != 2)
    {
        SVC_WAIT(10);
        if (!CHK_TIMEOUT(-1, end))
        {
            close(fp);
            close(fp1);
            return 	WRITE_FAIL;
        }
    }

    //**************************************************************************************
    //Now thw bitmap will have the data in the reverse hence the last data of the bitmap
    //will be the first data hnece get the data in reverse
    //the value of the buffer is memset to 0x0c 1100 0000 which will not print data
    //Container should have 1 1			0 0 0 0 0 0
    //						| |			| | | | | |
    //					Non printing	Printing data
    //**************************************************************************************

    len = strlen( str );
    if (len > 30)	/* 只打印一行 */
        len = 30;

    memset(linebuf, 0, sizeof(linebuf));

    for (i = 0; i < 12; i ++)
    {

        memset(buf, 0xC0, sizeof(buf));

        for (j = 0; j < len; j ++ )
        {
            if (bIsGbkFont12 && (unsigned char)str[j] >= 0x81 && (unsigned char)str[j+1] >= 0x40)
            {
                lseek(fp, inPrint_GetGBKOffset(str[j], str[j+1])*24 + 128*12 + 16 + (i * 2), SEEK_SET);

                read(fp, tmp, 2);

                /* 12x12点阵每行由2个字节组成，实际只占用了12个BIT */
                buf[j] |= tmp[0] >> 2;
                buf[j + 1] |= (tmp[0] << 6) >> 2;
                buf[j + 1] |= tmp[1] >> 4;
                j ++;
            }
            else if ((unsigned char)str[j] >= 0xA1 && (unsigned char)str[j+1] >= 0xA0)
            {
                lseek(fp, inPrint_GetGB2312Offset(str[j], str[j+1])*24 + (i * 2), SEEK_SET);

                read(fp, tmp, 2);

                /* 12x12点阵每行由2个字节组成，实际只占用了12个BIT */
                buf[j] |= tmp[0] >> 2;
                buf[j + 1] |= (tmp[0] << 6) >> 2;
                buf[j + 1] |= tmp[1] >> 4;
                j ++;
            }
            else if (i > 3) //使用5x8的英文，下对齐
            {
                lseek(fp1, (8 + str[j] * 9 + 1 + i - 4), SEEK_SET);
                read(fp1, tmp, 1);

                /* 5x8点阵每行1个字节，实际占用后5个BIT */
                buf[j] |= tmp[0];
            }
        }
        buf[j] = 0x21;	/* 换行 */

        if (j == 30)
            memcpy(linebuf + i * j, buf, j);
        else
            memcpy(linebuf + i * (j + 1), buf, j + 1);
    }
    linebuf[strlen((char *)linebuf)] = 0x21;	/* 行距一个像素 */

    do
    {
        while (get_port_status(h_printer, ioctl_status)!=0)
            SVC_WAIT(0);

        retVal = write(h_printer,(char *)linebuf, strlen((const char *)linebuf));
    }
    while ((retVal<0) && (errno == ENOSPC || errno == ENXIO));

    //***********************************************************************************************
    // Exit from the graphics mode passing the 0x18 CAN command ie the 0001 0100 the fifth bit should be set to 1
    //***********************************************************************************************
    memset(Printmode,0,4);
    Printmode[0]=0x18;
    while (get_port_status(h_printer, ioctl_status)!=0)
        SVC_WAIT(0);

    ret = PRINT_OK;
    end = set_itimeout(-1, 2, TM_SECONDS);
    while (write(h_printer,Printmode,1) < 0)
    {
        SVC_WAIT(10);
        if (!CHK_TIMEOUT(-1, end))
        {
            ret = WRITE_FAIL;
            break;
        }
    }

    close(fp);
    close(fp1);
    SVC_WAIT(10);
    return ret;

}


int Print_SetSprocketDotMode(int dotmode)
{
    open_block_t parm;

    memset(&parm, 0, sizeof(parm));
    get_opn_blk( h_printer, &parm );

    if(dotmode == MODE_360_DOTS)
    {
        m_sprocket_dot_mode = MODE_360_DOTS;
        parm.trailer.bytes.b[0] = 1;
    }
    else
    {
        m_sprocket_dot_mode = MODE_180_DOTS ;
        parm.trailer.bytes.b[0] = 0;
    }

    set_opn_blk( h_printer, &parm );

    //SVC_WAIT( 100 );

    return m_sprocket_dot_mode;
}



/*=========================================*
*   P R I V A T E     F U N C T I O N S   *
*=========================================*/

static char inPrint_IsChinese(char *s)
{
    unsigned char us[3];
    memcpy(us, s, 2);

    if (isGbkFont16())	//added by baijz 20130515 for GBK
    {
        if (us[0] >= 0x81 && us[1] >= 0x40)
            return 1;
    }
    else
    {
        if (us[0] >= 0xA1 && us[1] > 0xA0)
            return 1;
    }

    return 0;
}

int inPrint_CheckNotshzCasheString( char *str )
{
    char cTmpBuf[ 3 ];

    memset( cTmpBuf, 0, sizeof( cTmpBuf ) );
    memcpy( cTmpBuf, str, 2 );

    if( strstr( inprinter_notshzstring, cTmpBuf ) )
        return 1;
    else
        return 0;
}

int inPrint_GetNNotshzCacheString( void )
{
    int fp;
    long file_size = 0;

    memset( inprinter_notshzstring, 0, sizeof( inprinter_notshzstring ) );

    if( (fp = open( "/notshz.txt", O_RDONLY ) ) <= 0 )
        return 0;

    file_size = lseek(fp, 0L, SEEK_END);

    if ( file_size > sizeof( inprinter_notshzstring ) )
        file_size = sizeof( inprinter_notshzstring );

    lseek( fp, 0L, SEEK_SET );
    read( fp, inprinter_notshzstring, file_size );
    close( fp );

    Print_Log( "not switchhz cache:%s", inprinter_notshzstring );

    return 1;
}

int inPrint_GetPrnCacheString( void )
{
    int fp;
    long file_size = 0;

    memset( inprinter_prthzcachestring, 0, sizeof( inprinter_prthzcachestring ) );

    if( (fp = open( PRNCHN, O_RDONLY ) ) <= 0 )
    {
        fp = open(PRNCHN, O_WRONLY | O_CREAT);
        close( fp );
        return 0;
    }

    file_size = lseek(fp, 0L, SEEK_END);

    if ( file_size > sizeof( inprinter_prthzcachestring ) )
        file_size = sizeof( inprinter_prthzcachestring );

    lseek( fp, 0L, SEEK_SET );
    read( fp, inprinter_prthzcachestring, file_size );
    close( fp );

    //Print_Log( "sting cache:%s", inprinter_prthzcachestring );

    return strlen( inprinter_prthzcachestring );
}

static int inPrint_SearchHZ( char *str, int *index )
{
    int i;
    int len = strlen( inprinter_prthzcachestring );

    if( len <= 0 )
    {
        *index = 0;
        return -1;
    }

    for( i = 0; i < len; i += 2 )
    {
        if( memcmp( inprinter_prthzcachestring + i, str, 2 ) == 0 )
        {
            i >>= 1;	// i /= 2;
            return i;
        }
    }

    *index = i >> 1;
    return -1;
}


int inPrint_Download_Hz_font(char Tab_id, char Ch_id, unsigned char *buf_pft)
{
    int ret, retry;
    char scmd[128];

    // select font size id and table id
    sprintf(scmd, "\x1bl1%d;", Tab_id);
    inPrint_WriteStr(h_printer, (unsigned char *)scmd);

    // prepare download font command
    scmd[1] = 'm';
    scmd[2] = Ch_id;
    scmd[35] = ';';
    memcpy(scmd + 3, buf_pft, 32);

    retry = 0;
    SVC_WAIT( 5 );

    while((ret = write(h_printer, scmd, 36)) != 36 && retry < 6)
    {
        SVC_WAIT( 200 );
        retry++;
    }

    return ret;
}


static int inPrint_DownloadHZ(int printer_h , unsigned char *str, int totalhz)
{
    int fp;
    int i, j, k, qu, wei;
    unsigned long offset;
    int Ch_id;
    unsigned char buf[ 126 ], buf1[ 126 ];
    char fontName[21];
    BOOL bIsGbkFont16;

    if ( printer_h <= 0 )
        return 0;

    bIsGbkFont16 = isGbkFont16();
    if (bIsGbkFont16)
    {
        strcpy(fontName, GBKFONT_16);
        qu = str[ 0 ] - 0x81;
        wei = str[ 1 ] - 0x40;
    }
    else
    {
        strcpy(fontName, CHNFONT);
        qu = str[ 0 ] - 0xA1;
        wei = str[ 1 ] - 0xA1;
    }
    if ( qu < 0 || wei < 0 )
        return 0;

    if( bIsGbkFont16 )
        offset = ( qu * 191 + wei + 64 ) * 32L + 16;
    else
        offset = ( qu * 94 + wei + 64 ) * 32L + 16;

    fp = open( fontName, O_RDONLY );
    if( fp < 0 )
        return 0;

    lseek( fp, offset, SEEK_SET );

    memset( buf, 0, sizeof( buf ) );
    read( fp, (char *)buf, 32 );

    close( fp );

    memcpy( buf1, buf + 8, 8);
    memcpy( buf + 8, buf + 16, 8);
    memcpy( buf + 16, buf1, 8);
    memset( buf1, 0, sizeof(buf1));

    for (k = 0; k < 32; k += 8)
    {
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
                buf1[ i + k ] |= ( (buf[ j + k ] >> i) & 0x01 ) << (7 - j);
        }
    }

    Ch_id = totalhz;
    if( inprinter_newprnmode == 0 )
    {
        inPrint_Download_Hz_font(3 + (Ch_id >> 7) * 4, Ch_id & 0x7f, buf1 );
    }
    else
    {
        if( inPrint_CheckNotshzCasheString( (char *)str ) == 1 )
        {
            inPrint_Compress_16x16_h_only( buf, buf1);

            inPrint_Download_Hz_font(3 + (Ch_id >> 7) * 4, Ch_id & 0x7f, buf );

            inPrint_Download_Hz_font(43 + (Ch_id >> 7) * 4, Ch_id & 0x7f, buf );
        }
        else
        {
            inPrint_Compress_16x16_h_only( buf, buf1 );
            inPrint_Download_Hz_font(3 + (Ch_id >> 7) * 4, Ch_id & 0x7f, buf);

            inPrint_Compress_16x16_hv(buf, buf1 );
            inPrint_Download_Hz_font(43 + (Ch_id >> 7) * 4, Ch_id & 0x7f, buf);
        }
    }

    memcpy( inprinter_prthzcachestring + strlen( inprinter_prthzcachestring ), str, 2 );

    fp = open( PRNCHN, O_RDWR );
    if( fp < 0 )
        return 0;

    lseek( fp, 0, SEEK_END );
    write( fp, (char *)str, 2 );
    close( fp );

    return 0;
}

static int inPrint_GetGB2312Offset(unsigned char h, unsigned char l)
{
    return((h-0xa1-15)*94 + (l-0xa1) + 1410); /* 1410是汉字的偏移量 */
}

static int inPrint_GetGBKOffset(unsigned char h, unsigned char l)
{
    return((h-0x81)*191 + (l-0x40));
}

static int inPrint_CheckMechFailure( int iResetFlag )
{
    int iTry = 0;
    int iFlag = iResetFlag;
    int iRet;
    char cBuffer[ 3 ];

    while( iTry++ < 5 )
    {
        if( iFlag == 1 )
        {
            reset_port_error( h_printer );

            Print_Log( "machine mechanism fail,reset_port_error and please wait 500ms" );
            SVC_WAIT( 500 );
        }

        iRet = write( h_printer, "\x1B\x64", 2 );
        if( iRet < 2 )
            break;

        memset( cBuffer, 0, sizeof( cBuffer ) );
        while( (iRet = read(h_printer, cBuffer, 1)) == 0 )
            SVC_WAIT( 10 );

        if( iRet < 0 )
            break;

        Print_Log( "check mech failure,read buffer[0]=%02x", cBuffer[ 0 ] );

        if( !(cBuffer[ 0 ] & 0x40) )
            return 1;

        iFlag = 1;
    }

    return 0;
}

static int inPrint_CheckReadlyStatus( int iStatus )
{
    int iRet;
    char cBuffer[ 3 ];

    //<ESC>d
    iRet = write(h_printer, "\x1B\x64", 2 );
    if( iRet < 2 )
        return -3;

    memset( cBuffer, 0, sizeof( cBuffer ) );
    while( (iRet = read(h_printer, cBuffer, 1)) == 0 )
        SVC_WAIT( 10 );

    if( iRet < 0 )
        return -4;

    /*cBuffer[0]=
    int prnId=80;
    int ctsReq=0;
    int paperLow=0x00;
    int paperOut=0x02;
    int mechFail=0x40;
    int ramErr=0;
    int firmwareCorrupt=0x04;
    */

    Print_Log( "in printing, read buffer=[%X]", cBuffer[0] );

    if( cBuffer[ 0 ] & 0x40 )
    {
        inPrint_CheckMechFailure( 1 );
        //return 0;
    }

    if( cBuffer[ 0 ] & 0x02 )
    {
        inPrint_CheckMechFailure( iStatus );

        Print_Log( "paper out" );

        MmiUtil_ClearLines(LINE3, LINE4);
        MmiUtil_DisplayCenter("打印机缺纸", LINE3, FALSE);
        MmiUtil_DisplayCenter("请装纸后按键继续", LINE4, FALSE);
        get_char();
        MmiUtil_UserClr(LINE3);
        MmiUtil_DisplayCenter("打印中", LINE3, FALSE);
        //return 0;
    }

    if( cBuffer[ 0 ] & 0x01 )
        return 1;

    return 0;
}

static int inPrint_CheckStatus( int iStatus )
{

    {
        int i;

        i = 0;
        while( inPrint_CheckReadlyStatus( iStatus ) )
        {
            if( iStatus != 1 )
                break;

            SVC_WAIT( 100 );

            if( i++ > 50 )
                break;
        }

        return PRINT_OK;
    }
#if 0
    else
    {
        int p_status;
        int ret_status = 0;

        if( h_printer <= 0 )
            return PRINT_NOHANDLE;

        p_status = p3700_status( h_printer, 2 );

        Print_Log( "p_status=%d", p_status );

        // Added by @laikey_20131127 -->
        if ( p_status == -20 )
        {
            long end;

            end = set_itimeout( -1, 5, TM_SECONDS );

            while ( CHK_TIMEOUT( -1, end ) )
            {
                reset_port_error( h_printer );

                SVC_WAIT( 500 );

                p_status = p3700_status( h_printer, 2 );
                if ( p_status != -20 )
                    break;
            }
        }
        // <--

        // 增加返回代码的判断提示	@laikey_20130109
        switch ( p_status )
        {
        case -4:
            break;
        case -5:
            MmiUtil_ClearLine(LINE4);
            MmiUtil_DispEnglish( "PAPER LOW", 1, LINE8 );
            get_char();
            break;
        case -10:
            MmiUtil_ClearLine(LINE4);
            MmiUtil_DispEnglish( "PRINTER RAM ERROR", 1, LINE8 );
            get_char();
            break;
        case -20:
            MmiUtil_ClearLine(LINE4);
            MmiUtil_DispEnglish( "PRINTER FAILURE", 1, LINE8 );
            get_char();
            break;
        case -21:
            ret_status = PRINT_NOPAPER;
            MmiUtil_ClearLines(LINE3, LINE4);
            MmiUtil_DisplayCenter("打印机缺纸", LINE3, FALSE);
            MmiUtil_DisplayCenter("请装纸后按键继续", LINE4, FALSE);
            get_char();
            MmiUtil_UserClr(LINE3);
            MmiUtil_DisplayCenter("打印中", LINE3, FALSE);
            break;
        case -23:
            MmiUtil_ClearLine(LINE4);
            MmiUtil_DispEnglish( "Escape sequence not found", 1, LINE8 );
            get_char();
            break;
        case -24:
            MmiUtil_ClearLine(LINE4);
            MmiUtil_DispEnglish( "PRINTER NOT INIT", 1, LINE8 );
            get_char();
            break;
        case -27:
            MmiUtil_ClearLine(LINE4);
            MmiUtil_DispEnglish( "PRINTER FW CORRUPT", 1, LINE8 );
            get_char();
            break;
        default:
            break;
        }

        return ret_status;
    }
#endif
}

#if 0
static int inPrint_CheckFinishStatus( void )
{
    int paperOut = 0x02;
    int mechFail = 0x40;
    int firmwareCorrupt = 0x04;
    int iRet;
    unsigned long lTimer;
    char cBuffer[ 3 ];

    lTimer = read_ticks();
    while( read_ticks() - lTimer < 10000 )
    {
        iRet = write( h_printer, "\x1B\x64", 2 );
        if( iRet < 2 )
            return -3;

        memset( cBuffer, 0, sizeof( cBuffer ) );
        while( (iRet = read(h_printer, cBuffer, 1)) == 0 )
            SVC_WAIT( 10 );

        if( iRet < 0 )
            return -4;

        Print_Log( "check finish status:%02X", cBuffer[ 0 ] );

        if( cBuffer[ 0 ] & 0x01 )
            return 1;

        // check for a mechanism failure
        if(( cBuffer[0] & mechFail) != 0 )
        {
            // wait 500ms and try again
            // add here...
            return PRINT_FAIL;
        }

        if(( cBuffer[0] & firmwareCorrupt) != 0 )
            return PRINT_FAIL;

        if(( cBuffer[0] & paperOut) != 0 )
        {
            MmiUtil_ClearLines(LINE3, LINE4);
            MmiUtil_DisplayCenter("打印机缺纸", LINE3, FALSE);
            MmiUtil_DisplayCenter("请装纸后按键继续", LINE4, FALSE);
            get_char();
            MmiUtil_UserClr(LINE3);
            MmiUtil_DisplayCenter("打印中", LINE3, FALSE);
            return PRINT_NOPAPER;
        }
    }

    return 0;
}
#endif

//---------------------------------------------------

// 快速连续打印可能导致打印乱码, 增加判断若获取字库表或者打印失败,暂停50ms，然后再尝试一次
// @laikey_20130109 -->

static int inPrint_p3700_print( short h_comm_port, unsigned char *pPrintString )
{
#if 1
    int iRet;

    iRet = p3700_print( h_comm_port, pPrintString );
    if ( iRet < 1 )
    {
        SVC_WAIT( 50 );

        iRet = p3700_print( h_comm_port, pPrintString );
    }

    return iRet;
#else
    inPrint_WriteStr( h_comm_port, pPrintString );
    return 0;
#endif
}


static short inPrint_p3700_select_font( short h_comm_port, short font_size, short font_table)
{
    int iRet;

    iRet = p3700_select_font( h_comm_port, font_size, font_table);
    if ( iRet < 1 )
    {
        SVC_WAIT( 50 );

        iRet = p3700_select_font( h_comm_port, font_size, font_table);
    }

    return iRet;
}


static short inPrint_p3700_dnld_graphic_file(short h_comm_port, short h_graphic_file )
{
    int iRet;

    iRet = p3700_dnld_graphic_file( h_comm_port, h_graphic_file );
    if ( iRet < 1 )
    {
        // deleted it for print delay, @laikey
        //SVC_WAIT( 50 );

        //iRet = p3700_dnld_graphic_file( h_comm_port, h_graphic_file );
    }

    return iRet;
}

static int inPrint_OpenDevice( void )
{
    int iOwnner = 0;
    int iHandle = 0;
    int tryTime = 0;

    do
    {
        iHandle = get_owner( DEV_COM4, &iOwnner);

        if ( iHandle < 0 )
            return iHandle;

        if (iOwnner != 0 && iOwnner != get_task_id())
        {
            return FALSE;
        }

        if (iOwnner == get_task_id()) //the device is used by myself
        {
            return TRUE;
        }

        iHandle = open( DEV_COM4, 0);
    }
    while (iOwnner != get_task_id() && tryTime++ < 3);

    return TRUE;
}


static int inPrint_CloseDev( void )
{
    int iOwnner = 0;
    int iHandle = 0;
    int tryTime = 0;

    do
    {
        iHandle = get_owner( DEV_COM4, &iOwnner);
        if ( iHandle < 0 )
        {
            return iHandle;
        }

        if ( iOwnner != 0 && iOwnner != get_task_id())
        {
            return FALSE ;
        }

        if ( iOwnner == 0 )
            break;

        close( iHandle );
    }
    while ( iOwnner != 0 && tryTime++ < 3);

    return TRUE;
}


static int inPrint_CheckPrintAutoOpenStatus( void )
{
    int iRet = 1;
    int iCurrGID = get_group( );
    char cBuffer[ 6 ];

    set_group( 15 );

    memset( cBuffer, 0, sizeof( cBuffer ) );
    get_env( "*AOPRTDEV", cBuffer, sizeof( cBuffer) );

    set_group( iCurrGID );

    if ( strlen( cBuffer ) > 0 )
        iRet = atoi( cBuffer );

    return iRet;
}

// <-- @laikey_20130109


// Added by @laikey_20130321 for Vx805 printer -->

static void inPrint_WaitPrinter( int inHandle )
{
    char cTmpBuf[ 5 ];
    int  iRet;
    unsigned long lTrytimer;

    lTrytimer = set_itimeout( -1, 10, TM_SECONDS );

    do
    {
        memset( cTmpBuf, 0x00, sizeof( cTmpBuf ) );

        iRet = get_port_status( inHandle, cTmpBuf );

        if ((iRet == 0) && (cTmpBuf[0] == 0) && (cTmpBuf[1] == 0) &&(cTmpBuf[2] == 0x1E))
            break;

        SVC_WAIT( 20 );

    }
    while ( (iRet >= 0) && (CHK_TIMEOUT( -1, lTrytimer)) );
}


static int inPrint_SafeWrite(int iHandle, char *pWriteData, int iDataLength )
{
    int bytesOut;
    int iLen;
    unsigned long lTrytimer;
    //char cStsBuf[ 5 ];

    lTrytimer = set_itimeout( -1, 10, TM_SECONDS );

#if 1
    iLen = 0;
    while( CHK_TIMEOUT( -1, lTrytimer ) )
    {
        errno = 0;
        bytesOut = write( iHandle, pWriteData + iLen, iDataLength - iLen );

        //while( get_port_status( iHandle, cStsBuf ) != 0);   // Flush all data in COM4
        //SVC_WAIT( 2 );

        if( bytesOut >= iDataLength || iLen >= iDataLength )
            break;

        Print_Log( "safe write length=%d,%d%d, errno=%d", iDataLength, bytesOut, iLen, errno );

        if( bytesOut <= 0 )
        {
            if( errno == ENOSPC || errno == ENXIO)
            {
                errno = 0;
                SVC_WAIT( 20 );
            }
        }
        else
        {
            iLen += bytesOut;
            SVC_WAIT( 10 );
        }
    }

    return iLen;
#else
    do
    {
        errno = 0;

        bytesOut = write( iHandle, pWriteData, iDataLength );

        //while( get_port_status( iHandle, cStsBuf ) != 0);   // Flush all data in COM4
        //SVC_WAIT( 2 );

        if ( bytesOut != iDataLength )
        {
            if( errno == ENOSPC || errno == ENXIO)
            {
                errno = 0;
                SVC_WAIT( 20 );
            }
            else if( bytesOut > 0 )
                break;
        }
        else
            break;
    }
    while ( (bytesOut != iDataLength) && (CHK_TIMEOUT( -1, lTrytimer)) );

    return bytesOut;
#endif


}

/* --------------------------------------------------------------------------
* FUNCTION NAME: inPrint_WriteStr.
* DESCRIPTION:   打印任意中英文字串的函数
* PARAMETERS:
*     str (in) -- 要打印的字符串
*     attrib (in) -- 打印属性
* RETURN:
*   PRINT_OK/PRINT_NOHANDLE/PRINT_NOPAPER/PRINT_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
static void inPrint_WriteStr( int hdl, unsigned char *pcString )
{
#if 0
    int iLen, iRet;
    unsigned long lTrytimer;

    iLen = strlen( (char *)pcString );
    iRet = write( hdl, (char *)pcString, iLen );

    lTrytimer = set_itimeout( -1, 3, TM_SECONDS );

    while( (iRet != iLen) && (CHK_TIMEOUT(-1, lTrytimer)) )
    {
        SVC_WAIT(50);
        iRet = write( hdl, (char *)pcString, iLen );
    }
#else
    inPrint_SafeWrite( hdl, (char *)pcString, strlen( (char *)pcString ) );
#endif
}


static int inPrint_Write_Vx805(int iHandle, char *pWriteData, int iDataLength )
{
    int bytesOut;
    unsigned long lTrytimer;

    lTrytimer = set_itimeout( -1, 10, TM_SECONDS );

    do
    {
        errno = 0;

        bytesOut = write( iHandle, pWriteData, iDataLength );

        if ( bytesOut != iDataLength )
        {
            if ( errno == ENOSPC )
            {
                errno = 0;
                SVC_WAIT( 20 );
            }
            else
                break;
        }
        else
            break;
    }
    while ( (bytesOut != iDataLength) && (CHK_TIMEOUT( -1, lTrytimer)) );

    return bytesOut;
}


static void inPrint_Feed_Vx805( int flag, int length )
{
    char cBuffer[ 20 ];
    int  iRet;

    if ( (flag != FORWARD_PAPER) && (flag != REVERSE_PAPER) )
        return;

    inPrint_WaitPrinter( h_printer );
    cBuffer[ 0 ] = 0x18;
    inPrint_Write_Vx805( h_printer, cBuffer, 1 );

    memset(cBuffer, 0, sizeof(cBuffer));
    iRet = sprintf(cBuffer, "\x1B""b%d,%d;", length, flag);

    inPrint_Write_Vx805( h_printer, cBuffer, iRet);
    return;
}



static int inPrint_Print_Str_12_Vx805(char *pPrintString, unsigned char attrib)
{
    int i, j;
    int iRet;
    int iLength;
    short fp, fp1;
    char cTmpBuf[ 4 ];
    char cPrintMode[ 4 ]= {0};
    unsigned char cBuffer[ 32 ], cBuffer1Line[ 32 * 12 ];	// 12行, 每行15个字，每个字的每行
    BOOL bIsGbkFont12;

    iLength = strlen(pPrintString);
    if (iLength == 0)
        return PRINT_OK;

    bIsGbkFont12 = isGbkFont12();

    //strcpy(filename, CHNFONT_12);
    if (bIsGbkFont12)
    {
        if ((fp = open(GBKFONT_12,O_RDONLY)) < 0)
            return PRINT_NOHANDLE;
    }
    else
    {
        if ((fp = open(CHNFONT_12,O_RDONLY)) < 0)
            return PRINT_NOHANDLE;
    }

    if ((fp1 = open(ENGFONT_5x8,O_RDONLY)) < 0)
        return PRINT_NOHANDLE;

    //**************************************************************************************
    // Start the graphics mode of printing the data
    //ESC g is the command for going to the graphicsa mode
    //**************************************************************************************
    cPrintMode[0]=0x1b;
    cPrintMode[1]=0x67;

    inPrint_WaitPrinter(h_printer);

    iRet = inPrint_Write_Vx805(h_printer, cPrintMode,2);
    if (iRet != 2)
    {
        close(fp);
        close(fp1);
        return WRITE_FAIL;
    }

    //**************************************************************************************
    //Now thw bitmap will have the data in the reverse hence the last data of the bitmap
    //will be the first data hnece get the data in reverse
    //the value of the buffer is memset to 0x0c 1100 0000 which will not print data
    //Container should have 1 1			0 0 0 0 0 0
    //						| |			| | | | | |
    //					Non printing	Printing data
    //**************************************************************************************

    //iLength = strlen(pPrintString);
    if (iLength > 30)	// 只打印一行
        iLength = 30;

    memset( cBuffer1Line, 0, sizeof( cBuffer1Line ) );

    for (i = 0; i < 12; i ++)
    {
        memset(cBuffer, 0xC0, sizeof( cBuffer ) );

        for (j = 0; j < iLength; j ++ )
        {
            if (bIsGbkFont12 && (unsigned char)pPrintString[j] >= 0x81 && (unsigned char)pPrintString[j+1] >= 0x40)
            {
                lseek(fp, inPrint_GetGBKOffset(pPrintString[j], pPrintString[j+1])*24 + 128*12 + 16 + (i * 2), SEEK_SET);

                read(fp, cTmpBuf, 2);

                /* 12x12点阵每行由2个字节组成，实际只占用了12个BIT */
                cBuffer[j] |= cTmpBuf[0] >> 2;
                cBuffer[j + 1] |= (cTmpBuf[0] << 6) >> 2;
                cBuffer[j + 1] |= cTmpBuf[1] >> 4;
                j ++;
            }
            else if ((unsigned char)pPrintString[j] >= 0xA1 && (unsigned char)pPrintString[j+1] >= 0xA0)
            {
                lseek(fp, inPrint_GetGB2312Offset(pPrintString[j], pPrintString[j+1])*24 + (i * 2), SEEK_SET);

                read(fp, cTmpBuf, 2);

                /* 12x12点阵每行由2个字节组成，实际只占用了12个BIT */
                cBuffer[j] |= cTmpBuf[0] >> 2;
                cBuffer[j + 1] |= (cTmpBuf[0] << 6) >> 2;
                cBuffer[j + 1] |= cTmpBuf[1] >> 4;
                j ++;
            }
            else if (i > 3) //使用5x8的英文，下对齐
            {
                lseek(fp1, (8 + pPrintString[j] * 9 + 1 + i - 4), SEEK_SET);
                read(fp1, cTmpBuf, 1);

                // 5x8点阵每行1个字节，实际占用后5个BIT
                cBuffer[j] |= cTmpBuf[0];
            }
        }
        cBuffer[j] = 0x21;	// 换行

        if (j == 30)
            memcpy(cBuffer1Line + i * j, cBuffer, j);
        else
            memcpy(cBuffer1Line + i * (j + 1), cBuffer, j + 1);
    }

    //Avoid calling strlen two times
    iLength = strlen((char *)cBuffer1Line);
    cBuffer1Line[iLength++] = 0x21;	// 行距一个像素

    inPrint_Write_Vx805( h_printer,(char *)cBuffer1Line, iLength );

    //***********************************************************************************************
    // Exit from the graphics mode passing the 0x18 CAN command ie the 0001 0100 the fifth bit should be set to 1
    //***********************************************************************************************
    cPrintMode[0]=0x18;

    inPrint_WaitPrinter(h_printer);

    inPrint_Write_Vx805(h_printer, cPrintMode,1);

    close( fp );
    close( fp1 );
    return PRINT_OK;

}

// <-- @laikey_20130321


/*
static void inPrint_Compress_16x16_v_only( unsigned char* buf_zip, unsigned char* buf_pft)
{
    int i, k;
    char bmask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    char tmpbuf[ 32 ];

    memset( tmpbuf, 0, sizeof( tmpbuf ) );

    // Shrink vertical adjacent dots
    for(k = 0; k < 32; k += 2)
    {
        tmpbuf[k] = 0;
        tmpbuf[k+1] = 0;

        for(i = 7; i >= 0; i--)
        {
            tmpbuf[k+1] += (buf_pft[k] | buf_pft[k+1]) & bmask[i];
        }
    }

    memcpy( buf_zip, tmpbuf, 32 );
}*/

static void inPrint_Compress_16x16_h_only( unsigned char* buf_zip, unsigned char* buf_pft)
{
    int i, k;
    unsigned char tmp, bmask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    unsigned char tmpbuf[ 32 ];

    memset( tmpbuf, 0, sizeof( tmpbuf ) );

    // Shrink horizontal adjacent dots
    for(k = 0; k < 8; k++)
    {
        // Process from top left
        for(tmp = 0, i = 7; i >= 0; i--)
        {
            if(buf_pft[k] & bmask[i])
                tmp += bmask[i--];
        }

        tmpbuf[k] = tmp;

        // Process top right
        for(tmp = 0, i = (tmpbuf[k] & 0x01) ? 6 : 7; i >= 0; i--)
            if(buf_pft[k+8] & bmask[i])
                tmp += bmask[i--];

        tmpbuf[k+8] = tmp;

        // Process  bottom left
        for(tmp = 0, i = 7; i >= 0; i--)
            if(buf_pft[k+16] & bmask[i])
                tmp += bmask[i--];

        tmpbuf[k+16] = tmp;

        // Process bottom right
        for(tmp = 0, i = (tmpbuf[k+16] & 0x01) ? 6 : 7; i >= 0; i--)
            if(buf_pft[k+24] & bmask[i])
                tmp += bmask[i--];

        tmpbuf[k+24] = tmp;
    }

    memcpy( buf_zip, tmpbuf, 32 );
}

static void inPrint_Compress_16x16_hv( unsigned char* buf_zip, unsigned char* buf_pft)
{
    int i, k;
    unsigned char bmask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    unsigned char tmpbuf[ 32 ];

    memset( tmpbuf, 0, sizeof( tmpbuf ) );

    // Shrink vertical adjacent dots
    for(k = 0; k < 32; k += 2)
    {
        tmpbuf[k] = 0;
        tmpbuf[k+1] = 0;

        for(i = 7; i >= 0; i--)
        {
            tmpbuf[k+1] += (buf_pft[k] | buf_pft[k+1]) & bmask[i];
        }
    }

    // Shrink horizontal adjacent dots
    inPrint_Compress_16x16_h_only(buf_zip, tmpbuf);
}

static int inPrint_Download_EnglishFont_8x14(void)
{
    int i, j, retry, code, cnt;
    int fp;
    char buf[4096], scmd[128];
    long file_size = 0;

    if((fp = open( ENGFONT_8x14, O_RDONLY)) <= 0)
    {
        MmiUtil_Warning( "英文8X14文件未找到" );
        return -1;
    }

    file_size = lseek(fp, 0L, SEEK_END);

    if(file_size > sizeof(buf))
        file_size = sizeof(buf);

    lseek(fp, 0L, SEEK_SET);
    read(fp, buf, file_size);

    // Check the font file is with correct format
    if(strcmp(buf, "I\x1b\x6c") != 0)
        return -2;

    inPrint_WriteStr(h_printer, (unsigned char *)"\x1bl31;"); //<ESC>l31; Select 8x14 font table 1
    memset(scmd, 0, sizeof(scmd));
    scmd[0] = 0x1b;
    scmd[1] = 'm';
    scmd[17] = ';';
    i = 8;
    code = 0x21;
    cnt = 0;

    while(i < file_size  &&  code < 0x7f)
    {
        scmd[2] = buf[i++];

        for(j = 3; j < 16; j += 2)
        {
            if(buf[i] == 0x7f)
                buf[i] = 0;

            scmd[j] = buf[i++];
        }

        retry = 0;
        SVC_WAIT(5);

        while(write(h_printer, scmd, 18) != 18 && retry < 6)
        {
            retry++;
        }

        i++;
        code = buf[i];
        cnt++;
    }

    Print_Log( "download english font 8x14 OK!" );
    SVC_WAIT( 200 );
    return cnt;
}

static void inPrint_Print_Hz( char *str, unsigned char attrib )
{
    int  i, scLen, hzLen, ccode, index;
    char cmdTable[16], scmd[2048];
    int  idTable = 0, newTab;

    hzLen = strlen(str);
    strcpy(scmd, "\x1bH");
    scLen = 2;

    for(i = 0; i < hzLen; i += 2, scLen += 2)
    {
        ccode = inPrint_SearchHZ( &str[i], &index );
        if( ccode < 0 )
        {
            Print_Log( "search_hz:%2.2s not found in cache stings", &str[ i ] );
            continue;
        }

        if( inprinter_newprnmode == 0 )
            newTab = (ccode >> 7)*4 + 3;
        else
        {
            if( (attrib & DH) || (attrib & DW ) )
                newTab = (ccode >> 7)*4 + 3;	// h only
            else
                newTab = (ccode >> 7)*4 + 43;	// hv both
        }

        if( newTab != idTable )
        {
            // before switch new table, write out previous string
            if( scLen > 2)
            {
                strcat( scmd, ";" );
                inPrint_WriteStr( h_printer, (unsigned char *)scmd );
                scmd[2] = '\0';
                scLen = 2;
            }

            idTable = newTab;
            sprintf(cmdTable, "\x1bl%d%d;", 1, idTable);
            inPrint_WriteStr(h_printer, (unsigned char *)cmdTable);
        }

        sprintf(scmd + scLen, "%02X", (ccode & 0x7f));
    }

    strcat (scmd, ";");
    inPrint_WriteStr( h_printer, (unsigned char *)scmd );
    return;
}

static void inPrint_Print_English( unsigned char attrib, unsigned char *txtenglish )
{
    int i = 0;
    unsigned char attribuf[ 16 ];

    if( inprinter_newprnmode == 0 )
    {
        p3700_select_font( h_printer, 0x04, 0 );

        if( (attrib & DH) || (attrib & DW) )
        {
            i = 0;
            memset( attribuf, 0, sizeof( attribuf ) );

            if( attrib & INV )
                attribuf[ i++ ] = 0x12;

            if( (attrib & DH) || (attrib & DW) )
            {
                memcpy( attribuf + i, "\x1b\x66\x30\x31\x3b", 5 );
                i += 5;
            }

            if( attrib & DW )
                attribuf[ i ++ ] = 0x1e;

            inPrint_WriteStr( h_printer, attribuf );
        }

        inPrint_WriteStr( h_printer, txtenglish );
        return;
    }

    // for spt new printer mode
    if( (attrib & DH) || (attrib & DW) || (attrib & INV) )
    {
        if( stprinter_parm.trailer.bytes.b[ 0 ] == 1 )	// 360 dot mode
            inPrint_WriteStr(h_printer, (unsigned char *)"\x1bl40;");	// tall alpha/numeric
        else
            inPrint_WriteStr(h_printer, (unsigned char *)"\x1bl20;");	// short alpha/numeric

        i = 0;
        memset( attribuf, 0, sizeof( attribuf ) );

        if( attrib & INV )
            attribuf[ i++ ] = 0x12;	// INVERSE;

        if( attrib & DH )
        {
            memcpy( attribuf + i, "\x1b\x66\x30\x31\x3b", 5 );	// DBL_HEIGHT;
            i += 5;
        }

        if( attrib & DW )
            attribuf[ i ++ ] = 0x1e;	// DBL_WIDTH;

        inPrint_WriteStr( h_printer, attribuf );
    }
    else
    {
        if( stprinter_parm.trailer.bytes.b[0] == 1 )	// 360 dot mode
            inPrint_WriteStr( h_printer, (unsigned char *)"\x1bl41;");
        else
            inPrint_WriteStr( h_printer, (unsigned char *)"\x1bl20;");
    }

    inPrint_WriteStr( h_printer, txtenglish );

    return;
}

