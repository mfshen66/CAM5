/*	2001/11/17 nt
 *	implementation file of class CRefCur
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "RefCur.h"

////////////////////////////////////////////////////////////////
//	refcur
/*	2001/11/17 nt
 *	refcur
 */
CRefCur::CRefCur()
{
	m_dtype = 0 ;
	m_curve = NULL ;
	m_interval.t1 = 0.0 ;
	m_interval.t2 = 1.0 ;
	m_strpt = NULL ;
	mathClnBox(&m_box) ;
	m_atts = 0 ;
	// nt add 2009/12/22
	// nt modify 2010/1/19
	m_color = 255 ;
	// nt add 2010/4/22
	m_u = 0. ;
	// nt add 2010/11/27
	m_width = 0 ;
	m_ltName = "" ;
}

CRefCur::CRefCur(int dtype, BSCUR* nurbs)
{
	m_dtype = dtype ;
	m_curve = bcurveCreate(nurbs) ;
	m_interval.t1 = nurbs->u_k[nurbs->ord_u-1] ; // nt modify 2013/6/26
	m_interval.t2 = nurbs->u_k[nurbs->cnt_u] ; // nt modify 2013/6/26
	m_strpt = NULL ;
	if( m_curve )
	{
		Disc(1., 0.15) ;
		CalBox() ;
	}
	else
		mathClnBox(&m_box) ;
	// nt add 2009/12/22
	// nt modify 2010/1/19
	m_color = 255 ;
	m_atts = 0 ; // nt modify 2010/5/23
	// nt add 2010/4/22
	m_u = 0. ;
	// nt add 2010/11/27
	m_width = 0 ;
	m_ltName = "" ;
}

CRefCur::CRefCur(int dtype, CURVE* curve, INTERVAL* interval)
{
	m_dtype = dtype ;
	m_curve = curve ;
	if( interval )
		m_interval = *interval ;
	else
		curveGetInterval(curve, &m_interval) ;
	m_strpt = NULL ;
	if( curve )
	{
		Disc(1., 0.15) ;
		CalBox() ;
	}
	else
		mathClnBox(&m_box) ;
	// nt add 2009/12/22
	// nt modify 2010/1/19
	m_color = 0 ;
	m_atts = 0 ; // nt modify 2010/5/23, old use m_cur
	// nt add 2010/4/22
	m_u = 0. ;
	// nt add 2010/11/27
	m_width = 0 ;
	m_ltName = "" ;
}

CRefCur::CRefCur(PNT3D p, PNT3D q)
{
	m_dtype = 0 ;
	m_curve = NULL ;
	m_strpt = NULL ;
	VEC3D dir ;
	mathGetVec(p, q, dir) ;
	double d = mathDist(p, q) ; // length
	if( d > MIN_DBL )
	{
		dir[0] /= d ;
		dir[1] /= d ;
		dir[2] /= d ;
		m_curve = lineCreate(p, dir) ; // line
		m_interval.t1 = 0. ;
		m_interval.t2 = d ;
		Disc(1., 0.15) ;
		CalBox() ;
	}
	m_color = 0 ;
	m_atts = 0 ;
	m_u = 0. ;
	m_width = 0 ;
	m_ltName = "" ;

	return ;
}

CRefCur::~CRefCur()
{
	if( m_curve )
		entityFree(m_curve) ;
	if( m_strpt )
		strptFree(m_strpt) ;
}
//--------------------------------------------------------------
// nt add 2009/5/27
CFeature* CRefCur::Copy()
{
	CURVE* curve = geometryCopy(m_curve) ;
	CRefCur* pRefCur = new CRefCur(m_dtype, curve, &m_interval) ;
	pRefCur->SetWidth(m_width) ; // nt add 2010/11/27
	pRefCur->SetLTName(m_ltName) ; // nt add 2010/11/27

	return pRefCur ;
}

int	CRefCur::GetType()
{
	return ftRefCur ;
}

int	CRefCur::GetDetailType()
{
	return m_dtype ;
}

// nt add 2010/11/27
int CRefCur::GetWidth()
{
	return m_width ;
}

// nt add 2010/11/27
void CRefCur::SetWidth(int width)
{
	m_width = width ;
}

// nt add 2010/11/27
void CRefCur::GetLTName(CString& ltName)
{
	ltName = m_ltName ;
}

// nt add 2010/11/27
void CRefCur::SetLTName(CString& ltName)
{
	m_ltName = ltName ;
}

void CRefCur::GetInterval(INTERVAL* interval)
{
	if( interval )
		*interval = m_interval ;
}

int	CRefCur::CalBox()
{
	if( !m_curve )
		return 0 ;

	curveCalBox(m_curve, &m_interval, &m_box) ;

	return 1 ;
}

// nt add 2010/4/22
double CRefCur::GetU()
{
	return m_u ;
}

// nt add 2010/4/22
int CRefCur::SetU(double u)
{
	m_u = u ;
	return 1 ;
}

CURVE* CRefCur::GetCurve()
{
	return m_curve ;
}

BSCUR* CRefCur::GetNurbs()
{
	int exact ;
	BSCUR* nurbs ;

	// nt modify 2006/8/19, del tol
	curveToNurbs(m_curve, &m_interval, &nurbs, &exact) ; // nt modify 2003/6/2

	return nurbs ;
}

int CRefCur::GetBegin(PNT3D p)
{
	if( !m_curve )
		return 0 ;

	curveEvalPos(m_curve, m_interval.t1, p) ;

	return 1 ;
}

int CRefCur::GetEnd(PNT3D p)
{
	if( !m_curve )
		return 0 ;

	curveEvalPos(m_curve, m_interval.t2, p) ;

	return 1 ;
}

// nt add 2006/8/19
int	CRefCur::ModInterval(INTERVAL* newInterval)
{
	m_interval = *newInterval ;
	Disc(1., 0.15) ;

	return 1 ;
}

// nt add 2003/10/30
STRPT* CRefCur::GetDisc()
{
	return m_strpt ;
}

void CRefCur::SetCur(int visible)
{
	if( visible == 0 )
	{
		if( m_atts&1 )
			m_atts -= 1 ;
	}
	else
	{
		if( !(m_atts&1) )
			m_atts += 1 ;
	}
}

// nt add 2010/8/28
int CRefCur::GetAtts()
{
	return m_atts ;
}

// nt add 2010/5/22
void CRefCur::SwapDrawU()
{
	if( m_atts&2 )
		m_atts -= 2 ;
	else
		m_atts += 2 ;
}

// nt add 2010/5/23
// 只处理NURBS曲线，要求其参数域为[0,1]
int CRefCur::RevsDir()
{
	int type = entityGetType(m_curve) ;
	if( type == etBCurve )
	{
		BSCUR* nurbs = curveGetNurbs(m_curve) ;
		if( nurbs )
		{
			bscurRevsDir(nurbs) ;
			if( m_strpt )
			{
				for( int i = 0 ; i < m_strpt->np ; i++ )
				{
					m_strpt->ts[i] = 1.0-m_strpt->ts[i] ;
				}
			}
			double min, max ;
			min = m_interval.t1 ;
			max = m_interval.t2 ;
			m_interval.t1 = 1-max ;
			m_interval.t2 = 1-min ;
		}
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
// nt add 2009/5/27
int CRefCur::Move(VEC3D v)
{
	if( m_curve )
		geometryMove(m_curve, v) ;
	if( m_strpt )
	{
		if( m_strpt->ps )
		{
			for( int i = 0 ; i < m_strpt->np ; i++ )
			{
				m_strpt->ps[i][0] += v[0] ;
				m_strpt->ps[i][1] += v[1] ;
				m_strpt->ps[i][2] += v[2] ;
			}
		}
	}
	CalBox() ;

	return 1 ;
}

// nt add 2009/5/27
int CRefCur::Rotate(PNT3D pivot, VEC3D axis, double angle)
{
	if( m_curve )
		geometryRotate(m_curve, pivot, axis, angle) ;
	if( m_strpt )
	{
		if( m_strpt->ps )
		{
			for( int i = 0 ; i < m_strpt->np ; i++ )
			{
				mathRotPnt(axis, pivot, angle, m_strpt->ps[i], m_strpt->ps[i]) ;
			}
		}
		if( m_strpt->vs )
		{
			for( int i = 0 ; i < m_strpt->np ; i++ )
			{
				mathRotVec(axis, pivot, angle, m_strpt->vs[i], m_strpt->vs[i]) ;
			}
		}
	}
	CalBox() ;

	return 1 ;
}

// nt add 2010/10/18
int CRefCur::Mirror(PNT3D pivot, VEC3D normal)
{
	if( m_curve )
		geometryMirror(m_curve, pivot, normal) ;
	if( m_strpt )
	{
		if( m_strpt->ps )
		{
			for( int i = 0 ; i < m_strpt->np ; i++ )
			{
				mathMirPnt(pivot, normal, m_strpt->ps[i], m_strpt->ps[i]) ;
			}
		}
		if( m_strpt->vs )
		{
			for( int i = 0 ; i < m_strpt->np ; i++ )
			{
				mathMirVec(pivot, normal, m_strpt->vs[i], m_strpt->vs[i]) ;
			}
		}
	}
	CalBox() ;

	return 1 ;
}

// nt add 2010/10/17
int CRefCur::Scale(double scale)
{
	if( m_curve )
	{
		geometryScale(m_curve, scale) ;
		if( entityGetType(m_curve) == etLine )
		{
			m_interval.t1 *= scale ;
			m_interval.t2 *= scale ;
		}
	}
	if( m_strpt )
	{
		if( m_strpt->ps )
		{
			for( int i = 0 ; i < m_strpt->np ; i++ )
			{
				m_strpt->ps[i][0] *= scale ;
				m_strpt->ps[i][1] *= scale ;
				m_strpt->ps[i][2] *= scale ;
			}
		}
	}
	CalBox() ;

	return 1 ;
}
//--------------------------------------------------------------
// nt modify 2009/5/9 : 考虑到导航类型的优先级别
// nt modify 2009/5/30
int CRefCur::Navigate(DNAVI* dn)
{
	if( dn &&
		m_curve )
	{
		double d ;
		PNT3D p ;

		// begin p
		if( dn->type < dnEnd )
		{
			if( curveEvalPos(m_curve, m_interval.t1, p) != ERSUCSS )
				return 0 ;
			d = mathDistPntLin(p, dn->pos, dn->dir) ;
			if( d < 2.5*dn->r ) // nt add 2.5 2009/5/30
			{
				dn->type = dnEnd ; 
				dn->featType = ftRefCur ;
				dn->feat = this ;
				memcpy(dn->nav_p, p, sizeof(PNT3D)) ;
				return 1 ;
			}
		}

		// end p
		if( dn->type < dnEnd )
		{
			if( curveEvalPos(m_curve, m_interval.t2, p) != ERSUCSS )
				return 0 ;
			d = mathDistPntLin(p, dn->pos, dn->dir) ;
			if( d < 2.5*dn->r ) // nt add 2.5 2009/5/30
			{
				dn->type = dnEnd ; 
				dn->featType = ftRefCur ;
				dn->feat = this ;
				memcpy(dn->nav_p, p, sizeof(PNT3D)) ;
				return 1 ;
			}
		}

		// middle
		if( dn->type < dnMiddle )
		{
			if( curveEvalPos(m_curve, 
							 0.5*(m_interval.t1+m_interval.t2), 
							 p) != ERSUCSS ) // middle p
				return 0 ;
			d = mathDistPntLin(p, dn->pos, dn->dir) ;
			if( d < 2.5*dn->r ) // nt add 2.5 2009/5/30
			{
				dn->type = dnMiddle ; 
				dn->featType = ftRefCur ;
				dn->feat = this ;
				memcpy(dn->nav_p, p, sizeof(PNT3D)) ;
				return 1 ;
			}
		}

		// on curve
		if( dn->type < dnOn )
		{
			if( m_strpt )
			{
				int picked ;
				double u, z ;
				if( strptPick(m_strpt, // check if it is on the curve
							  dn->pos,
							  dn->dir,
							  dn->r,
							  &z,
							  &u, 
							  &picked) != ERSUCSS )
					return 0 ;
				if( picked == ID_YES )
				{
					dn->type = dnOn ;
					dn->featType = ftRefCur ;
					dn->feat = this ;
					curveEvalPos(m_curve, u, dn->nav_p) ;
					return 1 ; // nt add 2003/7/22
				}
			}
		}

		return 2 ; // nt modify 2009/5/9, old return 1
	}
	else
		return 0 ; // error
}

// nt add 2010/3/26
int CRefCur::NavigateKeyPt(DNAVI* dn)
{
	if( dn &&
		m_curve )
	{
		double d ;
		PNT3D p ;

		// begin p
		if( dn->type < dnEnd )
		{
			if( curveEvalPos(m_curve, m_interval.t1, p) != ERSUCSS )
				return 0 ;
			d = mathDist(p, dn->pos) ;
			if( d < 2.5*dn->r )
			{
				dn->type = dnEnd ; 
				dn->featType = ftRefCur ;
				dn->feat = this ;
				memcpy(dn->nav_p, p, sizeof(PNT3D)) ;
				return 1 ;
			}
		}

		// end p
		if( dn->type < dnEnd )
		{
			if( curveEvalPos(m_curve, m_interval.t2, p) != ERSUCSS )
				return 0 ;
			d = mathDist(p, dn->pos) ;
			if( d < 2.5*dn->r )
			{
				dn->type = dnEnd ; 
				dn->featType = ftRefCur ;
				dn->feat = this ;
				memcpy(dn->nav_p, p, sizeof(PNT3D)) ;
				return 1 ;
			}
		}

		return 2 ;
	}
	else
		return 0 ; // error
}

// nt add 2011/8/22
int CRefCur::Replace(CURVE* curve, INTERVAL* interval)
{
	if( curve == NULL ||
		interval == NULL )
		return 0 ;
	if( m_curve )
		entityFree(m_curve) ;
	m_curve = curve ;
	m_interval = *interval ;
	Disc(1., 0.15) ;
	return 1 ;
}
//--------------------------------------------------------------
// nt modify 2010/12/24 : 原来显示的是曲率半径，现在直接显示曲率
int CRefCur::Disc(double tol, double ang)
{
	int i ;
	double c ;
	PNT3D p ;
	VEC3D T, N, B ;

	if( !m_curve )
		return 0 ;

	if( tol < 1.0e-6 ||
		ang < 1.0e-3 )
		return 0 ;

	if( m_strpt )
		strptFree(m_strpt) ;

	// nt add 2004/9/7
	if( curveDisc(m_curve, 
				  &m_interval, 
				  tol, 
				  ang,
				  1.,//10.,
				  STRPT_PT | STRPT_VEC | STRPT_PARAM,
				  &m_strpt) != ERSUCSS ||
				  m_strpt == NULL )
		return 0 ;

	for( i = 0 ; i < m_strpt->np ; i++ )
	{
		if( curveEvalCur(m_curve,
						 m_strpt->ts[i],
						 p,
						 T,
						 N,
						 B,
						 &c) != ERSUCSS )
			 return 0 ;
		c *= 10000. ;
		m_strpt->vs[i][0] = p[0]-N[0]*c ; // + => -, nt 2010/12/24
		m_strpt->vs[i][1] = p[1]-N[1]*c ; // + => -, nt 2010/12/24
		m_strpt->vs[i][2] = p[2]-N[2]*c ; // + => -, nt 2010/12/24
	}

	return 1 ;
}
//--------------------------------------------------------------
// nt rewrite 2009/12/22
// nt modify 2010/12/24
int	CRefCur::Draw(void* pVI, int drawMode)
{
	PNT3D p ;

	if( !m_strpt )
		return 0 ;

	SetDrawColor(drawMode) ;

	// nt add 2010/11/27
	double s = viGetScale(pVI) ;
	if( s < 1.e-10 )
		return 0 ;
	double w = m_width*0.01 ; // unit:mm
	w /= s ;
	if( w < 1. )
		w = 1. ;

	int i ;
	glLineWidth((float)w) ; // nt modify 2010/11/27
	glBegin(GL_LINE_STRIP) ;
		for( i = 0 ; i < m_strpt->np ; i++ )
			glVertex3dv(m_strpt->ps[i]) ;
	glEnd() ;

	if( m_atts&1 ) // draw curvature vectors
	{
		glBegin(GL_LINES) ;
			for( i = 0 ; i < m_strpt->np ; i++ )
			{
				glVertex3dv(m_strpt->ps[i]) ;
				glVertex3dv(m_strpt->vs[i]) ;
			}
		glEnd() ;
		// nt add 2010/12/24
		glBegin(GL_LINE_STRIP) ;
		for( i = 0 ; i < m_strpt->np ; i++ )
			glVertex3dv(m_strpt->vs[i]) ;
		glEnd() ;
	}
	glLineWidth(1.f) ; // nt modify 2010/11/27

	if( m_atts&2 ) // draw parameter mark, nt 2010/5/23
	{ // nt modify 2010/11/18
		double d = 15*viGetScale(pVI) ; // nt modify 2010/11/27
		VEC3D tvec ;
		curveEvalTangent(m_curve,
					     0.5*(m_interval.t1+m_interval.t2),
					     p,
					     tvec) ;
		tvec[0] *= d ;
		tvec[1] *= d ;
		tvec[2] *= d ;
		viDrawVector2(pVI, p, tvec, 10) ;
	}

	return 1 ;
}

int	CRefCur::Draw2(void* pVI, int drawMode)
{
	return Draw(pVI, drawMode) ;
}
//--------------------------------------------------------------
void CRefCur::Prompt(CStr& str)
{
	double d, r ;
	PNT3D p1, p2, center, temp ;
	CStr s ;

	GetBegin(p1) ;
	GetEnd(p2) ;
	d = mathDist(p1, p2) ;

	int type = entityGetType(m_curve) ;
	switch(type)
	{
	case etLine:
		str.Format("线段:B=(%6.2lf,%6.2lf,%6.2lf), E=(%6.2lf,%6.2lf,%6.2lf), L=%6.2lf",
			       p1[0],
				   p1[1],
				   p1[2],
				   p2[0],
				   p2[1],
				   p2[2],
				   d) ;
		break ;
	case etCircle:
		curveEvalPos(m_curve, 0.0, temp) ;
		curveEvalPos(m_curve, PI1, center) ;
		mathMidPnt(temp, center, center) ;
		r = mathDist(center, temp) ;
		str.Format("O=(%6.2lf,%6.2lf,%6.2lf), R=%6.2lf, L=%6.2lf", // 圆，圆弧共用
			       center[0],
				   center[1],
				   center[2],
				   r,
				   r*fabs(m_interval.t2-m_interval.t1)) ;
		if( d < 1.0e-8 )
			str = "整圆: "+str ;
		else
		{
			str = "圆弧: "+str ;
			s.Format(", B=(%6.2lf,%6.2lf,%6.2lf), E=(%6.2lf,%6.2lf,%6.2lf)",
			       p1[0],
				   p1[1],
				   p1[2],
				   p2[0],
				   p2[1],
				   p2[2]) ;
			str += s ;
		}
		break ;
	case etBCurve:
		str.Format("曲线:B=(%6.2lf,%6.2lf,%6.2lf), E=(%6.2lf,%6.2lf,%6.2lf)",
			       p1[0],
				   p1[1],
				   p1[2],
				   p2[0],
				   p2[1],
				   p2[2]) ;
		break ;
	default:
		return ;
	}

	// nt add 2010/4/8
	//CString lName ;
	//GetLName(lName) ;
	//if( lName != "" )
	//{
	//	str += ", " ;
	//	str += lName ;
	//}

	return ;
}
//--------------------------------------------------------------
int	CRefCur::Pick(PINFO* pi)
{
	int picked ;
	double u, z ;

	if( !m_strpt )
		return 0 ;

	// nt add 2001/11/28
	if( mathChkBoxLinInt(&m_box, pi->pos, pi->dir, pi->r) == IDNOINT )
		return 2 ; // nt modify 2003/7/22

	if( strptPick(m_strpt,
				  pi->pos,
				  pi->dir,
				  pi->r,
				  &z,
				  &u, 
				  &picked) != ERSUCSS )
		return 0 ;
	if( picked == ID_YES &&
		(pi->obj == NULL ||
		pi->z > z) )
	{
		pi->ot = ftRefCur ; // nt add 2003/7/22
		pi->obj = this ;
		pi->z = z ;
		pi->u = u ;
		pi->v = 0.0 ;
		m_u = u ; // nt add 2010/4/22
		pi->att = 1 ; // nt add 2011/7/6
		return 1 ; // nt add 2003/7/22
	}

	return 2 ; // nt modify 2003/7/22
}

// nt add 2010/4/22
int CRefCur::PickByBox(RFRAME* pLF, BOX2D* pLB, double r)
{
	int i ;
	PNT3D p, q ;

	if( m_strpt == NULL ||
		m_strpt->np < 1 )
		return 0 ;

	mathTransWorldPnt3D(pLF, m_strpt->ps[0], p) ;
	for( i = 1 ; i < m_strpt->np ; i++ )
	{
		mathTransWorldPnt3D(pLF, m_strpt->ps[i], q) ;
		if( mathChkBox2DSegmInt(pLB, // intersect with segment
								p, 
								q,
								r) == IDINT )
			return 1 ;
		memcpy(p, q, sizeof(PNT3D)) ;
	}

	return 2 ;
}
//--------------------------------------------------------------
// nt modify 2010/11/27 : 
// (1)add flag
// (2)add m_w
// (3)add m_ltName
// 注意m_dtype<256
int	CRefCur::Serialize(CArchive& ar)
{
	int type, flag ;

	CFeature::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		flag = m_dtype+256 ; // nt add 2010/11/27
		ar << flag //m_dtype
		   << m_interval.t1
		   << m_interval.t2
		   << m_width // nt add 2010/11/27
		   << m_ltName ; // nt add 2010/11/27
		ar.Write(&m_box, sizeof(BOX3D)) ;
		type = entityGetType(m_curve) ;
		ar << type ;
		curveSerialize(m_curve, 0, &ar, arReadWrite) ; // nt modify 2020/4/11
	}
	else
	{
		ar >> flag ; // nt add 2010/11/27
		if( flag >= 256 ) // nt add 2010/11/27
			m_dtype = flag%256 ; // nt add 2010/11/27
		ar >> m_interval.t1
		   >> m_interval.t2 ;
		if( flag >= 256 ) // nt add 2010/11/27
		{
			ar >> m_width // nt add 2010/11/27
			   >> m_ltName ; // nt add 2010/11/27
		}
		else
		{
			m_width = 0 ; // nt add 2010/11/27
			m_ltName = "" ; // nt add 2010/11/27
		}
		ar.Read(&m_box, sizeof(BOX3D)) ;
		ar >> type ;
		m_curve = entityAlloc(type) ;
		if( curveSerialize(m_curve, 1, &ar, arReadWrite) == ERSUCSS ) // nt modify 2020/4/11
		{
			//Scale(1000.) ;
			Disc(1., 0.15) ;
		}
		else // nt add 2009/5/25
		{
			envFree(m_curve) ;
			m_curve = NULL ;
			return 0 ; // nt add 2011/5/3
		}
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
