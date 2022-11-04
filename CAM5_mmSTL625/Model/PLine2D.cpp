/*	2002/10/5 nt
 *	implementation file of class CPLine2D
 *	nt modified 2004/9/7 for 3d applications
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "RefCur.h"
#include "PLine2D.h"

////////////////////////////////////////////////////////////////
/*	2002/10/5 nt
 *	PLine2D
 */
CPLine2D::CPLine2D()
{
	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;

	m_np = 0 ;
	m_ps = NULL ;
}

CPLine2D::~CPLine2D()
{
	m_dirFlags.RemoveAll() ;
	m_refCurs.RemoveAll() ;
	if( m_ps )
		delete []m_ps ;
}
//--------------------------------------------------------------
int	CPLine2D::Create(COA& refCurs, double tol)
{
	int i, n ;
    PNT3D p11, p12, p21, p22 ;
    CRefCur* pRefCur ;

	if( refCurs.GetSize() < 1 )
		return 0 ;

	pRefCur = (CRefCur*)refCurs[0] ;
	refCurs.RemoveAt(0) ;
	m_refCurs.Add(pRefCur) ;
	m_dirFlags.Add(1) ;
	pRefCur->GetBegin(p11) ;
	pRefCur->GetEnd(p12) ;
	while(1)
	{
		n = refCurs.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			pRefCur = (CRefCur*)refCurs[i] ;
			pRefCur->GetBegin(p21) ;
			pRefCur->GetEnd(p22) ;
			if( mathDist2D(p21, p11) < tol )
			{
				refCurs.RemoveAt(i) ;
				m_dirFlags.InsertAt(0, -1) ;
				m_refCurs.InsertAt(0, pRefCur) ;
				memcpy(p11, p22, sizeof(PNT2D)) ;
			}
			else
			if( mathDist2D(p22, p11) < tol )
			{
				refCurs.RemoveAt(i) ;
				m_dirFlags.InsertAt(0, 1) ;
				m_refCurs.InsertAt(0, pRefCur) ;
				memcpy(p11, p21, sizeof(PNT2D)) ;
			}
			else
			if( mathDist2D(p21, p12) < tol )
			{
				refCurs.RemoveAt(i) ;
				m_dirFlags.Add(1) ;
				m_refCurs.Add(pRefCur) ;
				memcpy(p12, p22, sizeof(PNT2D)) ;
			}
			else
			if( mathDist2D(p22, p12) < tol )
			{
				refCurs.RemoveAt(i) ;
				m_dirFlags.Add(-1) ;
				m_refCurs.Add(pRefCur) ;
				memcpy(p12, p21, sizeof(PNT2D)) ;
			}
			if( refCurs.GetSize() < n )
				break ;
		}
		if( refCurs.GetSize() >= n )
			break ;
    }

    return 1 ;
}

int	CPLine2D::GetNumOfCur()
{
	return m_refCurs.GetSize() ;
}

void CPLine2D::GetBegin(PNT2D p)
{
	int n = m_refCurs.GetSize() ;
	PNT3D temp ;

	if( n > 0 )
	{
		INTERVAL interval ;
		CRefCur* pRefCur = (CRefCur*)m_refCurs[0] ;
		pRefCur->GetInterval(&interval) ;
		double t = m_dirFlags[0] == 1 ? interval.t1 : interval.t2 ;
		CURVE* curve = pRefCur->GetCurve() ;
		curveEvalPos(curve, t, temp) ;
		p[0] = temp[0] ;
		p[1] = temp[1] ;
	}
	else
	{
		p[0] = -MAX_DBL ;
		p[1] = -MAX_DBL ;
	}
}

void CPLine2D::GetEnd(PNT2D p)
{
	int n = m_refCurs.GetSize() ;
	PNT3D temp ;

	if( n > 0 )
	{
		INTERVAL interval ;
		CRefCur* pRefCur = (CRefCur*)m_refCurs[n-1] ;
		pRefCur->GetInterval(&interval) ;
		double t = m_dirFlags[n-1] != 1 ? interval.t1 : interval.t2 ;
		CURVE* curve = pRefCur->GetCurve() ;
		curveEvalPos(curve, t, temp) ;
		p[0] = temp[0] ;
		p[1] = temp[1] ;
	}
	else
	{
		p[0] = MAX_DBL ;
		p[1] = MAX_DBL ;
	}
}

BOOL CPLine2D::IsClosed(double tol)
{
	PNT2D p1, p2 ;

	GetBegin(p1) ;
	GetEnd(p2) ;
	if( mathDist2D(p1, p2) < tol )
		return TRUE ;
	else
		return FALSE ;
}

int	CPLine2D::Disc(double tol, double ang, double chd)
{
	int i, j, k, n ;
	CRefCur* pRefCur ;
	CURVE* curve ;
	INTERVAL interval ;
	STRPT* strpt = NULL ;
	CCarrier* pCarrier ;
	COA carriers ;

	if( m_refCurs.GetSize() < 1 )
		return 0 ;

	for( i = 0, n = m_refCurs.GetSize() ; i < n ; i++ )
	{
		pRefCur = (CRefCur*)m_refCurs[i] ;
		curve = pRefCur->GetCurve() ;
		pRefCur->GetInterval(&interval) ;
		if( curveDisc(curve, 
				      &interval, 
					  tol,
					  ang, 
					  chd, 
					  STRPT_PT, 
					  &strpt) != ERSUCSS )
			goto ExceExit ;
		pCarrier = new CCarrier() ;
		pCarrier->SetVP(strpt) ;
		carriers.Add(pCarrier) ;
	}

	m_np = 0 ;
	for( i = 0 ; i < n ; i++ )
	{
		pCarrier = (CCarrier*)carriers[i] ;
		strpt = (STRPT*)pCarrier->GetVP() ;
		m_np += (strpt->np-1) ;
	}
	m_np++ ;
	m_ps = new PNT2D[m_np] ;
	for( i = 0, k = 0 ; i < n ; i++ )
	{
		pCarrier = (CCarrier*)carriers[i] ;
		strpt = (STRPT*)pCarrier->GetVP() ;
		if( m_dirFlags[i] == 1 )
		{
			for( j = 0 ; j < strpt->np-1 ; j++, k++ )
			{
				m_ps[k][0] = strpt->ps[j][0] ;
				m_ps[k][1] = strpt->ps[j][1] ;
			}
			if( i == n-1 )
			{
				m_ps[k][0] = strpt->ps[strpt->np-1][0] ;
				m_ps[k][1] = strpt->ps[strpt->np-1][1] ;
				k++ ;
			}
		}
		else
		{
			for( j = strpt->np-1 ; j > 0 ; j--, k++ )
			{
				m_ps[k][0] = strpt->ps[j][0] ;
				m_ps[k][1] = strpt->ps[j][1] ;
			}
			if( i == n-1 )
			{
				m_ps[k][0] = strpt->ps[0][0] ;
				m_ps[k][1] = strpt->ps[0][1] ;
				k++ ;
			}
		}
		strptFree(strpt) ;
		pCarrier->SetVP(NULL) ;
		delete pCarrier ;
	}
	carriers.RemoveAll() ;

	return 1 ;
ExceExit:
	for( i = 0, n = carriers.GetSize() ; i < n ; i++ )
	{
		pCarrier = (CCarrier*)carriers[i] ;
		strpt = (STRPT*)pCarrier->GetVP() ;
		strptFree(strpt) ;
		pCarrier->SetVP(NULL) ;
		delete pCarrier ;
	}
	carriers.RemoveAll() ;

	return 0 ;
}

int	CPLine2D::GetNumOfDiscPt()
{
	return m_np ;
}

void CPLine2D::GetDiscPt(int index, PNT2D p)
{
	if( p &&
		m_ps )
		memcpy(p, m_ps[index], sizeof(PNT2D)) ;
}

void CPLine2D::GetBox(BOX2D* box)
{
	if( box )
		*box = m_box ;
}

int	CPLine2D::CalBox()
{
	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	for( int i = 0, n = m_refCurs.GetSize() ; i < n ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)m_refCurs[i] ;
		BOX3D box ;
		pRefCur->GetBox(&box) ;
		if( m_box.min[0] > box.min[0] )
			m_box.min[0] = box.min[0] ;
		if( m_box.min[1] > box.min[1] )
			m_box.min[1] = box.min[1] ;
		if( m_box.max[0] < box.max[0] )
			m_box.max[0] = box.max[0] ;
		if( m_box.max[1] < box.max[1] )
			m_box.max[1] = box.max[1] ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
