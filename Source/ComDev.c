/***********************************************************************
* FILE NAME:    ComDev.C
* PROGRAMMER:   luqr
* DESCRIPTION:    COM设备操作模块
* REVISION:      01.00.00 08-17-10.
**********************************************************************/
/*==========================================*
*         I N T R O D U C T I O N          *
*==========================================*/

/* void */

/*==========================================*
*             I N C L U D E S              *
*==========================================*/

#include "AppTools.h"

/*==========================================*
*        D E F I N I T I O N S             *
*==========================================*/
/*结构及宏定义、常量定义*/

/*==========================================*
*         P U B L I C  D A T A             *
*==========================================*/
/* 全局变量定义 */

/*==========================================*
*        P R I V A T E  D A T A            *
*==========================================*/
/* 局部变量定义 */

/*=====================================================*
*   I N C L U D E  P R I V A T E  F U N C T I O N S   *
*=====================================================*/
/* 私有函数声明 */
void vdWaitForPrinter(int hPrt);

/*=========================================*
*   P U B L I C     F U N C T I O N S     *
*=========================================*/
/* --------------------------------------------------------------------------
* FUNCTION NAME: Device_GetHandle
* DESCRIPTION: 获取串口设备句柄
* PARAMETERS:
*   device_name (in) -- COM设备名称
* RETURN:
*   device_handle -- 设备句柄（成功）
*   COMDEV_NOHANDLE -- 获取句柄失败
* NOTES:
* ------------------------------------------------------------------------ */
int Device_GetHandle(const char *device_name)
{
    int task_id, local_taskid;
    int device_handle;

    local_taskid = get_task_id();
    //ApplLog_Debug("ATOOL","local task id = [%d]", local_taskid);

    device_handle = get_owner(device_name, &task_id);
    //ApplLog_Debug("ATOOL","com device owner = [%d], handle = [%d]", task_id, device_handle);

    if (device_handle <= 0 || task_id != local_taskid)
    {
        return COMDEV_NOHANDLE;
    }

    return device_handle;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Init
* DESCRIPTION:初始化COM端口
* PARAMETERS:
*   device_name (in) -- 串口设备名称
*   com_param   (in) -- com_param端口参数
* RETURN:
*   COMDEV_OK/COMDEV_NOHANDLE/COMDEV_INIT_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int ComDev_Init(const char *device_name, struct Opn_Blk *com_param)
{
    int device_handle;
    int ret;

    device_handle = Device_GetHandle(device_name);

    if (device_handle < 0)
        return COMDEV_NOHANDLE;

    ret = set_opn_blk(device_handle, com_param);

    if (ret < 0)
        return COMDEV_INIT_FAIL;

    return COMDEV_OK;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Read
* DESCRIPTION: 读COM数据
* PARAMETERS:
*   device_name (in)   -- 串口设备名称
*   buffer (out) -- 读出数据
*   count  (in)  -- 读数据的最大长度
* RETURN:
*   bytes_read -- 读取数据的实际长度
*   COMDEV_NOHANDLE/COMDEV_READ_FAIL -- 读串口失败
* NOTES:
* ------------------------------------------------------------------------ */
int ComDev_Read(const char *device_name, char *buf, int data_len)
{
    int device_handle;
    int bytes_read;

    device_handle = Device_GetHandle(device_name);

    if (device_handle < 0)
        return COMDEV_NOHANDLE;

    bytes_read = read(device_handle, buf, data_len);

    //ApplLog_Debug("ATOOL","com read handle=[%d] ret=[%d] errno=[%d]", device_handle, bytes_read, errno);
    if (bytes_read < 0)
        return COMDEV_READ_FAIL;

    return bytes_read;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Write
* DESCRIPTION:写串口数据
* PARAMETERS:
*   device_name (in)   -- 串口设备名称
*   buffer (in)  -- 写入的数据
*   count  (in)  -- 写入数据的长度
* RETURN:
*   bytes_written -- 写入数据的长度
*   COMDEV_NOHANDLE/COMDEV_WRITE_FAIL -- 写串口失败
* NOTES:
* ------------------------------------------------------------------------ */
int ComDev_Write(const char *device_name, char *buffer, int count)
{
    int device_handle;
    int bytes_written;
    char tmp[10];

    device_handle = Device_GetHandle(device_name);

    if (device_handle < 0)
        return COMDEV_NOHANDLE;

    /*写串口之前先清空串口缓冲区*/
    while (read(device_handle, tmp, 10) > 0)
        SVC_WAIT(5);

    vdWaitForPrinter(device_handle);
    SVC_WAIT(4);

    do
    {

        errno = 0;

        bytes_written = write(device_handle, buffer, count);
        if (bytes_written != count)
        {
            if (errno == ENOSPC)
                SVC_WAIT(20);
        }

    }
    while ((bytes_written < 0) && (errno == ENOSPC || errno == ENXIO));


    //ApplLog_Debug("ATOOL","com write handle=[%d] ret=[%d] errno=[%d]", device_handle, bytes_written, errno);
    if (bytes_written < 0)
        return COMDEV_WRITE_FAIL;

    return  bytes_written;

}

void vdWaitForPrinter(int hPrt)
{
    char b[5];
    int  status;

    memset(b, 0x00, sizeof(b));
    do
    {
        status = get_port_status(hPrt, b);
        if (status == 0)
            break;

        SVC_WAIT(5);
    }
    while (status >= 0);

}
