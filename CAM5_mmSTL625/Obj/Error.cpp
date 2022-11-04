/*	1999/7/5 nt
 *	implementation file of Error Control for system
 */
#include "StdAfx.h"

////////////////////////////////////////////////////////////////
// global variable for error control
static int stError = 0 ;

int stGetError()
{
	return stError ;
}

void stSetError(int error)
{
	stError = error ;
}
////////////////////////////////////////////////////////////////
