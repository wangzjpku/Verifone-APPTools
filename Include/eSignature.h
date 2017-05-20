#ifndef __ESIGNATURE_H__
#define __ESIGNATURE_H__

/*电子签名返回*/
typedef enum
{
	ESIGN_OK		=  0,		/*签名正常*/
	ESIGN_FAIL		= -1,		/*签名故障*/
	ESIGN_FILEFAIL  = -2,		/*文件操作失败*/
	ESIGN_CANCEL	= -3,		/*签名取消*/
	ESIGN_TIMEOUT	= -4,		/*签名超时*/
	ESIGN_NOMEMORY	= -5,
}enumEsignRet;

typedef enum
{
	INNER_SIGNPAD 		= 0,	/*内置签名板*/
	EXTERNAL_SIGNPAD	= 1,	/*外置签名板*/
	UNSUPPORT_SIGNPAD	= -1,	/* 不支持 */
}SIGNPAD_TYPE;


typedef enum
{
	PRN_RIGHT		= 0,
	PRN_CENTER		= 1,
	PRN_LEFT		= 2,
}ESIGN_PRN_POS;

#define INNER_SIGN_WIDTH 		240
#define INNER_SIGN_HEIGHT 		120


#define SIGN_COLOR 0x000000FF


#define PRINT_WIDTH	384

//added by baijz 20120828 外置签字板定义
#define   SIGN_HANDSHAKE_REQ						0xA0	//握手请求
#define   SIGN_HANDSHAKE_RES						0xB0	//握手响应

#define   SIGN_INFORM_UPLOAD_STATUS_REQ				0xA1	//前一笔签字上传状态结果通知
#define   SIGN_INFORM_UPLOAD_STATUS_RES				0xB1	//前一笔签字上传状态结果响应

#define   SIGN_SIGNATURE_BEGIN_REQ          		0xA2	//签字输入请求
#define   SIGN_SIGNATURE_BEGIN_RES_OK               0xB2	//签字完成成功响应
#define   SIGN_SIGNATURE_BEGIN_RES_ERR              0xC2	//签字完成异常响应

#define   SIGN_SIGNATURE_END_REQ             		0xA3	//签字结束请求
#define   SIGN_SIGNATURE_END_RES					0xB3	//签字结束响应

#define   SIGN_BATCH_UPLOAD_BEGIN_REQ				0xA4	//上传失败签字批量传输请求
#define   SIGN_BATCH_UPLOAD_BEGIN_RES				0xB4	//上传失败签字批量传输响应

#define   SIGN_BATCH_UPLOAD_END_REQ					0xA5	//上传失败签字批量传输结束请求
#define   SIGN_BATCH_UPLOAD_END_RES					0xB5	//上传失败签字批量传输结束响应

#define	  SIGN_BATCH_UPLOAD_RECV_OK					0xA8	//上传失败签字批量传输接收成功
#define	  SIGN_BATCH_UPLOAD_RECV_ERR				0xA9	//上传失败签字批量传输接收失败

#define   SIGN_STATUS_DELETEED                      0x01  //电子签字已删除 
#define   SIGN_STATUS_SAVE_OK						0x02  //电子签字已存储 
#define   SIGN_STATUS_LOW_STORAGE					0x03  //电子签字成功存储，剩于存储空间低于10笔 
#define   SIGN_STATUS_SAVE_ERR                      0x04  //电子签字存储失败

#define	  SIGN_SIGN_TIMEOUT							0xD0	//设置外置签名版超时时间

#define	  SIGN_RESIGN_TIME							0xF8	//设置签名版的签名次数
#define	  SIGN_WRITING_DEGREE						0xF9	//设置签名版签名笔迹的粗细程度
#define	  SIGN_BYPASS								0xE5	//设置是否输出空白签字图片
#define	  SIGN_SETPIC_ZONE							0xE3	//输出图片大小
#define   SIGN_MAX_BUF_LEN							1024*10+1

typedef struct
{
    unsigned char  Stx;								// 命令数据包
    unsigned char  Len[2];							// 命令数据单元长度
    unsigned char  Data[SIGN_MAX_BUF_LEN];			// 命令数据单元缓冲区
    unsigned char  Etx;								// 命令数据包尾
    unsigned char  Lrc;								// 命令数据单元部分的LRC校验码
} SIGN_COM_DATA;


void eSign_SetComPort( char *pcComString );

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_Signature
* DESCRIPTION:   输出签名及特征码合成的位图文件
* PARAMETERS:
*   code   	(in)  	-- 特征码
*   len  	(in)  	-- 特征码长度
*   timeout (in)  	-- 超时时间
*	signpad_type(in)-- 签名板类型,参见SIGNPAD_TYPE
*	f_bitmap(out) 	-- 输出位图文件名称
	times(in)         --允许重输次数 0-无限次
	bypass（in）	--白板时，按下确认键是否让过
* RETURN: 参考enumEsignRet定义
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_Signature(char * code, int len, int timeout, char signpad_type, char *f_bitmap, int times,boolean bypass);

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_PrintPBMImage
* DESCRIPTION:   签名打印函数
* PARAMETERS:
*   f_bitmap(in)  -- 输入PBM文件名称
*	cPosFlag(in)  -- 参考 ESIGN_PRN_POS
* RETURN: 参考enumEsignRet定义
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_PrintPBMImage(const char *f_bitmap, char cPosFlag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_CompressImage
* DESCRIPTION:   签名压缩函数,pbm->jbg
* PARAMETERS:
*   fnin	(in)  -- PBM签名文件
*   fnout  	(out) -- JBIG压缩输出文件
* RETURN: 参考enumEsignRet定义
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_CompressImage(const char *fnin, const char *fnout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_UnCompressImage
* DESCRIPTION:   签名解压函数,jbg->pbm
* PARAMETERS:
*   fnin	(in)  -- JBIG压缩输出文件
*   fnout  	(out) -- PBM签名文件
* RETURN: 参考enumEsignRet定义
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_UnCompressImage(const char *fnin, const char *fnout);

int eSign_Check(char signpad_type);

#if 0
/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_SetSignatureTimeout
* DESCRIPTION:   设置外置签名板超时时间
* PARAMETERS:
*   iTimeOut  	(in) -- 超时时间<256s
* RETURN: 目前暂时针对Vx820
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_SetSignatureTimeout( unsigned int iTimeOut );
#endif

#endif

