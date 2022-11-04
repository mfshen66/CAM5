/*	2002/8/20 nt
 *	implementation file of class CTriangle
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "Triangle.h"

////////////////////////////////////////////////////////////////
//	refpt
/*	2010/11/14 nt
 *	CTriangle
 */
CTriangle::CTriangle()
{
	m_index = 0 ;
	m_v1[0] = 0. ;
	m_v1[1] = 0. ;
	m_v1[2] = 0. ;
	m_normal1[0] = 0. ;
	m_normal1[1] = 0. ;
	m_normal1[2] = 1. ;
	m_v2[0] = 0. ;
	m_v2[1] = 0. ;
	m_v2[2] = 0. ;
	m_normal2[0] = 0. ;
	m_normal2[1] = 0. ;
	m_normal2[2] = 1. ;
	m_v3[0] = 0. ;
	m_v3[1] = 0. ;
	m_v3[2] = 0. ;
	m_normal3[0] = 0. ;
	m_normal3[1] = 0. ;
	m_normal3[2] = 1. ;
	m_color = 4 ;
	CalBox() ;
}

CTriangle::CTriangle(PNT3D v1, PNT3D v2, PNT3D v3)
{
	memcpy(m_v1, v1, sizeof(PNT3D)) ;
	m_normal1[0] = 0. ;
	m_normal1[1] = 0. ;
	m_normal1[2] = 1. ;
	memcpy(m_v2, v2, sizeof(PNT3D)) ;
	m_normal2[0] = 0. ;
	m_normal2[1] = 0. ;
	m_normal2[2] = 1. ;
	memcpy(m_v3, v3, sizeof(PNT3D)) ;
	m_normal3[0] = 0. ;
	m_normal3[1] = 0. ;
	m_normal3[2] = 1. ;
	m_color = 4 ;
	CalBox() ;
}

CTriangle::CTriangle(PNT3D v1, 
					 VEC3D normal1, 
					 PNT3D v2, 
					 VEC3D normal2, 
					 PNT3D v3, 
					 VEC3D normal3)
{
	memcpy(m_v1, v1, sizeof(PNT3D)) ;
	memcpy(m_normal1, normal1, sizeof(VEC3D)) ;
	memcpy(m_v2, v2, sizeof(PNT3D)) ;
	memcpy(m_normal2, normal2, sizeof(VEC3D)) ;
	memcpy(m_v3, v3, sizeof(PNT3D)) ;
	memcpy(m_normal3, normal3, sizeof(VEC3D)) ;
	m_color = 4 ;
	CalBox() ;
}

CTriangle::~CTriangle()
{
}
//--------------------------------------------------------------
// nt add 2009/5/27
CFeature* CTriangle::Copy()
{
	CTriangle* pTriangle = new CTriangle(m_v1, m_normal1, 
										 m_v2, m_normal2, 
										 m_v3, m_normal3) ;

	return pTriangle ;
}

int	CTriangle::GetType()
{
	return ftTriangle ;
}

int CTriangle::GetIndex()
{
	return m_index ;
}

void CTriangle::SetIndex(int index)
{
	m_index = index ;
}

int	CTriangle::CalBox()
{
	for( int k = 0 ; k < 3 ; k++ )
	{
		m_box.min[k] = mathMIN(m_v1[k], mathMIN(m_v2[k], m_v3[k])) ;
		m_box.max[k] = mathMAX(m_v1[k], mathMAX(m_v2[k], m_v3[k])) ;
	}

	return 1 ;
}

// nt add 2009/5/27
int CTriangle::Move(VEC3D v)
{
	m_v1[0] += v[0] ;
	m_v1[0] += v[0] ;
	m_v1[0] += v[0] ;
	m_v2[0] += v[0] ;
	m_v2[0] += v[0] ;
	m_v2[0] += v[0] ;
	m_v3[0] += v[0] ;
	m_v3[0] += v[0] ;
	m_v3[0] += v[0] ;
	CalBox() ;

	return 1 ;
}

// nt add 2009/5/27
int CTriangle::Rotate(PNT3D pivot, VEC3D axis, double angle)
{
	mathRotPnt(axis, pivot, angle, m_v1, m_v1) ;
	mathRotVec(axis, pivot, angle, m_normal1, m_normal1) ;
	mathRotPnt(axis, pivot, angle, m_v2, m_v2) ;
	mathRotVec(axis, pivot, angle, m_normal2, m_normal2) ;
	mathRotPnt(axis, pivot, angle, m_v3, m_v3) ;
	mathRotVec(axis, pivot, angle, m_normal3, m_normal3) ;
	CalBox() ;

	return 1 ;
}

// nt add 2010/10/17
int CTriangle::Mirror(PNT3D pivot, VEC3D normal)
{
	PNT3D v ;
	mathMirPnt(pivot, normal, m_v1, v) ;
	mathMirPnt(pivot, normal, m_v2, m_v1) ;
	memcpy(m_v2, v, sizeof(PNT3D)) ;
	mathMirPnt(pivot, normal, m_v3, m_v3) ;

	VEC3D n ;
	mathMirVec(pivot, normal, m_normal1, n) ;
	mathMirVec(pivot, normal, m_normal2, m_normal1) ;
	memcpy(m_normal2, n, sizeof(VEC3D)) ;
	mathMirVec(pivot, normal, m_normal3, m_normal3) ;

	return 1 ;
}

// nt add 2010/10/17
int CTriangle::Scale(double scale)
{
	m_v1[0] *= scale ;
	m_v1[1] *= scale ;
	m_v1[2] *= scale ;
	m_v2[0] *= scale ;
	m_v2[1] *= scale ;
	m_v2[2] *= scale ;
	m_v3[0] *= scale ;
	m_v3[1] *= scale ;
	m_v3[2] *= scale ;
	CalBox() ;

	return 1 ;
}

// nt modify 2009/5/9 : 考虑到导航类型的优先级别
int CTriangle::Navigate(DNAVI* dn)
{
	return 0 ;
}
//--------------------------------------------------------------
int CTriangle::Pick(PINFO* pi)
{
	return 2 ;
}

// nt add 2010/4/22
// 注意已经用包围盒判断过了
int CTriangle::PickByBox(RFRAME* pLF, BOX2D* pLB, double r)
{
	return 2 ;
}
//--------------------------------------------------------------
// nt rewrite 2009/12/22
int CTriangle::Draw(void* pVI, int drawMode)
{
	SetDrawColor(drawMode) ;
	glBegin(GL_TRIANGLES) ;
		glNormal3dv(m_normal1) ;
		glVertex3dv(m_v1) ;
		glNormal3dv(m_normal2) ;
		glVertex3dv(m_v2) ;
		glNormal3dv(m_normal3) ;
		glVertex3dv(m_v3) ;
	glEnd() ;

	return 1 ;
}

int CTriangle::Draw2(void* pVI, int drawMode)
{
	return Draw(pVI, drawMode) ;
}
//--------------------------------------------------------------
int CTriangle::Serialize(CArchive& ar)
{
	CFeature::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		ar << m_index
		   << m_v1[0]
		   << m_v1[1]
		   << m_v1[2]
		   << m_v2[0]
		   << m_v2[1]
		   << m_v2[2]
		   << m_v3[0]
		   << m_v3[1]
		   << m_v3[2]
		   << m_normal1[0]
		   << m_normal1[1]
		   << m_normal1[2]
		   << m_normal2[0]
		   << m_normal2[1]
		   << m_normal2[2]
		   << m_normal3[0]
		   << m_normal3[1]
		   << m_normal3[2] ;
	}
	else
	{
		ar >> m_index
		   >> m_v1[0]
		   >> m_v1[1]
		   >> m_v1[2]
		   >> m_v2[0]
		   >> m_v2[1]
		   >> m_v2[2]
		   >> m_v3[0]
		   >> m_v3[1]
		   >> m_v3[2]
		   >> m_normal1[0]
		   >> m_normal1[1]
		   >> m_normal1[2]
		   >> m_normal2[0]
		   >> m_normal2[1]
		   >> m_normal2[2]
		   >> m_normal3[0]
		   >> m_normal3[1]
		   >> m_normal3[2] ;
		CalBox() ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
