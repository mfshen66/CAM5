/*	2011/4/20 nt
 *	implementation file of class CMark
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "Mark.h"

////////////////////////////////////////////////////////////////
//	Mark
/*	2011/4/20 nt
 *	Mark
 */
CMark::CMark()
{
	m_type = 0 ;
	m_color = 4 ;
	//m_lins = NULL ;
	CalBox() ;
}

CMark::CMark(int type)
{
	m_type = type ;
	m_color = 4 ;
	//m_lins = NULL ;
	CalBox() ;
}

CMark::~CMark()
{
}
//--------------------------------------------------------------
void CMark::AddLin(PNT3D p, PNT3D q)
{
	//LIN3D* pLin = mathCreLin3D(p, q) ;
	//pLin->next = m_lins ;
	//m_lins = pLin ;
	return ;
}

void CMark::AddLin2(PNT3D p, PNT3D q, double w)
{
	PNT3D Q ;

	Q[0] = p[0]+(q[0]-p[0])*w ;
	Q[1] = p[1]+(q[1]-p[1])*w ;
	Q[2] = p[2]+(q[2]-p[2])*w ;
	AddLin(p, Q) ;

	return ;
}

CFeature* CMark::Copy()
{
	CMark* pMark = new CMark(m_type) ;
	/*for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	{
		pMark->AddLin(pLin->p1, pLin->p2) ;
	}*/
	pMark->m_box = m_box ;

	return pMark ;
}

int	CMark::GetType()
{
	return ftMark ;
}

int	CMark::GetDetailType()
{
	return m_type ;
}

int	CMark::CalBox()
{
//	int k ;
	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;
	/*for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			if( m_box.min[k] > pLin->p1[k] )
				m_box.min[k] = pLin->p1[k] ;
			if( m_box.max[k] < pLin->p1[k] )
				m_box.max[k] = pLin->p1[k] ;
			if( m_box.min[k] > pLin->p2[k] )
				m_box.min[k] = pLin->p2[k] ;
			if( m_box.max[k] < pLin->p2[k] )
				m_box.max[k] = pLin->p2[k] ;
		}
	}*/
	return 1 ;
}

int CMark::Move(VEC3D v)
{
	/*for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	{
		pLin->p1[0] += v[0] ;
		pLin->p1[1] += v[1] ;
		pLin->p1[2] += v[2] ;
		pLin->p2[0] += v[0] ;
		pLin->p2[1] += v[1] ;
		pLin->p2[2] += v[2] ;
	}*/
	m_box.min[0] += v[0] ;
	m_box.min[1] += v[1] ;
	m_box.min[2] += v[2] ;
	m_box.max[0] += v[0] ;
	m_box.max[1] += v[1] ;
	m_box.max[2] += v[2] ;

	return 1 ;
}

int CMark::Rotate(PNT3D pivot, VEC3D axis, double angle)
{
	/*for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	{
		mathRotPnt(axis, pivot, angle, pLin->p1, pLin->p1) ;
		mathRotPnt(axis, pivot, angle, pLin->p2, pLin->p2) ;
	}*/
	CalBox() ;

	return 1 ;
}

int CMark::Mirror(PNT3D pivot, VEC3D normal)
{
	/*for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	{
		mathMirPnt(pivot, normal, pLin->p1, pLin->p1) ;
		mathMirPnt(pivot, normal, pLin->p2, pLin->p2) ;
	}*/
	CalBox() ;

	return 1 ;
}

int CMark::Scale(double scale)
{
	/*for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	{
		pLin->p1[0] *= scale ;
		pLin->p1[1] *= scale ;
		pLin->p1[2] *= scale ;
		pLin->p2[0] *= scale ;
		pLin->p2[1] *= scale ;
		pLin->p2[2] *= scale ;
	}*/
	CalBox() ;

	return 1 ;
}

int CMark::Scale2(double scale)
{
	//VEC3D v ;

	/*for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	{
		v[0] = pLin->p2[0]-pLin->p1[0] ;
		v[1] = pLin->p2[1]-pLin->p1[1] ;
		v[2] = pLin->p2[2]-pLin->p1[2] ;

		pLin->p2[0] = pLin->p1[0]+v[0]*scale ;
		pLin->p2[1] = pLin->p1[1]+v[1]*scale ;
		pLin->p2[2] = pLin->p1[2]+v[2]*scale ;
	}*/
	CalBox() ;

	return 1 ;
}

int CMark::Navigate(DNAVI* dn)
{
	if( dn )
		return 2 ;
	else
		return 0 ; // error
}
//--------------------------------------------------------------
// nt add 2020/2/20
void CMark::Prompt(CStr& str)
{
	str = "Mark" ;
	if( m_name != "" )
	{
		str += ":" ;
		str += m_name ;
	}

	return ;
}

int CMark::Pick(PINFO* pi)
{
	//PNT3D p1, p2 ;

	if( mathChkBoxLinInt(&m_box, pi->pos, pi->dir, pi->r) == IDNOINT )
		return 2 ;

	//for( LIN3D* pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
	//{
	//	mathPrjPntPln(pLin->p1, pi->pos, pi->dir, p1) ;
	//	mathPrjPntPln(pLin->p2, pi->pos, pi->dir, p2) ;
	//	if( mathChkPntSegm(pi->pos, p1, p2, pi->r) == IDIN )
	//	{
	//		pi->ot = ftMark ;
	//		pi->obj = this ;
	//		pi->z = 0. ; //???
	//		pi->u = 0. ;
	//		pi->v = 0. ;
	//		pi->att = 1 ; // nt add 2011/7/6
	//		return 1 ;
	//	}
	//}

	return 2 ;
}

// nt add 2010/4/22
// 注意已经用包围盒判断过了
int CMark::PickByBox(RFRAME* pLF, BOX2D* pLB, double r)
{
	return 2 ;
}
//--------------------------------------------------------------
int CMark::Draw(void* pVI, int drawMode)
{
	//LIN3D* pLin ;

	SetDrawColor(drawMode) ;

	glLineWidth(1.f) ;
	glBegin(GL_LINES) ;
		/*for( pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
		{
			glVertex3dv(pLin->p1) ;
			glVertex3dv(pLin->p2) ;
		}*/
	glEnd() ;

	// nt add 2014/3/22
	glPointSize(2.f) ;
	glBegin(GL_POINTS) ;
		/*for( pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
		{
			glVertex3dv(pLin->p1) ;
		}*/
	glEnd() ;

	return 1 ;
}

int CMark::Draw2(void* pVI, int drawMode)
{
	return Draw(pVI, drawMode) ;
}
//--------------------------------------------------------------
int CMark::Serialize(CArchive& ar)
{
	int n = 0, i ;
	//LIN3D* pLin = NULL ;
	PNT3D p1, p2 ;

	CFeature::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		ar << m_type ;
		/*for( pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
		{
			n++ ;
		}
		ar << n ;
		for( pLin = m_lins ; pLin != NULL ; pLin = pLin->next )
		{
			ar << pLin->p1[0]
			   << pLin->p1[1]
			   << pLin->p1[2]
			   << pLin->p2[0]
			   << pLin->p2[1]
			   << pLin->p2[2] ;
		}*/
	}
	else
	{
		ar >> m_type ;
		ar >> n ;
		for( i = 0 ; i < n ; i++ )
		{
			ar >> p1[0]
			   >> p1[1]
			   >> p1[2]
			   >> p2[0]
			   >> p2[1]
			   >> p2[2] ;
			AddLin(p1, p2) ;
		}
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
