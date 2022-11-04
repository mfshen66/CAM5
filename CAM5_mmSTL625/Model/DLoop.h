#ifndef _DLoop_h_
#define _DLoop_h_

////////////////////////////////////////////////////////////////
// DCURVE离散曲线的法矢是曲面的自然法矢
#define DLOOP_MAX 100000

typedef struct _dloop DLOOP ;
struct _dloop
{
	DLOOP* prev ;
	DLOOP* next ;
	int n ;
	DCURVE** dcurves ; // 按其自然走向首尾相连, 对应的参数域曲线在参数域满足外环逆时针，内环顺时针
} ;

/*
 *	DLoop.cpp
 */
DLOOP* dloopCreate(int n) ;
void dloopFree(DLOOP* dloop) ;
void dloopsFree(DLOOP* loops) ;
int dloopIsClose(DLOOP* dloop, double tol) ;
int dloopsIsClose(DLOOP* loops, double tol) ;
int dloopsCut(DLOOP* loops, DCURVE* tool, double tol, int res, DLOOP** lps, int* error) ;
////////////////////////////////////////////////////////////////

#endif