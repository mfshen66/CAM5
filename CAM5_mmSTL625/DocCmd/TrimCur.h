/*	2000/8/19 nt
 *	header file of class CTrimCur
 */
#ifndef _TrimCur_h_
#define _TrimCur_h_

class CCAMDoc ;
class CCAMView ;

///////////////////////////////////////////////////////////////
//	trim curve
class CTrimCur : public CCommand // ¶¯Ì¬²Ã¼ô
{
private:
public:
	CTrimCur(CCAMDoc* pDoc, CCAMView* pView) ;
	~CTrimCur() ;

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