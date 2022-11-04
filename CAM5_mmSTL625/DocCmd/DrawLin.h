/*	2000/8/19 nt
 *	header file of class CDrawLin
 */
#ifndef _DrawLin_h_
#define _DrawLin_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
//	dynamic draw line
class CDrawLin : public CCommand // ¶¯Ì¬»­ÏßÀà
{
private:
	int m_stage ;
	PNT3D m_p0 ;
	PNT3D m_p1 ;
	PNT3D m_p2 ;

public:
	CDrawLin(CCAMDoc* pDoc, CCAMView* pView) ;
	~CDrawLin() ;

	int	GetID() ;
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