/*	2002/8/19 nt
 *	implementation file doc command CDrawCir
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "DrawCir.h"

///////////////////////////////////////////////////////////////
//	dynamic circle 2d
/*	2002/8/19 nt
 *	动态圆的创建,删除与初始化
 */
CDrawCir::CDrawCir(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_drawed = 0 ; // nt add 2009/9/15
	m_stage = 0 ;
	m_cor = 1 ; // nt add 2009/11/25

	return ;
}

CDrawCir::~CDrawCir()
{
}
//--------------------------------------------------------------
int CDrawCir::GetID()
{
	return dctDrawCir ;
}

// nt add 2009/11/25
int CDrawCir::SetCor(int cor)
{
	m_cor = cor ;
	return 1 ;
}

/*	2001/11/22 nt
 *	call when lbutton down
 */
int CDrawCir::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	// stage from 0 to 1
	if( m_stage == 0 )
	{
		Navigate(0, nFlags, p, v) ;
		GetNavP(m_p1) ;
		m_stage++;
	}
	else
	if( m_stage == 1 )
	{
		if( m_drawed )
			Draw() ;
		Navigate(0, nFlags, p, v) ;
		GetNavP(m_p2) ;
		m_stage++;
	}

	return 1 ;
}

int CDrawCir::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	if( m_stage == 2 )
	{
		m_stage = 0 ;
		double tol = 2.5*viGetScale(m_pVI) ;
		double r = mathDist(m_p1, m_p2) ;
		if( r < tol ) // if too near
			return 1 ;

		// nt add 2009/11/25
		camPromptClear() ;
		if( m_cor ) // 使相切
		{
			PNT3D p, c ;
			VEC3D dir ;
			double d, R, ctol = 7.5*viGetScale(m_pVI) ;
			if( PickLin(m_p2, p, dir) == 1 ) // 注意dir垂直于m_lf.Z
			{
				d = fabs(mathDistPntLin(m_p1, p, dir)) ;
				if( fabs(d-r) < ctol )
				{
					r = d ;
					camPromptStr("一个切点") ;
				}
			}
			else
			if( PickCir(m_p2, c, R) == 1 )
			{
				d = mathDist(m_p1, c) ;
				if( d < R &&
					fabs(R-r-d) < ctol ) // 找到一个内切圆
				{
					camPromptStr("一个切点") ;
					r = R-d ;
				}
				else
				if( d < r &&
					fabs(r-R-d) < ctol ) // 找到一个内切圆
				{
					camPromptStr("一个切点") ;
					r = R+d ;
				}
				else
				if( fabs(r+R-d) < ctol ) // 找到一个外切
				{
					camPromptStr("一个切点") ;
					r = d-R ;
				}
			}
		}

		// generate circle
		RFRAME lf = m_lf ;
		memcpy(lf.O, m_p1, sizeof(PNT3D)) ;
		CURVE* circle = circleCreate(&lf, r) ;
		if( !circle )
			return 0 ;
		INTERVAL interval ;
		interval.t1 = 0.0 ;
		interval.t2 = PI2 ;

		// generate refcur
		CRefCur* pRefCur = new CRefCur(0, circle, &interval) ;
		if( !pRefCur )
		{
			entityFree(circle) ;
			return 0 ;
		}
		// add in part
		m_pDoc->AddFeature(pRefCur) ; // nt add 2003/1/6
	}

	return 1 ;
}

int CDrawCir::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	//
	camPromptClear() ;
	if( m_stage == 0 )
	{
		Navigate(0, nFlags, p, v) ;
		GetNavP(m_p1) ;
	}
	else
	{
		SetCenter(m_p1) ;
		Navigate(0, nFlags, p, v) ;
	}
	
	// nt modify 2009/10/11
	if( m_stage == 1 )
	{
		if( m_drawed )
			Draw() ; // eraze
		GetNavP(m_p2) ; // stage from 1 to 2
		Draw() ; // draw
	}

	return 1 ;
}

int CDrawCir::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( m_drawed ) // nt add 2009/9/15
		Draw() ;
	m_pDoc->ClearCmdAll() ; // nt add 2002/12/4

	return 1 ;
}

// nt add 2009/10/21
// 只处理3种情况:
// (1)r,
// (2)x,y
// (3)dx,dy
// nt modify 2009/11/4 : 处理局部坐标系
int CDrawCir::OnReturn()
{
	double d ;
	VEC3D v ;
	CStr str ;
	camPromptGet(str) ;
	camPromptClear() ;
	double x, y, r ;
	if( str.Find(',') >= 0 || // 不只一个参数
		str.Find(' ') >= 0 )
	{
		if( str.Find("DX=") >= 0 || // dx,dy
			str.Find("dx=") >= 0 ||
			str.Find("DY=") >= 0 ||
			str.Find("dy=") >= 0 )
		{
			if( m_stage == 1 &&
				strToNums(str, x, y) == 2 )
			{
				if( m_drawed )
					Draw() ;
				PNT2D p ;
				ToPlane(m_p1, p) ; // to local
				m_p2[0] = p[0]+x ;
				m_p2[1] = p[1]+y ;
				ToWorld(m_p2, m_p2) ;
				m_stage++ ;
			}
			else
				return 1 ;
		}
		else
		if( strToNums(str, x, y) == 2 )
		{ // x,y
			if( m_stage == 0 )
			{
				m_p1[0] = x ; // local
				m_p1[1] = y ; // local
				ToWorld(m_p1, m_p1) ;
				m_stage++ ;
				return 1 ;
			}
			else
			if( m_stage == 1 )
			{
				if( m_drawed )
					Draw() ;
				m_p2[0] = x ; // local
				m_p2[1] = y ; // local
				ToWorld(m_p2, m_p2) ;
				m_stage++ ;
			}
			else
				return 1 ;
		}
		else
			return 1 ;
	}
	else
	{
		if( m_stage == 1 &&
			strToNums(str, r) == 1 &&
			r > MIN_RAD )
		{
			v[0] = m_p2[0]-m_p1[0] ;
			v[1] = m_p2[1]-m_p1[1] ;
			v[2] = m_p2[2]-m_p1[2] ;
			d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
			if( d > MIN_LEN )
			{
				v[0] /= d ;
				v[1] /= d ;
				v[2] /= d ;
			}
			else
			{
				v[0] = 1. ;
				v[1] = 0. ;
				v[2] = 0. ;
			}
			if( m_drawed )
				Draw() ;
			m_p2[0] = m_p1[0]+v[0]*r ;
			m_p2[1] = m_p1[1]+v[1]*r ;
			m_p2[2] = m_p1[2]+v[2]*r ;
			m_stage++ ;
		}
		else
			return 1 ;
	}
	OnLButtonUp(0, m_p2, v) ;

	return 1 ;
}

int CDrawCir::Cancel()
{
	return 1 ;
}

/*	2002/8/19
 *	画动态圆
 */
void CDrawCir::Draw()
{
	double r = mathDist(m_p1, m_p2) ;
	if( r > MIN_RAD )
	{
		glColor3ub(255, 255, 255) ;
		RFRAME lf = m_lf ;
		memcpy(lf.O, m_p1, sizeof(PNT3D)) ;
		DrawArc(&lf, r, PI2) ;
		m_drawed = m_drawed?0:1 ; // nt add 2009/9/15
	}
	
	return  ;
}
///////////////////////////////////////////////////////////////
