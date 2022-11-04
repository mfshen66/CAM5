/*	2009/5/7 nt
 *	implementation file CDrawLin
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "DrawLin.h"

///////////////////////////////////////////////////////////////
//	dynamic line
/*	2002/8/19 nt
 *	动态线段的创建,删除与初始化
 *	nt modify 2009/9/15 : 允许连续画线
 */
CDrawLin::CDrawLin(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_stage = 0 ;

	return ;
}

CDrawLin::~CDrawLin()
{
}
//--------------------------------------------------------------
int CDrawLin::GetID()
{
	return dctDrawLin ;
}

/*	2001/11/22 nt
 *	call when lbutton down
 */
int CDrawLin::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	// stage from 0 to 1
	Navigate(0, nFlags, p, v) ;
	if( m_stage == 0 )
	{
		GetNavP(m_p1) ;
		memcpy(m_p0, m_p1, sizeof(PNT3D)) ;
	}
	else
	if( m_stage == 1 )
	{
		if( m_drawed )
			Draw() ; // eraze
		GetNavP(m_p2) ;
	}
	m_stage++;

	return 1 ;
}

int CDrawLin::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	if( m_stage == 2 )
	{
		double r = 2.5*viGetScale(m_pVI) ;
		if( mathDist(m_p0, m_p2) < r ) // 基本为闭合的
			m_stage = 0 ;
		else
			m_stage = 1 ;

		// generate line
		VEC3D dir ;
		dir[0] = m_p2[0]-m_p1[0] ;
		dir[1] = m_p2[1]-m_p1[1] ;
		dir[2] = m_p2[2]-m_p1[2] ;
		double d = mathDist(m_p1, m_p2) ;
		if( d < r ) // if too near
			return 1 ;
		dir[0] /= d ;
		dir[1] /= d ;
		dir[2] /= d ;
		CURVE* line = lineCreate(m_p1, dir) ;
		if( !line )
			return 0 ;
		INTERVAL interval ;
		interval.t1 = 0.0 ;
		interval.t2 = d ;

		// generate refcur
		CRefCur* pRefCur = new CRefCur(0, line, &interval) ;
		m_pDoc->AddFeature(pRefCur) ;
		memcpy(m_p1, m_p2, sizeof(PNT3D)) ;
		m_drawed = 0 ;
	}

	return 1 ;
}

int CDrawLin::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	Navigate(0, nFlags, p, v) ;
	if( m_stage == 0 )
		GetNavP(m_p1) ;
	else	
	if( m_stage == 1 )
	{
		if( m_drawed )
			Draw() ; // eraze
		GetNavP(m_p2) ;
		Draw() ; // draw
	}

	return 1 ;
}

int CDrawLin::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( m_drawed ) // nt add 2009/9/15
		Draw() ;
	m_pDoc->ClearCmdAll() ; // nt add 2002/12/4

	return 1 ;
}

// nt add 2009/10/21
// 只处理4种情况:
// (1)L,
// (2)x,y
// (3)dx,dy
// (4)L,A
// nt modify 2009/11/4 : 处理局部坐标系
int CDrawLin::OnReturn()
{
	double d ;
	VEC3D v ;
	CStr str ;
	camPromptGet(str) ;
	camPromptClear() ;
	double L, x, y, a ;
	PNT2D planePt ;
	if( str.Find(',') >= 0 || // 不只一个参数
		str.Find(' ') >= 0 )
	{
		if( str.Find("A=") >= 0 || // L,a
			str.Find("a=") >= 0 )
		{
			if( m_stage == 1 &&
				strToNums(str, L, a) == 2 )
			{
				if( m_drawed )
					Draw() ;
				ToPlane(m_p1, planePt) ;
				planePt[0] += cos(a/180*PI1)*L ;
				planePt[1] += sin(a/180*PI1)*L ;
				ToWorld(planePt, m_p2) ;
				Draw() ;
				m_stage++ ;
			}
			else
				return 1 ;
		}
		else
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
				ToPlane(m_p1, planePt) ; // to local
				planePt[0] += x ; // nt add 2010/9/13
				planePt[1] += y ; // nt add 2010/9/13
				ToWorld(planePt, m_p2) ;
				Draw() ;
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
				planePt[0] = x ; // local point
				planePt[1] = y ; // local point
				ToWorld(planePt, m_p1) ;
				m_stage++ ;
				return 1 ;
			}
			else
			if( m_stage == 1 )
			{
				if( m_drawed )
					Draw() ;
				planePt[0] = x ; // local point
				planePt[1] = y ; // local point
				ToWorld(planePt, m_p2) ;
				Draw() ;
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
			strToNums(str, L) == 1 &&
			L > MIN_LEN )
		{
			v[0] = m_p2[0]-m_p1[0] ;
			v[1] = m_p2[1]-m_p1[1] ;
			v[2] = m_p2[2]-m_p1[2] ;
			d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
			if( d > MIN_LEN )
			{
				if( m_drawed )
					Draw() ;
				m_p2[0] = m_p1[0]+v[0]/d*L ;
				m_p2[1] = m_p1[1]+v[1]/d*L ;
				m_p2[2] = m_p1[2]+v[2]/d*L ;
				Draw() ;
				m_stage++ ;
			}
			else
				return 1 ;
		}
		else
			return 1 ;
	}
	OnLButtonUp(0, m_p2, NULL) ;

	return 1 ;
}

int CDrawLin::Cancel()
{
	return 1 ;
}

/*	2002/8/19
 *	画动态线段
 */
void CDrawLin::Draw()
{
	glColor3ub(255, 255, 255) ;
	DrawLin(m_p1, m_p2) ;
	m_drawed = m_drawed?0:1 ;

	return ;
}
///////////////////////////////////////////////////////////////
