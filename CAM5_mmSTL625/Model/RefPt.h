/*	2001/11/16 nt
 *	head file of class CRefPt definitions
 */
#ifndef _RefPt_h_
#define _RefPt_h_

////////////////////////////////////////////////////////////////
//	RefPt
/*	2002/8/20 nt
 *	type of ref pt
 */
enum ERefPtType
{
	rpFree,
	rpInt
} ;

/*	2002/8/20 nt
 *	CRefPt
 */
class CRefPt : public CFeature
{
private:
	int	m_type ;
	PNT3D m_p ; // position
public:
	CRefPt() ;
	CRefPt(int type, PNT3D p) ;
	CRefPt(int type, int color, PNT3D p);
	~CRefPt() ;

	CFeature* Copy() ; // nt add 2009/5/27
	int	GetType() ;
	int	GetDetailType() ;
	void GetP(PNT3D p) ;
	int	CalBox() ;
	int Move(VEC3D v) ; // nt add 2009/5/27
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ; // nt add 2009/5/27
	int Mirror(PNT3D pivot, VEC3D normal) ; // nt add 2010/10/17
	int Scale(double scale) ; // nt add 2010/9/11
	int	Navigate(DNAVI* dn) ;

	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ; // nt add 2010/4/22
	int	Draw(void* pVI, int drawMode) ;
	int	Draw2(void* pVI, int drawMode) ; // nt add 2010/4/23
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif