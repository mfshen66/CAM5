#include "stdafx.h"
#include "API\AL_API.h"
#include "CAM.h"
#include "MainFrm.h"
#include "Models.h"
#include "Mills.h"
#include "SelSet.h"
#include "CAMDoc.h"
#include "API\Smooth_API.h"
#include <fstream>

using namespace std ;

//==============================================================
/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取待光顺的曲线...") ;
	if( GetSelected(ftRefCur, 1) != 1 ) // get one curves
		return ;
	CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
	CURVE* curve = pRefCur->GetCurve() ; // curve need to be faired
	INTERVAL interval ;
	pRefCur->GetInterval(&interval) ;
	int type = entityGetType(curve) ;
	double d, e1, e2 ;
	BSCUR* nurbs ;
	if( type == etBCurve )
	{
		nurbs = curveGetNurbs(curve) ;
		e1 = bscurCalEnergy(nurbs, &interval, 6, 0.01, MIN_LEN) ;
		bscurSmoothJerk(nurbs, IDG0, 0.05, &d) ;
		e2 = bscurCalEnergy(nurbs, &interval, 6, 0.01, MIN_LEN) ;
	}

	return ;
}*/

/*@brief
 *	2014/9/2 nt
 *	create the derivative of a b-spline curve, note that the derivative
 *	of a b-spline curve is also a b-spline curve
 *@param[in] bc : b-spline curve
 *@return the derivative of bc
 */
BSCUR* bscurCreateDer(BSCUR* bc)
{
	int i, k, p, n ;
	double d ;
	BSCUR* dc ;
	
	p = bc->ord_u-1 ;
	n = bc->cnt_u-1 ;
	if( p <= 0 )
		return NULL ;
	dc = bscurCreate(p, n) ;
	if( dc == NULL )
		return NULL ; // error
	for( i = 0 ; i < dc->ord_u+dc->cnt_u ; i++ )
		dc->u_k[i] = bc->u_k[i+1] ;
	for( i = 0 ; i < dc->cnt_u ; i++ )
	{
		d = fabs(bc->u_k[i+p+1]-bc->u_k[i+1]) ;
		if( d < MIN_DBL ) // MIN_KNT
		{
			bscurFree(dc) ;
			return NULL ; // error
		}
		for( k = 0 ; k < 3 ; k++ )
			dc->u_p[i][k] = p*(bc->u_p[i+1][k]-bc->u_p[i][k])/d ;
		dc->u_p[i][3] = 1. ;
	}

	return dc ;
}

/*@brief
 *	2014/9/14 nt
 *	create the derivative of a b-spline surface, note that the derivative
 *	of a b-spline surface is also a b-spline surface
 *@param[in] bs : b-spline surface, S(u,v)
 *@return the derivative of bs, Su(u,v)
 */
BSSUR* bssurCreateDeru(BSSUR* bs)
{
	int i, j, k, p, n ;
	double d ;
	BSSUR* ds ;
	
	p = bs->ord_u-1 ;
	n = bs->cnt_u-1 ;
	if( p <= 0 )
		return NULL ;
	ds = bssurCreate(p, n, bs->ord_v, bs->cnt_v) ;
	if( ds == NULL )
		return NULL ; // error
	for( i = 0 ; i < ds->ord_u+ds->cnt_u ; i++ )
		ds->u_k[i] = bs->u_k[i+1] ;
	for( j = 0 ; j < ds->ord_v+ds->cnt_v ; j++ )
		ds->v_k[j] = bs->v_k[j] ;
	for( i = 0 ; i < ds->cnt_u ; i++ )
	{
		d = fabs(bs->u_k[i+p+1]-bs->u_k[i+1]) ;
		if( d < MIN_DBL ) // MIN_KNT
		{
			bssurFree(ds) ;
			return NULL ; // error
		}
		for( j = 0 ; j < bs->cnt_v ; j++ )
		{
			for( k = 0 ; k < 3 ; k++ )
				ds->uv_p[i][j][k] = p*(bs->uv_p[i+1][j][k]-bs->uv_p[i][j][k])/d ;
			ds->uv_p[i][j][3] = 1. ;
		}
	}

	return ds ;
}

/*@brief
 *	2014/9/14 nt
 *	create the derivative of a b-spline surface, note that the derivative
 *	of a b-spline surface is also a b-spline surface
 *@param[in] bs : b-spline surface, S(u,v)
 *@return the derivative of bs, Sv(u,v)
 */
BSSUR* bssurCreateDerv(BSSUR* bs)
{
	int i, j, k, q, m ;
	double d ;
	BSSUR* ds ;
	
	q = bs->ord_v-1 ;
	m = bs->cnt_v-1 ;
	if( q <= 0 )
		return NULL ;
	ds = bssurCreate(bs->ord_u, bs->cnt_u, q, m) ;
	if( ds == NULL )
		return NULL ; // error
	for( i = 0 ; i < ds->ord_u+ds->cnt_u ; i++ )
		ds->u_k[i] = bs->u_k[i] ;
	for( j = 0 ; j < ds->ord_v+ds->cnt_v ; j++ )
		ds->v_k[j] = bs->v_k[j+1] ;
	for( j = 0 ; j < ds->cnt_v ; j++ )
	{
		d = fabs(bs->v_k[j+q+1]-bs->v_k[j+1]) ;
		if( d < MIN_DBL ) // MIN_KNT
		{
			bssurFree(ds) ;
			return NULL ; // error
		}
		for( i = 0 ; i < bs->cnt_u ; i++ )
		{
			for( k = 0 ; k < 3 ; k++ )
				ds->uv_p[i][j][k] = q*(bs->uv_p[i][j+1][k]-bs->uv_p[i][j][k])/d ;
			ds->uv_p[i][j][3] = 1. ;
		}
	}

	return ds ;
}

void mathMatxMultx(double* matrix1,
				   double* matrix2,
				   double* matrix3,
				   int row,
				   int xxx,
				   int col)
{
    int i, j, k, l ;

    for( i = 0 ; i < row ; i++ )
	{
        for( j = 0 ; j < col ; j++ )
        {
            l = i * col + j ;
			matrix3[l] = 0.0 ;
            for( k = 0 ; k < xxx ; k++ )
                matrix3[l] += matrix1[i*xxx+k]*matrix2[k*col+j] ;
        }
	}
}

/*@brief
 *	2014/9 nt
 *	make a b-spline curve Xuuu = 0, 
 *	only modify the x coordinates
 *	the degree of curve >= 5
 *	要求bc的控制顶点分布在x轴上，
 *	控制顶点沿x轴正向分布，
 *	keep the first, middle and the last
 *	control point's x coordinates unchanged
 *@parma[in,out] bc
 */
int bscurMakeXuuuZero(BSCUR* bc)
{
	int i, k, n, rt = ERUNSUC, N, p ;
	double *as, *bs, *cs, *A, *X, *B, /**I,*/ d ;

	if( bc == NULL ||
		bc->ord_u < 6 )
		return ERINPUT ;
	p = bc->ord_u-1 ;
	n = bc->cnt_u ;
	as = (double*)envAlloc((n-1)*sizeof(double)) ;
	bs = (double*)envAlloc((n-2)*sizeof(double)) ;
	cs = (double*)envAlloc((n-3)*sizeof(double)) ;
	A = (double*)envAlloc(n*n*sizeof(double)) ;
	//I = (double*)envAlloc(n*n*sizeof(double)) ;
	X = (double*)envAlloc(n*sizeof(double)) ;
	B = (double*)envAlloc(n*sizeof(double)) ; // will solve AX = B
	if( !as || !bs || !cs || !A || !X || !B )
		return ERMEM ;
	N = n*n ;
	for( i = 0 ; i < N ; i++ )
		A[i] = 0. ;

	for( i = 0 ; i < n-1 ; i++ ) // 一阶导数Qi = as[i]*(Pi+1 - Pi)
	{
		d = fabs(bc->u_k[i+p+1]-bc->u_k[i+1]) ;
		if( d < MIN_DBL ) // MIN_KNT
			goto ExceExit ;
		as[i] = p/d ;
	}
	for( i = 0 ; i < n-2 ; i++ ) // 二阶导数Ri = bs[i]*(Qi+1 - Qi)
	{
		d = fabs(bc->u_k[i+p+1]-bc->u_k[i+2]) ;
		if( d < MIN_DBL ) // MIN_KNT
			goto ExceExit ;
		bs[i] = (p-1)/d ;
	}
	for( i = 0 ; i < n-3 ; i++ ) // 三阶导数Li = cs[i]*(Ri+1 - Ri)
	{
		d = fabs(bc->u_k[i+p+1]-bc->u_k[i+3]) ;
		if( d < MIN_DBL ) // MIN_KNT
			goto ExceExit ;
		cs[i] = (p-2)/d ;
	}
	d = 0. ;
	for( i = 0 ; i < n-3 ; i++ )
	{
		A[n*i+i+0] = -cs[i]*bs[i]*as[i] ;
		A[n*i+i+1] = cs[i]*((bs[i+1]+bs[i])*as[i+1]+bs[i]*as[i]) ;
		A[n*i+i+2] = -cs[i]*(bs[i+1]*as[i+2]+(bs[i+1]+bs[i])*as[i+1]) ;
		A[n*i+i+3] = cs[i]*bs[i+1]*as[i+2] ;
		for( k = 0 ; k < 4 ; k++ )
		{
			if( d < fabs(A[n*i+i+k]) )
				d = fabs(A[n*i+i+k]) ;
		}
		B[i] = 0. ;
	}
	if( d < MIN_DBL )
		return 0 ;
	// |A[n*i+i+k]|可能太大影响解方程
	for( i = 0 ; i < n-3 ; i++ )
	{
		for( k = 0 ; k < 4 ; k++ )
			A[n*i+i+k] /= d ;
	}
	A[n*(n-3)+0] = 1. ;
	B[n-3] = bc->u_p[0][0] ; // 保持第一个控制顶点的x坐标不变
	A[n*(n-2)+n-1] = 1. ;
	B[n-2] = bc->u_p[n-1][0] ; // 保持最后一个控制顶点的x坐标不变
	i = n/2 ;
	A[n*(n-1)+i] = 1. ;
	B[n-1] = bc->u_p[i][0] ;

	FILE* fp ;
	fopen_s(&fp, "d:\\1111.txt", "w+t") ;
	for( i = 0 ; i < N ; i++ )
		fprintf(fp, "%16lf\n", A[i]) ;
	for( i = 0 ; i < n ; i++ )
		fprintf(fp, "%16lf\n", B[i]) ;
	fclose(fp) ;

	//mathMatxInvs(n, n, A, I) ;
	//mathMatxMultx(I, B, X, n, n, 1) ;
	mLE(n, A, B, X) ;
	for( i = 0 ; i < n ; i++ ) // write back
		bc->u_p[i][0] = X[i] ;

	rt = ERSUCSS ;
ExceExit:
	envFree(as) ;
	envFree(bs) ;
	envFree(cs) ;
	envFree(A) ;
	envFree(X) ;
	envFree(B) ;

	return rt ;
}

/*@brief
 *	2014/9/14 nt
 *	make a b-spline surface Xuuu = 0,
 *	only modify the x coordinates
 *	suppose that the degree of curve >= 5
 *	要求bs的控制顶点分布在xy平面上的矩形区域内，
 *	u向控制顶点沿x轴正向，v向控制顶点沿y轴正向，
 *	keep the first, middle and the last
 *	control point's x coordinates unchanged
 *@parma[in,out] bs
 */
int bssurMakeXuuuZero(BSSUR* bs)
{
	int i, j ;
	BSCUR* bc = NULL ;

	bc = bscurCreate(bs->ord_u, bs->cnt_u) ;
	memcpy(bc->u_k, bs->u_k, sizeof(double)*(bs->ord_u+bs->cnt_u)) ;
	for( i = 0 ; i < bs->cnt_u ; i++ )
	{
		bc->u_p[i][0] = bs->uv_p[i][0][0] ;
		bc->u_p[i][1] = 0. ;
		bc->u_p[i][2] = 0. ;
		bc->u_p[i][3] = 1. ;
	}
	if( bscurMakeXuuuZero(bc) != ERSUCSS ) // adjust x coordinates
	{
		bscurFree(bc) ;
		return ERUNSUC ;
	}
	for( i = 0 ; i < bs->cnt_u ; i++ )
		for( j = 0 ; j < bs->cnt_v ; j++ )
			bs->uv_p[i][j][0] = bc->u_p[i][0] ;
	bscurFree(bc) ;
	bc = NULL ;

	return ERSUCSS ;
}

/*@brief
 *	2014/9/14 nt
 *	make a b-spline surface Yuuu = 0,
 *	only modify the y coordinates
 *	suppose that the degree of curve >= 5
 *	要求bs的控制顶点分布在xy平面上的矩形区域内，
 *	u向控制顶点沿x轴正向，v向控制顶点沿y轴正向，
 *	keep the first, middle and the last
 *	control point's y coordinates unchanged
 *@parma[in,out] bs
 */
int bssurMakeYuuuZero(BSSUR* bs)
{
	int i, j ;
	BSCUR* bc = NULL ;

	bc = bscurCreate(bs->ord_v, bs->cnt_v) ;
	memcpy(bc->u_k, bs->v_k, sizeof(double)*(bs->ord_v+bs->cnt_v)) ;
	for( j = 0 ; j < bs->cnt_v ; j++ )
	{
		bc->u_p[j][0] = bs->uv_p[0][j][1] ;
		bc->u_p[j][1] = 0. ;
		bc->u_p[j][2] = 0. ;
		bc->u_p[j][3] = 1. ;
	}
	if( bscurMakeXuuuZero(bc) != ERSUCSS ) // adjust x coordinates
	{
		bscurFree(bc) ;
		return ERUNSUC ;
	}
	for( i = 0 ; i < bs->cnt_u ; i++ )
		for( j = 0 ; j < bs->cnt_v ; j++ )
			bs->uv_p[i][j][1] = bc->u_p[j][0] ;
	bscurFree(bc) ;
	bc = NULL ;

	return ERSUCSS ;
}
//--------------------------------------------------------------
/*void CCAMDoc::OnTest()
{
	BSSUR* nurbs = bssurCreate(3, 3, 3, 3) ;

	int i, j, n = 0 ;
	for( i = 0 ; i < 3 ; i++ )
	{
		for( j = 0 ; j < 3 ; j++ )
		{
			nurbs->uv_p[i][j][0] = 0.5*i ;
			nurbs->uv_p[i][j][1] = 0.5*j ;
			nurbs->uv_p[i][j][2] = 0. ;
		}
	}
	nurbs->uv_p[1][1][2] = 0.75 ;
	nurbs->uv_p[1][0][2] = 0.5 ;
	nurbs->uv_p[1][2][2] = 0.5 ;
	nurbs->uv_p[0][1][2] = 0.5 ;
	nurbs->uv_p[2][1][2] = 0.5 ;

	RFRAME lf ;
	mathInitRFrame(&lf) ;
	lf.O[0] = 0.5 ;
	lf.O[1] = 0.5 ;
	lf.O[2] = 0.5 ;
	CURVE* circle = circleCreate(&lf, 0.25) ;

	FILE* fp ;
	PNT3D p ;
	fopen_s(&fp, "d:\\2.txt", "w+t") ;
	for( i = 0 ; i <= 10 ; i++ )
	{
		for( j = 0 ; j <= 10 ; j++ )
		{
			if( i == 0 ||
				i == 10 ||
				j == 0 ||
				j == 10 )
			{
				bssurEvalPos(nurbs, 0.1*i, 0.1*j, p) ;
				fprintf_s(fp, "%lf %lf %lf\n", p[0], p[1], p[2]) ;
				n++ ;
			}
		}
	}
	for( i = 0 ; i < 50 ; i++ )
	{
		curveEvalPos(circle, i*0.02*PI2, p) ;
		fprintf_s(fp, "%lf %lf %lf\n", p[0], p[1], p[2]) ;
		n++ ;
	}
	fclose(fp) ;

	return ;
}*/

/*void CCAMDoc::OnTest()
{
	int i, j, n, i0, j0 ;
	double (*ps)[3] = new double[500][3], u, v, d, Nu[20], Nv[20] ;
	PNT3D p ;

	FILE* fp ;
	fopen_s(&fp, "d:\\6.txt", "r+t") ;
	fscanf_s(fp, "%d", &n) ;
	for( i = 0 ; i < n ; i++ )
		fscanf_s(fp, "%lf %lf %lf", &(ps[i][0]), &(ps[i][1]), &(ps[i][2])) ;
	fclose(fp) ;
	for( i = 0 ; i < n ; i++ )
	{
		ps[i][0] *= 100. ;
		ps[i][1] *= 100. ;
		ps[i][2] *= 100. ;
	}

	BSSUR* nurbs ;
	nurbs = bssurCreate(6, 30, 6, 30) ; // 准均匀B样条
	//nurbs = bssurCreate(20, 20, 20, 20) ; // Bezier
	for( i = 0 ; i < 30 ; i++ )
	{
		for( j = 0 ; j < 30 ; j++ )
		{
			nurbs->uv_p[i][j][0] = i*100./29. ;
			nurbs->uv_p[i][j][1] = j*100./29. ;
			nurbs->uv_p[i][j][2] = 0. ;
			nurbs->uv_p[i][j][3] = 1. ;
		}
	}
	double dd = 1./25 ;
	for( i = 6 ; i < 30 ; i++ )
	{
		nurbs->u_k[i] = (i-5)*dd ;
		nurbs->v_k[i] = (i-5)*dd ;
	}
	bssurMakeXuuuZero(nurbs) ;
	bssurMakeYuuuZero(nurbs) ;

	fopen_s(&fp, "d:\\6SurfaceSmoothData2.txt", "w+t") ;
	bssurSaveASCII(nurbs, fp) ;
	fprintf_s(fp, "%d\n", n) ;
	for( i = 0 ; i < n ; i++ )
	{
		fprintf_s(fp, "\n") ;
		fprintf_s(fp, "%16.16lf %16.16lf %16.16lf\n", ps[i][0], ps[i][1], ps[i][2]) ;
		u = ps[i][0]/100. ;
		v = ps[i][1]/100. ;
		bssurSnap(nurbs, ps[i], 1.e-5, p, &u, &v, &d) ;
		fprintf_s(fp, "%16.16lf %16.16lf\n", u, v) ;

		kvectorEvalBFun(nurbs->ord_u,
						nurbs->cnt_u,
						nurbs->u_k,
					    u,
					    Nu,
					    NULL,
					    NULL,
					    &i0) ;
		i0 = i0-nurbs->ord_u+1 ;
		fprintf_s(fp, "%d\n", i0) ;
		for( j = 0 ; j < nurbs->ord_u ; j++ )
			fprintf_s(fp, "%16.16lf ", Nu[j]) ;
		fprintf_s(fp, "\n") ;
		kvectorEvalBFun(nurbs->ord_v,
						nurbs->cnt_v,
						nurbs->v_k,
					    v,
					    Nv,
					    NULL,
					    NULL,
					    &j0) ;
		j0 = j0-nurbs->ord_v+1 ;
		fprintf_s(fp, "%d\n", j0) ;
		for( j = 0 ; j < nurbs->ord_v ; j++ )
			fprintf_s(fp, "%16.16lf ", Nv[j]) ;
		fprintf_s(fp, "\n") ;
	}
	fclose(fp) ;

	bssurFree(nurbs) ;
	delete []ps ;

	return ;
}//*/

/*void CCAMDoc::OnTest()
{
	int i, n ;
	PNT3D p ;
	FILE* fp ;

	fopen_s(&fp, "d:\\1.txt", "r+t") ;
	fscanf_s(fp, "%d", &n) ;
	for( i = 0 ; i < n ; i++ )
	{
		fscanf_s(fp, "%lf %lf %lf", &p[0], &p[1], &p[2]) ;
		p[0] *= 100. ;
		p[1] *= 100. ;
		p[2] *= 100. ;
		CRefPt* pRefPt = new CRefPt(0, p) ;
		AddFeature(pRefPt) ;
	}
	fclose(fp) ;
}*/

int pnetCalBiVar(int nu, int nv, double (**uvs)[4], int dim, double bvs[3])
{
	int i, j, k, s, t, as[4], cs[4][4] ;
	double d, fu = 0., fv = 0., f = 0., ds[4] ;

	if( nu < 7 || nv < 7 )
		return 0 ;

	cs[0][0] = -12 ;
	cs[0][1] = 21 ;
	cs[0][2] = -11 ;
	cs[0][3] = 2 ;
	cs[1][0] = -9 ;
	cs[1][1] = 19 ;
	cs[1][2] = -13 ;
	cs[1][3] = 3 ;
	cs[2][0] = -80 ;
	cs[2][1] = 185 ;
	cs[2][2] = -141 ;
	cs[2][3] = 36 ;
	cs[3][0] = -5 ;
	cs[3][1] = 13 ;
	cs[3][2] = -12 ;
	cs[3][3] = 4 ;
	ds[0] = 5. ;
	ds[1] = 5./6 ;
	ds[2] = 1./36 ;
	ds[3] = 0.25 ;

	for( i = 0 ; i < nu ; i++ )
	{
		//for( j = 0 ; j <= nv-4 ; j++ )
		for( j = 4 ; j <= nv-8 ; j++ )
		{
			d = uvs[i][j][dim]-3*uvs[i][j+1][dim]+3*uvs[i][j+2][dim]-uvs[i][j+3][dim] ;
			if( fv < fabs(d) )
				fv = fabs(d) ;
		}
		for( j = 0 ; j < 4 ; j++ )
		{
			d = 0. ;
			for( k = 0 ; k < 4 ; k++ )
				d += cs[j][k]*uvs[i][j+k][dim] ;
			d *= ds[j] ;
			if( fv < fabs(d) )
				fv = fabs(d) ;
			d = 0. ;
			for( k = 0 ; k < 4 ; k++ )
				d += cs[j][k]*uvs[i][nv-1-j-k][dim] ;
			d *= ds[j] ;
			if( fv < fabs(d) )
				fv = fabs(d) ;
		}
	}

	for( j = 0 ; j < nv ; j++ )
	{
		//for( i = 0 ; i <= nu-4 ; i++ )
		for( i = 4 ; i <= nu-8 ; i++ )
		{
			d = uvs[i][j][dim]-3*uvs[i+1][j][dim]+3*uvs[i+2][j][dim]-uvs[i+3][j][dim] ;
			if( fu < fabs(d) )
				fu = fabs(d) ;
		}
		for( i = 0 ; i < 4 ; i++ )
		{
			d = 0. ;
			for( k = 0 ; k < 4 ; k++ )
				d += cs[i][k]*uvs[i+k][j][dim] ;
			d *= ds[i] ;
			if( fu < fabs(d) )
				fu = fabs(d) ;
			d = 0. ;
			for( k = 0 ; k < 4 ; k++ )
				d += cs[i][k]*uvs[nu-1-i-k][j][dim] ;
			d *= ds[i] ;
			if( fu < fabs(d) )
				fu = fabs(d) ;
		}
	}

	as[0] = 1 ;
	as[1] = -3 ;
	as[2] = 3 ;
	as[3] = -1 ;
	for( i = 0 ; i <= nu-4 ; i++ )
	{
		for( j = 0 ; j <= nv-4 ; j++ )
		{
			d = 0. ;
			for( s = 0 ; s < 4 ; s++ )
			{
				for( t = 0 ; t < 4 ; t++ )
				{
					d += uvs[i+s][j+t][dim]*as[s]*as[t] ;
				}
			}
			if( f < fabs(d) )
				f = fabs(d) ;
		}
	}

	bvs[0] = fu ;
	bvs[1] = fv ;
	bvs[2] = f ;

	return 1 ;
}

// 显示曲面三阶导数模长分布情况
/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取若干张NURBS曲面...") ;
	int n = GetSelected(ftRefSur, 1) ; // surfaces
	if( n > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		CMark* pMark = new CMark() ;
		int i, j, k ;
		double u, v, d, uu, vv, stepu, stepv, max = 0. ;
		PNT3D p, q ;
		VEC3D normal, du, dv, duu, duv, dvv, duu2, dvv2 ;
		stepu = (boxuv.max[0]-boxuv.min[0])/25 ;
		stepv = (boxuv.max[1]-boxuv.min[1])/25 ;
		uu = 0.0001*stepu ;
		vv = 0.0001*stepv ;
		for( i = 0 ; i < 25 ; i++ )
		{
			u = boxuv.min[0]+(i+0.5)*stepu ;
			for( j = 0 ; j < 25 ; j++ )
			{
				v = boxuv.min[1]+(j+0.5)*stepv ;
				surfaceEvalDer2O(surface, u, v, p, du, dv, duu, duv, dvv) ;
				mathVProduct(du, dv, normal) ;
				mathUniVec(normal, MIN_LEN) ;
				surfaceEvalDer2O(surface, u+uu, v, q, du, dv, duu2, duv, dvv2) ;
				for( k = 0 ; k < 3 ; k++ )
				{
					duu2[k] = (duu2[k]-duu[k])/uu ;
					dvv2[k] = (dvv2[k]-dvv[k])/vv ;
				}
				d = 0.01*mathVecLen(duu2) ; // 三阶导数模长
				if( max < d )
					max = d ;
				mathEvalLin(p, normal, d, q) ;
				pMark->AddLin(p, q) ;
			}
		}
		AddFeature(pMark) ;
		max *= 100. ;
		char str[64] ;
		sprintf_s(str, 64, "Suuu模长最大值=%lf", max) ;
		camPromptStr(str) ;
	}
	if( n > 0 )
		Redraw() ;

	return ;
}//*/

// 显示曲面(duu,duuu)分布情况
/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取若干张NURBS曲面...") ;
	int n = GetSelected(ftRefSur, 1) ; // surfaces
	if( n > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		CMark* pMark = new CMark() ;
		int i, j, k ;
		double u, v, d, uu, vv, stepu, stepv ;
		PNT3D p, q ;
		VEC3D normal, du, dv, duu, duv, dvv, duu2, dvv2 ;
		stepu = (boxuv.max[0]-boxuv.min[0])/25 ;
		stepv = (boxuv.max[1]-boxuv.min[1])/25 ;
		uu = 0.001*stepu ;
		vv = 0.001*stepv ;
		for( i = 0 ; i < 25 ; i++ )
		{
			u = boxuv.min[0]+(i+0.5)*stepu ;
			for( j = 0 ; j < 25 ; j++ )
			{
				v = boxuv.min[1]+(j+0.5)*stepv ;
				surfaceEvalDer2O(surface, u, v, p, du, dv, duu, duv, dvv) ;
				mathVProduct(du, dv, normal) ;
				mathUniVec(normal, MIN_LEN) ;
				surfaceEvalDer2O(surface, u+uu, v, q, du, dv, duu2, duv, dvv2) ;
				for( k = 0 ; k < 3 ; k++ )
				{
					duu2[k] = (duu2[k]-duu[k])/uu ;
					dvv2[k] = (dvv2[k]-dvv[k])/vv ;
				}
				d = 0.01*mathOProduct(duu, duu2) ;
				mathEvalLin(p, normal, d, q) ;
				pMark->AddLin(p, q) ;
			}
		}
		AddFeature(pMark) ;
	}
	if( n > 0 )
		Redraw() ;

	return ;
}//*/

/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取若干张NURBS曲面...") ;
	int n = GetSelected(ftRefSur, 1) ; // surfaces
	if( n > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		FILE* fp ;
		fopen_s(&fp, "d:\\6.txt", "r+t") ;
		int i, n ;
		double u, v, d, dev = 0. ;
		PNT3D p, q ;
		fscanf_s(fp, "%d", &n) ;
		for( i = 0 ; i < n ; i++ )
		{
			fscanf_s(fp, "%lf %lf %lf", p, p+1, p+2) ;
			u = p[0] ;
			v = p[1] ;
			p[0] *= 100 ;
			p[1] *= 100 ;
			p[2] *= 100 ;
			surfaceSnap(surface, p, q, &u, &v) ;
			d = mathDist(p, q) ;
			if( dev < d )
				dev = d ;
		}
		fclose(fp) ;
		char str[64] ;
		sprintf_s(str, 64, "%lf", dev) ;
		camPromptStr(str) ;
	}
}*/

// 检查NURBS曲面的三阶导数模长最大值、三阶导数曲面控制顶点情况及bvs
/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取NURBS曲面...") ;
	int n = GetSelected(ftRefSur, 1) ; // surfaces
	if( n > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BSSUR* nurbs = surfaceGetNurbs(surface) ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		BSSUR* derU = bssurCreateDeru(nurbs) ;
		BSSUR* derUU = bssurCreateDeru(derU) ;
		BSSUR* derUUU = bssurCreateDeru(derUU) ;
		BSSUR* derV = bssurCreateDerv(nurbs) ;
		BSSUR* derVV = bssurCreateDerv(derV) ;
		BSSUR* derVVV = bssurCreateDerv(derVV) ;

		int i, j, k ;
		double d, SuuuVertexMax, SvvvVertexMax, bvs[3] ;
		FILE* fp ;
		fopen_s(&fp, "d:\\SuuuSvvvBVS.txt", "w+t") ;
		for( k = 0 ; k < 3 ; k++ )
		{
			fprintf_s(fp, "%d\n", k) ;
			SuuuVertexMax = 0. ;
			for( i = 0 ; i < derUUU->cnt_u ; i++ )
			{
				for( j = 0 ; j < derUUU->cnt_v ; j++ )
				{
					d = fabs(derUUU->uv_p[i][j][k]) ;
					if( SuuuVertexMax < d )
						SuuuVertexMax = d ;
				}
			}
			fprintf_s(fp, "SuuuVertexMax=%16.16lf\n", SuuuVertexMax) ;

			SvvvVertexMax = 0. ;
			for( i = 0 ; i < derVVV->cnt_u ; i++ )
			{
				for( j = 0 ; j < derVVV->cnt_v ; j++ )
				{
					d = fabs(derVVV->uv_p[i][j][k]) ;
					if( SvvvVertexMax < d )
						SvvvVertexMax = d ;
				}
			}
			fprintf_s(fp, "SvvvVertexMax=%16.16lf\n", SvvvVertexMax) ;

			pnetCalBiVar(nurbs->cnt_u, nurbs->cnt_v, nurbs->uv_p, k, bvs) ;
			fprintf_s(fp, "bvs[0]=%16.16lf bvs[1]=%16.16lf bvs[2]=%16.16lf\n",
				      bvs[0], bvs[1], bvs[2]) ;
		}

		double u, v, uu, vv, stepu, stepv, SuuuMax = 0., SvvvMax = 0. ;
		PNT3D p, q ;
		VEC3D du, dv, duu, duv, dvv, duu2, dvv2 ;
		stepu = (boxuv.max[0]-boxuv.min[0])/100 ;
		stepv = (boxuv.max[1]-boxuv.min[1])/100 ;
		uu = 0.001*stepu ;
		vv = 0.001*stepv ;
		for( i = 1 ; i < 100 ; i++ )
		{
			u = boxuv.min[0]+i*stepu ;
			for( j = 1 ; j < 100 ; j++ )
			{
				v = boxuv.min[1]+j*stepv ;
				surfaceEvalDer2O(surface, u-uu, v, p, du, dv, duu, duv, dvv) ;
				surfaceEvalDer2O(surface, u+uu, v, q, du, dv, duu2, duv, dvv2) ;
				for( k = 0 ; k < 3 ; k++ )
					duu2[k] = 0.5*(duu2[k]-duu[k])/uu ;
				d = mathVecLen(duu2) ; // 三阶导数模长
				if( SuuuMax < d )
					SuuuMax = d ;
				surfaceEvalDer2O(surface, u, v-vv, p, du, dv, duu, duv, dvv) ;
				surfaceEvalDer2O(surface, u, v+vv, q, du, dv, duu2, duv, dvv2) ;
				for( k = 0 ; k < 3 ; k++ )
					dvv2[k] = 0.5*(dvv2[k]-dvv[k])/vv ;
				d = mathVecLen(dvv2) ; // 三阶导数模长
				if( SvvvMax < d )
					SvvvMax = d ;
			}
		}
		fprintf_s(fp, "SuuuMax=%16.16lf\n", SuuuMax) ;
		fprintf_s(fp, "SvvvMax=%16.16lf\n", SvvvMax) ;
		fclose(fp) ;

		bssurFree(derU) ;
		bssurFree(derUU) ;
		bssurFree(derUUU) ;
		bssurFree(derV) ;
		bssurFree(derVV) ;
		bssurFree(derVVV) ;
	}

	return ;
}//*/

/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取NURBS曲面...") ;
	int n = GetSelected(ftRefSur, 1) ; // surfaces
	if( n > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BSSUR* nurbs = surfaceGetNurbs(surface) ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;

		BSCUR* ws = bscurCreate(4, 4) ;
		int i, j ;
		for( i = 0 ; i < 4 ; i++ )
		{
			ws->u_p[i][0] = i/3. ;
			ws->u_p[i][1] = 1. ;
			ws->u_p[i][2] = 0. ;
			ws->u_p[i][3] = 1. ;
		}
		ws->u_p[1][1] = 0.9 ;
		ws->u_p[2][1] = 0.9 ;

		double t, wu, wv ;
		PNT3D p ;
		for( i = 0 ; i < nurbs->cnt_u ; i++ )
		{
			t = (double)i/(nurbs->cnt_u-1) ;
			bscurEvalPos(ws, t, p) ;
			wu = p[1] ;
			for( j = 0 ; j < nurbs->cnt_v ; j++ )
			{
				t = (double)j/(nurbs->cnt_v-1) ;
				bscurEvalPos(ws, t, p) ;
				wv = p[1] ;
				nurbs->uv_p[i][j][3] = wu*wv ;
				nurbs->uv_p[i][j][0] *= nurbs->uv_p[i][j][3] ;
				nurbs->uv_p[i][j][1] *= nurbs->uv_p[i][j][3] ;
				nurbs->uv_p[i][j][2] *= nurbs->uv_p[i][j][3] ;
			}
		}
		FILE* fp ;
		fopen_s(&fp, "d:\\6BS2改了权因子.GE", "w+t") ;
		fprintf_s(fp, "PANDACAD-GE\nBSSUR\n") ;
		bssurSaveASCII(nurbs, fp) ;
		fclose(fp) ;
	}

	return ;
}*/

// 计算曲面的二阶导数上界
/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取曲面...") ;
	int i, n = GetSelected(ftRefSur, 1) ;
	double u, v, d, ub1[2], ub2[3], m1[2], m2[3] ;
	PNT3D p ;
	VEC3D du, dv, duu, duv, dvv ;

	if( n > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BSSUR* nurbs = surfaceGetNurbs(surface) ;
		if( nurbs )
		{
			bssurCalUB1(nurbs, ub1) ;
			bssurCalUB2(nurbs, ub2) ;
			m1[0] = 0. ;
			m1[1] = 0. ;
			m2[0] = 0. ;
			m2[1] = 0. ;
			m2[2] = 0. ;
			for( i = 0 ; i < 10000000 ; i++ )
			{
				u = mathRand01() ;
				v = mathRand01() ;
				bssurEvalDer2O(nurbs, u, v, p, du, dv, duu, duv, dvv) ;
				d = mathVecLen(du) ;
				if( m1[0] < d )
					m1[0] = d ;
				d = mathVecLen(dv) ;
				if( m1[1] < d )
					m1[1] = d ;
				d = mathVecLen(duu) ;
				if( m2[0] < d )
					m2[0] = d ;
				d = mathVecLen(duv) ;
				if( m2[1] < d )
					m2[1] = d ;
				d = mathVecLen(dvv) ;
				if( m2[2] < d )
					m2[2] = d ;
			}
		}
	}
	i = 0 ;

	return ;
}*/

double mathClamp01(double d)
{
	if( d < 0. )
		return 0. ;
	else
	if( d > 1. )
		return 1. ;
	else
		return d ;
 }

/*	1997/6/16 nt
 *	在线段上求一点与已知点距离最近
 */
double mathGetPntSegmNearestPnt(PNT3D p,
							    PNT3D begin,
							    PNT3D end,
							    PNT3D nearest_p)
{
    VEC3D v1, v2 ;
    double A, B, t ; //, C ;

    v1[0] = end[0]-begin[0] ;
	v1[1] = end[1]-begin[1] ;
	v1[2] = end[2]-begin[2] ;
    v2[0] = begin[0]-p[0] ;
	v2[1] = begin[1]-p[1] ;
	v2[2] = begin[2]-p[2] ;
    A = v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2] ;
    B = v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2] ;
    //C = v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2] ;
    t = A > 1.e-20 ? mathClamp01(-B/A) : 0.0 ;
    nearest_p[0] = begin[0]+t*v1[0] ;
    nearest_p[1] = begin[1]+t*v1[1] ;
    nearest_p[2] = begin[2]+t*v1[2] ;

	return mathDist(p, nearest_p) ;
}

/*	2003/4/16 nt
 *	find a 3d point in a 3d triangle which is nearest to the given 3d point
 */
double mathGetTriNearestPnt(PNT3D p0, 
						    PNT3D p1, 
						    PNT3D p2,
						    PNT3D p,
						    double tol,
						    PNT3D nearest_p)
						 
{
	int k ;
	PNT3D prj_p ;
	VEC3D v0, v1, v2, n0, n1, n2, n ;

	// special vectors
	for( k = 0 ; k < 3 ; k++ )
	{
		v0[k] = p2[k]-p1[k] ;
		v1[k] = p0[k]-p2[k] ;
		v2[k] = p1[k]-p0[k] ;
	}
	if( mathUniVec(v0, tol) != ERSUCSS )
		return mathGetPntSegmNearestPnt(p, p0, p1, nearest_p) ; // degenerative
	if(	mathUniVec(v1, tol) != ERSUCSS )
		return mathGetPntSegmNearestPnt(p, p1, p2, nearest_p) ; // degenerative
	if( mathUniVec(v2, tol) != ERSUCSS )
		return mathGetPntSegmNearestPnt(p, p2, p0, nearest_p) ; // degenerative
	mathPrjPntLin(p0, p1, v0, prj_p) ;
	n0[0] = prj_p[0]-p0[0] ;
	n0[1] = prj_p[1]-p0[1] ;
	n0[2] = prj_p[2]-p0[2] ;
	if( mathUniVec(n0, tol) != ERSUCSS )
		return mathGetPntSegmNearestPnt(p, p1, p2, nearest_p) ; // degenerative
	mathPrjPntLin(p1, p2, v1, prj_p) ;
	n1[0] = prj_p[0]-p1[0] ;
	n1[1] = prj_p[1]-p1[1] ;
	n1[2] = prj_p[2]-p1[2] ;
	if( mathUniVec(n1, tol) != ERSUCSS )
		return mathGetPntSegmNearestPnt(p, p2, p0, nearest_p) ; // degenerative
	mathPrjPntLin(p2, p0, v2, prj_p) ;
	n2[0] = prj_p[0]-p2[0] ;
	n2[1] = prj_p[1]-p2[1] ;
	n2[2] = prj_p[2]-p2[2] ;
	if( mathUniVec(n2, tol) != ERSUCSS )
		return mathGetPntSegmNearestPnt(p, p0, p1, nearest_p) ; // degenerative

	// corners
	if( (p[0]-p0[0])*v1[0]+(p[1]-p0[1])*v1[1]+(p[2]-p0[2])*v1[2] > -MIN_DBL &&
		(p[0]-p0[0])*v2[0]+(p[1]-p0[1])*v2[1]+(p[2]-p0[2])*v2[2] < MIN_DBL )
	{
		memcpy(nearest_p, p0, sizeof(PNT3D)) ;
		return mathDist(p0, p) ;
	}
	if( (p[0]-p1[0])*v0[0]+(p[1]-p1[1])*v0[1]+(p[2]-p1[2])*v0[2] < MIN_DBL &&
		(p[0]-p1[0])*v2[0]+(p[1]-p1[1])*v2[1]+(p[2]-p1[2])*v2[2] > -MIN_DBL )
	{
		memcpy(nearest_p, p1, sizeof(PNT3D)) ;
		return mathDist(p1, p) ;
	}
	if( (p[0]-p2[0])*v0[0]+(p[1]-p2[1])*v0[1]+(p[2]-p2[2])*v0[2] > -MIN_DBL &&
		(p[0]-p2[0])*v1[0]+(p[1]-p2[1])*v1[1]+(p[2]-p2[2])*v1[2] < MIN_DBL )
	{
		memcpy(nearest_p, p2, sizeof(PNT3D)) ;
		return mathDist(p2, p) ;
	}
	
	// edges
	if( (p[0]-p2[0])*v0[0]+(p[1]-p2[1])*v0[1]+(p[2]-p2[2])*v0[2] < MIN_DBL &&
		(p[0]-p1[0])*v0[0]+(p[1]-p1[1])*v0[1]+(p[2]-p1[2])*v0[2] > -MIN_DBL &&
		(p[0]-p1[0])*n0[0]+(p[1]-p1[1])*n0[1]+(p[2]-p1[2])*n0[2] > -MIN_DBL )
	{
		mathPrjPntLin(p, p1, v0, prj_p) ;
		return mathDist(p, prj_p) ;
	}
	if( (p[0]-p0[0])*v1[0]+(p[1]-p0[1])*v1[1]+(p[2]-p0[2])*v1[2] < MIN_DBL &&
		(p[0]-p2[0])*v1[0]+(p[1]-p2[1])*v1[1]+(p[2]-p2[2])*v1[2] > -MIN_DBL &&
		(p[0]-p2[0])*n1[0]+(p[1]-p2[1])*n1[1]+(p[2]-p2[2])*n1[2] > -MIN_DBL )
	{
		mathPrjPntLin(p, p2, v1, prj_p) ;
		return mathDist(p, prj_p) ;
	}
	if( (p[0]-p1[0])*v2[0]+(p[1]-p1[1])*v2[1]+(p[2]-p1[2])*v2[2] < MIN_DBL &&
		(p[0]-p0[0])*v2[0]+(p[1]-p0[1])*v2[1]+(p[2]-p0[2])*v2[2] > -MIN_DBL &&
		(p[0]-p0[0])*n2[0]+(p[1]-p0[1])*n2[1]+(p[2]-p0[2])*n2[2] > -MIN_DBL )
	{
		mathPrjPntLin(p, p0, v2, prj_p) ;
		return mathDist(p, prj_p) ;
	}

	// inner
	mathVProduct(v0, v1, n) ;
	if( mathUniVec(n, 1.e-20) != ERSUCSS )
		return ERDEGE ;
	mathPrjPntPln(p, p0, n, prj_p) ;
	return mathDist(p, prj_p) ;
}

typedef double (*PSTR3)[4] ;
typedef double (**PNET)[4] ;
PNET pnetCreate(int nu, int nv) ;
void pnetFree(PNET pnet) ;

PNET pnetCreate(int nu, int nv)
{
	PNET pnet = new PSTR3[nu] ;
	for( int i = 0 ; i < nu ; i++ )
		pnet[i] = new double[nv][4] ;

	return pnet ;
}

void pnetFree(int nu, PNET pnet)
{
	if( pnet )
	{
		for( int i = 0 ; i < nu ; i++ )
			delete []pnet[i] ;
		delete pnet ;
	}

	return ;
}

/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取曲面...") ;
	int k = GetSelected(ftRefSur, 1) ;
	double d, ub1[2], ub2[3] ;
	PNT3D p, q ;
	int i, j, n = 35, m = 35 ; // n,m是u,v方向上分割的段数
	double (**grid)[4], u, v, dev = 0. ;
	grid = pnetCreate(n+1, m+1) ;

	if( k > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BSSUR* nurbs = surfaceGetNurbs(surface) ;
		if( !nurbs )
			return ;
		bssurCalUB1(nurbs, ub1) ;
		bssurCalUB2(nurbs, ub2) ;

		for( i = 0 ; i <= n ; i++ )
		{
			u = (double)i/n ;
			for( j = 0 ; j <= m ; j++ )
			{
				v = (double)j/m ;
				bssurEvalPos(nurbs, u, v, grid[i][j]) ;
			}
		}

		for( i = 0 ; i < n ; i++ )
		{
			for( j = 0 ; j < m ; j++ )
			{
				for( k = 0 ; k < 10000 ; k++ )
				{
					u = mathRand01()/n ;
					v = mathRand01()/m ;
					bssurEvalPos(nurbs, (double)i/n+u, (double)j/m+v, p) ;
					d = mathGetTriNearestPnt(grid[i][j], 
											 grid[i+(u>v?1:0)][j+(u>v?0:1)], 
											 grid[i+1][j+1],
											 p,
											 0.00001,
											 q) ;
					if( dev < d )
						dev = d ;
				}
			}
		}
	}
	pnetFree(n+1, grid) ;

	return ;
}*/

void mathMixVec3(int dim, 
				 double* v1, 
				 double* v2, 
				 double* v3,
				 double w1,
				 double w2,
				 double w3,
				 double* rtvec)
{
	int i ;

	for( i = 0 ; i < dim ; i++ )
	{
		rtvec[i] = w1*v1[i]+w2*v2[i]+w3*v3[i] ;
	}

	return ;
}

int mathEstm3PCur2(PNT3D A, PNT3D B, PNT3D C, double* pcur)
{
	double dAB, dBC, tB, d, dd ;
	PNT3D q ;
	VEC3D tvec, dvec, v ;

	*pcur = 0. ;
	dAB = mathDist(A, B) ;
	dBC = mathDist(B, C) ;
	if( dAB < MIN_LEN ||
		dBC < MIN_LEN )
		return 0 ; //ERDEGE ;
	tB = dAB/(dAB+dBC) ; // 0. < tB < 1.
	d = (1-tB)*tB ;
	dd = (1-tB)/tB ;
	// q = 0.5*(B/d-dd*A-C/dd) ;
	mathMixVec3(3, B, A, C, 0.5/d, -0.5*dd, -0.5/dd, q) ;
	// bezier curve = (1-t)^2*A+2(1-t)t*q+t^2*C
	mathMixVec3(3, A, q, C, tB-1, 1-2*tB, tB, tvec) ;
	tvec[0] *= 2. ;
	tvec[1] *= 2. ;
	tvec[2] *= 2. ;
	dvec[0] = 2*(A[0]-2*q[0]+C[0]) ; // r"
	dvec[1] = 2*(A[1]-2*q[1]+C[1]) ;
	dvec[2] = 2*(A[2]-2*q[2]+C[2]) ;
	d = mathVecLen(tvec) ; // ||r'||
	if( d < 1.e-50 )
		return 0 ; //ERDEGE ;
	mathVProduct(tvec, dvec, v) ; // r'x r"
	*pcur = mathVecLen(v)/(d*d*d) ;

	return 1 ; //ERSUCSS ;
}

/*	2015/8/22 nt
 *  estimate curvature and curvature variation
 *	利用p及与p点距离约为span的两个点p1,p2估计p点处的曲率，
 *  p点处的曲率变差估计值=|p1点曲率-p2点曲率|
 *	本函数用于样条曲线在节点处的曲率估计
 */
int curveEstmCurVar(CURVE* curve, double t, double span, PNT3D p, double cv[2])
{
	double c1, c2, d ;
	PNT3D p1, p2 ;
	VEC3D T, N, B ;

	if( curveEvalDer1O(curve, t, p, T) != ERSUCSS )
		return ERUNSUC ;
	d = mathVecLen(T) ;
	if( d < MIN_LEN )
		return ERUNSUC ;
	if( curveEvalCur(curve, t-span/d, p1, T, N, B, &c1) != ERSUCSS || // 曲率1
		curveEvalCur(curve, t+span/d, p2, T, N, B, &c2) != ERSUCSS ) // 曲率2
		return ERUNSUC ;
	mathEstm3PCur2(p1, p, p2, &(cv[0])) ;
	cv[1] = fabs(c1-c2) ;

	return ERSUCSS ;
}

/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取曲线...") ;
	int k, n = GetSelected(ftRefCur, 1000) ;

	for( k = 0 ; k < n ; k++ )
	{
		FILE* fp ;
		fopen_s(&fp, "d:\\1.txt", "w+t") ;
		CRefCur* pRefCur = (CRefCur*)GetObj(k) ;
		CURVE* curve = pRefCur->GetCurve() ;
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ;
		int i ;
		double c, t, step ;
		PNT3D A, B, C, p ;
		step = (interval.t2-interval.t1)/100000 ;
		for( i = 1 ; i < 100000 ; i++ )
		{
			t = i*step ;
			curveEvalPos(curve, t-step, A) ;
			curveEvalPos(curve, t, B) ;
			curveEvalPos(curve, t+step, C) ;
			mathEstm3PCur2(A, B, C, &c) ;
			curveEvalCur(curve, t, p, A, B, C, &c) ;
			fprintf_s(fp, "%2.2lf\n", c) ;
		}
		fclose(fp) ;
	}

	return ;
}*/

void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取曲面...") ;
	int k = GetSelected(ftRefSur, 1) ;

	if( k > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;

		int i, j, nu = 101, nv = 101 ;
		double u, v, d = 10. ;
		PNT3D p, q ;
		VEC3D normal ;
		CPV* pPV = new CPV() ;
		pPV->CreateGrid(nu, nv) ;
		for( i = 0 ; i < nu ; i++ )
		{
			u = boxuv.min[0]+(boxuv.max[0]-boxuv.min[0])*i/(nu-1) ;
			for( j = 0 ; j < nv ; j++ )
			{
				v = boxuv.min[1]+(boxuv.max[1]-boxuv.min[1])*j/(nv-1) ;
				surfaceEvalNormal(surface, u, v, p, normal) ;
				mathEvalLin(p, normal, d, q) ;
				pPV->AddGridPt(i*nv+j, q) ;
			}
		}
		AddPV(pPV) ;
	}

	return ;
}
//==============================================================