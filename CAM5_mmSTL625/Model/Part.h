/*	1999/8/20 nt
 *	head file of class CPart
 */
#ifndef _Part_h_
#define _Part_h_

#ifdef __cplusplus

#include "STL.h" // nt add 2022/6/15 temp

///////////////////////////////////////////////////////////////
//	Part Model
/*	1999/8/20 nt
 *	CPart
 */
#define MAX_NUM_FEATURE 32
class CPart : public CModel
{
//private:
public:
	// m_etCounts是用来给Entity自动命名
	int	m_ftCounts[MAX_NUM_FEATURE] ; // feature counter
	CPCloud* m_pPCloud ; // nt add 2003/11/6
	//CSketch* m_pEditingSketch ; // currently editing sketch
	COA	m_features ; // all features
	//CBody	m_body ; // solid body
	int m_curNo ; // 当前CFeature中最大的no, nt add 2010/9/26

	// nt add 2022/6/15 临时
	GM* m_pGM ; // grid model

public:
	CPart() ;
	~CPart() ;
	
    //int Init() ; // 0,1:error,sucess
	CFeature* GetFeatureByNo(int no) ; // nt add 2010/9/26
	int	LoadPts(CString& fileName) ; // nt add 2003/11/6
	CPCloud* GetPCloud() ; // nt add 2003/12/3

	//CSketch* GetEditingSketch() ;
	//void*	SetEditSketch(CSketch* pSketch) ;
	int	AddRefPlaneOfXYZ() ;
	int	GetNumOfFeature() ;
	CFeature* GetFeature(int index) ; // local index
	//CBody* GetBody() ;

	int Add(CFeature* pF) ; // 0,1:error,sucess
	//int Suppress(CFeature* pFeat, int flag = 1) ; // 特征挂起或恢复正常,0,1:error,sucess
	int	Delete(CFeature* pF) ; // 特征删除, 0,1:error,sucess
	int	Edit(CFeature* pF) ; // 特征编辑,0,1:error,sucess
	//int EditDim(CDim* pDim) ; // 尺寸编辑,call by dim double click
	int Copy(COA& fs, COA& copied_fs) ; // nt add 2009/5/27
	int Move(COA& fs, VEC3D v) ; // nt add 2009/5/27
	int Rotate(COA& fs, PNT3D pivot, VEC3D axis, double angle) ; // nt add 2009/5/27
	int Mirror(COA& fs, PNT3D pivot, VEC3D normal) ; // nt add 2010/11/19
	int GetInitFeatureName(int ft, CStr& ftName) ;

    //int Rebuild() ; // call after Serialize() (loading),0,1:error,sucess
	int	Disc(double tol, double ang) ; // 0,1:error,sucess

    //int IsEmpty() ; // 1:empty;0:not empty
	//int NeedUpdate() ; // 0:not need update:non-zero, the first stepId that need update
	//int Update() ; // 0:error;1:sucess complete;2:sucess,but need continue to update
	int	Navigate(DNAVI* dn) ;
	int	NavigateKeyPt(DNAVI* dn) ; // nt add 2010/3/26
	int CalBox() ; // 0,1:error,sucess
	int Trim(PNT2D p, COA& trimmed, COA& created) ; // nt add 2009/5/7
	int Draw(void* pVI, int drawMode) ;
	int	Pick(PINFO* pi) ; // 1:picked;2:pick none;0:error
	int	PickByBox(RFRAME* pLF, BOX2D* pBox, double r, COA& es) ; // 1:picked;2:pick none;0:error
	int	SerializeFs(CArchive& ar, COA& fs) ; // 0,1:error,sucess, nt add 2010/10/17
    int	Serialize(CArchive& ar) ; // 0,1:error,sucess
	int	DxfOut(CString& fileName) ; // nt add 2003/11/8
	int	ObjOut(CString& fileName) ; // wang dan add 2004
	int	Out3DS(CString& fileName) ; // wang dan add 2004
	int	VRMLOut(CString& fileName, CString& bitsName) ; // wang dan add 2004
	int	IGESOut(CString& FileName) ; // wang dan add 2004
	int LoadGE(CString& fileName) ; // nt add 2009/4/25
	int ImportIGES(CString& fileName, int flags) ; // nt add 2010/5/8
	int ImportIGES3(CString& fileName, int flags) ; // nt add 2021/8/5 专用
	int ImportObj(CString& fileName) ; // nt add 2010/10/21
	int ExportPts(CString& fileName) ; // nt add 2015/5/30
	int ExportPts2(CString& fileName, COA& es) ; // nt add 2015/5/30
	int LoadPCD(CString& fileName) ; // nt add 2018/7/14
} ;
///////////////////////////////////////////////////////////////
#endif

#endif
