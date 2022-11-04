/*	2000/5/13 nt
 *	Header File of Class CPan
 */
#ifndef _Pan_h_
#define _Pan_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
/*	1999/4/8 nt
 *	CPan
 */
class CPan : public CCommand
{
private:
	double m_p[3] ;
public:
	CPan(CCAMDoc* pDoc, CCAMView* pView) ;
	~CPan() ;

	int	GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int OnReturn() ;
	int	Cancel() ;
} ;
///////////////////////////////////////////////////////////////

#endif