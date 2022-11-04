/*	2011/4/20 nt
 *	head file of class CMark definitions
 */
#ifndef _Mark_h_
#define _Mark_h_

////////////////////////////////////////////////////////////////
//	Mark
/*	2011/4/20 nt
 *	type of mark
 */
enum EMarkType
{
	markK = 0,
	markKuKv = 1
} ;

/*	2011/4/20 nt
 *	CMark
 */
class CMark : public CFeature
{
private:
	int	m_type ;
	//LIN3D* m_lins ;
public:
	CMark() ;
	CMark(int type) ;
	~CMark() ;

	void AddLin(PNT3D p, PNT3D q) ;
	void AddLin2(PNT3D p, PNT3D q, double w) ; // nt add 2014/3/23
	CFeature* Copy() ;
	int	GetType() ;
	int	GetDetailType() ;
	int	CalBox() ;
	int Move(VEC3D v) ;
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ;
	int Mirror(PNT3D pivot, VEC3D normal) ;
	int Scale(double scale) ;
	int Scale2(double scale) ; // nt add 2020/2/20
	int	Navigate(DNAVI* dn) ;

	void Prompt(CStr& str) ; // nt add 2020/2/20
	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ;
	int	Draw(void* pVI, int drawMode) ;
	int	Draw2(void* pVI, int drawMode) ;
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif