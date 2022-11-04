/*	2010/5/18 nt
 *	head file of structure TSM definitions
 */
#ifndef _TSM_h_
#define _TSM_h_

////////////////////////////////////////////////////////////////
typedef struct _tedge TEDGE ; // edge
typedef struct _tfin TFIN ; // fin
typedef struct _tloop TLOOP ; // loop
typedef struct _tface TFACE ; // face
typedef struct _tsm TSM ; // surface model
//--------------------------------------------------------------
struct _tedge
{
	TEDGE* next ;
	TFIN* tfins ; // use TFIN->link
	BOX3D box ;
} ;

TEDGE* tedgeCreate() ;
void tedgeFree(TEDGE* tedge) ;
void tedgeAdd(TEDGE* tedge, TFIN* tfin, BOX3D* box) ;
CURVE* tedgeGetCurve(TEDGE* tedge, INTERVAL* interval, int* flag) ;
double tedgeGetDD(TEDGE* tedge) ;
int tedgeGetNumOfTFin(TEDGE* tedge) ;
//--------------------------------------------------------------
/*	2010/5/18 nt
 *	trim fin
 */
struct _tfin
{
	TFIN* link ; // used only in TEDGE
	TLOOP* tloop ; // parent
	TEDGE* tedge ; // 对应的TEDGE
	int flag ;
	CURVE* curve ; // in 3d space
	INTERVAL interval ;
	STRPT* disc ;
} ;

TFACE* tfinGetTFace(TFIN* tfin) ;

/*	2010/5/18 nt
 *	trim loop
 */
struct _tloop
{
	TFACE* tface ; // parent
	int flag ; // 0=outer loop, >0=inner loop
	int n ; // how many trim edges
	TFIN* tfins ; // array of TFIN, index from 0 to n-1
} ;

int tloopAllocFins(TLOOP* tloop, int nFin) ;
int tloopFreeFins(TLOOP* tloop) ;
//--------------------------------------------------------------
/*	2010/5/18 nt
 *	trim face
 */
struct _tface
{
	TFACE* next ; // nt add 2021/8/2
	SURFACE* surface ;
	BOXUV boxuv ;
	double su ;
	double sv ;
	double dd ;
	double pes[2] ;
	void* pSDPI ;
	int n ; // how many trim loops
	TLOOP* tloops ; // array, index from 0 to n-1
	int sp ; // tloops defined : 1=in space, 0=in parameter domain
	BOX3D box ; // nt add 2021/8/2
	FACET* facet ; // nt add 2021/8/2
} ;

TFACE* tfaceCreate(SURFACE* surface, BOXUV* boxuv, int nLoop, int sp) ;
int tfaceFree(TFACE* tsur) ;
int tfaceGetNumOfFin(TFACE* tface) ;
void tfaceCopyFinAll(TFACE* tface, COA& cs) ; // nt add 2021/3/6
//--------------------------------------------------------------
struct _tsm
{
	TEDGE* tedges ; // all edges, use TEDGE->next
	TFACE* tfaces ; // all trimmed surface. use TFACE->next
} ;

TSM* tsmCreate() ;
void tsmFree(TSM* tsm) ;
void tsmAddEdge(TSM* tsm, TEDGE* tedge) ;
void tsmAddFace(TSM* tsm, TFACE* tface, double tol) ; // 增加TFACE的过程中匹配边，生成TEDGE, tol是点重合容差
int tsmEstmChd(TSM* tsm, double dTol, double chd, double min) ;
int tsmDiscEdge(TSM* tsm, double qTol, double dTol, double chd, double min) ;
int tsmCorrChd(TSM* tsm, double min, double chd) ; // nt add 2021/8/8
int tsmTess(TSM* tsm) ;
////////////////////////////////////////////////////////////////

#endif