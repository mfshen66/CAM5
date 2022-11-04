/*	2002/8/19 nt
 *	implementation file doc command CDrawArc
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "DrawArc.h"

///////////////////////////////////////////////////////////////
//	dynamic arc 2d
/*	2002/8/19 nt
 *	动态圆弧的创建,删除与初始化
 */
CDrawArc::CDrawArc(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_drawed = 0 ; // nt add 2009/9/15
	m_stage = 0 ;
	m_cor = 1 ; // nt add 2009/11/26

	return ;
}

CDrawArc::~CDrawArc()
{
}
//--------------------------------------------------------------
int CDrawArc::GetID()
{
	return dctDrawArc ;
}

// nt add 2009/11/25
int CDrawArc::SetCor(int cor)
{
	m_cor = cor ;
	return 1 ;
}

/*	2001/11/22 nt
 *	call when lbutton down
 *  nt modify 2009/9/15
 */
int CDrawArc::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	// project p along v to current workplane
	Navigate(0/*nvFreePt*/, nFlags, p, v) ;
	switch(m_stage)
	{
	case 0:
		GetNavP2D(m_p1) ;
		m_stage++ ;
		break ;
	case 1:
		if( m_drawed )
			Draw() ;
		GetNavP2D(m_p3) ;
		m_stage++ ;
		break ;
	case 2:
		if( m_drawed )
			Draw() ;
		GetNavP2D(m_p2) ;
		m_stage++ ;
		break ;
	}

	return 1 ;
}

// nt modify 2009/9/15
int CDrawArc::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	// generate arc
	if( m_stage == 3 )
	{
		// nt add 2009/11/26
		camPromptClear() ;
		if( m_cor )
		{
			int ks[2], k ;
			double rs[2] ;
			PNT2D cs[2] ;
			VEC2D vs[2] ;
			for( k = 0 ; k < 2 ; k++ )
			{
				if( PickLin2D(k<1?m_p1:m_p3, cs[k], vs[k]) == 1 )
					ks[k] = 0 ; // pick a line
				else
				if( PickCir2D(k<1?m_p1:m_p3, cs[k], rs[k]) == 1 )
					ks[k] = 1 ; // pick a circle
				else
					ks[k] = -1 ; // pick none
			}

			double ctol = 10*viGetScale(m_pVI) ;
			if( ks[0] == 0 && // line
				ks[1] == -1 ) // none
			{
				if( lcCorArc(cs[0], vs[0], m_p3, 0., ctol, m_p1, m_p2, m_p3, NULL, NULL) == 1 )
					camPromptStr("切点数=1") ;
			}
			else
			if( ks[0] == -1 && // none
				ks[1] == 0 ) // line
			{
				if( lcCorArc(cs[1], vs[1], m_p1, 0., ctol, m_p3, m_p2, m_p1, NULL, NULL) == 1 )
					camPromptStr("切点数=1") ;
			}
			else
			if( ks[0] == 1 && // circle
				ks[1] == -1 ) // none
			{
				if( ccCorArc(cs[0], rs[0], m_p3, 0., ctol, m_p1, m_p2, m_p3, NULL, NULL) == 1 )
					camPromptStr("切点数=1") ;
			}
			else
			if( ks[0] == -1 && // none
				ks[1] == 1 ) // circle
			{
				if( ccCorArc(m_p1, 0., cs[1], rs[1], ctol, m_p1, m_p2, m_p3, NULL, NULL) == 1 )
					camPromptStr("切点数=1") ;
			}
			else
			if( ks[0] == 0 && // line
				ks[1] == 0 ) // line
			{
				if( llCorArc(cs[0], vs[0], cs[1], vs[1], ctol, m_p1, m_p2, m_p3, NULL, NULL) == 1 )
					camPromptStr("切点数=2") ;
			}
			else
			if( ks[0] == 0 && // line
				ks[1] == 1 ) // cirle
			{
				if( lcCorArc(cs[0], vs[0], cs[1], rs[1], ctol, m_p1, m_p2, m_p3, NULL, NULL) == 1 )
					camPromptStr("切点数=2") ;
			}
			if( ks[0] == 1 && // circle
				ks[1] == 0 ) // line
			{
				if( lcCorArc(cs[1], vs[1], cs[0], rs[0], ctol, m_p3, m_p2, m_p1, NULL, NULL) == 1 )
					camPromptStr("切点数=2") ;
			}
			else
			if( ks[0] == 1 && // circle
				ks[1] == 1 ) // circle
			{
				if( ccCorArc(cs[0], rs[0], cs[1], rs[1], ctol, m_p1, m_p2, m_p3, NULL, NULL) == 1 )
					camPromptStr("切点数=2") ;
			}
		}

		m_stage = 0 ;
		m_drawed = 0 ;
		PNT2D center ;
		double r, a1, a2 ;
		if( math2dCalCRAAEx1(m_p1, 
							 m_p2,
							 m_p3, 
							 MIN_LEN, 
							 MIN_ANG, 
							 center,
							 &r, 
							 &a1,
							 &a2) != ERSUCSS )
				goto ExceExit ;
		RFRAME lf = m_lf ;
		ToWorld(center, lf.O) ;
		CURVE* circle = circleCreate(&lf, r) ;
		if( !circle )
			return 0 ;
		INTERVAL interval ;
		interval.t1 = a1 ;
		interval.t2 = a2 ;

		// generate refcur
		CRefCur* pRefCur = new CRefCur(0, circle, &interval) ;
		if( !pRefCur )
		{
			entityFree(circle) ;
			goto ExceExit ;
		}
		m_pDoc->AddFeature(pRefCur) ;
	}

	return 1 ;
ExceExit:
	if( m_drawed )
		Draw() ;
	return 1 ;
}

int CDrawArc::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	//
	double tol, d ;
	PNT2D q ;
	switch(m_stage)
	{
	case 1: // 已经得到m_p1, in the case of stage_1
		if( m_drawed )
			Draw() ;
		SetBegin(m_p1) ;
		Navigate(0/*nvLinEnd*/, nFlags, p, v) ;
		GetNavP2D(m_p3) ;
		Draw() ; // draw
		break ;
	case 2: // 已经得到m_p1,m_p3, in the case of stage_2
		if( m_drawed )
			Draw() ;
		SetP1(m_p1) ;
		SetP2(m_p3) ;
		Navigate(0/*nvArc3P*/, nFlags, p, v) ;
		GetNavP2D(q) ;
		tol = viGetScale(m_pVI) ;
		d = mathDistPntLin2DEx(q, m_p1, m_p3) ;
		if( fabs(d) > tol )
		{
			memcpy(m_p2, q, sizeof(PNT2D)) ;
			Draw() ;
		}
		break ;
	default: // nt add 2004/7/12
		Navigate(0/*nvFreePt*/, nFlags, p, v) ;
		break ;
	}

	return 1 ;
}

int CDrawArc::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( m_drawed ) // nt add 2009/9/15
		Draw() ;
	m_pDoc->ClearCmdAll() ; // nt add 2002/12/4

	return 1 ;
}

// nt add 2009/10/21
// nt modify 2009/11/4 : 处理局部坐标系
int CDrawArc::OnReturn()
{
	int n ;
	double x, y, r, d, h ;
	PNT2D p, center ;
	VEC2D v ;
	CStr str ;
	camPromptGet(str) ;
	camPromptClear() ;
	n = strToNums(str, x, y) ;
	switch(m_stage)
	{
	case 0:
		if( n == 2 )
		{
			m_p1[0] = x ; // local
			m_p1[1] = y ; // local
			m_stage++ ;
		}
		break ;
	case 1:
		if( n == 2 )
		{
			if( m_drawed )
				Draw() ;
			if( str.Find("DX=") >= 0 || // dx,dy
				str.Find("dx=") >= 0 ||
				str.Find("DY=") >= 0 ||
				str.Find("dy=") >= 0 )
			{
				m_p3[0] = m_p1[0]+x ;
				m_p3[1] = m_p1[1]+y ;
			}
			else
			{
				m_p3[0] = x ;
				m_p3[1] = y ;
			}
			m_stage++ ;
		}
		break ;
	case 2:
		if( n == 2 )
		{
			if( m_drawed )
				Draw() ;
			if( str.Find("DX=") >= 0 || // dx,dy
				str.Find("dx=") >= 0 ||
				str.Find("DY=") >= 0 ||
				str.Find("dy=") >= 0 )
			{
				m_p2[0] = m_p3[0]+x ;
				m_p2[1] = m_p3[1]+y ;
			}
			else
			{
				m_p2[0] = x ;
				m_p2[1] = y ;
			}
			m_stage++ ;
		}
		else
		if( n == 1 ) // x作为半径
		{
			r = x ;
			d = 0.5*math2dDist(m_p1, m_p3) ;
			if( d > MIN_LEN &&
				r > d-MIN_LEN )
			{
				h = sqrt(fabs(r*r-d*d)) ;
				v[0] = (m_p1[1]-m_p3[1])/(2*d) ;
				v[1] = (m_p3[0]-m_p1[0])/(2*d) ;
				math2dMidPnt(m_p1, m_p3, p) ;
				if( (m_p2[0]-p[0])*v[0]+
					(m_p2[1]-p[1])*v[1] > 0. )
					h = -h ;
				center[0] = p[0]+v[0]*h ;
				center[1] = p[1]+v[1]*h ;
				v[0] = m_p2[0]-center[0] ;
				v[1] = m_p2[1]-center[1] ;
				d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
				if( d > MIN_LEN )
				{
					if( m_drawed )
						Draw() ;
					v[0] /= d ;
					v[1] /= d ;
					m_p2[0] = center[0]+v[0]*r ;
					m_p2[1] = center[1]+v[1]*r ;
					m_stage++ ;
				}
			}
		}
		break ;
	}
	if( m_stage == 3 )
		OnLButtonUp(0, NULL, NULL) ;

	return 1 ;
}

int CDrawArc::Cancel()
{
	return 1 ;
}

/*	2002/8/19
 *	画
 */
void CDrawArc::Draw()
{
	// draw
	switch(m_stage)
	{
	case 1:
		glColor3ub(255, 255, 255) ;
		DrawLin2D(m_p1, m_p3) ;
		m_drawed = m_drawed?0:1 ;
		break ;
	case 2:
		double r, a1, a2 ;
		PNT2D center ;
		if( math2dCalCRAAEx1(m_p1, 
							 m_p2,
							 m_p3, 
							 MIN_LEN, 
							 MIN_ANG, 
							 center,
							 &r, 
							 &a1,
							 &a2) != ERSUCSS )
			return ;
		glColor3ub(255, 255, 255) ;
		DrawArc2D(center, r, a1, a2) ;
		m_drawed = m_drawed?0:1 ;
		break ;
	}

	return ;
}
///////////////////////////////////////////////////////////////

