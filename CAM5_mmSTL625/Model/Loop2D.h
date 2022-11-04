/*	2004/9/7 nt
 *	head file of class CLoop2D definition
 */
#ifndef _Loop2D_h_
#define _Loop2D_h_

///////////////////////////////////////////////////////////////
//	loop 2d
/*	2004/9/7 nt
 *	CLoop2D
 */
class CLoop2D
{
public:
	BOX2D m_box ;
	int	m_nLoop ;
	int* m_nPLps ; ;
	double (**m_loops)[2] ;
	double (**m_infos)[3] ;

public:
    CLoop2D() ;
    ~CLoop2D() ;

	// choose first refcur in refcurs as seed...
	int	Create(COA& refCurs, double tol, double ang, double chd) ; // tol is appx tol
	int	CalBndDirs() ; // nt add 2004/9/9
	double CalNearestDis(PNT2D p) ; // nt add 2004/9/8
} ;
///////////////////////////////////////////////////////////////

#endif