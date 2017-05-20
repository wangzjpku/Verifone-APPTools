#ifndef __TIMEUTIL_H__
#define __TIMEUTIL_H__

/*------------------------------------------------------------------------
*  FUNCTION NAME :   TimeUtil_MillSec
*  DESCRIPTION   :   计时（毫秒）
*  PARAMETER     :   
*  RETURN        :   
*  NOTE          :  
*-----------------------------------------------------------------------*/
ulint TimeUtil_MillSec(void);

/*------------------------------------------------------------------------
*  FUNCTION NAME :   TimeUtil_CheckDate()
*  DESCRIPTION   :   检测BCD格式日期是否有效
*  PARAMETER     :   date: BCD日期(YYYYMMDD)4bytes
*  RETURN        :   TRUE/FALSE
*  NOTE          :   
*-----------------------------------------------------------------------*/
boolean TimeUtil_CheckDate(byte *date);

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_GetDate
* DESCRIPTION:   returns current date into long int as 'yyyymmdd'
* RETURN:        lint date
* ------------------------------------------------------------------------ */
lint TimeUtil_GetDate( void );

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_GetTime
* DESCRIPTION:   returns current time into long int as 'hhmmss'
* RETURN:        lint time
* ------------------------------------------------------------------------ */
lint TimeUtil_GetTime( void );

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_SaveToNos
* DESCRIPTION:   save DATE and TIME in the NOS
*  PARAMETER     NewDate  : (in)BCD日期(MMDD)2bytes
*                NewDate  : (in)BCD时间(HHMMSS)3bytes 
* RETURN:        none
* ------------------------------------------------------------------------ */
void TimeUtil_SaveToNos(byte *NewDate, byte *NewTime);

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_DateAndTimeAsStr
* DESCRIPTION:
* RETURN:        string for printing ASC NULL terminal (YYYY/MM/DD HH:MM:SS)
* NOTES:
* ------------------------------------------------------------------------ */
void TimeUtil_DateAndTimeAsStr(char *time_str);

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_SetYear
* DESCRIPTION:   set year of date 
*  PARAMETER     newYear  : (in) (YYYY)4bytes
* RETURN:        none
* ------------------------------------------------------------------------ */
void TimeUtil_SetYear(byte *newYear);

#endif
