/*	2010/4/18 nt
 *	implementation file of class CVO
 */
#include "StdAfx.h"

////////////////////////////////////////////////////////////////
//	CVO
/*	2010/4/18 nt
 *	Class CVO
 */
CVO::CVO()
{
	m_flags = 0 ;
}

CVO::~CVO()
{
}
//--------------------------------------------------------------
// return 1=picked, 0=not picked
int CVO::IsPicked()
{
	return m_flags&VO_PICKED ;
}

int CVO::SetPick(int pick)
{
	if( pick ) // set picked
		m_flags |= VO_PICKED ;
	else // set unpicked
		m_flags &= (0xFFFF-VO_PICKED) ;
	return 1 ;
}
////////////////////////////////////////////////////////////////