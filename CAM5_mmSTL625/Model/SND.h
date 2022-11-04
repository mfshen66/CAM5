#pragma once

////////////////////////////////////////////////////////////////
// structure of boundary
typedef struct _bnd_cur_node BNDNODE ; // boundary curve
struct _bnd_cur_node
{
	CURVE* curve ;
	INTERVAL interval ;
	BOX3D box ;
	PNT3D vs[2] ;
} ;

/*
 *	SND.cpp
 */
BNDNODE* bndnodeCreate(CURVE* curve, INTERVAL* interval, BOX3D* box) ;
void bndnodeFree(BNDNODE* bndnode) ;
int bndnodeSnap(BNDNODE* bndnode, PNT3D p, double tol, PNT3D q, double* t, int* found) ;
//--------------------------------------------------------------
typedef struct _tr_sur_node TRNODE ; // trimmed surface node
struct _tr_sur_node
{
	SURFACE* surface ;
	BOXUV boxuv;
	FACET* facet ;
	BOX3D box ;
	PNT3D vs[4] ;
} ;

/*
 *	SND.cpp
 */
TRNODE* trnodeCreate(SURFACE* surface, FACET* facet, BOXUV* boxuv, BOX3D* box) ;
void trnodeFree(TRNODE* trnode) ;
int trnodeSnap(TRNODE* trnode, PNT3D p, double tol, PNT3D q, double uv[2], int* found) ;
//--------------------------------------------------------------
// 调用方法:
// (1)sndCreate() : 赋初始值
// (2)反复调用sndSetTr(),sndSetBnd() : 构造
// (3)反复调用sndCal() : 计算最近距离
// (4)sndFree() : 释放空间
//--------------------------------------------------------------
typedef struct _snd SND ; // trimmed surface nearest distance
struct _snd
{
	int nTr ; // 裁剪曲面总数
	TRNODE** trs ;
	int nBnd ;
	BNDNODE** bnds ;
	double d ; // 裁剪曲面剖分的空间误差
	double r ; // 空间范围，在计算空间点到裁剪曲面的最近距离时，要求该点离曲面不超过r
	double tol ; // 判断是否为最近点的算法的收敛条件
} ;

/*
 *	SND.cpp
 */
SND* sndCreate(int nTr, int nBnd, double d, double r, double tol) ;
int sndFree(SND* snd) ;
void sndSetTr(SND* snd, int i, SURFACE* surface, FACET* facet, BOXUV* boxuv, BOX3D* box) ;
void sndSetBnd(SND* snd, int i, CURVE* curve, INTERVAL* interval, BOX3D* box) ;
// p是空间一点（要求该点在裁剪曲面包围盒的一定范围内），找出一张曲面或曲线*pgeo,
// q是*psur上一点距离p最近, uv[0],uv[1]是q在*pgeo上的参数
// entityIsSurface(ENTITY* entity) ; // ID_NO:is not;ID_YES:is
int sndCal(SND* snd, PNT3D p, GEOMETRY** pgeo, PNT3D q, PNT2D uv) ;
////////////////////////////////////////////////////////////////
