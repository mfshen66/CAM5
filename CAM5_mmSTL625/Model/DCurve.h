#ifndef _DCurve_h_
#define _DCurve_h_

////////////////////////////////////////////////////////////////
typedef struct _dcurve DCURVE ;
struct _dcurve
{
	DCURVE* next ;
	int np ; // number of point
	double (*ps)[3] ; // point array
	double (*ns)[3] ; // unit normal vector array
	int flag ;
} ;

/*
 *	DCurve.cpp
 *	主要接口有dcurveInt(),dcurveAnalysis()和dcurveTrim()
 */
DCURVE* dcurveCreate(int np) ;
void dcurveFree(DCURVE* dcurve) ;
void dcurvesFree(DCURVE* dcurves) ;
DCURVE* dcurveCopy(DCURVE* dcurve) ;
int dcurveRev(DCURVE* dcurve, int revVec) ;
double* dcurveGetBeginPt(DCURVE* dcurve, PNT3D b) ;
double* dcurveGetEndPt(DCURVE* dcurve, PNT3D e) ;
double* dcurveGetBeginTangent(DCURVE* dcurve, PNT3D b, VEC3D tvec) ;
double* dcurveGetEndTangent(DCURVE* dcurve, PNT3D e, VEC3D tvec) ;
int dcurveIsLinkAt(DCURVE* dcurve1, DCURVE* dcurve2, double tol) ; // nt add 2021/5/2
int dcurveMove(DCURVE* dcurve, VEC3D v) ;
int dcurveRotate(DCURVE* dcurve, PNT3D pivot, VEC3D axis, double angle) ;
int dcurveMirror(DCURVE* dcurve, PNT3D pivot, VEC3D normal) ;
int dcurveToLocal(DCURVE* dcurve, RFRAME* pLF) ;
int dcurveToWorld(DCURVE* dcurve, RFRAME* pLF) ;
int dcurveScale(DCURVE* dcurve, double scale) ;
int dcurveCalBox(DCURVE* dcurve, double tol, BOX3D* box) ;
int dcurveSnap(DCURVE* dcurve, double tol, PNT3D p, PNT3D q, double* pt, double* pd) ;
int dcurveIsPtOn(DCURVE* dcurve, PNT3D p, double tol, int* pyesno) ;
int dcurveIsClose(DCURVE* dcurve, double tol) ;
int dcurveIsOverlap(DCURVE* dcurve1, DCURVE* dcurve2, double tol, int* pyesno) ;
int dcurvePick(DCURVE* dcurve, PNT3D pos, VEC3D dir, double scope, double* pz, double* pu, int* picked) ;
int dcurveIntLin(DCURVE* dcurve, PNT3D begin, VEC3D dir, double tol, double mol, INTPT** intpts) ;
int dcurveIntPln(DCURVE* dcurve, PNT3D pivot, VEC3D normal, double tol, double mol, INTPT** intpts) ;
int dcurveInt(DCURVE* dcurve1, BOX3D* box1, DCURVE* dcurve2, BOX3D* box2, double tol, int overlap, INTPT** intpts) ;
int dcurveEstmTC(DCURVE* dcurve, int i, VEC3D tvec, double* c) ;
int dcurveEvalPos(DCURVE* dcurve, double t, PNT3D pos, VEC3D normal) ;
void dcurveRemDbl(DCURVE* dcurve, double tol) ; // 去掉首尾重合的节点
DCURVE* dcurveMake(DCURVE* dcurve, double s, double t, double tol) ;
int dcurveAnalysis(DCURVE* dcurve, int chkAll, PNT3D p, double A, double rs[5], int* nFlags) ;
typedef struct _dctrim DCTRIM ; // only used in dcurveTrim
struct _dctrim
{
	int error ;
	DCURVE* dcurves ; // 单向链表
	INTPT* intpts ; // 双向链表
} ;
void dctrimInit(DCTRIM* dctrim) ;
void dctrimAddDCurve(DCTRIM* dctrim, DCURVE* dcurve) ;
void dctrimAddIntPt(DCTRIM* dctrim, INTPT* intpt) ;
int dctrimGetNumOfDCurve(DCTRIM* dctrim) ;
int dctrimGetNumOfIntPt(DCTRIM* dctrim) ;
int dcurveTrim(DCURVE* dcurve, DCURVE* tool, double tol, DCTRIM* rs) ;
////////////////////////////////////////////////////////////////

#endif