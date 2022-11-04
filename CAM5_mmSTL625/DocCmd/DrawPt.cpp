/*	2009/5/7 nt
 *	implementation file CDrawPt
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "DrawPt.h"

///////////////////////////////////////////////////////////////
//	dynamic CRefPt
/*	2009/5/7 nt
 *	动态绘制点
 */
CDrawPt::CDrawPt(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_n = 0 ;

	return ;
}

CDrawPt::~CDrawPt()
{
}
//--------------------------------------------------------------
int CDrawPt::GetID()
{
	return dctDrawPt ;
}

/*	2009/5/7 nt
 *	call when lbutton down
 */
int CDrawPt::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( Navigate(0, nFlags, p, v) )
	{
		GetNavP(m_p) ;
		m_n++ ;
	}

	return 1 ;
}

int CDrawPt::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	if( m_n > 0 )
	{
		CRefPt* pRefPt = new CRefPt(0, m_p) ;
		m_pDoc->AddFeature(pRefPt) ;
	}
	return 1 ;
}

int CDrawPt::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	Navigate(0, nFlags, p, v) ;

	return 1 ;
}

int CDrawPt::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	m_pDoc->ClearCmdAll() ;
	return 1 ;
}

// nt add 2009/10/21
int CDrawPt::OnReturn()
{
	CStr str ;
	camPromptGet(str) ;
	camPromptClear() ;
	PNT2D planePt ;
	double x, y ;
	if( strToNums(str, x, y) == 2 )
	{
		if( str.Find("dx=") >= 0 || // 作为增量处理
			str.Find("DX=") >= 0 ||
			str.Find("dy=") >= 0 ||
			str.Find("DY=") >= 0 )
		{
			if( m_n < 1 )
				return 1 ;
			ToPlane(m_p, planePt) ; // to workplane
			planePt[0] += x ; // local
			planePt[1] += y ; // local
			ToWorld(planePt, m_p) ;
		}
		else
		{
			planePt[0] = x ; // local x
			planePt[1] = y ; // local y
			ToWorld(planePt, m_p) ; // wo world
		}
		m_n++ ;
		OnLButtonUp(0, NULL, NULL) ;
	}

	return 1 ;
}

int CDrawPt::Cancel()
{
	return 1 ;
}
///////////////////////////////////////////////////////////////
