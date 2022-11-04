#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\API\IGES_API.h"
#include "IgesImport.h"

////////////////////////////////////////////////////////////////
// IGES import
CIgesImport::CIgesImport(int flags)
{
	m_flags = flags ; // 缺省只读裁剪曲面
	m_iges = NULL ;
	m_max = 100 ;
	m_deep = 0 ;
	m_tol = 1.e-4 ; // nt add 2010/5/20, nt modify 2010/11/7 old is 1.e-6
}

CIgesImport::~CIgesImport()
{
	int i, n = m_fs.GetSize() ;
	for( i = 0 ; i < n ; i++ )
		delete (CFeature*)m_fs[i] ;
	m_fs.RemoveAll() ;
	if( m_iges )
	{
		igesFree(m_iges) ;
		m_iges = NULL ;
	}

	// nt add 2021/3/6
	n = m_errBnds.GetSize() ;
	for( i = 0 ; i < n ; i++ )
		delete (CFeature*)m_errBnds[i] ;
	m_errBnds.RemoveAll() ;

	return ;
}
//--------------------------------------------------------------
int CIgesImport::GetMax()
{
	return m_max ;
}

void CIgesImport::SetMax(int max)
{
	m_max = max ;
}

int CIgesImport::GetNumOfFeature()
{
	return m_fs.GetSize() ;
}

CFeature* CIgesImport::GetFeature(int i)
{
	if( i >= 0 &&
		i < m_fs.GetSize() )
		return (CFeature*)m_fs[i] ;
	else
		return NULL ;
}

// nt add 2010/5/19
void CIgesImport::ClearFeatures()
{
	m_fs.RemoveAll() ;
}
//--------------------------------------------------------------
// nt add 2010/5/20
// return 1=space curve
// return 0=curve defined in parametric domain
int ientityIsSpaceCur(IENTITY* pE)
{
	DE de ;
	ientityGetDE(pE, &de) ;
	if( de.status[4] == '0' &&
		de.status[5] == '5' )
		return 0 ;
	else
		return 1 ;
}

// nt add 2010/5/8
// 根据m_flags的设置按需导入IGES的几何元素
// nt modify 2012/11/30 add sp
int CIgesImport::Import(CString& fileName)
{
	int debug_count = 0, nFail = 0 ; // nt add 2021/3/5
	if( m_iges )
		igesFree(m_iges) ;
	m_iges = igesCreate() ;
	if( !m_iges )
		return 0 ; // error
	int rt = igesImport(m_iges, fileName.GetBuffer(0)), nTS = 0 ;
	if( rt )
	{
		int pos ; // nt add 2010/11/23
		PNT3D p ;
		IENTITY* pE, *pB = NULL ;
		CURVE* curve ;
		INTERVAL interval ;
		SURFACE* surface ; // nt add 2010/11/23
		BOXUV boxuv ; // nt add 2010/11/23
		CFeature* pF = NULL ;
		TFACE* tface = NULL ;
		igesInitInParam(m_iges) ; // nt add 2021/3/23, before calling iges2MM()
		iges2MM(m_iges) ; // nt add 2011/11/15
		int i, type, n = igesGetNumOfEntity(m_iges), sp ;
		//for( i = 0 ; i < n ; i++ )
		for( i = n-1 ; i >= 0 ; i-- )
		{
			pE = igesGetEntity(m_iges, i) ;
			if( pE &&
				ientityIsOK(pE) == 1 )
			{
				pF = NULL ;
				type = ientityGetType(pE) ;
				switch(type)
				{
				case 116: // point
					if( m_flags&1 )
					{
						ientity2Point(pE, p) ;
						pF = new CRefPt(0, p) ; // create a CRefPt
					}
					break ;
				case 100: // circular arc
				case 110: // line
				case 112: // parametric spline curve
				case 126: // b-spline curve
				case 104: // ellipse...
				case 130: // offset curve...
					if( m_flags&2 &&
						ientityIsUsed(pE) != 1 && // nt add 2015/5/23
						ientityIsSpaceCur(pE) == 1 )
					{
						curve = ientity2Curve(pE, &interval) ;
						if( curve )
							pF = new CRefCur(0, curve, &interval) ;
					}
					break ;
				//case 108: // nt add 2011/11/16
				case 118: // nt add 2011/11/16
				case 120: // nt add 2011/11/16
				case 122: // nt add 2011/11/16
				case 128: // nt add 2010/11/23
				case 140: // nt add 2011/11/16
					if( ientityIsUsed(pE) != 1 ) // nt modify 2011/11/16
					{
						surface = ientity2Surface(pE, &boxuv) ;
						pF = new CRefSur(0, surface, &boxuv) ;
					}
					break ;
				case 144: // nt modify 2010/11/15
					debug_count++ ; // ---debug only
					//dump("d:\\1.txt", debug_count) ;
					if( debug_count == 57 ) // ---debug only
						debug_count = debug_count ; // ---debug only
					if( m_flags&4 )
					{
						for( sp = 1 ; sp >= 0 ; sp-- )
						{
							// 注意下面返回的tsur中的boxuv是曲面的自然参数域!!!!
							tface = ientity2TFace(pE, sp) ; // 在空间或参数域剖分
							if( tface &&
								tface->n > 0 ) // nt add 2021/3/22
							{
								pF = new CRefSur(tface, 0.5, PI1/3) ;
								nTS++ ; // nt add 2020/3/4
								if( ((CRefSur*)pF)->GetFacet() )
								{
									pos = tsurGetBase(pE) ;
									pB = igesGetEntity(m_iges, pos/2) ;
									ientityUsed(pB) ; // nt add 2011/11/16
									break ;
								}
								else
								{
									//((CRefSur*)pF)->Disc(0.5, 0.15) ; // -------------------debug only
									//m_fs.Add(pF) ; // -----------------------debug only
									//if( debug_count == 36 ) // --------------------------debug only
									tfaceCopyFinAll(tface, m_errBnds) ; // nt add 2021/3/6
									//break ; // ---------------------------debug only
									delete pF ;
									pF = NULL ; // nt add 2013/4/7
									if( sp == 0 ) // nt add 2021/3/24
										nFail++ ; // nt add 2021/3/24
								}
								tface->surface = NULL ;
								tfaceFree(tface) ;
								tface = NULL ;
							}
						}
					}
					break ;
				default:
					break ;
				}
				if( pF )
					m_fs.Add(pF) ;
			}
		}
	}
	igesFree(m_iges) ;
	m_iges = NULL ;

	return rt ;
}

int CIgesImport::Import3(CString& fileName)
{
	int debug_count = 0, nFail = 0, pos ; // nt add 2021/3/5
	if( m_iges )
		igesFree(m_iges) ;
	m_iges = igesCreate() ;
	if( !m_iges )
		return 0 ; // error
	int rt = igesImport(m_iges, fileName.GetBuffer(0)), nTS = 0 ;
	if( rt )
	{
		double qTol = 0.1 ; // nt add 2021/8/4
		PNT3D p ;
		IENTITY* pE, *pB = NULL ;
		CURVE* curve ;
		INTERVAL interval ;
		SURFACE* surface ; // nt add 2010/11/23
		BOXUV boxuv ; // nt add 2010/11/23
		CFeature* pF = NULL ;
		TFACE* tface = NULL ;
		TSM* tsm = tsmCreate() ; // nt add 2021/8/4
		igesInitInParam(m_iges) ; // nt add 2021/3/23, before calling iges2MM()
		iges2MM(m_iges) ; // nt add 2011/11/15
		int i, type, n = igesGetNumOfEntity(m_iges) ;
		//for( i = 0 ; i < n ; i++ )
		for( i = n-1 ; i >= 0 ; i-- )
		{
			pE = igesGetEntity(m_iges, i) ;
			if( pE &&
				ientityIsOK(pE) == 1 )
			{
				pF = NULL ;
				type = ientityGetType(pE) ;
				switch(type)
				{
				case 116: // point
					if( m_flags&1 )
					{
						ientity2Point(pE, p) ;
						pF = new CRefPt(0, p) ; // create a CRefPt
					}
					break ;
				case 100: // circular arc
				case 110: // line
				case 112: // parametric spline curve
				case 126: // b-spline curve
				case 104: // ellipse...
				case 130: // offset curve...
					if( m_flags&2 &&
						ientityIsUsed(pE) != 1 && // nt add 2015/5/23
						ientityIsSpaceCur(pE) == 1 )
					{
						curve = ientity2Curve(pE, &interval) ;
						if( curve )
							pF = new CRefCur(0, curve, &interval) ;
					}
					break ;
				//case 108: // nt add 2011/11/16
				case 118: // nt add 2011/11/16
				case 120: // nt add 2011/11/16
				case 122: // nt add 2011/11/16
				case 128: // nt add 2010/11/23
				case 140: // nt add 2011/11/16
					if( ientityIsUsed(pE) != 1 ) // nt modify 2011/11/16
					{
						surface = ientity2Surface(pE, &boxuv) ;
						pF = new CRefSur(surface, &boxuv) ;
					}
					break ;
				case 144: // nt modify 2010/11/15
					debug_count++ ; // ---debug only
					//dump("d:\\1.txt", debug_count) ;
					if( debug_count == 57 ) // ---debug only
						debug_count = debug_count ; // ---debug only
					if( m_flags&4 )
					{
						tface = ientity2TFace(pE, 1) ; // 在空间或参数域剖分
						tsmAddFace(tsm, tface, qTol) ;
						pos = tsurGetBase(pE) ;
						pB = igesGetEntity(m_iges, pos/2) ;
						ientityUsed(pB) ;
					}
					break ;
				default:
					break ;
				}
				if( pF )
					m_fs.Add(pF) ;
			}
		}

		int nFail = 0 ;
		OPTION* option = camGetOption() ; // nt add 2021/6/15
		tsmEstmChd(tsm, option->dTol, option->maxEdge, option->minEdge) ;
		tsmDiscEdge(tsm, qTol, option->dTol, option->maxEdge, option->minEdge) ;
		tsmCorrChd(tsm, option->minEdge, option->maxEdge) ; // nt add 2021/8/8
		tsmTess(tsm) ; 
		for( tface = tsm->tfaces ; tface ; tface = tface->next )
		{
			if( tface->facet )
			{
				pF = new CRefSur(tface) ;
				m_fs.Add(pF) ;
			}
			else
				nFail++ ;
		}
		tsmFree(tsm) ;
	}
	igesFree(m_iges) ;
	m_iges = NULL ;

	return rt ;
}
//--------------------------------------------------------------
// nt modify 2010/11/7 use transformation matrix
int CIgesImport::ientity2Point(IENTITY* pE, PNT3D p)
{
	pointGetP(pE, p) ;

	// nt add 2010/11/7
	int pos = ientityGetMatrix(pE) ;
	if( pos > 0 )
	{
		IENTITY* pM = igesGetEntity(m_iges, pos/2) ;
		RFRAME lf ;
		lf.scale = 1. ;
		matrixGet2(pM, lf.O, lf.X, lf.Y, lf.Z) ;
		PNT3D q ;
		memcpy(q, p, sizeof(PNT3D)) ;
		mathTransLocalPnt3D(&lf, q, p) ;
	}

	return 1 ;
}

// nt modify 2010/11/7
CURVE* CIgesImport::ientity2Curve(IENTITY* pE, INTERVAL* interval)
{
	if( !pE ||
		!interval )
		return NULL ;
	int type = ientityGetType(pE) ;
	CURVE* curve = NULL ;
	switch(type)
	{
	case 100: // circular arc
		curve = ientity2Arc(pE, interval) ;
		break ;
	case 110: // line
		curve = ientity2Line(pE, interval) ;
		break ;
	//case 112: // parametric spline curve
	//	curve = ientity2BScur2(pE, interval) ;
	//	break ;
	case 126: // b-spline curve
		curve = ientity2BSCur(pE, interval) ;
		break ;
	case 104: // ellipse...
		curve = ientity2Conic(pE, interval) ;
		break ;
	case 130: // offset curve...
		curve = ientity2OffCur(pE, interval) ;
		break ;
	default:
		break ;
	}

	// nt add 2010/11/7
	if( curve )
	{
		// nt add 2010/11/7
		int pos = ientityGetMatrix(pE) ;
		if( pos > 0 )
		{
			IENTITY* pM = igesGetEntity(m_iges, pos/2) ;
			RFRAME lf ;
			lf.scale = 1. ;
			matrixGet2(pM, lf.O, lf.X, lf.Y, lf.Z) ;
			geometryToWorld(curve, &lf) ;
		}
	}

	return curve ;
}

// nt modify 2010/11/7
SURFACE* CIgesImport::ientity2Surface(IENTITY* pE, BOXUV* boxuv)
{
	if( !pE ||
		!boxuv )
		return NULL ;
	int type = ientityGetType(pE) ;
	SURFACE* surface = NULL ;
	switch(type)
	{
	case 108: // plane
		surface = ientity2Plane(pE, boxuv) ;
		break ;
	case 118: // ruled surface
		surface = ientity2Rule2(pE, boxuv) ;
		break ;
	case 120: // revolved surface
		surface = ientity2Revolve(pE, boxuv) ;
		break ;
	case 122: // ruled surface
		surface = ientity2Rule(pE, boxuv) ;
		break ;
	case 128: // b-spline surface
		surface = ientity2BSSur(pE, boxuv) ;
		break ;
	case 140: // offset surface
		surface = ientity2OffSur(pE, boxuv) ;
		break ;
	default:
		break ;
	}

	// nt add 2010/11/7
	if( surface )
	{
		// nt add 2010/11/7
		int pos = ientityGetMatrix(pE) ;
		if( pos > 0 )
		{
			IENTITY* pM = igesGetEntity(m_iges, pos/2) ;
			RFRAME lf ;
			lf.scale = 1. ;
			matrixGet2(pM, lf.O, lf.X, lf.Y, lf.Z) ;
			geometryToWorld(surface, &lf) ;
		}
	}

	return surface ;
}
//--------------------------------------------------------------
// nt add 2010/5/18
// 144 to TSUR
// 把IGES的裁剪曲面存到TSUR数据结构中
// 如果环有问题，就返回一个非裁剪的曲面
// nt modify 2010/5/21 : add sp
// nt modify 2021/9/8 IGES PLANE has no parameter domain
TFACE* CIgesImport::ientity2TFace(IENTITY* pE, int sp)
{
	int n1, n2, n, i, k, pos, type ;
	SURFACE* base = NULL ;
	IENTITY* pE2 = NULL ;
	TFACE* tface = NULL ;
	BOXUV boxuv ;

	pos = tsurGetBase(pE) ;
	pE2 = igesGetEntity(m_iges, pos/2) ;
	if( sp == 0 ) // nt add 2021/9/8 tess in parameter domain
	{
		type = ientityGetType(pE2) ;
		if( type == 108 )
			goto ExceExit ;
	}
	base = ientity2Surface(pE2, &boxuv) ; // get base surface
	if( !base )
		goto ExceExit ;
	// nt add 2021/5/6
	if( sp == 1 ) // nt modify 2021/6/19, 仅针对B样条曲面情况修改
		surfaceModifyKVec01(base) ;
	// end 2021/5/6
	n1 = tsurGetN1(pE) ;
	n2 = tsurGetN2(pE) ;
	if( n1 < 0 ||
		n1 > 1 ||
		n2 < 0 )
		goto ExceExit ;
	n = n1+n2 ;
	//n = 0 ;
	tface = tfaceCreate(base, &boxuv, n, sp) ;
	for( i = 0 ; i < n ; i++ ) // 取出所有的内环与外环:TLOOPs
	{
		if( i < n1 )
			pos = tsurGetOuter(pE) ; // 外环
		else
			pos = tsurGetInner(pE, i-n1) ; // 内环
		pE2 = igesGetEntity(m_iges, pos/2) ;
		// pE2类型都是"curve on parametric surface"
		if( ientity2TLoop(pE2, &(tface->tloops[i]), sp) != 1 )
		{
			for( k = 0 ; k <= i ; k++ )
				tloopFreeFins(&(tface->tloops[k])) ;
			delete []tface->tloops ;
			tface->n = 0 ;
			tface->tloops = NULL ;
			return tface ;
		}
	}

	return tface ;
ExceExit:
	if( base )
		entityFree(base) ;
	return NULL ;
}

// nt add 2010/5/18
// 把IGES的曲面上的曲线存到TLOOP数据结构中
// 曲面只取其中的空间曲面信息，不取参数域上
// 曲线的信息
// nt modify 2010/5/21 : add sp
int CIgesImport::ientity2TLoop(IENTITY* pE, TLOOP* tloop, int sp)
{
	int pos ;
	CURVE* curve ;
	if( sp )
		pos = pcurveGetCurve(pE) ; // in space
	else
		pos = pcurveGetBnd(pE) ; // in parameter domain
	if( pos == 0 ) // nt add 2021/3/22
		return 0 ;
	IENTITY* pE2 = igesGetEntity(m_iges, pos/2) ; // 一条,若干条曲线
	if( !pE2 )
		return 0 ;
	int type = ientityGetType(pE2) ;
	INTERVAL interval ;
	if( type == 102 ) // 组合曲线=若干条
	{
		int n = compGetN(pE2) ;
		tloopAllocFins(tloop, n) ;
		for( int i = 0 ; i < n ; i++ ) // 组合曲线按IGES要求首尾相连
		{
			pos = compGetCurve(pE2, i) ;
			IENTITY* pE3 = igesGetEntity(m_iges, pos/2) ;
			curve = ientity2Curve(pE3, &interval) ;
			if( curve == NULL ) // nt add 2011/11/17
				return 0 ;
			tloop->tfins[i].curve = curve ;
			tloop->tfins[i].interval = interval ;
		}
	}
	else // 非组合曲面=一条
	{
		tloopAllocFins(tloop, 1) ;
		curve = ientity2Curve(pE2, &interval) ;
		if( curve == NULL ) // nt add 2011/11/17
			return 0 ;
		tloop->tfins[0].curve = curve ;
		tloop->tfins[0].interval = interval ;
	}

	return 1 ;
}
//--------------------------------------------------------------
// nt rewrite 2010/11/7 使参数域为[0,1]
CURVE* CIgesImport::ientity2Line(IENTITY* pE, INTERVAL* interval)
{
	if( m_deep++ > m_max )
		return 0 ;
	int sp = ientityIsSpaceCur(pE) ;
	PNT3D b, e ;
	lineGetB(pE, b) ;
	lineGetE(pE, e) ;
	// nt add 2010/11/7
	BSCUR* nurbs = bscurCreate(2, 2) ;
	memcpy(nurbs->u_p[0], b, sizeof(PNT3D)) ;
	memcpy(nurbs->u_p[1], e, sizeof(PNT3D)) ;
	CURVE* curve = bcurveCreate(nurbs) ;
	bscurFree(nurbs) ;
	interval->t1 = 0. ;
	interval->t2 = 1. ;
	//VEC3D dir ;
	//dir[0] = e[0]-b[0] ;
	//dir[1] = e[1]-b[1] ;
	//dir[2] = e[2]-b[2] ;
	//if( mathUniVec(dir, MIN_LEN) != ERSUCSS )
	//	return NULL ;
	//CURVE* curve = lineCreate(b, dir) ;
	//interval->t1 = 0. ;
	//interval->t2 = mathDist(b, e) ;
	m_deep-- ;
	return curve ;
}

// nt modify 2010/11/7
CURVE* CIgesImport::ientity2Arc(IENTITY* pE, INTERVAL* interval)
{
	double r, r1, r2 ;
	PNT2D b, e ;
	VEC3D v ;
	RFRAME lf ;
	if( m_deep++ > m_max )
		return NULL ;
	int sp = ientityIsSpaceCur(pE) ;
	mathInitRFrame(&lf) ;
	arcGetCenter(pE, lf.O) ;
	lf.O[2] = arcGetZ(pE) ;
	//if( sp )
	//	toM(lf.O) ;
	arcGetStart(pE, b) ;
	//if( sp )
	//	toM(b) ;
	arcGetEnd(pE, e) ;
	//if( sp )
	//	toM(e) ;
	r1 = mathDist2D(b, lf.O) ;
	r2 = mathDist2D(e, lf.O) ;
	if( fabs(r1-r2) > m_tol )
		return NULL ;
	r = 0.5*(r1+r2) ;
	v[0] = (b[0]-lf.O[0])/r ; // nt add r 2010/11/7
	v[1] = (b[1]-lf.O[1])/r ; // nt add r 2010/11/7
	v[2] = 0. ; // nt add 2010/11/7
	interval->t1 = mathGetAngleUnit(v, lf.X) ;
	if( v[1] < 0. )
		interval->t1 = PI2-interval->t1 ; // 计算与X轴正向的夹角
	v[0] = (e[0]-lf.O[0])/r ; // nt add r 2010/11/7
	v[1] = (e[1]-lf.O[1])/r ; // nt add r 2010/11/7
	v[2] = 0. ; // nt add 2010/11/7
	interval->t2 = mathGetAngleUnit(v, lf.X) ;
	if( v[1] < 0. )
		interval->t2 = PI2-interval->t2 ; // 计算与X轴正向的夹角
	if( interval->t2 < interval->t1+MIN_ANG )
		interval->t2 += PI2 ;
	CURVE* curve = circleCreate(&lf, r) ;
	m_deep-- ;
	return curve ;
}

// 含ellipse
// nt modify 2011/11/17
CURVE* CIgesImport::ientity2Conic(IENTITY* pE, INTERVAL* interval)
{
	double a, b, abcdef[6], A, B, C, D, E, F, dd ;
	PNT2D start, end ;
	RFRAME lf ;
	CURVE* curve = NULL ;
	if( m_deep++ > m_max )
		return 0 ;
	int sp = ientityIsSpaceCur(pE) ;
	mathInitRFrame(&lf) ;
	lf.O[2] = conicGetZ(pE) ;
	conicGetStart(pE, start) ;
	conicGetEnd(pE, end) ;
	conicGetABCDEF(pE, abcdef) ;
	A = abcdef[0] ;
	B = abcdef[1] ;
	C = abcdef[2] ;
	D = abcdef[3] ;
	E = abcdef[4] ;
	F = abcdef[5] ;
	
	// 目前只处理标准椭圆的情况
	dd = A*C-0.25*B*B ;
	if( fabs(A) > MIN_DBL &&
		fabs(C) > MIN_DBL &&
		dd > MIN_DBL )
	{
		a = sqrt(fabs(-F/A)) ;
		b = sqrt(fabs(-F/C)) ;
		curve = ellipseCreate(&lf, a, b) ;
		interval->t1 = mathSolAngleByCS(start[0]/a, start[1]/b) ;
		interval->t2 = mathSolAngleByCS(end[0]/a, end[1]/b) ;
		if( interval->t2 < interval->t1+MIN_ANG )
			interval->t2 += PI2 ;
	}
	m_deep-- ;
	return curve ;
}

// nt modify 2010/11/15 : 修改了参数域
CURVE* CIgesImport::ientity2BSCur(IENTITY* pE, INTERVAL* interval)
{
	if( m_deep++ > m_max )
		return 0 ;
	int sp = ientityIsSpaceCur(pE) ;
	int cnt_u = bscurGetCount(pE) ;
	int ord_u = bscurGetOrder(pE) ;
	BSCUR* nurbs = bscurCreate(ord_u, cnt_u) ;
	int i ;
	for( i = 0 ; i < ord_u+cnt_u ; i++ )
	{
		nurbs->u_k[i] = bscurGetKnot(pE, i) ;
	}
	// nt modify 2021/5/5
	double b = nurbs->u_k[0] ;
	double d = nurbs->u_k[ord_u+cnt_u-1]-b ;
	if( d > MIN_DBL )
	{
		for( i = 0 ; i < ord_u+cnt_u ; i++ )
		{
			nurbs->u_k[i] = (nurbs->u_k[i]-b)/d ;
		}
	}
	else
	{
		bscurFree(nurbs) ;
		m_deep-- ;
		return NULL ;
	}
	// end modify 2021/5/5
	interval->t1 = nurbs->u_k[ord_u-1] ; // nt modify 2010/11/15
	interval->t2 = nurbs->u_k[cnt_u] ; // nt modify 2010/11/15
	for( i = 0 ; i < cnt_u ; i++ )
	{
		bscurGetCP(pE, i, nurbs->u_p[i]) ;
		//if( sp )
		//	toM(nurbs->u_p[i]) ;
	}
	CURVE* curve = bcurveCreate(nurbs) ;
	m_deep-- ;
	return curve ;
}

CURVE* CIgesImport::ientity2OffCur(IENTITY* pE, INTERVAL* interval)
{
	int pos, flag ;
	double d, ts[2] ;
	double normal[3] ;
	IENTITY* pE2 = NULL ;
	INTERVAL intv ;
	CURVE* base = NULL,
		   *offcur = NULL ;
	if( m_deep++ > m_max )
		return 0 ;
	int sp = ientityIsSpaceCur(pE) ;
	pos = offcurGetBase(pE) ;
	pE2 = igesGetEntity(m_iges, pos/2) ;
	base = ientity2Curve(pE2, &intv) ;
	if( !base )
		goto ExceExit ;
	flag = offcurGetFlag(pE) ;
	if( flag != 1 ) // 目前只支持flag=1的情况
		goto ExceExit ;
	d = compGetDisCur(pE) ;
	//if( sp )
	//	d *= 0.001 ; // distance
	offcurGetNormal(pE, normal) ;
	offcur = offcurCreate(base, normal, d) ; 
	offcurGetTs(pE, ts) ; // parameter interval
	interval->t1 = ts[0] ;
	interval->t2 = ts[1] ;

ExceExit:
	if( base )
		entityFree(base) ;
	m_deep-- ;
	return offcur ;
}
//--------------------------------------------------------------
SURFACE* CIgesImport::ientity2Plane(IENTITY* pE, BOXUV* boxuv)
{
	double abcd[4] ;
	planeGetABCD(pE, abcd) ;
	if( m_deep++ > m_max )
		return 0 ;
	double d = sqrt(abcd[0]*abcd[0]+abcd[1]*abcd[1]+abcd[2]*abcd[2]) ;
	abcd[0] /= d ;
	abcd[1] /= d ;
	abcd[2] /= d ;
	RFRAME lf ;
	lf.scale = 1. ;
	memcpy(lf.Z, abcd, sizeof(double[3])) ;
	mathGetXYFromZ(lf.Z, lf.X, lf.Y) ;
	lf.O[0] = abcd[0]*abcd[3] ;
	lf.O[1] = abcd[1]*abcd[3] ;
	lf.O[2] = abcd[2]*abcd[3] ;
	//toM(lf.O) ;
	SURFACE* plane = planeCreate(&lf) ;
	boxuv->min[0] = -MAX_DBL ;
	boxuv->min[1] = -MAX_DBL ;
	boxuv->max[0] = MAX_DBL ;
	boxuv->max[1] = MAX_DBL ;
	m_deep-- ;
	return plane ;
}

// nt modify 2010/11/15 : 修改了参数域
SURFACE* CIgesImport::ientity2BSSur(IENTITY* pE, BOXUV* boxuv)
{
	if( m_deep++ > m_max )
		return 0 ;
	int cnt_u = bssurGetCountU(pE) ;
	int ord_u = bssurGetOrderU(pE) ;
	int cnt_v = bssurGetCountV(pE) ;
	int ord_v = bssurGetOrderV(pE) ;
	BSSUR* nurbs = bssurCreate(ord_u, cnt_u, ord_v, cnt_v) ;
	int i, j ;
	for( i = 0 ; i < ord_u+cnt_u ; i++ )
	{
		nurbs->u_k[i] = bssurGetKnotU(pE, i) ;
	}
	for( i = 0 ; i < ord_v+cnt_v ; i++ )
	{
		nurbs->v_k[i] = bssurGetKnotV(pE, i) ;
	}
	// nt modify 2021/5/5
	/*double b = nurbs->u_k[0] ;
	double d = nurbs->u_k[ord_u+cnt_u-1]-b ;
	if( d > MIN_DBL )
	{
		for( i = 0 ; i < ord_u+cnt_u ; i++ )
		{
			nurbs->u_k[i] = (nurbs->u_k[i]-b)/d ;
		}
	}
	else
	{
		bssurFree(nurbs) ;
		m_deep-- ;
		return NULL ;
	}
	b = nurbs->v_k[0] ;
	d = nurbs->v_k[ord_v+cnt_v-1]-b ;
	if( d > MIN_DBL )
	{
		for( i = 0 ; i < ord_v+cnt_v ; i++ )
		{
			nurbs->v_k[i] = (nurbs->v_k[i]-b)/d ;
		}
	}
	else
	{
		bssurFree(nurbs) ;
		m_deep-- ;
		return NULL ;
	}*/
	// end modify 2021/5/5
	boxuv->min[0] = nurbs->u_k[ord_u-1] ; // nt modify 2010/11/15
	boxuv->min[1] = nurbs->v_k[ord_v-1] ; // nt modify 2010/11/15
	boxuv->max[0] = nurbs->u_k[cnt_u] ; // nt modify 2010/11/15
	boxuv->max[1] = nurbs->v_k[cnt_v] ; // nt modify 2010/11/15
	//boxuv->min[0] = 0. ;
	//boxuv->min[1] = 0. ;
	//boxuv->max[0] = 1. ;
	//boxuv->max[1] = 1. ;
	for( i = 0 ; i < cnt_u ; i++ )
	{
		for( j = 0 ; j < cnt_v ; j++ )
		{
			bssurGetCP(pE, i, j, nurbs->uv_p[i][j]) ;
			//toM(nurbs->uv_p[i][j]) ;
		}
	}
	SURFACE* surface = bsurfaceCreate(nurbs) ;
	m_deep-- ;
	return surface ;
}

// nt add 2010/5/16
// nt modify 2021/5/7 最终生成的直纹面参数域为[0,1]x[0,1]
// 注意：没有处理Form_Num=0（按弧长参数化）情况，按Form_Num=1处理的
SURFACE* CIgesImport::ientity2Rule2(IENTITY* pE, BOXUV* boxuv)
{
	int pos, exact, dirFlag ;
	IENTITY* pCur1, *pCur2 ;
	INTERVAL interval1, interval2 ;
	CURVE* curve1 = NULL, *curve2 = NULL ;
	BSCUR* nurbs1 = NULL, *nurbs2 = NULL ;
	BSSUR* nurbs = NULL ;
	SURFACE* surface = NULL ;

	if( m_deep++ > m_max )
		return 0 ;
	pos = ruleGetCurve1(pE) ;
	pCur1 = igesGetEntity(m_iges, pos/2) ;
	curve1 = ientity2Curve(pCur1, &interval1) ;
	if( !curve1 )
		goto ExceExit ;
	pos = ruleGetCurve2(pE) ;
	pCur2 = igesGetEntity(m_iges, pos/2) ;
	curve2 = ientity2Curve(pCur2, &interval2) ;
	if( !curve2 )
		goto ExceExit ;
	if( curveToNurbs(curve1, // get first bnd of rule surface
					 &interval1, 
					 &nurbs1, 
					 &exact) != ERSUCSS )
		goto ExceExit ;
	bscurModifyKVec01(nurbs1) ; // nt add 2021/5/7
	if( curveToNurbs(curve2, // get second bnd of rule surface
					 &interval2, 
					 &nurbs2, 
					 &exact) != ERSUCSS )
		goto ExceExit ;
	bscurModifyKVec01(nurbs2) ; // nt add 2021/5/7
	dirFlag = ruleGetDirFlag(pE) ;
	if( dirFlag )
		bscurRevsDir(nurbs2) ;
	bssurCreRule(nurbs1, nurbs2, &nurbs) ;
	if( nurbs )
		surface = bsurfaceCreate(nurbs) ;
	boxuv->min[0] = nurbs->u_k[nurbs->ord_u-1] ; // nt modify 2021/5/6
	boxuv->max[0] = nurbs->u_k[nurbs->cnt_u] ; // nt modify 2021/5/6
	boxuv->min[1] = nurbs->v_k[nurbs->ord_v-1] ; // nt modify 2021/5/6
	boxuv->max[1] = nurbs->v_k[nurbs->cnt_u] ; // nt modify 2021/5/6

ExceExit:
	if( curve1 )
		entityFree(curve1) ;
	if( curve2 )
		entityFree(curve2) ;
	if( nurbs1 )
		bscurFree(nurbs1) ;
	if( nurbs2 )
		bscurFree(nurbs2) ;
	if( nurbs )
		bssurFree(nurbs) ;
	m_deep-- ;

	return surface ;
}

// nt add 2010/5/16
// nt modify 2021/5/7 所生成的直纹面参数域为[0,1]x[0,1]
SURFACE* CIgesImport::ientity2Rule(IENTITY* pE, BOXUV* boxuv)
{
	int pos, exact, i ;
	double b[3], e[3], v[3] ;
	INTERVAL interval ;
	CURVE* curve = NULL ;
	BSCUR* nurbs1 = NULL, 
		  *nurbs2 = NULL ;
	BSSUR* nurbs = NULL ;
	SURFACE* surface = NULL ;
	IENTITY* pE2 = NULL ;

	if( m_deep++ > m_max )
		return 0 ;
	pos = tcylGetCurve(pE) ; // get curve
	pE2 = igesGetEntity(m_iges, pos/2) ;
	curve = ientity2Curve(pE2, &interval) ;
	if( !curve )
		goto ExceExit ;
	if( curveToNurbs(curve, // get first bnd of rule surface
					 &interval, 
					 &nurbs1, 
					 &exact) != ERSUCSS )
		goto ExceExit ;
	bscurModifyKVec01(nurbs1) ; // nt add 2021/5/7
	bscurEvalPos(nurbs1, 0., b) ;
	tcylGetP(pE, e) ;
	//toM(e) ;
	mathGetVec(b, e, v) ;
	nurbs2 = bscurCopy(nurbs1) ;
	for( i = 0 ; i < nurbs2->cnt_u ; i++ ) // move nurbs1
	{
		nurbs2->u_p[i][0] += v[0]*nurbs2->u_p[i][3] ;
		nurbs2->u_p[i][1] += v[1]*nurbs2->u_p[i][3] ;
		nurbs2->u_p[i][2] += v[2]*nurbs2->u_p[i][3] ;
	}
	bssurCreRule(nurbs1, nurbs2, &nurbs) ;
	if( nurbs )
		surface = bsurfaceCreate(nurbs) ;
	boxuv->min[0] = nurbs->u_k[nurbs->ord_u-1] ; // nt modify 2021/5/6
	boxuv->max[0] = nurbs->u_k[nurbs->cnt_u] ; // nt modify 2021/5/6
	boxuv->min[1] = nurbs->v_k[nurbs->ord_v-1] ; // nt modify 2021/5/6
	boxuv->max[1] = nurbs->v_k[nurbs->cnt_u] ; // nt modify 2021/5/6

ExceExit:
	if( curve )
		entityFree(curve) ;
	if( nurbs1 )
		bscurFree(nurbs1) ;
	if( nurbs2 )
		bscurFree(nurbs2) ;
	if( nurbs )
		bssurFree(nurbs) ;
	m_deep-- ;
	return surface ;
}

// nt add 2010/5/16
// nt modify 2010/11/7
// nt modify 2011/11/16
SURFACE* CIgesImport::ientity2Revolve(IENTITY* pE, BOXUV* boxuv)
{
	int pos ;
	//double b[3], e[3], dir[3] ;
	PNT3D pivot ;
	VEC3D dir ;
	INTERVAL interval ;
	CURVE* curve = NULL, *line = NULL ;
	SURFACE* surface = NULL ;

	if( m_deep++ > m_max )
		return 0 ;
	pos = revolveGetAxis(pE) ; // get axis line
	IENTITY* pE2 = igesGetEntity(m_iges, pos/2) ;
	if( !pE2 )
		goto ExceExit ;
	// nt add 2010/11/7, ientity2Curve()实现了坐标变换
	line = ientity2Curve(pE2, &interval) ;
	if( !line )
		goto ExceExit ;
	//curveEvalTangent(line, 0., pivot, dir) ;
	curveEvalTangent(line, interval.t1, pivot, dir) ; // nt modify 2021/5/7
	entityFree(line) ; // nt add 2011/11/16
	line = NULL ; // nt add 2011/11/16
	//lineGetB(pE2, b) ;
	//toM(b) ;
	//lineGetE(pE2, e) ;
	//toM(e) ;
	//mathGetVec(b, e, dir) ;
	//if( mathUniVec(dir, MIN_LEN) != ERSUCSS )
	//	goto ExceExit ;
	pos = revolveGetCurve(pE) ; // get profile curve
	pE2 = igesGetEntity(m_iges, pos/2) ;
	curve = ientity2Curve(pE2, &interval) ;
	if( !curve )
		goto ExceExit ;
	// nt add 2011/11/16
	if( entityGetType(curve) != etBCurve ) // 要求B样条
	{
		int exact = 0 ;
		BSCUR* nurbs = NULL ;
		curveToNurbs(curve, &interval, &nurbs, &exact) ;
		interval.t1 = nurbs->u_k[nurbs->ord_u-1] ;
		interval.t2 = nurbs->u_k[nurbs->cnt_u] ;
		entityFree(curve) ;
		curve = bcurveCreate(nurbs) ;
		bscurFree(nurbs) ;
		nurbs = NULL ;
	}
	surface = revsurCreate(curve, pivot, dir) ;
	boxuv->min[0] = interval.t1 ;
	boxuv->max[0] = interval.t2 ;
	boxuv->min[1] = revolveGetSA(pE) ;
	boxuv->max[1] = revolveGetTA(pE) ;

ExceExit:
	if( curve )
		entityFree(curve) ;
	m_deep-- ;
	return surface ;
}

// 没有用到IGES的offset surface中的(Nx,Ny,Nz)
SURFACE* CIgesImport::ientity2OffSur(IENTITY* pE, BOXUV* boxuv)
{
	int pos ;
	double d = 0. ;
	IENTITY* pE2 = NULL ;
	SURFACE* base = NULL,
		     *offsur = NULL ;
	if( m_deep++ > m_max )
		return 0 ;
	pos = offsurGetBase(pE) ;
	pE2 = igesGetEntity(m_iges, pos/2) ;
	base = ientity2Surface(pE2, boxuv) ;
	if( !base )
		goto ExceExit ;
	//d = offsurGetD(pE)*0.001 ; // distance
	d = offsurGetD(pE) ;
	offsur = offsetCreate(base, d) ;

ExceExit:
	if( base )
		entityFree(base) ;
	m_deep-- ;
	return offsur ;
}
////////////////////////////////////////////////////////////////
