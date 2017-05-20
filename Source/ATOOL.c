/******************************************************************
  Filename   :ATOOL.C
  Product    :ATOOL Library
  Version    :1.0
  Module     :ATOOL
  Description:	This is the sample implementation of Verix V Shared library application

  Modification History:
   	 #		Date        		Who			Comments
	1.0		2010-8-18 10:36:47	    Administrator		Initial Implementation
*******************************************************************/

/******************************************************************
   Copyright (C) 2008-2010 by VeriFone Inc. All rights reserved.

 No part of this software may be used, stored, compiled, reproduced,
 modified, transcribed, translated, transmitted, or transferred, in
 any form or by any means  whether electronic, mechanical,  magnetic,
 optical, or otherwise, without the express prior written permission
                          of VeriFone, Inc.
*******************************************************************/
#include "apptools.h"
#include "logsys.h"

#ifdef _SHARED_LIB
int main(int argc, char **argv)
{
    // initialize your ATOOL Library initialized static data here

    MmiUtil_InitPrm();

	T9PY_InitPrm();
	
	Print_InitPrm();
}
#endif
