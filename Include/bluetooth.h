#ifndef	__BLUETOOTH_H__
#define __BLUETOOTH_H__

typedef enum
{
	BT_OK		      =  0,			/*串口设备正常*/
		BT_NOHANDLE	  = -1,		  /*获取句柄失败*/
		BT_INIT_FAIL	= -2,		  /*初始化失败*/
		BT_READ_FAIL	= -3,     /*串口设备读失败*/
		BT_WRITE_FAIL = -4,     /*串口设备写失败*/
}enumBTRet;

int BlueTooth_Open(void);

#endif	/* __BLUETOOTH_H__ */
