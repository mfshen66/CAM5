#ifndef _CUTSUR_h_
#define _CUTSUR_h_

////////////////////////////////////////////////////////////////
// 调用方法:
// (1)cutsurCreate() : 赋初始值
// (2)cutsurMake() : 构造分割曲面
// (3)cutsurFree() : 释放空间
//--------------------------------------------------------------
typedef struct _cutsur CUTSUR ;
struct _cutsur
{
	SURFACE* surface ; // 被切割的曲面
	BOXUV boxuv ; // of surface
	BOX3D box ; // nt add 2010/3/15
	int n ; // 1 or 2 : 用于切割的曲面总数
	SURFACE* cut1 ; // 用于切割的曲面1
	BOXUV boxuv1 ;
	BOX3D box1 ; // nt add 2010/3/15
	SURFACE* cut2 ; // 用于切割的曲面2
	BOXUV boxuv2 ;
	BOX3D box2 ; // nt add 2010/3/15

	// nt add 2010/3/15
	CURVE* cutcur1 ; // intersection curve of surface/cut1
	INTERVAL intv1 ;
	CURVE* cutcur2 ; // intersection curve of surface/cut2
	INTERVAL intv2 ;

	double tol ; // 构造分割曲面的精度
	// stepu和stepv仅定义于boxuv所指定的范围，即假设这个范围
	// 是[0,1]x[0,1]
	double stepu ; // 相邻的u=常数等参数线在参数域内的间距
	double stepv ; // 相邻的v=常数等参数线在参数域内的间距
	double step ; // 定义于surface参数域内
	int uvflag ; // 0(u=constant) or 1(v=constant)
	double uv ; // u or v
	CURVE* isocur ; // 对应uvflag和uv的surface的等参数线
	INTERVAL interval ; // isocur的interval
	int nSection ;
	double (*ss)[2] ; // surface的参数，分别对应于cut1和cut2
	int m ; // 得到NURBS曲面数
	BSSUR* bss[3] ;
} ;

/*
 *	CutSur.cpp
 */
CUTSUR* cutsurCreate(SURFACE* surface,
					 BOXUV* boxuv,
					 BOX3D* box,
					 SURFACE* cut1,
					 BOXUV* boxuv1,
					 BOX3D* box1,
					 SURFACE* cut2,
					 BOXUV* boxuv2,
					 BOX3D* box2,
					 double tol) ;
int cutsurFree(CUTSUR* cutsur) ;
int cutsurMake(CUTSUR* cutsur) ;
////////////////////////////////////////////////////////////////

#endif