/*	2000/8/19 nt
 *	header file of class CDrawBC
 */
#ifndef _DrawBC_h_
#define _DrawBC_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
//	dynamic draw Bezier curve
class CDrawBC : public CCommand // 动态BezierCurve的类
{
private:
	int m_mode ; // nt add 2011/8/15, 0=control point, 1=interpolation
	int m_n ;
	double m_ps[64][3] ; // 最大阶数是64!!!

public:
	CDrawBC(CCAMDoc* pDoc, CCAMView* pView) ;
	~CDrawBC() ;

	int	GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int OnReturn() ; // nt add 2009/10/21
	int	Cancel() ;
private:
	int Draw() ;
} ;
///////////////////////////////////////////////////////////////
#endif