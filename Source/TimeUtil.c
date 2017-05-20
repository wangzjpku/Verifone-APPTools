/****************************************************************************
* FILE NAME:   timeutil.C                                                   *
* MODULE NAME: timeutil                                                     *
* PROGRAMMER:  lanpw                                                   *
* DESCRIPTION:                                                             *
* REVISION:    01.00 09/11/10                                              *
****************************************************************************/

#include "AppTools.h"

/*==========================================*
*        P R I V A T E  DEFINE             *
*==========================================*/
#define leapyear(year) ((!((year)%4)&&((year)%100)) || !((year)%400))

/*==========================================*
*        P R I V A T E  D A T A            *
*==========================================*/

/*=========================================*
*   P R I V A T E   F U N C T I O N S     *
*=========================================*/

/*=========================================*
*   P U B L I C     F U N C T I O N S     *
*=========================================*/
/*------------------------------------------------------------------------
*  FUNCTION NAME :   TimeUtil_MillSec
*  DESCRIPTION   :   计时（毫秒）
*  PARAMETER     :
*  RETURN        :
*  NOTE          :
*-----------------------------------------------------------------------*/
ulint TimeUtil_MillSec(void)
{
    return read_ticks();
}

/*------------------------------------------------------------------------
*  FUNCTION NAME :   TimeUtil_CheckDate()
*  DESCRIPTION   :   CHECK IF THE BCD FORMAT DATE IS INVALIDATE
*  PARAMETER     :   date: BCD日期(YYYYMMDD)4bytes
*  RETURN        :   TRUE/FALSE
*  NOTE          :   ADDED BY LIGHTNING IN JAN-31-2000
*-----------------------------------------------------------------------*/
boolean TimeUtil_CheckDate(byte *date)
{
    ulint in_date;
    char buff[20];
    usint year, i;
    byte month, day;

    memset(buff, 0, sizeof(buff));

    /*check if date string are all numeric*/
    for (i = 0; i < 4; i++)
    {
        byte hb, lb;

        hb = (date[i] >> 4) & 0x0F;
        lb = date[i] & 0x0F;

        if ((hb > 9) || (lb > 9))
        {
            return FALSE;
        }

        buff[2*i] = '0' + hb;
        buff[2*i + 1] = '0' + lb;
    }

    in_date = atol((const char*)buff);
    year = in_date / 10000;
    month = (in_date % 10000) / 100;
    day = in_date % 100;

    if (year < 1900)
    {
        return FALSE;  /*if year is less than 1900*/
    }

    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:

        if (day > 31)
        {
            return FALSE;
        }

        break;
    case 2:

        if (leapyear(year))
        {
            if (day > 29)
            {
                return FALSE;
            }
        }
        else if (day > 28)
        {
            return FALSE;
        }

        break;

    case 4:
    case 6:
    case 9:
    case 11:

        if (day > 30)
        {
            return FALSE;
        }

        break;

    default:
        return FALSE;
    }

    return TRUE;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_GetDate
* DESCRIPTION:   returns current date into long int as 'yyyymmdd'
* RETURN:        lint date
* ------------------------------------------------------------------------ */
lint TimeUtil_GetDate(void)
{
    char  buffer[20];

    memset(buffer, 0, sizeof(buffer));

    if (SVC_CLOCK(0, buffer, 15) != 15)
        return 0;

    buffer[8] = 0;
    return atol(buffer);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_GetTime
* DESCRIPTION:   returns current time into long int as 'hhmmss'
* RETURN:        lint time
* ------------------------------------------------------------------------ */
lint TimeUtil_GetTime(void)
{
    char  buffer[20];

    memset(buffer, 0, sizeof(buffer));

    if (SVC_CLOCK(0, buffer, 15) != 15)
        return 0;

    buffer[14] = 0;

    return atol(&buffer[8]);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_SaveToNos
* DESCRIPTION:   save DATE and TIME in the NOS
*  PARAMETER     NewDate  : (in)BCD日期(MMDD)2bytes
*                NewDate  : (in)BCD时间(HHMMSS)3bytes
* RETURN:        none
* ------------------------------------------------------------------------ */
void TimeUtil_SaveToNos(byte *NewDate, byte *NewTime)
{
    byte buf[20];
    byte tmp[20];

    if (SVC_CLOCK(0, (char *)buf, 15) != 15)
        return;

    BankUtil_Asc2Bcd(buf, tmp, 14);

    if (NewDate != NULL)
    {
        tmp[2] = NewDate[0];
        tmp[3] = NewDate[1];
    }

    if (NewTime != NULL)
    {
        tmp[4] = NewTime[0];
        tmp[5] = NewTime[1];
        tmp[6] = NewTime[2];
    }

    if (!TimeUtil_CheckDate(tmp)) /* Invalid date */
    {
        return;
    }

    memset(buf, 0, sizeof(buf));
    BankUtil_Bcd2Asc(tmp, buf, 14);
    SVC_CLOCK(1, (void *)buf, 14);

    return;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_DateAndTimeAsStr
* DESCRIPTION:
* RETURN:        string for printing ASC NULL terminal (YYYY/MM/DD HH:MM:SS)
* NOTES:
* ------------------------------------------------------------------------ */
void TimeUtil_DateAndTimeAsStr(char *time_str)
{
    char buf[16];

    if (SVC_CLOCK(0, buf, 15) != 15)
        return;

    buf[14] = 0;

    memcpy(time_str, buf, 4);   //YYYY
    memcpy(time_str + 4, "/", 1);
    memcpy(time_str + 5, buf + 4, 2); //MM
    memcpy(time_str + 7, "/", 1);
    memcpy(time_str + 8, buf + 6, 2); //DD
    memcpy(time_str + 10, " ", 1);
    memcpy(time_str + 11, buf + 8, 2);  //HH
    memcpy(time_str + 13, ":", 1);
    memcpy(time_str + 14, buf + 10, 2); //MM
    memcpy(time_str + 16, ":", 1);
    memcpy(time_str + 17, buf + 12, 2); //SS
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: TimeUtil_SetYear
* DESCRIPTION:   set year of date
*  PARAMETER     newYear  : (in) (YYYY)4bytes
* RETURN:        none
* ------------------------------------------------------------------------ */
void TimeUtil_SetYear(byte *newYear)
{
    byte buf[20];

    if (SVC_CLOCK(0, (char *)buf, 15) != 15)
        return;

    memcpy(buf, newYear, 4);

    SVC_CLOCK(1, (void *)buf, 14);

    return;
}
