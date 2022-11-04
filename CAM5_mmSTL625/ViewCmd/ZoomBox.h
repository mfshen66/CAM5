/*	2000/5/13 nt
 *	Header File of Class commands used in CZoomBox
 */
#ifndef _ZoomBox_h_
#define _ZoomBox_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
/*	2004/11/5 nt
 *	CZoomBox
 */
class CZoomBox : public CCommand
{
private:
	double m_p1[3] ;
	double m_p2[3] ;
	int	m_drawed ;
public:
	CZoomBox(CCAMDoc* pDoc, CCAMView* pView) ;
	~CZoomBox() ;

	int	GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int OnReturn() ; // nt add 2009/12/17
	int	Cancel() ;
	int	Draw() ;
} ;
///////////////////////////////////////////////////////////////

#endif
