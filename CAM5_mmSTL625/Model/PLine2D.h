/*	2002/10/5 nt
 *	head file of class CPLine2D definition
 */
#ifndef _PLine2D_h_
#define _PLine2D_h_

////////////////////////////////////////////////////////////////
//	loop 2d
/*	2002/10/5 nt
 *	CPLine2D
 */
class CPLine2D : public CObj
{
private:
	BOX2D m_box ;
	CIA m_dirFlags ; // 1:same dir;-1:rev dir
	COA	m_refCurs ;

	int	m_np ;
	PNT2D* m_ps ;

public:
    CPLine2D() ;
    ~CPLine2D() ;

	// choose first refcur in refcurs as seed...
	int	Create(COA& refCurs, double tol) ; // tol is overlap tol

	int	GetNumOfCur() ;
	void GetBegin(PNT2D p) ;
	void GetEnd(PNT2D p) ;
	BOOL IsClosed(double tol) ; // TRUE or FALSE, tol is overlap tol
	int	Disc(double tol, double ang, double chd) ;
	int	GetNumOfDiscPt() ;
	void GetDiscPt(int index, PNT2D p) ;
	void GetBox(BOX2D* box) ;
    int	CalBox() ;
} ;
////////////////////////////////////////////////////////////////

#endif
