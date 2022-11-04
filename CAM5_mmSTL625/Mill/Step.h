/*	2010/9/25 nt
 *	the head file of CStep
 */
#ifndef _Step_h_
#define _Step_h_

////////////////////////////////////////////////////////////////
// CStep : 工步
#define camStep 80 // nt add 2010/11/29

enum EGeometryModelType
{
	gmtWorkpiece = 0,
	gmtPart = 1,
	gmtPSurface = 2
} ;

class CStep : public CVO // nt modify 2010/11/29 old is CObj
{
private:
	int m_color ; // 0 to 255, 颜色索引号, nt add 2010/11/29
	CStr m_name ; // 工步的名称
	int m_no ; // 编号
	// (1)几何模型
	CIA m_workpiece ; // 由一些列CRefSur的no组成（由CStep释放空间），就是加工前模型
	CIA m_part ; // 由一些列CRefSur的no组成（由CMill释放空间），就是加工后模型
	CIA m_psurfaces ; // 保护面，由一些列CRefSur和CRefCur的no组成（由CStep释放空间）
	// (2)当前配置
	RFRAME m_lf ; // 当前加工坐标系???
	CTool* m_pTool ; // 当前使用刀具（由CStep释放空间）
	CP m_cp ; // 当前切削参数(cutting parameters)
	// (3)轨迹
	CPath* m_pPath ;
	BOX3D m_box ;
	HTREEITEM m_hItem ; // nt add 2010/11/29

public:
	CStep() ;
	~CStep() ;

	int GetType() ; // nt add 2010/11/29
	CStep* Copy() ; // nt add 2010/9/26
	void GetName(CStr& name) ;
	void SetName(CStr& name) ;
	void GetName(CString& name) ; // nt add 2016/6/4
	void SetName(CString& name) ; // nt add 2016/6/4
	int GetNo() ;
	int SetNo(int no) ;
	int GetNumOfGeometry(int gmt) ; // nt add 2010/9/26
	int GetGeometryNo(int gmt, int index) ; // nt add 2010/9/26
	int AddGeometry(int gmt, int geometryNo) ; // nt add 2010/9/26
	int DeleteGeometry(int gmt, int geometryNo) ; // nt add 2010/9/26
	void ClearModel(int gmt) ; // nt add 2010/9/26
	int GetMCS(RFRAME* pLF) ; // 取加工坐标系
	int SetMCS(RFRAME* pLF) ; // 设置加工坐标系
	void GetCP(CP* pCP) ; // nt add 2010/9/26
	void SetCP(CP* pCP) ; // nt add 2010/9/26
	CTool* GetTool() ;
	void SetTool(CTool* pTool) ;
	CPath* GetPath() ;
	void SetPath(CPath* pPath) ;
	int GetColor() ;  // nt add 2010/11/29
	void SetColor(int color) ;  // nt add 2010/11/29
	HTREEITEM GetItem() ; // nt add 2010/11/29
	void SetItem(HTREEITEM hItem) ; // nt add 2010/11/29
	int CalBox() ;
	int GetBox(BOX3D* box) ;
	void SetBox(BOX3D* box) ; // nt add 2010/9/26
	int Draw(void* pVI, int drawMode) ;
	int Draw2(void* pVI, int drawMode) ; // nt add 2010/11/29
	int Pick(PINFO* pinfo) ;
	int	PickByBox(RFRAME* pLF, BOX2D* pBox, double r, COA& es) ; // 1:picked;2:pick none;0:error
	int Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////
#endif