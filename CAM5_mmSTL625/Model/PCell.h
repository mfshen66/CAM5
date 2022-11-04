/*	2004/6/19
 *	head file of class CPCell definitions
 */
#ifndef _PCell_h_
#define _PCell_h_

////////////////////////////////////////////////////////////////
/*	2003/11/6 nt
 *	point cell
 */
class CPCell : public CObj
{
//private:
public:
	int	m_type ;
	int	m_m ;
	int	m_n ;
	CMPoint* m_mpoints ;
	int	m_color ;
	BOX3D m_box ;
public:
	CPCell() ;
	CPCell(int type) ;
	CPCell(int n, double (*ps)[3]) ;
	~CPCell() ;

	int	GetNumOfMPoint() ; // include all CMPoint(with attrib = -1)
	CMPoint* GetMPoint(int i) ;
	CMPoint* GetMPointArray() ;
	int	Load(CString& fileName) ;
	int	LoadPCD(CString& fileName) ; // nt add 2018/7/14
	int	CalBox() ;
	int	GetBox(BOX3D* box) ;

	int	Draw(void* pVI, int drawMode) ;
	int Serialize(CArchive& ar) ;
	int	DxfOut(FILE* fp) ; // nt add 2003/11/8
	int	ObjOut(FILE* fp) ; // wang dan add 2004
	int	Out3DS(CString& fileName) ; // wang dan add 2004
	int	VRMLColorOut(FILE* fp, CString& bitsName) ; // wang dan add 2004
	int	VRMLOut(FILE* fp) ; // wang dan add 2004
	int IGESOutPara(FILE* fp) ; // wang dan add 2004
	int IGESOutIndex(FILE* fp) ;
} ;
////////////////////////////////////////////////////////////////

#endif