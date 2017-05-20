/************************************************
 * T9PY.C����ʹ����20Kȫ�ֱ����ռ�
 * ����ƴ��������ʹ��16Kȫ�ֱ����ռ�
 *************************************************/
#ifndef __T9PY_H__
#define __T9PY_H__

#define T9PY_NORMAL_MODE		0	//��������״̬����ƴ������״̬
#define T9PY_PY_MODE			1	//ƴ������״̬
#define T9PY_ABC_MODE			2	//��Сд��ĸ����״̬
#define	T9PY_NUMBER_MODE		3	//��������״̬
#define	T9PY_SYMBOL_MODE		4	//��������״̬

#define T9PY_MAX_PYLEN			6	//���ƴ������zhuang
#define T9PY_MAX_NOMINEES		10	//���ƴ���������(426=gao,hao...)

#define T9PY_SYMBOL_FLG1		1	//*��#�ż�������
#define T9PY_SYMBOL_FLG2		2	//��֧�ַ�������
#define T9PY_ABC_FLG			4	//��֧����ĸ����
#define T9PY_NUMBER_FLG			8	//��֧����������

#ifndef T9PY_HELP_FONT
#define T9PY_HELP_FONT		   "F:/6X8.vft"
#endif

#ifndef PYLIB
#define PYLIB			"F:/PYLIB"
#endif

#ifndef PYINDEX
#define PYINDEX			"F:/PYINDEX"
#endif

/*
 * void T9PY_Init(int disp, int select, int _ALPHAKEY, 
			 int _KEYA, int _KEYB, int _KEYC, int _KEYD)
 * ��ʼ��T9���뷨�����ù��ܼ�����ʾλ��
 * disp -- ������ַ���Ҫ��ʾ����λ�á�Ϊ0��ʹ��Ĭ��2, ���ַ�ʽ������
 * select -- ���뷨��ʾ����ʾ����ʼ��λ��.Ϊ0��ʹ��Ĭ��3,���ַ�ʽ������
 * _ALPHAKEY -- ���뷨ƴ���л��������뷨�л���Ϊ0��ʹ��Ĭ��ALPHA_KEY,
 * _KEYA -- ���뷨����ѡ�����Ϊ0��ʹ��Ĭ��KEY_a
 * _KEYB -- ���뷨����ѡ�����Ϊ0��ʹ��Ĭ��KEY_b
 * _KEYC -- ���뷨���ַ�ҳ����Ϊ0��ʹ��Ĭ��KEY_c
 * _KEYD -- ���뷨���ַ�ҳ����Ϊ0��ʹ��Ĭ��KEY_d
 * _DISABLE -- ���뷨֧�����ã�
              Ϊ0Ĭ������ȫ֧�֣���֧�ֺ��֣�֧����ĸ��֧�����֣�֧���ַ���֧��*�ź�#��
			  &1��֧�ַ������뵫������*�ź�#�ż�
			  &2��֧�ַ�������
			  &4��֧����ĸ����
			  &8��֧����������
*/
void T9PY_Init(int disp, int select, int _ALPHAKEY, 
			 int _KEYA, int _KEYB, int _KEYC, int _KEYD, int _DISABLE);

/* --------------------------------------------------------------------------
* FUNCTION NAME: T9PY_Input.
* DESCRIPTION:   T9ƴ���������뷨
* PARAMETERS:
*		szOutPut	(out) -- ������ַ���
*		iLen		(in)  -- �����ַ�������󳤶�
*		usTimeout	(in)  -- ��ʱʱ��(��)
* RETURN:
*		��0 -- ����ɹ��������ַ�������
*		INPUT_TIMEOUT/INPUT_USERABORT
* NOTES:
* ------------------------------------------------------------------------ */
int T9PY_Input(char * szOutPut, int iLen, usint usTimeout);

#ifdef _SHARED_LIB
void T9PY_InitPrm(void);
#endif

#endif
