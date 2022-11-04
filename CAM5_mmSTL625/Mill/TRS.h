/*	2010/3/30 nt
 *	the head file of tool revolve surface
 */
#ifndef _TRS_h_
#define _TRS_h_

////////////////////////////////////////////////////////////////
// TRS : 刀具旋转面(绕z轴旋转)
// 注意：在TRS的有关计算中只用到了TRS->curve的xz坐标
//       TRS->curve一般定于一个局部坐标系中
typedef struct _trs TRS ;
struct _trs
{
	CURVE* curve ; // 独享
	INTERVAL interval ;
	BOX3D box ;
	STRPT* strpt ;
	FACET* facet ;
	int n ; // 离散时，一周分多少份, n>=3
} ;

/*
 *	TRS.cpp
 */
TRS* trsCreate(CURVE* curve, INTERVAL* interval) ;
int trsFree(TRS* trs) ;
int trsCalBox(TRS* trs) ;
int trsGetBox(TRS* trs, BOX3D* box) ;
int trsDisc(TRS* trs, double dTol, double dAng, int n) ;
int trsDraw(TRS* trs, void* pVI, int drawMode, int color) ;
int trsPick(TRS* trs, PINFO* pi) ; // return 0=error;1=picked,2=not picked
int trsSerialize(TRS* trs, CArchive& ar) ;
////////////////////////////////////////////////////////////////
#endif