/*	2003/11/6 nt
 *	implementation file of class CMPoint
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "MPoint.h"

////////////////////////////////////////////////////////////////
//	measured point
/*	2003/11/6 nt
 *	measured point
 */
CMPoint::CMPoint()
{
	m_next = NULL ;
	m_link = NULL ;
	m_k1 = 0.0 ;
	m_k2 = 0.0 ;
	m_attrib = 0 ;
}

CMPoint::~CMPoint()
{
}
//--------------------------------------------------------------
// nt add 2010/4/22
int CMPoint::GetType()
{
	return MPOINT ;
}

int CMPoint::Draw(void* pVI, int drawMode)
{
	switch(drawMode)
	{
	case dmNormal:
		glColor3ub(0, 0, 0) ;
		break ;
	case dmPrompt:
		glColor3ub(255, 0, 0) ;
		break ;
	case dmSelect:
		glColor3ub(0, 255, 0) ;
		break ;
	case dmActive:
		glColor3ub(255, 0, 255) ;
		break ;
	case dmEraze:
		glColor3ub(255, 255, 255) ;
		break ;
	default:
		return 0 ;
	}

	glPointSize(2.f) ;
	glBegin(GL_POINTS) ;
		if( m_attrib != -1 )
			glVertex3dv(m_p) ;
	glEnd() ;

	return 1 ;
}

// nt add 2010/4/23
int CMPoint::Draw2(void* pVI, int drawMode)
{
	return Draw(pVI, drawMode) ;
}
////////////////////////////////////////////////////////////////
