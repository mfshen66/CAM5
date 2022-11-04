/*	2010/11/12
 *	head file of class CTriangle definitions
 */
#ifndef _Triangle_h_
#define _Triangle_h_

////////////////////////////////////////////////////////////////
//	triangle
/*	2010/11/14 nt
 *	CTriangle
 */
class CTriangle : public CFeature
{
private:
	int	m_index ;
	PNT3D m_v1 ;
	VEC3D m_normal1 ;
	PNT3D m_v2 ;
	VEC3D m_normal2 ;
	PNT3D m_v3 ;
	VEC3D m_normal3 ;

public:
	CTriangle() ;
	CTriangle(PNT3D v1, PNT3D v2, PNT3D v3) ;
	CTriangle(PNT3D v1, VEC3D normal1, PNT3D v2, VEC3D normal2, PNT3D v3, VEC3D normal3) ;
	~CTriangle() ;

	CFeature* Copy() ;
	int	GetType() ;
	int GetIndex() ;
	void SetIndex(int index) ;
	int	CalBox() ;
	int Move(VEC3D v) ;
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ;
	int Mirror(PNT3D pivot, VEC3D normal) ;
	int Scale(double scale) ;
	int	Navigate(DNAVI* dn) ;

	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ;
	int	Draw(void* pVI, int drawMode) ;
	int	Draw2(void* pVI, int drawMode) ;
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif