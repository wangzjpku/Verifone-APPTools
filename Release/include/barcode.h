#ifndef	__BARCODE_H__
#define __BARCODE_H__

typedef enum
{
	BARCODE_TIMEOUT   = -2,      /* 读条码超时*/
	BARCODE_FAIL      = -1,      /* 读条码失败*/
	BARCODE_OK        = 0,       /* 读条码成功*/
}enumBarcodeRet;

/*Operation Type */
typedef  enum
{ 
	EDGE_TRIGGER_MODE = 0x01,
	LEVEL_TRIGGER_MODE= 0x02,
	SOFT_TRIGGER_MODE = 0x04,
} TriggerMode;

/* --------------------------------------------------------------------------
* FUNCTION NAME: Barcode_Activate.
* DESCRIPTION:   Activate Scanner.  Powers up the barcode scanner.
* PARAMETERS:
* RETURN:
*		BARCODE_OK/BARCODE_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Barcode_Activate(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Barcode_Deactivate.
* DESCRIPTION:   Deactivate scanner. Powers off the scanner engine
* PARAMETERS:
* RETURN:
*		BARCODE_OK/BARCODE_FAIL
* NOTES:
* ------------------------------------------------------------------------ */
int Barcode_Deactivate(void);

/* --------------------------------------------------------------------------
* FUNCTION NAME: Barcode_Scan.
* DESCRIPTION:   barcode scan
* PARAMETERS:
	trigger_mode(in) - see enum TriggerMode
	timeout(in)	- in seconds
	barcode(out)
	barcode_Len(out)
* RETURN:
*		BARCODE_OK/BARCODE_FAIL/BARCODE_TIMEOUT
* NOTES:
* ------------------------------------------------------------------------ */
int Barcode_Scan(char trigger_mode, int timeout, char *barcode, int *barcode_Len);

#endif	/* __BARCODE_H__ */
