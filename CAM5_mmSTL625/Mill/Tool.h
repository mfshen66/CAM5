/*	2010/3/30 nt
 *	the head file of CTool
 */
#ifndef _Tool_h_
#define _Tool_h_

////////////////////////////////////////////////////////////////
// CTool : 刀具
/*	tool type
 */
enum EToolType2
{
	toolTypeUD = -1, // user defined
	toolTypeMill = 0, // 铣刀
	toolTypeDrill = 1,
	toolTypeLathe = 2
	// ...
} ;
//--------------------------------------------------------------
/*	tool subtype
 */
enum EToolSubType
{
	toolSubTypeUD = -1, // user defined
	toolSubTypeFlat = 0, // 平底刀
	toolSubTypeBall = 1, // 球头刀
	toolSubTypeTorus = 2, // 环形刀
	toolSubTypeBarrel = 3, // 鼓形刀
	toolSubTypeCone = 4, // 锥刀
	toolSubTypeBNose = 5, // 牛鼻刀
	toolSubTypeCBall = 6, // 锥度球头刀
	toolSubTypeCBNose = 7 // 锥度牛鼻刀
	// ...
} ;
//--------------------------------------------------------------
#define TOOL_MAXNUMPARAM 10
#define TOOL_MAXNUMTRS 32
/*	Usage:
 *		(1)CTool* pTool = new CTool() ;
 *		(2)pTool->CreateFlat(r, h) ;
 *		(3)pTool->Draw(pVI, drawMode) ;
 *		(4)delete pTool
 */
// nt modify 2010/9/22
class CTool : public CObj
{
public:
	int m_type ; // 类型
	int m_subType ; // 子类型
	int m_ver ; // version
	double m_ds[TOOL_MAXNUMPARAM] ; // 刀具长度参数
	double m_rs[TOOL_MAXNUMPARAM] ; // 刀具半径参数
	double m_as[TOOL_MAXNUMPARAM] ; // 刀具角度参数
	double m_rOffset ; // 半径补偿
	int m_roRegister ; // 半径补偿寄存器
	double m_zOffset ; // 长度补偿
	int m_zoRegister ; // 长度补偿寄存器
	int m_nBlade ; // 刀刃数
	int m_material ; // 材料号
	CStr m_name ; // 刀具名称
	int m_no ; // 刀具号

	BOX3D m_box ;
	RFRAME m_lf ; // 刀具自身的坐标系
	double m_dTol ;
	double m_dAng ;
	int m_color ; // RGB
	int m_n ; // 有多少个TRS
	TRS* m_trss[TOOL_MAXNUMTRS] ; // TRS数组

	HTREEITEM m_hItem ;
	
public:
	CTool() ;
	~CTool() ;

	int CreateFlat(double r, double h) ;
	int CreateBall(double r, double h) ;
	int CreateTorus(double r1,
			        double r2,
					double r3,
					double h1,
					double h2) ; // nt add 2010/6/28
	//...
	int Recreate() ; // nt add 2010/9/22
	CTool* Copy() ; // nt add 2010/9/22
	int GetType() ;
	int GetSubType() ;
	int GetNumOfTRS() ;
	TRS* GetTRS(int index) ;
	int AddProfile(CURVE* curve, INTERVAL* interval) ; // copy curve
	double GetDTol() ;
	int SetDTol(double dTol) ;
	double GetDAng() ;
	int SetDAng(double dAng) ;
	int GetLF(RFRAME* pLF) ;
	int SetLF(RFRAME* pLF) ;
	int GetName(CStr& name) ;
	int SetName(CStr& name) ;
	int GetName(CString& name) ; // nt add 2016/6/4
	int SetName(CString& name) ; // nt add 2016/6/4
	int GetNo() ;
	int SetNo(int no) ;
	HTREEITEM GetItem() ;
	void SetItem(HTREEITEM hItem) ;
	int GetColor() ;
	int SetColor(int color) ;
	int CalBox() ;
	int GetBox(BOX3D* box) ;
	int Disc() ;
	int Draw(void* pVI, int drawMode) ;
	int Pick(PINFO* pi) ; // return 0=error,1=picked,2=not picked
	int Save(FILE* fp) ; // nt add 2010/9/22
	int Load(FILE* fp) ; // nt add 2010/9/22
	int Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////
#endif