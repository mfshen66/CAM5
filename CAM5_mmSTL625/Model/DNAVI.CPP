/*	2001/11/24 nt
 *	implementation file of dynamic navigation
 */
#include "StdAfx.h"
#include "DNAVI.h"

////////////////////////////////////////////////////////////////
/*	2001/11/24 nt
 *	initialize
 *	nt modify 2009/5/9
 */
int dnaviInit(DNAVI* dn, 
			  double pos[3],
			  double dir[3],
			  double r)
{
	if( !dn )
		return 0 ;

	dn->pos[0] = pos != NULL ? pos[0] : 0. ; // init pos
	dn->pos[1] = pos != NULL ? pos[1] : 0. ;
	dn->pos[2] = pos != NULL ? pos[2] : 0. ;
	dn->dir[0] = dir != NULL ? dir[0] : 0. ; // init dir
	dn->dir[1] = dir != NULL ? dir[1] : 0. ;
	dn->dir[2] = dir != NULL ? dir[2] : 1. ;
	dn->r = r ; // init r

	dn->type = -1 ;
	dn->featType = -1 ; 
	dn->feat = NULL ;
	dn->nav_p[0] = 0. ; // init nav_p
	dn->nav_p[1] = 0. ;
	dn->nav_p[2] = 0. ;

	return 1 ;
}
////////////////////////////////////////////////////////////////
