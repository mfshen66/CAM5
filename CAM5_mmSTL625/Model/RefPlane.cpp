/*	2001/11/17 nt
 *	implementation file of class CRefPlane
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "DNAVI.h"
#include "Feature.h"
#include "RefPlane.h"

////////////////////////////////////////////////////////////////
//	refplane
/*	2001/11/17 nt
 *	refplane
 */
CRefPlane::CRefPlane()
{
	m_dtype = 0 ;
	m_halfSize = 25. ;
	mathInitRFrame(&m_lf) ;
	// nt add 2009/12/22
	m_color = 260 ; // white=256+4
	CalBox() ; // nt add 2010/10/22
}

CRefPlane::CRefPlane(int dtype, RFRAME* pLF)
{
	m_dtype = dtype ;
	m_halfSize = 25. ;
	if( pLF )
		m_lf = *pLF ;
	else
		mathInitRFrame(&m_lf) ;
	// nt add 2009/12/22
	m_color = 260 ; // white=256+4
	CalBox() ; // nt add 2010/10/22
}

CRefPlane::~CRefPlane()
{
}
//--------------------------------------------------------------
// nt add 2009/5/27
CFeature* CRefPlane::Copy()
{
	CRefPlane* pRefPlane = new CRefPlane(m_dtype, &m_lf) ;
	pRefPlane->m_halfSize = m_halfSize ;
	pRefPlane->m_box = m_box ;

	return pRefPlane ;
}

int CRefPlane::GetType()
{
	return ftRefPlane ;
}

int CRefPlane::GetDetailType()
{
	return m_dtype ;
}

double CRefPlane::GetSize()
{
	return 2.0*m_halfSize ;
}

void CRefPlane::SetSize(double size)
{
	m_halfSize = 0.5*size ;
}

int CRefPlane::Move(VEC3D v)
{
	m_lf.O[0] += v[0] ;
	m_lf.O[1] += v[1] ;
	m_lf.O[2] += v[2] ;
	CalBox() ;

	return 1 ;
}

// nt add 2009/5/27
int CRefPlane::Rotate(PNT3D pivot, VEC3D axis, double angle)
{
	mathRotateRFrame(pivot, axis, angle, &m_lf) ;
	CalBox() ;

	return 1 ;
}

// nt add 2010/10/17
int CRefPlane::Mirror(PNT3D pivot, VEC3D normal)
{
	mathMirPnt(pivot, normal, m_lf.O, m_lf.O) ;
	mathMirVec(pivot, normal, m_lf.X, m_lf.X) ;
	mathMirVec(pivot, normal, m_lf.Y, m_lf.Y) ;
	mathVProduct(m_lf.X, m_lf.Y, m_lf.Z) ;
	CalBox() ;

	return 1 ;
}

// nt add 2010/9/11
int CRefPlane::Scale(double scale)
{
	m_lf.O[0] *= scale ;
	m_lf.O[1] *= scale ;
	m_lf.O[2] *= scale ;
	m_halfSize = 25. ;
	CalBox() ;

	return 1 ;
}

void CRefPlane::GetLF(RFRAME* pLF)
{
	*pLF = m_lf ;
}

int CRefPlane::Project(PNT3D oldPt, VEC3D prjDir, PNT2D prjPt)
{
	double t, d ;
	PNT3D p ;

	d = mathOProduct(prjDir, m_lf.Z) ;
	if( fabs(d) < MIN_DBL )
		return 0 ;
	t = (mathOProduct(m_lf.O, m_lf.Z)-mathOProduct(oldPt, m_lf.Z))/d ;
	p[0] = oldPt[0]+t*prjDir[0] ;
	p[1] = oldPt[1]+t*prjDir[1] ;
	p[2] = oldPt[2]+t*prjDir[2] ;
	ToPlane(p, prjPt) ;

	return 1 ;
}

int CRefPlane::ToPlane(PNT3D worldPt, PNT2D planePt)
{
	VEC3D v ;

	v[0] = worldPt[0]-m_lf.O[0] ;
	v[1] = worldPt[1]-m_lf.O[1] ;
	v[2] = worldPt[2]-m_lf.O[2] ;
	planePt[0] = mathOProduct(m_lf.X, v) ;
	planePt[1] = mathOProduct(m_lf.Y, v) ;

	return 1 ;
}

int CRefPlane::ToWorld(PNT2D planePt, PNT3D worldPt)
{
	worldPt[0] = m_lf.O[0]+m_lf.X[0]*planePt[0]+m_lf.Y[0]*planePt[1] ;
	worldPt[1] = m_lf.O[1]+m_lf.X[1]*planePt[0]+m_lf.Y[1]*planePt[1] ;
	worldPt[2] = m_lf.O[2]+m_lf.X[2]*planePt[0]+m_lf.Y[2]*planePt[1] ;

	return 1 ;
}

// nt add gridSize, 2009/5/9
int CRefPlane::Navigate(DNAVI* dn, double gridSize)
{
	int k ;
	double d ;
	PNT2D rawPt, navPt ;

	if( dn &&
		gridSize > MIN_LEN )
	{
		if( dn->type < dnGrid )
		{
			if( Project(dn->pos, dn->dir, rawPt) != 1 ) // project to plane
				return 1 ;

			d = rawPt[0]/gridSize ; // x
			if( d > 0.0 )
				d += 0.5 ;
			else
				d -= 0.5 ;
			k = (int)d ;
			navPt[0] = gridSize*k ;

			d = rawPt[1]/gridSize ; // y
			if( d > 0.0 )
				d += 0.5 ;
			else
				d -= 0.5 ;
			k = (int)d ;
			navPt[1] = gridSize*k ;

			if( mathDist2D(rawPt, navPt) < dn->r ) // if in navigation scope
			{
				dn->type = dnGrid ;
				dn->featType = ftRefPlane ;
				dn->feat = this ;
				ToWorld(navPt, dn->nav_p) ;
				return 1 ;
			}
		}

		return 2 ;
	}
	else
		return 0 ; // error
}

int CRefPlane::CalBox()
{
	BOX2D box2d ;

	box2d.min[0] = box2d.min[1] = -m_halfSize ;
	box2d.max[0] = box2d.max[1] = m_halfSize ;
	mathTransLocalBox2D(&m_lf, &box2d, &m_box) ;

	return 1 ;
}
//--------------------------------------------------------------
int CRefPlane::DrawGrid(void* pVI, int color, double step)
{
	if( !pVI )
		return 0 ;

	viDrawWorkPlaneGrid(pVI,
						m_lf.O,
						m_lf.X,
						m_lf.Y, 
						step, 
						1, // p or line
						color,
						1.0) ;

	return 1 ;
}

// nt add 2011/7/6
int CRefPlane::DrawBorder(void* pVI, int drawMode)
{
	PNT2D planePt ;
	PNT3D p ;

	// nt add 2011/7/6
	RFRAME lf ;
	viGetCenterLF(pVI, lf.X, lf.Y, lf.Z, lf.O) ;
	double d = 4900. ;
	lf.O[0] += d*lf.Z[0] ;
	lf.O[1] += d*lf.Z[1] ;
	lf.O[2] += d*lf.Z[2] ;

	// 绘制边框
	SetDrawColor2(drawMode) ;
	glLineWidth(1.f) ; // nt add 2011/7/6
	glBegin(GL_LINE_LOOP) ;
		planePt[0] = -m_halfSize ;
		planePt[1] = -m_halfSize ;
		ToWorld(planePt, p) ;
		mathPrjPntPln(p, lf.O, lf.Z, p) ; // nt add 2011/7/6
		glVertex3dv(p) ;
		planePt[0] = m_halfSize ;
		planePt[1] = -m_halfSize ;
		ToWorld(planePt, p) ;
		mathPrjPntPln(p, lf.O, lf.Z, p) ; // nt add 2011/7/6
		glVertex3dv(p) ;
		planePt[0] = m_halfSize ;
		planePt[1] = m_halfSize ;
		ToWorld(planePt, p) ;
		mathPrjPntPln(p, lf.O, lf.Z, p) ; // nt add 2011/7/6
		glVertex3dv(p) ;
		planePt[0] = -m_halfSize ;
		planePt[1] = m_halfSize ;
		ToWorld(planePt, p) ;
		mathPrjPntPln(p, lf.O, lf.Z, p) ; // nt add 2011/7/6
		glVertex3dv(p) ;
	glEnd() ;

	// nt add 2010/10/15
	mathPrjPntPln(m_lf.O, lf.O, lf.Z, p) ; // nt add 2011/7/6
	viDrawLocalFrame2(pVI, p, m_lf.X, m_lf.Y) ;

	return 1 ;
}

// nt add 2011/7/6
int CRefPlane::DrawPlane(void* pVI, int drawMode)
{
	PNT2D planePt ;
	PNT3D p ;

	// nt add 2008/11/14
	SetDrawColor(drawMode) ;
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, 
				GL_ONE_MINUS_SRC_ALPHA) ;
	glBegin(GL_QUADS) ;
		glNormal3dv(m_lf.Z) ; // nt add 2009/12/18
		planePt[0] = -m_halfSize ;
		planePt[1] = -m_halfSize ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glVertex3dv(p) ;
		glNormal3dv(m_lf.Z) ; // nt add 2009/12/18
		planePt[0] = m_halfSize ;
		planePt[1] = -m_halfSize ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glVertex3dv(p) ;
		glNormal3dv(m_lf.Z) ; // nt add 2009/12/18
		planePt[0] = m_halfSize ;
		planePt[1] = m_halfSize ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glVertex3dv(p) ;
		glNormal3dv(m_lf.Z) ; // nt add 2009/12/18
		planePt[0] = -m_halfSize ;
		planePt[1] = m_halfSize ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glVertex3dv(p) ;
	glEnd() ;
	glDisable(GL_BLEND) ;

	return 1 ;
}

// nt modify 2009/12/22
// nt modify 2010/1/19
// nt rewrite 2011/7/6
int CRefPlane::Draw(void* pVI, int drawMode)
{
	DrawPlane(pVI, drawMode) ;
	DrawBorder(pVI, drawMode) ;	

	return 1 ;
}

// nt add 2010/1/19
int CRefPlane::Draw2(void* pVI, int drawMode)
{
	DrawBorder(pVI, drawMode) ;

	return 1 ;
}

// nt add 2010/11/27
int CRefPlane::DrawXYMark(void* pVI)
{
	double h = viGetScale(pVI)*25 ;
	glLineWidth(1.f) ;
	CS cs ;
	cs.scale = 1. ;
	memcpy(cs.X, m_lf.X, sizeof(VEC3D)) ;
	memcpy(cs.Y, m_lf.Y, sizeof(VEC3D)) ;
	memcpy(cs.Z, m_lf.Z, sizeof(VEC3D)) ;
	mathEvalLin(m_lf.O, m_lf.X, h, cs.O) ;
	glColor3ub(255, 0, 0) ;
	viTextOut3D(pVI, &cs, "X", h) ;
	mathEvalLin(m_lf.O, m_lf.Y, 2*h, cs.O) ;
	viTextOut3D(pVI, &cs, "Y", h) ;

	return 1 ;
}
//--------------------------------------------------------------
int CRefPlane::Pick(PINFO* pi)
{
	int i ;
	double z, A, B, C, D, t ;
	PNT2D planePt ;
	PNT3D worldPt, vs[4], p1, p2 ;
	VEC3D v, e1, e2 ;

	if( !pi )
		return 0 ;

	// 先判是否拾取到了边，求出4个角点, nt add 2011/7/6
	planePt[0] = -m_halfSize ;
	planePt[1] = -m_halfSize ;
	ToWorld(planePt, vs[0]) ;
	planePt[0] = m_halfSize ;
	planePt[1] = -m_halfSize ;
	ToWorld(planePt, vs[1]) ;
	planePt[0] = m_halfSize ;
	planePt[1] = m_halfSize ;
	ToWorld(planePt, vs[2]) ;
	planePt[0] = -m_halfSize ;
	planePt[1] = m_halfSize ;
	ToWorld(planePt, vs[3]) ;
	mathGetXYFromZ(pi->dir, e1, e2) ;
	mathTransWorldPnt3DByOXYZ(pi->pos, e1, e2, pi->dir, vs[0], p1) ;
	for( i = 1 ; i <= 4 ; i++ )
	{
		mathTransWorldPnt3DByOXYZ(pi->pos, e1, e2, pi->dir, vs[i<4?i:0], p2) ;
		A = (p2[0]-p1[0])*(p2[0]-p1[0])+(p2[1]-p1[1])*(p2[1]-p1[1]) ;
		B = p1[0]*(p2[0]-p1[0])+p1[1]*(p2[1]-p1[1]) ;
		C = p1[0]*p1[0]+p1[1]*p1[1] ;
		D = p2[0]*p2[0]+p2[1]*p2[1] ;
		t = -1. ;
		if( sqrt(C) < pi->r )
			t = 0. ;
		else
		if( sqrt(D) < pi->r )
			t = 1. ;
		else
		if( A > 1.e-20 && sqrt(fabs(C-B*B/A)) < pi->r )
			t = -B/A ;
		if( t > -MIN_DBL && t < 1.+MIN_DBL )
		{
			z = p1[2]*(1.-t)+p2[2]*t ;
			if( pi->obj == NULL ||
				pi->att == 0 ||
				pi->z > z )
			{
				pi->ot = ftRefPlane ;
				pi->obj = this ;
				Project(pi->pos, pi->dir, planePt) ;
				pi->u = planePt[0] ;
				pi->v = planePt[1] ;
				pi->z = z ;
				pi->att = 1 ;
				return 2 ;
			}
		}
		p1[0] = p2[0] ;
		p1[1] = p2[1] ;
		p1[2] = p2[2] ; // nt add 2011/7/6
	}

	Project(pi->pos, pi->dir, planePt) ;
	if( planePt[0] > -m_halfSize && // 是否严格在内部
		planePt[0] < m_halfSize &&
		planePt[1] > -m_halfSize &&
		planePt[1] < m_halfSize )
	{
		ToWorld(planePt, worldPt) ;
		v[0] = worldPt[0]-pi->pos[0] ;
		v[1] = worldPt[1]-pi->pos[1] ;
		v[2] = worldPt[2]-pi->pos[2] ;
		z = mathOProduct(v, pi->dir) ;
		if( pi->obj == NULL ||
			pi->z > z )
		{
			pi->ot = ftRefPlane ; // nt add 2003/7/22
			pi->obj = this ;
			pi->u = planePt[0] ;
			pi->v = planePt[1] ;
			pi->z = z ;
			pi->att = 0 ; // nt add 2011/7/6
			return 2 ; // nt add 2003/7/22
		}
	}

	return 2 ; // nt modify 2003/7/22
}

// nt add 2010/4/22
int CRefPlane::PickByBox(RFRAME* pLF, BOX2D* pLB, double r)
{
	int i, j ;
	double dx, dy ;
	PNT3D vs[2][2] ;

	for( i = 0 ; i < 2 ; i++ )
	{
		dx = 2*(i-0.5)*m_halfSize ;
		for( j = 0 ; j < 2 ; j++ )
		{
			dy = 2*(j-0.5)*m_halfSize ;
			vs[i][j][0] = m_lf.O[0]+dx*m_lf.X[0]+dy*m_lf.Y[0] ;
			vs[i][j][1] = m_lf.O[1]+dx*m_lf.X[1]+dy*m_lf.Y[1] ;
			vs[i][j][2] = m_lf.O[2]+dx*m_lf.X[2]+dy*m_lf.Y[2] ;
			mathPrjPntPln(vs[i][j], pLF->O, pLF->Z, vs[i][j]) ;
			if( pLB->min[0]-r < vs[i][j][0] &&
				vs[i][j][0] < pLB->max[0]+r &&
				pLB->min[1]-r < vs[i][j][1] &&
				vs[i][j][1] < pLB->max[1]+r )
				return 1 ;
		}
	}

	for( i = 0 ; i < 2 ; i++ )
	{
		if( mathChkBox2DSegmInt(pLB, // intersect with border
								vs[i][0], 
								vs[i][1],
								r) == IDINT ||
			mathChkBox2DSegmInt(pLB, // intersect with border
								vs[0][i], 
								vs[0][i],
								r) == IDINT )
			return 1 ;
	}

	return 2 ;
}
//--------------------------------------------------------------
int CRefPlane::Serialize(CArchive& ar)
{
	CFeature::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		ar << m_dtype
		   << m_halfSize ;
		ar.Write(&m_lf, sizeof(RFRAME)) ;
	}
	else
	{
		ar >> m_dtype
		   >> m_halfSize ;
		ar.Read(&m_lf, sizeof(RFRAME)) ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
