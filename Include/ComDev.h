/*-------------------------------------------------------------------
* Filename	:    ComDev.H
* PROGRAMMER:  	 luqr
* DESCRIPTION:   COM�豸����ģ��
* REVISION:      01.00.00 08-17-10.
**********************************************************************/

#ifndef COMDEV_H
#define COMDEV_H

/*�����豸����*/
typedef enum
{
	COMDEV_OK		      =  0,			/*�����豸����*/
	COMDEV_NOHANDLE	  = -1,		  /*��ȡ���ʧ��*/
	COMDEV_INIT_FAIL	= -2,		  /*��ʼ��ʧ��*/
	COMDEV_READ_FAIL	= -3,     /*�����豸��ʧ��*/
	COMDEV_WRITE_FAIL = -4,     /*�����豸дʧ��*/
}enumComDevRet;

/* --------------------------------------------------------------------------
* FUNCTION NAME: Device_GetHandle
* DESCRIPTION: ��ȡ�����豸���
* PARAMETERS:
*		device_name (in) -- COM�豸����
* RETURN:
*		device_handle -- �豸������ɹ���
*		COMDEV_NOHANDLE -- ��ȡ���ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int Device_GetHandle(const char *device_name);

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Init
* DESCRIPTION:��ʼ��COM�˿�
* PARAMETERS:
*		device_name (in) -- �����豸����
*		com_param   (in) -- com_param�˿ڲ���
* RETURN:
*		COMDEV_OK/COMDEV_NOHANDLE/COMDEV_INIT_FAIL
* NOTES: 
* ------------------------------------------------------------------------ */
int ComDev_Init(const char *device_name, struct Opn_Blk *com_param);

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Read
* DESCRIPTION: ��COM����
* PARAMETERS:
*		device_name (in)	 -- �����豸����
*		buffer (out) -- ��������
*		count  (in)  -- �����ݵ���󳤶�
* RETURN:
*		bytes_read -- ��ȡ���ݵ�ʵ�ʳ���
*		COMDEV_NOHANDLE/COMDEV_READ_FAIL -- ������ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int ComDev_Read(const char *device_name, char *buf, int data_len);

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Write
* DESCRIPTION:д��������
* PARAMETERS:
*		device_name (in)	 -- �����豸����
*		buffer (in)  -- д�������
*		count  (in)  -- д�����ݵĳ���
* RETURN:
*		bytes_written -- д�����ݵĳ���
*		COMDEV_NOHANDLE/COMDEV_WRITE_FAIL -- д����ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int ComDev_Write(const char *device_name, char *buffer, int count);

#endif

