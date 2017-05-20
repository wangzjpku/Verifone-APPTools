/***********************************************************************
* FILE NAME:     FILE.C
* PROGRAMMER:   luqr
* DESCRIPTION:    �ļ�����ģ��
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

typedef struct _HashNode HashNode;
typedef struct _hashTable HashTable;

struct _HashNode  //ɢ�б���
{
    char * key;
    char * value;
    HashNode *next; //��һ���������ָ��
    HashNode *pt;   //�����ĵڶ���hashindex��
};

struct _hashTable
{
    unsigned int size;
    unsigned int current;
    HashNode items[1];
};

/*Operation Type */
typedef  enum
{
    RECORD_FIND = 0,    //����
    RECORD_UPDATE,      //����
    RECORD_DEL,         //ɾ��
    RECORD_ADD,         //���
} OPER_TYPE;

#define MAX_FILE_SIZE   4096        /*TLV�ļ�����С*/
#define MAX_INI_LINELEN 256

/*==========================================*
*         P U B L I C  D A T A             *
*==========================================*/
/* ȫ�ֱ������� */

/*==========================================*
*        P R I V A T E  D A T A            *
*==========================================*/
/* �ֲ��������� */

typedef struct
{
    usint t;  /*!< \brief Tag ID */
    usint l;  /*!< \brief Length of Data */
    byte *v;  /*!< \brief Value */
} appTLV;

typedef struct
{
    usint mlen;   /*!< \brief max data length */
    usint len;      /*!< \brief buffer data length */
    byte  *buf; /*!< \brief data buffer */
} TLVbuf;


/*=====================================================*
*   I N C L U D E  P R I V A T E  F U N C T I O N S   *
*=====================================================*/
/* ˽�к������� */

int File_OperRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index, char OperFlag);
int File_OperRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion, char OperFlag);
int get_env_From_DLD(const char *filename, const char *key, char *buf, int size);
int put_env_To_DLD(const char *filename, const char *key, const char *buf, int size);

int tb_find(const TLVbuf  *tb, usint t, appTLV  *tlv);
int tb_add(TLVbuf  *tb, appTLV  *tlv);
int tb_del(TLVbuf  *tb, usint t);

HashTable * HashExpend(HashTable ** To);

/*=========================================*
*   P U B L I C     F U N C T I O N S     *
*=========================================*/
/* --------------------------------------------------------------------------
* FUNCTION NAME: File_DirGetFirst
* DESCRIPTION:��ȡĿ¼�µ�һ���ļ���
* PARAMETERS:
*   drive (in)  - I:(for arm)/F:(for flash)
*   drive (out) - ��Ŀ¼�µ�һ���ļ���
* RETURN:
*   0  -- �ɹ�
*   -1 -- ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_DirGetFirst(char *drive)
{
    return dir_get_first(drive);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_DirGetNext
* DESCRIPTION:��ȡĿ¼����һ���ļ���
* PARAMETERS:
*   buffer (in) - ��Ŀ¼�µ�ǰ�ļ���
*   buffer (out) - ��Ŀ¼����һ���ļ���
* RETURN:
*   0  -- �ɹ�
*   -1 -- ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_DirGetNext(char *buffer)
{
    return dir_get_next(buffer);
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Open
* DESCRIPTION:���ļ�
* PARAMETERS:
*   FileName (in) - ���ļ���, ��NULL�������32���ֽ�
*   Flag     (in) - ���ļ���ʽ, ȡֵ�ο�FILE_FLAGS����
* RETURN:
*   file_handle -- �ļ�open�ɹ�
*   FILE_OPEN_FAIL -- �ļ�openʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Open(const char *FileName, int Flag)
{
    int fh;

    //ApplLog_Debug("ATOOL","file open name=[%s] flag=[%d]", FileName, Flag);
    fh = open(FileName, Flag);

    //ApplLog_Debug("ATOOL","file open ret=[%d] errno=[%d]", fh, errno);
    if (fh <= 0)
    {
        return FILE_OPEN_FAIL;
    }

    return fh;
}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Read
* DESCRIPTION:���ļ���¼
* PARAMETERS:
*   Handle (in)  -- �ļ����
*   buffer (out) -- ��������
*   count  (in)  -- �����ݵ���󳤶�
* RETURN:
*   bytes_read -- ��ȡ���ݵ�ʵ�ʳ���
*   FILE_READ_FAIL -- ���ļ�ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Read(int handle, char *buffer, int count)
{
    int bytes_read;

    bytes_read = read(handle, buffer, count);

    //ApplLog_Debug("ATOOL","file read handle=[%d] count=[%d] ret=[%d] errno=[%d]", handle, count, bytes_read, errno);
    if (bytes_read < 0)
        return FILE_READ_FAIL;

    return  bytes_read;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Write
* DESCRIPTION:д�ļ���¼
* PARAMETERS:
*   Handle (in)  -- �ļ����
*   buffer (in)  -- д�������
*   count  (in)  -- д�����ݵĳ���
* RETURN:
*   bytes_written -- д�����ݵĳ���
*   FILE_WRITE_FAIL -- д�ļ�ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Write(int handle, char *buffer, int count)
{
    int bytes_written;

    bytes_written = write(handle, buffer, count);

    //ApplLog_Debug("ATOOL","file write handle=[%d] ret=[%d] errno=[%d]", handle, bytes_written, errno);
    if (bytes_written < 0)
        return FILE_WRITE_FAIL;

    return  bytes_written;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Lseek
* DESCRIPTION:��λ�ļ�ָ��
* PARAMETERS:
*   Handle (in)  -- �ļ����
*   offset (in)  -- ָ���ƶ��ı�����
*   origin (in)  -- ��ʼλ�ã��ο�FILE_SEEK_FLAGS
* RETURN:
*   bytes_seek -- �ļ�ָ������ļ���ͷ���ֽ���
*   FILE_SEEK_FAIL -- ��λ�ļ�ָ��ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
long File_Lseek(int handle, long offset, int origin)
{
    long bytes_seek;

    bytes_seek = lseek(handle, offset, origin);

    //ApplLog_Debug("ATOOL","file lseek handle=[%d] ret=[%d] errno=[%d]", handle, bytes_seek, errno);
    if (bytes_seek < 0)
        return FILE_SEEK_FAIL;

    return  bytes_seek;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Delete
* DESCRIPTION:ɾ���ļ���¼
* PARAMETERS:
*   Handle (in)  -- �ļ����
*   count  (in)  --  ɾ�����ݳ���
* RETURN:
*   FILE_SUCCESS     -- ɾ���ļ���¼�ɹ�
*   FILE_DELETE_FAIL -- ɾ���ļ���¼ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Delete(int handle, unsigned int count)
{
    int bytes_delete;

    bytes_delete = delete(handle, count);

    //ApplLog_Debug("ATOOL","file delete handle=[%d] ret=[%d] errno=[%d]", handle, bytes_delete, errno);
    if (bytes_delete < 0)
        return FILE_DELETE_FAIL;

    return FILE_SUCCESS;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Close
* DESCRIPTION:�ر��ļ�
* PARAMETERS:
*   Handle (in)  -- �ļ����
* RETURN:
*   FILE_SUCCESS    -- �ر��ļ��ɹ�
*   FILE_CLOSE_FAIL -- �ر��ļ�ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Close(int handle)
{
    int ret;

    ret = close(handle);
    SVC_WAIT( 2 );

    //ApplLog_Debug("ATOOL","file close handle=[%d] ret=[%d] errno=[%d]", handle, ret, errno);
    if (ret < 0)
        return FILE_CLOSE_FAIL;

    return FILE_SUCCESS;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Remove
* DESCRIPTION:ɾ���ļ�
* PARAMETERS:
*     FileName (in) - ɾ���ļ���, ��NULL�������32���ֽ�
* RETURN:
*     FILE_SUCCESS     -- �ر��ļ��ɹ�
*     FILE_REMOVE_FAIL -- ɾ���ļ�ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Remove(const char *filename)
{
    int ret;

    //ApplLog_Debug("ATOOL","file remove name=[%s]", filename);

    ret = _remove(filename);

    //ApplLog_Debug("ATOOL","file remove ret=[%d] errno=[%d]", ret, errno);
    if (ret < 0)
        return FILE_REMOVE_FAIL;

    return FILE_SUCCESS;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Rename
* DESCRIPTION: �������ļ�
* PARAMETERS:
*     oldname (in)  - ���ļ���
*     newname (in)    - ���ļ���
* RETURN:
*     FILE_SUCCESS  -- �ر��ļ��ɹ�
*     FILE_FAIL   -- �������ļ�ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Rename(const char *oldname, const char *newname)
{
    int ret;

    //ApplLog_Debug("ATOOL","file _rename old=[%s] new=[%d]", oldname, newname);

    ret = _rename(oldname, newname);

    //ApplLog_Debug("ATOOL","file remove ret=[%d] errno=[%d]", ret, errno);
    if (ret < 0)
        return FILE_FAIL;

    return FILE_SUCCESS;

}

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Clear
* DESCRIPTION:����ļ�
* PARAMETERS:
*     FileName (in) - ����ļ���, ��NULL�������32���ֽ�
* RETURN:
*     FILE_SUCCESS   -- ����ļ��ɹ�
*     FILE_OPEN_FAIL -- ����ļ�ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_Clear(const char *FileName)
{
    int fh;
    int i = 0;

    for (i = 0; i < 2; i++)
    {
        if ((fh = File_Open(FileName, O_RDWR | O_TRUNC)) < 0)
        {
            //ApplLog_Debug("ATOOL","file open fail [%d] [%d]", fh, errno);
            /*��ʧ���򴴽����ļ�*/
            if ((fh = File_Open(FileName, O_RDWR | O_CREAT)) < 0)
            {
                //ApplLog_Debug("ATOOL","file open fail [%d] [%d]", fh, errno);
                continue;
            }
            else
                break;
        }
        else
            break;
    }

    //ApplLog_Debug("ATOOL","file open ret=[%d] errno=[%d]", fh, errno);

    if (fh)
    {
        File_Close(fh);
        return FILE_SUCCESS;
    }
    else
    {
        return fh;
    }
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetFreeSpace.
* DESCRIPTION:  ȡ�ļ�ϵͳʣ��ռ�
* PARAMETERS:
*   drive     (in)      - ��������("I:"��"F:")
* RETURN:
*   Avail     -  ʣ��ռ�
*   FILE_FAIL -  �ļ�ϵͳ����ʧ��
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
long File_GetFreeSpace(const char *drive)
{
    struct fs_size fs;
    int ret;

    ret = dir_get_sizes(drive, &fs);

    if (ret < 0)
    {
        return FILE_FAIL;
    }

    return fs.Avail;
}


/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetNumberOfRecords.
* DESCRIPTION:  ȡ�ļ���¼����
* PARAMETERS:
*   FileName     (in)      - �ļ�����
*   Record_Len   (in)      - ��¼����
* RETURN:
*   num_record     -  ��¼����
*   FILE_OPEN_FAIL -  �ļ�����ʧ��
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
int File_GetNumberOfRecords(const char *FileName, int Record_Len)
{
    int       fh;
    int      num_record;

    if ((fh = File_Open(FileName, O_RDONLY)) < 0)
    {
        return FILE_OPEN_FAIL;
    }

    num_record = File_Lseek(fh, 0L, SEEK_END) / Record_Len;

    if (num_record <= 0)
    {
        File_Close(fh);
        return 0;
    }

    File_Close(fh);
    //ApplLog_Debug("ATOOL","File_GetRecordNumber =[%d]", num_record);
    return num_record;
}


/* --------------------------------------------------------------------------
* FUNCTION NAME: File_AppendRecord
* DESCRIPTION:���ļ�ĩβ�����һ��������¼�ļ������ļ�������ʱ���Զ��������ļ���
* PARAMETERS:
*     FileName    (in)  -- �ļ���, ��NULL�������32���ֽ�
*     Record      (in)  -- ��¼����
*     Record_Len  (in)  -- ��¼���ݵĳ���
* RETURN:
*     FILE_SUCCESS   -- ׷�Ӽ�¼�ɹ�
*     FILE_CREATE_FAIL/FILE_WRITE_FAIL -- ׷�Ӽ�¼ʧ��
* NOTES:
* ------------------------------------------------------------------------ */
int File_AppendRecord(const char *FileName, char *Record, int Record_Len)
{
    int fh;

    /*��ʧ���򴴽����ļ�*/
    if ((fh = File_Open(FileName, O_RDWR)) < 0)
    {
        if ((fh = File_Open(FileName, O_RDWR | O_CREAT)) < 0)
        {
            return FILE_CREATE_FAIL;
        }
    }

    /*ָ���ļ�β*/
    File_Lseek(fh, 0L, SEEK_END);

    if (File_Write(fh, (void*)Record, Record_Len) < 0)
    {
        File_Close(fh);
        return FILE_WRITE_FAIL;
    }

    File_Close(fh);

    SVC_WAIT( 0 );	// @llai_20130108, change 50->0

    return FILE_SUCCESS;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetRecordByIndex.
* DESCRIPTION:  ͨ����¼������������һ����¼
* PARAMETERS:
*     FileName     (in)      - �ļ�����
*     Record       (out)      - ��¼����
*     Record_Len   (in)      - ��¼����
*     Record_Index (in)      - ��¼����(��0��ʼ)
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index)
{
    return File_OperRecordByIndex(FileName, Record, Record_Len, Record_Index, RECORD_FIND);
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_UpdateRecordByIndex.
* DESCRIPTION:  ͨ����¼������������һ����¼
* PARAMETERS:
*     FileName     (in)      - �ļ�����
*     Record       (in)      - ��¼����
*     Record_Len   (in)      - ��¼����
*     Record_Index (in)      - ��¼����(��0��ʼ)
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_WRITE_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* -------------------------------------------------------------------------------------------------- */
int File_UpdateRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index)
{
    return File_OperRecordByIndex(FileName, Record, Record_Len, Record_Index, RECORD_UPDATE);
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteRecordByIndex.
* DESCRIPTION:  ͨ����¼����ɾ������һ����¼
* PARAMETERS:
*     FileName    (in)  - �ļ�����
*     Record_Len  (in)  - ��¼����
*     Record_Index(in)  - ��¼����(��0��ʼ)
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_DELETE_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
int File_DeleteRecordByIndex(const char *FileName, int Record_Len, int Record_Index)
{
    return File_OperRecordByIndex(FileName, NULL, Record_Len, Record_Index, RECORD_DEL);
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetRecord.
* DESCRIPTION:  ͨ��������������һ����¼
* PARAMETERS:
*     FileName     (in)      - �ļ�����
*     Record       (out)     - ��¼����
*     Record_Len   (in)      - ��¼����
*     Condtion   (in)      - ��ѯ�������ο�DBSearCond�ṹ
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion)
{
    return File_OperRecord(FileName, Record, Record_Len, Condtion, RECORD_FIND);
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_UpdateRecord.
* DESCRIPTION:  ͨ��������������һ����¼
* PARAMETERS:
*     FileName     (in)      - �ļ�����
*     Record       (in)     - ��¼����
*     Record_Len   (in)      - ��¼����
*     Condtion   (in)      - ��ѯ�������ο�DBSearCond�ṹ
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_WRITE_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_UpdateRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion)
{
    return File_OperRecord(FileName, Record, Record_Len, Condtion, RECORD_UPDATE);
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteRecord.
* DESCRIPTION:  ͨ������ɾ������һ����¼
* PARAMETERS:
*     FileName     (in)      - �ļ�����
*     Record_Len   (in)      - ��¼����
*     Condtion   (in)      - ��ѯ�������ο�DBSearCond�ṹ
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_DELETE_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_DeleteRecord(const char *FileName, int Record_Len, DBSearCond *Condtion)
{
    return File_OperRecord(FileName, NULL, Record_Len, Condtion, RECORD_DEL);
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_ReadTLV.
* DESCRIPTION:  ���Ƕ�����¼TLV
* PARAMETERS:
*     FileName  (in)  - �ļ�����
*     FldID   (in)  - ��ǩ(Tag) ��1��ʼ
*     Data    (in)  - ����(Value)
*     DataLen   (in)  - ����(length)
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_ReadTLV(char *FileName, usint FldID, char *Data, usint *DataLen)
{
    int       fh, ret;
    byte      fileBuffer[MAX_FILE_SIZE];
    long      file_size;
    TLVbuf    tb;
    appTLV      tlv;

    if ((fh = File_Open(FileName, O_RDWR)) < 0)
    {
        return FILE_OPEN_FAIL;
    }

    file_size = File_Lseek(fh, 0L, SEEK_END);

    if (file_size < 0 || file_size > MAX_FILE_SIZE)
    {
        File_Close(fh);
        return FILE_READ_FAIL;
    }

    memset(fileBuffer, 0, sizeof(fileBuffer));
    File_Lseek(fh, 0L, SEEK_SET);

    if (File_Read(fh, (char *)fileBuffer, file_size) != file_size)
    {
        File_Close(fh);
        return FILE_READ_FAIL;
    }

    tb.mlen = MAX_FILE_SIZE;
    tb.len = file_size;
    tb.buf = fileBuffer;

    //ApplLog_DebugHex("ATOOL", "file:", (char *)fileBuffer, file_size);
    //ApplLog_Debug("ATOOL", "FldID:[%d] file=[%x]", FldID, fileBuffer);

    ret = tb_find(&tb, FldID, &tlv);

    if (!ret)
    {
        File_Close(fh);
        return FILE_NO_RECORD;
    }

    *DataLen = tlv.l;
    memcpy(Data, tlv.v, tlv.l);

    File_Close(fh);
    return FILE_SUCCESS;

}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_WriteTLV.
* DESCRIPTION:  д�Ƕ�����¼TLV
* PARAMETERS:
*     FileName  (in)  - �ļ�����
*     FldID   (in)  - ��ǩ(Tag) ��1��ʼ
*     Data    (in)  - ����(Value)
*     DataLen   (in)  - ����(length)
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL/FILE_WRITE_FAIL  -  ���ݿ����ʧ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_WriteTLV(char *FileName, usint FldID, byte *Data, usint DataLen)
{
    int       fh;
    byte      fileBuffer[MAX_FILE_SIZE];
    long      file_size;
    TLVbuf    tb;
    appTLV      tlv;

    /*��ʧ���򴴽����ļ�*/
    if ((fh = File_Open(FileName, O_RDWR)) < 0)
    {
        if ((fh = File_Open(FileName, O_RDWR | O_CREAT)) < 0)
        {
            return FILE_CREATE_FAIL;
        }
    }

    file_size = File_Lseek(fh, 0L, SEEK_END);

    if (file_size < 0 || file_size > MAX_FILE_SIZE)
    {
        File_Close(fh);
        return FILE_READ_FAIL;
    }

    memset(fileBuffer, 0, sizeof(fileBuffer));
    File_Lseek(fh, 0L, SEEK_SET);

    if (File_Read(fh, (char *)fileBuffer, file_size) != file_size)
    {
        File_Close(fh);
        return FILE_READ_FAIL;
    }

    //ApplLog_DebugHex("ATOOL","file1:", (char *)fileBuffer, file_size);

    tb.mlen = MAX_FILE_SIZE;
    tb.len = file_size;
    tb.buf = fileBuffer;

    tlv.t = FldID;
    tlv.l = DataLen;
    tlv.v = Data;

    //ApplLog_DebugHex("ATOOL","TLV:", (char *)tlv.v, tlv.l);
    //ApplLog_Debug("ATOOL","tb len1=[%d] tlv len=[%d]", tb.len, DataLen);

    tb_add(&tb, &tlv);

    //ApplLog_Debug("ATOOL","tb len2=[%d] ", tb.len);
    //ApplLog_DebugHex("ATOOL","file2:", (char *)fileBuffer, tb.len);

    File_Lseek(fh, 0L, SEEK_SET);

    if (File_Delete(fh, file_size) != FILE_SUCCESS)
    {
        File_Close(fh);
        return FILE_DELETE_FAIL;
    }

    if (File_Write(fh, (char *)fileBuffer, tb.len) != tb.len)
    {
        File_Close(fh);
        return FILE_WRITE_FAIL;
    }

    File_Close(fh);
    return FILE_SUCCESS;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteTLV.
* DESCRIPTION:  ɾ���Ƕ�����¼TLV
* PARAMETERS:
*     FileName  (in)  - �ļ�����
*     FldID   (in)  - ��ǩ(Tag) ��1��ʼ
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL/FILE_WRITE_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_DeleteTLV(char *FileName, usint FldID)
{
    int       fh, ret;
    byte      fileBuffer[MAX_FILE_SIZE];
    long      file_size;
    TLVbuf    tb;

    if ((fh = File_Open(FileName, O_RDWR)) < 0)
    {
        return FILE_OPEN_FAIL;
    }

    file_size = File_Lseek(fh, 0L, SEEK_END);

    if (file_size < 0 || file_size > MAX_FILE_SIZE)
    {
        File_Close(fh);
        return FILE_READ_FAIL;
    }

    memset(fileBuffer, 0, sizeof(fileBuffer));
    File_Lseek(fh, 0L, SEEK_SET);

    if (File_Read(fh, (char *)fileBuffer, file_size) != file_size)
    {
        File_Close(fh);
        return FILE_READ_FAIL;
    }

    tb.mlen = MAX_FILE_SIZE;
    tb.len = file_size;
    tb.buf = fileBuffer;

    //ApplLog_Debug("ATOOL", "tb len1=[%d]", tb.len);
    //ApplLog_DebugHex("ATOOL", "file1:", (char *)fileBuffer, file_size);

    ret = tb_del(&tb, FldID);

    if (!ret)
    {
        File_Close(fh);
        return FILE_NO_RECORD;
    }

    //ApplLog_Debug("ATOOL", "tb len2=[%d] ", tb.len);
    //ApplLog_DebugHex("ATOOL", "file2:", (char *)fileBuffer, tb.len);

    File_Lseek(fh, 0L, SEEK_SET);

    if (File_Delete(fh, file_size) != FILE_SUCCESS)
    {
        File_Close(fh);
        return FILE_DELETE_FAIL;
    }

    if (File_Write(fh, (char *)fileBuffer, tb.len) != tb.len)
    {
        File_Close(fh);
        return FILE_WRITE_FAIL;
    }

    File_Close(fh);
    return FILE_SUCCESS;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetKey.
* DESCRIPTION:  ��DLD����
* PARAMETERS:
*     FileName  (in)  - �ļ�����
*     key     (in)  - �ؼ���
*     buf     (out) - ����
*     size    (in)  - ���ݳ���
* RETURN:
*     buf_len    -  ���ݳ��ȣ����ݿ�����ɹ���
*     FILE_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetKey(const char *key, char *buf, int size, const char *filename)
{
    int ret ;

    if (filename == NULL)
    {
        ret = get_env(key, buf, size);
    }
    else
    {
        ret = get_env_From_DLD(filename, key, buf, size);

        //ApplLog_Debug("ATOOL","get_env_From_DLD [ret=%d] ....." , ret);
        if (ret < 0)
            ret = get_env(key, buf, size);
    }

    if (ret < 0)
        return FILE_FAIL;
    else if (ret == 0)
        return FILE_NO_RECORD;
    else
        return ret;

}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_PutKey.
* DESCRIPTION:  дDLD����
* PARAMETERS:
*     FileName  (in)  - �ļ�����
*     key     (in)  - �ؼ���
*     buf     (in)  - ����
*     size    (in)  - ���ݳ���
* RETURN:
*     buf_len    -  ���ݳ��ȣ����ݿ�����ɹ���
*     FILE_FAIL  -  ���ݿ����ʧ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_PutKey(const char *key, const char *buf, int size, const char *filename)
{
    int ret ;

    if (filename == NULL)
    {
        ret = put_env(key, buf, size);
    }
    else
    {
        ret = put_env_To_DLD(filename, key, buf, size);

        if (ret < 0)
            ret = put_env(key, buf, size);
    }

    if (ret < 0)
        return FILE_FAIL;
    else
        return ret;

}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_ReadLine.
* DESCRIPTION:  ��һ���ı�
* PARAMETERS:
*     pFile   (in)  - �ļ�handle
*     pLineBuff (out) - ��ȡ���ı�����
*     LineBuffSize(in)  - �����С
* RETURN:
*     buf_len    -  ���ݳ��ȣ����ݿ�����ɹ���
*     EOF      -  ���ļ�����
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_ReadLine(int pFile, char *pLineBuff, unsigned int LineBuffSize)
{
    //    char    TestBuff[MAX_INI_LINELEN];
    int     Count = 0;

    if (pLineBuff == NULL)
        return -1;

    //    memset(TestBuff, 0x00, sizeof(TestBuff));
    /* If at the file end without reading any thing, return EOF */
    for (Count = 0; Count < LineBuffSize;)
    {
        if (File_Read(pFile, pLineBuff + Count, 1) != 1)
        {
//            Count = EOF;
            pLineBuff[Count] = 0;
            return Count;
        }

        if ((pLineBuff[Count] == '\r') || (pLineBuff[Count] == '\n'))
        {
            pLineBuff[Count] = 0;

            if (Count == 0)
            {
                continue;
            }
            else
            {
                break;
            }
        }

        Count++;
    }

    if (Count == 0)
    {
        Count = EOF;
    }

    //     else if (pLineBuff != NULL)
    //     {
    //         strcpy(pLineBuff, TestBuff);
    //     }

    //ApplLog_Debug("ATOOL","count=[%d] LineBuff=[%s]",Count, pLineBuff);
    //get_char();

    return Count;
}

/*=========================================*
*   P R I V A T E     F U N C T I O N S   *
*=========================================*/

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: GethashValue.
* DESCRIPTION:  ��ȡhashֵ
* PARAMETERS:
*     key   (in)  - �ؼ���
* RETURN:
*     hash    -  ����key�����hashֵ
* NOTES:
* ----------------------------------------------------------------------------------------------- */
unsigned int GethashValue(char *key)
{
    unsigned int hash;
    unsigned char *p;

    for (hash = 0, p = (unsigned char *)key; *p ; p++)
        hash = (31 * hash + *p) & 0x7FFFFFFF;

    return hash;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashTableNew.
* DESCRIPTION:  ����ָ����С���¹�ϣ��
* PARAMETERS:
*     length    (in)  - �����ĳ���
* RETURN:
*     HashTable*    -  hash��ָ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
HashTable * HashTableNew(int length)
{
    HashTable *hs = (HashTable *)malloc(length * (sizeof(HashNode)) + sizeof(HashTable));

    if (hs == NULL)
        return NULL;

    //ApplLog_Debug("ATOOL", "Hash malloc mem[size=%d] [ADDR:%x]", length*(sizeof(HashNode)) + sizeof(HashTable), hs);

    memset(hs, 0, length*(sizeof(HashNode)) + sizeof(HashTable));

    hs->size = length;
    hs->current = 0;
    return hs;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashContainKey.
* DESCRIPTION:  �Ƿ����ָ����
* PARAMETERS:
*     T   (in)    - hash��ָ��
*     key   (in)  -   �ؼ���
* RETURN:
*     1(����)/0(������)
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int HashContainKey(HashTable *T, char * key)
{
    unsigned int hash = GethashValue(key);
    int index = hash % T->size;
    HashNode *node = T->items[index].pt;

    while (node)
    {
        if (!strcmp(key, node->key))
            return 1;

        node = node->next;
    }

    return 0;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashAdd.
* DESCRIPTION:  ����һ������ܻᵼ���ظ������ٶȽϿ�
* PARAMETERS:
*     To    (in)    - hash��ָ���ַ
*     key   (in)  -   ��ǩ
*     value (in)  -   ֵ
* RETURN:
* NOTES:
* ----------------------------------------------------------------------------------------------- */
void HashAdd(HashTable **To, char * key, void *value)
{
    HashTable   * T = *To;
    unsigned int hash;
    int index;
    HashNode *node;

    if ((T->size*95) < (T->current*100))/**���ڱ߽�0.75����չ*/
    {
        HashExpend(To);
        T = *To;
    }

    hash = GethashValue(key);
    index = hash % T->size;

    T->items[T->current].key = key;
    T->items[T->current].value = value;

    if (T->items[index].pt)
    {
        node = T->items[index].pt;

        while (node->next)
            node = node->next;

        node->next = &T->items[T->current];
        node = node->next;
        node->next = NULL;
    }
    else
        T->items[index].pt = &T->items[T->current];

    T->current++;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashCopy.
* DESCRIPTION:  ��������hash��
* PARAMETERS:
*     Tn    (in)    - Դhash��ָ���ַ
*     To    (in)  -   Ŀ��hash��ָ��
* RETURN:
* NOTES:
* ----------------------------------------------------------------------------------------------- */
void HashCopy(HashTable **Tn, HashTable *To)
{
    unsigned int i;
    HashNode * nodeT = To->items;

    for (i = 0; i < To->size; i++)
    {
        if (nodeT[i].key)
        {
            HashAdd(Tn, nodeT[i].key, nodeT[i].value);
        }
    }
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashCopy.
* DESCRIPTION:  ��չ���еı�
* PARAMETERS:
*     To    (in)    - hash��ָ���ַ
* RETURN:
*     hash��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
HashTable * HashExpend(HashTable ** To)
{
    HashTable *T = *To;
    unsigned int length = (T->current) * 2 + 1;
    HashTable *hs = (HashTable *)malloc(length * (sizeof(HashNode)) + sizeof(HashTable));

    if (hs == NULL)
        return NULL;

    //ApplLog_Debug("ATOOL", "Hash malloc mem[size=%d] [ADDR:%x]", length*(sizeof(HashNode)) + sizeof(HashTable), hs);
    memset(hs, 0, length*(sizeof(HashNode)) + sizeof(HashTable));
    hs->size = length;
    hs->current = 0;
    HashCopy(&hs, T);

    //ApplLog_Debug("ATOOL", "Hash free mem [ADDR:%x]", *To);
    free(*To);

    *To = hs;
    return hs;
}


/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashGet.
* DESCRIPTION:  ȡ�ø���key��ֵ
* PARAMETERS:
*     T   (in)    - hash��ָ��
*     key   (in)  -   ��ǩ
* RETURN:
*     value�ĵ�ַ/��������ھͷ��ؿ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
void * HashGet(HashTable *T, char * key)
{
    unsigned int hash = GethashValue(key);
    int index = hash % T->size;
    HashNode *node = T->items[index].pt;

    //ApplLog_Debug("ATOOL","key=[%s] index=[%d] node_key=[%s]", key, index, node->key);

    while (node)
    {
        if (!strcmp(key, node->key))
            return node->value;

        node = node->next;
    }

    return NULL;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashSet.
* DESCRIPTION:  ����һ�����ȷ�������Ƿ��Ѿ����ڣ�������ھͽ�������
* PARAMETERS:
*     To    (in)    - hash��ָ���ַ
*     key   (in)  -   ��ǩ
*     value (in)  -   ֵ
* RETURN:
* NOTES:
* ----------------------------------------------------------------------------------------------- */
void HashSet(HashTable **To, char * key, char *value)
{
    HashTable   *T = *To;
    unsigned int hash;
    int index;
    HashNode *node;

    if ((T->size*75) < (T->current*100))/**���ڱ߽�0.75����չ*/
    {
        HashExpend(To);
        T = *To;
    }

    hash = GethashValue(key);
    index = hash % T->size;
    node = T->items[index].pt;

    //ApplLog_Debug("ATOOL","current=[%d] key[%s] index [%d] \n", T->current, key, index);
    //get_char();

    if (!node)
        T->items[index].pt = &T->items[T->current];
    else
    {
        while (node)
        {
            //ApplLog_Debug("ATOOL","key [%s] [%s] [%p]", key, node->key, node->next);
            //get_char();

            if (!strcmp(key, node->key))
            {
                node->value = value;
                return;
            }

            if (node->next)
                node = node->next;
            else
                break;
        }

        node->next = &T->items[T->current];
        node = node->next;
        node->next = NULL;
    }

    T->items[T->current].key = key;
    T->items[T->current].value = value;
    T->items[T->current].next = NULL;
    T->current++;

    //ApplLog_Debug("ATOOL","current=[%d]", T->current);
    //get_char();
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashRemove.
* DESCRIPTION:  ɾ��ָ����
* PARAMETERS:
*     T   (in)    - hash��ָ��
*     key   (in)  -   ��ǩ
* RETURN:
* NOTES:
* ----------------------------------------------------------------------------------------------- */
void HashRemove(HashTable *T, char * key)
{
    unsigned int hash = GethashValue(key);
    int index = hash % T->size;
    HashNode *node = T->items[index].pt, *node1;
    node1 = node;

    while (node)
    {
        if (!strcmp(key, node->key))
        {
            node->key = NULL;
            node->value = NULL;

            if (node == T->items[index].pt)
                T->items[index].pt = NULL;
            else
                node1->next = node->next;

            return;
        }

        node1 = node;
        node = node->next;
    }
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: PrintHash.
* DESCRIPTION:  ��ӡhash��
* PARAMETERS:
*     T   (in)    - hash��ָ��
*     key   (in)  -   ��ǩ
* RETURN:
* NOTES:
* ----------------------------------------------------------------------------------------------- */
void PrintHash(HashTable *T)
{
    HashNode *node = T->items, *node1;
    int i;

    for (i = 0; i < T->size; i++)
    {
        //ApplLog_Debug("ATOOL","��ǰ�����ѭ����-----%d", i);
        if (node[i].key)
        {
            //ApplLog_Debug("ATOOL","����[%p]-Key[%s]-Value[%s]-PT[%p]-Next[%p]",node[i], node[i].key,node[i].value,node[i].pt,node[i].next);

        }

        node1 = node[i].pt;

        while (node1)
        {
            //ApplLog_Debug("ATOOL","�����Key:%s \tPT:%p,\tNext:%p,%p\n",i, node1->key,node1->value,node1->pt,node1->next,node1);
            node1 = node1->next;
        }
    }
}


/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: HashFree.
* DESCRIPTION:  �ͷ�һ��hash��
* PARAMETERS:
*     T   (in)    - hash��ָ��
* RETURN:
* NOTES:
* ----------------------------------------------------------------------------------------------- */
void HashFree(HashTable *T)
{
    //ApplLog_Debug("ATOOL", "free mem [ADDR:%x]", T);
    free(T);
}

void StrUpCase(char *Str)
{
    int i ;
    int len ;
    len = strlen(Str);

    if (len <= 0)
        return ;

    for (i = 0 ; i < len ; i ++)
    {
        if (*(Str + i) >= 'a' && *(Str + i) <=  'z')
            *(Str + i) -= 0x20;
    }
}

int File_IsExist(const char *FileName)
{
    int fp = open(FileName , O_RDONLY);

    if (fp < 0)
        return 0;

    close(fp);

    return 1;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: get_env_From_DLD.
* DESCRIPTION:  ��DLD����
* PARAMETERS:
*     FileName  (in)  - �ļ�����
*     key     (in)  - �ؼ���
*     buf     (out) - ����
*     size    (in)  - ���ݳ���
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int get_env_From_DLD(const char *filename, const char *key, char *buf, int size)
{
    int fp_dld;
    char /**filebuf, */*key_buf, *value_buf;
    char /*LineBuff[256],*/temp[256+1];
    char *Linebuff;
    char *pTemp;
    HashTable *hs;
    int count = 0, offset = 0, s_off = 0;
    long file_size = 0 , len_size = 0;
    char KeyName[256+1] = {0};
    char filebuf[MAX_FILE_SIZE] = {0};
    char BakFileName[128];
    sprintf(BakFileName , "%s.bak", filename);


    strncpy(KeyName, key, 128);
    StrUpCase(KeyName);

    fp_dld = File_Open(filename, O_RDONLY);

    if (fp_dld < 0)
    {
        //ApplLog_Debug("ATOOL", "file open[%s] failed[ret=%d]!!! ", filename, fp_dld);

        if (File_IsExist(BakFileName)) //������ڱ����ļ�����ָ�Ϊ��ǰ�ļ�
        {
            file_copy(BakFileName, filename);
            //ApplLog_Debug("ATOOL", "file copy[%s==>%s] ", BakFileName, filename);
        }

        fp_dld = File_Open(filename, O_RDONLY);

        if (fp_dld < 0)
        {
            //ApplLog_Debug("ATOOL", "file open[%s] failed[ret=%d]!!! ", filename, fp_dld);
            return FILE_OPEN_FAIL;
        }
    }

    file_size = File_Lseek(fp_dld, 0L, SEEK_END);

    if (file_size <= 0)
    {
        //ApplLog_Debug("ATOOL", "file[%s] size[%d] is less than 0", filename, file_size);
        File_Close(fp_dld);
        _remove(filename);
        //ApplLog_Debug("ATOOL", "remove file[%s] ....", filename);

        if (File_IsExist(BakFileName)) //������ڱ����ļ�����ָ�Ϊ��ǰ�ļ�
        {
            file_copy(BakFileName, filename);
            //ApplLog_Debug("ATOOL", "file copy[%s==>%s] ", BakFileName, filename);
        }

        fp_dld = File_Open(filename, O_RDONLY);

        if (fp_dld < 0)
        {
            //ApplLog_Debug("ATOOL", "file open[%s] failed[ret=%d]!!! ", filename, fp_dld);
            return FILE_OPEN_FAIL;
        }

        file_size = File_Lseek(fp_dld, 0L, SEEK_END);

        if (file_size <= 0)
        {
            File_Close(fp_dld);
            //ApplLog_Debug("ATOOL", "file[%s] size[%d] is less than 0", filename, file_size);
            return FILE_READ_FAIL;
        }
    }

    if (file_size > sizeof(filebuf))
    {
        File_Close(fp_dld);
        //ApplLog_Debug("ATOOL", "file[%s] size[%d] more than buffer[%d]", filename, file_size, sizeof(filebuf));
        return FILE_READ_FAIL;
    }


    hs = (HashTable *)HashTableNew(200);

    if (hs == NULL)
    {
        File_Close(fp_dld);
        //ApplLog_Debug("ATOOL", "HashTableNew[%d] failed ........", 200);
        return FILE_FAIL;
    }

//    ApplLog_Debug("ATOOL","HashTableNew[%d] OK!",200);

    memset(filebuf, 0, sizeof(filebuf));
    File_Lseek(fp_dld, 0L, SEEK_SET);

//////////////////////////////////////////////////////////////////////////
    len_size = File_Read(fp_dld, filebuf, file_size);

//    ApplLog_Debug("ATOOL","File_Read [len=%d]  [ret=%d]...",file_size,len_size);
    if (len_size != file_size)
    {
        File_Close(fp_dld);
        HashFree(hs);
        //ApplLog_Debug("ATOOL", "read buffer[len=%d] != file size[%d] ", len_size, file_size);
        return FILE_FAIL;
    }

    File_Close(fp_dld);

//    ApplLog_Debug("ATOOL","try to update HastTable...");
    offset = 0 ;

    do
    {
        s_off = 0;

        if ((pTemp = (char *) strstr(filebuf + offset, "\n")) != NULL || (pTemp = (char *) strstr(filebuf + offset, "\r")) != NULL)
        {
            Linebuff = filebuf + offset;

            if ((pTemp - filebuf) > 0 && (*pTemp == 0x0a && *(pTemp - 1) == 0x0d))
            {
                pTemp -= 1;
                s_off = 1;
            }

            *pTemp = '\0';
            count = pTemp - Linebuff ;

            if ((pTemp = (char *) strchr(Linebuff, '=')) != NULL)
            {
                *pTemp = '\0';

                key_buf = Linebuff;
                value_buf = pTemp + 1;

                StrUpCase(key_buf);
//                ApplLog_Debug("ATOOL","%s=%s",key_buf,value_buf);

                HashSet(&hs, key_buf, value_buf);
            }
        }
        else
        {
            count = file_size - offset ;
            Linebuff = filebuf + offset ;

            if ((pTemp = (char *) strchr(Linebuff, '=')) != NULL)
            {
                *pTemp = '\0';

                key_buf = Linebuff;
                value_buf = pTemp + 1;

                StrUpCase(key_buf);
//                ApplLog_Debug("ATOOL","%s=%s",key_buf,value_buf);

                HashSet(&hs, key_buf, value_buf);
            }

        }

        offset += (count + 1 + s_off);
    }
    while (offset < file_size);

//////////////////////////////////////////////////////////////////////////


    pTemp = (char *)HashGet(hs, (char *)KeyName);

//    ApplLog_Debug("ATOOL","try to find[%s] in HastTable [ret=0x%x]...",KeyName,pTemp);
    if (pTemp == NULL)
    {
        HashFree(hs);
        return FILE_NO_RECORD;
    }

//    ApplLog_Debug("ATOOL","pTemp[%s][len=%d]...",pTemp,strlen(pTemp));

    memset(temp, 0, sizeof(temp));
    strncpy(temp, pTemp, size >= sizeof(temp) ? (sizeof(temp) - 1) : size);
    strncpy(buf, temp, strlen(temp));

    HashFree(hs);
    return strlen(temp);

}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: put_env_To_DLD.
* DESCRIPTION:  дDLD����
* PARAMETERS:
*     FileName  (in)  - �ļ�����
*     key     (in)  - �ؼ���
*     buf     (in)  - ����
*     size    (in)  - ���ݳ���
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int put_env_To_DLD(const char *filename, const char *key, const char *buf, int size)
{
    int fp_dld;
    char /**filebuf,*/ *key_buf, *value_buf;
    char LineBuffer[256]/*, temp_value[128+1]*/;
    char *Linebuff;
//    char *NewFileBuf;
    char *pTemp;
    int count = 0, offset = 0, i;
    int s_off = 0 ;
    long file_size = 0;
    HashTable *hs;
    HashNode *node1;
    int ret ;
    char KeyName[256+1] = {0}, keyValue[256+1] = {0};
    char filebuf[MAX_FILE_SIZE] = {0}, NewFileBuf[MAX_FILE_SIZE] = {0};
    char BakFileName[128];

    sprintf(BakFileName , "%s.bak", filename);

    strncpy(KeyName, key, 128);
    StrUpCase(KeyName);

    strncpy(keyValue, buf, size);

    fp_dld = File_Open(filename, O_RDONLY);

    if (fp_dld < 0)
    {
        if (File_IsExist(BakFileName))
        {
            file_copy(BakFileName, filename);
            //ApplLog_Debug("ATOOL", "file copy[%s==>%s] ", BakFileName, filename);
            fp_dld = File_Open(filename, O_RDONLY);
        }


        if (fp_dld < 0)
        {
            //ApplLog_Debug("ATOOL", "open[%s] with [O_RDONLY] failed [%d]........", filename, fp_dld);
            fp_dld = File_Open(filename, O_CREAT | O_RDWR);

            if (fp_dld < 0)
            {
                //ApplLog_Debug("ATOOL", "open[%s] [O_CREAT|O_RDWR] failed [%d]........", filename, fp_dld);
                return FILE_OPEN_FAIL;
            }
        }
    }

    file_size = File_Lseek(fp_dld, 0L, SEEK_END);

    if (file_size < 0)
    {
        File_Close(fp_dld);
        //ApplLog_Debug("ATOOL", "File_Lseek failed [%d]........", file_size);
        return FILE_READ_FAIL;
    }

    if (file_size > sizeof(filebuf))
    {
        File_Close(fp_dld);
        //ApplLog_Debug("ATOOL", "file size[%d] more than buffer[%d]", file_size, sizeof(filebuf));
        return FILE_READ_FAIL;
    }

    hs = (HashTable *)HashTableNew(200);

    if (hs == NULL)
    {
        File_Close(fp_dld);
        //ApplLog_Debug("ATOOL", "HashTableNew[%d] failed ........", 200);
        return FILE_FAIL;
    }

    memset(filebuf, 0, sizeof(filebuf));
    File_Lseek(fp_dld, 0L, SEEK_SET);
    //////////////////////////////////////////////////////////////////////////
    ret = File_Read(fp_dld, filebuf, file_size);

    if (ret != file_size)
    {

        File_Close(fp_dld);
        HashFree(hs);
        //ApplLog_Debug("ATOOL", "File_Read[handle=%d][%d] failed [ret=%d]........", fp_dld, file_size, ret);
        return FILE_FAIL;
    }

    File_Close(fp_dld);

    offset = 0 ;

    do
    {
        s_off = 0 ;

        if ((pTemp = (char *) strstr(filebuf + offset, "\n")) != NULL || (pTemp = (char *) strstr(filebuf + offset, "\r")) != NULL)
        {
            Linebuff = filebuf + offset;

            if ((pTemp - filebuf) > 0 && (*pTemp == 0x0a && *(pTemp - 1) == 0x0d))
            {
                pTemp -= 1;
                s_off = 1;
            }

            *pTemp = '\0';
            count = pTemp - Linebuff ;

            if ((pTemp = (char *) strchr(Linebuff, '=')) != NULL)
            {
                *pTemp = '\0';

                key_buf = Linebuff;
                value_buf = pTemp + 1;

                StrUpCase(key_buf);
                HashSet(&hs, key_buf, value_buf);
            }
        }
        else
        {
            count = file_size - offset ;
            Linebuff = filebuf + offset ;

            if ((pTemp = (char *) strchr(Linebuff, '=')) != NULL)
            {
                *pTemp = '\0';

                key_buf = Linebuff;
                value_buf = pTemp + 1;

                StrUpCase(key_buf);
                HashSet(&hs, key_buf, value_buf);
            }

        }

        offset += (count + 1 + s_off);
    }
    while (offset < file_size);

    //////////////////////////////////////////////////////////////////////////

//����һ�������ļ���������ָ��µ�ǰ�ļ������ʱ������ȫ����ʧ�����
    _remove(BakFileName);
    file_copy(filename , BakFileName);


    HashSet(&hs, (char *)KeyName, (char *)keyValue);

    fp_dld = File_Open(filename, O_RDWR | O_TRUNC);
    File_Lseek(fp_dld, 0L, SEEK_SET);

    memset(NewFileBuf, 0 , sizeof(NewFileBuf));

    for (i = 0; i < hs->size; i++)
    {
        if (hs->items[i].key)
        {
            memset(LineBuffer, 0, sizeof(LineBuffer));
            sprintf(LineBuffer, "%s=%s\n", hs->items[i].key, hs->items[i].value);

#if 0
            ret = File_Write(fp_dld, LineBuffer, strlen(LineBuffer));
            SVC_WAIT(0);
#else

            if (strlen(NewFileBuf) + strlen(LineBuffer) >= (file_size > 1024 ? 1024 : file_size))
            {
                ret = File_Write(fp_dld, NewFileBuf, strlen(NewFileBuf));
                SVC_WAIT(0);
//                 memset(NewFileBuf, 0 , file_size+1);
                memset(NewFileBuf, 0 , sizeof(NewFileBuf));
            }

            strcat(NewFileBuf , LineBuffer);
#endif

            node1 = hs->items[i].pt;

            while (node1)
            {
                node1 = node1->next;
            }
        }
        else
        {
#if 1

            if (strlen(NewFileBuf))
            {
                ret = File_Write(fp_dld, NewFileBuf, strlen(NewFileBuf));
                SVC_WAIT(0);
                //                 memset(NewFileBuf, 0 , file_size+1);
                memset(NewFileBuf, 0 , sizeof(NewFileBuf));
            }

#endif
            break;
        }
    }

    File_Close(fp_dld);

    HashFree(hs);

    _remove(BakFileName);//ɾ�������ļ�

    return size;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_OperRecordByIndex.
* DESCRIPTION:  ͨ����¼��������/ɾ��/������������һ����ˮ��¼
* PARAMETERS:
*   FileName     (in)      - �ļ�����
*   Record       (in/out)  - ��¼����
*   Record_Len   (in)      - ��¼����
*   Record_Index (in)      - ��¼����(��0��ʼ)
*   OperFlag -  RECORD_FIND,     //��ѯ
*           RECORD_UPDATE,     //����
*           RECORD_DEL,      //ɾ��
* RETURN:
*   FILE_SUCCESS    -  ���ݿ�����ɹ�
*   FILE_OPEN_FAIL/FILE_READ_FAIL/FILE_WRITE_FAIL/FILE_DELETE_FAIL  -  ���ݿ����ʧ��
*   FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
int File_OperRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index, char OperFlag)
{
    int   fh, recs;
    long    offset;

    if ((fh = File_Open(FileName, O_RDWR)) < 0)
    {
        return FILE_OPEN_FAIL;
    }

    /*ȡ��¼����*/
    recs = File_Lseek(fh, 0L, SEEK_END) / Record_Len;

    if (recs <= 0 || Record_Index >= recs || Record_Index < 0)
    {
        File_Close(fh);
        return FILE_NO_RECORD;
    }

    /*ָ�붨λ��Ҫ�����ļ�¼*/
    offset = Record_Len * Record_Index;
    File_Lseek(fh, (long)offset, SEEK_SET);

    if (OperFlag == RECORD_UPDATE)      /*���¼�¼*/
    {
        if (File_Write(fh, (void*)Record, Record_Len) != Record_Len)
        {
            File_Close(fh);
            return FILE_WRITE_FAIL;
        }
    }
    else if (OperFlag == RECORD_DEL)    /*ɾ����¼*/
    {
        if (File_Delete(fh, Record_Len) < 0)
        {
            File_Close(fh);
            return FILE_DELETE_FAIL;
        }
    }
    else if (OperFlag == RECORD_FIND)   /*��ѯ��¼*/
    {
        if (File_Read(fh, (void*)Record, Record_Len) != Record_Len)
        {
            File_Close(fh);
            return FILE_READ_FAIL;
        }
    }

    File_Close(fh);
    return FILE_SUCCESS;

}


/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetRecord.
* DESCRIPTION:  ͨ��������������һ����¼
* PARAMETERS:
*     FileName     (in)      - �ļ�����
*     Record       (out)     - ��¼����
*     Record_Len   (in)      - ��¼����
*     Condtion   (in)      - ��ѯ�������ο�DBSearCond�ṹ
*     OperFlag -  RECORD_FIND,     //��ѯ
*             RECORD_UPDATE,     //����
*             RECORD_DEL,      //ɾ��
* RETURN:
*     FILE_SUCCESS    -  ���ݿ�����ɹ�
*     FILE_OPEN_FAIL/FILE_READ_FAIL  -  ���ݿ����ʧ��
*     FILE_NO_RECORD -  ��¼δ�ҵ�
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_OperRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion, char OperFlag)
{
    int   fh;
    int   rec_index = 0;
    unsigned char tmp_DB[2048];
    int   recs;

    if ((fh = File_Open(FileName, O_RDWR)) < 0)
    {
        return FILE_OPEN_FAIL;
    }

    /*��ȡ��¼����*/
    recs = File_Lseek(fh, 0L, SEEK_END) / Record_Len;

    if (recs <= 0)
    {
        File_Close(fh);
        return FILE_NO_RECORD;
    }

    for (rec_index = 0; rec_index < recs ; rec_index++)
    {

        /*��ȡ�ļ�*/
        memset(tmp_DB, 0, sizeof(tmp_DB));
        File_Lseek(fh, (long)rec_index*Record_Len, SEEK_SET);

        if (File_Read(fh, (void*)tmp_DB, Record_Len) != Record_Len)
        {
            File_Close(fh);
            return FILE_READ_FAIL;
        }

        /*�Ƚ������Ƿ����*/
        if (!memcmp(tmp_DB + Condtion->StartOffset, Condtion->SearchKey, Condtion->Length))
        {
            File_Lseek(fh, (long)rec_index*Record_Len, SEEK_SET);

            if (OperFlag == RECORD_UPDATE)    /*���¼�¼*/
            {
                if (File_Write(fh, (void*)Record, Record_Len) != Record_Len)
                {
                    File_Close(fh);
                    return FILE_WRITE_FAIL;
                }
            }
            else if (OperFlag == RECORD_DEL)  /*ɾ����¼*/
            {
                if (File_Delete(fh, Record_Len) < 0)
                {
                    File_Close(fh);
                    return FILE_DELETE_FAIL;
                }
            }
            else if (OperFlag == RECORD_FIND) /*��ѯ��¼*/
            {
                memcpy((void*)Record, (void*)tmp_DB, Record_Len);
            }

            File_Close(fh);
            return FILE_SUCCESS;
        }
    }

    File_Close(fh);
    return FILE_NO_RECORD;

}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: tlv_tlv0.
* DESCRIPTION:  ����tlv����
* PARAMETERS:
*     rbuf    (in)  - ����Դ��TLV���ݼ��ϣ�
*     rlen    (in)  - ����Դ����
*     tlv     (out) - ������TLV����
* RETURN:
*       1(�ɹ�)/-1(ʧ��)
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int tlv_tlv0(const byte  *rbuf, int rlen, appTLV  *tlv)
{
    int i;

    if (rlen <= 4)
        return -1;

    i = 0;
    memcpy(&tlv->t, rbuf, 2);
    i += 2;

    memcpy(&tlv->l, rbuf + i, 2);
    i += 2;

    rlen -= i;
    rbuf += i;

    tlv->v = (byte*)rbuf;
    return 1;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: tlv_parseTLV.
* DESCRIPTION:  ����tlv����
* PARAMETERS:
*     rbuf    (in)  - ����Դ��TLV���ݼ��ϣ�
*     rlen    (in)  - ����Դ����
*     tlv     (out) - ������TLV����
* RETURN:
*     1    -  �ɹ�
*     <0   -  ʧ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int tlv_parseTLV(const byte  *rbuf, int rlen, appTLV  *tlv)
{
    int i;

    if ((i = tlv_tlv0(rbuf, rlen, tlv)) <= 0)
    {
        return i;
    }

    rlen -= (int)(tlv->v - rbuf);

    if (tlv->l > rlen)
    {
        return -1;
    }

    return 1;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: tb_find.
* DESCRIPTION:  ����tlv
* PARAMETERS:
*     b     (in)  - ����Դ��TLV���ݼ��ϣ�
*     l     (in)  - ����Դ����
*     tag     (in)  - �ؼ���
*     tlv     (out) - ������TLV����
* RETURN:
*     ��0    -  ���ҳɹ�
*     0  -      ���ݲ�����
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int tlv_find(const byte  *b, int l, usint tag, appTLV  *tlv)
{
    appTLV t;
    byte  *ptr;
    ptr = (byte *) b;

    if (tag == 0) return 0;

    while (tlv_parseTLV(b, l, &t) > 0)
    {
        //ApplLog_Debug("ATOOL", "t.t=[%d] t.l=[%d] t.v=[%x]", t.t, t.l, t.v);

        if (t.t == tag)
        {
            if (tlv != NULL)
                memcpy((char*)tlv, (char*)(&t), sizeof(appTLV));

            return (int)(t.v - ptr);
        }

        t.v += t.l;
        l -= (int)(t.v - b);
        b = t.v;
    }

    if (tlv != NULL)
    {
        memset((char*)tlv, 0, sizeof(appTLV));
        tlv->t = 0xff;
    }

    return 0;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: tb_find.
* DESCRIPTION:  ����tlv
* PARAMETERS:
*     tb      (in)  - ����Դ��TLV���ݼ��ϣ�
*     tag     (in)  - �ؼ���
*     tlv     (out) - ������TLV����
* RETURN:
*     ��0    -  ���ҳɹ�
*     0  -      ���ݲ�����
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int tb_find(const TLVbuf  *tb, usint tag, appTLV  *tlv)
{
    return tlv_find(tb->buf, tb->len, tag, tlv);
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: tb_del.
* DESCRIPTION:  ����tlv
* PARAMETERS:
*     tb      (in)  - ����Դ��TLV���ݼ��ϣ�
*     tag     (in)  - �ؼ���
* RETURN:
*     1   -  ɾ���ɹ�
*     0 -  ���ݲ�����
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int tb_del(TLVbuf  *tb, usint t)
{
    appTLV tlv;

    if (!tb_find(tb, t, &tlv)) return 0;

    tlv.l += 4;
    tlv.v -= 4;

    memcpy(tlv.v, tlv.v + tlv.l, tb->buf + tb->len - (tlv.v + tlv.l));
    tb->len -= tlv.l;
    return 1;
}

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: tb_add.
* DESCRIPTION:  ����tlv
* PARAMETERS:
*     tb      (in)  - ����Դ��TLV���ݼ��ϣ�
*     tlv     (in)  - ���ӵ�TLV����
* RETURN:
*     1   -  �ɹ�
*     0 -  ʧ��
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int tb_add(TLVbuf  *tb, appTLV  *tlv)
{
    byte  *b;

    if (tlv->t == 0)
        return 0;

    if (tlv->l == 0)
        return 0;

    tb_del(tb, tlv->t);

    if (tb->len + tlv->l >= tb->mlen)
        return 0;

    b = tb->buf + tb->len;

    memcpy(b, &tlv->t, 2);
    b += 2;

    memcpy(b, &tlv->l, 2);
    b += 2;

    memcpy(b, tlv->v, tlv->l);
    tlv->v = b;
    b += tlv->l;
    tb->len = (usint)(b - tb->buf);

    return 1;
}


