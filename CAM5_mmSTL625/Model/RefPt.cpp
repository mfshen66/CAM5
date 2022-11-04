/*	2002/8/20 nt
 *	implementation file of class CRefPt
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "RefPt.h"

////////////////////////////////////////////////////////////////
//	refpt
/*	2002/8/20 nt
 *	refpt
 */
// nt modify 2010/10/22
CRefPt::CRefPt()
{
	m_type = 0 ;
	m_p[0] = 0.0 ;
	m_p[1] = 0.0 ;
	m_p[2] = 0.0 ;
	// nt add 2009/12/22
	m_color = 4 ;
	CalBox() ; // nt add 2010/10/22
}

// nt modify 2010/10/22
CRefPt::CRefPt(int type, PNT3D p)
{
	m_type = type ;
	m_color = 4 ;
	if( p )
		memcpy(m_p, p, sizeof(PNT3D)) ;
	else
	{
		m_p[0] = 0.0 ;
		m_p[1] = 0.0 ;
		m_p[2] = 0.0 ;
	}
	CalBox() ; // nt add 2010/10/22
}

// jh add 2022/8/18
CRefPt::CRefPt(int type, int color, PNT3D p)
{
	m_type = type;
	m_color = color;
	if (p)
		memcpy(m_p, p, sizeof(PNT3D));
	else
	{
		m_p[0] = 0.0;
		m_p[1] = 0.0;
		m_p[2] = 0.0;
	}
	CalBox(); // nt add 2010/10/22
}

CRefPt::~CRefPt()
{
}
//--------------------------------------------------------------
// nt add 2009/5/27
CFeature* CRefPt::Copy()
{
	CRefPt* pRefPt = new CRefPt(m_type, m_p) ;
	pRefPt->m_box = m_box ;

	return pRefPt ;
}

int	CRefPt::GetType()
{
	return ftRefPt ;
}

int	CRefPt::GetDetailType()
{
	return m_type ;
}

void CRefPt::GetP(PNT3D p)
{
	if( p )
		memcpy(p, m_p, sizeof(PNT3D)) ;
}

int	CRefPt::CalBox()
{
	memcpy(m_box.min, m_p, sizeof(PNT3D)) ;
	memcpy(m_box.max, m_p, sizeof(PNT3D)) ;

	return 1 ;
}

// nt add 2009/5/27
int CRefPt::Move(VEC3D v)
{
	m_p[0] += v[0] ;
	m_p[0] += v[0] ;
	m_p[0] += v[0] ;
	CalBox() ;

	return 1 ;
}

// nt add 2009/5/27
int CRefPt::Rotate(PNT3D pivot, VEC3D axis, double angle)
{
	mathRotPnt(axis, pivot, angle, m_p, m_p) ;
	CalBox() ;

	return 1 ;
}

// nt add 2010/10/17
int CRefPt::Mirror(PNT3D pivot, VEC3D normal)
{
	mathMirPnt(pivot, normal, m_p, m_p) ;
	CalBox() ; // nt add 2011/4/20

	return 1 ;
}

// nt add 2010/9/11
int CRefPt::Scale(double scale)
{
	m_p[0] *= scale ;
	m_p[1] *= scale ;
	m_p[2] *= scale ;
	CalBox() ;

	return 1 ;
}

// nt modify 2009/5/9 : 考虑到导航类型的优先级别
int CRefPt::Navigate(DNAVI* dn)
{
	if( dn )
	{
		if( dn->type < dnRefPt )
		{
			double d = mathDistPntLin(m_p, dn->pos, dn->dir) ;
			if( d < dn->r )
			{
				dn->type = dnRefPt ; 
				dn->featType = ftRefPt ;
				dn->feat = this ;
				memcpy(dn->nav_p, m_p, sizeof(PNT3D)) ;
				return 1 ;
			}
		}
		return 2 ;
	}
	else
		return 0 ; // error
}
//--------------------------------------------------------------
int CRefPt::Pick(PINFO* pi)
{
	PNT3D prj_p ;

	mathPrjPntLin(m_p, pi->pos, pi->dir, prj_p) ;
	if( mathDist(prj_p, m_p) < pi->r )
	{
		pi->ot = ftRefPt ; // nt add 2003/7/22
		pi->obj = this ;
		pi->z = (m_p[0]-pi->pos[0])*pi->dir[0]+
			    (m_p[1]-pi->pos[1])*pi->dir[1]+
				(m_p[2]-pi->pos[2])*pi->dir[2] ;
		pi->u = 0.0 ;
		pi->v = 0.0 ;
		pi->att = 2 ; // nt add 2011/7/6
		 return 1 ; // nt add 2003/7/22
	}

	return 2 ; // nt modify 2003/7/22 : old return 1
}

// nt add 2010/4/22
// 注意已经用包围盒判断过了
int CRefPt::PickByBox(RFRAME* pLF, BOX2D* pLB, double r)
{
	return 2 ;
}
//--------------------------------------------------------------
// nt rewrite 2009/12/22
// nt modify 2010/10
// nt modify 2011/7/3
int CRefPt::Draw(void* pVI, int drawMode)
{
	SetDrawColor(drawMode) ;
	glPointSize(5.f) ;
	glBegin(GL_POINTS) ;
		glVertex3dv(m_p) ;
	glEnd() ;
	// nt add 2011/7/3
	/*RFRAME lf ;
	viGetCenterLF(pVI, lf.X, lf.Y, lf.Z, lf.O) ;
	double d = 4900. ;
	lf.O[0] += d*lf.Z[0] ;
	lf.O[1] += d*lf.Z[1] ;
	lf.O[2] += d*lf.Z[2] ;
	PNT3D p ;
	mathPrjPntPln(m_p, lf.O, lf.Z, p) ;
	// end
	viDrawCross2(pVI, p, 5) ;*/

	return 1 ;
}

int CRefPt::Draw2(void* pVI, int drawMode)
{
	return Draw(pVI, drawMode) ;
}
//--------------------------------------------------------------
int CRefPt::Serialize(CArchive& ar)
{
	CFeature::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		ar << m_type
		   << m_p[0]
		   << m_p[1]
		   << m_p[2] ;
	}
	else
	{
		ar >> m_type
		   >> m_p[0]
		   >> m_p[1]
		   >> m_p[2] ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
