/*	2001/11/16 nt
 *	head file of CRefCur definitions
 */
#ifndef _RefCur_h_
#define _RefCur_h_

////////////////////////////////////////////////////////////////
//	CRefCur
/*	1999/8/24 nt
 *	ERefCurType
 */
enum ERefCurType
{
	rcLine,
	rcCircle,
	rcEllipse,
	rcThrPts, // through some vertex
	rcInt, // two CRefSur intersect curve
	rcHelix // make a circle(sketch) into helix
} ;

/*	1999/8/24 nt
 *	CRefCur
 */
class CRefCur : public CFeature
{
private:
	int	m_dtype ; // old name m_type
	CURVE* m_curve ; // defined in world frame
	INTERVAL m_interval ;
	STRPT* m_strpt ; // defined in word frame
	int	m_atts ; // &1=draw curvature vector, &2=draw parameter dir
	double m_u ; // nt add 2010/4/22, 拾取点对应的参数u
	int m_width ; // nt add 2010/11/27，BYLAYER, >0就是毫米的100倍
	CString m_ltName ; // 线型名称，若为空则无线型, nt add 2010/11/27
public:
	CRefCur() ;
	CRefCur(int type, BSCUR* nurbs) ;
	CRefCur(int type, CURVE* curve, INTERVAL* interval) ;
	CRefCur(PNT3D p, PNT3D q) ; // nt add 2022/7/10
	~CRefCur() ;

	CFeature* Copy() ; // nt add 2009/5/27
	int	GetType() ;
	int	GetDetailType() ;
	int GetWidth() ; // nt add 2010/11/27
	void SetWidth(int width) ; // nt add 2010/11/27
	void GetLTName(CString& ltName) ; // nt add 2010/11/27
	void SetLTName(CString& ltName) ; // nt add 2010/11/27
	CURVE* GetCurve() ;
	BSCUR* GetNurbs() ;
	void GetInterval(INTERVAL* interval) ;
	int	CalBox() ;
	int	Disc(double tol, double ang) ;
	int	GetBegin(PNT3D p) ; // nt add 2004/9/7
	int	GetEnd(PNT3D p) ; // nt add 2004/9/7
	STRPT* GetDisc() ; // nt add 2003/10/30
	int	ModInterval(INTERVAL* newInterval) ; // nt add 2006/8/19
	double GetU() ; // nt add 2010/4/22
	int SetU(double u) ; // nt add 2010/4/22
	void SetCur(int visible) ; // 0,1:invisible, visible
	int GetAtts() ; // nt add 2010/8/28
	void SwapDrawU() ; // nt add 2010/5/23
	int RevsDir() ; // nt add 2010/5/23
	int Move(VEC3D v) ; // nt add 2009/5/27
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ; // nt add 2009/5/27
	int Mirror(PNT3D pivot, VEC3D normal) ; // nt add 2010/10/17
	int Scale(double scale) ; // nt add 2010/9/11
	int	Navigate(DNAVI* dn) ;
	int	NavigateKeyPt(DNAVI* dn) ; // nt add 2010/3/26
	int Replace(CURVE* curve, INTERVAL* interval) ; // nt add 2011/8/22
	
	int	Draw(void* pVI, int drawMode) ;
	int	Draw2(void* pVI, int drawMode) ; // nt add 2010/4/23
	void Prompt(CStr& str) ; // nt add 2010/10
	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ; // nt add 2010/4/22
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif