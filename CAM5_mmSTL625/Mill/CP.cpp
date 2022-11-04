/*	2010/9/25 nt
 *	implementation file of CP(cutting parameters)
 */
#include "StdAfx.h"
#include "CP.h"

///////////////////////////////////////////////////////////////
//	CP
/*	2010/9/25 nt
 *	CP
 */
void cpInit(CP* pCP)
{
	if( pCP )
	{
		for( int i = 0 ; i < MOTIONTYPE_MAXNUM ; i++ )
			pCP->speeds[i] = 0. ;
		pCP->spindle = 0 ;
		pCP->depth = 0. ;
	}
}
///////////////////////////////////////////////////////////////