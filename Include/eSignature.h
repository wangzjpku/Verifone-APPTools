#ifndef __ESIGNATURE_H__
#define __ESIGNATURE_H__

/*����ǩ������*/
typedef enum
{
	ESIGN_OK		=  0,		/*ǩ������*/
	ESIGN_FAIL		= -1,		/*ǩ������*/
	ESIGN_FILEFAIL  = -2,		/*�ļ�����ʧ��*/
	ESIGN_CANCEL	= -3,		/*ǩ��ȡ��*/
	ESIGN_TIMEOUT	= -4,		/*ǩ����ʱ*/
	ESIGN_NOMEMORY	= -5,
}enumEsignRet;

typedef enum
{
	INNER_SIGNPAD 		= 0,	/*����ǩ����*/
	EXTERNAL_SIGNPAD	= 1,	/*����ǩ����*/
	UNSUPPORT_SIGNPAD	= -1,	/* ��֧�� */
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

//added by baijz 20120828 ����ǩ�ְ嶨��
#define   SIGN_HANDSHAKE_REQ						0xA0	//��������
#define   SIGN_HANDSHAKE_RES						0xB0	//������Ӧ

#define   SIGN_INFORM_UPLOAD_STATUS_REQ				0xA1	//ǰһ��ǩ���ϴ�״̬���֪ͨ
#define   SIGN_INFORM_UPLOAD_STATUS_RES				0xB1	//ǰһ��ǩ���ϴ�״̬�����Ӧ

#define   SIGN_SIGNATURE_BEGIN_REQ          		0xA2	//ǩ����������
#define   SIGN_SIGNATURE_BEGIN_RES_OK               0xB2	//ǩ����ɳɹ���Ӧ
#define   SIGN_SIGNATURE_BEGIN_RES_ERR              0xC2	//ǩ������쳣��Ӧ

#define   SIGN_SIGNATURE_END_REQ             		0xA3	//ǩ�ֽ�������
#define   SIGN_SIGNATURE_END_RES					0xB3	//ǩ�ֽ�����Ӧ

#define   SIGN_BATCH_UPLOAD_BEGIN_REQ				0xA4	//�ϴ�ʧ��ǩ��������������
#define   SIGN_BATCH_UPLOAD_BEGIN_RES				0xB4	//�ϴ�ʧ��ǩ������������Ӧ

#define   SIGN_BATCH_UPLOAD_END_REQ					0xA5	//�ϴ�ʧ��ǩ�����������������
#define   SIGN_BATCH_UPLOAD_END_RES					0xB5	//�ϴ�ʧ��ǩ���������������Ӧ

#define	  SIGN_BATCH_UPLOAD_RECV_OK					0xA8	//�ϴ�ʧ��ǩ������������ճɹ�
#define	  SIGN_BATCH_UPLOAD_RECV_ERR				0xA9	//�ϴ�ʧ��ǩ�������������ʧ��

#define   SIGN_STATUS_DELETEED                      0x01  //����ǩ����ɾ�� 
#define   SIGN_STATUS_SAVE_OK						0x02  //����ǩ���Ѵ洢 
#define   SIGN_STATUS_LOW_STORAGE					0x03  //����ǩ�ֳɹ��洢��ʣ�ڴ洢�ռ����10�� 
#define   SIGN_STATUS_SAVE_ERR                      0x04  //����ǩ�ִ洢ʧ��

#define	  SIGN_SIGN_TIMEOUT							0xD0	//��������ǩ���泬ʱʱ��

#define	  SIGN_RESIGN_TIME							0xF8	//����ǩ�����ǩ������
#define	  SIGN_WRITING_DEGREE						0xF9	//����ǩ����ǩ���ʼ��Ĵ�ϸ�̶�
#define	  SIGN_BYPASS								0xE5	//�����Ƿ�����հ�ǩ��ͼƬ
#define	  SIGN_SETPIC_ZONE							0xE3	//���ͼƬ��С
#define   SIGN_MAX_BUF_LEN							1024*10+1

typedef struct
{
    unsigned char  Stx;								// �������ݰ�
    unsigned char  Len[2];							// �������ݵ�Ԫ����
    unsigned char  Data[SIGN_MAX_BUF_LEN];			// �������ݵ�Ԫ������
    unsigned char  Etx;								// �������ݰ�β
    unsigned char  Lrc;								// �������ݵ�Ԫ���ֵ�LRCУ����
} SIGN_COM_DATA;


void eSign_SetComPort( char *pcComString );

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_Signature
* DESCRIPTION:   ���ǩ����������ϳɵ�λͼ�ļ�
* PARAMETERS:
*   code   	(in)  	-- ������
*   len  	(in)  	-- �����볤��
*   timeout (in)  	-- ��ʱʱ��
*	signpad_type(in)-- ǩ��������,�μ�SIGNPAD_TYPE
*	f_bitmap(out) 	-- ���λͼ�ļ�����
	times(in)         --����������� 0-���޴�
	bypass��in��	--�װ�ʱ������ȷ�ϼ��Ƿ��ù�
* RETURN: �ο�enumEsignRet����
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_Signature(char * code, int len, int timeout, char signpad_type, char *f_bitmap, int times,boolean bypass);

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_PrintPBMImage
* DESCRIPTION:   ǩ����ӡ����
* PARAMETERS:
*   f_bitmap(in)  -- ����PBM�ļ�����
*	cPosFlag(in)  -- �ο� ESIGN_PRN_POS
* RETURN: �ο�enumEsignRet����
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_PrintPBMImage(const char *f_bitmap, char cPosFlag);

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_CompressImage
* DESCRIPTION:   ǩ��ѹ������,pbm->jbg
* PARAMETERS:
*   fnin	(in)  -- PBMǩ���ļ�
*   fnout  	(out) -- JBIGѹ������ļ�
* RETURN: �ο�enumEsignRet����
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_CompressImage(const char *fnin, const char *fnout);

/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_UnCompressImage
* DESCRIPTION:   ǩ����ѹ����,jbg->pbm
* PARAMETERS:
*   fnin	(in)  -- JBIGѹ������ļ�
*   fnout  	(out) -- PBMǩ���ļ�
* RETURN: �ο�enumEsignRet����
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_UnCompressImage(const char *fnin, const char *fnout);

int eSign_Check(char signpad_type);

#if 0
/* --------------------------------------------------------------------------
* FUNCTION NAME: eSign_SetSignatureTimeout
* DESCRIPTION:   ��������ǩ���峬ʱʱ��
* PARAMETERS:
*   iTimeOut  	(in) -- ��ʱʱ��<256s
* RETURN: Ŀǰ��ʱ���Vx820
* NOTES:
* ------------------------------------------------------------------------ */
int eSign_SetSignatureTimeout( unsigned int iTimeOut );
#endif

#endif

