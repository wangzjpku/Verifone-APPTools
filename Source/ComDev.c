/***********************************************************************
* FILE NAME:    ComDev.C
* PROGRAMMER:   luqr
* DESCRIPTION:    COM�豸����ģ��
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
/*�ṹ���궨�塢��������*/

/*==========================================*
*         P U B L I C  D A T A             *
*==========================================*/
/* ȫ�ֱ������� */

/*==========================================*
*        P R I V A T E  D A T A            *
*==========================================*/
/* �ֲ��������� */

/*=====================================================*
*   I N C L U D E  P R I V A T E  F U N C T I O N S   *
*=====================================================*/
/* ˽�к������� */
void vdWaitForPrinter(int hPrt);

/*=========================================*
*   P U B L I C     F U N C T I O N S     *
*=========================================*/
/* --------------------------------------------------------------------------
* FUNCTION NAME: Device_GetHandle
* DESCRIPTION: ��ȡ�����豸���
* PARAMETERS:
*   device_name (in) -- COM�豸����
* RETURN:
*   device_handle -- �豸������ɹ���
*   COMDEV_NOHANDLE -- ��ȡ���ʧ��
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
* DESCRIPTION:��ʼ��COM�˿�
* PARAMETERS:
*   device_name (in) -- �����豸����
*   com_param   (in) -- com_param�˿ڲ���
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
* DESCRIPTION: ��COM����
* PARAMETERS:
*   device_name (in)   -- �����豸����
*   buffer (out) -- ��������
*   count  (in)  -- �����ݵ���󳤶�
* RETURN:
*   bytes_read -- ��ȡ���ݵ�ʵ�ʳ���
*   COMDEV_NOHANDLE/COMDEV_READ_FAIL -- ������ʧ��
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
* DESCRIPTION:д��������
* PARAMETERS:
*   device_name (in)   -- �����豸����
*   buffer (in)  -- д�������
*   count  (in)  -- д�����ݵĳ���
* RETURN:
*   bytes_written -- д�����ݵĳ���
*   COMDEV_NOHANDLE/COMDEV_WRITE_FAIL -- д����ʧ��
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

    /*д����֮ǰ����մ��ڻ�����*/
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
