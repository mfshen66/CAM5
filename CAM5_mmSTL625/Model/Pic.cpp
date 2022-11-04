/*	2003/6/24 nt
 *	implementation file of class CPic
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "Pic.h"

////////////////////////////////////////////////////////////////
//	picture
/*	2003/6/24 nt
 *	picture
 */
CPic::CPic()
{
	m_picW = 0 ;
	m_picH = 0 ;
	m_texW = 0 ;
	m_texH = 0 ;
	m_bits = NULL ;
	m_size = 1.e-1 ; // 1.e-4
	m_lf.O[0] = 0.0 ;
	m_lf.O[1] = 0.0 ;
	m_lf.O[2] = 0.0 ;
	m_lf.X[0] = 1.0 ;
	m_lf.X[1] = 0.0 ;
	m_lf.X[2] = 0.0 ;
	m_lf.Y[0] = 0.0 ;
	m_lf.Y[1] = 1.0 ;
	m_lf.Y[2] = 1.0 ;
	m_lf.scale = 1.0 ;
	m_transparent = 0 ;
	m_tdeg = 255 ; // nt add 2004/6/10
	// nt add 2009/12/22
	m_color = 259 ; // 256+3
}

/*	2004/6/3 nt
 *	根据图形文件(bmp, jpeg, gif)创建一个可用于opengl纹理显示的bgr(or bgra)数组
 *	tranparent = 0 : opique ; 1, transparent
 *	if transparent = 1, pixels which rgb componets are in [min, max] are opique,
 *	otherwise are transparent
 *	texW >= picW, texH >= picH
 *	texW must be 2^n,
 *	texH must be 2^m
*/
void* bitsCreate(CString& fileName, 
			     int transparent,
				 int tdeg,
				 int min,
				 int max,
			     int& picW, 
			     int& picH,
			     int& texW,
			     int& texH)
{
	int i, m, n ;
	n = fileName.GetLength() ;
	for( m = -1, i = n-1 ; i >= 0 ; i-- )
	{
		if( fileName.GetAt(i) == '.' )
		{
			m = i ;
			break ;
		}
	}
	CString ext = fileName.Right(n-m-1) ;
	ext.MakeLower();
	if( ext != _T("bmp") &&
		ext != _T("jpeg") &&
		ext != _T("jpg") &&
		ext != _T("gif") &&
		ext != _T("png") )
		return NULL ;

	// nt rewrite 2016/5/31 use CImage
	CImage image;
	HRESULT hResult = image.Load(fileName.GetBuffer(0));
	if( FAILED(hResult) )
		return NULL ;
	picW = image.GetWidth();
	picH = image.GetHeight();
	int k ;
	for( k = 1 ; k < picW ; k *= 2 ) ;
	texW = k ;
	for( k = 1 ; k < picH ; k *= 2 ) ;
	texH = k ;
	char* bits ;
	if( transparent == 0 )
		bits = new char[3*texW*texH] ;
	else
		bits = new char[4*texW*texH] ;
	int j ;
	unsigned char rgb[4], t ;
	COLORREF pixel;
	for( i = 0 ; i < texH ; i++ )
	{
		for( j = 0 ; j < texW ; j++ )
		{
			if( i < picH && j < picW )
			{
				pixel = image.GetPixel(j, i);
				rgb[0] = GetRValue(pixel);
				rgb[1] = GetGValue(pixel);
				rgb[2] = GetBValue(pixel);
				t = rgb[0] ;
				rgb[0] = rgb[2] ;
				rgb[2] = t ;
			}
			else
			{
				rgb[0] = 255 ;
				rgb[1] = 255 ;
				rgb[2] = 255 ;
			}
			if( transparent == 0 )
				memcpy(((char*)bits)+i*texW*3+j*3, rgb, sizeof(char)*3) ;
			else
			{
				if( (rgb[0] < min &&
					rgb[1] < min &&
					rgb[2] < min) ||
					(rgb[0] > max &&
					rgb[1] > max &&
					rgb[2] > max) )
					rgb[3] = 0 ;
				else
					rgb[3] = (char)tdeg ; // nt modify 2004/6/10, old code is : 255
				memcpy(((char*)bits)+i*texW*4+j*4, rgb, sizeof(char)*4) ;
			}
		}
	}

	return bits ;
}

CPic::CPic(RFRAME* pLF, 
		   CString& fileName, 
		   int transparent,
		   int tdeg,
		   int min,
		   int max,
		   int& ok)
{
	m_picW = 0 ;
	m_picH = 0 ;
	m_texW = 0 ;
	m_texH = 0 ;
	m_bits = NULL ;
	m_size = 5.e-1 ; // 5.e-4
	m_transparent = transparent ;
	m_tdeg = tdeg ;

	if( pLF )
		m_lf = *pLF ;

	m_bits = bitsCreate(fileName,
						transparent,
						tdeg,
						min,
						max,
						m_picW,
						m_picH,
						m_texW,
						m_texH) ;
	if( !m_bits )
		ok = 0 ;
	else
		ok = 1 ;

	return ;
}

CPic::~CPic()
{
	if( m_bits )
		delete []m_bits ;
}
//--------------------------------------------------------------
void CPic::GetLF(RFRAME* pLF)
{
	*pLF = m_lf ;
}

double CPic::GetOnePixelSize()
{
	return m_size ;
}

void CPic::SetOnePixelSize(double size)
{
	m_size = size ;

	CalBox() ;

	return ;
}

int CPic::Project(PNT3D oldPt, VEC3D prjDir, PNT2D prjPt)
{
	double t, d ;
	PNT3D p ;

	d = mathOProduct(prjDir, m_lf.Z) ;
	if( fabs(d) < MIN_DBL )
		return 0 ;
	t = (mathOProduct(m_lf.O, m_lf.Z)-mathOProduct(oldPt, m_lf.Z))/d ;
	p[0] = oldPt[0]+t*prjDir[0] ;
	p[1] = oldPt[1]+t*prjDir[1] ;
	p[2] = oldPt[2]+t*prjDir[2] ;
	ToPlane(p, prjPt) ;

	return 1 ;
}

int CPic::ToPlane(PNT3D worldPt, PNT2D planePt)
{
	VEC3D v ;

	v[0] = worldPt[0]-m_lf.O[0] ;
	v[1] = worldPt[1]-m_lf.O[1] ;
	v[2] = worldPt[2]-m_lf.O[2] ;
	planePt[0] = mathOProduct(m_lf.X, v) ;
	planePt[1] = mathOProduct(m_lf.Y, v) ;

	return 1 ;
}

int CPic::ToWorld(PNT2D localPt, PNT3D worldPt)
{
	PNT3D p ;

	p[0] = localPt[0] ;
	p[1] = localPt[1] ;
	worldPt[0] = m_lf.O[0]+m_lf.X[0]*p[0]+m_lf.Y[0]*p[1] ; 
	worldPt[1] = m_lf.O[1]+m_lf.X[1]*p[0]+m_lf.Y[1]*p[1] ;
	worldPt[2] = m_lf.O[2]+m_lf.X[2]*p[0]+m_lf.Y[2]*p[1] ;

	return 1 ;
}
//--------------------------------------------------------------
int CPic::GetType()
{
	return ftPic ;
}

CFeature* CPic::Copy()
{
	CPic* pPic = new CPic() ;
	pPic->m_picW = m_picW ;
	pPic->m_picH = m_picH ;
	pPic->m_texW = m_texW ;
	pPic->m_texH = m_texH ;
	pPic->m_size = m_size ;
	pPic->m_transparent = m_transparent ;
	int k ;
	if( m_transparent == 0 )
		k = 3 ;
	else
		k = 4 ;
	pPic->m_bits = new char[m_texW*m_texH*k] ;
	memcpy(pPic->m_bits, m_bits, sizeof(char)*m_texW*m_texH*k) ;
	pPic->m_lf = m_lf ;

	return pPic ;
}

/*  2003/7/3
 *	GetBox: return the box of pic ;
 *	Author: mijy 2003/07/03
 */
int CPic::GetBox(BOX3D* box)
{
	if( !box )
		return 0 ;

	CalBox() ;
	*box = m_box ;

	return 1 ;
}

int CPic::CalBox()
{
	BOX2D box2d ;

	box2d.min[0] = -0.5*m_size*m_picW ;
	box2d.min[1] = -0.5*m_size*m_picH ;
	box2d.max[0] = 0.5*m_size*m_picW ;
	box2d.max[1] = 0.5*m_size*m_picH ; ;
	mathTransLocalBox2D(&m_lf, &box2d, &m_box) ;

	return 1 ;
}

// nt add 2009/5/27
int CPic::Move(VEC3D v)
{
	m_lf.O[0] += v[0] ;
	m_lf.O[1] += v[1] ;
	m_lf.O[2] += v[2] ;
	CalBox() ;

	return 1 ;
}

// nt add 2009/5/27
int CPic::Rotate(PNT3D pivot, VEC3D axis, double angle)
{
	mathRotateRFrame(pivot, axis, angle, &m_lf) ;
	CalBox() ;

	return 1 ;
}

// nt add 2010/10/17
int CPic::Mirror(PNT3D pivot, VEC3D normal)
{
	mathMirPnt(pivot, normal, m_lf.O, m_lf.O) ;
	mathMirVec(pivot, normal, m_lf.X, m_lf.X) ;
	mathMirVec(pivot, normal, m_lf.Y, m_lf.Y) ;
	mathVProduct(m_lf.X, m_lf.Y, m_lf.Z) ;
	CalBox() ;

	return 1 ;
}

// nt add 2010/9/11
int CPic::Scale(double scale)
{
	m_lf.O[0] *= scale ;
	m_lf.O[1] *= scale ;
	m_lf.O[2] *= scale ;
	m_size *= scale ;
	CalBox() ;

	return 1 ;
}

/*int CPic::Prompt(CString& str) // nt add 2002/12/5
{
	str.Format("图像: 中心(%4.2lf, %4.2lf)",
		       1000.*(m_lf.O[0]+0.5*m_size*(m_lf.X[0]*m_picW+m_lf.Y[0]*m_picH)),
			   1000.*(m_lf.O[1]+0.5*m_size*(m_lf.X[1]*m_picW+m_lf.Y[1]*m_picH))) ;

	return 1 ;
}*/
//--------------------------------------------------------------
// nt add 2011/7/6
int CPic::DrawBorder(void* pVI, int drawMode)
{
	PNT2D planePt ;
	PNT3D p ;

	// 绘制边框
	int old = m_color ;
	m_color = 2 ;
	SetDrawColor2(drawMode) ;
	m_color = old ;
	glBegin(GL_LINE_LOOP) ;
		planePt[0] = -0.5*m_size*m_picW ;
		planePt[1] = -0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glVertex3dv(p) ;
		planePt[0] = 0.5*m_size*m_picW ;
		planePt[1] = -0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glVertex3dv(p) ;
		planePt[0] = 0.5*m_size*m_picW ;
		planePt[1] = 0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glVertex3dv(p) ;
		planePt[0] = -0.5*m_size*m_picW ;
		planePt[1] = 0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glVertex3dv(p) ;
	glEnd() ;

	return 1 ;
}

// nt modify 2009/12/22
int CPic::DrawPic(void* pVI, int drawMode)
{
	PNT2D planePt ;
	PNT3D p ;

	SetDrawColor(drawMode) ;
	viAddTexture(pVI, m_transparent, m_texW, m_texH, m_bits) ;
	glBegin(GL_QUADS) ;
		planePt[0] = -0.5*m_size*m_picW ;
		planePt[1] = -0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glTexCoord2d(0.0, 0.0) ;
		glVertex3dv(p) ;
		planePt[0] = 0.5*m_size*m_picW ;
		planePt[1] = -0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glTexCoord2d((double)m_picW/m_texW, 0.0) ;
		glVertex3dv(p) ;
		planePt[0] = 0.5*m_size*m_picW ;
		planePt[1] = 0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glTexCoord2d((double)m_picW/m_texW, (double)m_picH/m_texH) ;
		glVertex3dv(p) ;
		planePt[0] = -0.5*m_size*m_picW ;
		planePt[1] = 0.5*m_size*m_picH ;
		ToWorld(planePt, p) ;
		glNormal3dv(m_lf.Z) ;
		glTexCoord2d(0.0, (double)m_picH/m_texH) ;
		glVertex3dv(p) ;
	glEnd() ;
	viDelTexture(pVI, m_transparent) ;

	return 1 ;
}

// nt add 2010/4/23
// nt rewrite 2011/7/6
int CPic::Draw(void* pVI, int drawMode)
{
	DrawPic(pVI, drawMode) ;
	DrawBorder(pVI, drawMode) ;

	return 1 ;
}

// nt add 2010/4/23
int CPic::Draw2(void* pVI, int drawMode)
{
	DrawBorder(pVI, drawMode) ;

	return 1 ;
}
//--------------------------------------------------------------
// nt modify 2009/12/19
int CPic::Pick(PINFO* pi)
{
	double z ;
	PNT2D planePt ;
	PNT3D worldPt ;
	VEC3D v ;

	if( !pi )
		return 0 ;

	Project(pi->pos, pi->dir, planePt) ;
	if( planePt[0] > -0.5*m_size*m_picW-pi->r &&
		planePt[0] < 0.5*m_size*m_picW+pi->r &&
		planePt[1] > -0.5*m_size*m_picH-pi->r &&
		planePt[1] < 0.5*m_size*m_picH+pi->r )
	{
		ToWorld(planePt, worldPt) ;
		v[0] = worldPt[0]-pi->pos[0] ;
		v[1] = worldPt[1]-pi->pos[1] ;
		v[2] = worldPt[2]-pi->pos[2] ;
		z = mathOProduct(v, pi->dir) ;
		if( pi->obj == NULL ||
			pi->z > z )
		{
			pi->ot = ftPic ;
			pi->obj = this ;
			pi->u = planePt[0] ;
			pi->v = planePt[1] ;
			pi->z = z ;
			pi->att = 0 ; // nt add 2011/7/6
			return 2 ;
		}
	}

	return 2 ;
}

// nt add 2010/4/22
int CPic::PickByBox(RFRAME* pLF, BOX2D* pLB, double r)
{
	int i, j ;
	double dx, dy ;
	PNT3D vs[2][2] ;

	for( i = 0 ; i < 2 ; i++ )
	{
		dx = 2*(i-0.5)*m_size*m_picW ;
		for( j = 0 ; j < 2 ; j++ )
		{
			dy = 2*(j-0.5)*m_size*m_picH ;
			vs[i][j][0] = m_lf.O[0]+dx*m_lf.X[0]+dy*m_lf.Y[0] ;
			vs[i][j][1] = m_lf.O[1]+dx*m_lf.X[1]+dy*m_lf.Y[1] ;
			vs[i][j][2] = m_lf.O[2]+dx*m_lf.X[2]+dy*m_lf.Y[2] ;
			mathPrjPntPln(vs[i][j], pLF->O, pLF->Z, vs[i][j]) ;
			if( pLB->min[0]-r < vs[i][j][0] &&
				vs[i][j][0] < pLB->max[0]+r &&
				pLB->min[1]-r < vs[i][j][1] &&
				vs[i][j][1] < pLB->max[1]+r )
				return 1 ;
		}
	}

	for( i = 0 ; i < 2 ; i++ )
	{
		if( mathChkBox2DSegmInt(pLB, // intersect with border
								vs[i][0], 
								vs[i][1],
								r) == IDINT ||
			mathChkBox2DSegmInt(pLB, // intersect with border
								vs[0][i], 
								vs[0][i],
								r) == IDINT )
			return 1 ;
	}

	return 2 ;
}
//--------------------------------------------------------------
int CPic::Serialize(CArchive& ar)
{
	CFeature::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		ar << m_picW
		   << m_picH
		   << m_texW
		   << m_texH
		   << m_size
		   << m_transparent
		   << m_tdeg ;
		ar.Write(&m_lf, sizeof(RFRAME)) ;
		ar.Write(m_bits, (m_transparent+3)*sizeof(char)*m_texW*m_texH) ;
	}
	else
	{
		ar >> m_picW
		   >> m_picH
		   >> m_texW
		   >> m_texH
		   >> m_size
		   >> m_transparent
		   >> m_tdeg ;
		ar.Read(&m_lf, sizeof(RFRAME)) ;
		if( m_bits )
			delete []m_bits ;
		m_bits = new char[(m_transparent+3)*m_texW*m_texH] ;
		ar.Read(m_bits, (m_transparent+3)*sizeof(char)*m_texW*m_texH) ;
	}

	return 1 ;
}

/*int CPic::DxfOut(FILE* fp)
{
	if( !fp )
		return 0 ;

	return 1 ;
}*/
////////////////////////////////////////////////////////////////
