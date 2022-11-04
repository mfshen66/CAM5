/*	2002/10/5 nt
 *	implementation file of class CCarrier
 */
#include "StdAfx.h"

///////////////////////////////////////////////////////////////
//	carrier
/*	2002/10/5 nt
 *	CCarrier
 */
CCarrier::CCarrier()
{
	m_vp = NULL ;
}

CCarrier::~CCarrier()
{
}

void* CCarrier::GetVP()
{
	return m_vp ;
}

void CCarrier::SetVP(void* vp)
{
	m_vp = vp ;
}
///////////////////////////////////////////////////////////////
