/*	2000/7/18 nt
 *	header file of class CCommand
 */
#ifndef _Command_h_
#define _Command_h_

class CCAMDoc ;
class CCAMView ;

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
//	1999/4/5 nt
//	CCommand
// nt add 2009/9/4
// 在本系统暂时未用到
enum ENaviType
{
	nvFreePt = 0,
	nvLinEnd = 1,
	nvCirPt = 2,
	nvArc3P = 3
} ;

class CCommand
{
protected:
	CCAMDoc* m_pDoc ;
	CCAMView* m_pView ;
	RFRAME m_lf ; // nt add 2010/9/13
	void* m_pVI ; // nt add 2009/9/4
	int	m_mouse ; // 0 up, 1 down
	int	m_drawed ;
	int	m_color ; // RGB
	int m_ctol ; // nt add 2010/9/13, move here from CAD2D

	// nt add 2009/9/4, for navigation
	PNT3D m_raw_p ;
	PNT3D m_begin ;
	PNT3D m_center ;
	PNT3D m_p1 ;
	PNT3D m_p2 ;
	PNT3D m_nav_p ;

public:
	CCommand() ;
	virtual ~CCommand() ;

	int Init(CCAMDoc* pDoc, CCAMView* pView) ;
	void GetLF(RFRAME* pLF) ; // nt add 2010/9/13, get current workplane
	// p和worldPt都是世界坐标系的点
	int	ToWP(PNT3D p, VEC3D v, PNT3D worldPt) ; // nt add 2004/8/18
	int ToWP2D(PNT3D p, VEC3D v, PNT2D wpPt) ; // nt add 2010/9/14
	int ToPlane(PNT3D worldPt, PNT2D planePt) ; // nt add 2009/12/17
	int ToWorld(PNT2D planePt, PNT3D worldPt) ; // nt add 2009/12/17
	int GetColor() ; // nt add 2009/12/17
	int SetColor(int color) ; // nt add 2009/12/17
	// nt modify 2004/7/11--------------------------------------
	int SetBegin(PNT3D begin) ;
	int SetCenter(PNT3D center) ;
	int SetP1(PNT3D p1) ;
	int SetP2(PNT3D p2) ;
	int	Navigate(int type, UINT nFlags, PNT3D raw_p, VEC3D v) ;
	int GetNavP(PNT3D nav_p) ;
	int GetNavP2D(PNT2D nav_p) ; // nt add 2010/9/14
	//----------------------------------------------------------
	int GetCTol() ;
	int SetCTol(int ctol) ;
	int PickLin(PNT3D p, PNT3D begin, VEC3D dir) ;
	int PickCir(PNT3D p, PNT3D center, double& r) ;
	//----------------------------------------------------------
	int PickLin2D(PNT2D p, PNT2D begin, VEC2D dir) ; // nt add 2010/9/14
	int PickCir2D(PNT2D p, PNT2D center, double& r) ; // nt add 2010/9/14
	//----------------------------------------------------------
	void DrawLin(PNT3D p1, PNT3D p2) ;
	void DrawRect(PNT3D p1, PNT3D p2) ;
	void DrawArc(RFRAME* pLF, double r, double a) ;
	//----------------------------------------------------------
	void DrawLin2D(PNT2D p1, PNT2D p2) ; // nt add 2010/9/14
	void DrawRect2D(PNT2D p1, PNT2D p2) ; // nt add 2010/9/14
	void DrawArc2D(PNT2D center, double r, double a1, double a2) ; // nt add 2010/9/14
	//----------------------------------------------------------

	virtual int	GetID() = 0 ;
	virtual int OnLButtonDown(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnLButtonUp(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnMouseMove(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnRButtonDown(UINT nFlags, double p[3], double v[3]) = 0 ;
	virtual int OnReturn() = 0 ; // nt add 2009/10/21
	virtual int Cancel() = 0 ;
} ;

/*	2002/9 nt
 *	doc command type
 */
enum EDocCmdType
{
	// draw
	dctDrawPt = 0,
	dctDrawLin = 1,
	dctDrawRect = 2,
	dctDrawCir = 3,
	dctDrawArc = 4,
	dctDrawPPR = 5,
	dctDrawPPA = 6,
	dctDynSpln = 7,
	dctScribble = 8,
	dctDrawImage = 9,
	dctDrawFillet = 10,
	dctDrawChamfer = 11,
	dctDrawBC = 12,
	dctDrawDim = 100,
	dctDrawNote = 101,
	dctDrawText = 102,
	dctDrawTable = 103,
	dctTrim = 200,
	dctOffset = 201, // nt add 2006/1/11
	dctTrimCur = 202, // nt add 2009/12/16
	dctModSurCPt = 203, // nt add 2009/12/16
	dctMove = 300,
	dctRotate = 301,
	dctPaste = 304,
	dctScale = 305,
	// drag
	dctDragImage = 1000,
	dctDragDim = 1001,
	dctDragLin = 1002,
	dctDragCir = 1003,
	dctDragArc = 1004,
	dctDragNote = 1005,
	dctDragPt = 1006,
	dctDragCur = 1007,
	dctDragCell = 1008
} ;

/*	2009/12/1 nt
 *	view command type
 */
enum EViewCmdType
{
	vctPan = 0,
	vctZoomBox = 1,
	vctZoom = 2,
	vctRotate = 3
} ;
///////////////////////////////////////////////////////////////

#endif
