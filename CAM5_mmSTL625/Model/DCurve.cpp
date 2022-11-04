/*	2021/6/29 nt
 *	implementation file of discrete curve
 */
#include "StdAfx.h"
#include "DCurve.h"

////////////////////////////////////////////////////////////////
// nt add 2021/7/1 : 要求A,B,C不能重合
/*  1995/1 nt
 *  两条线段交点，IDNOINT无交,IDINT交一点,IDOVERL有重合段,IDDEGE线退化为点
 */
void dctrimInit(DCTRIM* dctrim)
{
	dctrim->error = 0 ;
	dctrim->dcurves = NULL ;
	dctrim->intpts = NULL ;

	return ;
}

void dctrimAddDCurve(DCTRIM* dctrim, DCURVE* dcurve)
{
	if( dctrim &&
		dcurve )
	{
		dcurve->next = dctrim->dcurves ;
		dctrim->dcurves = dcurve ;
	}

	return ;
}

void dctrimAddIntPt(DCTRIM* dctrim, INTPT* intpt)
{
	if( dctrim &&
		intpt )
	{
		intpt->prev = NULL ;
		intpt->next = dctrim->intpts ;
		if( dctrim->intpts )
			dctrim->intpts->prev = intpt ;
		dctrim->intpts = intpt ;
	}

	return ;
}

int dctrimGetNumOfDCurve(DCTRIM* dctrim)
{
	return dctrim?mathGetNumOfItemInSDChns(dctrim->dcurves):0 ;
}

int dctrimGetNumOfIntPt(DCTRIM* dctrim)
{
	return dctrim?mathGetNumOfItemInBDChns(dctrim->dcurves):0 ;
}
//==============================================================
/*	2001/9/1 nt
 *	create DCURVE
 */
DCURVE* dcurveCreate(int np)
{
	DCURVE* dcurve ;

	if( np < 1 )
		return NULL ;

	dcurve = (DCURVE*)malloc(sizeof(DCURVE)) ;
	if( !dcurve )
		return NULL ;
	dcurve->next = NULL ;
	dcurve->np = np ;
	dcurve->ps = (PNT3D*)malloc(sizeof(PNT3D)*np) ; // points
	dcurve->ns = (VEC3D*)malloc(sizeof(VEC3D)*np) ; // vectors
	dcurve->flag = 0 ;

	return dcurve ;
}

/*	2001/9/1 nt
 *	free DCURVE
 */
void dcurveFree(DCURVE* dcurve)
{
	if( dcurve )
	{
		if( dcurve->ps )
			free(dcurve->ps) ; // points
		if( dcurve->ns )
			free(dcurve->ns) ; // vectors
		free(dcurve) ;
	}

	return ;
}

void dcurvesFree(DCURVE* dcurves)
{
	DCURVE* temp ;

	while(dcurves)
	{
		temp = dcurves->next ;
		dcurveFree(dcurves) ;
		dcurves = temp ;
	}

	return ;
}

// nt add 2021/5/2
double* dcurveGetBeginPt(DCURVE* dcurve, PNT3D b)
{
	if (dcurve && dcurve->ps)
	{
		if( b )
			memcpy(b, dcurve->ps[0], sizeof(PNT3D)) ;
		return &(dcurve->ps[0][0]) ;
	}
	else
		return NULL ;
}

// nt add 2021/5/2
double* dcurveGetEndPt(DCURVE* dcurve, PNT3D e)
{
	if (dcurve && dcurve->ps)
	{
		if( e )
			memcpy(e, dcurve->ps[dcurve->np-1], sizeof(PNT3D)) ;
		return &(dcurve->ps[dcurve->np-1][0]) ;
	}
	else
		return NULL ;
}

double* dcurveGetBeginTangent(DCURVE* dcurve, PNT3D b, VEC3D tvec)
{
	if (dcurve && dcurve->ps)
	{
		if( b )
			memcpy(b, dcurve->ps[0], sizeof(PNT3D)) ;
		if( tvec )
			mathGetVecUnit(dcurve->ps[0], dcurve->ps[1], tvec) ;
		return &(dcurve->ps[0][0]) ;
	}
	else
		return NULL ;
}

double* dcurveGetEndTangent(DCURVE* dcurve, PNT3D e, VEC3D tvec)
{
	if (dcurve && dcurve->ps)
	{
		if( e )
			memcpy(e, dcurve->ps[dcurve->np-1], sizeof(PNT3D)) ;
		if( tvec )
			mathGetVecUnit(dcurve->ps[dcurve->np-2], dcurve->ps[dcurve->np-1], tvec) ;
		return &(dcurve->ps[dcurve->np-1][0]) ;
	}
	else
		return NULL ;
}

// return 0=dcurve1与dcurve2不连接
// return 1=dcurve1的起点与dcurve2连接
// return 2=dcurve1的终点与dcurve2连接
// return 3=dcurve1的起点和终点都与dcurve2连接
int dcurveIsLinkAt(DCURVE* dcurve1, DCURVE* dcurve2, double tol)
{
	int flag = 0 ;

	if (mathDist(dcurve1->ps[0], dcurve2->ps[0]) < tol ||
		mathDist(dcurve1->ps[0], dcurve2->ps[dcurve2->np-1]) < tol)
		flag += 1 ;
	if (mathDist(dcurve1->ps[dcurve1->np-1], dcurve2->ps[0]) < tol ||
		mathDist(dcurve1->ps[dcurve1->np-1], dcurve2->ps[dcurve2->np-1]) < tol)
		flag += 2 ;

	return flag ;
}

/*	2002/2/17 nt
 *	copy dcurve
 */
DCURVE* dcurveCopy(DCURVE* dcurve)
{
	DCURVE* copy ;

	if( !dcurve )
		return NULL ;

	copy = dcurveCreate(dcurve->np) ;
	if( !copy )
		return NULL ;
	copy->next = NULL ;
	copy->np = dcurve->np ;
	if( dcurve->ps )
		memcpy(copy->ps, dcurve->ps, sizeof(PNT3D)*dcurve->np) ;
	if( dcurve->ns )
		memcpy(copy->ns, dcurve->ns, sizeof(VEC3D)*dcurve->np) ;
	copy->flag = dcurve->flag ;

	return copy ;
}

/*	2021/4/30 nt
 *	reverse direction
 *	注意切线方向也反向
 */
int dcurveRev(DCURVE* dcurve, int revVec)
{
	int i, j ;
	double p[3] ;

	if( !dcurve )
		return ERUNSUC ;

	for( i = 0, j = dcurve->np-1 ; i < j ; i++, j-- )
	{
		if (dcurve->ps)
		{
			memcpy(p, dcurve->ps[i], sizeof(double[3])) ;
			memcpy(dcurve->ps[i], dcurve->ps[j], sizeof(double[3])) ;
			memcpy(dcurve->ps[j], p, sizeof(double[3])) ;
		}

		if (dcurve->ns)
		{
			memcpy(p, dcurve->ns[i], sizeof(double[3])) ;
			memcpy(dcurve->ns[i], dcurve->ns[j], sizeof(double[3])) ;
			memcpy(dcurve->ns[j], p, sizeof(double[3])) ;
		}
	}

	if (revVec &&
		dcurve->ns)
	{
		for( i = 0 ; i < dcurve->np ; i++ )
		{
			dcurve->ns[i][0] = -dcurve->ns[i][0] ;
			dcurve->ns[i][1] = -dcurve->ns[i][1] ;
			dcurve->ns[i][2] = -dcurve->ns[i][2] ;
		}
	}

	return ERSUCSS ;
}

/*	2002/12/24 nt
 *	move
 */
int dcurveMove(DCURVE* dcurve, VEC3D v)
{
	int i ;

	if( !dcurve ||
		!v )
		return ERUNSUC ;

	if( dcurve->ps == NULL )
		return ERSUCSS ;

	for( i = 0 ; i < dcurve->np ; i++ )
	{
		dcurve->ps[i][0] += v[0] ;
		dcurve->ps[i][1] += v[1] ;
		dcurve->ps[i][2] += v[2] ;
	}

	return ERSUCSS ;
}

/*	2002/12/24 nt
 *	rotate
 */
int dcurveRotate(DCURVE* dcurve, PNT3D pivot, VEC3D axis, double angle)
{
	int i ;

	if( !dcurve ||
		!pivot ||
		!axis )
		return ERUNSUC ;

	for( i = 0 ; i < dcurve->np ; i++ )
	{
		if( dcurve->ps )
			mathRotPnt(axis, pivot, angle, dcurve->ps[i], dcurve->ps[i]) ;
		if( dcurve->ns )
			mathRotVec(axis, pivot, angle, dcurve->ns[i], dcurve->ns[i]) ;
	}

	return ERSUCSS ;
}

/*	2002/12/24 nt
 *	mirror
 */
int dcurveMirror(DCURVE* dcurve, PNT3D pivot, VEC3D normal)
{
	int i ;

	if( !dcurve ||
		!pivot ||
		!normal )
		return ERUNSUC ;

	for( i = 0 ; i < dcurve->np ; i++ )
	{
		if( dcurve->ps )
			mathMirPnt(pivot, normal, dcurve->ps[i], dcurve->ps[i]) ;
		if( dcurve->ns )
			mathMirVec(pivot, normal, dcurve->ns[i], dcurve->ns[i]) ;
	}

	return ERSUCSS ;
}

/*	2002/3/7 nt
 *	to local frame
 */
int dcurveToLocal(DCURVE* dcurve, RFRAME* pLF) // nt modify 2002/7/14
{
	int i ;

	if( !dcurve ||
		!pLF )
		return ERUNSUC ;

	if( dcurve->ps )
	{
		for( i = 0 ; i < dcurve->np ; i++ )
			mathTransWorldPnt3D(pLF, dcurve->ps[i], dcurve->ps[i]) ;
	}
	if( dcurve->ns )
	{
		for( i = 0 ; i < dcurve->np ; i++ )
			mathTransWorldVec3D(pLF, dcurve->ns[i], dcurve->ns[i]) ;
	}

	return ERSUCSS ;
}

/*	2002/3/7 nt
 *	to world frame
 */
int dcurveToWorld(DCURVE* dcurve, RFRAME* pLF)
{
	int i ;

	if( !dcurve ||
		!pLF )
		return ERUNSUC ;

	if( dcurve->ps )
	{
		for( i = 0 ; i < dcurve->np ; i++ )
			mathTransLocalPnt3D(pLF, dcurve->ps[i], dcurve->ps[i]) ;
	}
	if( dcurve->ns )
	{
		for( i = 0 ; i < dcurve->np ; i++ )
			mathTransLocalVec3D(pLF, dcurve->ns[i], dcurve->ns[i]) ;
	}

	return ERSUCSS ;
}

/*	2002/12/24 nt
 *	scale
 */
int dcurveScale(DCURVE* dcurve, double scale)
{
	int i ;

	if( !dcurve )
		return ERUNSUC ;

	for( i = 0 ; i < dcurve->np ; i++ )
	{
		dcurve->ps[i][0] *= scale ;
		dcurve->ps[i][1] *= scale ;
		dcurve->ps[i][2] *= scale ;
	}

	return ERSUCSS ;
}

/*  1997/6/20 nt
 *  计算离散的折线的包围盒, tol is DCURVE tol
 */
int dcurveCalBox(DCURVE* dcurve, double tol, BOX3D* box)
{
    if( !dcurve ||
		!box )
		return ERUNSUC ;

    mathCalBoxStr(dcurve->np, dcurve->ps, box) ;
	box->min[0] -= tol ;
	box->min[1] -= tol ;
	box->min[2] -= tol ;
	box->max[0] += tol ;
	box->max[1] += tol ;
	box->max[2] += tol ;

    return ERSUCSS ;
}
//--------------------------------------------------------------
/*	2002/2/18 nt
 *	to find a set of nearest points on the polyline, considering the tolerance
 *	note that : dcurve->ps, dcurve->ts are necessary, must be none NULL
 *	            nNearestPt and ts may be NULL
 */
int dcurveSnap(DCURVE* dcurve,
			   double tol,
			   PNT3D p,
			   PNT3D q,
			   double* pt,
			   double* pd)
{
	int i ;
	double w, d, a ;
	VEC3D v1, v2, v ;

	// 1.
	if( !dcurve ||
		!dcurve->ps ||
		!p ||
		!q ||
		!pt )
		return ERUNSUC ;

	// 2.
	v1[0] = p[0]-dcurve->ps[0][0] ; // the vector of ps[0] to p
	v1[1] = p[1]-dcurve->ps[0][1] ;
	v1[2] = p[2]-dcurve->ps[0][2] ;
	a = sqrt(v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]) ; // distance of ps[0] to p
	//*pt = dcurve->ts[0] ;
	*pt = 0. ;
	for( *pd = a, i = 1 ; i < dcurve->np ; i++ )
	{
		v2[0] = dcurve->ps[i][0]-p[0] ; // the vector of p to ps[i]
		v2[1] = dcurve->ps[i][1]-p[1] ;
		v2[2] = dcurve->ps[i][2]-p[2] ;
		v[0]  = dcurve->ps[i][0]-dcurve->ps[i-1][0] ; // vector of ps[i-1] to ps[i]
		v[1]  = dcurve->ps[i][1]-dcurve->ps[i-1][1] ;
		v[2]  = dcurve->ps[i][2]-dcurve->ps[i-1][2] ;
		if( v1[0]*v[0]+v1[1]*v[1]+v1[2]*v[2] > MIN_DBL &&
			v2[0]*v[0]+v2[1]*v[1]+v2[2]*v[2] > MIN_DBL )
		{
			a = mathDistPntLinEx(p, dcurve->ps[i-1], dcurve->ps[i]) ;
			if( *pd > a )
			{
				d = mathDist(dcurve->ps[i-1], dcurve->ps[i]) ;
				mathPrjPntLinEx(p, dcurve->ps[i-1], dcurve->ps[i], q) ;
				w = d < MIN_DBL ? 0.5 : mathDist(dcurve->ps[i-1], q)/d ;
				//*pt = dcurve->ts[i-1]*(1.0-w)+dcurve->ts[i]*w ;
				*pt = (double)(i-1)+w ;
				*pd = a ;
			}
		}
		else
		{
			a = sqrt(v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2]) ;
			if( *pd > a )
			{
				memcpy(q, dcurve->ps[i], sizeof(PNT3D)) ;
				//*pt = dcurve->ts[i] ;
				*pt = (double)i ;
				*pd = a ;
			}
		}
		v1[0] = -v2[0] ;
		v1[1] = -v2[1] ;
		v1[2] = -v2[2] ;
	}

	return ERSUCSS ;
}

/*	2002/2/20 nt
 *	is point on the dcurve under tolerance
 */
int dcurveIsPtOn(DCURVE* dcurve, PNT3D p, double tol, int* pyesno)
{
	int i ;

	// 1.
	if( !dcurve ||
		!dcurve->ps ||
		!p )
		return ERUNSUC ;
	*pyesno = ID_YES ;

	// 2.
	for( i = 1 ; i < dcurve->np ; i++ )
	{
		if( mathIsPntInCyl(p, dcurve->ps[i-1], dcurve->ps[i], tol) == ID_YES )
			return ERSUCSS ;
	}
	*pyesno = ID_NO ;

	return ERSUCSS ;
}

int dcurveIsClose(DCURVE* dcurve, double tol)
{
	PNT3D b, e ;

	dcurveGetBeginPt(dcurve, b) ;
	dcurveGetEndPt(dcurve, e) ;
	if( mathDist(b, e) < tol )
		return ID_YES ;
	else
		return ID_NO ;
}

/*	2002/2/18 nt
 *	check if two dcurves are overlap under the given toloerance : tol
 *	tol is DCURVE tol
 */
int dcurveIsOverlap(DCURVE* dcurve1, DCURVE* dcurve2, double tol, int* pyesno)
{
	int i ;

	if( !dcurve1 ||
		!dcurve1->ps || 
		!dcurve2 ||
		!dcurve2->ps )
		return ERUNSUC ;

	for( i = 0 ; i < dcurve1->np ; i++ )
	{
		if( dcurveIsPtOn(dcurve2, dcurve1->ps[i], tol, pyesno) != ERSUCSS )
			return ERUNSUC ;
		if( *pyesno == ID_NO )
			return ERSUCSS ;
	}

	for( i = 0 ; i < dcurve2->np ; i++ )
	{
		if( dcurveIsPtOn(dcurve1, dcurve2->ps[i], tol, pyesno) != ERSUCSS )
			return ERUNSUC ;
		if( *pyesno == ID_NO )
			return ERSUCSS ;
	}

	*pyesno = ID_YES ;

	return ERSUCSS ;
}

double triCalW(double a, double b, double c, double d)
{
	int i, n ;
	double xs[2] ;

	if( c < MIN_DBL )
		return 0. ; // error
	if( a < MIN_DBL )
		return d/c ;

	// cosB = (a*a+c*c-b*b)/(2*a*c) ;
	// (b-a)*(b+a)/c-c = -2.*a*(a*a+c*c-b*b)/(2.*a*c),
	n = mathSolQuadEqua(1., 
					    (b-a)*(b+a)/c-c,
		                (a+d)*(a-d),
					    MIN_DBL,
						xs) ;
	for( i = 0 ; i < 2 ; i++ )
	{
		if( xs[i] >= 0. &&
			xs[i] <= c &&
			xs[i] <= a+d &&
			d <= xs[i]+a &&
			a <= xs[i]+d )
			return xs[i]/c ;
	}

	return 0. ; // error
}

int mathIntSegmLin(PNT3D begin,
			       PNT3D end,
				   PNT3D b,
				   VEC3D dir,
				   double tol,
				   double mol,
				   PNT3D p1,
				   PNT3D p2,
				   double* pt1,
				   double* pt2)
{
	double d, d1, d2, dd, h ;
	PNT3D q ;
	VEC3D v ;

	// 1. prj
	*pt1 = 0. ; // nt add 2003/2/16
	*pt2 = 1. ; // nt add 2003/2/16
    mathPrjPntPln(begin, b, dir, p1) ;
    mathPrjPntPln(end, b, dir, p2) ;
	v[0] = p2[0]-p1[0] ;
	v[1] = p2[1]-p1[1] ;
	v[2] = p2[2]-p1[2] ;
	d1 = mathDist(p1, b) ;
	d2 = mathDist(p2, b) ;
	dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;

	// 2. case 1
	if( dd < MIN_DBL || // nt add 2021/6/30, 对于两种情况合并处理
		(d1 <= tol && d2 <= tol) )
	{
		memcpy(p1, begin, sizeof(PNT3D)) ;
		memcpy(p2, end, sizeof(PNT3D)) ;
		d = mathDist(begin, end) ;
		if( 0.5*(d1+d2) <= tol )
			return d<mol?IDINT:IDOVERL ;
		else
			return IDNOINT ;
	}

	// 3. case 2
	if( d1 < tol || d2 < tol )
	{
		memcpy(p1, d1<tol?begin:end, sizeof(PNT3D)) ;
		*pt1 = d1<tol?0.:1. ;
		*pt2 = 1.-triCalW(d2, d1, dd, tol) ;
		mathMixPnt(begin, end, 1.-pt2[0], pt2[0], p2) ;
		if( mathDist(p1, p2) > mol )
			return IDOVERL ;
		else
			return IDINT ;
	}

	// 4. d1>tol且d2>tol
	v[0] /= dd ;
	v[1] /= dd ;
	v[2] /= dd ;
	d = v[0]*(b[0]-p1[0])+v[1]*(b[1]-p1[1])+v[2]*(b[2]-p1[2]) ;
	if( 0. < d && d < dd )
	{
		mathEvalLin(p1, v, d, q) ;
		h = mathDist(b, q) ;
		if( h < tol )
		{
			h = sqrt((tol-h)*(tol+h)) ;
			if( (*pt1 = (d-h)/dd) < 0. )
				*pt1 = 0. ;
			mathMixPnt(begin, end, 1.-pt1[0], pt1[0], p1) ;
			if( (*pt2 = (d+h)/dd) > 1. )
				*pt2 = 1. ;
			mathMixPnt(begin, end, 1.-pt2[0], pt2[0], p2) ;
			if( mathDist(p1, p2) > mol )
				return IDOVERL ;
			else
			{
				*pt1 = d/dd ;
				mathMixPnt(begin, end, 1.-pt1[0], pt1[0], p1) ;
				return IDINT ;
			}
		}
	}

	return IDNOINT ;
}

/*  1997/4 nt
 *  拾取polyline
 *	nt modify 2011/7/6
 */
int dcurvePick(DCURVE* dcurve,
               PNT3D pos,
			   VEC3D dir,
               double scope,
               double* pz,
			   double* pu,
			   int* picked)
{
    int i ;
	double A, B, C, D, t ;
	PNT3D p1, p2 ;
	VEC3D e1, e2 ;

	if( !dcurve )
		return ERUNSUC ;

	*picked = ID_NO ;
	mathGetXYFromZ(dir, e1, e2) ;
	mathTransWorldPnt3DByOXYZ(pos, e1, e2, dir, dcurve->ps[0], p1) ;
	for( i = 1 ; i < dcurve->np ; i++ )
	{
		mathTransWorldPnt3DByOXYZ(pos, e1, e2, dir, dcurve->ps[i], p2) ;
		A = (p2[0]-p1[0])*(p2[0]-p1[0])+(p2[1]-p1[1])*(p2[1]-p1[1]) ;
		B = p1[0]*(p2[0]-p1[0])+p1[1]*(p2[1]-p1[1]) ;
		C = p1[0]*p1[0]+p1[1]*p1[1] ;
		D = p2[0]*p2[0]+p2[1]*p2[1] ;
		t = -1.0 ;

		if( sqrt(C) < scope )
			t = 0.0 ;
		else
		if( sqrt(D) < scope )
			t = 1.0 ;
		else
		if( A > 1.e-20 && sqrt(fabs(C-B*B/A)) < scope )
			t = -B/A ;

		if( t > -MIN_DBL && t < 1.0 + MIN_DBL )
		{
			pu[0] = (double)(i-1)+t ;
			pz[0] = p1[2]*(1.0-t)+p2[2]*t ;
			*picked = ID_YES ;
			return ERSUCSS ;
		}

		p1[0] = p2[0] ;
		p1[1] = p2[1] ;
		p1[2] = p2[2] ; // nt add 2011/7/6
	}

	return ERSUCSS ;
}
/*-----------------------------------------------------------*/
/*	2002/3/27 nt
 *	calculate intersection points of DCURVE with line, not consider overlap
 *	nt modify 2003/2/16
 *	nt modify 2021/6/30
 */
int dcurveIntLin(DCURVE* dcurve, 
				 PNT3D begin, 
				 VEC3D dir, 
				 double tol,
				 double mol,
				 INTPT** intpts)
{
	int i, rt ;
	double t1, t2 ;
	PNT3D p1, p2 ;

	// 1.
	if( !dcurve ||
		!begin ||
		!dir )
		return ERUNSUC ;
	*intpts = NULL ;

	// 2.
	for( i = 1 ; i < dcurve->np ; i++ ) // for every small segment in dcurve
	{
		rt = mathIntSegmLin(dcurve->ps[i-1],
							dcurve->ps[i],
							begin,
							dir,
							tol,
							mol,
						    p1,
							p2,
							&t1,
							&t2) ;
		if( rt == IDINT )
		{
			if( (*intpts) == NULL || mathDist((*intpts)->p, p1) > tol )
				intptCreateIns(intpts, ipSimple, p1, NULL, i-1+t1, 0., 0., 0.) ;
		}
		else
		if( rt == IDOVERL )
		{
			if( (*intpts) == NULL || mathDist((*intpts)->p, p1) > tol )
				intptCreateTwo(intpts, p1, p2, i-1+t1, 0., i-1+t2, 0.) ;
			else
			{
				if( (*intpts)->type != ipOverlap )
				{
					*intpts = (INTPT*)mathDelItemInBDChns(*intpts, *intpts) ;
					intptCreateTwo(intpts, p1, p2, i-1+t1, 0., i-1+t2, 0.) ;
				}
				else
				{
					memcpy((*intpts)->p, p2, sizeof(PNT3D)) ;
					(*intpts)->uv[0] = i-1+t1 ;
				}
			}
		}
	}

	return ERSUCSS ;
}

/*	2002/3/12 nt
 *	calculate the intersection points of DCURVE with plane, consider the overlap
 *	nt modify 2002/8/11
 *	nt modify 2003/2/16
 *	nt modify 2021/6/30
 */
int dcurveIntPln(DCURVE* dcurve,
				 PNT3D pivot, 
				 VEC3D normal, 
				 double tol,
				 double mol,
				 INTPT** intpts)
{
	int i ;
	double d1, d2, d, w ;
	PNT3D p ;

	// 1.
	if( !dcurve ||
		dcurve->np < 1 ||
		!pivot ||
		!intpts )
		return ERUNSUC ;
	*intpts = NULL ;

	// 2.
	d1 = mathDistPntPln(dcurve->ps[0], pivot, normal) ;
	d2 = mathDistPntPln(dcurve->ps[1], pivot, normal) ;
	if( fabs(d1) < tol &&
		fabs(d2) > tol )
	{
		memcpy(p, dcurve->ps[0], sizeof(PNT3D)) ;
		//t = dcurve->ts[0] ;
		intptCreateIns(intpts, ipSimple, p, NULL, 0., 0., 0., 0.) ;
	}
	for( i = 1 ; i < dcurve->np ; i++ ) // for every small segment in dcurve
	{
		d2 = mathDistPntPln(dcurve->ps[i], pivot, normal) ;
		d = mathDist(dcurve->ps[i-1], dcurve->ps[i]) ;
		if( fabs(d1) < tol &&
			fabs(d2) < tol )
		{
			if( d < mol )
			{
				mathMidPnt(dcurve->ps[i-1], dcurve->ps[i], p) ;
				//t = 0.5*(dcurve->ts[i-1]+dcurve->ts[i]) ;
				if( (*intpts) == NULL || mathDist((*intpts)->p, p) > tol )
					intptCreateIns(intpts, ipTangent, p, NULL, i-0.5, 0., 0., 0.) ;
			}
			else
			{
				if( (*intpts) == NULL || mathDist((*intpts)->p, dcurve->ps[i-1]) > tol )
					intptCreateTwo(intpts,
								   dcurve->ps[i-1],
								   dcurve->ps[i], 
								   (double)(i-1),//dcurve->ts[i-1],
								   0.,
								   (double)i,//dcurve->ts[i],
								   0.) ;
				else
				{
					if( (*intpts)->type != ipOverlap )
					{
						*intpts = (INTPT*)mathDelItemInBDChns(*intpts, *intpts) ;
						intptCreateTwo(intpts,
									   dcurve->ps[i-1],
									   dcurve->ps[i], 
									   (double)(i-1),//dcurve->ts[i-1],
									   0.,
									   (double)i,//dcurve->ts[i],
									   0.) ;
					}
					else
					{
						memcpy((*intpts)->p, dcurve->ps[i], sizeof(PNT3D)) ;
						(*intpts)->uv[0] = (double)i ; //dcurve->ts[i] ;
					}
				}
			}
		}
		else
		if( fabs(d2) < tol )
		{
			memcpy(p, dcurve->ps[i], sizeof(PNT3D)) ;
			//t = dcurve->ts[i] ;
			if( (*intpts) == NULL || mathDist((*intpts)->p, p) > tol )
				intptCreateIns(intpts, ipSimple, p, NULL, (double)i, 0., 0., 0.) ;
		}
		else
		if( d1*d2 < 0. )
		{
			w = d1/(d1-d2) ;
			mathMixPnt(dcurve->ps[i-1], dcurve->ps[i], 1.-w, w, p) ;
			//t = dcurve->ts[i-1]*(1.-w)+dcurve->ts[i]*w ;
			if( (*intpts) == NULL || mathDist((*intpts)->p, p) > tol )
				intptCreateIns(intpts, ipSimple, p, NULL, (double)(i-1)+w, 0., 0., 0.) ;
		}
		d1 = d2 ;
	}

	return ERSUCSS ;
}

int dcurveInt(DCURVE* dcurve1,
			  BOX3D* box1,
			  DCURVE* dcurve2,
			  BOX3D* box2,
			  double tol,
			  int overlap,
			  INTPT** intpts)
{
	int i, j, rt ;
	double s1, s2, t1, t2 ;
	PNT3D p, q ;
	BOX3D box ;

	if( dcurve1 == NULL ||
		box1 == NULL ||
		dcurve2 == NULL ||
		box2 == NULL ||
		intpts == NULL )
		return ERUNSUC ;
	*intpts = NULL ;

	if( mathChkBoxInt(box1, box2, tol) != IDINT )
		return ERSUCSS ;

	for( i = 1 ; i < dcurve1->np ; i++ )
	{
		mathDefBox(dcurve1->ps[i-1], dcurve1->ps[i], &box) ;
		if( mathChkBoxInt(&box, box2, tol) != IDINT )
			continue ;
		for( j = 1 ; j < dcurve2->np ; j++ )
		{
			rt = mathIntSegm2(dcurve1->ps[i-1],
							  dcurve1->ps[i],
							  dcurve2->ps[j-1],
							  dcurve2->ps[j],
							  tol,
							  5*tol,
							  p,
							  q) ;
			if( rt == IDINT )
			{
				s1 = mathCalPntSegmPara(dcurve1->ps[i-1], dcurve1->ps[i], p) ;
				t1 = mathCalPntSegmPara(dcurve2->ps[j-1], dcurve2->ps[j], p) ;
				intptCreateIns(intpts, ipSimple, p, NULL, i-1+s1, 0., j-1+t1, 0.) ;
			}
			else
			if( rt == IDOVERL &&
				overlap == 1 )
			{
				s1 = mathCalPntSegmPara(dcurve1->ps[i-1], dcurve1->ps[i], p) ;
				t1 = mathCalPntSegmPara(dcurve2->ps[j-1], dcurve2->ps[j], p) ;
				s2 = mathCalPntSegmPara(dcurve1->ps[i-1], dcurve1->ps[i], q) ;
				t2 = mathCalPntSegmPara(dcurve2->ps[j-1], dcurve2->ps[j], q) ;
				intptCreateTwo(intpts, p, q, i-1+s1, i-1+s2, j-1+t1, j-1+t2) ;
			}
		}
	}

	return ERSUCSS ;
}
//--------------------------------------------------------------
// 估计在节点i处的切矢和曲率
int dcurveEstmTC(DCURVE* dcurve, int i, VEC3D tvec, double* c)
{
	if( dcurve->np < 2 )
		return ERUNSUC ;
	*c = 0. ;

	if( dcurve->np == 2 )
		mathGetVec(dcurve->ps[0], dcurve->ps[1], tvec) ;
	else
	{
		if( i == 0 )
			mathEstm3PTC(dcurve->ps[i], dcurve->ps[i+1], dcurve->ps[i+2], 0, tvec, c) ;
		else
		if( i < dcurve->np-1 )
			mathEstm3PTC(dcurve->ps[i-1], dcurve->ps[i], dcurve->ps[i+1], 1, tvec, c) ;
		else
			mathEstm3PTC(dcurve->ps[i-2], dcurve->ps[i-1], dcurve->ps[i], 2, tvec, c) ;
	}

	if( dcurve->ns )
		mathPrjVecPln(tvec, dcurve->ns[i], tvec) ;
	mathUniVec(tvec, MIN_LEN) ;

	return ERSUCSS ;
}

// i>=0, i<dcurve->n-1
int dcurveEvalPos(DCURVE* dcurve, double t, PNT3D pos, VEC3D normal)
{
	int i, k ;

	i = (int)floor(t) ;
	t -= (double)i ;
	if( i < 0 ||
		i >= dcurve->np )
		return ERUNSUC ;
	for( k = 0 ; k < 3 ; k++ )
	{
		pos[k] = dcurve->ps[i][k]+t*(dcurve->ps[i+1][k]-dcurve->ps[i][k]) ;
		normal[k] = dcurve->ns[i][k]+t*(dcurve->ns[i+1][k]-dcurve->ns[i][k]) ;
		mathUniVec(normal, MIN_LEN) ;
	}

	return ERSUCSS ;
}

// 去掉首尾重合的节点, 中间节点应该是无重合的情况
void dcurveRemDbl(DCURVE* dcurve, double tol)
{
	int i, n = dcurve->np ;

	if( mathDist(dcurve->ps[n-2], dcurve->ps[n-1]) < tol ) // 去掉节点n-2
	{
		memcpy(dcurve->ps[n-2], dcurve->ps[n-1], sizeof(PNT3D)) ;
		memcpy(dcurve->ns[n-2], dcurve->ns[n-1], sizeof(VEC3D)) ;
		n-- ;
	}

	if( mathDist(dcurve->ps[0], dcurve->ps[1]) < tol ) // 去掉节点1
	{
		for( i = 2 ; i < n ; i++ )
		{
			memcpy(dcurve->ps[i-1], dcurve->ps[i], sizeof(PNT3D)) ;
			memcpy(dcurve->ns[i-1], dcurve->ns[i], sizeof(VEC3D)) ;
		}
		n-- ;
	}

	dcurve->np = n ;

	return ;
}

DCURVE* dcurveMake(DCURVE* dcurve, double s, double t, double tol)
{
	int i, j, k, n ;
	DCURVE* temp = NULL ;

	i = (int)floor(s) ;
	j = (int)floor(t) ;

	if( i <= j )
	{
		n = j-i+2 ;
		temp = dcurveCreate(n) ;
		dcurveEvalPos(dcurve, s, temp->ps[0], temp->ns[0]) ;
		dcurveEvalPos(dcurve, t, temp->ps[n-1], temp->ns[n-1]) ;
		for( k = 1 ; k < n-1 ; k++ )
		{
			memcpy(temp->ps[k], dcurve->ps[i+k], sizeof(PNT3D)) ; // 位置
			memcpy(temp->ns[k], dcurve->ns[i+k], sizeof(VEC3D)) ; // 法矢
		}
	}
	else // 闭合情况才可能有
	{
		n = dcurve->np+2 ; // 最多这么多节点
		temp = dcurveCreate(n) ;
		dcurveEvalPos(dcurve, s, temp->ps[0], temp->ns[0]) ;
		n = 1 ;
		for( k = i+1 ; k < dcurve->np ; k++, n++ ) // k from i+1 to end
		{
			memcpy(temp->ps[n], dcurve->ps[k], sizeof(PNT3D)) ;
			memcpy(temp->ns[n], dcurve->ns[k], sizeof(VEC3D)) ;
		}
		for( k = 1 ; k <= j ; k++, n++ ) // k from 1 to j
		{
			memcpy(temp->ps[n], dcurve->ps[k], sizeof(PNT3D)) ;
			memcpy(temp->ns[n], dcurve->ns[k], sizeof(VEC3D)) ;
		}
		dcurveEvalPos(dcurve, t, temp->ps[n], temp->ns[n]) ;
		n++ ;
		temp->np = n ;
	}

	dcurveRemDbl(temp, tol) ;

	return temp ;
}
//--------------------------------------------------------------
/*	2021/7/1 nt
 *  输入参数:
 *		dcurve:离散曲线
 *		chkAll,0=不检查连续性,1=仅检查端点处连续性,2=检查所有节点处连续性
 *		p:一个节点的位置坐标
 *		a:连续性阈值，单位=弧度
 *	输出参数:
 *		rs:rs[0],rs[1],rs[2]为p点处单位切矢,
           rs[3],rs[4]为曲率及曲率半径
		nFlags:nFlags&1=1,起点不连续;nFlags&2=1,终点不连续;nFLags&4=1,p点不连续
 *	返回:
 *		-1=error,>=0为不满足连续性的节点的个数
 */
int dcurveAnalysis(DCURVE* dcurve, int chkAll, PNT3D p, double A, double rs[5], int* nFlags)
{
	int i, d, k, I = -1, n = 0 ;
	double c, min = MAX_DBL ;
	VEC3D tvec, v ;

	if( dcurve == NULL ||
		dcurve->np < 2 )
		return -1 ;
	rs[0] = rs[1] = rs[2] = rs[3] = rs[4] = 0. ;
	if( nFlags )
		*nFlags = 0 ;

	if( chkAll > 0 )
	{
		d = chkAll>1?1:(dcurve->np-1) ;
		for( i = 0 ; i < dcurve->np ; i += d )
		{
			dcurveEstmTC(dcurve, i, tvec, &c) ;
			k = i<dcurve->np-1?0:-1 ;
			mathGetVec(dcurve->ps[i+k], dcurve->ps[i+1+k], v) ;
			mathUniVec(v, MIN_LEN) ;
			if( mathGetAngleUnit(tvec, v) > A )
			{
				if( nFlags )
				{
					if( i == 0 )
						(*nFlags) += 1 ;
					else
					if( i == dcurve->np-1 )
						(*nFlags) += 2 ;
				}
				n++ ;
			}
		}
	}

	if( p && rs )
	{
		for( i = 0 ; i < dcurve->np ; i++ )
		{
			c = mathDist(dcurve->ps[i], p) ;
			if( c < MIN_LEN )
			{
				I = i ;
				break ;
			}
			if( min > c )
			{
				I = i ;
				min = c ;
			}
		}
		if( I < 0 )
			return -1 ;
		dcurveEstmTC(dcurve, I, rs, &(rs[3])) ;
		if( fabs(rs[3]) > MIN_DBL )
			rs[4] = 1./rs[3] ;
		else
			rs[4] = MAX_DBL ;
		if( nFlags )
		{
			k = I<dcurve->np-1?0:-1 ;
			mathGetVec(dcurve->ps[I+k], dcurve->ps[I+1+k], v) ;
			mathUniVec(v, MIN_LEN) ;
			if( mathGetAngleUnit(tvec, v) > A )
				(*nFlags) += 4 ;
		}
	}

	return n ;
}
//--------------------------------------------------------------
/*	2021/7/1 nt
 *  输入参数:
 *		dcurve:离散曲线，被裁剪的
 *		tool:离散曲线，剪刀
 *		tol:容差
 *	输出参数:
 *		rs:包括错误码,保留曲线段链表,以及交点链表
 *	返回:
 *		ERUNSCU,ERSUCSS
 *	注意:
 *	    1)未特殊处理dcurve闭合的情况
 *		2)未修改、释放dcurve和tool
 *		3)rs中DCURVE和INTPT是新生成的
 *		4)不处理重合的情况
 */
int dcurveTrim(DCURVE* dcurve, DCURVE* tool, double tol, DCTRIM* rs)
{
	double s ;
	PNT3D p, b, e ;
	BOX3D box1, box2 ;
	INTPT* intpts = NULL, *intpt ; //, *temp ;

	if( dcurve == NULL ||
		tool == NULL ||
		rs == NULL )
		return ERUNSUC ;
	dctrimInit(rs) ;
	
	dcurveCalBox(dcurve, tol, &box1) ;
	dcurveCalBox(tool, tol, &box2) ;
	if( dcurveInt(dcurve, &box1, tool, &box2, tol, 0, &intpts) != ERSUCSS )
	{
		rs->error = 128 ; // 求交错误
		return ERUNSUC ;
	}
	dcurveGetBeginPt(dcurve, b) ;
	dcurveGetEndPt(dcurve, e) ;
	//for( intpt = intpts ; intpt ; intpt = temp ) // 去掉与端点重合的
	//{
	//	temp = intpt->next ;
	//	if( mathDist(b, intpt->p) < tol ||
	//		mathDist(e, intpt->p) < tol )
	//		intpts = (INTPT*)mathDelItemInBDChns(intpt, intpts) ;
	//}
	if( intpts == NULL )
		return ERSUCSS ;
	intptFilter(&intpts, tol) ;
	if( intpts == NULL )
		return 0 ;

	s = 0. ;
	intpt = intpts ;
	if( dcurveIsClose(dcurve, tol) == ID_YES ) // 闭合情况从第一个交点开始
	{
		s = intpt->uv[0] ;
		memcpy(p, intpt->p, sizeof(PNT3D)) ;
		intpt = intpt->next ;
	}
	else // 不闭合情况从起点开始
		memcpy(p, b, sizeof(PNT3D)) ;
	for( ; intpt ; intpt = intpt->next )
	{
		if( intpt->prev == NULL && // first
			mathDist(b, intpt->p) < tol )
			continue ;
		dctrimAddDCurve(rs, dcurveMake(dcurve, s, intpt->uv[0], tol)) ;
		memcpy(p, intpt->p, sizeof(PNT3D)) ;
		s = intpt->uv[0] ;
	}
	if( dcurveIsClose(dcurve, tol) == ID_YES ) // 闭合的情况，需要补充一段
		dctrimAddDCurve(rs, dcurveMake(dcurve, s, intpts->uv[0], tol)) ;
	else
	if( mathDist(p, e) > tol ) // 开的情况未处理终点
		dctrimAddDCurve(rs, dcurveMake(dcurve, s, dcurve->np-1., tol)) ;
	rs->intpts = intpts ;

	return ERSUCSS ;
}
////////////////////////////////////////////////////////////////
