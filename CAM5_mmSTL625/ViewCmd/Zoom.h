/*	2000/5/13 nt
 *	Header File of Class commands used in CZoom
 */
#ifndef _Zoom_h_
#define _Zoom_h_

class CCAMDoc ;
class CCAMView ;

////////////////////////////////////////////////////////////////
/*	1999/4/8 nt
 *	CZoom
 */
class CZoom : public CCommand
{
private:
	double m_p[3] ;
	double m_scale ;
public:
	CZoom(CCAMDoc* pDoc, CCAMView* pView) ;
	~CZoom() ;

	int	GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnReturn() ; // nt add 2009/12/17
	int	Cancel() ;
} ;
////////////////////////////////////////////////////////////////

#endif
