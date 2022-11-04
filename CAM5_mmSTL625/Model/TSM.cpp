/*	2010/5/18-2021/8/3 nt
 *	implementation file of structure : trimmed surface model
 *	TEDGE, TFIN, TLOOP, TFACE, TSM
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "DNAVI.h"
#include "Feature.h"
#include "TSM.h"
#include "Mark.h"
#include "RefCur.h"
#include "RefSur.h"

////////////////////////////////////////////////////////////////
// nt add 2021/6/16 按等弧长剖分

//==============================================================
TEDGE* tedgeCreate()
{
	TEDGE* tedge = new TEDGE() ;
	tedge->next = NULL ;
	tedge->tfins = NULL ;
	mathClnBox(&tedge->box) ;

	return tedge ;
}

void tedgeFree(TEDGE* tedge)
{
	if( tedge )
		delete tedge ;

	return ;
}

void tedgeAdd(TEDGE* tedge, TFIN* tfin, BOX3D* box)
{
	if( tedge &&
		tfin )
	{
		tfin->link = tedge->tfins ;
		tfin->tedge = tedge ; // nt add 2021/8/5
		tedge->tfins = tfin ;
		if( box )
			tedge->box = *box ;
	}

	return ;
}

CURVE* tedgeGetCurve(TEDGE* tedge, INTERVAL* interval, int* flag)
{
	TFIN* tfin = NULL ;

	if( tedge &&
		(tfin = tedge->tfins) )
	{
		if( interval )
			*interval = tfin->interval ;
		if( flag )
			*flag = tfin->flag ;
		return tfin->curve ;
	}
	else
		return NULL ;
}

double tedgeGetDD(TEDGE* tedge)
{
	double dd = MAX_DBL ;
	TFIN* tfin = NULL ;
	TFACE* tface = NULL ;

	if( tedge == NULL )
		return MAX_DBL ;

	for( tfin = tedge->tfins ; tfin ; tfin = tfin->link )
	{
		tface = tfinGetTFace(tfin) ;
		if( tface &&
			dd > tface->dd )
			dd = tface->dd ;
	}

	return dd ;
}

int tedgeGetNumOfTFin(TEDGE* tedge)
{
	int n = 0 ;
	TFIN* tfin = NULL ;

	for( tfin = tedge->tfins ; tfin ; tfin = tfin->link )
		n++ ;

	return n ;
}
//--------------------------------------------------------------
TFACE* tfinGetTFace(TFIN* tfin)
{
	if( tfin &&
		tfin->tloop )
		return tfin->tloop->tface ;
	else
		return NULL ;
}

/*	2010/5/18 nt
 *	trim loop
 */
int tloopAllocFins(TLOOP* tloop, int nFin)
{
	if( tloop )
	{
		if( nFin > 0 )
		{
			tloop->tfins = new TFIN[nFin] ;
			for( int i = 0 ; i < nFin ; i++ )
			{
				tloop->tfins[i].link = NULL ; // nt add 2021/8/2
				tloop->tfins[i].tloop = tloop ; // nt add 2021/8/2
				tloop->tfins[i].tedge = NULL ; // nt add 2021/8/2
				tloop->tfins[i].curve = NULL ;
				tloop->tfins[i].interval.t1 = -MAX_DBL ;
				tloop->tfins[i].interval.t1 = MAX_DBL ;
				tloop->tfins[i].flag = 1 ;
				tloop->tfins[i].disc = NULL ; // nt add 2021/8/3
			}
			tloop->n = nFin ;
		}
		else
		{
			tloop->tfins = NULL ;
			tloop->n = 0 ;
		}
		return 1 ;
	}
	else
		return 0 ;
}

int tloopFreeFins(TLOOP* tloop)
{
	if( tloop )
	{
		for( int i = 0 ; i < tloop->n ; i++ )
		{
			if( tloop->tfins[i].curve )
				entityFree(tloop->tfins[i].curve) ;
			if( tloop->tfins[i].disc ) // nt add 2021/8/3
				strptFree(tloop->tfins[i].disc) ;
		}
		delete [](tloop->tfins) ;
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
/*	2010/5/18 nt
 *	trim surface
 */
TFACE* tfaceCreate(SURFACE* surface, BOXUV* boxuv, int nLoop, int sp)
{
	TFACE* tface = new TFACE() ;
	tface->surface = surface ;
	tface->facet = NULL ; // nt add 2021/8/2
	tface->su = MAX_DBL ; // nt add 2021/8/3
	tface->sv = MAX_DBL ; // nt add 2021/8/3
	tface->dd = MAX_DBL ; // nt add 2021/8/3
	if( boxuv )
		tface->boxuv = *boxuv ;
	else
	{
		tface->boxuv.min[0] = -MAX_DBL ;
		tface->boxuv.min[1] = -MAX_DBL ;
		tface->boxuv.max[0] = MAX_DBL ;
		tface->boxuv.max[1] = MAX_DBL ;
	}
	tface->n = nLoop ;
	if( nLoop > 0 )
	{
		tface->tloops = new TLOOP[nLoop] ;
		for( int i = 0 ; i < nLoop ; i++ )
		{
			tface->tloops[i].tface = tface ; // nt add 2021/8/2
			tface->tloops[i].flag = i ;
			tface->tloops[i].n = 0 ;
			tface->tloops[i].tfins = NULL ;
		}
	}
	else
		tface->tloops = NULL ;
	tface->sp = sp ; // nt add 2010/5/21
	// nt add 2021/8/2
	surfaceCalBox(surface, &tface->boxuv, &tface->box) ; // nt add 2021/8/2
	return tface ;
}

int tfaceFree(TFACE* tface)
{
	if( tface )
	{
		if( tface->surface )
			entityFree(tface->surface) ;
		for( int i = 0 ; i < tface->n ; i++ )
		{
			TLOOP* tloop = &(tface->tloops[i]) ;
			tloopFreeFins(tloop) ;
		}
		if( tface->facet ) // nt add 2021/8/2
			facetFree(tface->facet) ;
		delete [](tface->tloops) ;
		delete tface ;
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
int tfaceGetNumOfFin(TFACE* tface)
{
	if( tface )
	{
		int i, n = 0 ;
		for( i = 0 ; i < tface->n ; i++ )
		{
			n += tface->tloops[i].n ;
		}
		return n ;
	}
	else
		return 0 ;
}

// nt add 2021/3/6
void tfaceCopyFinAll(TFACE* tface, COA& cs)
{
	int i, j, k = 0, flag ;
	CURVE* curve ;
	INTERVAL interval ;
	CRefCur* pRefCur ;

	for( i = 0 ; i < tface->n ; i++ )
	{
		for( j = 0 ; j < tface->tloops[i].n ; j++ )
		{
			curve = curveCopy(tface->tloops[i].tfins[j].curve) ;
			interval = tface->tloops[i].tfins[j].interval ;
			flag = (i+1)*tface->tloops[i].tfins[j].flag ;
			k++ ;
			pRefCur = new CRefCur(0, curve, &interval) ;
			cs.Add(pRefCur) ;
		}
	}

	return ;
}
//==============================================================
TSM* tsmCreate()
{
	TSM* tsm = (TSM*)malloc(sizeof(TSM)) ;
	if( tsm == NULL )
		return NULL ;
	tsm->tedges = NULL ;
	tsm->tfaces = NULL ;

	return tsm ;
}

void tsmFree(TSM* tsm)
{
	TEDGE* tedge ;
	TFACE* tface ;

	if( tsm )
	{
		while(tsm->tedges)
		{
			tedge = tsm->tedges->next ;
			tedgeFree(tsm->tedges) ;
			tsm->tedges = tedge ;
		}

		while(tsm->tfaces)
		{
			tface = tsm->tfaces->next ;
			tfaceFree(tsm->tfaces) ;
			tsm->tfaces = tface ;
		}

		free(tsm) ;
	}

	return ;
}

// 增加TFACE的过程中匹配边，生成TEDGE, tedge已完成赋值
void tsmAddEdge(TSM* tsm, TEDGE* tedge)
{
	if( tsm &&
		tedge )
	{
		tedge->next = tsm->tedges ;
		tsm->tedges = tedge ;
	}

	return ;
}

// 增加TFACE的过程中匹配边，生成TEDGE
// tol对应IGES模型的容差
// tface中的surface和tloops已经赋值
void tsmAddFace(TSM* tsm, TFACE* tface, double tol)
{
	int i, j, flag, yesno ;
	INTERVAL interval1, interval2 ;
	BOX3D box ;
	CURVE* curve1, *curve2 ;
	TEDGE* tedge ;
	TFIN* tfin ;

	if( tsm &&
		tface )
	{
		tface->next = tsm->tfaces ;
		tsm->tfaces = tface ;

		for( i = 0 ; i < tface->n ; i++ )
		{
			for( j = 0 ; j < tface->tloops[i].n ; j++ )
			{
				tfin = &(tface->tloops[i].tfins[j]) ;
				curve1 = tfin->curve ;
				interval1 = tfin->interval ;
				curveCalBoxBE(curve1, &interval1, &box) ;
				for( tedge = tsm->tedges ; tedge ; tedge = tedge->next )
				{
					if( mathDist(box.min, tedge->box.min) < tol &&
						mathDist(box.max, tedge->box.max) < tol )
					{
						curve2 = tedgeGetCurve(tedge, &interval2, &flag) ;
						curveIsOverlapBnd(curve1,
										  &interval1,
										  curve2,
										  &interval2,
										  tol,
										  5,
										  &yesno) ;
						if( yesno == ID_YES )
						{
							tedgeAdd(tedge, tfin, NULL) ;
							break ;
						}
					}
				}
				if( tedge == NULL ) // 如果没有匹配到, 则生成一个新的TEDGE
				{
					tedge = tedgeCreate() ;
					tedgeAdd(tedge, tfin, &box) ;
					tsmAddEdge(tsm, tedge) ;
				}
			}
		}
	}

	return ;
}

int tsmEstmChd(TSM* tsm, double dTol, double chd, double min)
{
	TFACE* tface ;

	if( tsm )
	{
		for( tface = tsm->tfaces ; tface ; tface = tface->next )
		{
			surfaceCalLAppxStep2(tface->surface, 
								 NULL, 
								 4, 
								 dTol, 
								 chd,
								 min,
								 &tface->su, 
								 &tface->sv,
								 &tface->dd) ;
		}

		return 1 ;
	}
	else
		return 0 ;
}

// qTol是IGES模型的容差, 即点重合容差
int tsmDiscEdge(TSM* tsm, double qTol, double dTol, double chd, double min)
{
	int k, yesno, err = 0 ;
	double s, t, d, dd, tol = gpGetTol() ;
	PNT3D p, refp ;
	VEC3D v ;
	STRPT* disc = NULL, *temp = NULL ;
	TFIN* tfin ;
	TEDGE* tedge ;
	TFACE* tface ;

	if( tsm == NULL )
		return 0 ;

	for( tedge = tsm->tedges ; tedge ; tedge = tedge->next )
	{
		tfin = tedge->tfins ; // 取出TEDGE中的第一个TFIN进行剖分处理
		if( tfin == NULL )
			return 0 ; // error
		dd = tedgeGetDD(tedge) ; // 取tfins中最小的dd
		if( tfin->curve == NULL ||
			curveDiscAL(tfin->curve, 
						&(tfin->interval),
						dd,
						dTol,
						8,
						STRPT_PT | STRPT_VEC, // nt modify 2021/4/30
						&temp) != ERSUCSS ||
			temp == NULL ||
			temp->np < 2 )
			return 0 ; // error

		for( tfin = tedge->tfins ; tfin ; tfin = tfin->link )
		{
			tface = tfinGetTFace(tfin) ;
			disc = strptCopy(temp) ;
			curveStrPtIsSameDir(tfin->curve, &tfin->interval, disc, qTol, &yesno) ;
			if( yesno == ID_NO )
				strptRev(disc, 0) ;
			s = t = 1.e20 ;
			for( k = 0 ; k < disc->np ; k++ )
			{
				surfaceSnap(tface->surface, disc->ps[k], p, &s, &t) ;
				disc->vs[k][0] = s ;
				disc->vs[k][1] = t ;
				d = mathDist(disc->ps[k], p) ;
				if( d > qTol ) // nt add 2021/5/3
					err++ ; // nt add 2021/5/3
				if( sdpiGetNumOfDegePtAPI(tface->pSDPI) > 0 ) // 处理有退化点的情况
				{
					if( k == 0 && // 曲线起点是是曲面的退化点
						sdpiIsAtDegePtAPI(tface->pSDPI, disc->ps[k], tol) == ID_YES )
					{
						curveEvalTangent(tfin->curve, tfin->interval.t1, p, v) ;
						mathEvalLin(p, v, tol*100, refp) ;
						sdpiCorUVAtDegePtAPI(tface->pSDPI, p, refp, tol, disc->vs[k]) ;
					}
					else
					if( k == disc->np-1 &&  // 曲线终点是是曲面的退化点
						sdpiIsAtDegePtAPI(tface->pSDPI, disc->ps[k], tol) == ID_YES )
					{
						curveEvalTangent(tfin->curve, tfin->interval.t2, p, v) ;
						mathEvalLin(p, v, -tol*100, refp) ;
						sdpiCorUVAtDegePtAPI(tface->pSDPI, p, refp, tol, disc->vs[k]) ;
					}
				}
			}
			if( tfin->flag < 0 ) // nt add 2010/5/19
				strptRev(disc, 0) ;
			tfin->disc = disc ;
		}
		strptFree(temp) ;
		temp = NULL ;
	}

	return 1 ;
}

int tsmCorrChd(TSM* tsm, double min, double chd)
{
	int i, j, k ;
	double d, s, dd ;
	STRPT* disc ;
	TFIN* tfin ;
	TFACE* tface ;

	if( tsm )
	{
		for( tface = tsm->tfaces ; tface ; tface = tface->next )
		{
			if( tface->dd > 0.5*chd )
			{
				dd = MAX_DBL ;
				for( i = 0 ; i < tface->n ; i++ )
				{
					for( j = 0 ; j < tface->tloops[i].n ; j++ )
					{
						tfin = &(tface->tloops[i].tfins[j]) ;
						disc = tfin->disc ;
						if( disc &&
							disc->np > 1 )
						{
							for( s = 0., k = 1 ; k < disc->np ; k++ )
								s += mathDist(disc->ps[k-1], disc->ps[k]) ;
							d = mathMAX(s/(disc->np-1), 10*min) ;
							if( dd > d )
								dd = d ;
						}
					}
				}
				if( tface->dd > dd )
				{
					tface->su *= (dd/tface->dd) ;
					tface->sv *= (dd/tface->dd) ;
					tface->dd = dd ;
				}
			}
		}

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2021/8/3 对所有裁剪曲面进行剖分
int tsmTess(TSM* tsm)
{
	int i, j, k, n ;
	TSI* tsi = NULL ;
	OPTION* option = NULL ;
	TEDGE* tedge = NULL ;
	TFIN* tfin = NULL ;
	TFACE* tface = NULL ;

	if( tsm == NULL )
		return 0 ; // error
	option = camGetOption() ;

	for( tface = tsm->tfaces ; tface ; tface = tface->next )
	{
		n = tfaceGetNumOfFin(tface) ;
		tsi = tsiCreate2(n) ;
		tsi->surface = tface->surface ;
		tsi->su = tface->su ; // nt add 2021/8/4
		tsi->sv = tface->sv ; // nt add 2021/8/4
		tsi->aat = 0 ; // nt add 2021/6/26
		tsi->dTol = option->dTol ; // 0.25
		tsi->dAng = PI1/8 ; // nt add 2012/11/30
		tsi->qTol = 0.1 ; // nt add 2021/5/3
		tsi->pTol = 1.e-5 ; // nt add 2021/5/3
		tsi->min = option->minEdge ; // nt add 2021/6/16
		tsi->chd = option->maxEdge ; // nt add 2021/6/16
		tsi->ang = option->minAngle/180*PI1 ; // nt add 2021/6/16
		tsi->area = option->minArea ; // nt add 2021/6/16
		tsi->boxuv = tface->boxuv ;
		tsi->sp = tface->sp ;
		tsi->nOpt = 1000 ; // 6 ; // nt add 2021/6/23
		tsi->nRefine = 8 ; // 8 ; // nt add 2021/6/23
		k = 0 ;
		tsi->nd = n ; // nt add 2021/8/3
		for( i = 0 ; i < tface->n ; i++ )
		{
			for( j = 0 ; j < tface->tloops[i].n ; j++ )
			{
				tfin = &(tface->tloops[i].tfins[j]) ;
				tsi->flags[k] = (i+1)*tfin->flag ;
				tsi->ds[k] = tfin->disc ; // nt add 2021/8/3
				k++ ;
			}
		}
		tsiTess3(tsi) ;
		tface->facet = tsi->facet ;
		//for( i = 0 ; i < 50 ; i++ ) // nt add 2021/8/18
		//{
		//	if( facetOpt(tface->facet, 30., 1000) < 1 )
		//		break ;
		//}
		for( k = 0 ; k < n ; k++ )
			tsi->ds[k] = NULL ;
		tsi->facet = NULL ;
		tsiFree(tsi) ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////