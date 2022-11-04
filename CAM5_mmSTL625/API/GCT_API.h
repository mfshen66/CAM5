/*	2004/12/9 nt
 *	GCODE API
 *	nt modify 2005/4/26
 */
#ifndef _GCT_API_h_
#define _GCT_API_h_

#ifdef GCT_EXPORTS
#define GCT_API __declspec(dllexport)
#else
#define GCT_API __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////
/*typedef struct _pick_info PINFO ;
struct _pick_info // 拾取信息
{
    int type ; // entity type
    float pos[3] ; // mouse position(relative to model coordinate frame)
    float dir[3] ; // view dir(from eye to object)
    float r ; // picking radius

	int	ot ; // object type
    void* obj ; // picked object
    float u, v ; // u, v parameter
	float z ; // depth from pos to entity, (-)--->(+),dir:--->
} ;*/
//--------------------------------------------------------------
/*	2005/5/11 nt
 *	tool type
 *	nt modify 2005/5/28
 */
enum EToolType
{
	ttUnknown = -1,
	ttCone = 0,
	ttBullNose = 1,
	ttFlatEnd = 2,
	ttBallEnd = 3,
	ttCBEnd = 6,
	ttCBNose = 7,
	ttTorus = 8, // 环形刀
	ttDrum = 9 // 鼓形刀
} ;

typedef struct _milltool MTOOL ;
struct _milltool
{
	int type ;
	float d1 ;
	float d2 ;
	float d3 ;
	float d4 ;
	int no ; // 编号
} ;
//--------------------------------------------------------------
//	define gcode running state
#define RS_G00 0x1
#define RS_G01 0x2
#define RS_G02 0x4
#define RS_G03 0x8
#define RS_G90 0x10
#define RS_G91 0x20
#define RS_G17 0x40
#define RS_G18 0x80
#define RS_G19 0x100
#define	RS_G53 0x200
#define RS_G54 0x400
#define RS_G55 0x800
#define RS_G56 0x1000
#define RS_G57 0x2000
#define RS_G58 0x4000
#define RS_G59 0x8000
#define RS_GFT 0x10000 // fall to
#define RS_GNT 0x20000 // near to
#define RS_GLF 0x40000 // lift
#define RS_HLF 0x80000 // half, nt add 2006/1/22, ds has multiply 0.5f
#define RS_SUB 0x100000 // nt add 2006/2/21

/*	2004/11/13 nt
 *	TNODE
 *	nt modify 2005/3/11
 *	nt modify 2006/2/21
 */
typedef struct _tnode TNODE ;
struct _tnode // toolpath node
{
	int rs ; // running state
	float p[3] ;
	float F ; // feeadrate, nt add 2005/5/3, unit : mm/min
	float S ; // spindle speed, nt add 2005/5/3, unit : RPM
	int bi ;
	float d ; // length from begin to end, nt add 2006/1/17
	float a ; // a at end of this node, nt add 2006/1/17
	float t ; // nt modify 2006/2/21
} ;
//--------------------------------------------------------------
/*	2005/3/18 nt
 *	ANODE type
 */
enum EANodeType
{
	anPause = 0, // pause
	anDelay = 1, // value = time(msecond)
	anFeedrate = 2, // value = speed, mm/min
	anSpindle = 3, // value < 0(acw)(M04), value = 0(stop)(M05), value > 0(cw)(M03)
	anTool = 4, // value = tool no.
	anChnTool = 5, // change tool
	anCool = 6, // value = 0(off), 1(on)
	anEnd = 7, // program end
	anSSpeed = 8 // spindle speed, nt add 2005/8/30, 对应S指令
} ;

/*	2005/3/18 nt
 *	ANODE
 */
typedef struct _anode ANODE ;
struct _anode // auxilary node
{
	int type ;
	int value ;
	int bi ;
} ;
//--------------------------------------------------------------
typedef void TSEGMENT ;
GCT_API int tsegmentDraw(TSEGMENT* tsegment, void* pVI, int drawMode) ;
GCT_API int tsegmentGetMinMaxBIs(TSEGMENT* tsegment, int bis[2]) ; // nt add 2005/8/18
//--------------------------------------------------------------
typedef void GCODETPATH ;
GCT_API GCODETPATH* gcodetpathCreate() ;
GCT_API int gcodetpathFree(GCODETPATH* gcodetpath) ;
GCT_API int gcodetpathSetGCodeFileName(GCODETPATH* gcodetpath, TCHAR* pathName) ;
GCT_API int gcodetpathSetCutterPos(GCODETPATH* gcodetpath, float pos[3]) ; // nt add 2005/7/25
GCT_API int gcodetpathSetCWCS(GCODETPATH* gcodetpath, int cwcs) ; // nt add 2005/4/26
GCT_API int gcodetpathSetWCS(GCODETPATH* gcodetpath, int i, float wcs[3]) ; // nt modify 2005/4/26
GCT_API int gcodetpathSetDiscPrecision(GCODETPATH* gcodetpath, float dsc) ;
GCT_API int gcodetpathSetNearWorkpieceDis(GCODETPATH* gcodetpath, float znr) ;
GCT_API int gcodetpathSetSafeHeight(GCODETPATH* gcodetpath, float sh) ; // nt add 2005/5/11
GCT_API int gcodetpathSetDelayTime(GCODETPATH* gcodetpath, float dt) ; // nt add 2005/5/11
GCT_API int gcodetpathSetSpindleDelay(GCODETPATH* gcodetpath, float sdelay) ; // nt add 2006/4/21
GCT_API int gcodetpathSetDrillD(GCODETPATH* gcodetpath, float d) ; // nt add 2005/6/28
GCT_API int gcodetpathGetNumOfTool(GCODETPATH* gcodetpath) ; // nt add 2005/5/11
GCT_API int gcodetpathGetTool(GCODETPATH* gcodetpath, int iTool, MTOOL* pTool) ; // nt add 2005/5/11
GCT_API int gcodetpathSetScope(GCODETPATH* gcodetpath, float min[3], float max[3]) ;
GCT_API int gcodetpathMakeTPath(GCODETPATH* gcodetpath) ;
GCT_API int gcodetpathGetG5xs(GCODETPATH* gcodetpath, int g5xs[6]) ; // nt add 2005/9/6
GCT_API int gcodetpathGetTPathBox(GCODETPATH* gcodetpath, float min[3], float max[3]) ;
GCT_API int gcodetpathGetNumOfBlock(GCODETPATH* gcodetpath) ; // nt add 2005/5/17
GCT_API int gcodetpathGetCode(GCODETPATH* gcodetpath, int bi, char code[256]) ;
GCT_API int gcodetpathGetPos(GCODETPATH* gcodetpath, int bi, float p[3]) ; // nt add 2005/5/31
GCT_API int gcodetpathGetNumOfTNode(GCODETPATH* gcodetpath) ; // nt add 2010/3/28
GCT_API int gcodetpathGetTNode(GCODETPATH* gcodetpath, int i, TNODE* tnode) ; // nt add 2010/3/28
GCT_API int gcodetpathGetNumOfANode(GCODETPATH* gcodetpath) ; // nt add 2010/3/28
GCT_API int gcodetpathGetANode(GCODETPATH* gcodetpath, int i, ANODE* anode) ; // nt add 2010/3/28
GCT_API int gcodetpathChkFirstXYZ(GCODETPATH* gcodetpath, int XYZ[3]) ; // nt add 2005/8/31
GCT_API int gcodetpathChkLimits(GCODETPATH* gcodetpath) ;
GCT_API int gcodetpathMove(GCODETPATH* gcodetpath, float v[3]) ;
GCT_API int gcodetpathMoveZ(GCODETPATH* gcodetpath, float dz) ; // nt add 2005/8/14
GCT_API int gcodetpathScale(GCODETPATH* gcodetpath, float scale) ; // nt add 2010/4/1
GCT_API int gcodetpathDraw(GCODETPATH* gcodetpath, void* pVI, int drawMode) ;
GCT_API int gcodetpathPick(GCODETPATH* gcodetpath, PINFO* pi) ;
GCT_API int gcodetpathDump(GCODETPATH* gcodetpath, TCHAR* fname) ; // nt add 2005/6/5
GCT_API int gcodetpathGetEInfo(GCODETPATH* gcodetpath, char* eInfo) ; // nt add 2005/9/14
GCT_API int gcodetpathSetMaxG00(GCODETPATH* gcodetpath, float maxG00) ; // nt add 2006/1/17
GCT_API int gcodetpathSetMaxG01(GCODETPATH* gcodetpath, float maxG01) ; // nt add 2006/1/17
////////////////////////////////////////////////////////////////

#endif