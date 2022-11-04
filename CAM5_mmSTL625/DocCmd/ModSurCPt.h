/*	2000/8/19 nt
 *	header file of class CModSurfPt
 */
#ifndef _ModSurfPt_h_
#define _ModSurfPt_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
//	dynamic modify nurbs surface control point
class CModSurCPt : public CCommand // 动态修改NURBS曲面控制顶点
{
private:
	CRefSur* m_pRefSur ;
	BSSUR* m_nurbs ;
	int	m_i ;
	int	m_j ;
	VEC3D m_normal ;
	PNT3D m_p ;

public:
	CModSurCPt(CCAMDoc* pDoc, CCAMView* pView) ;
	~CModSurCPt() ;

	int Create(CRefSur* pRefSur) ;
	int	GetID() ;
	int	OnLButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int	OnLButtonUp(UINT nFlags, double p[3], double v[3]) ;
	int	OnMouseMove(UINT nFlags, double p[3], double v[3]) ;
	int	OnRButtonDown(UINT nFlags, double p[3], double v[3]) ;
	int OnReturn() ; // nt add 2009/10/21
	int	Cancel() ;
	void Draw() ;
} ;
///////////////////////////////////////////////////////////////
#endif