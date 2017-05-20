/****************************************************************************
* FILE NAME:   BankUtil.c                                                   *
* MODULE NAME: BankUtil                                                     *
* PROGRAMMER:  lanpw                                                    *
* DESCRIPTION:                                                             *
* REVISION:    01.00 09/11/10                                              *
****************************************************************************/

/*==========================================*
*             I N C L U D E S              *
*==========================================*/
#include "AppTools.h"

/*=====================================================*
*   I N C L U D E  P R I V A T E  F U N C T I O N S   *
*=====================================================*/
/* 私有函数声明 */
unsigned int strlcpy(char *dst, const char *src, unsigned int siz);

/*=========================================*
*   P U B L I C     F U N C T I O N S     *
*=========================================*/


/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_InputDate
* DESCRIPTION:   日期输入IN ASC (MMDD)
* PARAMETERS:    msgPrompt - message for prompting,
*                buf    - [in] original string, [out] return string,
*                MaxLen - maximum digits for entering.
* RETURN:        TRUE - amount entered / FALSE - user break.
* NOTES:         buf will be terminated by '\0'
* ------------------------------------------------------------------------ */
boolean BankUtil_InputDate(int disp_line, char *msgPrompt, char *buf, usint MaxLen)
{
    byte tmp[6], tmp1[6], sdate[10];
    ulint ldate = TimeUtil_GetDate();

    while (TRUE)
    {
        memset(tmp, 0, sizeof(tmp));

        if (MmiUtil_InputNum(disp_line, (char *)msgPrompt, (char *)tmp, MaxLen, MaxLen, 2) <= 0)
        {
            return FALSE;
        }

        if (strlen((const char*)tmp) != 4)
            continue;

        memset(sdate, 0, sizeof(sdate));
        sprintf((char *)sdate, "%d%s", ldate / 10000, tmp);
        /*BankUtil_Asc2Bcd(tmp, tmp1, 4);*/
        memset(tmp1, 0, sizeof(tmp1));
        BankUtil_Asc2Bcd(sdate, tmp1, 8);

        if (TimeUtil_CheckDate((byte*)tmp1) == FALSE)
            continue;

        break;
    }

    memcpy(buf, tmp, 4);

    return TRUE;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Bcd2Asc.
* DESCRIPTION:   Convert BCD code to ASCII code.
* PARAMETERS:    BcdBuf - BCD input buffer,
*                AscBuf - converted result, terminated by '\0',
*                Len - double length of BCD code, must be even.
* RETURN:        T - success / F - failed(not legal BCD code buffer).
* NOTES:
* ------------------------------------------------------------------------ */
boolean BankUtil_Bcd2Asc(byte *BcdBuf, byte *AscBuf, usint Len)
{
    sint  i;

    for (i = 0; i < Len; i++)
    {
        AscBuf[i] = (i % 2) ? (BcdBuf[i/2] & 0x0f) : ((BcdBuf[i/2] >> 4) & 0x0f);
        AscBuf[i] += ((AscBuf[i] > 9) ? ('A' - 10) : '0');
    }

    return TRUE;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Asc2Bcd.
* DESCRIPTION:   Convert ASCII code to BCD code.
* PARAMETERS:    AscBuf - Ascii input buffer, must ended by '\0'
*                BcdBuf - converted result
*                Len - double length of BCD code, should be even.
* RETURN:        T - success / F - failed(illegal char in ASCII buffer).
* NOTES:         support 'A'-'F' convertion.
* ------------------------------------------------------------------------ */
boolean BankUtil_Asc2Bcd(byte *AscBuf, byte *BcdBuf, usint Len)
{
    sint  i;
    byte  str[2];

    memset(str, 0, sizeof(str));

    for (i = 0; i < Len; i += 2)
    {
        if ((AscBuf[i] >= 'a') && (AscBuf[i] <= 'f'))
        {
            str[0] = AscBuf[i] - 'a' + 0x0A;
        }
        else if ((AscBuf[i] >= 'A') && (AscBuf[i] <= 'F'))
        {
            str[0] = AscBuf[i] - 'A' + 0x0A;
        }
        else if (AscBuf[i] >= '0')
        {
            str[0] = AscBuf[i] - '0';
        }
        else
        {
            str[0] = 0;
        }

        if ((AscBuf[i+1] >= 'a') && (AscBuf[i+1] <= 'f'))
        {
            str[1] = AscBuf[i+1] - 'a' + 0x0A;
        }
        else if ((AscBuf[i+1] >= 'A') && (AscBuf[i+1] <= 'F'))
        {
            str[1] = AscBuf[i+1] - 'A' + 0x0A;
        }
        else if (AscBuf[1] >= '0')
        {
            str[1] = AscBuf[i+1] - '0';
        }
        else
        {
            str[1] = 0;
        }

        BcdBuf[i/2] = (str[0] << 4) | (str[1] & 0x0F);
    }

    return TRUE;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_ZipNumber.
* DESCRIPTION:   Convert ASCII number to BCD code, ended by 'F'.
* PARAMETERS:    AscBuf - Asc input buffer,
*                BcdBuf - converted result, terminated by 'F'
*                Len - Max length of AscBuf
* RETURN:
* NOTES:
* ------------------------------------------------------------------------ */
void BankUtil_ZipNumber(char *AscBuf, byte *BcdBuf, usint Len)
{
    if (Len % 2)
    {
        BankUtil_Asc2Bcd((byte *)AscBuf, BcdBuf, Len + 1);
        BcdBuf[(Len-1)/2] |= 0x0F;
    }
    else
        BankUtil_Asc2Bcd((byte *)AscBuf, BcdBuf, Len);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_UnzipNumber.
* DESCRIPTION:   Convert BCD code to ASCII number, AscBuf end with '\0'
*                when meet 'F'.
* PARAMETERS:    AscBuf - Ascii input buffer, should ended by '\0'
*                BcdBuf - converted result, right-filled with 'F',
*                Len - Max length of AscBuf
* RETURN:
* NOTES:         support 'A'-'F' convertion.
* ------------------------------------------------------------------------ */
void BankUtil_UnzipNumber(byte *BcdBuf, char *AscBuf, usint Len)
{
    if (Len % 2)
    {
        BankUtil_Bcd2Asc(BcdBuf, (byte *)AscBuf, Len + 1);
        AscBuf[Len] = 0;
    }
    else
        BankUtil_Bcd2Asc(BcdBuf, (byte *)AscBuf, Len);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Amt2Asc.
* DESCRIPTION:   Convert amount to ASCII string. result will be terminated
*                by '\0', and will be left-filled with '0'.
* PARAMETERS:    amt - input amount,
*                AscBuf - converted result, left-filled with '0',
*                AscLen - ASCII string length.
* RETURN:        none.
* NOTES:
* ------------------------------------------------------------------------ */
void BankUtil_Amt2Asc(ulint amt, byte *AscBuf, usint AscLen)
{
    char    format[16];

    memset(format, 0, sizeof(format));
    sprintf((char*)format, "%c0%lulu", '%', (ulint)AscLen);
    sprintf((char*)AscBuf, format, amt);

    return;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Asc2Amt.
* DESCRIPTION:   Convert ASCII string to amount
* PARAMETERS:    amt - output amount,
*                AscBuf - input
*                AscLen - ASCII string length.
* RETURN:        none.
* NOTES:
* ------------------------------------------------------------------------ */
void BankUtil_Asc2Amt(ulint *amt, byte *AscBuf, usint AscLen)
{
    char  buf[20];

    memset(buf, 0, sizeof(buf));
    memcpy((char*)buf, (char*)AscBuf, AscLen);
    *amt = atol(buf);

    return;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Len2Bcd
* DESCRIPTION:   Convert usint to n bytes BCD
* PARAMETERS:    Len - source data
*                Bcd - destination data
*                BcdLen - Length of BCD code
* RETURN:        T - success / F - failed.
* ------------------------------------------------------------------------ */
boolean BankUtil_Len2Bcd(usint Len, byte *Bcd, byte BcdLen)
{
    char  format[10], str[20];

    if (BcdLen > 10)
    {
        return FALSE;
    }

    memset(format, 0, sizeof(format));
    sprintf(format, "%c0%lulu", '%', (ulint)(BcdLen*2));
    sprintf((char*)str, (char*)format, Len);
    BankUtil_Asc2Bcd((byte*)str, Bcd, BcdLen*2);

    return TRUE;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Bcd2Len
* DESCRIPTION:   Convert BcdLen bytes BCD length to usint.
* PARAMETERS:    Bcd - source data,
*                Len - destination data,
*                BcdLen - Bcd bytes(1 or 2)
* RETURN:        T - success / F - failed.
* ------------------------------------------------------------------------ */
boolean BankUtil_Bcd2Len(byte *Bcd, usint *Len, byte BcdLen)
{
    sint  i;
    usint len;

    len = 0;

    for (i = 0; i < BcdLen; i++)
    {
        len = len * 100 + (((*(Bcd + i)) & 0xF0) >> 4) * 10 + ((*(Bcd + i)) & 0xF);
    }

    *Len = len;
    return TRUE;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_GenLrc
* DESCRIPTION:   Generate LRC, upper function should add STX, etc for
*                asyn data.
* PARAMETERS:    Data - input data buffer,
*                DataLen - data length.
* RETURN:        LRC byte.
* ------------------------------------------------------------------------ */
byte BankUtil_GenLrc(byte *Data, usint DataLen)
{
    sint  i;
    byte  lrc = 0;

    for (i = 0; i < DataLen; i++)
    {
        lrc ^= Data[i];
    }

    return lrc;
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_DES
* DESCRIPTION:   Performs DES, DESX, and 3DES computations.
* PARAMETERS:    ucDeaOption(in)  -- Algorithm
*                Key        (in)  -- Pointer to 8N-byte key block (N=1,2,3)
*        Input_Data (in)  -- Pointer to 8-byte input block.
*        Output_Data(out) -- Pointer to 8-byte output block.
* RETURN:       0（成功） / -1 失败
* ------------------------------------------------------------------------ */
int BankUtil_DES(byte ucDeaOption, byte * Key, byte * Input_Data, byte * Output_Data)
{
    return DES(ucDeaOption, Key, Input_Data, Output_Data);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_CStrlcpy
* DESCRIPTION:   中文字符串智能copy函数，不会出现半个汉字的情况，
        只拷贝最多siz-1个字节并最后附加0x00
* PARAMETERS:    dst  (out)  -- 目标缓冲
*                src   (in)  -- 源缓冲
*         siz (in)  -- copy的字符串长度
* RETURN:       返回已拷贝的字符数
* ------------------------------------------------------------------------ */
unsigned int BankUtil_CStrlcpy(char *dst, const char *src, unsigned int siz)
{
    char *p = dst;
    int icNum = 0, i = 0, ic = 0, pos = 0;

    strlcpy(p, src, siz);
    ic = strlen(p);

    if (ic > 0)
    {
        for (i = 0; i < ic; i++)
        {
            if ((unsigned char)(*(p + i)) == 0x00)
            {
                break;
            }

            if ((unsigned char)(*(p + i)) > 0xA0)
            {
                icNum++;
                pos = i;
            }
        }

        if (icNum % 2 == 1)
        {
            *(p + pos) = 0x00;
            ic = pos;
        }
    }

    return (ic);
}

/*=========================================*
*   P R I V A T E     F U N C T I O N S   *
*=========================================*/

//////////////////////////////////////////////////////////////////////////
/* Function : strlcpy           保护版本strncpy
   Paraments: char *dst 目标串, char *src源串, 只拷贝最多siz-1个字节并最后附加0x00
   Return   : 返回源串的字串长度, 不包含NULL, dst 与 src指针不得重叠
 */
unsigned int strlcpy(char *dst, const char *src, unsigned int siz)
{
    char *d = dst;
    const char *s = src;
    size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0)
    {
        while (--n != 0)
        {
            if ((*d++ = *s++) == '\0')
                break;
        }
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0)
    {
        if (siz != 0)
            *d = '\0';      /* NUL-terminate dst */

        while (*s++)
            ;
    }

    return(s - src - 1);    /* count does not include NUL */
}



