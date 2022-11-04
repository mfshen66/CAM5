/*	2001/11/16 nt
 *	head file of class CPic definitions
 */
#ifndef _Pic_h_
#define _Pic_h_

////////////////////////////////////////////////////////////////
//	图像类
/*	2003/6/2 nt
 *	CPic
 */
class CPic : public CFeature 
{
public:
	int	m_picW ;
	int	m_picH ;
	int	m_texW ;
	int	m_texH ;
	void* m_bits ;
	double m_size ;
	RFRAME m_lf ;
	int	m_transparent ; // 0，1:非透明，透明
	// 在透明情况中，背景部分一定是全透明的，非背景部分的alpha值由m_tdeg来定义
	int	m_tdeg ; // 透明度，0 to 255:  

public:
	CPic() ;
	CPic(RFRAME* pLF, 
		 CString& fileName, 
		 int transparent,
		 int tdeg,
		 int min, 
		 int max, 
		 int& ok) ;
	~CPic() ;

	void GetLF(RFRAME* pLF) ;
	double GetOnePixelSize() ;
	void SetOnePixelSize(double size) ;
	int	Project(PNT3D oldPt, VEC3D prjDir, PNT2D prjPt) ;
	int	ToPlane(PNT3D worldPt, PNT2D planePt) ;
	int	ToWorld(PNT2D localPt, PNT3D worldPt) ;
	int	GetType() ;
	CFeature* Copy() ;
	int GetBox(BOX3D* box) ; // nt add 2003/7/14
	int	CalBox() ;
	int Move(VEC3D v) ; // nt add 2009/5/27
	int Rotate(PNT3D pivot, VEC3D axis, double angle) ; // nt add 2009/5/27
	int Mirror(PNT3D pivot, VEC3D normal) ; // nt add 2010/10/17
	int Scale(double scale) ; // nt add 2010/9/11

	int	DrawBorder(void* pVI, int drawMode) ; // nt add 2011/7/6
	int	DrawPic(void* pVI, int drawMode) ; // nt add 2011/7/6
	int	Draw(void* pVI, int drawMode) ;
	int	Draw2(void* pVI, int drawMode) ; // nt add 2010/4/23
	int	Pick(PINFO* pi) ;
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r) ; // nt add 2010/4/22
	int	Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif