/*	2000/8/19 nt
 *	header file of class CDrawArc
 */
#ifndef _DrawArc_h_
#define _DrawArc_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
//	dynamic draw arc by 3 points
class CDrawArc : public CCommand // 动态画Arc3P的类
{
private:
	int	m_stage ;
	PNT2D m_p1 ;
	PNT2D m_p2 ;
	PNT2D m_p3 ;
	int m_cor ; // nt add 2009/11/26, 1=做相切校正

public:
	CDrawArc(CCAMDoc* pDoc, CCAMView* pView) ;
	~CDrawArc() ;

	int	GetID() ;
	int SetCor(int cor) ; // nt add 2009/11/25
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int OnReturn() ; // nt add 2009/10/21
	int	Cancel() ;
private:
	void Draw() ;
} ;
///////////////////////////////////////////////////////////////
#endif