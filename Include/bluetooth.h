#ifndef	__BLUETOOTH_H__
#define __BLUETOOTH_H__

typedef enum
{
	BT_OK		      =  0,			/*�����豸����*/
		BT_NOHANDLE	  = -1,		  /*��ȡ���ʧ��*/
		BT_INIT_FAIL	= -2,		  /*��ʼ��ʧ��*/
		BT_READ_FAIL	= -3,     /*�����豸��ʧ��*/
		BT_WRITE_FAIL = -4,     /*�����豸дʧ��*/
}enumBTRet;

int BlueTooth_Open(void);

#endif	/* __BLUETOOTH_H__ */
