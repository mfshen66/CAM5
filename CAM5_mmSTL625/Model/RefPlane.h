/*	2001/11/16 nt
 *	head file of feature definitions
 */
#ifndef _RefPlane_h_
#define _RefPlane_h_

////////////////////////////////////////////////////////////////
//	Features
//	refplane
/*	1999/8/23 nt
 *	ERefPlaneType
 */
enum ERefPlaneType
{
	rpXY = 0,
	rpYZ = 1,
	rpZX = 2,
	rpOffset = 3, // plane face or refplane offset
	rpThreePt = 4, // pass through 3 points
	rpNormal = 5// curve normal plane
} ;

/*	1999/8/23 nt
 *	CRefPlane
 */
class CRefPlane : public CFeature
{
private:
	int	m_dtype ;
	double m_halfSize ;
	RFRAME m_lf ; // local frame
public:
	CRefPlane() ;
	CRefPlane(int dtype, RFRAME* pLF) ;
	~CRefPlane() ;

	CFeature* Copy() ; // nt add 2009/5/27
	int	GetType() ;
	int	GetDetailType() ; // return EWorkplaneType
	double GetSize() ;
	void SetSize(double size) ;
	int Move(VEC3D v) ; // v is in world frame
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ; // nt add 2009/5/27
	int Mirror(PNT3D pivot, VEC3D normal) ; // nt add 2010/10/17
	int Scale(double scale) ; // nt add 2010/9/11

	void GetLF(RFRAME* pLF) ;
	int	Project(PNT3D oldPt, VEC3D prjDir, PNT2D prjPt) ; // project to this refplane
	int	ToPlane(PNT3D worldPt, PNT2D planePt) ;
	int	ToWorld(PNT2D planePt, PNT3D worldPt) ;
	// 0:no navigate;1:navigate grid node;2:navigate origin
	int	Navigate(DNAVI* dn, double gridSize) ; // nt add gridSize 2009/5/9
	int	CalBox() ;
	int	DrawGrid(void* pVI, int color, double step) ;

	int DrawBorder(void* pVI, int drawMode) ; // nt add 2011/7/6
	int DrawPlane(void* pVI, int drawMode) ; // nt add 2011/7/6
	int	Draw(void* pVI, int drawMode) ;
	int Draw2(void* pVI, int drawMode) ; // nt add 2010/1/19
	int DrawXYMark(void* pVI) ; // nt add 2010/11/27
	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ; // nt add 2010/4/22
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif