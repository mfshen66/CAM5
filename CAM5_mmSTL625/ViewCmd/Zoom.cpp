/*	2000/5/13 nt
 *	implementation file CZoom
 */
#include "StdAfx.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "Zoom.h"

///////////////////////////////////////////////////////////////
/*	1999/4/8 nt
 *	CZoom
 */
CZoom::CZoom(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_p[0] = 0.0 ;
	m_p[1] = 0.0 ;
	m_p[2] = 0.0 ;
	m_scale = 1.0 ;
}

CZoom::~CZoom()
{
}
//--------------------------------------------------------------
int CZoom::GetID()
{
	return vctZoom ;
}

int CZoom::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	m_p[0] = p[0] ;
	m_p[1] = p[1] ;
	m_p[2] = p[2] ;
	m_mouse = 1 ;
	m_scale = 1.0 ;
	return 1 ;
}

int CZoom::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 0 ;
	return 1 ;
}

int CZoom::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	if( m_mouse )
	{
		double w, h, d, zm_scale ;
		double eye[3], center[3], up[3], vec[3] ;
		viGetEyeCenterUp(m_pVI, eye, center, up) ;
		vec[0] = p[0]-m_p[0] ;
		vec[1] = p[1]-m_p[1] ;
		vec[2] = p[2]-m_p[2] ;
		d = vec[0]*up[0]+vec[1]*up[1]+vec[2]*up[2] ;
		viGetViewWidthHeight(m_pVI, &w, &h) ;
		h = fabs(h) ;
		if( d < -h )
			d = -h ;
		if( d >  h )
			d =  h ;
		if( h < 1.0e-20 )
			return 1 ;
		d /= h ;
		zm_scale = 49.0*d*d+1.0 ;
		if( d < 0.0 )
			zm_scale = 1.0 / zm_scale ;
		if( fabs(zm_scale-1.0) < 0.01 ||
			m_scale < 1.0e-10 ||
			m_scale > 1.0e+10 )
			return 1 ;
		if( viZoomByMouse(m_pVI, zm_scale/m_scale, m_p) != 1 )
		{
			camPromptStr("Can't zoom any more!") ;
			return 0 ;
		}
		m_scale = zm_scale ;
		m_pView->Invalidate(FALSE) ;
	}

	return 1 ;
}

int CZoom::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CZoom::OnReturn()
{
	return 1 ;
}

int CZoom::Cancel()
{
	return 1 ;
}
///////////////////////////////////////////////////////////////
