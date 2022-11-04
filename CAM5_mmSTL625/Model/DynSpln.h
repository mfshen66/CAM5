/*	1997/4/6 nt
 *	动态样条的头文件
 */
#ifndef _DynSpln_h_
#define _DynSpln_h_

///////////////////////////////////////////////////////////////
//	dynamic b-spline curve
class CDynSpln // 动态样条类
{
private:
	int			m_maxNum ;// 最大控制顶点个数
    int         m_idDraw ; // 0:尚未画曲线,1:已画曲线
	int         m_idSave ;
	int			m_idModify ;// 0:非修改,1:可修改,2:已修改
    int			m_idPeriodic ;
    int			m_colorIndex ;// 颜色索引
    int         m_idNew ;// 1:开始画新的曲线

    int         m_posi ; // 若为m_nCount+1,则增加一插值点;
                          // 若小于或等于m_nCount,则修改已有插值点
	int			m_nCount ;// 插值点总数
    PCSPLINE	m_staticSpline ;/ 静态样条

    STR1D		m_p_u ;// [0,m_maxNum-1]
    STR3D       m_tangPoints ;// [0,m_maxNum-1]
	STR3D		m_intpPoints ;// 插值点串[0,m_maxNum-1]
	PCSPLINE	m_dynmSpline ;// 动态样条
	
	int			m_nAppxi ; // 逼近点的个数
	STR3D		m_appxis ; // 曲线的离散逼近
public:
	CDynSpln(int maxNum = 128) ;
	~CDynSpln() ;
	
	void		Init() ;
	int			Start(PNT3D p) ;// 开始第一个插值点
	int			Append(PNT3D p) ; // 增加一个插值点
	int			Modify(PNT3D p) ; // 修改最后插值点
	int			Update() ;// 更新样条曲线
	void		Draw() ; // 画曲线变动部分
    void        Eraze() ;
	int			Disc(double accumChord) ;
	PCSPLINE	GetSpline() ;
    int         GetIdNew() ;
    int         GetIdDraw() ;
    void        SetIdDraw(int idDraw) ;
    void        DrawIntpPoints() ;
} ;
///////////////////////////////////////////////////////////////
	
#endif