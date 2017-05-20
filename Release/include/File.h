#ifndef __FILE_H__
#define __FILE_H__

/*文件操作模式*/
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

/*文件指针定位*/
typedef  enum
{
	F_SEEK_SET = 0,
	F_SEEK_CUR,
	F_SEEK_END 
}FILE_SEEK_FLAGS;


/*Error Code */
typedef enum
{
	FILE_SUCCESS		=  0,              //文件操作成功
	FILE_FAIL			= -1,              //异常
	FILE_OPEN_FAIL  	= -2,              //打开错误
	FILE_CREATE_FAIL 	= -3,              //文件重建错误
	FILE_WRITE_FAIL		= -4,              //写错误
	FILE_READ_FAIL		= -5,              //读错误
	FILE_SEEK_FAIL		= -6,              //定位文件指针错误
	FILE_DELETE_FAIL	= -7,              //删除文件记录错误
	FILE_CLOSE_FAIL		= -8,              //关闭文件错误
	FILE_REMOVE_FAIL	= -9,              //关闭文件错误
	FILE_NO_RECORD		= -10,              //记录未找到
}enumFileRet2;

typedef struct
{
	usint   StartOffset;/*从结构的第几个字节开始查找*/
	usint   Length;      /*比较字节长度*/
	byte*   SearchKey;   /*查找关键字*/
}DBSearCond;

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_DirGetFirst
* DESCRIPTION:获取目录下第一个文件名
* PARAMETERS:
*		drive (in)	- I:(for arm)/F:(for flash)
*		drive (out) - 该目录下第一个文件名
* RETURN:
*		0  -- 成功
*		-1 -- 失败
* NOTES:
* ------------------------------------------------------------------------ */
int File_DirGetFirst(char *drive);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_DirGetNext
* DESCRIPTION:获取目录下下一个文件名
* PARAMETERS:
*		buffer (in)	- 该目录下当前文件名
*		buffer (out) - 该目录下下一个文件名
* RETURN:
*		0  -- 成功
*		-1 -- 失败
* NOTES:
* ------------------------------------------------------------------------ */
int File_DirGetNext(char *buffer);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Open
* DESCRIPTION:打开文件
* PARAMETERS:
*		FileName (in)	- 打开文件名, 以NULL结束，最长32个字节
*		Flag     (in) - 打开文件方式, 取值参考FILE_FLAGS定义
* RETURN:
*		file_handle -- 文件open成功
*		FILE_OPEN_FAIL -- 文件open失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Open(const char *FileName, int Flag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Read
* DESCRIPTION:读文件记录
* PARAMETERS:
*		Handle (in)	 -- 文件句柄
*		buffer (out) -- 读出数据
*		count  (in)  -- 读数据的最大长度
* RETURN:
*		bytes_read -- 读取数据的实际长度
*		FILE_READ_FAIL -- 读文件失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Read(int handle, char *buffer, int count);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_ReadLine.
* DESCRIPTION:  读一行文本
* PARAMETERS:
*			pFile		(in)	- 文件handle
*			pLineBuff	(out)	- 读取的文本数据
*			LineBuffSize(in)	- 缓冲大小
* RETURN:
*			buf_len    -  数据长度（数据库操作成功）
*			EOF		   -  读文件结束
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_ReadLine(int pFile, char *pLineBuff,unsigned int LineBuffSize);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Write
* DESCRIPTION:写文件记录
* PARAMETERS:
*		Handle (in)	 -- 文件句柄
*		buffer (in)  -- 写入的数据
*		count  (in)  -- 写入数据的长度
* RETURN:
*		bytes_written -- 写入数据的长度
*		FILE_WRITE_FAIL -- 写文件失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Write(int handle, char *buffer, int count);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Lseek
* DESCRIPTION:定位文件指针
* PARAMETERS:
*		Handle (in)	 -- 文件句柄
*		offset (in)  -- 指针移动的便宜量
*		origin (in)  -- 开始位置，参考FILE_SEEK_FLAGS
* RETURN:
*		bytes_seek -- 文件指针距离文件开头的字节数
*		FILE_SEEK_FAIL -- 定位文件指针失败
* NOTES: 
* ------------------------------------------------------------------------ */
long File_Lseek(int handle, long offset, int origin);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Delete
* DESCRIPTION:删除文件记录
* PARAMETERS:
*		Handle (in)	 -- 文件句柄
*		count  (in)  --  删除数据长度
* RETURN:
*		FILE_SUCCESS     -- 删除文件记录成功
*		FILE_DELETE_FAIL -- 删除文件记录失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Delete(int handle, unsigned int count);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Close
* DESCRIPTION:关闭文件
* PARAMETERS:
*		Handle (in)	 -- 文件句柄
* RETURN:
*		FILE_SUCCESS     -- 关闭文件成功
*		FILE_CLOSE_FAIL -- 关闭文件失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Close(int handle);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Remove
* DESCRIPTION:删除文件
* PARAMETERS:
*		FileName (in)	- 删除文件名, 以NULL结束，最长32个字节
* RETURN:
*		FILE_SUCCESS     -- 关闭文件成功
*		FILE_REMOVE_FAIL -- 删除文件失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Remove(const char *filename);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Rename
* DESCRIPTION: 重命名文件
* PARAMETERS:
*			oldname (in)	- 旧文件名
*			newname (in)    - 新文件名
* RETURN:
*			FILE_SUCCESS	-- 关闭文件成功
*			FILE_FAIL		-- 重命名文件失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Rename(const char *oldname, const char *newname);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_Clear
* DESCRIPTION:清除文件
* PARAMETERS:
*		FileName (in)	- 清除文件名, 以NULL结束，最长32个字节
* RETURN:
*		FILE_SUCCESS   -- 清除文件成功
*		FILE_OPEN_FAIL -- 清除文件失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_Clear(const char *FileName);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetFreeSpace.
* DESCRIPTION:  取文件系统剩余空间
* PARAMETERS:    
*		drive     (in)      - 驱动名称("I:"或"F:")
* RETURN:       
*		Avail     -  剩余空间
*		FILE_FAIL -  文件系统操作失败
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
long File_GetFreeSpace(const char *drive);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetNumberOfRecords.
* DESCRIPTION:  取文件记录条数
* PARAMETERS:    
*		FileName     (in)      - 文件名称
*		Record_Len   (in)      - 记录长度
* RETURN:       
*		num_record     -  记录条数
*		FILE_OPEN_FAIL -  文件操作失败
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
int File_GetNumberOfRecords(const char *FileName, int Record_Len);

/* --------------------------------------------------------------------------
* FUNCTION NAME: File_AppendRecord
* DESCRIPTION:在文件末尾，添加一条定长记录文件。当文件不存在时，自动创建该文件；
* PARAMETERS:
*		FileName    (in)	-- 文件名, 以NULL结束，最长32个字节
*		Record      (in)	-- 记录数据
*		Record_Len  (in)	-- 记录数据的长度
* RETURN:
*		FILE_SUCCESS   -- 追加记录成功
*		FILE_CREATE_FAIL/FILE_WRITE_FAIL -- 追加记录失败
* NOTES: 
* ------------------------------------------------------------------------ */
int File_AppendRecord(const char *FileName, char *Record, int Record_Len);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetRecordByIndex.
* DESCRIPTION:  通过记录索引查找任意一条记录
* PARAMETERS:    
*		FileName     (in)      - 文件名称
*		Record       (out)     - 记录数据
*		Record_Len   (in)      - 记录长度
*		Record_Index (in)      - 记录索引(从0开始)
* RETURN:       FILE_SUCCESS    -  数据库操作成功
*               FILE_OPEN_FAIL/FILE_READ_FAIL  -  数据库操作失败
*               FILE_NO_RECORD -  记录未找到
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_UpdateRecordByIndex.
* DESCRIPTION:  通过记录索引更新任意一条记录
* PARAMETERS:    
*		FileName     (in)      - 文件名称
*		Record       (in)      - 记录数据
*		Record_Len   (in)      - 记录长度
*		Record_Index (in)      - 记录索引(从0开始)
* RETURN:       FILE_SUCCESS    -  数据库操作成功
*               FILE_OPEN_FAIL/FILE_WRITE_FAIL  -  数据库操作失败
*               FILE_NO_RECORD -  记录未找到
* NOTES:
* -------------------------------------------------------------------------------------------------- */
int File_UpdateRecordByIndex(const char *FileName, void *Record, int Record_Len, int Record_Index);


/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteRecordByIndex.
* DESCRIPTION:  通过记录索引删除任意一条记录
* PARAMETERS:    
*		FileName    (in)  - 文件名称
*		Record_Len  (in)  - 记录长度
* 		Record_Index(in)  - 记录索引(从0开始)
* RETURN:       FILE_SUCCESS    -  数据库操作成功
*               FILE_OPEN_FAIL/FILE_DELETE_FAIL  -  数据库操作失败
*               FILE_NO_RECORD -  记录未找到
* NOTES:
* ---------------------------------------------------------------------------------------------------- */
int File_DeleteRecordByIndex(const char *FileName, int Record_Len, int Record_Index);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetRecord.
* DESCRIPTION:  通过条件查找任意一条记录
* PARAMETERS:    
*			FileName     (in)      - 文件名称
*			Record       (out)     - 记录数据
*			Record_Len   (in)      - 记录长度
*			Condtion	 (in)      - 查询条件，参考DBSearCond结构
* RETURN:       
*			FILE_SUCCESS    -  数据库操作成功
*			FILE_OPEN_FAIL/FILE_READ_FAIL  -  数据库操作失败
*			FILE_NO_RECORD -  记录未找到
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_UpdateRecord.
* DESCRIPTION:  通过条件更新任意一条记录
* PARAMETERS:    
*			FileName     (in)      - 文件名称
*			Record       (in)     - 记录数据
*			Record_Len   (in)      - 记录长度
*			Condtion	 (in)      - 查询条件，参考DBSearCond结构
* RETURN:       
*			FILE_SUCCESS    -  数据库操作成功
*			FILE_OPEN_FAIL/FILE_READ_FAIL  -  数据库操作失败
*			FILE_NO_RECORD -  记录未找到
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_UpdateRecord(const char *FileName, void *Record, int Record_Len, DBSearCond *Condtion);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteRecord.
* DESCRIPTION:  通过条件删除任意一条记录
* PARAMETERS:    
*			FileName     (in)      - 文件名称
*			Record_Len   (in)      - 记录长度
*			Condtion	 (in)      - 查询条件，参考DBSearCond结构
* RETURN:       
*			FILE_SUCCESS    -  数据库操作成功
*			FILE_OPEN_FAIL/FILE_DELETE_FAIL  -  数据库操作失败
*			FILE_NO_RECORD -  记录未找到
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_DeleteRecord(const char *FileName, int Record_Len, DBSearCond *Condtion);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_ReadTLV.
* DESCRIPTION:  读非定长记录TLV
* PARAMETERS:    
*			FileName	(in)	- 文件名称
*			FldID		(in)	- 标签(Tag)
*			Data		(in)	- 数据(Value)
*			DataLen		(in)	- 长度(length)
* RETURN:       
*			FILE_SUCCESS    -  数据库操作成功
*			FILE_OPEN_FAIL/FILE_READ_FAIL  -  数据库操作失败
*			FILE_NO_RECORD -  记录未找到
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_ReadTLV(char *FileName, usint FldID, char *Data, usint *DataLen);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_WriteTLV.
* DESCRIPTION:  写非定长记录TLV
* PARAMETERS:    
*			FileName	(in)	- 文件名称
*			FldID		(in)	- 标签(Tag)
*			Data		(in)	- 数据(Value)
*			DataLen		(in)	- 长度(length)
* RETURN:       
*			FILE_SUCCESS    -  数据库操作成功
*			FILE_OPEN_FAIL/FILE_READ_FAIL/FILE_WRITE_FAIL  -  数据库操作失败
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_WriteTLV(char *FileName,usint FldID,byte *Data,usint DataLen);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_DeleteTLV.
* DESCRIPTION:  删除非定长记录TLV
* PARAMETERS:    
*			FileName	(in)	- 文件名称
*			FldID		(in)	- 标签(Tag)
* RETURN:       
*			FILE_SUCCESS    -  数据库操作成功
*			FILE_OPEN_FAIL/FILE_READ_FAIL/FILE_WRITE_FAIL  -  数据库操作失败
*			FILE_NO_RECORD -  记录未找到
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_DeleteTLV(char *FileName, usint FldID);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_GetKey.
* DESCRIPTION:  读DLD参数
* PARAMETERS:    
*			FileName	(in)	- 文件名称
*			key			(in)	- 关键字
*			buf			(out)	- 数据
*			size		(in)	- 数据长度
* RETURN:       
*			buf_len    -  数据长度（数据库操作成功）
*			FILE_FAIL  -  数据库操作失败
*			FILE_NO_RECORD -  记录未找到
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_GetKey(const char *key, char *buf, int size, const char *filename);

/* --------------------------------------------------------------------------------------------------
* FUNCTION NAME: File_PutKey.
* DESCRIPTION:  写DLD参数
* PARAMETERS:    
*			FileName	(in)	- 文件名称
*			key			(in)	- 关键字
*			buf			(in)	- 数据
*			size		(in)	- 数据长度
* RETURN:       
*			buf_len    -  数据长度（数据库操作成功）
*			FILE_FAIL  -  数据库操作失败
* NOTES:
* ----------------------------------------------------------------------------------------------- */
int File_PutKey(const char *key, const char *buf, int size, const char *filename);

#endif
