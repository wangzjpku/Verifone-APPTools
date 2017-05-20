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


// OUTPUT BITS
#define BT_MASK_LINK_DROP  (1<<0)
#define BT_MASK_P20        (1<<1)
#define BT_MASK_P24        (1<<2)
#define BT_MASK_EAN        (1<<3)
#define BT_MASK_RST_N      (1<<4)
#define BT_MASK_IND_DEVICE (1<<5)
#define BT_MASK_WAKEUP     (1<<6)

// INPUT BITS
#define BT_MASK_IND_HOST   (1<<0)
#define BT_MASK_STAUTS_IND (1<<1)

void bt_set_mode(int hdl, int mode)
{
    int pins;

    set_bt_pins(hdl, 0x55);

    pins = BT_MASK_LINK_DROP | BT_MASK_P20 | BT_MASK_P24; // App mode

    switch (mode)
    {
    case 1: // set mode 1 - Application mode
        pins |= BT_MASK_P20;
        pins |= BT_MASK_P24;
        pins &= ~BT_MASK_EAN;
        set_bt_pins(hdl,pins);
        SVC_WAIT(1);
        pins &= ~BT_MASK_RST_N;
        set_bt_pins(hdl,pins); // Assert reset
        SVC_WAIT(100);
        pins |= BT_MASK_RST_N;
        set_bt_pins(hdl,pins); // Release reset
        SVC_WAIT(500);
        break;
    case 2: // set mode 2 - TEST/EEPROM mode
        pins &= ~BT_MASK_P20;
        pins |= BT_MASK_P24;
        pins &= ~BT_MASK_EAN;
        set_bt_pins(hdl,pins);
        SVC_WAIT(1);
        pins &= ~BT_MASK_RST_N;
        set_bt_pins(hdl,pins); // Assert reset
        SVC_WAIT(100);
        pins |= BT_MASK_RST_N;
        set_bt_pins(hdl,pins); // Release reset
        SVC_WAIT(500);
        break;
    case 3: // set mode 3 - WRITE FLASH mode
        pins &= ~BT_MASK_P20;
        pins &= ~BT_MASK_P24;
        pins |= BT_MASK_EAN;
        set_bt_pins(hdl,pins);
        SVC_WAIT(1);
        pins &= ~BT_MASK_RST_N;
        set_bt_pins(hdl,pins); // Assert reset
        SVC_WAIT(100);
        pins |= BT_MASK_RST_N;
        set_bt_pins(hdl,pins); // Release reset
        SVC_WAIT(500);
        break;
    }

    {
        //int bt_status;
        //bt_status = get_bt_pins(hdl);
        //LOG_PRINTF(("get_bt_pins=[%x] ", bt_status));
    }

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: BlueTooth_Open
* DESCRIPTION:打开蓝牙设备并初始化
* PARAMETERS:
* RETURN:
*   bytes_written -- 写入数据的长度
*   BT_NOHANDLE/ -- 蓝牙设备操作失败
* NOTES:
* ------------------------------------------------------------------------ */
int BlueTooth_Open(void)
{
    int port;
    struct Opn_Blk com_blk;

    port = open (DEV_COM3, 0);
    if (port < 0)
    {
        return BT_NOHANDLE;
    }

    com_blk.rate     = Rt_115200;
    com_blk.format   = Fmt_A8N1;
    com_blk.protocol = P_char_mode;
    set_opn_blk(port, &com_blk);

    bt_set_mode(port, 1); // set application mode

    return BT_OK;

}



