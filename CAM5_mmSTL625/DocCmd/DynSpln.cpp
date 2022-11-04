/*	1997/4/6 nt
 *	implementation file CDynSpln
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "DynSpln.h"

///////////////////////////////////////////////////////////////
//	dynamic b-spline curve
/*	1997/4/6 nt
 *	动态样条的创建,删除与初始化
 *  nt rewrite 2010/9/7
 */
CDynSpln::CDynSpln(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;

	m_r = 0.0 ;
	mathInitRFrame(&m_lf) ;
	m_dTol = 0.1 ;
	m_moved = 0 ;
	m_posi = -1 ;
	m_max = 0 ;
	m_np = 0 ;
    m_ps = NULL ;
	m_temp = NULL ; // nt add 2010/9/7
	m_nurbs = NULL ;
	m_na = 0 ;
	m_as = NULL ;

	return ;
}

CDynSpln::~CDynSpln()
{
	if( m_ps )
		delete []m_ps ;
	if( m_temp ) // nt add 2010/9/7
		delete []m_temp ;
	if( m_nurbs )
		bscurFree(m_nurbs) ;
	if( m_as )
		delete []m_as ;
}
//--------------------------------------------------------------
int CDynSpln::Create(double r, double dTol, int max)
{
	if( !m_pView ||
		!m_pDoc )
		return 0 ;
	CRefPlane* pWP = m_pDoc->GetWorkPlane() ;
	if( !pWP )
		return 0 ;
	pWP->GetLF(&m_lf) ; // nt add 2010/9/7

	m_r = r ;
	m_dTol = dTol ;
	m_moved = 0 ;
	m_posi = -1 ;
	m_max = max < 128 ? 128 : max ;
	m_np = 0 ;
	m_ps = new double[m_max][3] ;
	m_temp = new double[m_max][3] ; // nt add 2010/9/7
	m_nurbs = NULL ;
	m_na = 0 ;
	m_as = new double[m_max][3] ; ; // !!!存逼近点
	
	if( 	!m_ps ||
		!m_as )
		goto ExceExit ;

	return 1 ;
ExceExit:
	if( m_ps )
		delete []m_ps ;
	if( m_as )
		delete []m_as ;

	return 0 ;
}

int CDynSpln::GetID()
{
	return 0 ;
}

/*	2001/11/22 nt
 *	call when lbutton down
 */
int CDynSpln::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	// project p along v to current workplane
	PNT3D worldPt ;
	Navigate(0, nFlags, p, v) ;
	GetNavP(worldPt) ;

	// to find an near interpolating point from the world point worldPt
	int posi = Near(worldPt) ;

	// add a new or drag old
	if( m_np == 1 )
		return 0 ; // impossible
	else
	if( posi < 0 ) // when empty, or need draw new curve
	{
		End() ;
		Start() ;
		memcpy(m_ps[0], worldPt, sizeof(PNT3D)) ; // initialize first intp point
		memcpy(m_ps[1], worldPt, sizeof(PNT3D)) ; // initialize second intp point
		m_np = 2 ;
		m_posi = 1 ;
	}
	else
		Append(posi) ;

	// set lbutton state
	m_mouse = 1 ;

	// set mouse moved
	m_moved = 0 ;

	return 1 ;
}

int CDynSpln::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	// special case : lbutton down and immediately up
	if( m_mouse == 1 &&
		m_moved == 0 )
		RemoveDoublePt() ;

	// set lbutton state
	m_mouse = 0 ;

	// set drag position
	m_posi = -1 ;

	// set mouse moved
	m_moved = 0 ;

	return 1 ;
}

int CDynSpln::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	// world pt
	PNT3D worldPt ;
	Navigate(0, nFlags, p, v) ;
	GetNavP(worldPt) ;

	// check
	if( m_mouse == 0 || // lbutton up
		m_posi == -1 ||
		m_np < 2 )
		return 1 ;

	// eraze
	Draw(0) ;

	// modify
	Modify(worldPt) ;

	// draw
	Draw(1) ;

	// set mouse moved
	m_moved = 1 ;

	return 1 ;
}

// nt modify 2011/6/12
int CDynSpln::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	//if( End() != 1 )
	//	return 0 ;
	//Start() ;
	// nt add 2011/6/12, 实现按右键时终止绘图命令
	if( m_drawed )
		Draw(0) ;
	End() ;
	m_pDoc->ClearCmdAll() ;

	return 1 ;
}

// nt add 2009/10/21
int CDynSpln::OnReturn()
{
	return 1 ;
}

int CDynSpln::Cancel()
{
	if( End() != 1 )
		return 0 ;
	Start() ;

	return 1 ;
}

int CDynSpln::Start()
{
	if( 	!m_ps ||
		!m_as )
		return 0 ;

	m_drawed = 0 ;
	m_moved = 0 ;
	m_posi = -1 ;
	m_np = 0 ;
	m_na = 0 ;
	if( m_nurbs )
	{
		bscurFree(m_nurbs) ;
		m_nurbs = NULL ;
	}

	return 1 ;
}

/*	2001/11/22 nt
 *	结束曲线绘制,生成曲线
 *	generate b-spline curve and save to the doc model if possible
 */
int CDynSpln::End()
{
	// check state
	if( !m_pDoc )
		return 0 ;
	
	// generate nurbs
	if( m_nurbs )
	{
		CRefCur* pRefCur = new CRefCur(rcThrPts, m_nurbs) ;
		m_pDoc->AddFeature(pRefCur) ;
		bscurFree(m_nurbs) ;
		m_nurbs = NULL ;
	}
	
	return 1 ;
}
/*-----------------------------------------------------------*/
/*	1997/4/6 nt
 *	画动态样条
 *	nt modify 2010-1-29 : use opengl
 */
void CDynSpln::Draw(int mode)
{
	// invalid input
	if( mode != 0 &&
		mode != 1 )
		return ;

	// not need to draw or eraze
    if( mode == m_drawed )
		return ;

	// draw interpolating point
	void* pVI = m_pView->GetVI() ;
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	int i ;
	
	// draw approximating polyline of the dynamic b-spline curve
	if( m_na > 1 )
	{
		glColor3ub(255, 255, 255) ;
		glLineWidth(1.f) ;
		glBegin(GL_LINE_STRIP) ;
			for( i = 0 ; i < m_na ; i++ )
				glVertex3dv(m_as[i]) ;
		glEnd() ;
	}

	glColor3ub(255, 0, 0) ;
	glPointSize(2.5f) ;
	int periodic = 0, x1 = -100000000, y1 = -100000000, x2, y2 ;
	if( IsPeriodic() )
		periodic = 1 ;
	glBegin(GL_POINTS) ;
		for( i = 0 ; i < m_np-periodic ; i++ )
		{
			viGetScreenCoord(pVI, m_ps[i], &x2, &y2) ;
			if( x1 != x2 ||
				y1 != y2 )
				glVertex3dv(m_ps[i]) ;
			x1 = x2 ;
			y1 = y2 ;
		}
	glEnd() ;

	glDisable(GL_COLOR_LOGIC_OP) ;
	glFlush() ;

	// change draw state: m_drawed
    m_drawed = mode ;

	return ;
}

/*	1997/4/6 nt
 *	(当鼠标左键按下)根据当前状态,对动态样条类进行初始化
 */
int CDynSpln::Append(int posi)
{
	if( (m_posi = posi) < 0 ) // no need to add new interpolating p
		return 1 ;

	// check if drag old point
	if( m_posi > 0 && // drag old point
		m_posi < m_np-1 )
		return 1 ;

	// it can not add new interpolating point to a periodic curve
	if(	IsPeriodic() )
		return 1 ; // in this case : m_posi == 0 or m_np-1

	// check max
	if( m_np >= m_max )
	{
		m_posi = -1 ;
		return 1 ;
	}

	// check if add new interpolating point at the head or the tail
	if( m_posi == m_np-1 )
	{
		memcpy(m_ps[m_np], m_ps[m_np-1], sizeof(PNT3D)) ; // add at tail
		m_posi++ ;
		m_np++ ;
	}
	else
	if( m_posi == 0 )
	{
		for( int i = m_np-1 ; i >= 0 ; i-- ) // add at head
			memcpy(m_ps[i+1], m_ps[i], sizeof(PNT3D)) ; 
		m_np++ ;
	}
	else
		m_posi = -1 ;

	return 1 ;
}

/*	2001/11/23 nt
 *	remove the double interpolating point at the head or the tail
 */
int CDynSpln::RemoveDoublePt()
{
	int i, posi ;

	if( m_np < 2 )
		return 1 ;

	if( mathDist(m_ps[0], m_ps[1]) < MIN_LEN )
		posi = 0 ;
	else
	if( mathDist(m_ps[m_np-2], m_ps[m_np-1]) < MIN_LEN )
		posi = m_np-1 ;
	else
		posi = -1 ;

	if( posi == -1 )
		return 1 ;

	for( i = posi+1 ; i < m_np ; i++ )
		memcpy(m_ps[i-1], m_ps[i], sizeof(PNT3D)) ;
	m_np-- ;

	if( m_np <= 1 )
		m_np = 0 ;

	if( m_np == 0 &&
		m_nurbs )
	{
		bscurFree(m_nurbs) ;
		m_nurbs = NULL ;
	}

	if( m_np == 0 )
		m_na = 0 ;

	m_posi = -1 ;

	return 1 ;
}

/*	1997/4/6 nt
 *	(当鼠标左键按下并移动鼠标)对动态样条进行更新
 */
int CDynSpln::Modify(PNT3D p)
{
	// check
	if( m_posi < 0 ||
		m_posi > m_np-1 )
		return 1 ;
	double d = mathDist(m_ps[m_posi], p) ; // d:the move distance
	if( d < 0.01*m_r )
		return 1 ;
	int periodic = IsPeriodic() ; // before change value of m_ps[]
	m_ps[m_posi][0] = p[0] ;
	m_ps[m_posi][1] = p[1] ;
	m_ps[m_posi][2] = p[2] ;

	// periodic
	if( m_np > 3 &&	
		periodic )
	{
		if( m_posi == 0 ) // move head
			memcpy(m_ps[m_np-1], m_ps[0], sizeof(PNT3D)) ;
		else // move tail
			memcpy(m_ps[0], m_ps[m_np-1], sizeof(PNT3D)) ;
	}

	// interpolate and discrete
	if( 	IntpPsTs() != 1 ||
		Disc() != 1 )
		return 0 ;
	else
		return 1 ;
}
/*-----------------------------------------------------------*/
/*	2001/11/22 nt
 *	calculate near point index
 */
int CDynSpln::Near(PNT3D p)
{
	for( int i = 0 ; i < m_np ; i++ )
	{
		if( mathDist(m_ps[i], p) < m_r )
			return i ;
	}

	return -1 ;
}

/*	2001/11/22 nt
 *	discrete
 */
int CDynSpln::Disc()
{
	if( m_np < 1 ) // there are no valid interpolating points exist
		m_na = 0 ;
	else
	if( m_np < 2 ) // only one interpolating point
	{
		m_na = 1 ;
		memcpy(m_as[0], m_ps[0], sizeof(PNT3D)) ;
	}
	else // there are at least two interpolating points exist
	{
		double step ;
		if( bscurGetMaxiStepLin(m_nurbs, m_dTol, &step) != ERSUCSS )
            return 0 ;
		int n = (int)ceil(1.0/step) ;
		if( n < 1 )
			n = 1 ;
		if( n > m_max-1 )
			n = m_max-1 ;
		for( int i = 0 ; i <= n ; i++ )
			bscurEvalPos(m_nurbs, (double)i/n, m_as[i]) ;
		m_na = n+1 ;
	}

	return 1 ;
}

/*	2001/11/22 nt
 *	is periodic cure
 */
int CDynSpln::IsPeriodic()
{
	if( m_np > 2 &&
		mathDist(m_ps[0], m_ps[m_np-1]) < m_r*0.5 )
		return 1 ;
	else
		return 0 ;
}

/*	2010/9/7
 *  估计p1,p2,p3在p2处的切矢
 */
int tvecEstmAtMid(PNT3D p1, 
				  PNT3D p2, 
				  PNT3D p3, 
				  VEC3D tvec)
{
	double d1, d2, t ;
	PNT3D p ;

	if( !p1 ||
		!p2 ||
		!p3 ||
		!tvec )
		return ERUNSUC ;

	d1 = mathDist(p1, p2) ;
	d2 = mathDist(p2, p3) ;
	if( d1 < MIN_LEN &&
		d2 < MIN_LEN )
		return ERUNSUC ;
	if( d1 < MIN_LEN )
		mathGetVec(p1, p3, tvec) ;
	else
	if( d2 < MIN_LEN )
		mathGetVec(p1, p2, tvec) ;
	else
	{
		t = d1/(d1+d2) ;
		p[0] = p2[0]-(1.0-t)*(1.0-t)*p1[0]-t*t*p3[0] ;
		p[1] = p2[1]-(1.0-t)*(1.0-t)*p1[1]-t*t*p3[1] ;
		p[2] = p2[2]-(1.0-t)*(1.0-t)*p1[2]-t*t*p3[2] ;
		t = 2.0*(1.0-t)*t ;
		p[0] /= t ;
		p[1] /= t ;
		p[2] /= t ;
		tvec[0] = t*(p1[0]-2.*p[0]+p3[0])+p[0]-p1[0] ;
		tvec[1] = t*(p1[1]-2.*p[1]+p3[1])+p[1]-p1[1] ;
		tvec[2] = t*(p1[2]-2.*p[2]+p3[2])+p[2]-p1[2] ;
	}
	if( mathUniVec(tvec, MIN_DBL) != ERSUCSS )
		return ERUNSUC ;

	return ERSUCSS ;
}

/*	1997/4/6 nt
 *	从新插值动态样条
 */
int CDynSpln::IntpPsTs()
{    
    return 0 ;
}
///////////////////////////////////////////////////////////////
