/*	2004/6/19 nt
 *	head file of point cloud definitions
 */
#ifndef _PCloud_h_
#define _PCloud_h_

////////////////////////////////////////////////////////////////
//	point cloud
/*	2003/11/6 nt
 *	point cloud
 */
class CPCloud
{
private:
	double m_mtol ; // measurement tolerance
	double m_mindis ;
	double m_maxdis ;
	
	COA	m_pcells ;

	// cube(i,j,k) : m_cubes[k*m_ns[0]*m_ns[1]+j*m_ns[0]+i]
	// cube(i,j,k) min corner : m_start+w*(X*i+Y*j+Z*k)
	// 0<=i<m_ns[0], 0<=j<m_ns[1], 0<=k<m_ns[2]
	int	m_ns[3] ; // number of cubes in x, y, z direction
	double m_w ; // cube edge wide
	PNT3D m_startp ; // lower left front corner
	CPCube* m_pcubes ; // array of cube
	double m_ctol ;

	BOX3D m_box ;
public:
	CPCloud() ;
	~CPCloud() ;

	int LoadPCD(CString& fileName) ; // nt add 2018/7/14
	int	AddPCell(CPCell* pPCell) ;
	int	CalBox() ;
	int	GetBox(BOX3D* box) ;
	CPCube* GetPCube(int i, int j, int k) ;
	int AddPCellToPCubes(CPCell* pPCell) ; // nt add 2003/11/8
	int	MakePCubes() ;
	CMPoint* GetNearestPt(PNT3D p, double r) ; // nt add 2003/12/3

	int	Pick(PINFO* pi) ; // return 0(error), 1(picked), 2(not picked)
	int PickByBox(RFRAME* pLF, BOX2D* pLB, double r, COA& mps) ;
	int PickedAll(COA& mps) ; // nt add 2010/4/22
	int	Draw(void* pVI, int drawMode) ;
	int	Serialize(CArchive& ar) ;
	int	DxfOut(FILE* fp) ; // nt add 2003/11/8
	int	ObjOut(FILE *fp) ; // wang dan add 2004
	int	Out3DS(CString& fileName) ; // wang dan add 2004
	int	VRMLOut(FILE* fp, CString& bitsName) ; // wang dan add 2004
	int	IGESOut(FILE* fp) ; // wang dan add 2004
} ;
////////////////////////////////////////////////////////////////

#endif