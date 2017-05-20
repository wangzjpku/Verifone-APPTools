/****************************************************************************
* FILE NAME:   Barcode.c                                                    *
* MODULE NAME: Barcode                                                      *
* PROGRAMMER:  Xingyu                                                       *
* DESCRIPTION: Barcode application for testing the Payware Mobile Agnostic  *                                                          *
* REVISION:    01.00 09/13/12                                               *
****************************************************************************/
/*==========================================*
*             I N C L U D E S              *
*==========================================*/
#include "AppTools.h"

#define DC1	0x11

//  FRAME TYPES HOST TO SCANNER
#define HS_READ         0X40
#define HS_WRITE        0X41
#define HS_STATUS       0X43
#define HS_PERM_READ    0X44
#define HS_PERM_WRITE   0X44

// FRAME TYPES SCANNER TO HOST
#define SH_REPLY        0X50
#define SH_RESULT       0X51
#define SH_STATUS_READY 0X53
#define SH_PERM_REPLY   0X54
#define SH_BAR_DATA     0X60
#define SH_EVENT        0X61
#define SH_SETUP_BAR_DATA 0X62

/** EvaluateResponse returned Status bits **/
#define EPS_ACK     0X00000001                  // ACK low level frame receive
#define EPS_NAK     0X00000002                  // NAK low level frame received
#define EPS_BUSY    0X00000004                  // BUSY low level frame received
#define EPS_RESEND  0X00000008                  // RESEND low level frame received
#define EPS_RESP    0X00000010                  // Packet Response received
#define EPS_FDATA   0X00000020                  // Barcode data withing a frame received
#define EPS_DATA    0X00000040                  // Raw barcode data received
#define EPS_ERROR   0X10000000                  // Unknown response

static int hBarcode;
static char *responsePtr;
static int responseLen;

void Debug_Hex(char *Msg, unsigned char *Data, int DataLen)
{
    char  Out_Buf[1024*2], Out_Data[2048], tmp[1024];
    int   i, j, k;

    memset(Out_Data, 0, sizeof(Out_Data));
    memset(Out_Buf, 0, sizeof(Out_Buf));
    //LOG_PRINTF(("----START----:[%s ][%d]", Msg, DataLen));

    j = (DataLen % 8) ? (DataLen / 8 + 1) : (DataLen / 8);

    for (i = 0; i < j; i++)
    {
        memset(Out_Data, 0, sizeof(Out_Data));
        sprintf(Out_Data, "%03u| ", i * 8);

        for (k = 0; k < 8; k++)
        {
            sprintf(tmp, "%02X", Data[i*8+k]);
            strcat(Out_Data, tmp);

            if (k == 3)
            {
                strcat(Out_Data, " ");
            }
        }

        //LOG_PRINTF(((char *)Out_Data));
    }

    //LOG_PRINTF(("----END----"));

}

/* ===========================================================================
 * FUNCTION:  EvaluateResponse
 * ===========================================================================
 * Description:
 * Evaluates response data from the BCS.  A low level ack is usually expect
 * but can be accompanied by a response packet as well.  If a response packet
 * is found, the global responsePtr and responseLen variables have been
 * updated pointing to the response that we want to pass upward to the host.
 * Evaluation Response flags are returned based on what was found in the
 * data received from the BCS. The status flags returned are in the following
 * format:
 * EPS_ACK     0X00000001   // ACK low level frame receive
 * EPS_NAK     0X00000002   // NAK low level frame received
 * EPS_BUSY    0X00000004   // BUSY low level frame received
 * EPS_RESEND  0X00000008   // RESEND low level frame received
 * EPS_RESP    0X00000010   // Packet Response received
 * EPS_FDATA   0X00000020   // Barcode data withing a frame received
 * EPS_DATA    0X00000040   // Raw barcode data received
 * EPS_ERROR   0X10000000   // Unknown response
 *  * ======================================================================== */
unsigned int EvaluateResponse( char *buf, int len)
{
    char *dataEnd = buf + len;
    unsigned int ERStatus = 0;                  // evaluate ROM status
    union
    {
        unsigned short *sPtr;
        char *cPtr;
    } dPtr;
    char rid;

    unsigned char frameNumber;

    responsePtr = buf;

    while ( (dataEnd - buf) >= 3 )              // at lease 1 frame size available, change from 5 to 3
    {
        if ( buf[0] == STX )
        {
            switch ( buf[2] )                   // frame type
            {
            case ACK:
                ERStatus |= EPS_ACK;
                buf += 5;
                frameNumber++;
                //                LOG( "ACK received");
                break;

            case NAK:
                ERStatus |= EPS_NAK;
                buf += 5;
                //                LOG( "NAK received");
                break;

            case ESC:                           // BUSY
                ERStatus |= EPS_BUSY;
                buf += 5;
                //                LOG( "ESC received");
                break;

            case ENQ:                           // RESEND
                ERStatus |= EPS_RESEND;
                buf += 5;
                //                LOG( "ENQ received");
                break;

                /**  !!!!!! MUST KNOW THE SIZE OF THE FRAME !!!!!!  ASK INTERMEC */
            case SH_REPLY:
            case SH_PERM_REPLY:
            case SH_EVENT:
                dPtr.cPtr = strchr( buf, ETX ); // find end of frame
                if ( dPtr.cPtr++ > 0 )          // increment to begin of next frame
                {
                    ERStatus |= EPS_RESP;
                    responsePtr = buf;
                    responseLen = buf - dPtr.cPtr;
                    buf = dPtr.cPtr;
                    //                    LOG( "SH_REPLY - packet received");
                }
                else
                {
                    ERStatus |= EPS_ERROR;
                    buf = dataEnd;              // serious error bail out
                    //                    LOG("SH_REPLY - packet format error");
                    //LOG(buf);
                }
                break;

            case SH_STATUS_READY:
                //                 dPtr.cPtr = strchr( buf, ETX ); // find end of frame
                //                 LOG( "ETX  pos=%d", dPtr.cPtr );
                //                 if( dPtr.cPtr++ > 0 ) {         // increment to begin of next frame
                //                     ERStatus |= EPS_RESP;
                //                     LOG( "STRP  start=%d", buf );
                //                     LOG( "STRP    end=%d", dPtr.cPtr );
                //                     responsePtr = buf;
                //                     responseLen = buf - dPtr.cPtr;
                //                     buf = dPtr.cPtr;
                //                     LOG( "STRP - packet received");
                //                     LOG( "STRP lenght=%d", responseLen );
                //                 }
                //                 else {
                //                     ERStatus |= EPS_ERROR;
                //                     LOG("STRP - packet format error");
                //                     LOG( "STRP  start=%d", buf );
                //                     LOG( "STRP    end=%d", dPtr.cPtr );
                //                     LOG( "STRP lenght=%d", buf - dPtr.cPtr );
                //                     LOG(buf+6);
                //                     buf = dataEnd;              // serious error bail out
                // //                    dbprintf( "SH_REPLY - packet format error\n");
                //                 }

                dPtr.cPtr = strchr( buf+6, ETX ); // find end of frame, skip 0x00 data
                //                LOG( "ETX  pos=%d", dPtr.cPtr );
                if ( dPtr.cPtr++ > 0 )          // increment to begin of next frame
                {
                    ERStatus |= EPS_RESP;
                    //                    LOG( "STRP  start=%d", buf );
                    //                    LOG( "STRP    end=%d", dPtr.cPtr );
                    responsePtr = buf;
                    responseLen = dPtr.cPtr - buf;
                    buf = dPtr.cPtr;
                    //                    LOG( "STRP - packet received");
                    //                    LOG( "STRP lenght=%d", responseLen );
                }
                else
                {
                    ERStatus |= EPS_ERROR;
                    //                    LOG("STRP - packet format error");
                    //                    LOG( "STRP  start=%d", buf );
                    //                    LOG( "STRP    end=%d", dPtr.cPtr );
                    //                    LOG( "STRP lenght=%d", buf - dPtr.cPtr );
                    //                    LOG(buf+6);
                    buf = dataEnd;              // serious error bail out
                    //                    dbprintf( "SH_REPLY - packet format error\n");
                }
                break;

            case SH_RESULT:
                ERStatus |= EPS_RESP;
                responsePtr = buf;
                rid = buf[3];
                if ( (rid == 0x81) || (rid == 0x82) )
                {
                    buf += 6;
                }
                else if ( rid == 0x83 )
                {
                    buf += 4;
                }
                buf += 8;                       // minimum frame size
                break;

            case SH_BAR_DATA:
                ERStatus |= EPS_FDATA;
                responsePtr = buf;
                dPtr.sPtr = (unsigned short *)&buf[3];    // packet data length
                buf += *dPtr.sPtr;              // add barcode data length
                buf += 10;                      // minimum frame size
                //ScanBeep();
                break;

            case SH_SETUP_BAR_DATA:
                ERStatus |= EPS_RESP;
                responsePtr = buf;
                dPtr.sPtr = (unsigned short *)&buf[3];    // packet data length
                buf += *dPtr.sPtr;              // add barcode data length
                buf += 10;                      // minimum frame size
                break;

            default:
                ERStatus |= EPS_ERROR;          // unknown packet type
                return 0;                       // fix infinite loop
            }                                   /// End Switch
        }
        else if ( buf[0] >= 0x20 )              // Raw Barcode data
        {
            ERStatus |= EPS_DATA;
            responseLen = len;
            buf += len;                         // minimum packet lentgth
            //ScanBeep();
        }
        else
        {
            return 0;                           // fix infinite loop
        }
    }                                           /// end while()

    return ERStatus;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Barcode_Activate.
* DESCRIPTION:   Activate Scanner.  Powers up the barcode scanner.
* PARAMETERS:
* RETURN:
*		BARCODE_OK/BARCODE_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Barcode_Activate()
{
    char	cmd[4];
    int		rv;

    hBarcode = Device_GetHandle(DEV_BAR);
    //LOG_PRINTF(("DEV_BAR handle =[%d]", hBarcode));
    if (hBarcode < 0)
        return BARCODE_FAIL;

    cmd[0] = DC1;
    cmd[1] = 0x01;
    rv = write(hBarcode, cmd, 2);
    //LOG_PRINTF(("Activate Barcode rv=[%d %d]", rv, errno));
    if (rv < 0)
    {
        return BARCODE_FAIL;
    }

    SVC_WAIT(100);

    return BARCODE_OK;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Barcode_Deactivate.
* DESCRIPTION:   Deactivate scanner. Powers off the scanner engine
* PARAMETERS:
* RETURN:
*		BARCODE_OK/BARCODE_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Barcode_Deactivate()
{
    char	cmd[4];
    int		rv;

    //LOG_PRINTF(("DEV_BAR handle =[%d]", hBarcode));
    if (hBarcode < 0)
        return BARCODE_FAIL;

    cmd[0] = DC1;
    cmd[1] = 0x02;
    rv = write(hBarcode, cmd, 2);
    //LOG_PRINTF(("Deactivate Barcode rv=[%d %d]", rv, errno));
    if (rv < 0)
    {
        return BARCODE_FAIL;
    }

    return BARCODE_OK;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: Barcode_Scan.
* DESCRIPTION:   barcode scan
* PARAMETERS:
	trigger_mode(in) - see enum TriggerMode
	timeout(in)	- in seconds
	barcode(out)
	barcode_Len(out)
* RETURN:
*
* NOTES:
* ------------------------------------------------------------------------ */
int Barcode_Scan(char trigger_mode, int timeout, char *barcode, int *barcode_Len)
{
    long	event;
    int		rv, len;
    unsigned int flags;
    char	cmd[10];
    unsigned long	ms_timeout = timeout*1000;
    unsigned long ulTimerCount=0;
    char buf[400];

    if (hBarcode < 0)
        return BARCODE_FAIL;

    //clear barcode buf
    while (read(hBarcode, (char *)buf, 1) > 0);

    //40 ¨C Sets the trigger mode
    rv = _control(hBarcode, 40, (char *)trigger_mode);
    //LOG_PRINTF(("Sets trigger mode [%d]=[%d %d]", trigger_mode, rv, errno));
    if (rv < 0)
    {
        return BARCODE_FAIL;
    }

    //Set timeout for Edge Trigger Mode
    if (trigger_mode == EDGE_TRIGGER_MODE)
    {
        cmd[0] = DC1;
        cmd[1] = 0x03;
        memcpy(cmd+2, &ms_timeout, 4);		//characters (offset 2-5) contain the number of milliseconds
        rv = write(hBarcode, cmd, 6);
        //LOG_PRINTF(("Set timeout for Edge Trigger Mode=[%d %d]", rv, errno));
        if (rv < 0)
        {
            return BARCODE_FAIL;
        }
    }

    //Enable trigger for Soft and Passive trigger modes.
    if (trigger_mode == SOFT_TRIGGER_MODE)
    {
        cmd[0] = DC1;
        cmd[1] = 0x04;
        rv = write(hBarcode, cmd, 2);
        //LOG_PRINTF(("Enable trigger=[%d %d]", rv, errno));
        if (rv < 0)
        {
            return BARCODE_FAIL;
        }
    }

    ulTimerCount = set_itimeout(-1, timeout, TM_SECONDS);
    while (1)
    {
        /* Timeout occurred */
        if (CHK_TIMEOUT(-1, ulTimerCount) == 0)
        {
            return BARCODE_TIMEOUT;
        }

        event = read_evt(EVT_BAR);
        if (event & EVT_BAR)
        {
            memset(buf, 0, sizeof(buf));
            len = read(hBarcode, buf, sizeof(buf));

            if (len > 0)
            {
                Debug_Hex("scan:", (byte *)buf, len);
                flags = EvaluateResponse( buf, len);
                //LOG_PRINTF(("resp flag:[0x%08X] len=[%d]", flags, responseLen));
                if ( flags & (EPS_RESP+EPS_FDATA+EPS_DATA) )
                {
                    *barcode_Len = responseLen;
                    memcpy(barcode, responsePtr, responseLen);
                    return BARCODE_OK;
                }
            }
        }
    }

}

