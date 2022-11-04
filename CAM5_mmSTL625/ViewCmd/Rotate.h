/*	2000/5/13 nt
 *	Header File of Class commands used in CRotate
 */
#ifndef _Rotate_h_
#define _Rotate_h_

class CCAMDoc ;
class CCAMView ;

////////////////////////////////////////////////////////////////
/*	1999/4/8 nt
 *	CRotate
 */
class CRotate : public CCommand
{
private:
	double m_p[3] ;
	PNT3D m_center ; // nt add 2016/3/20
public:
	CRotate(CCAMDoc* pDoc, CCAMView* pView) ;
	~CRotate() ;

	int	GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int OnReturn() ; // nt add 2009/12/17
	int	Cancel() ;
} ;
////////////////////////////////////////////////////////////////

#endif
