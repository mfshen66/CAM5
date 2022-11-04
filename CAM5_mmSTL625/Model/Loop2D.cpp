/*	2004/9/7 nt
 *	implementation file of class CLoop2D
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "RefCur.h"
#include "PLine2D.h"
#include "Loop2D.h"

////////////////////////////////////////////////////////////////
/*	2004/9/7 nt
 *	CLoop2D
 */
CLoop2D::CLoop2D()
{
	m_nLoop = 0 ;
	m_nPLps = NULL ;
	m_loops = NULL ;
	m_infos = NULL ;
}

CLoop2D::~CLoop2D()
{
	int i ;

	if( m_nPLps )
		delete []m_nPLps ;
	if( m_loops )
		loop2dsFree(m_nLoop, m_loops) ;
	if( m_infos )
	{
		for( i = 0 ; i < m_nLoop ; i++ )
		{
			if( m_infos[i] )
				free(m_infos[i]) ;
		}
		free(m_infos) ;
	}
}
//--------------------------------------------------------------
/*	2004/9/7 nt
 *	create a loops from curves
 */
typedef double (*NETPT)[2] ;
int CLoop2D::Create(COA& refCurs, double tol, double ang, double chd)
{
	int i, k, n, *flags ;
	double (*temp)[2] ;
	CPLine2D* pPLine ;
	COA loops ;

	// free old
	if( m_loops )
		loop2dsFree(m_nLoop, m_loops) ;
	m_nLoop = 0 ;
	m_loops = NULL ;
	if( m_nPLps )
		delete []m_nPLps ;
	m_nPLps = NULL ;

	// extract loops
	while(refCurs.GetSize() > 0 )
	{
		pPLine = new CPLine2D() ;
		pPLine->Create(refCurs, tol) ;
		loops.Add(pPLine) ;
	}

	// check valid
	for( i = 0 , n = loops.GetSize() ; i < n ; i++ )
	{
		pPLine = (CPLine2D*)loops[i] ;
		if( pPLine->IsClosed(tol) == FALSE )
			goto ExceExit ;
	}
	// intersection check ???
	// ...

	// discrete
	m_nLoop = n ;
	m_nPLps = new int[m_nLoop] ;
	for( i = 0 ; i < m_nLoop ; i++ )
	{
		pPLine = (CPLine2D*)loops[i] ;
		if( pPLine->Disc(tol, ang, chd) != 1 )
			goto ExceExit ;
		m_nPLps[i] = pPLine->GetNumOfDiscPt()-1 ;
	}
	m_loops = loop2dsCreate(m_nLoop, m_nPLps) ;
	for( i = 0 ; i < m_nLoop ; i++ )
	{
		pPLine = (CPLine2D*)loops[i] ;
		for( k = 0 ; k < m_nPLps[i] ; k++ )
			pPLine->GetDiscPt(k, m_loops[i][k]) ;
	}

	// free CPLines
	for( i = 0 , n = loops.GetSize() ; i < n ; i++ )
	{
		pPLine = (CPLine2D*)loops[i] ;
		delete pPLine ;
	}
	loops.RemoveAll() ;

	// classify loops
	flags = new int[m_nLoop] ;
	if( loop2dClassify(m_nLoop,
					   m_nPLps,
					   m_loops,
					   0.01*tol,
					   flags) != 1 )
	{
		delete []flags ;
		goto ExceExit ;
	}
	for( i = 0, k = 0 ; i < m_nLoop ; i++ )
	{
		if( flags[i] == 0 )
			k++ ;
	}

	//
	if( k != 1 )
	{
		delete []flags ;
		goto ExceExit ;
	}
	if( flags[0] != 0 )
	{
		temp = m_loops[0] ;
		for( i = 1 ; i < m_nLoop ; i++ )
		{
			if( flags[i] == 0 )
			{
				m_loops[0] = m_loops[i] ;
				m_loops[i] = temp ;
			}
		}
	}
	delete []flags ;

	return 1 ;
ExceExit:
	for( i = 0 , n = loops.GetSize() ; i < n ; i++ )
	{
		pPLine = (CPLine2D*)loops[i] ;
		delete pPLine ;
	}
	loops.RemoveAll() ;

	return 0 ;

}

/*	2004/9/9 nt
 *
 */
int CLoop2D::CalBndDirs()
{
	int i, k, k_plus_one ;
	double dx, dy, d ;

	if( m_infos )
	{
		for( i = 0 ; i < m_nLoop ; i++ )
		{
			if( m_infos[i] )
				free(m_infos[i]) ;
		}
		free(m_infos) ;
	}
	m_infos = (double(**)[3])malloc(sizeof(double(*)[3])*m_nLoop) ;
	for( i = 0 ; i < m_nLoop ; i++ )
	{
		m_infos[i] = (double(*)[3])malloc(sizeof(double[3])*m_nPLps[i]) ;
		for( k = 0 ; k < m_nPLps[i] ; k++ )
		{
			k_plus_one = k == m_nPLps[i]-1 ? 0 : k+1 ;
			dx = m_loops[i][k_plus_one][0]-m_loops[i][k][0] ;
			dy = m_loops[i][k_plus_one][1]-m_loops[i][k][1] ;
			d = sqrt(dx*dx+dy*dy) ;
			if( d < MIN_DBL )
				return 0 ;
			m_infos[i][k][0] = dx/d ;
			m_infos[i][k][1] = dy/d ;
			m_infos[i][k][2] = d ;
		}
	}

	return 1 ;
}

/*	2004/9/8 nt
 *	计算点到环的最近距离,在边界上为0,在内部为正,在外部为负.直接利用点到直线的距
 *	离求解
 */
double CLoop2D::CalNearestDis(PNT2D p)
{
	int i, k, k_plus_one ;
	double min1 = MAX_DBL, min2 = MAX_DBL, d ;
	VEC2D v ;

	for( i = 0 ; i < m_nLoop ; i++ )
	{
		for( k = 0 ; k < m_nPLps[i] ; k++ )
		{
			k_plus_one = k == m_nPLps[i]-1 ? 0 : k+1 ;
			v[0] = p[0]-m_loops[i][k][0] ;
			v[1] = p[1]-m_loops[i][k][1] ;
			d = m_infos[i][k][0]*v[0]+m_infos[i][k][1]*v[1] ;
			if( d < 0.0 )
			{
				d = v[0]*v[0]+v[1]*v[1] ;
				if( min2 > d )
					min2 = d ;
			}
			else
			if( d > m_infos[i][k][2] )
			{
				v[0] = p[0]-m_loops[i][k_plus_one][0] ;
				v[1] = p[1]-m_loops[i][k_plus_one][1] ;
				d = v[0]*v[0]+v[1]*v[1] ;
				if( min2 > d )
					min2 = d ;
			}
			else
			{
				d = fabs(m_infos[i][k][1]*v[0]-m_infos[i][k][0]*v[1]) ;
				if( min1 > d )
					min1 = d ;
			}
		}
	}

	min2 = sqrt(min2) ;
	if( min1 > min2 )
		min1 = min2 ;

	return min1 ;
}
////////////////////////////////////////////////////////////////
