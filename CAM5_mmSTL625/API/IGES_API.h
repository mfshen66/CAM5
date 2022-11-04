#ifndef _IGES_API_h_
#define _IGES_API_h_

#ifdef IGES_EXPORTS
#define IGES_API __declspec(dllexport)
#else
#define IGES_API __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////
//	IGES API
//==============================================================
typedef struct _globalsec GLOBALSEC ;
struct _globalsec
{
	char pdc ; // field 1---Parameter delimeter
	char rdc ; // field 2---Record delimeter
	char sIdSend[512] ; // field 3---Product ID of sending system
	char sFileSend[512] ; // field 4---Name of IGES file
	char sSystem[512] ;   // field 5---System id
	char sVersion[64] ;  // field 6---Version of preprocessor
	int nBitInt ; // field 7---Num of binary bits for integer Rep.
	int nPowFlt ; // field 8---Single precision Magnitude
	int nBitFlt ; // field 9---Single precision Significance
	int nPowDbl ; // field 10---Double precision Magnitude
	int nBitDbl ; // field 11---Double precision Significance
	char sIdReceive[512] ;// field 12---Product ID for the receiver
	double dScaleModel ; // field 13---Model space scale
	int nFlagUnit ; // field 14---Unit flag
	char sUnit[64] ; // field 15---A String naming the Model Units
	int nWeightLine ; // field 16---Max Num of Line weight gradations
	double dWidthLine ; // field 17---Thickest Line width
	char sTime[128] ; // field 18---Date and time when IGES file created
	double dRes ; // field 19---Min. user_intended Resolution
	double dMaxCoord ; // field 20---Approximate Max Coor. Value
	char sAuthor[128] ; // field 21---Name of Author
	char sOrgan[512] ; // field 22---Author's Organization
	int nIgesVer ; // field 23---IGES version number
	int nCode ; // field 24---Drafting standard code
	char sTime2[512] ; // field 25
	char sDes[512] ; // field 26
	int nPage ; // 占用多少行
} ;
//==============================================================
typedef struct _de DE ;
struct _de
{
	int type ; // directory entry field 1                         #
	int pdPos ; // directory entry field 2=parameter data pos     =>
	int strct ; // directory entry field 3                        # =>
	int font ; // directory entry field 4                         # =>
	int level ; // directory entry field 5                        # =>
	int view ; // directory entry field 6                         0 =>
	int matrix ; // directory entry field 7                       0 =>
	int lda ; // directory entry field 8=lable display associate  0 =>
	char status[9] ; // directory entry field 9                   #
	int seqNo ; // directory entry field 10                       D#
	// same with field 1                                          #
	int lineWeight ; // directory entry field 12                  #
	int color ; // directory entry field 13                       # =>
	int nPage ; // directory entry field 14=parameter line count  #
	int form ; // directory entry field 15                        #
	// field 16 reserved
	// field 17 reserved
	int lable ; // directory entry field 18                       #
	int sub ; // directory entry field 19                         #
	// directory entry field 20 : m_seqNo+1
} ;
//==============================================================
typedef void IENTITY ;
IGES_API IENTITY* ientityCreate(int type) ;
IGES_API void ientityFree(IENTITY* pE) ;
IGES_API void ientityGetDE(IENTITY* pE, DE* pDE) ;
IGES_API void ientitySetDE(IENTITY* pE, DE* pDE) ;
IGES_API int ientityIsOK(IENTITY* pE) ;
IGES_API int ientityIsUsed(IENTITY* pE) ; // nt add 2011/11/16
IGES_API void ientityUsed(IENTITY* pE) ; // nt add 2011/11/16
IGES_API int ientityGetType(IENTITY* pE) ;
IGES_API int ientityGetColor(IENTITY* pE) ; // nt add 2010/11/7, return directory entry of coor
IGES_API int ientityGetMatrix(IENTITY* pE) ; // nt add 2010/11/7, return directory entry of matrix
//--------------------------------------------------------------
// matrix
IGES_API void matrixGet(IENTITY* pE, double a[4][4]) ;
IGES_API void matrixSet(IENTITY* pE, double a[4][4]) ;
IGES_API void matrixGet2(IENTITY* pE, double o[3], double X[3], double Y[3], double Z[3]) ; // nt add 2010/11/7
IGES_API void matrixSet2(IENTITY* pE, double o[3], double X[3], double Y[3], double Z[3]) ; // nt add 2010/11/7
//--------------------------------------------------------------
// color
IGES_API void colorGet(IENTITY* pE, double rgb[3]) ;
IGES_API void colorSet(IENTITY* pE, double rgb[3]) ;
//--------------------------------------------------------------
// LD
IGES_API int ldGetIP(IENTITY* pE) ;
IGES_API void ldSetIP(IENTITY* pE, int IP) ;
IGES_API int ldGetN(IENTITY* pE) ;
IGES_API void ldCreate(IENTITY* pE, int n) ;
IGES_API double ldGetZ(IENTITY* pE) ;
IGES_API void ldSetZ(IENTITY* pE, double z) ;
IGES_API void ldGetTuple(IENTITY* pE, int i, double tuple[6]) ;
IGES_API void ldSetTuple(IENTITY* pE, int i, double tuple[6]) ;
//--------------------------------------------------------------
// point : point
IGES_API void pointGetP(IENTITY* pE, double p[3]) ;
IGES_API void pointSetP(IENTITY* pE, double p[3]) ;
//--------------------------------------------------------------
// curve : line
IGES_API void lineGetB(IENTITY* pE, double b[3]) ;
IGES_API void lineGetE(IENTITY* pE, double e[3]) ;
IGES_API void lineSetB(IENTITY* pE, double b[3]) ;
IGES_API void lineSetE(IENTITY* pE, double e[3]) ;
//--------------------------------------------------------------
// curve : arc
IGES_API double arcGetR(IENTITY* pE) ;
IGES_API double arcGetZ(IENTITY* pE) ;
IGES_API void arcSetZ(IENTITY* pE, double z) ;
IGES_API void arcGetCenter(IENTITY* pE, double center[2]) ;
IGES_API void arcSetCenter(IENTITY* pE, double center[2]) ;
IGES_API void arcGetStart(IENTITY* pE, double start[2]) ;
IGES_API void arcSetStart(IENTITY* pE, double start[2]) ;
IGES_API void arcGetEnd(IENTITY* pE, double end[2]) ;
IGES_API void arcSetEnd(IENTITY* pE, double end[2]) ;
//--------------------------------------------------------------
// curve : conic
IGES_API void conicGetABCDEF(IENTITY* pE, double abcdef[6]) ;
IGES_API void conicSetABCDEF(IENTITY* pE, double abcdef[6]) ;
IGES_API double conicGetZ(IENTITY* pE) ;
IGES_API void conicSetZ(IENTITY* pE, double z) ;
IGES_API void conicGetStart(IENTITY* pE, double start[2]) ;
IGES_API void conicSetStart(IENTITY* pE, double start[2]) ;
IGES_API void conicGetEnd(IENTITY* pE, double end[2]) ;
IGES_API void conicSetEnd(IENTITY* pE, double end[2]) ;
//--------------------------------------------------------------
// curve : NURBS curve
IGES_API int bscurGetCount(IENTITY* pE) ;
IGES_API int bscurGetOrder(IENTITY* pE) ;
IGES_API double bscurGetKnot(IENTITY* pE, int i) ; // get one knot
IGES_API void bscurGetCP(IENTITY* pE, int i, double cp[4]) ; // get one control point
IGES_API int bscurCreate(IENTITY* pE, int count, int order) ; // allocate memory
IGES_API void bscurSetKnot(IENTITY* pE, int i, double knot) ; // set one knot
IGES_API void bscurSetCP(IENTITY* pE, int i, double cp[4]) ; // set one control point
//--------------------------------------------------------------
// curve : offset curve
IGES_API int offcurGetBase(IENTITY* pE) ;
IGES_API void offcurSetBase(IENTITY* pE, int base) ;
IGES_API int offcurGetFlag(IENTITY* pE) ;
IGES_API void offcurSetFlag(IENTITY* pE, int flag) ;
IGES_API int compGetDisCur(IENTITY* pE) ; // DE pos of the distance curve
IGES_API void compSetDisCur(IENTITY* pE, int disCur) ; // DE pos of the distance curve
IGES_API int offcurGetIDim(IENTITY* pE) ;
IGES_API void offcurSetIDim(IENTITY* pE, int iDim) ;
IGES_API int offcurGetPType(IENTITY* pE) ;
IGES_API void offcurSetPType(IENTITY* pE, int ptype) ;
IGES_API void offcurGetD1D2T1T2(IENTITY* pE, double d1d2[2], double t1t2[2]) ;
IGES_API void offcurSetD1T1D2T2(IENTITY* pE, double d1d2[2], double t1t2[2]) ;
IGES_API void offcurGetNormal(IENTITY* pE, double normal[3]) ;
IGES_API void offcurSetNormal(IENTITY* pE, double normal[3]) ;
IGES_API void offcurGetTs(IENTITY* pE, double ts[2]) ; // parameter interval
IGES_API void offcurSetTs(IENTITY* pE, double ts[2]) ;
//--------------------------------------------------------------
// curve : parametric spline curve
IGES_API int pscurGetCType(IENTITY* pE) ;
IGES_API void pscurSetCType(IENTITY* pE, int ctype) ;
IGES_API int pscurGetCont(IENTITY* pE) ;
IGES_API void pscurSetCont(IENTITY* pE, int cont) ;
IGES_API int pscurGetNDim(IENTITY* pE) ;
IGES_API void pscurSetNDim(IENTITY* pE, int nDim) ;
IGES_API int pscurGetNSegm(IENTITY* pE) ;
IGES_API void pscurSetNSegm(IENTITY* pE, int nSegm) ; // need allocate memory
IGES_API double pscurGetT(IENTITY* pE, int i) ;
IGES_API void pscurSetT(IENTITY* pE, int i, double t) ;
IGES_API void pscurGetCs(IENTITY* pE, int i, double cs[4][3]) ; // 第i段的4个矢量系数
IGES_API void pscurSetCs(IENTITY* pE, int i, double cs[4][3]) ;
//--------------------------------------------------------------
// curve : composite curve
IGES_API int compGetN(IENTITY* pE) ;
IGES_API void compCreate(IENTITY* pE, int n) ;
IGES_API int compGetCurve(IENTITY* pE, int i) ; // DE pos of the curve
IGES_API void compSetCurve(IENTITY* pE, int i, int curve) ; // DE pos of the curve
//--------------------------------------------------------------
// curve : curve on surface
IGES_API int pcurveGetMethod(IENTITY* pE) ;
IGES_API void pcurveSetMethod(IENTITY* pE, int method) ;
IGES_API int pcurveGetSurface(IENTITY* pE) ;
IGES_API void pcurveSetSurface(IENTITY* pE, int surface) ;
IGES_API int pcurveGetCurve(IENTITY* pE) ;
IGES_API void pcurveSetCurve(IENTITY* pE, int curve) ;
IGES_API int pcurveGetBnd(IENTITY* pE) ;
IGES_API void pcurveSetBnd(IENTITY* pE, int bnd) ;
IGES_API int pcurveGetPrefer(IENTITY* pE) ;
IGES_API void pcurveSetPrefer(IENTITY* pE, int prefer) ;
//--------------------------------------------------------------
// surface : plane
IGES_API void planeGetABCD(IENTITY* pE, double abcd[4]) ;
IGES_API void planeSetABCD(IENTITY* pE, double abcd[4]) ;
IGES_API int planeGetLoop(IENTITY* pE) ; // DE pos of the loop
IGES_API void planeSetLoop(IENTITY* pE, int loop) ; // DE pos of the loop
IGES_API void planeGetP(IENTITY* pE, double p[3]) ;
IGES_API void planeSetP(IENTITY* pE, double p[3]) ;
IGES_API double planeGetSize(IENTITY* pE) ;
IGES_API void planeSetSize(IENTITY* pE, double size) ;
//--------------------------------------------------------------
// surface : ruled surface
IGES_API int ruleGetCurve1(IENTITY* pE) ;
IGES_API void ruleSetCurve1(IENTITY* pE, int curve1) ;
IGES_API int ruleGetCurve2(IENTITY* pE) ;
IGES_API void ruleSetCurve2(IENTITY* pE, int curve2) ;
IGES_API int ruleGetDirFlag(IENTITY* pE) ;
IGES_API void ruleSetDirFlag(IENTITY* pE, int dirFlag) ;
IGES_API int ruleIsDevelopable(IENTITY* pE) ;
IGES_API void ruleSetDevelopable(IENTITY* pE, int bDev) ;
//--------------------------------------------------------------
// surface : ruled surface
IGES_API int revolveGetAxis(IENTITY* pE) ; // axis line
IGES_API void revolveSetAxis(IENTITY* pE, int axis) ; // axis line
IGES_API int revolveGetCurve(IENTITY* pE) ;
IGES_API void revolveSetCurve(IENTITY* pE, int curve) ;
IGES_API double revolveGetSA(IENTITY* pE) ;
IGES_API void revolveSetSA(IENTITY* pE, double sa) ;
IGES_API double revolveGetTA(IENTITY* pE) ;
IGES_API void revolveSetTA(IENTITY* pE, double ta) ;
//--------------------------------------------------------------
// surface : NURBS surface
IGES_API int bssurGetCountU(IENTITY* pE) ;
IGES_API int bssurGetCountV(IENTITY* pE) ;
IGES_API int bssurGetOrderU(IENTITY* pE) ;
IGES_API int bssurGetOrderV(IENTITY* pE) ;
IGES_API double bssurGetKnotU(IENTITY* pE, int i) ; // get one knot
IGES_API double bssurGetKnotV(IENTITY* pE, int i) ; // get one knot
IGES_API void bssurGetCP(IENTITY* pE, int i, int j, double cp[4]) ; // get one control point
IGES_API int bssurCreate(IENTITY* pE, int countu, int countv, int orderu, int orderv) ; // allocate memory
IGES_API void bssurSetKnotU(IENTITY* pE, int i, double knot) ; // set one knot
IGES_API void bssurSetKnotV(IENTITY* pE, int i, double knot) ; // set one knot
IGES_API void bssurSetCP(IENTITY* pE, int i, int j, double cp[4]) ; // set one control point
//--------------------------------------------------------------
// surface : tabulated cylinder
IGES_API int tcylGetCurve(IENTITY* pE) ;
IGES_API void tcylSetCurve(IENTITY* pE, int curve) ;
IGES_API void tcylGetP(IENTITY* pE, double p[3]) ;
IGES_API void tcylSetP(IENTITY* pE, double p[3]) ;
//--------------------------------------------------------------
// surface : offset surface
IGES_API void offsurGetNormal(IENTITY* pE, double normal[3]) ;
IGES_API void offsurSetNormal(IENTITY* pE, double normal[3]) ;
IGES_API double offsurGetD(IENTITY* pE) ;
IGES_API void offsurSetD(IENTITY* pE, double d) ;
IGES_API int offsurGetBase(IENTITY* pE) ;
IGES_API void offsurSetBase(IENTITY* pE, int base) ;
//--------------------------------------------------------------
// surface : parametric spline surface
IGES_API int pssurGetCType(IENTITY* pE) ;
IGES_API void pssurSetCType(IENTITY* pE, int ctype) ;
IGES_API int pssurGetPType(IENTITY* pE) ;
IGES_API void pssurSetPType(IENTITY* pE, int ptype) ;
IGES_API void pssurGetMN(IENTITY* pE, int MN[2]) ;
IGES_API void pssurSetMN(IENTITY* pE, int MN[2]) ;
IGES_API double pssurGetU(IENTITY* pE, int i) ;
IGES_API void pssurSetU(IENTITY* pE, int i, double u) ;
IGES_API double pssurGetV(IENTITY* pE, int i) ;
IGES_API void pssurSetV(IENTITY* pE, int i, double v) ;
IGES_API void pssurGetCs(IENTITY* pE, int i, int j, double cs[16][3]) ; // 第(i,j)片的16个矢量系数
IGES_API void pssurSetCs(IENTITY* pE, int i, int j, double cs[16][3]) ;
//--------------------------------------------------------------
// trimmed surface
IGES_API int tsurGetBase(IENTITY* pE) ;
IGES_API void tsurSetBase(IENTITY* pE, int base) ;
IGES_API int tsurGetN1(IENTITY* pE) ;
IGES_API void tsurSetN1(IENTITY* pE, int n1) ;
IGES_API int tsurGetN2(IENTITY* pE) ;
IGES_API void tsurSetN2(IENTITY* pE, int n2) ; // need to allocate memory
IGES_API int tsurGetOuter(IENTITY* pE) ; // get outer loop
IGES_API void tsurSetOuter(IENTITY* pE, int outer) ; // get outer loop
IGES_API int tsurGetInner(IENTITY* pE, int i) ; // get inner loop
IGES_API void tsurSetInner(IENTITY* pE, int i, int inner) ; // get outer loop
//--------------------------------------------------------------
// solid : cylinder
IGES_API double cylinderGetH(IENTITY* pE) ;
IGES_API void cylinderSetH(IENTITY* pE, double h) ;
IGES_API double cylinderGetR(IENTITY* pE) ;
IGES_API void cylinderSetR(IENTITY* pE, double r) ;
IGES_API void cylinderGetCenter(IENTITY* pE, double center[3]) ;
IGES_API void cylinderSetCenter(IENTITY* pE, double center[3]) ;
IGES_API void cylinderGetAxis(IENTITY* pE, double axis[3]) ;
IGES_API void cylinderSetAxis(IENTITY* pE, double axis[3]) ;
//--------------------------------------------------------------
// solid : cone
IGES_API double coneGetH(IENTITY* pE) ;
IGES_API void coneSetH(IENTITY* pE, double h) ;
IGES_API double coneGetR1(IENTITY* pE) ;
IGES_API void coneSetR1(IENTITY* pE, double r1) ;
IGES_API double coneGetR2(IENTITY* pE) ;
IGES_API void coneSetR2(IENTITY* pE, double r2) ;
IGES_API void coneGetCenter(IENTITY* pE, double center[3]) ;
IGES_API void coneSetCenter(IENTITY* pE, double center[3]) ;
IGES_API void coneGetAxis(IENTITY* pE, double axis[3]) ;
IGES_API void coneSetAxis(IENTITY* pE, double axis[3]) ;
//--------------------------------------------------------------
// solid : sphere
IGES_API double sphereGetR(IENTITY* pE) ;
IGES_API void sphereSetR(IENTITY* pE, double r) ;
IGES_API void sphereGetCenter(IENTITY* pE, double center[3]) ;
IGES_API void sphereSetCenter(IENTITY* pE, double center[3]) ;
//--------------------------------------------------------------
// solid : torus
IGES_API double torusGetR1(IENTITY* pE) ;
IGES_API void torusSetR1(IENTITY* pE, double r1) ;
IGES_API double torusGetR2(IENTITY* pE) ;
IGES_API void torusSetR2(IENTITY* pE, double r2) ;
IGES_API void torusGetCenter(IENTITY* pE, double center[3]) ;
IGES_API void torusSetCenter(IENTITY* pE, double center[3]) ;
IGES_API void torusGetAxis(IENTITY* pE, double axis[3]) ;
IGES_API void torusSetAxis(IENTITY* pE, double axis[3]) ;
//==============================================================
typedef void IGES ;
IGES_API IGES* igesCreate() ;
IGES_API void igesFree(IGES* iges) ;
IGES_API void igesGetStart(IGES* iges, char* str, int size) ;
IGES_API void igesSetStart(IGES* iges, char* str) ;
IGES_API void igesGetGlobal(IGES* iges, GLOBALSEC* pGlobal) ;
IGES_API void igesSetGlobal(IGES* iges, GLOBALSEC* pGlobal) ;
IGES_API void igesAdd(IGES* iges, IENTITY *pE) ;
IGES_API int igesGetNumOfEntity(IGES* iges) ;
IGES_API IENTITY* igesGetEntity(IGES* iges, int i) ;
IGES_API int igesImport(IGES* iges, TCHAR* pathName) ; // nt add 2016/6/2
IGES_API int igesExport(IGES* iges, TCHAR* pathName) ; // nt add 2016/6/2
IGES_API void igesInitInParam(IGES* iges) ; // nt add 2021/3/23
IGES_API int iges2MM(IGES* iges) ; // nt add 2011/11/15
IGES_API int iges2M(IGES* iges) ; // nt add 2011/11/15
////////////////////////////////////////////////////////////////

#endif