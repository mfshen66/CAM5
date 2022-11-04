/*	2000/7/18 nt
 *	implementation file class command
 */
#include "StdAfx.h"
#include "Models.h"
#include "Mills.h" // nt add 2010/4/1
#include "CAMDoc.h"
#include "CAMView.h"
#include "Command.h"

///////////////////////////////////////////////////////////////
//	2000/5/13 nt
//	CViewCmd
CCommand::CCommand()
{
}

CCommand::~CCommand()
{
}
//--------------------------------------------------------------
// nt modify 2010/9/14 : 考虑了没有当前工作平面的情况
int CCommand::Init(CCAMDoc* pDoc, CCAMView* pView)
{
	if( pDoc &&
		pView )
	{
		m_pDoc = pDoc ;
		m_pView = pView ;
		m_pVI = pView->GetVI() ;
		m_mouse = 0 ;
		m_drawed = 0 ;
		m_raw_p[0] = 0. ;
		m_raw_p[1] = 0. ;
		m_raw_p[2] = 0. ;
		m_begin[0] = 0. ;
		m_begin[1] = 0. ;
		m_begin[2] = 0. ;
		m_center[0] = 0. ;
		m_center[1] = 0. ;
		m_center[2] = 0. ;
		m_p1[0] = 0. ;
		m_p1[1] = 0. ;
		m_p1[2] = 0. ;
		m_p2[0] = 0. ;
		m_p2[1] = 0. ;
		m_p2[2] = 0. ;
		m_nav_p[0] = 0. ;
		m_nav_p[1] = 0. ;
		m_nav_p[2] = 0. ;
		m_color = 0 ;
		m_ctol = 1 ; // nt add 2010/9/13
		CRefPlane* pWorkPlane = pDoc->GetWorkPlane() ;
		if( pWorkPlane )
			pWorkPlane->GetLF(&m_lf) ;
		else
		{
			viGetCenterLF(m_pVI, m_lf.X, m_lf.Y, m_lf.Z, m_lf.O) ;
			m_lf.scale = 1. ;
		}
		return 1 ;
	}
	else
	{
		m_pDoc = NULL ;
		m_pView = NULL ;
		m_pVI = NULL ;
		return 0 ;
	}
}
//--------------------------------------------------------------
// nt add 2010/9/13
void CCommand::GetLF(RFRAME* pLF)
{
	if( pLF )
		*pLF = m_lf ;
	return ;
}

int CCommand::ToWP(PNT3D p, VEC3D v, PNT3D worldPt)
{
	if( mathIntLinPln(p,
					  v,
					  m_lf.O,
					  m_lf.Z,
					  MIN_LEN,
					  MIN_ANG,
					  worldPt) == IDINT )
		return 1 ;
	else
		return 0 ;
}

// nt add 2010/9/14
// 计算在m_lf的XY平面上的一个二维点
int CCommand::ToWP2D(PNT3D p, VEC3D v, PNT2D wpPt)
{
	PNT3D worldPt ;
	if( mathIntLinPln(p,
					  v,
					  m_lf.O,
					  m_lf.Z,
					  MIN_LEN,
					  MIN_ANG,
					  worldPt) == IDINT )
	{
		VEC3D v ;
		v[0] = worldPt[0]-m_lf.O[0] ;
		v[1] = worldPt[1]-m_lf.O[1] ;
		v[2] = worldPt[2]-m_lf.O[2] ;
		wpPt[0] = v[0]*m_lf.X[0]+v[1]*m_lf.X[1]+v[2]*m_lf.X[2] ;
		wpPt[1] = v[0]*m_lf.Y[0]+v[1]*m_lf.Y[1]+v[2]*m_lf.Y[2] ;
		return 1 ;
	}
	else
	{
		wpPt[0] = 0. ;
		wpPt[1] = 0. ;
		return 0 ;
	}
}

// nt add 2009/12/17
int CCommand::ToPlane(PNT3D worldPt, PNT2D planePt)
{
	VEC3D v ;

	v[0] = worldPt[0]-m_lf.O[0] ;
	v[1] = worldPt[1]-m_lf.O[1] ;
	v[2] = worldPt[2]-m_lf.O[2] ;
	planePt[0] = mathOProduct(m_lf.X, v) ;
	planePt[1] = mathOProduct(m_lf.Y, v) ;

	return 1 ;
}

// nt add 2009/12/17
int CCommand::ToWorld(PNT2D planePt, PNT3D worldPt)
{
	double x, y ;
	x = planePt[0] ;
	y = planePt[1] ;
	worldPt[0] = m_lf.O[0]+m_lf.X[0]*x+m_lf.Y[0]*y ;
	worldPt[1] = m_lf.O[1]+m_lf.X[1]*x+m_lf.Y[1]*y ;
	worldPt[2] = m_lf.O[2]+m_lf.X[2]*x+m_lf.Y[2]*y ;
	return 1 ;
}
//--------------------------------------------------------------
// nt add 2009/12/17
int CCommand::GetColor()
{
	return m_color ;
}

// nt add 2009/12/17
int CCommand::SetColor(int color)
{
	m_color = color ;
	return 1 ;
}
//--------------------------------------------------------------
int CCommand::SetBegin(PNT3D begin)
{
	memcpy(m_begin, begin, sizeof(PNT3D)) ;
	return 1 ;
}

int CCommand::SetCenter(PNT3D center)
{
	memcpy(m_center, center, sizeof(PNT3D)) ;
	return 1 ;
}

int CCommand::SetP1(PNT3D p1)
{
	memcpy(m_p1, p1, sizeof(PNT3D)) ;
	return 1 ;
}

int CCommand::SetP2(PNT3D p2)
{
	memcpy(m_p2, p2, sizeof(PNT3D)) ;
	return 1 ;
}

/*	2003/1/6 nt
 *	navigate
 *	nt modify 2009/9/4 : 合并了几种导航情况
 *  p1,p2可能为空
 *	注意调用该函数前，设置好
 *		(1)m_begin, or
 *		(2)m_center, or
 *		(3)m_p1, m_p2
 *	这个导航接口的思路来源于CAM2D,
 *	目前暂不支持SetBegin(),
 *			    SetCenter(),
 *				...
 */
int CCommand::Navigate(int type, UINT nFlags, PNT2D raw_p, VEC3D v)
{
	if( m_pView &&
		m_pDoc &&
		m_pVI &&
		raw_p &&
		v )
	{
		int rt = 0 ;
		if( nFlags & MK_CONTROL )
			rt = ToWP(raw_p, v, m_nav_p) ;
		else
		{
			double r = 5.0*viGetScale(m_pVI) ;
			DNAVI dn ;
			dnaviInit(&dn, raw_p, v, r) ; // nt modify 2009/5/9
			m_pDoc->Navigate(&dn) ;
			if( dn.feat )
			{
				memcpy(m_nav_p, dn.nav_p, sizeof(double[3])) ;
				rt = 1 ;
			}
			else
				rt = ToWP(raw_p, v, m_nav_p) ;
		}
		return rt ;
	}
	else
		return 0 ;
}

int CCommand::GetNavP(PNT3D nav_p)
{
	memcpy(nav_p, m_nav_p, sizeof(PNT3D)) ;
	return 1 ;
}

// nt add 2010/9/14
int CCommand::GetNavP2D(PNT2D nav_p)
{
	ToPlane(m_nav_p, nav_p) ;
	return 1 ;
}
//--------------------------------------------------------------
// nt add 2009/11/26
int CCommand::GetCTol()
{
	return m_ctol ;
}

// nt add 2009/11/26
int CCommand::SetCTol(int ctol)
{
	m_ctol = ctol ;
	return 1 ;
}

// line, nt add 2009/11/26
// return 0=error,
//        1=picked,
//        2=not picked
int CCommand::PickLin(PNT3D p, PNT3D begin, VEC3D dir)
{
	if( m_pDoc )
	{
		CPart* pPart = m_pDoc->GetPart() ;
		if( pPart )
		{
			PINFO pi ;
			pi.obj = NULL ;
			pi.ot = 0 ;
			pi.u = 0. ;
			pi.v = 0. ;
			pi.z = 0. ;
			pi.type = ftRefCur ;
			pi.r = 2.5*viGetScale(m_pVI) ; // ???
			CRefCur* pRefCur ;
			CURVE* curve ;
			int type ;
			PNT3D p1, p2 ;
			memcpy(pi.pos, p, sizeof(PNT3D)) ;
			memcpy(pi.dir, m_lf.Z, sizeof(VEC3D)) ;
			if( pPart->Pick(&pi) == 1 )
			{
				pRefCur = (CRefCur*)pi.obj ;
				curve = pRefCur->GetCurve() ;
				type = entityGetType(curve) ;
				if( type == etLine )
				{
					curveEvalPos(curve, 0., p1) ;
					curveEvalPos(curve, 1., p2) ;
					memcpy(begin, p1, sizeof(PNT3D)) ;
					dir[0] = p2[0]-p1[0] ;
					dir[1] = p2[1]-p1[1] ;
					dir[2] = p2[2]-p1[2] ;
					if( mathIsPerpendicular(dir, m_lf.Z, 100*MIN_ANG) == ID_YES &&
						fabs(mathDistPntPln(p1, m_lf.O, m_lf.Z)) < 1.e-3 )
						return 1 ;
					else
						return 2 ;
				}
			}
		}
	}

	return 2 ;
}

// circle or arc, nt add 2009/11/26
// return 0=error,
//        1=picked,
//        2=not picked
int CCommand::PickCir(PNT3D p, PNT3D center, double& r)
{
	if( m_pDoc )
	{
		CPart* pPart = m_pDoc->GetPart() ;
		if( pPart )
		{
			PINFO pi ;
			pi.obj = NULL ;
			pi.ot = 0 ;
			pi.u = 0. ;
			pi.v = 0. ;
			pi.z = 0. ;
			pi.type = ftRefCur ;
			pi.r = 2.5*viGetScale(m_pVI) ; // ???
			CRefCur* pRefCur ;
			CURVE* curve ;
			int type ;
			PNT3D p1, p2, p3 ;
			VEC3D X, Y ;
			memcpy(pi.pos, p, sizeof(PNT3D)) ;
			memcpy(pi.dir, m_lf.Z, sizeof(VEC3D)) ;
			if( pPart->Pick(&pi) == 1 )
			{
				pRefCur = (CRefCur*)pi.obj ;
				curve = pRefCur->GetCurve() ;
				type = entityGetType(curve) ;
				if( type == etCircle )
				{
					curveEvalPos(curve, 0., p1) ;
					curveEvalPos(curve, PI1, p2) ;
					mathMidPnt(p1, p2, center) ;
					r = 0.5*mathDist(p1, p2) ;
					X[0] = (p1[0]-center[0])/r ;
					X[1] = (p1[1]-center[1])/r ;
					X[2] = (p1[2]-center[2])/r ;
					curveEvalPos(curve, 0.5*PI1, p3) ;
					Y[0] = (p3[0]-center[0])/r ;
					Y[1] = (p3[1]-center[1])/r ;
					Y[2] = (p3[2]-center[2])/r ;
					if( mathIsPerpendicular(X, m_lf.Z, 100*MIN_ANG) == ID_YES &&
						mathIsPerpendicular(Y, m_lf.Z, 100*MIN_ANG) == ID_YES &&
						fabs(mathDistPntPln(center, m_lf.O, m_lf.Z)) < 1.e-3 )
						return 1 ;
					else
						return 2 ;
				}
			}
		}
	}

	return 2 ;
}
//--------------------------------------------------------------
// nt add 2010/9/14
int CCommand::PickLin2D(PNT2D p, PNT2D begin, VEC2D dir)
{
	PNT3D q, b ;
	VEC3D v, prj_v ;
	ToWorld(p, q) ;
	int rt = PickLin(q, b, v) ;
	if( rt == 1 )
	{
		ToPlane(b, begin) ;
		mathPrjVecPln(v, m_lf.Z, prj_v) ;
		prj_v[2] = 0. ;
		mathUniVec(prj_v, MIN_DBL) ;
		dir[0] = prj_v[0] ;
		dir[1] = prj_v[1] ;
	}
	return rt ;
}

// nt add 2010/9/14
int CCommand::PickCir2D(PNT2D p, PNT2D center, double& r)
{
	PNT3D q, c ;
	ToWorld(p, q) ;
	int rt = PickCir(q, c, r) ;
	if( rt == 1 )
	{
		ToPlane(c, center) ;
	}
	return rt ;
}
//--------------------------------------------------------------
// nt add 2009/12/17
void CCommand::DrawLin(PNT3D p1, PNT3D p2)
{
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	glBegin(GL_LINES) ;
		glVertex3dv(p1) ;
		glVertex3dv(p2) ;
	glEnd() ;
	glDisable(GL_COLOR_LOGIC_OP) ;
	glFlush() ;

	return ;
}

// nt add 2009/12/17
void CCommand::DrawRect(PNT3D p1, PNT3D p2)
{
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	double X[3], Y[3], Z[3] ;
	double O[3], c1[3], c2[3] ;
	viGetCenterLF(m_pVI, X, Y, Z, O) ;
	viPrjPntLin(p2, p1, X, c1) ;
	viPrjPntLin(p2, p1, Y, c2) ;
	glBegin(GL_LINE_LOOP) ;
		glVertex3dv(p1) ;
		glVertex3dv(c1) ;
		glVertex3dv(p2) ;
		glVertex3dv(c2) ;
	glEnd() ;
	glDisable(GL_COLOR_LOGIC_OP) ;
	glFlush() ;
	return ;
}

// nt add 2009/9/15
void CCommand::DrawArc(RFRAME* pLF, 
					   double r, 
					   double a)
{
	if( !pLF || // nt add 2009/9/18
		r < MIN_RAD ||
		a < MIN_ANG )
		return ;
	double _tol = 5.*viGetScale(m_pVI) ;
	if( _tol > 0.5*r )
		_tol = 0.5*r ;
	double h = mathMIN(sqrt((2.*r-_tol)*_tol), 0.5*r) ;
	double ag = 2.*asin(h/r) ;
	double _ang = 0.15 ;
	ag = mathMAX(ag, MIN_ANG) ; // ag > MIN_ANG
	ag = mathMIN(ag, _ang) ; // ag < _ang
	int nSegm = (int)ceil(fabs(a)/ag) ;
	nSegm = (nSegm/6)*6 ; // nt add 2009/9/16
	nSegm = mathMAX(nSegm, 6) ; // nSegm >= 1
	if( nSegm > 1000 ) // nt add 2009/9/18
		nSegm = 1000 ;
	double step = a/nSegm ;
	double ca = cos(step) ;
	double sa = sin(step) ;
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	glBegin(GL_LINE_STRIP) ;
		int i ;
		PNT3D p ;
		double x1, y1, x2, y2 ;
		for( i = 0, x1 = r, y1 = 0. ; i <= nSegm ; i++ )
		{
			p[0] = pLF->O[0]+pLF->X[0]*x1+pLF->Y[0]*y1 ;
			p[1] = pLF->O[1]+pLF->X[1]*x1+pLF->Y[1]*y1 ;
			p[2] = pLF->O[2]+pLF->X[2]*x1+pLF->Y[2]*y1 ;
			glVertex3dv(p) ;
			x2 = x1*ca-y1*sa ;
			y2 = y1*ca+x1*sa ;
			x1 = x2 ;
			y1 = y2 ;
		}
	glEnd() ;
	glDisable(GL_COLOR_LOGIC_OP) ;
	glFlush() ;

	return ;
}
//--------------------------------------------------------------
// nt add 2010/9/14
void CCommand::DrawLin2D(PNT2D p1, PNT2D p2)
{
	PNT3D q1, q2 ;
	ToWorld(p1, q1) ;
	ToWorld(p2, q2) ;
	DrawLin(q1, q2) ;

	return ;
}

// nt add 2010/9/14
void CCommand::DrawRect2D(PNT2D p1, PNT2D p2)
{
	PNT3D q1, q2 ;
	ToWorld(p1, q1) ;
	ToWorld(p2, q2) ;
	DrawRect(q1, q2) ;

	return ;
}

// nt add 2010/9/14
void CCommand::DrawArc2D(PNT2D center, double r, double a1, double a2)
{
	RFRAME lf = m_lf ;
	ToWorld(center, lf.O) ;
	mathRotVec(lf.Z, lf.O, a1, lf.X, lf.X) ;
	mathRotVec(lf.Z, lf.O, a1, lf.Y, lf.Y) ;
	DrawArc(&lf, r, a2-a1) ;

	return ;
}
///////////////////////////////////////////////////////////////
