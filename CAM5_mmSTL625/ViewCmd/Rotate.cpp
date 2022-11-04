/*	2000/5/13 nt
 *	implementation file CRotate
 */
#include "StdAfx.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "Rotate.h"

///////////////////////////////////////////////////////////////
/*	1999/4/8 nt
 *	CRotate
 */
CRotate::CRotate(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_p[0] = 0.0 ;
	m_p[1] = 0.0 ;
	m_p[2] = 0.0 ;
	m_center[0] = 0.0 ; // nt add 2016/3/20
	m_center[1] = 0.0 ; // nt add 2016/3/20
	m_center[2] = 0.0 ; // nt add 2016/3/20
}

CRotate::~CRotate()
{
}
//--------------------------------------------------------------
int CRotate::GetID()
{
	return vctRotate ;
}

int CRotate::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( m_pDoc == NULL || // nt modify 2021/6/24
		m_pDoc->GetGPP(m_center) == 0 )
	{
		memcpy(m_p, p, sizeof(PNT3D)) ;
		// nt modify 2016/3/20
		double min[3], max[3], cp[3] ;
		m_pDoc->GetBox(min, max) ;
		cp[0] = 0.5*(min[0]+max[0]) ;
		cp[1] = 0.5*(min[1]+max[1]) ;
		cp[2] = 0.5*(min[2]+max[2]) ;
		mathPrjPntLin(cp, p, v, m_center) ;
		// end
	}
	
	m_mouse = 1 ;

	return 1 ;
}

int CRotate::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 0 ;
	return 1 ;
}

int CRotate::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	if( m_mouse ||
		nFlags&MK_MBUTTON ) // nt modify 2010/9/14
	{
		double d, w, h, angle ;
		double viewdir[3], raxis[3], vec[3] ;
		vec[0] = p[0]-m_p[0] ;
		vec[1] = p[1]-m_p[1] ;
		vec[2] = p[2]-m_p[2] ;
		viGetViewWidthHeight(m_pVI, &w, &h) ;
		d = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]) ;
		if( d < 0.0001*(w+h) )
			return 1 ;
		angle = 32*d/(w+h) ;
		if( angle < 0.02 )
			angle = 0.02 ;
		if( angle > 0.2  )
			angle = 0.2 ;

		viGetViewDir(m_pVI, viewdir) ;
		viVProduct(viewdir, vec, raxis) ;
		if( !viUniVec(raxis, 1.0e-10) )
			return 1 ;
		m_p[0] = p[0] ;
		m_p[1] = p[1] ;
		m_p[2] = p[2] ;
		//viRotate(m_pVI, raxis, angle, m_p) ;
		viRotate(m_pVI, raxis, angle, m_center) ;
		mathRotPnt(raxis, m_center, angle, m_p, m_p) ;
		m_pView->Invalidate(FALSE) ;
	}

	return 1 ;
}

int CRotate::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CRotate::OnReturn()
{
	return 1 ;
}

int CRotate::Cancel()
{
	return 1 ;
}
///////////////////////////////////////////////////////////////
