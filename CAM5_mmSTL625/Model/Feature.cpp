/*	2001/11/17 nt
 *	implementation file of class CFeature
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"

///////////////////////////////////////////////////////////////
//	feature
/*	2001/11/17 nt
 *	feature
 */
CFeature::CFeature()
{
	m_state = fsNormal ;
	// initialize box
	m_box.min[0] = m_box.min[1] = m_box.min[2] = MAX_DBL ;
	m_box.max[0] = m_box.max[1] = m_box.max[2] = -MAX_DBL ;
	// nt add 2009/12/22
	m_color = 0 ;
	// nt add 2010/9/9-----------------------------------
	m_featId = 0 ;
	m_name = "" ;
	m_createTime = 0 ;
	m_it = 0 ;
	m_waitUpdateFlag = 0 ;
	m_flags = 15 ;
	m_hItem = 0 ; // nt add 2010/9/9
	// end
	m_no = -1 ; // nt add 2010/9/26
}

CFeature::~CFeature()
{
}
//--------------------------------------------------------------
int CFeature::GetNo()
{
	return m_no ;
}

void CFeature::SetNo(int no)
{
	m_no = no ;
}

int CFeature::GetState()
{
	return m_state ;
}

void CFeature::SetState(int state)
{
	m_state = state ;
}

// nt add 2010/9/9
HTREEITEM CFeature::GetItem()
{
	return m_hItem ;
}

// nt add 2010/9/9
void CFeature::SetItem(HTREEITEM hItem)
{
	m_hItem = hItem ;
}

void CFeature::GetName(CStr& name)
{
	name = m_name ;
}

void CFeature::GetName(CString& name)
{
	name = m_name;
}

// nt add 2012/10/12
int CFeature::GetFlags()
{
	return m_flags ;
}

// nt add 2012/10/12
void CFeature::SetFlags(int flags)
{
	m_flags = flags ;
}

void CFeature::SetName(CStr& name)
{
	m_name = name ;
}

void CFeature::SetName(CString& name)
{
	m_name = name;
}

// nt add 2009/12/21
int CFeature::GetColor()
{
	return m_color ;
}

// nt add 2009/12/21
int CFeature::SetColor(int color)
{
	m_color = color ;
	return 1 ;
}

// nt add 2009/12/21
// nt modify 2011/7/3
int CFeature::SetDrawColor(int drawMode)
{
	if( m_color <= 255 )
	{
		// nt add 2011/7/3
		/*MATERIAL material ;
		materialGet2(&material, 5) ;
		materialSet(&material) ;*/

		if( drawMode == dmNormal ||
			drawMode == dmShaded ||
			drawMode == dmWireframe )
		{
			unsigned char rgb[3] = {0, 0, 0} ;
			rgbsGet(m_color, rgb) ;
			glColor3ub(rgb[0], rgb[1], rgb[2]) ;
		}
		else
		if( drawMode == dmPrompt )
			glColor3ub(255, 0, 0) ;
		else
		if( drawMode == dmSelect )
			glColor3ub(100, 100, 255) ;
		else
			glColor3ub(50, 50, 100) ;
	}
	else
	{
		MATERIAL material ;
		if( drawMode == dmNormal ||
			drawMode == dmShaded ||
			drawMode == dmWireframe )
			materialGet2(&material, m_color-256) ;
		else
		if( drawMode == dmPrompt )
			materialGet2(&material, 1) ;
		else
		if( drawMode == dmSelect )
			materialGet2(&material, 41) ;
		else
			return 0 ;
		materialSet(&material) ;
	}

	return 1 ;
}

// nt add 2011/7/6 : 用于画线类的图元
int CFeature::SetDrawColor2(int drawMode)
{
	int old = m_color ;
	m_color = 0 ;
	SetDrawColor(drawMode) ;
	m_color = old ;

	return 1 ;
}

int CFeature::GetBox(BOX3D* box)
{
	if( !box )
		return 0 ;

	*box = m_box ;

	return 1 ;
}

// nt add 2014/3/19
void CFeature::SetBox(BOX3D* box)
{
	if( box )
		m_box = *box ;

	return ;
}

// nt add 2010/10/12
void CFeature::Prompt(CStr& str)
{
	str = "" ;
}

// nt modify 2010/1/5
// nt modify 2010/9/26
int CFeature::Serialize(CArchive& ar)
{
	int sv, iLayer = -1 ; // m_state+versionNum

	if( ar.IsStoring() )
	{
		sv = 2048 ;
		ar << sv
		   << m_color ;
		ar.Write(&m_box, sizeof(BOX3D)) ;
		// nt add 2010/9/9
		ar << m_name
		   << m_state 
		   << m_featId
		   << m_createTime
		   << m_lastModifyTime
		   << m_it
		   << m_waitUpdateFlag
		   << m_flags
		   << iLayer
		   << m_no ;
	}
	else
	{
		ar >> sv ;
		if( sv >= 512 )
			ar >> m_color ;
		ar.Read(&m_box, sizeof(BOX3D)) ;
		// nt add 2010/9/9
		if( sv >= 1024 )
		{
			ar >> m_name
			   >> m_state
			   >> m_featId
			   >> m_createTime
			   >> m_lastModifyTime
			   >> m_it
			   >> m_waitUpdateFlag
			   >> m_flags
			   >> iLayer ;
		}
		if( sv >= 2048 )
			ar >> m_no ;
	}

	return 1 ;
}
///////////////////////////////////////////////////////////////
