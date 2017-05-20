/*-------------------------------------------------------------------
* Filename	:    ComDev.H
* PROGRAMMER:  	 luqr
* DESCRIPTION:   COM设备操作模块
* REVISION:      01.00.00 08-17-10.
**********************************************************************/

#ifndef COMDEV_H
#define COMDEV_H

/*串口设备返回*/
typedef enum
{
	COMDEV_OK		      =  0,			/*串口设备正常*/
	COMDEV_NOHANDLE	  = -1,		  /*获取句柄失败*/
	COMDEV_INIT_FAIL	= -2,		  /*初始化失败*/
	COMDEV_READ_FAIL	= -3,     /*串口设备读失败*/
	COMDEV_WRITE_FAIL = -4,     /*串口设备写失败*/
}enumComDevRet;

/* --------------------------------------------------------------------------
* FUNCTION NAME: Device_GetHandle
* DESCRIPTION: 获取串口设备句柄
* PARAMETERS:
*		device_name (in) -- COM设备名称
* RETURN:
*		device_handle -- 设备句柄（成功）
*		COMDEV_NOHANDLE -- 获取句柄失败
* NOTES: 
* ------------------------------------------------------------------------ */
int Device_GetHandle(const char *device_name);

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Init
* DESCRIPTION:初始化COM端口
* PARAMETERS:
*		device_name (in) -- 串口设备名称
*		com_param   (in) -- com_param端口参数
* RETURN:
*		COMDEV_OK/COMDEV_NOHANDLE/COMDEV_INIT_FAIL
* NOTES: 
* ------------------------------------------------------------------------ */
int ComDev_Init(const char *device_name, struct Opn_Blk *com_param);

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Read
* DESCRIPTION: 读COM数据
* PARAMETERS:
*		device_name (in)	 -- 串口设备名称
*		buffer (out) -- 读出数据
*		count  (in)  -- 读数据的最大长度
* RETURN:
*		bytes_read -- 读取数据的实际长度
*		COMDEV_NOHANDLE/COMDEV_READ_FAIL -- 读串口失败
* NOTES: 
* ------------------------------------------------------------------------ */
int ComDev_Read(const char *device_name, char *buf, int data_len);

/* --------------------------------------------------------------------------
* FUNCTION NAME: ComDev_Write
* DESCRIPTION:写串口数据
* PARAMETERS:
*		device_name (in)	 -- 串口设备名称
*		buffer (in)  -- 写入的数据
*		count  (in)  -- 写入数据的长度
* RETURN:
*		bytes_written -- 写入数据的长度
*		COMDEV_NOHANDLE/COMDEV_WRITE_FAIL -- 写串口失败
* NOTES: 
* ------------------------------------------------------------------------ */
int ComDev_Write(const char *device_name, char *buffer, int count);

#endif

