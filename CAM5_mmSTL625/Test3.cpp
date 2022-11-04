#include "stdafx.h"
#include "API\AL_API.h"
#include "CAM.h"
#include "Models.h"
#include "Mills.h"
#include "SelSet.h"
#include "CAMDoc.h"
#include "Model\SND.h"
#include "Dialogs.h"

/*	nt add 2016/7/4
 *	planarID:1=planar curve
 */
int curveGetNormal(CURVE* curve, int* planarId, VEC3D normal)
{
	INTERVAL interval;
	PNT3D p1, p2, p3;

	curveGetInterval(curve, &interval);
	curveEvalPos(curve, interval.t1+0.25*(interval.t2-interval.t1), p1);
	curveEvalPos(curve, interval.t1+0.5*(interval.t2-interval.t1), p2);
	curveEvalPos(curve, interval.t1+0.75*(interval.t2-interval.t1), p3);
	mathGetTriNormal(p1, p2, p3, MIN_LEN, normal);
	*planarId = 1;

	return ERSUCSS;
}

/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取待测试的曲线...") ;
	if( GetSelected(ftRefCur, 1) != 1 ) // get one curves
		return ;
	CRefCur* pRefCur = (CRefCur*)GetObj(0) ; // get CRefCur
	CURVE* curve = pRefCur->GetCurve() ; // geometry curve need to be tested
	INTERVAL interval ;
	pRefCur->GetInterval(&interval) ; // get interval of the curve
	STRPT* old = pRefCur->GetDisc();

	int id, i;
	VEC3D normal;
	curveGetNormal(curve, &id, normal);

	BSCUR* r = bscurCreate(5, 5); // variable distance function defined in [0.,1.]
	for (i = 0; i < 5; i++)
	{
		r->u_p[i][0] = 0.25*i;
		r->u_p[i][1] = 0.05;
		r->u_p[i][2] = 0.;
		r->u_p[i][3] = 1.;
	}
	r->u_p[2][1] = 0.1;

	double t;
	PNT3D p, q;
	VEC3D tvec, v;
	STRPT* disc = strptCreate(old->np, STRPT_PT);
	for (i = 0; i < old->np; i++)
	{
		t = old->ts[i];
		curveEvalDer1O(curve, t, p, tvec);
		mathVProduct(tvec, normal, v);
		mathUniVec(v, MIN_DBL);
		bscurEvalPos(r, t, q);
		mathEvalLin(p, v, q[1], disc->ps[i]);
	}
	bscurFree(r);

	CPV* pPV = new CPV();
	pPV->AddPLine(disc);
	AddPV(pPV);

	return ;
}*/

/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取所有裁剪曲面及其边界...") ;

	int i ;
	CRefCur* pRefCur ;
	COA cs ;
	int nBnd = GetSelected(ftRefCur, 10000) ;
	for( i = 0 ; i < nBnd ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ; // get CRefCur
		cs.Add(pRefCur) ;
	}

	CRefSur* pRefSur ;
	COA ss ;
	int nTr = GetSelected(ftRefSur, 10000) ;
	for( i = 0 ; i < nTr ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ; // get CRefSur
		ss.Add(pRefSur) ;
	}

	SND* snd = sndCreate(nTr, nBnd, 0.1, 100., 0.0001) ;
	for( i = 0 ; i < nBnd ; i++ )
	{
		pRefCur = (CRefCur*)cs[i] ;
		CURVE* curve = pRefCur->GetCurve() ; // geometry curve
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ; // get interval
		BOX3D box ;
		pRefCur->GetBox(&box) ;
		sndSetBnd(snd, i, curve, &interval, &box) ;
	}
	for( i = 0 ; i < nTr ; i++ )
	{
		pRefSur = (CRefSur*)ss[i] ;
		SURFACE* surface = pRefSur->GetSurface() ; // geometry surface
		FACET* facet = pRefSur->GetFacet() ;
		BOX3D box ;
		pRefSur->GetBox(&box) ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		sndSetTr(snd, i, surface, facet, &boxuv, &box) ;
	}

	int n = GetSelected(ftRefPt, 10000) ;

	PNT3D p, q ;
	PNT2D uv ;
	GEOMETRY* geo ;
	for( i = 0 ; i < n ; i++ )
	{
		CRefPt* pRefPt = (CRefPt*)GetObj(i) ;
		pRefPt->GetP(p) ;
		sndCal(snd, p, &geo, q, uv) ;
		if( geo )
			AddLin(p, q) ;
	}

	return ;
}*/

// check periodic of surface
/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取周期曲面...") ;

	int i, n = GetSelected(ftRefSur, 1) ;
	if( n == 1 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ; // get CRefSur
		SURFACE* surface = pRefSur->GetSurface() ;
		PARAM param_u, param_v ;
		surfaceGetParam(surface, &param_u, &param_v) ;
		if( param_u.periType == 2 )
			camPromptStr("u periodic") ;
		if( param_v.periType == 2 )
			camPromptStr("v periodic") ;
	}

	return ;
}*/

// make spiral curve on a surface
/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取周期曲面...") ;

	int i, n = GetSelected(ftRefSur, 1), mRound = 20, nU, nV, nPoint = 1000 ;
	double (*ps)[3] ;
	ps = new double[nPoint][3] ;
	if( n == 1 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ; // get CRefSur
		SURFACE* surface = pRefSur->GetSurface() ;
		PARAM param_u, param_v ;
		surfaceGetParam(surface, &param_u, &param_v) ;
		nU = 1 ;
		nV = 1 ;
		if( param_u.periType == 2 )
			nU = mRound ;
		else
		if( param_v.periType == 2 )
			nV = mRound ;
		double u, v, step_u, step_v ;
		PNT3D p ;
		u = param_u.interval.t1 ;
		v = param_v.interval.t1 ;
		step_u = nU*(param_u.interval.t2-param_u.interval.t1)/(nPoint-1) ;
		step_v = nV*(param_v.interval.t2-param_v.interval.t1)/(nPoint-1) ;
		for( i = 0 ; i < nPoint ; i++ )
		{
			surfaceEvalPos(surface, u, v, p) ;
			//AddPt(p) ;
			memcpy(ps[i], p, sizeof(PNT3D)) ;
			u += step_u ;
			v += step_v ;
		}

		BSCUR* nurbs ;
		bscurIntpPs(nPoint, ps, 4, &nurbs) ;
		CRefCur* pRefCur = new CRefCur(0, nurbs) ;
		AddFeature(pRefCur) ;
	}
	delete []ps ;

	return ;
}*/

// 测试：将曲线贴合到多张裁剪曲面上
/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取要贴合的曲线、所有裁剪曲面及其边界...") ;

	// 提取曲线、曲面
	int i, k ;
	CRefCur* pRefCur ;
	COA cs ;
	int nBnd = GetSelected(ftRefCur, 10000) ;
	for( i = 0 ; i < nBnd ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ; // get CRefCur
		cs.Add(pRefCur) ;
	}

	CRefSur* pRefSur ;
	COA ss ;
	int nTr = GetSelected(ftRefSur, 10000) ;
	for( i = 0 ; i < nTr ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ; // get CRefSur
		ss.Add(pRefSur) ;
	}

	// 构造结构SND用于计算点到裁剪曲面的最近距离
	SND* snd = sndCreate(nTr, nBnd-1, 0.1, 100., 1.e-5) ;
	for( i = 1 ; i < nBnd ; i++ )
	{
		pRefCur = (CRefCur*)cs[i] ;
		CURVE* curve = pRefCur->GetCurve() ; // geometry curve
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ; // get interval
		BOX3D box ;
		pRefCur->GetBox(&box) ;
		sndSetBnd(snd, i-1, curve, &interval, &box) ;
	}
	for( i = 0 ; i < nTr ; i++ )
	{
		pRefSur = (CRefSur*)ss[i] ;
		SURFACE* surface = pRefSur->GetSurface() ; // geometry surface
		FACET* facet = pRefSur->GetFacet() ;
		BOX3D box ;
		pRefSur->GetBox(&box) ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		sndSetTr(snd, i, surface, facet, &boxuv, &box) ;
	}

	// 提取要贴合的曲线，并将其离散
	pRefCur = (CRefCur*)cs[0] ;
	CURVE* merged = pRefCur->GetCurve() ; // curve to be merged
	INTERVAL interval ;
	pRefCur->GetInterval(&interval) ; // get interval
	STRPT* disc = NULL ;
	if( curveDisc(merged, 
				  &interval, 
		          0.01, 
		          PI1/20,
		          1.,
		          STRPT_PT | STRPT_VEC,
		          &disc) != ERSUCSS ||
		          disc == NULL )
	{
		sndFree(snd) ;
		return ;
	}

	// 将曲线上的离散点投影到裁剪曲面上并插值成曲线
	PNT3D q ;
	PNT2D uv ;
	GEOMETRY* geo ;
	BSCUR* nurbs = NULL ;
	for( i = 1 ; i < disc->np-1 ; i++ )
	//for( i = 0 ; i < disc->np ; i++ )
	{
		sndCal(snd, disc->ps[i], &geo, q, uv) ;
		if( geo )
			memcpy(disc->ps[i], q, sizeof(PNT3D)) ;
		else
			disc->ps[i][0] = 1.e15 ;
	}
	k = 0 ;
	for( i = 0 ; i < disc->np ; i++ )
	{
		if( disc->ps[i][0] < 1.e10 ) // 有效的
		{
			if( i > k )
				memcpy(disc->ps[k], disc->ps[i], sizeof(PNT3D)) ;
			k++ ;
		}
	}
	if( k >= 4 )
	{
		disc->np = k ;
		bscurIntpPs(disc->np, disc->ps, 4, &nurbs) ;
	}
	strptFree(disc) ;
	if( nurbs )
	{
		pRefCur = new CRefCur(0, nurbs) ;
		AddFeature(pRefCur) ;
	}
	else
	{
		sndFree(snd) ;
		return ;
	}

	// 误差检测
	double d, e = 0., t ;
	PNT3D p ;
	pRefCur->GetInterval(&interval) ;
	for( i = 0 ; i < 1000 ; i++ )
	{
		t = interval.t1+mathRand01()*(interval.t2-interval.t1) ;
		bscurEvalPos(nurbs, t, p) ;
		sndCal(snd, p, &geo, q, uv) ;
		if( geo )
		{
			d = mathDist(p, q) ;
			if( e < d )
				e = d ;
		}
	}
	sndFree(snd) ;
	char str[64] ;
	sprintf_s(str, "贴合曲面的误差=%4.3lf", e) ;
	camPromptStr(str) ;

	return ;
}*/

// 测试：将一张完整曲面贴合到多张裁剪曲面上
/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取要贴合的曲面、所有裁剪曲面及其边界...") ;

	// 提取曲线、曲面
	int i, j ;
	CRefCur* pRefCur ;
	COA cs ;
	int nBnd = GetSelected(ftRefCur, 10000) ;
	for( i = 0 ; i < nBnd ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ; // get CRefCur
		cs.Add(pRefCur) ;
	}

	CRefSur* pRefSur ;
	COA ss ;
	int nTr = GetSelected(ftRefSur, 10000) ;
	for( i = 0 ; i < nTr ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ; // get CRefSur
		ss.Add(pRefSur) ;
	}

	// 构造结构SND用于计算点到裁剪曲面的最近距离
	SND* snd = sndCreate(nTr-1, nBnd, 0.1, 100., 1.e-5) ;
	for( i = 0 ; i < nBnd ; i++ )
	{
		pRefCur = (CRefCur*)cs[i] ;
		CURVE* curve = pRefCur->GetCurve() ; // geometry curve
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ; // get interval
		BOX3D box ;
		pRefCur->GetBox(&box) ;
		sndSetBnd(snd, i, curve, &interval, &box) ;
	}
	for( i = 1 ; i < nTr ; i++ )
	{
		pRefSur = (CRefSur*)ss[i] ;
		SURFACE* surface = pRefSur->GetSurface() ; // geometry surface
		FACET* facet = pRefSur->GetFacet() ;
		BOX3D box ;
		pRefSur->GetBox(&box) ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		sndSetTr(snd, i-1, surface, facet, &boxuv, &box) ;
	}

	// 提取要贴合的曲面，并将其离散
	pRefSur = (CRefSur*)ss[0] ;
	SURFACE* merged = pRefSur->GetSurface() ; // curve to be merged
	BOXUV boxuv ;
	pRefSur->GetBoxUV(&boxuv) ; // get boxuv
	int nu = 20, nv = 50 ; // 根据情况设置!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
	double u, v, (**array)[3] ;
	array = (double(**)[3])malloc(sizeof(double(*)[3])*nu) ;
	for( i = 0 ; i < nu ; i++ )
		array[i] = (double(*)[3])malloc(sizeof(double[3])*nv) ;
	for( i = 0 ; i < nu ; i++ )
	{
		u = boxuv.min[0]+i*(boxuv.max[0]-boxuv.min[0])/(nu-1) ;
		for( j = 0 ; j < nv ; j++ )
		{
			v = boxuv.min[1]+j*(boxuv.max[1]-boxuv.min[1])/(nv-1) ;
			surfaceEvalPos(merged, u, v, array[i][j]) ;
		}
	}

	// 将曲面上的离散点投影到裁剪曲面上并插值成曲面
	snd->tol = 1.e-3 ; // ?????????????????????????????????
	PNT3D p, q ;
	PNT2D uv ;
	GEOMETRY* geo ;
	BSSUR* nurbs ;
	for( i = 0 ; i < nu ; i++ )
	{
		for( j = 0 ; j < nv ; j++ )
		{
			sndCal(snd, array[i][j], &geo, q, uv) ;
			if( geo )
				memcpy(array[i][j], q, sizeof(PNT3D)) ;
		}
	}
	bssurCrePntArray(nu, nv, array, &nurbs) ;
	for( i = 0 ; i < nu ; i++ )
		free(array[i]) ;
	free(array) ;
	pRefSur = new CRefSur(0, nurbs) ;
	AddFeature(pRefSur) ;

	// 误差检测
	double d, e = 0. ;
	pRefSur->GetBoxUV(&boxuv) ;
	for( i = 0 ; i < 2000 ; i++ )
	{
		u = boxuv.min[0]+mathRand01()*(boxuv.max[0]-boxuv.min[0]) ;
		v = boxuv.min[1]+mathRand01()*(boxuv.max[1]-boxuv.min[1]) ;
		bssurEvalPos(nurbs, u, v, p) ;
		sndCal(snd, p, &geo, q, uv) ;
		if( geo )
		{
			d = mathDist(p, q) ;
			if( e < d )
				e = d ;
		}
	}
	sndFree(snd) ;
	char str[64] ;
	sprintf_s(str, "贴合曲面的误差=%4.3lf", e) ;
	camPromptStr(str) ;

	return ;
}*/

// 测试：组合曲线
/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取要组合的曲线...") ;

	// 提取曲线
	int i, n = GetSelected(ftRefCur, 10000) ;
	if( n > 1 )
	{
		double tol = 0.01, ang = PI1*1.e-5 ;
		CURVE** curves = new CURVE*[n], *merged = NULL ;
		INTERVAL* intervals = new INTERVAL[n], intv ;
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ; // get CRefCur
			curves[i] = pRefCur->GetCurve() ;
			pRefCur->GetInterval(&(intervals[i])) ;
		}
		if( profileMerge(n, 
			             curves, 
			             intervals, 
			             tol,
			             ang, 
			             &merged, 
			             &intv) == ERSUCSS )
		{
			CRefCur* pRefCur = new CRefCur(0, merged, &intv) ;
			AddFeature(pRefCur) ;
			camPromptStr("组合成功") ;
		}
		else
			camPromptStr("组合失败") ;
	}

	return ;
}*/

// 测试：曲线延伸到给定的点
/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取要延伸的曲线及终止点...") ;

	// 提取终止点
	int m = GetSelected(ftRefPt, 1) ;
	if( m < 1 )
	{
		camPromptStr("未拾取终止点!") ;
		return ;
	}
	CRefPt* pRefPt = (CRefPt*)GetObj(0) ;
	PNT3D endPt ;
	pRefPt->GetP(endPt) ;

	// 提取曲线
	int n = GetSelected(ftRefCur, 10000) ;
	if( n < 1 )
	{
		camPromptStr("未拾取要延伸的曲线!") ;
		return ;
	}
	
	int i ;
	for( i = 0 ; i < n ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
		CURVE* curve = pRefCur->GetCurve(), *pExtend = NULL ;
		INTERVAL interval, intv ;
		pRefCur->GetInterval(&interval) ;
		if( curveExtendToPt(curve, 
							&interval, 
							endPt, 
							&pExtend, 
							&intv) == ERSUCSS &&
			pExtend )
		{
			pRefCur = new CRefCur(0, pExtend, &intv) ;
			AddFeature(pRefCur) ;
		}
		else
			camPromptStr("延伸失败一次!") ;
	}

	Redraw() ;

	return ;
}*/

/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取待测试的曲线...") ;
	if( GetSelected(ftRefCur, 1) != 1 ) // get one curves
		return ;
	CRefCur* pRefCur = (CRefCur*)GetObj(0) ; // get CRefCur
	CURVE* curve = pRefCur->GetCurve() ; // geometry curve need to be tested
	INTERVAL interval ;
	pRefCur->GetInterval(&interval) ; // get interval of the curve
	
	double u ;
	PNT3D p, q ;
	FILE* fp ;
	fopen_s(&fp, "d:\\test2.txt", "r") ;
	int i, n, rt ;
	fscanf_s(fp, "%d", &n) ;
	for (i = 0; i < n; i++)
	{
		fscanf_s(fp, "%lf %lf %lf", p, p+1, p+2) ;
		rt = curveSnap(curve, p, q, &u) ;
		AddLin(p, q) ;
	}

	return ;
}*/

/*void CCAMDoc::OnTest()
{
	camPromptStr("拾取待测试的曲线...") ;
	if( GetSelected(ftRefCur, 1) != 1 ) // get one curves
		return ;
	CRefCur* pRefCur = (CRefCur*)GetObj(0) ; // get CRefCur
	CURVE* curve = pRefCur->GetCurve() ; // geometry curve need to be tested
	INTERVAL interval ;
	pRefCur->GetInterval(&interval) ; // get interval of the curve
	BSCUR* nurbs = curveGetNurbs(curve) ;

	double u, d ;
	PNT3D p, q ;
	FILE* fp ;
	fopen_s(&fp, "d:\\test2.txt", "r") ;
	int i, n, rt ;
	fscanf_s(fp, "%d", &n) ;
	for (i = 0; i < n; i++)
	{
		fscanf_s(fp, "%lf %lf %lf", p, p+1, p+2) ;
		u = 1e10 ;
		rt = bscurSnapPnt(nurbs, p, 1.e-5, q, &u, &d) ;
		AddLin(p, q) ;
	}

	return ;
}*/

////////////////////////////////2017-9
/*void CCAMDoc::OnTest() 
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
}*/

/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取曲面...") ;
	int k, n = GetSelected(ftRefSur, 1) ;

	for( k = 0 ; k < n ; k++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(k) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		BSSUR* nurbs = surfaceGetNurbs(surface) ;
		if( nurbs )
		{
			int ord, cnt ;
			double d, max = 0. ;
			ord = nurbs->ord_u ;
			cnt = nurbs->cnt_u ;
			BSCUR* bc = bscurCreate(ord, cnt) ;
			memcpy(bc->u_k, nurbs->u_k, (ord+cnt)*sizeof(double)) ;
			int i, j ;
			for( j = 0 ; j < nurbs->cnt_v ; j++ )
			{
				for( i = 0 ; i < nurbs->cnt_u ; i++ ) // 取出曲面的控制顶点坐标
					memcpy(bc->u_p[i], nurbs->uv_p[i][j], sizeof(double[4])) ;
				bscurMakeCPConvex(bc, 10000) ; // 对曲线做保凸算法
				for( i = 0 ; i < nurbs->cnt_u ; i++ ) // 更新曲面的控制顶点坐标
				{
					d = mathDist(nurbs->uv_p[i][j], bc->u_p[i]) ;
					if( max < d ) // 更新最大的变差
						max = d ;
					memcpy(nurbs->uv_p[i][j], bc->u_p[i], sizeof(double[4])) ;
				}
			}
			bscurFree(bc) ;
			char s[64] ;
			sprintf_s(s, "控制顶点最大偏差=%4.3lf", max) ;
			camPromptStr(s) ;
		}
	}

	return ;
}*/

/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	FILE* fp = NULL ;
	fopen_s(&fp, "d:\\2.txt", "r") ;
	if( fp )
	{
		int n, i ;
		fscanf_s(fp, "%d", &n) ;
		double (*ps)[3], *p_u ;
		ps = new double[n][3] ;
		p_u = new double[n] ;
		for( i = 0 ; i < n ; i++ )
		{
			fscanf_s(fp, "%lf %lf %lf %lf", ps[i], ps[i]+1, ps[i]+2, p_u+i) ;
			AddPt(ps[i]) ;
		}

		for( i = 1 ; i < n ; i++ )
			p_u[i] -= p_u[0] ;
		p_u[0] = 0. ;

		BSCUR* nurbs ;
		bscurIntpPsFixP(n, ps, p_u, 4, &nurbs) ;
		CRefCur* pRefCur = new CRefCur(0, nurbs) ;
		AddFeature(pRefCur) ;
		delete []ps ;
		delete []p_u ;
		fclose(fp) ;
	}
}*/

// nt add 2018/7/14
/*void CCAMDoc::OnTest()
{
	CString szFilter ;
	szFilter += _T("OBJ Files(*.pcd)|*.pcd|") ;
	CFileDialog dlg(TRUE, _T("*.pcd"), NULL, OFN_HIDEREADONLY, szFilter) ;
	if( dlg.DoModal() == IDOK )
	{
		CString ext = dlg.GetFileExt() ;
		CString pathName = dlg.GetPathName() ;
		if( ext == _T("pcd") ||
			ext == _T("PCD") ||
			ext == _T("Pcd") )
		{
			if( m_pPart )
				m_pPart->LoadPCD(pathName) ;
			Redraw() ;
		}
	}
}*/

/*void CCAMDoc::OnTest()
{
	FILE* fp ;
	double p[3] ;
	int i, n ;
	fopen_s(&fp, "d:\\1.txt", "r+t") ;
	fscanf(fp, "%d", &n) ;
	for( i = 0 ; i < n ; i++ )
	{
		fscanf(fp, "%lf %lf %lf", &p[0], &p[1], &p[2]) ;
		AddPt(p) ;
	}
	fclose(fp) ;
}*/
//==============================================================
int bssurExactUB1(BSSUR* bs, BOXUV* boxuv, double ubs[2])
{
	int i, j ;
	double u, v, d ;
	PNT3D p ;
	VEC3D du, dv ;

	ubs[0] = 0. ;
	ubs[1] = 0. ;
	for( i = 0 ; i <= 100 ; i++ )
	{
		u = boxuv->min[0]+0.01*i*(boxuv->max[0]-boxuv->min[0]) ;
		for( j = 0 ; j <= 100 ; j++ )
		{
			v = boxuv->min[1]+0.01*j*(boxuv->max[1]-boxuv->min[1]) ;
			bssurEvalDer1O(bs, u, v, p, du, dv) ;
			d = mathVecLen(du) ;
			if( ubs[0] < d )
				ubs[0] = d ;
			d = mathVecLen(dv) ;
			if( ubs[1] < d )
				ubs[1] = d ;
		}
	}

	return 1 ;
}

int bssurExactUB2(BSSUR* bs, BOXUV* boxuv, double ubs[3])
{
	int i, j ;
	double u, v, d ;
	PNT3D p ;
	VEC3D du, dv, duu, duv, dvv ;

	ubs[0] = 0. ;
	ubs[1] = 0. ;
	ubs[2] = 0. ;
	for( i = 0 ; i <= 100 ; i++ )
	{
		u = boxuv->min[0]+0.01*i*(boxuv->max[0]-boxuv->min[0]) ;
		for( j = 0 ; j <= 100 ; j++ )
		{
			v = boxuv->min[1]+0.01*j*(boxuv->max[1]-boxuv->min[1]) ;
			bssurEvalDer2O(bs, u, v, p, du, dv, duu, duv, dvv) ;
			d = mathVecLen(duu) ;
			if( ubs[0] < d )
				ubs[0] = d ;
			d = mathVecLen(duv) ;
			if( ubs[1] < d )
				ubs[1] = d ;
			d = mathVecLen(dvv) ;
			if( ubs[2] < d )
				ubs[2] = d ;
		}
	}

	return 1 ;
}
//==============================================================
// 仅适用于Bezier曲面, 节点矢量[0,0...1,1]
int ubsCal12005IS1(BSSUR* bs, double ubs[2])
{
	int i, j, k, I, J, p, q, n, m ;
	double a, d, max, min, dd ;
	VEC3D v ;

	ubs[0] = 0. ;
	ubs[1] = 0. ;
	p = bs->ord_u ;
	q = bs->ord_v ;
	n = bs->cnt_u-1 ;
	m = bs->cnt_v-1 ;

	// Su(u,v)
	min = MAX_DBL ;
	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			if( min > bs->uv_p[i][j][3] )
				min = bs->uv_p[i][j][3] ;
			if( max < bs->uv_p[i][j][3] )
				max = bs->uv_p[i][j][3] ;
		}
	}
	a = max/min ;

	max = 0. ;
	for( i = 0 ; i < n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			for( J = 0 ; J <= m ; J++ )
			{
				for( k = 0 ; k < 3 ; k++ )
					v[k] = bs->uv_p[i+1][j][k]/bs->uv_p[i+1][j][3]-bs->uv_p[i][J][k]/bs->uv_p[i][J][3] ;
				dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
				if( max < dd )
					max = dd ;
			}
		}
	}
	d = max ;

	ubs[0] = (p-1)*a*a*d ;

	// Sv(u,v)
	max = 0. ;
	for( j = 0 ; j < m ; j++ )
	{
		for( i = 0 ; i <= n ; i++ )
		{
			for( I = 0 ; I <= n ; I++ )
			{
				for( k = 0 ; k < 3 ; k++ )
					v[k] = bs->uv_p[i][j+1][k]/bs->uv_p[i][j+1][3]-bs->uv_p[I][j][k]/bs->uv_p[I][j][3] ;
				dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
				if( max < dd )
					max = dd ;
			}
		}
	}
	d = max ;

	ubs[1] = (q-1)*a*a*d ;

	return ERSUCSS ;
}

// 仅适用于Bezier曲面, 节点矢量[0,0...1,1]
int ubsCal12005IS2(BSSUR* bs, double ubs[2])
{
	int i, j, k, I, J, p, q, n, m ;
	double a, b, c, d, max, dd ;
	VEC3D v ;

	ubs[0] = 0. ;
	ubs[1] = 0. ;
	p = bs->ord_u ;
	q = bs->ord_v ;
	n = bs->cnt_u-1 ;
	m = bs->cnt_v-1 ;

	// Su(u,v)
	max = 0. ;
	for( i = 0 ; i < n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i+1][j][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	a = max ;

	max = 0. ;
	for( i = 1 ; i <= n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i-1][j][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	b = max ;

	c = 1. ;
	for( k = 1 ; k < p ; k++ )
		c *= mathMAX(a, b) ;

	max = 0. ;
	for( i = 0 ; i < n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			for( J = 0 ; J <= m ; J++ )
			{
				for( k = 0 ; k < 3 ; k++ )
					v[k] = bs->uv_p[i+1][j][k]/bs->uv_p[i+1][j][3]-bs->uv_p[i][J][k]/bs->uv_p[i][J][3] ;
				dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
				if( max < dd )
					max = dd ;
			}
		}
	}
	d = max ;

	ubs[0] = (p-1)*c*d ;

	// Sv(u,v)
	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 0 ; j < m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i][j+1][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	a = max ;

	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 1 ; j <= m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i][j-1][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	b = max ;

	c = 1. ;
	for( k = 1 ; k < q ; k++ )
		c *= mathMAX(a, b) ;

	max = 0. ;
	for( j = 0 ; j < m ; j++ )
	{
		for( i = 0 ; i <= n ; i++ )
		{
			for( I = 0 ; I <= n ; I++ )
			{
				for( k = 0 ; k < 3 ; k++ )
					v[k] = bs->uv_p[i][j+1][k]/bs->uv_p[i][j+1][3]-bs->uv_p[I][j][k]/bs->uv_p[I][j][3] ;
				dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
				if( max < dd )
					max = dd ;
			}
		}
	}
	d = max ;

	ubs[1] = (q-1)*c*d ;

	return ERSUCSS ;
}

// 仅适用于Bezier曲面, 节点矢量[0,0...1,1]
int ubsCal12005IS3(BSSUR* bs, double ubs[2])
{
	int i, j, k, I, J, p, q, n, m ;
	double a, b, c, d, max, dd ;
	VEC3D v ;

	ubs[0] = 0. ;
	ubs[1] = 0. ;
	p = bs->ord_u ;
	q = bs->ord_v ;
	n = bs->cnt_u-1 ;
	m = bs->cnt_v-1 ;

	// Su(u,v)
	max = 0. ;
	for( i = 0 ; i < n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i+1][j][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	a = max ;

	max = 0. ;
	for( i = 1 ; i <= n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i-1][j][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	b = max ;

	c = mathMAX(a, b) ;

	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			for( I = 0 ; I <= n ; I++ )
			{
				for( J = 0 ; J <= m ; J++ )
				{
					for( k = 0 ; k < 3 ; k++ )
						v[k] = bs->uv_p[i][j][k]/bs->uv_p[i][j][3]-bs->uv_p[I][J][k]/bs->uv_p[I][J][3] ;
					dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
					if( max < dd )
						max = dd ;
				}
			}
		}
	}
	d = max ;

	ubs[0] = (p-1)*c*d ;

	// Sv(u,v)
	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 0 ; j < m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i][j+1][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	a = max ;

	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 1 ; j <= m ; j++ )
		{
			dd = bs->uv_p[i][j][3]/bs->uv_p[i][j-1][3] ;
			if( max < dd )
				max = dd ;
		}
	}
	b = max ;

	c = mathMAX(a, b) ;

	ubs[1] = (q-1)*c*d ;

	return ERSUCSS ;
}

int ubsCal12017WGJ(BSSUR* bs, double ubs[2])
{
	int i, j, k, I, J, p, q, n, m ;
	double a, b, c, d, e, max, min, dd ;
	VEC3D v ;

	ubs[0] = 0. ;
	ubs[1] = 0. ;
	p = bs->ord_u ;
	q = bs->ord_v ;
	n = bs->cnt_u-1 ;
	m = bs->cnt_v-1 ;

	// Su(u,v)
	min = MAX_DBL ;
	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			if( min > bs->uv_p[i][j][3] )
				min = bs->uv_p[i][j][3] ;
			if( max < bs->uv_p[i][j][3] )
				max = bs->uv_p[i][j][3] ;
		}
	}
	a = max/min ;

	min = MAX_DBL ;
	max = 0. ;
	for( i = 1 ; i <= n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			if( min > bs->uv_p[i][j][3] )
				min = bs->uv_p[i][j][3] ;
			if( max < bs->uv_p[i][j][3] )
				max = bs->uv_p[i][j][3] ;
		}
	}
	b = max/min ;

	min = MAX_DBL ;
	max = 0. ;
	for( i = 0 ; i < n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			if( min > bs->uv_p[i][j][3] )
				min = bs->uv_p[i][j][3] ;
			if( max < bs->uv_p[i][j][3] )
				max = bs->uv_p[i][j][3] ;
		}
	}
	c = max/min ;

	max = 0. ;
	for( i = 0 ; i < n ; i++ )
	{
		for( j = 0 ; j <= m ; j++ )
		{
			for( J = 0 ; J <= m ; J++ )
			{
				for( k = 0 ; k < 3 ; k++ )
					v[k] = bs->uv_p[i+1][j][k]/bs->uv_p[i+1][j][3]-bs->uv_p[i][J][k]/bs->uv_p[i][J][3] ;
				dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
				if( max < dd )
					max = dd ;
			}
		}
	}
	d = max ;

	min = MAX_DBL ;
	for( i = 0 ; i < n ; i++ )
	{
		dd = bs->u_k[i+p]-bs->u_k[i+1] ;
		if( min > dd )
			min = dd ;
	}
	e = min ;

	ubs[0] = (p-1)*a*a*b*c*d/e ;

	// Sv(u,v)
	min = MAX_DBL ;
	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 1 ; j <= m ; j++ )
		{
			if( min > bs->uv_p[i][j][3] )
				min = bs->uv_p[i][j][3] ;
			if( max < bs->uv_p[i][j][3] )
				max = bs->uv_p[i][j][3] ;
		}
	}
	b = max/min ;

	min = MAX_DBL ;
	max = 0. ;
	for( i = 0 ; i <= n ; i++ )
	{
		for( j = 0 ; j < m ; j++ )
		{
			if( min > bs->uv_p[i][j][3] )
				min = bs->uv_p[i][j][3] ;
			if( max < bs->uv_p[i][j][3] )
				max = bs->uv_p[i][j][3] ;
		}
	}
	c = max/min ;

	max = 0. ;
	for( j = 0 ; j < m ; j++ )
	{
		for( i = 0 ; i <= n ; i++ )
		{
			for( I = 0 ; I <= n ; I++ )
			{
				for( k = 0 ; k < 3 ; k++ )
					v[k] = bs->uv_p[i][j+1][k]/bs->uv_p[i][j+1][3]-bs->uv_p[I][j][k]/bs->uv_p[I][j][3] ;
				dd = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]) ;
				if( max < dd )
					max = dd ;
			}
		}
	}
	d = max ;

	min = MAX_DBL ;
	for( j = 0 ; j < m ; j++ )
	{
		dd = bs->v_k[j+q]-bs->v_k[j+1] ;
		if( min > dd )
			min = dd ;
	}
	e = min ;

	ubs[1] = (q-1)*a*a*b*c*d/e ;

	return ERSUCSS ;
}
//--------------------------------------------------------------
double bscurCalUB1Exact(BSCUR* bc) // debug only
{
	int i ;
	double d, t1, t2, ub = 0. ;
	PNT3D p ;
	VEC3D du ;

	t1 = bc->u_k[bc->ord_u-1] ;
	t2 = bc->u_k[bc->cnt_u] ;
	for( i = 0 ; i <= 1000000 ; i++ )
	{
		bscurEvalDer1O(bc, t1+0.000001*i*(t2-t1), p, du) ;
		d = mathVecLen(du) ;
		if( ub < d )
			ub = d ;
	}

	return ub ;
}

double bscurCalUB2Exact(BSCUR* bc) // debug only
{
	int i ;
	double d, t1, t2, ub = 0. ;
	PNT3D p ;
	VEC3D du, duu ;

	t1 = bc->u_k[bc->ord_u-1] ;
	t2 = bc->u_k[bc->cnt_u] ;
	for( i = 0 ; i <= 1000000 ; i++ )
	{
		bscurEvalDer2O(bc, t1+0.000001*i*(t2-t1), p, du, duu) ;
		d = mathVecLen(duu) ;
		if( ub < d )
			ub = d ;
	}

	return ub ;
}
//==============================================================
// 测试NURBS曲线二阶导数上界
/*void CCAMDoc::OnTest()
{
	// TODO: Add your command handler code here
	camPromptStr("选择曲线...") ;

	// test for the upper bounds of NURBS curve derivatives
	int n = GetSelected(ftRefCur, 1) ;
	if( n < 1 )
		AfxMessageBox(_T("拾取一条NURBS曲线")) ;
	else
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
		CURVE* curve = pRefCur->GetCurve() ;
		BSCUR* bc = curveGetNurbs(curve) ; // 取出NURBS
		double ub, ub1, UB, UB1 ;

		// 一阶导数计算
		// exact
		ub = bscurCalUB1Exact(bc) ;
		// estimate Ning, Zheng
		ubcCal1(bc, &ub1) ;
		
		// 二阶导数计算
		// exact
		UB = bscurCalUB2Exact(bc) ; // 近似精确二阶导数
		// estimate Ning, Zheng
		ubcCal2(bc, &UB1) ; // 估计二阶导数

		UB1 = UB1 ;
	}

	return ;
}*/

// 测试NURBS曲面二阶导数上界
void CCAMDoc::OnTest()
{
	// read a NURBS surface
	FILE* fp ;
	int i, j, ord_u, ord_v, cnt_u, cnt_v ;
	double x, y, z, w ;
	BSSUR* nurbs = NULL ;

	//---------------------------------------------------------
	// 格式1
	//fopen_s(&fp, "d:\\(8)BEZIER_5x5 20019-6-23.txt", "r+t") ;
	fopen_s(&fp, "d:\\(9)NURBS_4x3 2019-6-23.txt", "r+t") ;
	fscanf_s(fp, "%d %d", &ord_u, &cnt_u) ;
	fscanf_s(fp, "%d %d", &ord_v, &cnt_v) ;
	nurbs = bssurCreate(ord_u, cnt_u, ord_v, cnt_v) ;
	for( i = 0 ; i < ord_u+cnt_u ; i++ )
		fscanf_s(fp, "%lf", &(nurbs->u_k[i])) ;
	for( j = 0 ; j < ord_v+cnt_v ; j++ )
		fscanf_s(fp, "%lf", &(nurbs->v_k[j])) ;
	for( i = 0 ; i < cnt_u ; i++ )
	{
		for( j = 0 ; j < cnt_v ; j++ )
		{
			fscanf_s(fp, "%lf %lf %lf %lf", &x, &y, &z, &w) ;
			nurbs->uv_p[i][j][0] = w*x ;
			nurbs->uv_p[i][j][1] = w*y ;
			nurbs->uv_p[i][j][2] = w*z ;
			nurbs->uv_p[i][j][3] = w ;
		}
	}
	//---------------------------------------------------------
	// WGJ格式
	/*fopen_s(&fp, "d:\\R56.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R44.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R333.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R334.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R332.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R48.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R331.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R44改3344.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R331改3333.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R331改2233.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R56改节点矢量.txt", "r+t") ;
	//fopen_s(&fp, "d:\\R56改节点矢量改权因子.txt", "r+t") ;
	fscanf_s(fp, "%d %d", &ord_u, &ord_v) ;
	fscanf_s(fp, "%d %d", &cnt_u, &cnt_v) ;
	nurbs = bssurCreate(ord_u, cnt_u, ord_v, cnt_v) ;

	for( i = 0 ; i < cnt_u ; i++ )
	{
	for( j = 0 ; j < cnt_v ; j++ )
	{
	fscanf_s(fp, "%lf %lf %lf", &x, &y, &z) ;
	nurbs->uv_p[i][j][0] = x ;
	nurbs->uv_p[i][j][1] = y ;
	nurbs->uv_p[i][j][2] = z ;
	}
	}

	for( i = 0 ; i < cnt_u ; i++ )
	{
	for( j = 0 ; j < cnt_v ; j++ )
	{
	fscanf_s(fp, "%lf", &w) ;
	nurbs->uv_p[i][j][0] *= w ;
	nurbs->uv_p[i][j][1] *= w ;
	nurbs->uv_p[i][j][2] *= w ;
	nurbs->uv_p[i][j][3] = w ;
	}
	}

	for( i = 0 ; i < ord_u+cnt_u ; i++ )
	fscanf_s(fp, "%lf", &(nurbs->u_k[i])) ;

	for( j = 0 ; j < ord_v+cnt_v ; j++ )
	fscanf_s(fp, "%lf", &(nurbs->v_k[j])) ;*/
	//----------------------------------------------------

	fclose(fp) ;

	double ubs[2], ubs1[2], ubs2[2], ubs3[2], ubs4[2], ubs5[2] ;
	BOXUV boxuv ;
	boxuv.min[0] = nurbs->u_k[ord_u-1] ;
	boxuv.max[0] = nurbs->u_k[cnt_u] ;
	boxuv.min[1] = nurbs->v_k[ord_v-1] ;
	boxuv.max[1] = nurbs->v_k[cnt_v] ;
	bssurExactUB1(nurbs, &boxuv, ubs) ;
	ubsCal1(nurbs, ubs1) ;
	ubsCal12017WGJ(nurbs, ubs2) ;
	ubsCal12005IS1(nurbs, ubs3) ;
	ubsCal12005IS2(nurbs, ubs4) ;
	ubsCal12005IS3(nurbs, ubs5) ;

	double UBS[3], UBS1[3] ;
	bssurExactUB2(nurbs, &boxuv, UBS) ;
	ubsCal2(nurbs, UBS1) ;

	CRefSur* pRefSur = new CRefSur(0, nurbs) ;
	bssurFree(nurbs) ;
	nurbs = NULL ;
	AddFeature(pRefSur) ;

	return ;
}
