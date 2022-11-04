/*	2001/11/16 nt
 *	head file of class CRefSur definitions
 */
#ifndef _RefSur_h_
#define _RefSur_h_

////////////////////////////////////////////////////////////////
//	Features
/*	1999/8/24 nt
 *	ERefSurType
 */
enum ERefSurType
{
	rsExtrude, // extrude surface from sketch(with multiple contours, open or close)
	rsRevolve, // extrude surface from sketch(with multiple contours, open or close)
	rsSweep, // sweep surface from two sketches
	rsLoft, // loft surface from multiple sketches
	rsOffset, // from multiple connected faces
	rsImport, // from IGES file
	rsThrough, // through n x m point array, from data file
	rsFourBnd,
	rsThreeBnd
} ;
//--------------------------------------------------------------
class CMark ; // nt add 2011/4/20
//--------------------------------------------------------------
// nt add 2011/4/23
// the following info originally defined in CRefSur
// attType:
// 0=高斯曲率k1*k2
// 1=平均曲率(k1+k2)/2
// 2=k1*k1+k2*k2
// 3=min{k1,k2}
// 4=max{k1,k2}
// 5=sqrt(k1*k1+k2*k2)
// 6=ku
// 7=kv
typedef struct _isodef ISODEF ; // 用于定义等值面的一些必要的信息
struct _isodef
{
	int	attType ;
	double attMin ; // 如果attMin==attMax，则这两个值需要重新计算再使用
	double attMax ;
	double attTol ; // 如果=0，就用缺省的差
	int nZPlane ; // number of z cutting plane for attribute display
	double factor ; // 一个比例因子
} ;
//--------------------------------------------------------------
// nt add 2021/6/14
typedef struct _bndx BNDX ;
struct _bndx
{
	CURVE* curve ;
	INTERVAL interval ;
	BOX3D box ;
	STRPT* disc ;
	int nAdj ; // 属于几个邻面
	int flag ; // nt add 2021/8/13
} ;
//--------------------------------------------------------------
/*	1999/8/24 nt
 *	CRefSur
 */
class CRefSur : public CFeature
{
public:
	int	m_dtype ;
	SURFACE* m_surface ;
	BOXUV m_boxuv ;
	FACET* m_facet ;
	int	m_atts ; // &1=attribute display is on, &2=draw uv vector dirs
	int m_n ;
	BNDX* m_bnds ; // nt add 2021/6/14

	// nt add 2010/4/22
	double m_u ; // nt add 2010/4/22, 拾取点对应的参数u
	double m_v ; // nt add 2010/4/22, 拾取点对应的参数v
public:
	CRefSur() ;
	CRefSur(int type, BSSUR* nurbs) ;
	CRefSur(int type, SURFACE* surface, BOXUV* boxuv) ;
	CRefSur(SURFACE* surface, BOXUV* boxuv) ; // nt add 2021/6/25 专用
	CRefSur(TFACE* tface) ; // nt add 2010/5/19 专用
	CRefSur(TFACE* tface, double dTol, double dAng) ; // nt add 2010/5/19
	CRefSur(FACET* pFacet) ; // nt add 2010/10/21
	~CRefSur() ;

	CFeature* Copy() ; // nt add 2009/5/27
	int	GetType() ;
	int	GetDetailType() ;
	int	SetDetailType(int dtype) ; // nt add 2014/3/19
	SURFACE* GetSurface() ;
	BSSUR* GetNurbs() ;
	void GetBoxUV(BOXUV* boxuv) ;
	void SetBoxUV(BOXUV* boxuv) ; // nt add 2003/5/31
	int	CalBox() ;
	int Move(VEC3D v) ; // nt add 2009/5/27
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ; // nt add 2009/5/27
	int Mirror(PNT3D pivot, VEC3D normal) ; // nt add 2010/10/17
	int Scale(double scale) ; // nt add 2010/9/11
	int	Disc(double tol, double ang) ;
	FACET* GetFacet() ; // nt add 2010/5/20
	int	SetFacet(FACET* facet) ; // nt add 2003/10/30
	int GetNumOfBnd() ; // nt add 2014/3/19
	STRPT* GetBndDisc(int i) ; // nt add 2014/3/19
	CMark* CalKuKv(int nu, int nv, double factor) ; // nt add 2011/4/20
	CMark* CalIso(ISODEF* pIsoDef) ; // nt add 2011/4/23
	int RevsDir(int flag) ; // nt add 2010/5/23, 0=rev u, 1=vrev v, 2=u<->v
	int GetAtts() ; // nt add 2010/8/28
	void SetAtts(int visible) ; // 0,1:invisible, visible
	void SwapDrawUV() ; // nt add 2010/5/22
	double GetU() ; // nt add 2010/4/22
	double GetV() ; // nt add 2010/4/22
	int SetU(double u) ; // nt add 2010/4/22
	int SetV(double v) ; // nt add 2010/4/22
	void InitBndAdj() ; // nt add 2021/6/14
	int RefineFacet(double tol) ; // nt add 2021/6/19
	void OptTri(int nOpt) ; // nt add 2021/8/13
	int ReDisc() ; // nt add 2021/8/13

	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ; // nt add 2010/4/22
	int DrawSurface(void* pVI, int drawMode) ; // nt add 2011/7/3
	int DrawBorder(void* pVI, int drawMode) ; // nt add 2011/7/3
	int DrawContour(void* pVI, int drawMode) ; // nt add 2011/7/3
	int	Draw(void* pVI, int drawMode) ;
	int	Draw2(void* pVI, int drawMode) ; // nt add 2010/1/19
	int DrawUV(void* pVI, int drawMode) ; // nt add 2010/5/22
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif