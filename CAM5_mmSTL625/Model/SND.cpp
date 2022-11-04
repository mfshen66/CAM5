/*	2016/8/16 nt
 *	implementation file of point/trimmed_surfaces nearest distance algorithm
 */
#include "StdAfx.h"
#include "SND.h"

////////////////////////////////////////////////////////////////
// boundary curve node
BNDNODE* bndnodeCreate(CURVE* curve, INTERVAL* interval, BOX3D* box)
{
	BNDNODE* bndnode ;

	bndnode = new BNDNODE() ;
	bndnode->curve = curve ;
	bndnode->interval = *interval ;
	bndnode->box = *box ;
	curveEvalPos(curve, interval->t1, bndnode->vs[0]) ;
	curveEvalPos(curve, interval->t2, bndnode->vs[1]) ;			 

	return bndnode ;
}

void bndnodeFree(BNDNODE* bndnode)
{
	if( bndnode )
		delete bndnode ;

	return ;
}

int bndnodeSnap(BNDNODE* bndnode, PNT3D p, double tol, PNT3D q, double* t, int* found)
{
	return ERSUCSS ;
}
//--------------------------------------------------------------
/*  2016/8/27 nt
 *  判断点是否在参数域内
 *	return ID_OUT or ID_IN
 */
/*int facetIsInUV(FACET* facet, double uv[2])
{
	int i ;
	PNT3D gc ;

	if( !facet ||
		!uv )
		return ERUNSUC ;

	for( i = 0 ; i < facet->ntria ; i++ )
	{
		mathCalTri2DGCP(facet->nodes[facet->trias[i][0]].uv,
						facet->nodes[facet->trias[i][1]].uv,
						facet->nodes[facet->trias[i][2]].uv,
						uv,
						gc) ;
		if( gc[0] > -MIN_DBL &&
			gc[1] > -MIN_DBL &&
			gc[2] > -MIN_DBL )
			return IDIN ;
	}

	return IDOUT ;
}*/

// trimmed surface node
TRNODE* trnodeCreate(SURFACE* surface, FACET* facet, BOXUV* boxuv, BOX3D* box)
{
	double u, v ;
	TRNODE* trnode ;

	trnode = new TRNODE() ;
	trnode->surface = surface ;
	trnode->facet = facet ;
	trnode->boxuv = *boxuv ;
	trnode->box = *box ;

	u = trnode->boxuv.min[0] ;
	v = (trnode->boxuv.min[1]+trnode->boxuv.max[1])/2 ;
	surfaceEvalPos(surface, u, v, trnode->vs[0]) ;
	u = trnode->boxuv.max[0] ;
	surfaceEvalPos(surface, u, v, trnode->vs[1]) ;
	u = (trnode->boxuv.min[0]+trnode->boxuv.max[0])/2 ;
	v = trnode->boxuv.min[1] ; ;
	surfaceEvalPos(surface, u, v, trnode->vs[2]) ;
	v = trnode->boxuv.max[1] ; ;
	surfaceEvalPos(surface, u, v, trnode->vs[3]) ;

	return trnode ;
}

void trnodeFree(TRNODE* trnode)
{
	if( trnode )
		delete trnode ;

	return ;
}

int trnodeSnap(TRNODE* trnode, PNT3D p, double tol, PNT3D q, double uv[2], int* found)
{
	return ERSUCSS ;
}
//--------------------------------------------------------------
// surface nearest distance
// 2016/8/16 nt
// create SND
SND* sndCreate(int nTr, int nBnd, double d, double r, double tol)
{
	int i ;

	SND* snd = new SND() ;
	snd->nTr = nTr ;
	snd->trs = new TRNODE*[nTr] ;
	snd->nBnd = nBnd ;
	snd->bnds = new BNDNODE*[nBnd] ;
	for( i = 0 ; i < nTr ; i++ )
		snd->trs[i] = NULL ;
	for( i = 0 ; i < nBnd ; i++ )
		snd->bnds[i] = NULL ;
	snd->d = d ;
	snd->r = r ;
	snd->tol = tol ;

	return snd ;
}

int sndFree(SND* snd)
{
	int i ;

	if( snd )
	{
		if( snd->trs )
		{
			for( i = 0 ; i < snd->nTr ; i++ )
			{
				if( snd->trs[i] )
					trnodeFree(snd->trs[i]) ;
			}
			delete []snd->trs ;
		}
		if( snd->bnds )
		{
			for( i = 0 ; i < snd->nBnd ; i++ )
			{
				if( snd->bnds[i] )
					bndnodeFree(snd->bnds[i]) ;
			}
			delete []snd->bnds ;
		}
		delete snd ;

		return ERSUCSS ;
	}
	else
		return ERUNSUC ;
}
//--------------------------------------------------------------
void sndSetTr(SND* snd, int i, SURFACE* surface, FACET* facet, BOXUV* boxuv, BOX3D* box)
{
	if( snd )
	{
		snd->trs[i] = trnodeCreate(surface, facet, boxuv, box) ;
	}

	return ;
}

void sndSetBnd(SND* snd, int i, CURVE* curve, INTERVAL* interval, BOX3D* box)
{
	if( snd )
	{
		snd->bnds[i] = bndnodeCreate(curve, interval, box) ;
	}

	return ;
}
//--------------------------------------------------------------
/*double mathDistPntBox(PNT3D p, BOX3D* box)
{
	int k ;
	double d1, d2, d = 0. ;

	for( k = 0 ; k < 3 ; k++ )
	{
		d1 = box->min[k]-p[k] ;
		d2 = box->max[k]-p[k] ;
		if( d1*d2 > 0. )
		{
			d1 = fabs(d1) ;
			d2 = fabs(d2) ;
			if( d1 < d2 )
				d += d1*d1 ;
			else
				d += d2*d2 ;
		}
	}

	return sqrt(d) ;
}*/

int sndCal(SND* snd, PNT3D p, GEOMETRY** pgeo, PNT3D q, PNT2D uv)
{
	int i, found ;
	double min, d ;
	PNT3D temp_q ;
	PNT2D temp_uv ;

	*pgeo = NULL ;
	min = MAX_DBL ;
	for( i = 0 ; i < snd->nTr ; i++ )
	{
		temp_uv[0] = 1.e20 ;
		d = mathDistPntBox(p, &(snd->trs[i]->box)) ;
		if( min > d &&
			trnodeSnap(snd->trs[i], p, snd->tol, temp_q, temp_uv, &found) == ERSUCSS &&
			found == 1 )
		{
			d = mathDist(p, temp_q) ;
			if( min > d )
			{
				*pgeo = (GEOMETRY*)(snd->trs[i]->surface) ;
				memcpy(q, temp_q, sizeof(PNT3D)) ;
				memcpy(uv, temp_uv, sizeof(PNT2D)) ;
				min = d ;
			}
		}
	}
	if( *pgeo )
		return ERSUCSS ;

	min = MAX_DBL ;
	for( i = 0 ; i < snd->nBnd ; i++ )
	{
		temp_uv[0] = 1.e20 ;
		d = mathDistPntBox(p, &(snd->bnds[i]->box)) ;
		if( min > d &&
			bndnodeSnap(snd->bnds[i], p, snd->tol, temp_q, temp_uv, &found) == ERSUCSS &&
			found == 1 )
		{
			d = mathDist(p, temp_q) ;
			if( min > d )
			{
				*pgeo = (GEOMETRY*)(snd->bnds[i]->curve) ;
				memcpy(q, temp_q, sizeof(PNT3D)) ;
				uv[0] = temp_uv[0] ;
				min = d ;
			}
		}
	}

	return ERSUCSS ;
}
////////////////////////////////////////////////////////////////
