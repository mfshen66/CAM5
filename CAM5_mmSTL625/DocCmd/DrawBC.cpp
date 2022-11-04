/*	1997/4/6 nt
 *	implementation file CDrawBC
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "DrawBC.h"

///////////////////////////////////////////////////////////////
//	dynamic Bezier curve
/*	2009/5/7 nt
 *	动态绘Bezier curve
 */
CDrawBC::CDrawBC(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_mode = 0 ; // nt add 2011/8/15
	m_n = 0 ;

	return ;
}

CDrawBC::~CDrawBC()
{
}
//--------------------------------------------------------------
int CDrawBC::GetID()
{
	return dctDrawBC ;
}

/*	2009/5/7 nt
 *	call when lbutton down
 */
int CDrawBC::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	// project p along v to current workplane
	if( Navigate(0, nFlags, p, v) &&
		m_n < 64 )
	{
		GetNavP(m_ps[m_n]) ;
		m_n++ ;
		Draw() ;
	}

	return 1 ;
}

int CDrawBC::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

int CDrawBC::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	Navigate(0, nFlags, p, v) ;
	return 1 ;
}

int CDrawBC::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CDrawBC::OnReturn()
{
	CStr str ;
	camPromptGet(str) ;
	camPromptClear() ;
	PNT2D planePt ;
	double x, y ;
	if( m_n < 64 &&
		strToNums(str, x, y) == 2 )
	{
		if( str.Find("dx=") >= 0 || // 作为增量处理
			str.Find("DX=") >= 0 ||
			str.Find("dy=") >= 0 ||
			str.Find("DY=") >= 0 )
		{
			if( m_n < 1 )
				return 1 ;
			ToPlane(m_ps[m_n-1], planePt) ; // to plane
		}
		else
		{
			planePt[0] = 0. ;
			planePt[1] = 0. ;
		}
		planePt[0] += x ; // local
		planePt[1] += y ; // local
		ToWorld(planePt, m_ps[m_n]) ;
		m_n++ ;
	}
	else
	if( str.Find("c") >= 0 || // nt add 2011/8/15
		str.Find("C") >= 0 )
	{
		m_mode = 0 ; // control point
	}
	else
	if( str.Find("i") >= 0 || // nt add 2011/8/15
		str.Find("I") >= 0 )
	{
		m_mode = 1 ; // interpolating point
	}
	else
		OnRButtonDown(0, NULL, NULL) ;

	return 1 ;
}

int CDrawBC::Cancel()
{
	return 1 ;
}

int CDrawBC::Draw()
{
	if( m_n > 1 )
	{
		glLineWidth(1.f) ;
		glColor3ub(0, 0, 0) ;
		glBegin(GL_LINES) ;
			glVertex3dv(m_ps[m_n-2]) ;
			glVertex3dv(m_ps[m_n-1]) ;
		glEnd() ;
		glFlush() ;
	}

	return 1 ;
}
///////////////////////////////////////////////////////////////
