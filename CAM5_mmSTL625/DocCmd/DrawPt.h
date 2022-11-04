/*	2000/8/19 nt
 *	header file of class DrawPt
 */
#ifndef _DrawPt_h_
#define _DrawPt_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
//	dynamic draw CRefPt
class CDrawPt : public CCommand // 动态画点的类
{
private:
	int m_n ;
	PNT3D m_p ;
public:
	CDrawPt(CCAMDoc* pDoc, CCAMView* pView) ;
	~CDrawPt() ;

	int	GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int OnReturn() ; // nt add 2009/10/21
	int	Cancel() ;
} ;
///////////////////////////////////////////////////////////////
#endif