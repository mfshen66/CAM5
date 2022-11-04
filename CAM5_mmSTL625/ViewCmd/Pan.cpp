/*	2000/5/13 nt
 *	implementation file CPan
 */
#include "StdAfx.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "Pan.h"

///////////////////////////////////////////////////////////////
/*	1999/4/8 nt
 *	CPan
 */
CPan::CPan(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_p[0] = 0.0 ;
	m_p[1] = 0.0 ;
	m_p[2] = 0.0 ;
}

CPan::~CPan()
{
}
//--------------------------------------------------------------
int CPan::GetID()
{
	return vctPan ;
}

int CPan::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	m_p[0] = p[0] ;
	m_p[1] = p[1] ;
	m_p[2] = p[2] ;
	m_mouse = 1 ;
	return 1 ;
}

int CPan::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 0 ;
	return 1 ;
}

int CPan::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	if( m_mouse ||
		nFlags&MK_MBUTTON ) // nt modify 2010/9/14
	{
		double vec[3] ;
		vec[0] = p[0]-m_p[0] ;
		vec[1] = p[1]-m_p[1] ;
		vec[2] = p[2]-m_p[2] ;
		viPan(m_pVI, vec) ;
		m_pView->Invalidate(FALSE) ;
	}
	return 1 ;
}

int CPan::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CPan::OnReturn()
{
	return 1 ;
}

int CPan::Cancel()
{
	return 1 ;
}
///////////////////////////////////////////////////////////////
