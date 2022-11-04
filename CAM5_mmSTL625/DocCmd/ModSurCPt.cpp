/*	1997/4/6 nt
 *	implementation file CModSurCPt
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "ModSurCPt.h"

///////////////////////////////////////////////////////////////
//	dynamic modify NURBS surface control point
/*	2003/12/4 nt
 *	动态修改曲面控制顶点的创建,删除与初始化
 */
CModSurCPt::CModSurCPt(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
	m_pRefSur = NULL ;
	m_nurbs = NULL ;
	m_i = -1 ;
	m_j = -1 ;

	return ;
}

CModSurCPt::~CModSurCPt()
{
	// nt add 2010/9/1
	if( m_pRefSur )
	{
		m_pRefSur->Disc(0.5, 0.15) ;
		if( m_pDoc )
			m_pDoc->Redraw() ;
	}
}
//--------------------------------------------------------------
int CModSurCPt::Create(CRefSur* pRefSur)
{
	if( !m_pView ||
		!m_pDoc )
		return 0 ;
	m_pRefSur = pRefSur ;
	SURFACE* surface = pRefSur->GetSurface() ;
	m_nurbs = surfaceGetNurbs(surface) ;
	m_color = RGB(255, 0, 0) ;
	//Draw() ;
	return 1 ;
}

int CModSurCPt::GetID()
{
	return dctModSurCPt ;
}

/*	2003/12/4 nt
 *	call when lbutton down
 */
int CModSurCPt::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	// 注意不需要将p投影到当前工作平面上!!!!!!
	memcpy(m_p, p, sizeof(PNT3D)) ;

	// set lbutton state
	m_mouse = 1 ;

	//
	int i, j ;
	double d, min = MAX_DBL ;
	m_i = 0 ;
	m_j = 0 ;
	for( i = 0 ; i < m_nurbs->cnt_u ; i++ )
	{
		for( j = 0 ; j < m_nurbs->cnt_v ; j++ )
		{
			d = mathDistPntLin(m_nurbs->uv_p[i][j], p, v) ;
			if( min > d )
			{
				min = d ;
				m_i = i ;
				m_j = j ;
			}
		}
	}
	PNT3D pos ;
	bssurEvalNormal(m_nurbs,
					(double)m_i/(m_nurbs->cnt_u-1),
					(double)m_j/(m_nurbs->cnt_v-1),
					pos,
					m_normal) ;
	
	// draw control point grid
	Draw() ;

	return 1 ;
}

int CModSurCPt::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	m_mouse = 0 ;
	m_i = -1 ;
	m_j = -1 ;

	return 1 ;
}

int CModSurCPt::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	if( m_mouse != 1 )
		return 1 ;

	// eraze
	if( m_drawed )
		Draw() ;

	double d = mathDist(m_p, p) ;
	VEC3D X, Y, Z ;
	PNT3D O ;
	viGetCenterLF(m_pVI, X, Y, Z, O) ;
	if( (m_p[0]-p[0])*Y[0]+(m_p[1]-p[1])*Y[1]+(m_p[2]-p[2])*Y[2] < 0.0 )
		d = -d ;
	m_nurbs->uv_p[m_i][m_j][0] += m_normal[0]*d ;
	m_nurbs->uv_p[m_i][m_j][1] += m_normal[1]*d ;
	m_nurbs->uv_p[m_i][m_j][2] += m_normal[2]*d ;
	memcpy(m_p, p, sizeof(PNT3D)) ;

	// draw
	Draw() ;

	return 1 ;
}

int CModSurCPt::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	m_pDoc->ClearCmdAll() ;	
	return 1 ;
}

// nt add 2009/10/21
int CModSurCPt::OnReturn()
{
	return 1 ;
}

int CModSurCPt::Cancel()
{
	return 1 ;
}

/*	2003/12/4
 *	动态画
 */
void CModSurCPt::Draw()
{
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	glColor3ub(255, 0, 0) ;
	glLineWidth(1.5f) ;
	int i, j ;
	for( i = 0 ; i < m_nurbs->cnt_u ; i++ )
	{
		glBegin(GL_LINE_STRIP) ;
		for( j = 0 ; j < m_nurbs->cnt_v ; j++ )
			glVertex3dv(m_nurbs->uv_p[i][j]) ;
		glEnd() ;
	}
	for( j = 0 ; j < m_nurbs->cnt_v ; j++ )
	{
		glBegin(GL_LINE_STRIP) ;
		for( i = 0 ; i < m_nurbs->cnt_u ; i++ )
			glVertex3dv(m_nurbs->uv_p[i][j]) ;
		glEnd() ;
	}
	glDisable(GL_COLOR_LOGIC_OP) ;
	m_drawed = m_drawed?0:1 ;
	glLineWidth(1.f) ;
	glFlush() ;

	return ;
}
///////////////////////////////////////////////////////////////
