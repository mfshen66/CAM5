/*	2001/11/17 nt
 *	implementation file of class CRefSur
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "DNAVI.h"
#include "Feature.h"
#include "TSM.h"
#include "Mark.h" // nt add 2011/4/20
#include "RefSur.h"

double mathDistXD(int dim, double* p1, double* p2)
{
	int k ;
	double d ;

	for( d = 0.0, k = 0 ; k < dim ; k++ )
	{
		d += (p1[k]-p2[k])*(p1[k]-p2[k]) ;
	}

	return sqrt(fabs(d)) ;
}

/*  1997/7/5 nt
 *  计算线段的轮廓点:返回0,无;返回1,有.p1,p2和contourP是dim(2 or 3)维的点
 *	注意：这里不考虑线段的两个端点都是轮廓点的情况
 */
int mathEvalLinContPnt(int dim,
				       double* p1, 
					   double* p2,
                       double a1, 
					   double a2,
                       double attTol,
                       double* contourP)
{	// nt modify 1998/7/5
	int k ;
    double d ;

    if( a1*a2 < -1.0e-50 )
    {
        d = fabs(a1)/(fabs(a1)+fabs(a2)) ;
		for( k = 0 ; k < dim ; k++ )
			contourP[k] = (1.0-d)*p1[k]+d*p2[k] ;
        return 1 ;
    }
	else
    if( fabs(a1) < attTol ) // the begin point of the line is on zplane
    {
		memcpy(contourP, p1, sizeof(double)*dim) ;
        return 1 ;
    }
	else
    if( fabs(a2) < attTol ) // the end point of the line is on zplane
    {
        memcpy(contourP, p2, sizeof(double)*dim) ;
        return 1 ;
    }
    
    return 0 ;
}

/*  1997/7/5 nt
 *  计算三角形的轮廓线:返回0,无;返回1,有.p1,p2,p3和cp1,cp2是dim(2 or 3)维的点
 *	注意：这里不考虑三角形的三个顶点都是轮廓点的情况
 *	tol:1.0e-6(2D),1.0e-5米(3D)
 */
int mathEvalTriContLin(int dim,
					   double* p1, 
					   double* p2, 
					   double* p3,
					   double a1,
					   double a2,
					   double a3,
					   double attTol,
					   double tol, // space tol
					   double* cp1,
					   double* cp2 )
{	// nt modify 1998/7/5
	int np ;
    double cpx[3], d1, d2, d3 ;

    if( (a1 >  attTol && a2 >  attTol && a3 >  attTol) || // 排除绝大多数情况
        (a1 < -attTol && a2 < -attTol && a3 < -attTol) )
        return 0 ; 

    if( fabs(a1) < attTol )
    {
        if( fabs(a2) < attTol )
        {
			memcpy(cp1, p1, sizeof(double)*dim) ;
			memcpy(cp2, p2, sizeof(double)*dim) ;
            return 1 ;
        }
        else
        if( fabs(a3) < attTol )
        {
            memcpy(cp1, p1, sizeof(double)*dim) ;
            memcpy(cp2, p3, sizeof(double)*dim) ;
            return 1 ;
        }
    }

    if( fabs(a2) < attTol && fabs(a3) < attTol )
    {
        memcpy(cp1, p2, sizeof(double)*dim) ;
        memcpy(cp2, p3, sizeof(double)*dim) ;
        return 1 ;
    }

    np = 0 ;
    np += mathEvalLinContPnt(dim, p1, p2, a1, a2, attTol, cp1) ;
    np += mathEvalLinContPnt(dim, p2, p3, a2, a3, attTol, np==1? cp2 : cp1) ;
    if( np == 2 ) // nt modify 1997/12/10
    {
        if( mathEvalLinContPnt(dim, p3, p1, a3, a1, attTol, cpx) )
        {
            d1 = mathDistXD(dim, cp1, cpx) ;
            d2 = mathDistXD(dim, cp2, cpx) ;
            d3 = mathDistXD(dim, cp1, cp2) ;
            if( d1 > d2 ) // nt modify 1998/6/5
            {
				if( d1 > d3 )
					memcpy(cp2, cpx, sizeof(double)*dim) ;
            }
            else
            { // d1 <= d2
				if( d2 > d3 )
					memcpy(cp1, cpx, sizeof(double)*dim) ;
            }
        }
        return mathDistXD(dim, cp1, cp2) > tol ? 1 : 0 ;
    }
    np += mathEvalLinContPnt(dim, p3, p1, a3, a1, attTol, np==1? cp2 : cp1) ;
    if( np < 2 )
		return 0 ;

    return mathDistXD(dim, cp1, cp2) > tol ? 1 : 0 ;
}

////////////////////////////////////////////////////////////////
//	refsur
/*	2001/11/17 nt
 *	refsur
 */
CRefSur::CRefSur()
{
	m_dtype = 0 ;
	m_surface = NULL ;
	m_boxuv.min[0] = m_boxuv.min[1] = 0.0 ;
	m_boxuv.max[0] = m_boxuv.max[1] = 1.0 ;
	m_facet = NULL ;
	m_atts = 0 ;
	mathClnBox(&m_box) ;
	m_n = 0 ;
	m_bnds = NULL ; // nt add 2021/6/14

	// nt add 2009/12/22
	m_color = 256 ; // 256+0
	// nt add 2010/4/22
	m_u = 0. ;
	m_v = 0. ;
	// nt add 2021/8/10
	m_bnds = NULL ;
}

CRefSur::CRefSur(int type, BSSUR* nurbs)
{
	m_dtype = type ;
	m_surface = bsurfaceCreate(nurbs) ;
	m_boxuv.min[0] = nurbs->u_k[nurbs->ord_u-1] ; // nt modify 2014/8/19
	m_boxuv.min[1] = nurbs->v_k[nurbs->ord_v-1] ; // nt modify 2014/8/19
	m_boxuv.max[0] = nurbs->u_k[nurbs->cnt_u] ; // nt modify 2014/8/19
	m_boxuv.max[1] = nurbs->v_k[nurbs->cnt_v] ; // nt modify 2014/8/19
	m_facet = NULL ;
	m_atts = 0 ;
	// nt add 20101/1/19
	//m_np = 0 ;
	//m_ps = NULL ;
	m_n = 0 ;
	m_bnds = NULL ; // nt add 2021/6/14

	if( m_surface )
	{
		Disc(1., 0.15) ;
		CalBox() ;
	}
	else
	{
		mathClnBox(&m_box) ;
		//m_lins = NULL ; // nt modify 2011/4/23
	}
	// nt add 2009/12/22
	m_color = 256 ; // 256+0
	// nt add 2010/4/22
	m_u = 0. ;
	m_v = 0. ;

	return ;
}

CRefSur::CRefSur(int type, SURFACE* surface, BOXUV* boxuv)
{
	m_dtype = type ;
	m_surface = surface ;
	if( boxuv )
		m_boxuv = *boxuv ;
	else
		surfaceGetBoxUV(surface, &m_boxuv) ;
	m_facet = NULL ;
	m_atts = 0 ;
	// nt add 20101/1/19
	//m_np = 0 ;
	//m_ps = NULL ;
	m_n = 0 ;
	m_bnds = NULL ; // nt add 2021/6/14

	if( surface )
	{
		Disc(1., 0.15) ;
		CalBox() ;
	}
	else
	{
		mathClnBox(&m_box) ;
		//m_lins = NULL ; // nt modify 2011/4/23
	}
	// nt add 2009/12/22
	m_color = 256 ; // 256+0
	// nt add 2010/4/22
	m_u = 0. ;
	m_v = 0. ;

	return ;
}

// nt add 2010/5/19 for trim surface
// nt modify 2010/11/12
CRefSur::CRefSur(TFACE* tface)
{
	m_dtype = -1 ; // trim surface
	m_surface = tface->surface ;
	m_boxuv = tface->boxuv ;
	m_facet = NULL ;
	m_atts = 0 ;
	m_n = 0 ;
	m_bnds = NULL ; // nt add 2021/6/14
	int i, j, k = 0, n = tfaceGetNumOfFin(tface) ;
	m_bnds = new BNDX[n] ; // nt add 2021/6/14
	for( i = 0 ; i < n ; i++ )
	{
		m_bnds[i].curve = NULL ;
		m_bnds[i].disc = NULL ;
	}
	TEDGE* tedge = NULL ;
	TFIN* tfin = NULL ;

	// 已剖分完成, nt add 2021/8/4
	if( tface->facet )
	{
		m_facet = tface->facet ;
		tface->facet = NULL ;
		tface->surface = NULL ;
		for( i = 0 ; i < tface->n ; i++ )
		{
			for( j = 0 ; j < tface->tloops[i].n ; j++ )
			{
				tfin = &(tface->tloops[i].tfins[j]) ;
				tedge = tfin->tedge ;
				if( tedge == NULL )
					continue ;
				m_bnds[k].curve = tfin->curve ;
				m_bnds[k].interval = tfin->interval ;
				m_bnds[k].disc = tfin->disc ;
				m_bnds[k].box = tedge->box ;
				m_bnds[k].nAdj = tedgeGetNumOfTFin(tedge) ;
				m_bnds[k].flag = (i+1)*tfin->flag ; // nt add 2021/8/13
				tfin->curve = NULL ;
				tfin->disc = NULL ;
				k++ ;
			}
		}
	}
	m_n = k ;

	if( m_facet ) // nt add 2021/3/7
	{
		facetCalBoxUV(m_facet, &m_boxuv) ;
		facetCalBox(m_facet, &m_box) ;
	}
	// nt add 2009/12/22
	m_color = 256 ; // 256+0
	// nt add 2010/4/22
	m_u = 0. ;
	m_v = 0. ;

	return ;
}

CRefSur::CRefSur(TFACE* tface, double dTol, double dAng)
{
	m_dtype = -1 ; // trim surface
	m_surface = tface->surface ;
	m_boxuv = tface->boxuv ;
	m_facet = NULL ;
	m_atts = 0 ;
	m_n = 0 ;

	int i, j, k, n = tfaceGetNumOfFin(tface) ;
	TSI* tsi = tsiCreate(n) ;
	tsi->surface = tface->surface ;
	tsi->aat = 1 ; // nt add 2021/6/26
	tsi->dTol = dTol ; // 0.5
	tsi->dAng = dAng ; // PI1/4
	tsi->qTol = 0.1 ;
	tsi->pTol = 0.001 ;
	tsi->min = 0. ; // 0.01*dTol ;
	tsi->chd = 1000.*dTol ;
	tsi->ang = 0. ;
	tsi->area = MAX_DBL ;
	tsi->boxuv = m_boxuv ;
	tsi->sp = tface->sp ;
	tsi->nOpt = 0 ;
	tsi->nRefine = 0 ;
	k = 0 ;
	m_bnds = new BNDX[n] ;
	for( i = 0 ; i < n ; i++ )
	{
		m_bnds[i].curve = NULL ;
		m_bnds[i].disc = NULL ;
	}
	for( i = 0 ; i < tface->n ; i++ )
	{
		for( j = 0 ; j < tface->tloops[i].n ; j++ )
		{
			tsi->curves[k] = tface->tloops[i].tfins[j].curve ;
			tsi->intervals[k] = tface->tloops[i].tfins[j].interval ;
			tsi->flags[k] = (i+1)*tface->tloops[i].tfins[j].flag ;
			m_bnds[k].curve = tsi->curves[k] ;
			m_bnds[k].interval = tsi->intervals[k] ;
			curveCalBox(m_bnds[k].curve, &m_bnds[k].interval, &m_bnds[k].box) ;
			m_bnds[k].disc = NULL ;
			m_bnds[k].nAdj = 1 ;
			tface->tloops[i].tfins[j].curve = NULL ;
			k++ ;
		}
	}
	if( tsiTess(tsi) == ERSUCSS )
	{
		if( tsi->nd > 0 )
		{
			for( i = 0 ; i < tsi->nd ; i++ )
			{
				m_bnds[i].disc = tsi->ds[i] ;
				tsi->ds[i] = NULL ;
			}
			m_n = tsi->nd ;
		}
		m_facet = tsi->facet ;
		tsi->facet = NULL ;
	}
	tsiFree(tsi) ;

	if( m_facet )
	{
		facetCalBoxUV(m_facet, &m_boxuv) ;
		facetCalBox(m_facet, &m_box) ;
	}
	m_color = 256 ; // 256+0
	m_u = 0. ;
	m_v = 0. ;

	return ;
}

CRefSur::CRefSur(SURFACE* surface, BOXUV* boxuv)
{
}

// nt add 2010/10/21
CRefSur::CRefSur(FACET* pFacet)
{
	m_dtype = 0 ;
	m_surface = NULL ;
	m_boxuv.min[0] = m_boxuv.min[1] = 0.0 ;
	m_boxuv.max[0] = m_boxuv.max[1] = 1.0 ;
	m_facet = pFacet ;
	m_atts = 0 ;
	if( pFacet )
		facetCalBox(pFacet, &m_box) ;
	else
		mathClnBox(&m_box) ;
	m_n = 0 ;
	m_bnds = NULL ; // nt add 2021/6/14

	// nt add 2009/12/22
	m_color = 256 ; // 256+0
	// nt add 2010/4/22
	m_u = 0. ;
	m_v = 0. ;
}

CRefSur::~CRefSur()
{
	if( m_surface )
		entityFree(m_surface) ;
	if( m_facet )
		facetFree(m_facet) ;
	// nt modify 2021/6/14
	if( m_bnds )
	{
		for( int i = 0 ; i < m_n ; i++ )
		{
			if( m_bnds[i].disc )
				strptFree(m_bnds[i].disc) ;
			if( m_bnds[i].curve )
				entityFree(m_bnds[i].curve) ;
		}
		delete []m_bnds ;
	}

	return ;
}
//--------------------------------------------------------------
// nt add 2009/5/27
// nt rewrite 2014/3/19
CFeature* CRefSur::Copy() // ???trim
{
	CRefSur* pCopy = new CRefSur() ;
	pCopy->SetNo(m_no) ;
	pCopy->SetName(m_name) ;
	pCopy->SetItem(m_hItem) ;
	pCopy->SetFlags(m_flags) ;
	pCopy->SetColor(m_color) ;
	pCopy->SetState(m_state) ;
	pCopy->SetBox(&m_box) ;

	pCopy->SetDetailType(m_dtype) ;
	pCopy->SetBoxUV(&m_boxuv) ;
	pCopy->SetAtts(m_atts) ;
	pCopy->m_surface = geometryCopy(m_surface) ;
	FACET* facet = NULL ;
	if( m_facet )
	{
		facet = facetCreate(m_facet->nnode, m_facet->ntria, m_facet->nquad) ;
		if( facet->nnode > 0 )
			memcpy(facet->nodes, m_facet->nodes, sizeof(FNODE)*m_facet->nnode) ;
		if( facet->ntria > 0 )
			memcpy(facet->trias, m_facet->trias, sizeof(int[3])*m_facet->ntria) ;
		if( facet->nquad > 0 )
			memcpy(facet->quads, m_facet->quads, sizeof(int[3])*m_facet->nquad) ;
	}
	pCopy->SetFacet(facet) ;
	if( m_n > 0 )
	{
		pCopy->m_bnds = new BNDX[m_n] ;
		memcpy(pCopy->m_bnds, m_bnds, sizeof(BNDX)*m_n) ;
		for(int i = 0 ; i < m_n ; i++)
		{
			pCopy->m_bnds[i].curve = curveCopy(m_bnds[i].curve) ;
			pCopy->m_bnds[i].disc = strptCopy(m_bnds[i].disc) ;
		}
		pCopy->m_n = m_n ;
	}

	return pCopy ;
}

int	CRefSur::GetType()
{
	return ftRefSur ;
}

int	CRefSur::GetDetailType()
{
	return m_dtype ;
}

// nt add 2014/3/19
int	CRefSur::SetDetailType(int dtype)
{
	int old = m_dtype ;

	m_dtype = dtype ;

	return old ;
}

void CRefSur::GetBoxUV(BOXUV* boxuv)
{
	if( boxuv )
		*boxuv = m_boxuv ;
}

void CRefSur::SetBoxUV(BOXUV* boxuv) // nt add 2003/5/31
{
	if( !boxuv )
		return ;
	m_boxuv = *boxuv ;

	if( m_facet )
		facetFree(m_facet) ;
	m_facet = NULL ;
	if( m_surface )
	{
		Disc(1., 0.15) ;
		CalBox() ;
	}

	return ;
}

int	CRefSur::CalBox()
{
	if( !m_surface )
		return 0 ;

	surfaceCalBox(m_surface, &m_boxuv, &m_box) ;

	return 1 ;
}

// nt add 2009/5/27
int CRefSur::Move(VEC3D v)
{
	if( m_surface )
		geometryMove(m_surface, v) ;
	if( m_facet )
	{
		for( int i = 0 ; i < m_facet->nnode ; i++ )
		{
			m_facet->nodes[i].p[0] += v[0] ;
			m_facet->nodes[i].p[1] += v[1] ;
			m_facet->nodes[i].p[2] += v[2] ;
		}
	}
	// nt add 2010/1/20
	for( int i = 0 ; i < m_n ; i++ )
	{
		for( int k = 0 ; k < m_bnds[i].disc->np ; k++ )
		{
			m_bnds[i].disc->ps[k][0] += v[0] ;
			m_bnds[i].disc->ps[k][1] += v[1] ;
			m_bnds[i].disc->ps[k][2] += v[2] ;
		}
		if( m_bnds[i].curve ) // nt add 2021/6/13
			geometryMove(m_bnds[i].curve, v) ;
		mathMoveBox(&m_bnds[i].box, v) ;
	}
	
	m_box.min[0] += v[0] ;
	m_box.min[1] += v[1] ;
	m_box.min[2] += v[2] ;
	m_box.max[0] += v[0] ;
	m_box.max[1] += v[1] ;
	m_box.max[2] += v[2] ;

	return 1 ;
}

// nt add 2009/5/27
int CRefSur::Rotate(PNT3D pivot, VEC3D axis, double angle)
{
	if( m_surface )
		geometryRotate(m_surface, pivot, axis, angle) ;
	if( m_facet )
	{
		for( int i = 0 ; i < m_facet->nnode ; i++ )
		{
			mathRotPnt(axis, pivot, angle, m_facet->nodes[i].p, m_facet->nodes[i].p) ;
			mathRotVec(axis, pivot, angle, m_facet->nodes[i].normal, m_facet->nodes[i].normal) ;
		}
	}
	// nt add 2010/1/20
	for( int i = 0 ; i < m_n ; i++ )
	{
		for( int k = 0 ; k < m_bnds[i].disc->np ; k++ )
			mathRotPnt(axis, pivot, angle, m_bnds[i].disc->ps[k], m_bnds[i].disc->ps[k]) ;
		if( m_bnds[i].curve ) // nt add 2021/6/13
			geometryRotate(m_bnds[i].curve, pivot, axis, angle) ;
		mathRotateBox(&m_bnds[i].box, pivot, axis, angle) ;
	}
	CalBox() ;

	return 1 ;
}

// nt add 2010/10/17
int CRefSur::Mirror(PNT3D pivot, VEC3D normal)
{
	if( m_surface )
		geometryMirror(m_surface, pivot, normal) ;
	if( m_facet )
	{
		for( int i = 0 ; i < m_facet->nnode ; i++ )
		{
			mathMirPnt(pivot, normal, m_facet->nodes[i].p, m_facet->nodes[i].p) ;
			mathMirVec(pivot, normal, m_facet->nodes[i].normal, m_facet->nodes[i].normal) ;
			m_facet->nodes[i].normal[0] = -m_facet->nodes[i].normal[0] ;
			m_facet->nodes[i].normal[1] = -m_facet->nodes[i].normal[1] ;
			m_facet->nodes[i].normal[2] = -m_facet->nodes[i].normal[2] ;
		}
	}
	// nt add 2010/1/20
	for( int i = 0 ; i < m_n ; i++ )
	{
		for( int k = 0 ; k < m_bnds[i].disc->np ; k++ )
			mathMirPnt(pivot, normal, m_bnds[i].disc->ps[k], m_bnds[i].disc->ps[k]) ;
		if( m_bnds[i].curve ) // nt add 2021/6/13
			geometryMirror(m_bnds[i].curve, pivot, normal) ;
		curveCalBox(m_bnds[i].curve, &m_bnds[i].interval, &m_bnds[i].box) ;
	}
	CalBox() ;

	return 1 ;
}

// nt add 2010/9/11
// nt add 2010/10/17
int CRefSur::Scale(double scale)
{
	if( m_surface )
		geometryScale(m_surface, scale) ;
	if( m_facet )
	{
		for( int i = 0 ; i < m_facet->nnode ; i++ )
		{
			m_facet->nodes[i].p[0] *= scale ;
			m_facet->nodes[i].p[1] *= scale ;
			m_facet->nodes[i].p[2] *= scale ;
		}
	}
	// nt add 2010/1/20
	for( int i = 0 ; i < m_n ; i++ )
	{
		for( int k = 0 ; k < m_bnds[i].disc->np ; k++ )
		{
			m_bnds[i].disc->ps[k][0] *= scale ;
			m_bnds[i].disc->ps[k][1] *= scale ;
			m_bnds[i].disc->ps[k][2] *= scale ;
		}
		if( m_bnds[i].curve ) // nt add 2021/6/13
			geometryScale(m_bnds[i].curve, scale) ;
		curveCalBox(m_bnds[i].curve, &m_bnds[i].interval, &m_bnds[i].box) ;
	}
	CalBox() ;

	return 1 ;
}

SURFACE* CRefSur::GetSurface()
{
	return m_surface ;
}

BSSUR* CRefSur::GetNurbs()
{
	int exact ;
	BSSUR* nurbs ;

	surfaceToNurbs(m_surface, &m_boxuv, &nurbs, &exact) ;

	return nurbs ;
}

// nt add 2010/4/22
double CRefSur::GetU()
{
	return m_u ;
}

// nt add 2010/4/22
int CRefSur::SetU(double u)
{
	m_u = u ;
	return 1 ;
}

// nt add 2010/4/22
double CRefSur::GetV()
{
	return m_v ;
}

// nt add 2010/4/22
int CRefSur::SetV(double v)
{
	m_v = v ;
	return 1 ;
}

// nt add 2010/5/20
FACET* CRefSur::GetFacet()
{
	return m_facet ;
}

int CRefSur::SetFacet(FACET* facet)
{
	if( m_facet )
		facetFree(m_facet) ;
	m_facet = facet ;

	return 1 ;
}

// nt add 2014/3/19
int CRefSur::GetNumOfBnd()
{
	return m_n ;
}

// nt add 2014/3/19
STRPT* CRefSur::GetBndDisc(int i)
{
	if( i < 0 ||
		i >= m_n ||
		m_bnds == NULL )
		return NULL ;
	return m_bnds[i].disc ;
}

// nt add 2010/5/23, 0=rev u, 1=vrev v, 2=u<->v
// 目前只针对NURBS曲面
// 要求NURBS曲面的参数域为[0,1]x[0,1]
int CRefSur::RevsDir(int flag)
{
	int type = entityGetType(m_surface) ;
	if( type == etBSurface )
	{
		BSSUR* nurbs = surfaceGetNurbs(m_surface) ;
		if( nurbs )
		{
			double min, max, u, v ;
			bssurRevsDir(nurbs, flag) ;
			if( m_facet )
			{
				for( int i = 0 ; i < m_facet->nnode ; i++ )
				{
					if( flag == 0 ||
						flag == 1 )
						m_facet->nodes[i].uv[flag] = 1-m_facet->nodes[i].uv[flag] ;
					else
					{
						u = m_facet->nodes[i].uv[0] ;
						v = m_facet->nodes[i].uv[1] ;
						m_facet->nodes[i].uv[0] = v ;
						m_facet->nodes[i].uv[1] = u ;
					}
				}
			}
			if( flag == 0 ||
				flag == 1 )
			{
				min = m_boxuv.min[flag] ;
				max = m_boxuv.max[flag] ;
				m_boxuv.min[flag] = 1.0-max ;
				m_boxuv.max[flag] = 1.0-min ;
			}
			else
			{
				min = m_boxuv.min[0] ;
				max = m_boxuv.max[0] ;
				m_boxuv.min[0] = m_boxuv.min[1] ;
				m_boxuv.max[0] = m_boxuv.max[1] ;
				m_boxuv.min[1] = min ;
				m_boxuv.max[1] = max ;
			}
		}
		return 1 ;
	}
	else
		return 0 ;
}

// nt modify 2010/3/16 : 估计合适的nu和nv
int CRefSur::Disc(double tol, double ang)
{
	int i, j, n, nu, nv, k ;
	PNT3D dege_p ;
	double step ;
	CURVE* isocur ;
	INTERVAL interval ;
	STRPT* su, *sv, *temp ;

	// nt add 2020/3/25 for test only
	/*m_facet = facetCreate(4, 2, 0) ;
	memcpy(m_facet->nodes[0].uv, m_boxuv.min, sizeof(PNT2D)) ;
	m_facet->nodes[1].uv[0] = m_boxuv.max[0] ;
	m_facet->nodes[1].uv[1] = m_boxuv.min[1] ;
	memcpy(m_facet->nodes[2].uv, m_boxuv.max, sizeof(PNT2D)) ;
	m_facet->nodes[3].uv[0] = m_boxuv.min[0] ;
	m_facet->nodes[3].uv[1] = m_boxuv.max[1] ;
	for( n = 0 ; n < 4 ; n++ )
	{
		surfaceEvalNormal(m_surface,
						  m_facet->nodes[n].uv[0],
						  m_facet->nodes[n].uv[1],
						  m_facet->nodes[n].p,
						  m_facet->nodes[n].normal) ;
		m_facet->nodes[n].attrib = 0. ;
	}
	m_facet->trias[0][0] = 0 ;
	m_facet->trias[0][1] = 1 ;
	m_facet->trias[0][2] = 2 ;
	m_facet->trias[1][0] = 2 ;
	m_facet->trias[1][1] = 3 ;
	m_facet->trias[1][2] = 0 ;

	return 1 ;*/
	// end test

	if( !m_surface )
		return 0 ;
	isocur = NULL ;
	su = NULL ;
	sv = NULL ;
	temp = NULL ;

	if( tol < 1.0e-5 ||
		ang < 1.0e-2 )
		return 0 ;

	if( m_facet )
	{
		facetFree(m_facet) ;
		m_facet = NULL ;
	}

	// nt add 2014/3/18
	if( entityGetType(m_surface) == etOffset )
		goto MAKE_MESH ;

	// nt add 2010/3/16 : 估计u向剖分间距
	step = 0.5*(m_boxuv.max[1]-m_boxuv.min[1]) ;
	for( k = 0 ; k < 3 ; k++ )
	{
		if( surfaceGetIsoCurve(m_surface,
							   1,
							   m_boxuv.min[1]+k*step,
							   &isocur, 
							   &interval,
							   dege_p) == ERSUCSS &&
			isocur )
		{
			interval.t1 = m_boxuv.min[0] ;
			interval.t2 = m_boxuv.max[0] ;
			if( curveDisc(isocur, 
						  &interval, 
						  tol, 
						  ang,
						  100.,// nt modify 2012/5/22, old is 10.
						  STRPT_PT|STRPT_PARAM,
						  &temp) != ERSUCSS )
				goto ExceExit ;
			if( isocur )
				entityFree(isocur) ;
			isocur = NULL ;
			if( su == NULL )
				su = temp ;
			else
			{
				if( su->np < temp->np )
				{
					strptFree(su) ;
					su = temp ;
				}
				else
					strptFree(temp) ;
			}
			temp = NULL ;
		}
	}

	// nt add 2010/3/16 : 估计v向剖分间距
	step = 0.5*(m_boxuv.max[0]-m_boxuv.min[0]) ;
	for( k = 0 ; k < 3 ; k++ )
	{
		if( surfaceGetIsoCurve(m_surface,
							   0,
							   m_boxuv.min[0]+k*step,
							   &isocur, 
							   &interval,
							   dege_p) == ERSUCSS &&
			isocur )
		{
			interval.t1 = m_boxuv.min[1] ;
			interval.t2 = m_boxuv.max[1] ;
			if( curveDisc(isocur, 
						  &interval, 
						  tol, 
						  ang,
						  100.,// nt modify 2012/5/22, old is 10.
						  STRPT_PT|STRPT_PARAM,
						  &temp) != ERSUCSS )
				goto ExceExit ;
			if( isocur )
				entityFree(isocur) ;
			isocur = NULL ;
			if( sv == NULL )
				sv = temp ;
			else
			{
				if( sv->np < temp->np )
				{
					strptFree(sv) ;
					sv = temp ;
				}
				else
					strptFree(temp) ;
			}
			temp = NULL ;
		}
	}
MAKE_MESH:
	if( su == NULL ) // nt add 2010/5/19
	{
		su = strptCreate(26, STRPT_PT|STRPT_PARAM) ;
		for( i = 0 ; i < su->np ; i++ )
		{
			su->ps[i][0] = 0. ;
			su->ps[i][1] = 0. ;
			su->ps[i][2] = 0. ;
			su->ts[i] = m_boxuv.min[0]+i*(m_boxuv.max[0]-m_boxuv.min[0])/25 ;
		}
	}
	if(	sv == NULL ) // nt add 2010/5/19
	{
		sv = strptCreate(26, STRPT_PT|STRPT_PARAM) ;
		for( i = 0 ; i < sv->np ; i++ )
		{
			sv->ps[i][0] = 0. ;
			sv->ps[i][1] = 0. ;
			sv->ps[i][2] = 0. ;
			sv->ts[i] = m_boxuv.min[1]+i*(m_boxuv.max[1]-m_boxuv.min[1])/25 ;
		}
	}
	nu = su->np-1 ;
	nv = sv->np-1 ;

	if( nu > 100 ) // nt add 2012/5/22
		nu = 100 ;
	if( nv > 100 ) // nt add 2012/5/22
		nv = 100 ;
	m_facet = facetCreate((nu+1)*(nv+1), nu*nv*2, 0) ;
	if( !m_facet )
		goto ExceExit ;
	for( n = 0, i = 0 ; i <= nu ; i++ )
	{
		for( j = 0 ; j <= nv ; j++, n++ )
		{
			m_facet->nodes[n].uv[0] = su->ts[i] ; // obtain parameter u directly
			m_facet->nodes[n].uv[1] = sv->ts[j] ; // obtain parameter v directly
			m_facet->nodes[n].attrib = 0. ;
			surfaceEvalNormal(m_surface,
						      m_facet->nodes[n].uv[0],
						      m_facet->nodes[n].uv[1],
						      m_facet->nodes[n].p,
						      m_facet->nodes[n].normal) ;
		}
	} // n = i*(nv+1)+j
	if( su )
	{
		strptFree(su) ;
		su = NULL ;
	}
	if( sv )
	{
		strptFree(sv) ;
		sv = NULL ;
	}

	// nt add 20101/1/19 : border
	m_n = 4 ;
	m_bnds = new BNDX[4] ;
	m_bnds[0].disc = strptCreate(nu+1, STRPT_PT) ;
	for( i = 0 ; i <= nu ; i++ ) // j = 0
		memcpy(m_bnds[0].disc->ps[i], m_facet->nodes[i*(nv+1)].p, sizeof(PNT3D)) ;
	m_bnds[1].disc = strptCreate(nv+1, STRPT_PT) ;
	for( j = 0 ; j <= nv ; j++, n++ )
		memcpy(m_bnds[1].disc->ps[j], m_facet->nodes[nu*(nv+1)+j].p, sizeof(PNT3D)) ;
	m_bnds[2].disc = strptCreate(nu+1, STRPT_PT) ;
	for( i = 0 ; i <= nu ; i++ ) // j = nv
		memcpy(m_bnds[2].disc->ps[i], m_facet->nodes[i*(nv+1)+nv].p, sizeof(PNT3D)) ;
	m_bnds[3].disc = strptCreate(nv+1, STRPT_PT) ;
	for( j = 0 ; j <= nv ; j++ ) // i = 0
		memcpy(m_bnds[3].disc->ps[j], m_facet->nodes[j].p, sizeof(PNT3D)) ;
	for( i = 0 ; i < m_n ; i++ ) // nt add 2021/6/19
		m_bnds[i].curve = NULL ;

	for( n = 0, i = 0 ; i < nu ; i++ )
	{
		for( j = 0 ; j < nv ; j++, n += 2 )
		{
			m_facet->trias[n][0] = i*(nv+1)+j ;
			m_facet->trias[n][1] = (i+1)*(nv+1)+j ;
			m_facet->trias[n][2] = (i+1)*(nv+1)+(j+1) ;

			m_facet->trias[n+1][0] = i*(nv+1)+j ;
			m_facet->trias[n+1][1] = (i+1)*(nv+1)+(j+1) ;
			m_facet->trias[n+1][2] = i*(nv+1)+(j+1) ;
		}
	}

	return 1 ;
ExceExit:
	if( isocur )
		entityFree(isocur) ;
	if( temp )
		strptFree(temp) ;
	if( su )
		strptFree(su) ;
	if( sv )
		strptFree(sv) ;
	return 0 ;
}

// nt add 2010/5/22
// nt modify 2011/4/23
CMark* CRefSur::CalIso(ISODEF* pIsoDef)
{
	int i, n ;
	double k1, k2, z, attTol, c, attMin, attMax, fix ;
	PNT3D p, c1, c2 ;
	VEC3D dir1, dir2, normal ;
	CMark* pMark = NULL ;

	if( !m_surface ||
		!m_facet ||
		!pIsoDef ||
		pIsoDef->nZPlane < 1 )
		return NULL ;
	if( fabs(pIsoDef->attMax-pIsoDef->attMin) < MIN_DBL )
	{
		attMax = -MAX_DBL ;
		attMin = MAX_DBL ;
		fix = 0 ;
	}
	else
	{
		attMax = pIsoDef->attMax ;
		attMin = pIsoDef->attMin ;
		fix = 1 ;
	}
	for( i = 0 ; i < m_facet->nnode ; i++ )
	{
		surfaceEvalCur(m_surface,
					   m_facet->nodes[i].uv[0],
					   m_facet->nodes[i].uv[1],
					   p,
					   normal,
					   dir1,
					   dir2,
					   &k1,
					   &k2) ;
		switch(pIsoDef->attType)
		{
		case 0:
			c = k1*k2 ;
			break ;
		case 1:
			c = 0.5*(k1+k2) ;
			break ;
		case 2:
			c = k1*k1+k2*k2 ;
			break ;
		case 3:
			c = mathMIN(k1, k2) ;
			break ;
		case 4:
			c = mathMAX(k1, k2) ;
			break ;
		case 5:
			c = sqrt(k1*k1+k2*k2) ;
			break ;
		default:
			c = 0. ;
		}
		c *= pIsoDef->factor ;
		m_facet->nodes[i].attrib = c ;
		if( !fix )
		{
			if( attMax < c ) // nt add 2011/4/20
				attMax = c ;
			if( attMin > c ) // nt add 2011/4/20
				attMin = c ;
		}
	}

	if( attMax-attMin < MIN_DBL )
		return NULL ;
	pMark = new CMark(0) ;
	if( pIsoDef->attTol < MIN_DBL )
	{
		attTol = (attMax-attMin)*1.0e-5 ;
		pIsoDef->attTol = attTol ;
	}
	else
		attTol = pIsoDef->attTol ;
	for( n = 0 ; n < pIsoDef->nZPlane ; n++ )
	{
		z = attMin+(n+1)*(attMax-attMin)/(pIsoDef->nZPlane+1) ;
		for( i = 0 ; i < m_facet->ntria ; i++ )
		{
			if( mathEvalTriContLin(3, // calculate an attribute line on triangle 
								   m_facet->nodes[m_facet->trias[i][0]].p,
								   m_facet->nodes[m_facet->trias[i][1]].p,
								   m_facet->nodes[m_facet->trias[i][2]].p,
								   m_facet->nodes[m_facet->trias[i][0]].attrib-z,
								   m_facet->nodes[m_facet->trias[i][1]].attrib-z,
								   m_facet->nodes[m_facet->trias[i][2]].attrib-z,
								   attTol,
								   1.0e-5,
								   c1,
								   c2) )
			{
				pMark->AddLin(c1, c2) ;
			}
		}
	}

	return pMark ;
}

// nt add 2011/4/20
CMark* CRefSur::CalKuKv(int nu, int nv, double factor)
{
	int i, k ;
	double u, v, c ;
	PNT3D dege_p, p ;
	VEC3D T, N, B ;
	INTERVAL interval ;
	CURVE* curve = NULL ;
	CMark* pMark = NULL ;
	STRPT* strpt = NULL ;

	pMark = new CMark(0) ;

	for( k = 0 ; k <= nu ; k++ ) // u = constant
	{
		u = m_boxuv.min[0]+k*(m_boxuv.max[0]-m_boxuv.min[0])/nu ;
		if( surfaceGetIsoCurve(m_surface, 
							   IDU,
							   u,
							   &curve, 
							   &interval,
							   dege_p) == ERSUCSS &&
			curve )
		{
			interval.t1 = m_boxuv.min[1] ;
			interval.t2 = m_boxuv.max[1] ;
			if( curveDisc(curve, 
						  &interval, 
						  0.01, 
						  0.15,
						  10.,
						  STRPT_PT | STRPT_VEC | STRPT_PARAM,
						  &strpt) != ERSUCSS )
				goto ExceExit ;
			for( i = 0 ; i < strpt->np ; i++ )
			{
				if( curveEvalCur(curve,
								 strpt->ts[i],
								 p,
								 T,
								 N,
								 B,
								 &c) != ERSUCSS )
					goto ExceExit ;
				c *= factor ;
				strpt->vs[i][0] = p[0]-N[0]*c ;
				strpt->vs[i][1] = p[1]-N[1]*c ;
				strpt->vs[i][2] = p[2]-N[2]*c ;
				pMark->AddLin(p, strpt->vs[i]) ;
				if( i > 0 )
				{
					pMark->AddLin(strpt->ps[i-1], strpt->ps[i]) ;
					pMark->AddLin(strpt->vs[i-1], strpt->vs[i]) ;
				}
			}
			strptFree(strpt) ;
			strpt = NULL ;
			entityFree(curve) ;
			curve = NULL ;
		}
	}

	for( k = 0 ; k <= nv ; k++ ) // v = constant
	{
		v = m_boxuv.min[1]+k*(m_boxuv.max[1]-m_boxuv.min[1])/nv ;
		if( surfaceGetIsoCurve(m_surface, 
							   IDV,
							   v,
							   &curve, 
							   &interval,
							   dege_p) == ERSUCSS &&
			curve )
		{
			interval.t1 = m_boxuv.min[0] ;
			interval.t2 = m_boxuv.max[0] ;
			if( curveDisc(curve, 
						  &interval, 
						  0.01, 
						  0.15,
						  10.,
						  STRPT_PT | STRPT_VEC | STRPT_PARAM,
						  &strpt) != ERSUCSS )
				goto ExceExit ;
			for( i = 0 ; i < strpt->np ; i++ )
			{
				if( curveEvalCur(curve,
								 strpt->ts[i],
								 p,
								 T,
								 N,
								 B,
								 &c) != ERSUCSS )
					goto ExceExit ;
				c *= factor ;
				strpt->vs[i][0] = p[0]-N[0]*c ;
				strpt->vs[i][1] = p[1]-N[1]*c ;
				strpt->vs[i][2] = p[2]-N[2]*c ;
				pMark->AddLin(p, strpt->vs[i]) ;
				if( i > 0 )
				{
					pMark->AddLin(strpt->ps[i-1], strpt->ps[i]) ;
					pMark->AddLin(strpt->vs[i-1], strpt->vs[i]) ;
				}
			}
			strptFree(strpt) ;
			strpt = NULL ;
			entityFree(curve) ;
			curve = NULL ;
		}
	}
	
	return pMark ;
ExceExit:
	if( curve )
		entityFree(curve) ;
	if( strpt )
		entityFree(strpt) ;
	if( pMark )
		delete pMark ;
	return NULL ;
}

// nt add 2010/5/22
void CRefSur::SwapDrawUV()
{
	if( m_atts&2 )
		m_atts -= 2 ;
	else
		m_atts += 2 ;
}

// nt add 2010/8/28
int CRefSur::GetAtts()
{
	return m_atts ;
}

// nt add 2010/5/22
void CRefSur::SetAtts(int visible)
{
	if( visible == 0 )
	{
		if( m_atts&1 )
			m_atts -= 1 ;
	}
	else
	{
		if( !(m_atts&1) )
			m_atts += 1 ;
	}
}

// nt add 2021/6/14
void CRefSur::InitBndAdj()
{
	for(int i = 0 ; i < m_n ; i++)
	{
		m_bnds[i].nAdj = 1 ;
	}

	return ;
}

void FNodeGetMidPnt(FNODE* node1, FNODE* node2, FNODE* mid)
{
	mid->uv[0] = 0.5*(node1->uv[0]+node2->uv[0]) ;
	mid->uv[1] = 0.5*(node1->uv[1]+node2->uv[1]) ;
	mid->p[0] = 0.5*(node1->p[0]+node2->p[0]) ;
	mid->p[1] = 0.5*(node1->p[1]+node2->p[1]) ;
	mid->p[2] = 0.5*(node1->p[2]+node2->p[2]) ;
	mid->attrib = 0.5*(node1->attrib+node2->attrib) ;
	mid->normal[0] = 0.5*(node1->normal[0]+node2->normal[0]) ;
	mid->normal[1] = 0.5*(node1->normal[1]+node2->normal[1]) ;
	mid->normal[2] = 0.5*(node1->normal[2]+node2->normal[2]) ;

	return ;
}

void FNodeEval(SURFACE* surface, FNODE* node)
{
	surfaceEvalNormal(surface, node->uv[0], node->uv[1], node->p, node->normal) ;

	return ;
}

// (*is)[3] : 刚生成的结点信息
// 新结点是两个老结点的中点，第j个新结点：
// is[j][0] : 起点结点序号，对应一个老结点
// is[j][1] : 终点结点序号，对应一个老结点
// is[j][2] : 新结点的序号
int GetJustMadeNode(int m, int (*is)[3], int oldA, int oldB)
{
	int k ;

	for( k = 0 ; k < m ; k++ )
	{
		if( (is[k][0] == oldA && is[k][1] == oldB) || // 找已有
			(is[k][0] == oldB && is[k][1] == oldA) )
			return is[k][2] ;
	}

	return -1 ;
}

void TriSetNodeAll(int tri[3], int A, int B, int C)
{
	tri[0] = A ;
	tri[1] = B ;
	tri[2] = C ;

	return ;
}

// nt add 2021/6/19
int CRefSur::RefineFacet(double tol)
{
	int i, k, I, J, K, A, B, C, n, m = 0, (*is)[3] ;
	double u, v, d ;
	PNT3D p, q ;
	CIA tris ;
	FACET* f = NULL ;

	if( m_surface == NULL ||
		m_facet == NULL )
		return 1 ;

	for( i = 0 ; i < m_facet->ntria ; i++ )
	{
		I = m_facet->trias[i][0] ;
		J = m_facet->trias[i][1] ;
		K = m_facet->trias[i][2] ;

		u = (m_facet->nodes[I].uv[0]+
			 m_facet->nodes[J].uv[0]+
			 m_facet->nodes[K].uv[0])/3 ;
		v = (m_facet->nodes[I].uv[1]+
			 m_facet->nodes[J].uv[1]+
			 m_facet->nodes[K].uv[1])/3 ;
		for( k = 0 ; k < 3 ; k++ )
			p[k] = (m_facet->nodes[I].p[k]+
				    m_facet->nodes[J].p[k]+
				    m_facet->nodes[K].p[k])/3 ;
		//surfaceSnap(surface, p, q, &u, &v) ;
		surfaceEvalPos(m_surface, u, v, q) ;
		d = mathDist(p, q) ;
		if( d > tol )
			tris.Add(i) ;
	}
	n = tris.GetSize() ;
	if( n < 1 )
		return 0 ;

	// 注意：最多增加3*n个顶点和6*n个三角形
	is = new int[3*n][3] ;
	f = facetCreate(m_facet->nnode+3*n, m_facet->ntria+6*n, 0) ;
	f->nnode = m_facet->nnode ;
	f->ntria = m_facet->ntria ;
	memcpy(f->nodes, m_facet->nodes, sizeof(FNODE)*m_facet->nnode) ;
	memcpy(f->trias, m_facet->trias, sizeof(int[3])*m_facet->ntria) ;
	for( k = 0 ; k < n ; k++ )
	{
		i = tris[k] ;
		I = m_facet->trias[i][0] ;
		J = m_facet->trias[i][1] ;
		K = m_facet->trias[i][2] ;

		A = GetJustMadeNode(m, is, J, K) ;
		if( A < 0 )
		{
			A = f->nnode ;
			is[m][0] = J ;
			is[m][1] = K ;
			is[m][2] = A ; // 新的
			FNodeGetMidPnt(&f->nodes[J], &f->nodes[K], &f->nodes[A]) ;
			FNodeEval(m_surface, &f->nodes[A]) ;
			(f->nnode)++ ;
			m++ ;
		}

		B = GetJustMadeNode(m, is, I, K) ;
		if( B < 0 )
		{
			B = f->nnode ;
			is[m][0] = I ;
			is[m][1] = K ;
			is[m][2] = B ; // 新的
			FNodeGetMidPnt(&f->nodes[I], &f->nodes[K], &f->nodes[B]) ;
			FNodeEval(m_surface, &f->nodes[B]) ;
			(f->nnode)++ ;
			m++ ;
		}

		C = GetJustMadeNode(m, is, I, J) ;
		if( C < 0 )
		{
			C = f->nnode ;
			is[m][0] = I ;
			is[m][1] = J ;
			is[m][2] = C ; // 新的
			FNodeGetMidPnt(&f->nodes[I], &f->nodes[J], &f->nodes[C]) ;
			FNodeEval(m_surface, &f->nodes[C]) ;
			(f->nnode)++ ;
			m++ ;
		}

		TriSetNodeAll(f->trias[i], A, B, C) ;
		TriSetNodeAll(f->trias[f->ntria], I, C, B) ; // new create tri
		(f->ntria)++ ;
		TriSetNodeAll(f->trias[f->ntria], C, J, A) ; // new create tri
		(f->ntria)++ ;
		TriSetNodeAll(f->trias[f->ntria], A, K, B) ; // new create tri
		(f->ntria)++ ;
	}

	for( i = 0 ; i < f->ntria ; i++ ) // ntria可能在增加，一次只处理1条边
	{
		I = f->trias[i][0] ;
		J = f->trias[i][1] ;
		K = f->trias[i][2] ;

		A = GetJustMadeNode(m, is, J, K) ;
		if( A >= 0 )
		{
			TriSetNodeAll(f->trias[i], I, J, A) ; // modify old tri
			TriSetNodeAll(f->trias[f->ntria], A, K, I) ; // new create tri
			(f->ntria)++ ;
			i-- ; // 下次循环还是从i开始
			continue ;
		}

		B = GetJustMadeNode(m, is, I, K) ;
		if( B >= 0 )
		{
			TriSetNodeAll(f->trias[i], I, J, B) ; // modify old tri
			TriSetNodeAll(f->trias[f->ntria], B, J, K) ;
			(f->ntria)++ ;
			i-- ; // 下次循环还是从i开始
			continue ;
		}

		C = GetJustMadeNode(m, is, I, J) ;
		if( C >= 0 )
		{
			TriSetNodeAll(f->trias[i], I, C, K) ; // modify old tri
			TriSetNodeAll(f->trias[f->ntria], C, J, K) ;
			(f->ntria)++ ;
			i-- ; // 下次循环还是从i开始
			continue ;
		}
	}
	facetFree(m_facet) ; // release old
	m_facet = f ; // update
	tris.RemoveAll() ;
	delete []is ;

	return n ;
}

void CRefSur::OptTri(int nOpt)
{
	if( m_facet )
	{
		for( int i = 0 ; i < nOpt ; i++ )
		{
			int n = facetOpt(m_facet, 30., 1000) ;
			if( n < 1 )
				break ;
		}
	}
}

// nt add 2021/8/13
int CRefSur::ReDisc()
{
	int i, k, N = 0 ;
	double s = 0., dd, uu, vv ;
	PNT3D p ;
	VEC3D du, dv ;
	TSI* tsi = NULL ;
	STRPT* disc ;

	if( m_n < 1 ||
		m_bnds == NULL )
		return 0 ;

	tsi = tsiCreate2(m_n) ;
	tsi->surface = m_surface ;
	tsi->aat = 0 ;
	tsi->dTol = 0.1 ;
	tsi->dAng = PI1/8 ;
	tsi->qTol = 0.01 ;
	tsi->pTol = 0.001 ;
	tsi->min = 1. ;
	tsi->chd = 1000. ;
	tsi->ang = PI1/4 ;
	tsi->area = 1. ;
	tsi->boxuv = m_boxuv ;
	tsi->sp = 1 ;
	tsi->nOpt = 6 ;
	tsi->nRefine = 4 ;
	tsi->nd = m_n ;
	for( k = 0 ; k < m_n ; k++ )
	{
		tsi->flags[k] = m_bnds[k].flag ;
		disc = m_bnds[k].disc ;
		tsi->ds[k] = disc ;
		for( i = 1 ; i < disc->np ; i++ )
			s += mathDist(disc->ps[i-1], disc->ps[i]) ;
		N += (disc->np-1) ;
	}

	dd = s/N ;
	uu = 0.5*(m_boxuv.min[0]+m_boxuv.max[0]) ;
	vv = 0.5*(m_boxuv.min[1]+m_boxuv.max[1]) ;
	surfaceEvalDer1O(m_surface, uu, vv, p, du, dv) ;
	uu = mathVecLen(du) ;
	vv = mathVecLen(dv) ;
	double rd = 0.2*mathRand01()+0.4 ;
	tsi->su = rd*dd/uu ;
	tsi->sv = rd*dd/vv ;
	tsi->dd = dd ;

	tsiTess3(tsi) ;

	if( tsi->facet )
	{
		if( m_facet )
			facetFree(m_facet) ;
		m_facet = tsi->facet ;
		tsi->facet = NULL ;
	}
	for( k = 0 ; k < m_n ; k++ )
		tsi->ds[k] = NULL ;
	tsiFree(tsi) ;

	return 1 ;
}
//--------------------------------------------------------------
// nt add 2011/7/3
int CRefSur::DrawSurface(void* pVI, int drawMode)
{
	int i ;

	if( m_facet == NULL )
		return 0 ;

	if( drawMode != dmWireframe )
	{
		SetDrawColor(drawMode) ;
		glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), m_facet->nodes[0].p) ;
		glNormalPointer(GL_DOUBLE, sizeof(FNODE), m_facet->nodes[0].normal) ;
		glDrawElements(GL_TRIANGLES, m_facet->ntria*3, GL_UNSIGNED_INT, m_facet->trias) ;
	}
	else
	{
		for (i = 0; i < m_facet->ntria; i++)
		{
			glBegin(GL_LINE_LOOP);
				glVertex3dv(m_facet->nodes[m_facet->trias[i][0]].p);
				glVertex3dv(m_facet->nodes[m_facet->trias[i][1]].p);
				glVertex3dv(m_facet->nodes[m_facet->trias[i][2]].p);
			glEnd();
		}
	}
	
	return 1 ;
}

// nt add 2011/7/3
int CRefSur::DrawBorder(void* pVI, int drawMode)
{
	if( m_bnds == NULL )
		return 0 ;

	glLineWidth(3.5f) ;
	for( int i = 0 ; i < m_n ; i++ )
	{
		if( m_bnds[i].nAdj == 2 ) // nt add 2021/6/15
			glColor3ub(0, 255, 0) ; //SetDrawColor(drawMode) ; // nt add 2021/6/15
		else // nt add 2021/6/15
			glColor3ub(255, 0, 0) ; // nt add 2021/6/15
		glBegin(GL_LINE_STRIP) ;
			for( int j = 0 ; j < m_bnds[i].disc->np ; j++ )
				glVertex3dv(m_bnds[i].disc->ps[j]) ;
		glEnd() ;
	}

	return 1 ;
}

// nt add 2011/7/3
int CRefSur::DrawContour(void* pVI, int drawMode)
{
	int i, I, J, K ;
	double a, b, c ;
	PNT3D c1, c2 ;
	VEC3D viewDir ;

	if( !m_facet )
		return 0 ;
	viGetViewDir(pVI, viewDir) ;
	int old = m_color ;
	m_color = 0 ;
	SetDrawColor(drawMode) ;
	m_color = old ;
	glLineWidth(1.5f) ;

	glBegin(GL_LINES) ;
	for( i = 0 ; i < m_facet->ntria ; i++ )
	{
		I = m_facet->trias[i][0] ;
		J = m_facet->trias[i][1] ;
		K = m_facet->trias[i][2] ;
		a = mathOProduct(m_facet->nodes[I].normal, viewDir) ;
		b = mathOProduct(m_facet->nodes[J].normal, viewDir) ;
		c = mathOProduct(m_facet->nodes[K].normal, viewDir) ;
		if( mathEvalTriContLin(3, // cal attrib line on triangle 
							   m_facet->nodes[I].p,
							   m_facet->nodes[J].p,
							   m_facet->nodes[K].p,
							   a,
							   b,
							   c,
							   1.e-3,
							   1.e-5,
							   c1,
							   c2) )
		{
			glVertex3dv(c1) ;
			glVertex3dv(c2) ;
		}
	}
	glEnd() ;

	return 1 ;
}

int	CRefSur::Draw(void* pVI, int drawMode)
{
	DrawSurface(pVI, drawMode) ;

	if( drawMode != dmWireframe )
	{
		// nt add 2011/7/3
		DrawBorder(pVI, drawMode) ;	
		// nt add 2011/7/6
		if( m_atts&2 )
			DrawUV(pVI, drawMode) ;
	}

	return 1 ;
}

// nt add 2010/1/19
int	CRefSur::Draw2(void* pVI, int drawMode)
{
	if( drawMode == dmNormal )
	{
		glEnable(GL_COLOR_LOGIC_OP) ;
		glLogicOp(GL_XOR) ;
		glEnable(GL_LINE_STIPPLE) ;
		glLineStipple(1, 0x0F0F) ;
		DrawBorder(pVI, dmSelect) ;
		DrawContour(pVI, dmSelect) ;
		glDisable(GL_LINE_STIPPLE) ;
		glDisable(GL_COLOR_LOGIC_OP) ;

		viLighting(pVI, 1) ;
		viAddDefaultLight(pVI) ;	
		DrawSurface(pVI, drawMode) ;
		//DrawBorder(pVI, drawMode) ;
		viLighting(pVI, 0) ;
	}
	else
	if( drawMode == dmSelect )
	{
		viLighting(pVI, 1) ;
		viAddDefaultLight(pVI) ;	
		DrawSurface(pVI, drawMode) ;
		viLighting(pVI, 0) ;

		glEnable(GL_COLOR_LOGIC_OP) ;
		glLogicOp(GL_XOR) ;
		glEnable(GL_LINE_STIPPLE) ;
		glLineStipple(1, 0x0F0F) ;
		DrawBorder(pVI, dmSelect) ;
		DrawContour(pVI, dmSelect) ;
		glDisable(GL_LINE_STIPPLE) ;
		glDisable(GL_COLOR_LOGIC_OP) ;
	}

	return 1 ;
}

// nt add 2010/5/22
// nt rewrite 2010/11/27 draw mark ’u‘ and ’v‘
int CRefSur::DrawUV(void* pVI, int drawMode)
{
	double d = viGetScale(pVI)*30 ;
	PNT3D p ;
	VEC3D du, dv ;
	CS cs ;
	cs.scale = 1. ;
	surfaceEvalDer1O(m_surface,
						0.5*(m_boxuv.min[0]+m_boxuv.max[0]),
						0.5*(m_boxuv.min[1]+m_boxuv.max[1]),
						p,
						du,
						dv) ;
	// nt add 2011/7/3
	RFRAME lf ;
	viGetCenterLF(pVI, lf.X, lf.Y, lf.Z, lf.O) ;
	double dd = 4900. ;
	lf.O[0] += dd*lf.Z[0] ;
	lf.O[1] += dd*lf.Z[1] ;
	lf.O[2] += dd*lf.Z[2] ;
	mathPrjPntPln(p, lf.O, lf.Z, p) ;
	// end
	mathUniVec(du, MIN_DBL) ;
	mathUniVec(dv, MIN_DBL) ;
	memcpy(cs.X, du, sizeof(VEC3D)) ;
	memcpy(cs.Y, dv, sizeof(VEC3D)) ;
	mathVProduct(cs.X, cs.Y, cs.Z) ;
	mathUniVec(cs.Z, MIN_DBL) ;
	mathVProduct(cs.Z, cs.X, cs.Y) ;

	SetDrawColor2(drawMode) ;
	glLineWidth(1.f) ;
	mathEvalLin(p, du, 0.5*d, cs.O) ;
	glBegin(GL_LINES) ;
		glVertex3dv(p) ;
		glVertex3dv(cs.O) ;
	glEnd() ;
	glColor3ub(255, 0, 0) ;
	viTextOut3D(pVI, &cs, "U", d) ;
	mathEvalLin(p, dv, d, cs.O) ;
	glBegin(GL_LINES) ;
		glVertex3dv(p) ;
		glVertex3dv(cs.O) ;
	glEnd() ;
	viTextOut3D(pVI, &cs, "V", d) ;

	return 1 ;
}
//--------------------------------------------------------------
int	CRefSur::Pick(PINFO* pi)
{
	int picked ;
	double u, v, z ;

	if( !m_facet )
		return 0 ;

	// nt add 2001/11/28
	if( mathChkBoxLinInt(&m_box, pi->pos, pi->dir, pi->r) == IDNOINT )
		return 2 ;

	if( facetPick(m_facet,
				  pi->pos,
				  pi->dir,
				  pi->r,
				  &z,
				  &u, 
				  &v,
				  &picked) != ERSUCSS )
		return 0 ;
	if( picked == ID_YES &&
		(pi->obj == NULL ||
		pi->z > z) )
	{
		pi->ot = ftRefSur ; // nt add 2003/7/22
		pi->obj = this ;
		pi->z = z ;
		pi->u = u ;
		pi->v = v ;
		m_u = u ; // nt add 2010/4/22
		m_v = v ; // nt add 2010/4/22
		pi->att = 0 ; // nt add 2011/7/6
		return 2 ; // nt add 2003/7/22
	}

	return 2 ;
}

// nt add 2010/4/22
int CRefSur::PickByBox(RFRAME* pLF, BOX2D* pLB, double r)
{
	int i ;
	PNT3D p ;

	if( m_facet == NULL ||
		m_facet->nnode < 1 )
		return 0 ;

	for( i = 0 ; i < m_facet->nnode ; i++ )
	{
		mathTransWorldPnt3D(pLF, m_facet->nodes[i].p, p) ;
		if( pLB->min[0]-r < p[0] &&
			p[0] < pLB->max[0]+r &&
			pLB->min[1]-r < p[1] &&
			p[1] < pLB->max[1]+r )
			return 1 ;
	}

	return 2 ;
}
//--------------------------------------------------------------
int	CRefSur::Serialize(CArchive& ar)
{
	int type, flag, i ;

	CFeature::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		ar << m_dtype ;
		ar.Write(&m_boxuv, sizeof(BOXUV)) ;
		ar.Write(&m_box, sizeof(BOX3D)) ;
		type = entityGetType(m_surface) ;
		ar << type ;
		surfaceSerialize(m_surface, 0, &ar, arReadWrite) ; // nt modify 2020/4/11
		if( m_dtype == -1 ) // nt add 2010/5/20, for trim surface
		{
			if( m_facet )
			{
				ar << (flag=1) ;
				ar << m_facet->nnode << m_facet->ntria ;
				ar.Write(m_facet->nodes, sizeof(FNODE)*m_facet->nnode) ;
				ar.Write(m_facet->trias, sizeof(int[3])*m_facet->ntria) ;
			}
			else
				ar << (flag=0) ;
		}

		// nt add 2021/8/13
		ar << m_n ;
		if( m_n > 0 )
		{
			ar.Write(m_bnds, sizeof(BNDX)*m_n) ;
			for( i = 0 ; i < m_n ; i++ )
			{
				if( m_bnds[i].curve )
				{
					type = entityGetType(m_bnds[i].curve) ;
					ar << type ;
					curveSerialize(m_bnds[i].curve, 0, &ar, arReadWrite) ;
				}
				if( m_bnds[i].disc )
					strptSave(m_bnds[i].disc, &ar, arReadWrite) ;
			}
		}
	}
	else
	{
		ar >> m_dtype ;
		ar.Read(&m_boxuv, sizeof(BOXUV)) ;
		ar.Read(&m_box, sizeof(BOX3D)) ;
		ar >> type ;
		m_surface = entityAlloc(type) ;
		surfaceSerialize(m_surface, 1, &ar, arReadWrite) ; // nt add 2020/4/11
		if( m_dtype == -1 ) // nt add 2010/5/20, for trim surface
		{
			ar >> flag ;
			if( flag == 1 )
			{
				int nn, mm ;
				ar >> nn >> mm ;
				m_facet = facetCreate(nn, mm, 0) ;
				ar.Read(m_facet->nodes, sizeof(FNODE)*m_facet->nnode) ;
				ar.Read(m_facet->trias, sizeof(int[3])*m_facet->ntria) ;
			}
		}
		if( m_facet == NULL )
			Disc(1., 0.15) ;
		m_n = 0 ; // nt add 2021/6/19
		m_bnds = NULL ; // nt add 2021/6/19

		// nt add 2021/8/13
		int docVerNo = camGetDocVerNo() ;
		if( docVerNo >= 6 )
		{
			ar >> m_n ;
			if( m_n > 0 )
			{
				m_bnds = new BNDX[m_n] ;
				ar.Read(m_bnds, sizeof(BNDX)*m_n) ;
				for( i = 0 ; i < m_n ; i++ )
				{
					if( m_bnds[i].curve )
					{
						ar >> type ;
						m_bnds[i].curve = entityAlloc(type) ;
						curveSerialize(m_bnds[i].curve, 1, &ar, arReadWrite) ;
					}
					if( m_bnds[i].disc )
						m_bnds[i].disc = strptLoad(&ar, arReadWrite) ;
				}
			}
		}
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
