/*	2001/11/28 nt
 *	implementation file of geometric information callback functions
 */
#include "StdAfx.h"

////////////////////////////////////////////////////////////////
//	write and read
int arWrite(void* ar, void* buf, int size)
{
	((CArchive*)ar)->Write(buf, size) ;

	return 1 ;
}

int arRead(void* ar, void* buf, int size)
{
	if( ((CArchive*)ar)->Read(buf, size) > 0 )
		return 1 ;

	return 0 ;
}

// nt add 2020/4/11
int arReadWrite(void* ar, int isLoad, void* buf, int size)
{
	if( isLoad == 0 )
	{
		((CArchive*)ar)->Write(buf, size) ;
		return 1 ;
	}
	else
	{
		if( ((CArchive*)ar)->Read(buf, size) > 0 )
			return 1 ;

		return 0 ;
	}
}
////////////////////////////////////////////////////////////////

