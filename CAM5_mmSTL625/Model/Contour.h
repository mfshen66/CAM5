/*	2001/11/16 nt
 *	head file of CContour definitions
 */
#ifndef _Contour_h_
#define _Contour_h_

////////////////////////////////////////////////////////////////
//	Contour
/*	1999/9/13 nt
 *	EContourType
 */
enum EContourType
{
	ctDirect, // directly draw
	ctImport, // import from edge or contour
} ;

/*	1999/9/11 nt
 *	CContour
 */
class CContour : public CVO
{
private:
	int	m_id ; // contour id:parent sketch internal id
	CSketch* m_pSketch ; // parent sketch
	CURVE* m_curve ; // geometry curve which is defined in local frame
	STRPT* m_disc ; // curve polyline defined in local frame
	BOX2D m_box ;
public:
	CContour() ;
	CContour(int id, CSketch* pSkt, CURVE* curve) ;
	~CContour() ;

	int	GetId() ;
	CSketch* GetParentSketch() ;
	int	GetType() ;
	CURVE* GetCurve() ;
	int	GetBox(BOX3D* box) ;
	int	Disc(double tol, double ang) ;
	int	CalBox() ;
	STRPT* GetDisc() ;
	int	GetNumOfWVertex() ;
	CWVertex* GetWVertex(int index) ; // index

	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ; // nt add 2010/4/22
	int	Draw(void* pVI, int drawMode) ;
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif