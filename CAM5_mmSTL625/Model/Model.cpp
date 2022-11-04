/*	2001/11/16 nt
 *	implementation file of class CModel
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Model.h"

////////////////////////////////////////////////////////////////
/*	2001/11/16 nt
 *
 */
CModel::CModel()
{
	// nt modify 2009/5/9
	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;
	m_state = msNormal ;
}

CModel::~CModel()
{
}
//--------------------------------------------------------------
int CModel::GetBox(BOX3D* box)
{
	if( !box )
		return 0 ;

	*box = m_box ;

	return 1 ;
}

int CModel::SetBox(BOX3D* box)
{
	if( !box )
		return 0 ;

	m_box = *box ;

	return 1 ;
}

int CModel::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		int sv = m_state+2048 ; // nt add 2010/9/9
		ar << sv ;
		ar.Write(&m_box, sizeof(BOX3D)) ;
	}
	else
	{
		ar >> m_state ;
		ar.Read(&m_box, sizeof(BOX3D)) ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
