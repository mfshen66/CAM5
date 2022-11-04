/*	2010/3/29 nt
 *	the head file of CMill
 */
#ifndef _Mill_h_
#define _Mill_h_

////////////////////////////////////////////////////////////////
// CMill : 数控加工工艺数据
class CMill : public CObj
{
private:
	CStr m_name ; // nt add 2010/9/26
	// (1)几何模型
	CIA m_workpiece ; // 由一些列CRefSur组成（由CMill释放空间），就是加工前模型
	CStr m_partName ; // 被加工零件的路径及名称
	CIA m_part ; // 由一些列CRefSur组成（由CMill释放空间），就是加工后模型
	// (2)材料
	int m_material ; // 材料类型索引号
	// (3)机床模型
	CMachine* m_pMachine ; // 机床模型
	// (4)当前配置
	RFRAME m_lf ; // 当前加工坐标系
	CTool* m_pTool ; // 当前使用刀具（由CMill释放空间）
	CP m_cp ; // 当前切削参数(cutting parameters)
	// (5)工步(process_step)数组
	COA m_steps ; // 工步数组
	BOX3D m_box ;

public:
	CMill() ;
	~CMill() ;

	void GetName(CStr& name) ;
	void SetName(CStr& name) ;
	int GetNumOfGeometry(int gmt) ; // nt add 2010/9/26
	int GetGeometryNo(int gmt, int index) ; // nt add 2010/9/26
	int AddGeometry(int gmt, int geometryNo) ; // nt add 2010/9/26
	int DeleteGeometry(int gmt, int geometryNo) ; // nt add 2010/9/26
	void ClearModel(int gmt) ; // nt add 2010/9/26
	int GetMCS(RFRAME* pLF) ; // 取加工坐标系
	int SetMCS(RFRAME* pLF) ; // 设置加工坐标系
	void GetCP(CP* pCP) ; // nt add 2010/9/26
	void SetCP(CP* pCP) ; // nt add 2010/9/26
	int GetPartName(CStr& partName) ;
	int SetPartName(CStr& partName) ;
	CTool* GetTool() ;
	void SetTool(CTool* pTool) ;
	CMachine* GetMachine() ;
	void SetMachine(CMachine* pMachine) ;
	int GetNumOfStep() ;
	CStep* GetStep(int index) ;
	int GetMaxStepNo() ; // nt add 2010/9/26
	int Add(CStep* pStep) ;
	int Delete(CStep* pStep) ;
	int CalBox() ;
	int GetBox(BOX3D* box) ;
	int Draw(void* pVI, int drawMode) ;
	int Pick(PINFO* pinfo) ;
	int	PickByBox(RFRAME* pLF, BOX2D* pBox, double r, COA& es) ; // 1:picked;2:pick none;0:error
	int Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif