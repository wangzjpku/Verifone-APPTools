#ifndef __FILE_H__
#define __FILE_H__

/*�ļ�����ģʽ*/
typedef  enum
{
	F_RDONLY = 0x0000,
	F_WRONLY = 0x0001,
	F_RDWR = 0x0002,     
	F_NDELAY = 0x0004,    
	F_APPEND = 0x0008,    
	F_CREAT = 0x0100,    
	F_TRUNC = 0x0200,    
	F_EXCL = 0x0400,    
	F_CODEFILE = 0x4000
}FILE_FLAGS;

/*�ļ�ָ�붨λ*/
typedef  enum
{
	F_SEEK_SET = 0,
	F_SEEK_CUR,
	F_SEEK_END 
}FILE_SEEK_FLAGS;


/*Error Code */
typedef enum
{
	FILE_SUCCESS		=  0,              //�ļ������ɹ�
	FILE_FAIL			= -1,              //�쳣
	FILE_OPEN_FAIL  	= -2,              //�򿪴���
	FILE_CREATE_FAIL 	= -3,              //�ļ��ؽ�����
	FILE_WRITE_FAIL		= -4,              //д����
	FILE_READ_FAIL		= -5,              //������
	FILE_SEEK_FAIL		= -6,              //��λ�ļ�ָ�����
	FILE_DELETE_FAIL	= -7,              //ɾ���ļ���¼����
	FILE_CLOSE_FAIL		= -8,              //�ر��ļ�����
	FILE_REMOVE_FAIL	= -9,              //�ر��ļ�����
	FILE_NO_RECORD		= -10,              //��¼δ�ҵ�
}enumFileRet2;

typedef struct
{
	usint   StartOffset;/*�ӽṹ�ĵڼ����ֽڿ�ʼ����*/
	usint   Length;      /*�Ƚ��ֽڳ���*/
	byte*   SearchKey;   /*���ҹؼ���*/
}DBSearCond;

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_DirGetFirst
* DESCRIPTION:��ȡĿ¼�µ�һ���ļ���
* PARAMETERS:
*		drive (in)	- I:(for arm)/F:(for flash)
*		drive (out) - ��Ŀ¼�µ�һ���ļ���
* RETURN:
*		0  -- �ɹ�
*		-1 -- ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_DirGetFirst(char *drive);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_DirGetNext
* DESCRIPTION:��ȡĿ¼����һ���ļ���
* PARAMETERS:
*		buffer (in)	- ��Ŀ¼�µ�ǰ�ļ���
*		buffer (out) - ��Ŀ¼����һ���ļ���
* RETURN:
*		0  -- �ɹ�
*		-1 -- ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_DirGetNext(char *buffer);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Open
* DESCRIPTION:���ļ�
* PARAMETERS:
*		FileName (in)	- ���ļ���, ��NULL�������32���ֽ�
*		Flag     (in) - ���ļ���ʽ, ȡֵ�ο�FILE_FLAGS����
* RETURN:
*		file_handle -- �ļ�open�ɹ�
*		FILE_OPEN_FAIL -- �ļ�openʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Open(const char *FileName, int Flag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Read
* DESCRIPTION:���ļ���¼
* PARAMETERS:
*		Handle (in)	 -- �ļ����
*		buffer (out) -- ��������
*		count  (in)  -- �����ݵ���󳤶�
* RETURN:
*		bytes_read -- ��ȡ���ݵ�ʵ�ʳ���
*		FILE_READ_FAIL -- ���ļ�ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Read(int handle, char *buffer, int count);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_ReadLine.
* DESCRIPTION:  ��һ���ı�
* PARAMETERS:
*			pFile		(in)	- �ļ�handle
*			pLineBuff	(out)	- ��ȡ���ı�����
*			LineBuffSize(in)	- �����С
* RETURN:
*			buf_len    -  ���ݳ��ȣ����ݿ�����ɹ���
*			EOF		   -  ���ļ�����
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_ReadLine(int pFile, char *pLineBuff,unsigned int LineBuffSize);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Write
* DESCRIPTION:д�ļ���¼
* PARAMETERS:
*		Handle (in)	 -- �ļ����
*		buffer (in)  -- д�������
*		count  (in)  -- д�����ݵĳ���
* RETURN:
*		bytes_written -- д�����ݵĳ���
*		FILE_WRITE_FAIL -- д�ļ�ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Write(int handle, char *buffer, int count);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Lseek
* DESCRIPTION:��λ�ļ�ָ��
* PARAMETERS:
*		Handle (in)	 -- �ļ����
*		offset (in)  -- ָ���ƶ��ı�����
*		origin (in)  -- ��ʼλ�ã��ο�FILE_SEEK_FLAGS
* RETURN:
*		bytes_seek -- �ļ�ָ������ļ���ͷ���ֽ���
*		FILE_SEEK_FAIL -- ��λ�ļ�ָ��ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
long File_Lseek(int handle, long offset, int origin);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Delete
* DESCRIPTION:ɾ���ļ���¼
* PARAMETERS:
*		Handle (in)	 -- �ļ����
*		count  (in)  --  ɾ�����ݳ���
* RETURN:
*		FILE_SUCCESS     -- ɾ���ļ���¼�ɹ�
*		FILE_DELETE_FAIL -- ɾ���ļ���¼ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Delete(int handle, unsigned int count);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Close
* DESCRIPTION:�ر��ļ�
* PARAMETERS:
*		Handle (in)	 -- �ļ����
* RETURN:
*		FILE_SUCCESS     -- �ر��ļ��ɹ�
*		FILE_CLOSE_FAIL -- �ر��ļ�ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Close(int handle);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Remove
* DESCRIPTION:ɾ���ļ�
* PARAMETERS:
*		FileName (in)	- ɾ���ļ���, ��NULL�������32���ֽ�
* RETURN:
*		FILE_SUCCESS     -- �ر��ļ��ɹ�
*		FILE_REMOVE_FAIL -- ɾ���ļ�ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Remove(const char *filename);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Rename
* DESCRIPTION: �������ļ�
* PARAMETERS:
*			oldname (in)	- ���ļ���
*			newname (in)    - ���ļ���
* RETURN:
*			FILE_SUCCESS	-- �ر��ļ��ɹ�
*			FILE_FAIL		-- �������ļ�ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Rename(const char *oldname, const char *newname);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Clear
* DESCRIPTION:����ļ�
* PARAMETERS:
*		FileName (in)	- ����ļ���, ��NULL�������32���ֽ�
* RETURN:
*		FILE_SUCCESS   -- ����ļ��ɹ�
*		FILE_OPEN_FAIL -- ����ļ�ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Clear(const char *FileName);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetFreeSpace.
* DESCRIPTION:  ȡ�ļ�ϵͳʣ��ռ�
* PARAMETERS:    
*		drive     (in)      - ��������("I:"��"F:")
* RETURN:       
*		Avail     -  ʣ��ռ�
*		FILE_FAIL -  �ļ�ϵͳ����ʧ��
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
long File_GetFreeSpace(const char *drive);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetNumberOfRecords.
* DESCRIPTION:  ȡ�ļ���¼����
* PARAMETERS:    
*		FileName     (in)      - �ļ�����
*		Record_Len   (in)      - ��¼����
* RETURN:       
*		num_record     -  ��¼����
*		FILE_OPEN_FAIL -  �ļ�����ʧ��
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
int File_GetNumberOfRecords(const char *FileName, int Record_Len);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_AppendRecord
* DESCRIPTION:���ļ�ĩβ�����һ��������¼�ļ������ļ�������ʱ���Զ��������ļ���
* PARAMETERS:
*		FileName    (in)	-- �ļ���, ��NULL�������32���ֽ�
*		Record      (in)	-- ��¼����
*		Record_Len  (in)	-- ��¼���ݵĳ���
* RETURN:
*		FILE_SUCCESS   -- ׷�Ӽ�¼�ɹ�
*		FILE_CREATE_FAIL/FILE_WRITE_FAIL -- ׷�Ӽ�¼ʧ��
* NOTES: 
* ------------------------------------------------------------------------ */
int File_AppendRecord(const char *FileName, char *Record, int Record_Len);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetRecordByIndex.
* DESCRIPTION:  ͨ����¼������������һ����¼
* PARAMETERS:    
*		FileName     (in)      - �ļ�����
*		Record       (out)     - ��¼����
*		Record_Len   (in)      - ��¼����
*		Record_Index (in)      - ��¼����(��0��ʼ)
* RETURN:       FILE_SUCCESS    -  ���ݿ�����ɹ�
*               FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*               FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_UpdateRecordByIndex.
* DESCRIPTION:  ͨ����¼������������һ����¼
* PARAMETERS:    
*		FileName     (in)      - �ļ�����
*		Record       (in)      - ��¼����
*		Record_Len   (in)      - ��¼����
*		Record_Index (in)      - ��¼����(��0��ʼ)
* RETURN:       FILE_SUCCESS    -  ���ݿ�����ɹ�
*               FILE_OPEN_FAIL/FILE_WRITE_FAIL  -  ���ݿ����ʧ��
*               FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* -------------------------------------------------------------------------------------------------- */
int File_UpdateRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index);


/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteRecordByIndex.
* DESCRIPTION:  ͨ����¼����ɾ������һ����¼
* PARAMETERS:    
*		FileName    (in)  - �ļ�����
*		Record_Len  (in)  - ��¼����
* 		Record_Index(in)  - ��¼����(��0��ʼ)
* RETURN:       FILE_SUCCESS    -  ���ݿ�����ɹ�
*               FILE_OPEN_FAIL/FILE_DELETE_FAIL  -  ���ݿ����ʧ��
*               FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
int File_DeleteRecordByIndex(const char *FileName, int Record_Len, int Record_Index);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetRecord.
* DESCRIPTION:  ͨ��������������һ����¼
* PARAMETERS:    
*			FileName     (in)      - �ļ�����
*			Record       (out)     - ��¼����
*			Record_Len   (in)      - ��¼����
*			Condtion	 (in)      - ��ѯ�������ο�DBSearCond�ṹ
* RETURN:       
*			FILE_SUCCESS    -  ���ݿ�����ɹ�
*			FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*			FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_UpdateRecord.
* DESCRIPTION:  ͨ��������������һ����¼
* PARAMETERS:    
*			FileName     (in)      - �ļ�����
*			Record       (in)     - ��¼����
*			Record_Len   (in)      - ��¼����
*			Condtion	 (in)      - ��ѯ�������ο�DBSearCond�ṹ
* RETURN:       
*			FILE_SUCCESS    -  ���ݿ�����ɹ�
*			FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*			FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_UpdateRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteRecord.
* DESCRIPTION:  ͨ������ɾ������һ����¼
* PARAMETERS:    
*			FileName     (in)      - �ļ�����
*			Record_Len   (in)      - ��¼����
*			Condtion	 (in)      - ��ѯ�������ο�DBSearCond�ṹ
* RETURN:       
*			FILE_SUCCESS    -  ���ݿ�����ɹ�
*			FILE_OPEN_FAIL/FILE_DELETE_FAIL  -  ���ݿ����ʧ��
*			FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_DeleteRecord(const char *FileName, int Record_Len, DBSearCond *Condtion);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_ReadTLV.
* DESCRIPTION:  ���Ƕ�����¼TLV
* PARAMETERS:    
*			FileName	(in)	- �ļ�����
*			FldID		(in)	- ��ǩ(Tag)
*			Data		(in)	- ����(Value)
*			DataLen		(in)	- ����(length)
* RETURN:       
*			FILE_SUCCESS    -  ���ݿ�����ɹ�
*			FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*			FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_ReadTLV(char *FileName, usint FldID, char *Data, usint *DataLen);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_WriteTLV.
* DESCRIPTION:  д�Ƕ�����¼TLV
* PARAMETERS:    
*			FileName	(in)	- �ļ�����
*			FldID		(in)	- ��ǩ(Tag)
*			Data		(in)	- ����(Value)
*			DataLen		(in)	- ����(length)
* RETURN:       
*			FILE_SUCCESS    -  ���ݿ�����ɹ�
*			FILE_OPEN_FAIL/FILE_READ_FAIL/FILE_WRITE_FAIL  -  ���ݿ����ʧ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_WriteTLV(char *FileName,usint FldID,byte *Data,usint DataLen);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteTLV.
* DESCRIPTION:  ɾ���Ƕ�����¼TLV
* PARAMETERS:    
*			FileName	(in)	- �ļ�����
*			FldID		(in)	- ��ǩ(Tag)
* RETURN:       
*			FILE_SUCCESS    -  ���ݿ�����ɹ�
*			FILE_OPEN_FAIL/FILE_READ_FAIL/FILE_WRITE_FAIL  -  ���ݿ����ʧ��
*			FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_DeleteTLV(char *FileName, usint FldID);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetKey.
* DESCRIPTION:  ��DLD����
* PARAMETERS:    
*			FileName	(in)	- �ļ�����
*			key			(in)	- �ؼ���
*			buf			(out)	- ����
*			size		(in)	- ���ݳ���
* RETURN:       
*			buf_len    -  ���ݳ��ȣ����ݿ�����ɹ���
*			FILE_FAIL  -  ���ݿ����ʧ��
*			FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetKey(const char *key, char *buf, int size, const char *filename);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_PutKey.
* DESCRIPTION:  дDLD����
* PARAMETERS:    
*			FileName	(in)	- �ļ�����
*			key			(in)	- �ؼ���
*			buf			(in)	- ����
*			size		(in)	- ���ݳ���
* RETURN:       
*			buf_len    -  ���ݳ��ȣ����ݿ�����ɹ���
*			FILE_FAIL  -  ���ݿ����ʧ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_PutKey(const char *key, const char *buf, int size, const char *filename);

#endif
