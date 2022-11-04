/*	2000/5/13 nt
 *	implementation file CZoomBox
 */
#include "StdAfx.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "ZoomBox.h"

///////////////////////////////////////////////////////////////
//	zoom by window
/*	2004/11/5 nt
 *	用窗口放大视区
 *	nt modify 2005/3/31
 */
CZoomBox::CZoomBox(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_p1[0] = -1.0e10 ;
	m_p1[1] = -1.0e10 ;
	m_p1[2] = -1.0e10 ;
	m_p2[0] = -1.0e10 ;
	m_p2[1] = -1.0e10 ;
	m_p2[2] = -1.0e10 ;
	m_drawed = 0 ;

	return ;
}

CZoomBox::~CZoomBox()
{
}
//--------------------------------------------------------------
int CZoomBox::GetID()
{
	return vctZoomBox ;
}

/*
 *
 */
int CZoomBox::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 1 ;
	m_p1[0] = p[0] ;
	m_p1[1] = p[1] ;
	m_p1[2] = p[2] ;
	m_p2[0] = p[0] ;
	m_p2[1] = p[1] ;
	m_p2[2] = p[2] ;
	m_drawed = 0 ;

	return 1 ;
}

int CZoomBox::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 0 ;
	if( m_drawed == 1 )
		Draw() ;
	double d = sqrt((m_p2[0]-m_p1[0])*(m_p2[0]-m_p1[0])+
					(m_p2[1]-m_p1[1])*(m_p2[1]-m_p1[1])+
					(m_p2[2]-m_p1[2])*(m_p2[2]-m_p1[2])) ;
	if( d < 5.0*viGetScale(m_pVI) )
		return 1 ;
	viZoomByBox(m_pVI, m_p1, m_p2) ;
	m_pView->Invalidate() ;
	return 1 ;
}

int CZoomBox::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	if( m_mouse )
	{
		if( m_drawed == 1 )
			Draw() ;
		m_p2[0] = p[0] ;
		m_p2[1] = p[1] ;
		m_p2[2] = p[2] ;
		Draw() ;
	}

	return 1 ;
}

int CZoomBox::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

// nt add 2009/10/21
int CZoomBox::OnReturn()
{
	return 1 ;
}

int CZoomBox::Cancel()
{
	return 1 ;
}

int CZoomBox::Draw()
{
	glColor3ub(255, 255, 255) ;
	DrawRect(m_p1, m_p2) ;
	m_drawed = m_drawed?0:1 ;
	return 1 ;
}
///////////////////////////////////////////////////////////////
