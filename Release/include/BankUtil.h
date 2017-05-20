#ifndef __BANK_UTIL_H__
#define __BANK_UTIL_H__

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_InputDate
* DESCRIPTION:   日期输入IN ASC (MMDD)
* PARAMETERS:    msgPrompt - message for prompting,
*                buf    - [in] original string, [out] return string,
*                MaxLen - maximum digits for entering.
* RETURN:        TRUE - amount entered / FALSE - user break.
* NOTES:         buf will be terminated by '\0'
* ------------------------------------------------------------------------ */
boolean BankUtil_InputDate(int disp_line, char *msgPrompt, char *buf, usint MaxLen);

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Bcd2Asc.
* DESCRIPTION:   BCD码->ASCII码.
* PARAMETERS:    BcdBuf - BCD input buffer,
*                AscBuf - converted result, terminated by '\0',
*                Len - double length of BCD code, must be even.
* RETURN:        T - success / F - failed(not legal BCD code buffer).
* NOTES:
* ------------------------------------------------------------------------ */
boolean BankUtil_Bcd2Asc(byte *BcdBuf, byte *AscBuf, usint Len );

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Asc2Bcd.
* DESCRIPTION:   ASCII码->BCD码.
* PARAMETERS:    AscBuf - Ascii input buffer, must ended by '\0'
*                BcdBuf - converted result
*                Len - double length of BCD code, should be even.
* RETURN:        T - success / F - failed(illegal char in ASCII buffer).
* NOTES:         support 'A'-'F' convertion.
* ------------------------------------------------------------------------ */
boolean BankUtil_Asc2Bcd(byte *AscBuf, byte *BcdBuf, usint Len);

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_ZipNumber.
* DESCRIPTION:   ASCII 数字->BCD码, ended by 'F'.
* PARAMETERS:    AscBuf - Asc input buffer,
*                BcdBuf - converted result, terminated by 'F'
*                Len - Max length of AscBuf
* RETURN:        
* NOTES:
* ------------------------------------------------------------------------ */
void BankUtil_ZipNumber( char *AscBuf, byte *BcdBuf, usint Len );

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_UnzipNumber.
* DESCRIPTION:   BCD码->ASCII 数字, AscBuf end with '\0' when meet 'F'.
* PARAMETERS:    AscBuf - Ascii input buffer, should ended by '\0'
*                BcdBuf - converted result, right-filled with 'F',
*                Len - Max length of AscBuf
* RETURN:        
* NOTES:         support 'A'-'F' convertion.
* ------------------------------------------------------------------------ */
void BankUtil_UnzipNumber(byte *BcdBuf, char *AscBuf, usint Len );

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
void BankUtil_Amt2Asc(ulint amt, byte *AscBuf, usint AscLen);

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Asc2Amt.
* DESCRIPTION:   Convert ASCII string to amount
* PARAMETERS:    amt - output amount,
*                AscBuf - input
*                AscLen - ASCII string length.
* RETURN:        none.
* NOTES:
* ------------------------------------------------------------------------ */
void BankUtil_Asc2Amt(ulint *amt, byte *AscBuf, usint AscLen);

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Len2Bcd
* DESCRIPTION:   Convert usint to n bytes BCD
* PARAMETERS:    Len - source data
*                Bcd - destination data
*                BcdLen - Length of BCD code
* RETURN:        T - success / F - failed.
* ------------------------------------------------------------------------ */
boolean BankUtil_Len2Bcd(usint Len, byte *Bcd, byte BcdLen );

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_Bcd2Len
* DESCRIPTION:   Convert BcdLen bytes BCD length to usint.
* PARAMETERS:    Bcd - source data,
*                Len - destination data,
*                BcdLen - Bcd bytes(1 or 2)
* RETURN:        T - success / F - failed.
* ------------------------------------------------------------------------ */
boolean BankUtil_Bcd2Len( byte *Bcd, usint *Len, byte BcdLen );

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_GenLrc
* DESCRIPTION:   Generate LRC, upper function should add STX, etc for
*                asyn data.
* PARAMETERS:    Data - input data buffer,
*                DataLen - data length.
* RETURN:        LRC byte.
* ------------------------------------------------------------------------ */
byte BankUtil_GenLrc(byte *Data, usint DataLen);

/* --------------------------------------------------------------------------
* FUNCTION NAME: BankUtil_DES
* DESCRIPTION:   Performs DES, DESX, and 3DES computations.
* PARAMETERS:    ucDeaOption(in)  -- Algorithm 
																			DESE (08h): DEA encryption
																			DESD (09h): DEA decryption
																			TDES2KE (0Ch): TDEA encryption
																			TDES2KD (0Dh): TDEA decryption
*                Key        (in)  -- Pointer to 8N-byte key block (N=1,2,3)
*				 Input_Data (in)  -- Pointer to 8-byte input block.
*				 Output_Data(out) -- Pointer to 8-byte output block.
* RETURN:       0（成功） / -1 失败
* ------------------------------------------------------------------------ */
int BankUtil_DES(byte ucDeaOption, byte * Key, byte * Input_Data, byte * Output_Data);


 /* --------------------------------------------------------------------------
 * FUNCTION NAME: BankUtil_CStrlcpy
 * DESCRIPTION:   中文字符串智能copy函数，不会出现半个汉字的情况，
				  只拷贝最多siz-1个字节并最后附加0x00
 * PARAMETERS:    dst	(out)  -- 目标缓冲
 *                src   (in)  -- 源缓冲
 *				  siz	(in)  -- copy的字符串长度
 * RETURN:       返回已拷贝的字符数
* ------------------------------------------------------------------------ */
unsigned int BankUtil_CStrlcpy(char *dst, const char *src, unsigned int siz);

#endif
