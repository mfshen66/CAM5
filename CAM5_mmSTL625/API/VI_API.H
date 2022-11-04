/*	2000/5/5 nt
 *	VI DLL API
 */
#ifndef _VI_API_h_
#define _VI_API_h_

#ifdef VI_EXPORTS
#define VI_API __declspec(dllexport)
#else
#define VI_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" 
{
#endif
////////////////////////////////////////////////////////////////
//	(直角)坐标系
typedef struct _cs2d CS2D ; // coordinate system 2d
struct _cs2d
{
	double X[2] ;
	double Y[2] ;
	double O[2] ;
	double scale ;
} ;

VI_API int cs2dInit(CS2D* pCS) ;

typedef struct _cs CS ; // coordinate system 3d
struct _cs
{
	double X[3] ;
	double Y[3] ;
	double Z[3] ;
	double O[3] ;
	double scale ;
} ;
VI_API int csInit(CS* pCS) ;
//==============================================================
// 材质
typedef struct _material MATERIAL ;
struct _material
{
	char name[24] ;
	float ambient[4] ; // for RGBA
	float diffuse[4] ; // for RGBA
	float specular[4] ; // for RGBA
	float shiness ; // from 0 to 128
	int ff ; // front_face, 0(cw), 1(ccw), see glFrontFace()
} ;

VI_API int materialGet(MATERIAL* pM, char* name) ;
VI_API int materialGet2(MATERIAL* pM, int i) ; // nt add 2009/12/21
VI_API void materialSet(MATERIAL* pM) ; // nt add 2009/12/21
//==============================================================
// 光源
typedef struct _light LIGHT ;
struct _light
{
	int no ; // from 0 to 7
	int enable ; // 1:enable, 0:disable
	int type ; // 暂时未用,0:点光源,1:线光源,2:面光源
	float ambient[4] ; // for RGBA
	float diffuse[4] ; // for RGBA
	float specular[4] ; // for RGBA
	float position[4] ; //
} ;

VI_API int lightInit(LIGHT* pL, int no) ;
//==============================================================
// nt add 2010/8/26
typedef struct _vinfo VINFO ;
struct _vinfo
{
	int m ;
	int w ;
	int h ;
	double d ;
	double e[3] ;
	double c[3] ;
	double u[3] ;
	double n ;
	double f ;
	int b ;
	int color ;
} ;
//==============================================================
/*	1999/4/5 nt
 *	EDrawMode
 */
enum EDrawMode
{
	dmWireframe = 0,
	dmHiddenLineInGray = 1,
	dmHiddenLine = 2,
	dmShaded = 3,
	dmNormal = 4,
	dmPrompt = 5,
	dmSelect = 6,
	dmEraze = 7,
	dmActive = 8,
	dmInvalid = 9,
	dmRecover = 10,
	dmPrint = 11 // nt add 2010/3/10
} ;
//==============================================================
/*	2000/5/6 nt
 *	EViewport
 */
enum EViewport
{
	vpXY = 0, // from positive Z to negative Z
	vpYZ = 1, // from positive X to negative X
	vpZX = 2, // from positive Y to negative Y
	vpI2 = 3,
	vpXY_ = 4, // from negative Z to positive Z
	vpYZ_ = 5, // from negative X to positive X
	vpZX_ = 6, // from negative Y to positive Y
	vpI2_ = 7
} ;
//==============================================================
//	CVI
VI_API void* viCreate() ;
VI_API void viFree(void* pVI) ;
VI_API int viSetWnd(void* pVI, void* pWnd) ;
VI_API int viSetDC(void* pVI, HDC hDC, int bDepthBuf) ; // nt add 2010/3/9
VI_API int viSetPixelSize(void* pVI, double d) ;
VI_API int viSetProjectMode(void* pVI, int projectMode) ;
VI_API int viSetInitZ(void* pVI, double z) ;
VI_API int viSetSize(void* pVI, int cx, int cy) ;
VI_API int viSetFovy(void* pVI, double fovy) ;
//--------------------------------------------------------------
VI_API void* viCreCVI(int projectMode, double d, double z, void* pWnd) ;
VI_API void viFreCVI(void* pVI) ;
// set device context
VI_API void viWake(void* pVI) ;
VI_API void viSleep(void* pVI) ;
VI_API int viIsWake(void* pVI) ;
VI_API void viSetGL(void* pVI) ;
VI_API void* viGetWnd(void* pVI) ; // nt add 2002/9/10
VI_API void* viGetDC(void* pVI) ;// nt add 2003/7/10 ;

VI_API int viOnQueryNewPalette(void* pVI, BOOL* pNRet) ;
VI_API void viOnPaletteChanged(void* pVI) ;
VI_API int viOnSize(void* pVI, int cx, int cy) ;
// set and get viewport attributes
VI_API void viGetWinWidthHeight(void* pVI, int* pWidth, int* pHeight) ;
VI_API void viGetViewWidthHeight(void* pVI, double* pWidth, double* pHeight) ;
VI_API double viGetFovy(void* pVI) ;
VI_API void viGetEyeCenterUp(void* pVI, double eye[3], double center[3], double up[3]) ;
VI_API void viSetEyeCenterUp(void* pVI, double eye[3], double center[3], double up[3]) ;
VI_API void viGetNearFar(void* pVI, double* pN, double* pF) ;
VI_API void viSetNearFar(void* pVI, double n, double f) ;
VI_API COLORREF viGetBGColor(void* pVI) ;
VI_API void viSetBGColor(void* pVI, COLORREF bgColor) ;
VI_API COLORREF viGetCurColor(void* pVI) ;
VI_API void viSetCurColor(void* pVI, COLORREF curColor) ;

VI_API void viGetCenterLF(void* pVI, double xaxis[3], double yaxis[3], double zaxis[3], double origin[3]) ;
VI_API void viSetCenterLF(void* pVI, double xaxis[3], double yaxis[3], double origin[3]) ;
VI_API double viGetScale(void* pVI) ;
VI_API double viGetScope(void* pVI) ;
VI_API void viGetViewDir(void* pVI, double vd[3]) ;
VI_API void viGetViewDirThrPt(void* pVI, double p[3], double vd[3]) ;
VI_API void viGetBoxOfFrustum(void* pVI, double min[3], double max[3]) ;
VI_API void viGetCorners(void* pVI, double v1[3], double v2[3], double v3[3], double v4[3]) ;

VI_API void viClear(void* pVI) ;
VI_API void viClear2(void* pVI) ; // nt add 2005/12/6
VI_API void viClearEx(void* pVI) ;
VI_API void viClearWhite(void* pVI) ;
VI_API void viClearBlack(void* pVI) ;
VI_API void viSetViewport(void* pVI, int type) ;

// pan, zoom and rotate camera
VI_API void viPan(void* pVI, double vec[3]) ;
VI_API int viZoomByBox(void* pVI, double p0[3], double p1[3]) ;
VI_API void viZoom(void* pVI, double zm_scale) ;
VI_API int viZoomByMouse(void* pVI, double zm_scale, double fixed_p[3]) ;
VI_API int viCalRotateAxis(void* pVI, double p[3], double raxis[3]) ;
VI_API void viRotate(void* pVI, double raxis[3], double angle, double p[3]) ;

// to screen
VI_API void viGetScreenCoord(void* pVI, double p[3], int* pX, int* pY) ;
VI_API void viGetScreenCoord2D(void* pVI, double p[2], int* pX, int* pY) ; // nt add 2002/9/10
// to world
VI_API void viGetWorldPointViewD(void* pVI, int wx, int wy, double p[3], double vd[3]) ;
VI_API void viGetWorldPt2D(void* pVI, int wx, int wy, double p[2]) ; // nt add 2009/9/25
VI_API void viGetWorldPt(void* pVI, int wx, int wy, double p[3]) ; // nt add 2009/9/25

// light
VI_API int viGetNumOfLight(void* pVI) ;
VI_API void viLighting(void* pVI, int enable) ;
VI_API void viSetLight(void* pVI, LIGHT* pL) ;
VI_API void viAddDefaultLight(void* pVI) ; // nt add 2004/8/20
VI_API void viAddTexture(void* pVI, int transparent, int w, int h, void* bits) ;
VI_API void viDelTexture(void* pVI, int transparent) ;

// draw
VI_API void viDrawVector(void* pVI, double b[3], double v[3], int nPixel, COLORREF c) ;
VI_API void viDrawVector2(void* pVI, double b[3], double v[3], int nPixel) ;
VI_API void viDrawVectorNM(void* pVI, double b[3], double uniV[3], int nPixel, COLORREF c) ;
VI_API void viDrawVectorNM2(void* pVI, double b[3], double uniV[3], int nPixel) ;
VI_API void viDrawVectorBE(void* pVI, double b[3], double e[3], int nPixel, COLORREF c) ;
VI_API void viDrawWorldFrame(void* pVI, COLORREF c) ;
VI_API void viDrawWorldFrame2(void* pVI) ;
VI_API void viDrawLocalFrame(void* pVI, double origin[3], double e1[3], double e2[3], COLORREF c) ;
VI_API void viDrawLocalFrame2(void* pVI, double origin[3], double e1[3], double e2[3]) ;
VI_API void viDrawWorldFrameEx(void* pVI, double d, COLORREF c) ;
VI_API void viDrawWorldFrameSmooth(void* pVI, double d, COLORREF c) ;
VI_API void viDrawCross(void* pVI, double p[3], int nPixel, COLORREF c) ;
VI_API void viDrawCross2(void* pVI, double p[3], int nPixel) ;
VI_API int viDrawWorkPlaneGrid(void* pVI, double wpO[3], double wpX[3], double wpY[3], double step, int flag, COLORREF color, double size) ;
VI_API int viDrawWorkPlaneGridEx(void* pVI, double step, int flag, COLORREF color, double size) ;

VI_API void viSerialize(void* pVI, CArchive& ar) ; // nt add 1997/8/17
VI_API void viLoad(void* pVI, VINFO* vinfo) ; // nt add 2010/8/26
VI_API void viSave(void* pVI, VINFO* vinfo) ; // nt add 2010/8/26

VI_API void viSetPrintInfo(void* pVI, double paper_w, double paper_h, int print_w, int print_h) ;
VI_API void viGetPrintCoord(void* pVI, double p[2], int* px, int* py) ;
//==============================================================
//	CPyramid
VI_API void* pyCreCPyramid(double apex[3], double v1[3], double v2[3], double v3[3], double v4[3]) ;
VI_API void pyFreCPyramid(void* pPyramid) ;
VI_API int pyChkBoxInt(void* pPyramid, double min[3], double max[3], double tol) ;
//==============================================================
//	Draw
// used in viDrawArrow2D()
/*	1999/9/1 nt
 *	EArrowShapeType
 */
enum EDimArrowShapeType
{
	asHollowTri = 0, // <|------|>, 空心箭头
	asFilledTri	= 1, // <*------*>, 实心箭头
	asSplash = 2, // \------\, 斜线
	asFilledDot	= 3, // o------o, 实心圆圈
	asHollowDot	= 4, // *------*, 空心圆圈
	asShortFork	= 5 // <------>, // 简化箭头
} ;
VI_API int viDrawPoint(double p[3], int color, double size) ;
VI_API int viDrawPt2D(double p[2], int color, double size) ;
VI_API int viDrawLine(double p1[3], double p2[3], int color, double width) ;
VI_API int viDrawLin2D(double p1[2], double p2[2], int color, double width) ;
VI_API int viDrawRect2D(double p1[2], double p2[2]) ; // nt add 2009/8/25
VI_API int viDrawArc2D(double center[2], double start[2], double angle, double tol) ; // nt add 2009/8/25
VI_API int viDrawArrow2D(int type, double p1[2], double p2[2], double len) ; // nt add 2009/8/25
VI_API int viDrawPolyline(int n, double (*ps)[3], int color, double width) ;
VI_API int viDrawPoly2D(int n, double (*ps)[2], int color, double width) ;
VI_API int viDrawTris(int np, double (*ps)[3], double (*ns)[3], int skipsize, int nt, int (*ts)[3], int color) ;
VI_API int viDrawQuas(int np, double (*ps)[3], double (*ns)[3], int skipsize, int nq, int (*qs)[4], int color) ;
VI_API int viDrawBox(double min[3], double max[3], int c) ;
VI_API void viDrawString(double p[3], char* text) ;
//==============================================================
//	Math
VI_API double viDist(double p1[3], double p2[3]) ;
VI_API float viDist2F(float p1[2], float p2[2]) ;
VI_API double viVecLen(double v[3]) ;
VI_API double viDProduct(double v1[3], double v2[3]) ;
VI_API void viVProduct(double v1[3], double v2[3], double v[3]) ;
VI_API int viUniVec(double vec[3], double min_len) ;
VI_API int viGetDirP1P2P(double p1[3], double p2[3], double p[3], double tol, double v[3]) ;
VI_API void viPrjPntLin(double p[3], double b[3], double dir[3], double rtpnt[3] ) ;
VI_API void viPrjPntPln(double p[3], double pivot[3], double normal[3], double rtpnt[3] ) ;
VI_API void viPrjVecPln(double vector[3], double normal[3], double rtvec[3]) ;
VI_API void viPrjVecVec(double prjvec[3], double univec[3], double rtvec[3]) ;
VI_API int viGetNormalP3(double p1[3], double p2[3], double p3[3], double pp[3], double tol, double n[3]) ;
VI_API void viRotatePnt(double dir[3], double pivot[3], double angle, double point[3], double rtpnt[3]) ;
VI_API void viRotateVec(double dir[3], double pivot[3], double angle, double vector[3], double rtvec[3]) ;
VI_API int viIntLinePlan(double begin[3], double dir[3], double pivot[3], double normal[3], double minAngle, double minSpace, double p[3]) ;
VI_API int viGetWorkPlanePoint(double projPoint[3], double viewDir[3], double localFrame[13], double localPoint[2], double worldPoint[3]) ;
VI_API double viDistPntPlan(double point[3], double pivot[3], double normal[3]) ;
VI_API int viChkBoxPlanInt(double min[3], double max[3], double pivot[3], double normal[3], double tol) ;
VI_API void viClamp(double t1, double t2, double& t) ;
VI_API void viGetBoxOfSegm(double p1[3], double p2[3], double min[3], double max[3]) ;
VI_API void viGetBoxOfTria(double p1[3], double p2[3], double p3[3], double min[3], double max[3]) ;
VI_API int viGetVecMinComSubX(int count, double* v) ;
VI_API int viGetAxisXYFromAxisZX(double e3[3], double e1[3], double e2[3]) ;
VI_API int viChkHalfSpaceBoxInt(double p[3], double n[3], double min[3], double max[3], double tol) ;
//==============================================================
//	Texture
VI_API void* viLoadDIBitmap(TCHAR *filename, BITMAPINFO **info) ;
VI_API int viSaveDIBitmap(TCHAR *filename, BITMAPINFO *info, void *bits) ;
VI_API void* viReadDIBitmap(BITMAPINFO **info) ;
VI_API int viPrintDIBitmap(HWND owner, BITMAPINFO *info, void *bits) ;
//==============================================================
/*	2004/4/14 nt
 *	Text.cpp
 */
VI_API int viTextOut1(double pos[3], TCHAR* ttext, TCHAR* fontName, double c[3], double s) ;
VI_API int viTextOut2(double O[3], double X[3], double Y[3], double Z[3], TCHAR* ttext, TCHAR* fontName, double c[3], double s) ;
VI_API int viTextOut3(double pos[3], double dir[3], int align, void* pVI, TCHAR* ttext, TCHAR* fontName, double c[3], double s) ;
//==============================================================
//	TRUETYPE字体
/*	2004/9/14 nt
 *	Font.cpp
 */
VI_API void* glfCreate(int format, char* fontName) ;
VI_API int glfFree(void* glf) ;
VI_API int glfLoadSyms(void* glf, char* pathName) ; // nt add 2020/5/9
VI_API int glfTextOut2D(void* glf, double pos[3], char* text, double h) ;
VI_API int glfTextOut2DALF(void* glf, double pos[2], char* text, double h, double ls, double L) ;
VI_API int glfTextOut2DVer(void* glf, double pos[2], char* text, double h, double ls) ;
VI_API int glfTextOut2DVerALF(void* glf, double pos[2], char* text, double h, double ls, double L) ;
VI_API int glfTextOut2DEx(void* glf, double O[2], double X[2], char* text, double h, double ls, double L, int ver) ;
VI_API int glfTextOut3D(void* glf, CS* cs, char* text, double h) ;
VI_API int glfTextOut3DALF(void* glf, CS* cs, char* text, double h, double ls, double L) ;
VI_API int glfTextOut3DEx(void* glf, CS* cs, char* text, double h, double viewX[3], double viewY[3], int box) ;
//--------------------------------------------------------------
VI_API void viSetGLF(void* pVI, void* glf) ;
VI_API int viTextOut2D(void* pVI, double pos[3], char* text, double h) ;
VI_API int viTextOut2DALF(void* pVI, double pos[2], char* text, double h, double ls, double L) ;
VI_API int viTextOut2DVer(void* pVI, double pos[2], char* text, double h, double ls) ;
VI_API int viTextOut2DVerALF(void* pVI, double pos[2], char* text, double h, double ls, double L) ;
VI_API int viTextOut2DEx(void* pVI, double O[2], double X[2], char* text, double h, double ls, double L, int ver) ;
VI_API int viTextOut3D(void* pVI, CS* cs, char* text, double h) ;
VI_API int viTextOut3DALF(void* pVI, CS* cs, char* text, double h, double ls, double L) ;
VI_API int viTextOut3DEx(void* pVI, CS* cs, char* text, double h, double viewX[3], double viewY[3], int box) ;
//==============================================================
//	矢量字体库
/*	2009/8 nt
 *	CL.cpp
 */
VI_API void* gclCreate() ;
VI_API int gclFree(void* pGCL) ;
VI_API int gclLoadTxt(void* pGCL, char* pathName) ;
VI_API int gclLoadHZ(void* pGCL, char* pathName) ;
VI_API int gclLoadSLib(void* pGCL, char* pathName) ; // nt add 2009/8/25
VI_API int gclGetNumOfSLib(void* pGCL) ; // nt add 2009/8/25
VI_API void* gclGetSLib(void* pGCL, int i) ; // nt add 2009/8/25
VI_API int gclBuildIndice(void* pGCL) ;
VI_API int gclOut(void* pGCL, char* str) ;
VI_API int gclOutVertical(void* pGCL, char* str) ; // nt add 2019/8/5
VI_API int gclOut2(void* pGCL, char* str, CDC* pDC, int x0, int y0, int h) ; // nt add 2009/9/10
VI_API int gclDraw2D(void* pGCL, char* str, CS2D* pCS, double h) ;
VI_API int gclDrawP2(void* pGCL, char* str, double p[2], double h) ; // nt add 2009/8/25
VI_API int gclDrawP2Vertical(void* pGCL, char* str, double p[2], double h) ; // nt add 2019/8/5
VI_API int gclDraw(void* pGCL, char* str, CS* pCS, double h) ;
VI_API int gclOutML(void* pGCL, char* str, double ls) ; // nt add 2011/10/24
VI_API int gclOut2ML(void* pGCL, char* str, CDC* pDC, int x0, int y0, int h, int ls) ;  // nt add 2011/10/24
VI_API int gclDraw2DML(void* pGCL, char* str, CS2D* pCS, double h, double ls) ; // nt add 2011/10/24
VI_API int gclDrawP2ML(void* pGCL, char* str, double p[2], double h, double ls) ;  // nt add 2011/10/24
VI_API int gclDrawML(void* pGCL, char* str, CS* pCS, double h, double ls) ; // nt add 2011/10/24
VI_API int gclOutAL(void* pGCL, char* str, double ls, double L) ; // nt add 2011/10/30
VI_API int gclOut2AL(void* pGCL, char* str, CDC* pDC, int x0, int y0, int h, int ls, int L) ; // nt add 2011/10/30
VI_API int gclDxfOut2D(void* pGCL, char* str, CS2D* pCS, double h, int color, FILE* fp) ;
VI_API int gclDraw2DAL(void* pGCL, char* str, CS2D* pCS, double h, double ls, double L) ; // nt add 2011/10/30
VI_API int gclDrawP2AL(void* pGCL, char* str, double p[2], double h, double ls, double L) ; // nt add 2011/10/30
VI_API int gclDrawAL(void* pGCL, char* str, CS* pCS, double h, double ls, double L) ; // nt add 2011/10/30
//--------------------------------------------------------------
VI_API int gclOutGetBox(char* str, double min[2], double max[2]) ; // nt add 2015/9/15
VI_API int gclOutVerticalGetBox(char* str, double min[2], double max[2]) ; // nt add 2019/8/5
VI_API int gclOutMLGetBox(char* str, double ls, double min[2], double max[2]) ; // nt add 2015/9/15
VI_API int gclOutALGetBox(char* str, double ls, double L, double min[2], double max[2]) ; // nt add 2015/9/15
VI_API int gclOut2GetBox(char* str, int x0, int y0, int h, int min[2], int max[2]) ; // nt add 2015/9/15
VI_API int gclOut2MLGetBox(char* str, int x0, int y0, int h, int ls, int min[2], int max[2]) ; // nt add 2015/9/15
VI_API int gclOut2ALGetBox(char* str, int x0, int y0, int h, int ls, int L, int min[2], int max[2]) ; // nt add 2015/9/15
VI_API int gclDrawP2GetBox(char* str, double p[2], double h, double min[2], double max[2]) ; // nt add 2015/9/15
VI_API int gclDrawP2VerticalGetBox(char* str, double p[2], double h, double min[2], double max[2]) ; // nt add 2019/8/5
VI_API int gclDrawP2MLGetBox(char* str, double p[2], double h, double ls, double min[2], double max[2]) ; // nt add 2015/9/15
VI_API int gclDrawP2ALGetBox(char* str, double p[2], double h, double ls, double L, double min[2], double max[2]) ; // nt add 2015/9/15
//==============================================================
//	RGB索引表
VI_API int rgbsCreate(char* pathName) ; // 根据文件创建RGB索引表, nt add 2009/9/3
VI_API int rgbsFree() ; // 释放RGB索引表
VI_API int rgbsGet(int i, unsigned char rgb[3]) ; // 根据索引号得到RGB值
VI_API int rgbsMatch(unsigned char rgb[3]) ; // RGB值转化为最佳匹配的索引号
///////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif