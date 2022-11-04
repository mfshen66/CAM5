/*	2001/11/16 nt
 *	head file of CSketch definitions
 */
#ifndef _Sketch_h_
#define _Sketch_h_

////////////////////////////////////////////////////////////////
//	Sketch
/*	1999/8/23 nt
 *	CSketch
 */
class CSketch : public CFeature
{
private:
	int	m_maxId ; // internal Id for defining wvertex and contour Id
	COA	m_wvertexes ; // all wvertexs
	COA	m_contours ; // all contours
	BOX2D m_box ; // defined in local frame
public:
	CSketch() ;
	~CSketch() ;

	CFeature* Copy() ; // nt add 2009/5/27
	int	GetType() ;
	int	Solve() ; // 0,1:error, success
	int	AlloclId() ;
	int	GetNumOfWVertex() ;
	CWVertex* GetWVertex(int index) ; // local index
	int	GetNumOfContour() ;
	CContour* GetContour(int index) ; // local index
	int	CalBox() ;
	int Move(VEC3D v) ; // nt add 2009/5/27
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ; // nt add 2009/5/27

	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ; // nt add 2010/4/22
	int	Draw(void* pVI, int drawMode) ;
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif