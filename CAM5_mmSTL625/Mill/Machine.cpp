/*	2010/9/25 nt
 *	implementation file of class CMachine
 */
#include "StdAfx.h"
#include "TRS.h"
#include "Tool.h"
#include "TSubGroup.h"
#include "TGroup.h"
#include "TLib.h"
#include "Machine.h"

///////////////////////////////////////////////////////////////
//	CMachine
/*	2010/9/25 nt
 *	CMachine
 */
CMachine::CMachine(int nAxis)
{
}

CMachine::~CMachine()
{
}
//--------------------------------------------------------------
int CMachine::Serialize(CArchive& ar)
{
	int ver = 0 ;

	if( ar.IsStoring() )
	{
		ar << ver
		   << m_name
		   << m_type
		   << m_nAxis ;

		ar.Write(&m_box, sizeof(BOX3D)) ;
		ar.Write(&m_lf, sizeof(RFRAME)) ;
	}
	else
	{
		ar >> ver
		   >> m_name
		   >> m_type
		   >> m_nAxis ;

		ar.Read(&m_box, sizeof(BOX3D)) ;
		ar.Read(&m_lf, sizeof(RFRAME)) ;
	}

	return 1 ;
}
///////////////////////////////////////////////////////////////