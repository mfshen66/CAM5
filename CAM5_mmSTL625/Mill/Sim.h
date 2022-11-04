/*	2010/3/31 nt
 *	the head file of CSim
 */
#ifndef _Sim_h_
#define _Sim_h_

////////////////////////////////////////////////////////////////
// CSim : 刀具运动仿真
class CSim
{
private:
	int m_state ; // 0=未开始, 1=正在, 2=完成
	CTool* m_pTool ; // 引用
	RFRAME m_old ; // m_pTool的原始位置
	GCODETPATH* m_pGT ; // gcode for 3-axis, 引用, 仅用于三坐标的情况
	int m_n ; // 刀位点总数
	double m_minsp ; // mm/min
	double m_maxsp ; // mm/min
	int m_pause ; // nt add 2010/7/2, 1=pause

	UINT m_t ; // last update time, unit=second/1000, 0=initial_value
	int m_i ; // 上次更新刚走过第i个刀位点, 0=initial_value
	double m_speed ; // mm/min
	RFRAME m_lf1 ; // 刀位点:i
	RFRAME m_lf2 ; // 刀位点:i+1
	double m_d ; // from m_lf1.O to m_lf2.O
	RFRAME m_lf ; // m_pTool的当前位置,在m_lf1和m_lf2之间
	int m_bi ; // 当前G代码的块索引号
	char m_cb[256] ; // 当前G代码的一行，即一个block
	int m_rs ; // running state, see GCT_API.h
	
public:
	CSim() ;
	~CSim() ;

	int IsPaused() ; // nt add 2010/7/2
	int Pause() ; // nt add 2010/7/2
	int Continue() ; // nt add 2010/7/2
	int GetState() ;
	int SetGT(GCODETPATH* pGT) ;
	CTool* GetTool() ;
	int SetTool(CTool* pTool) ;
	int SetMinSpeed(double minsp) ;
	int SetMaxSpeed(double maxsp) ;
	int GetBI() ;
	int GetCurBlk(CString& cb) ;
	int GetCurPos(PNT3D cp) ;
	int GetRS() ;
	int UpdateTPN(int i) ; // update tool path node
	int Start(UINT ct) ; // ct=current time
	int Update(UINT ct) ; // ct=current time
	int Draw(void* pVI) ;
} ;
////////////////////////////////////////////////////////////////
#endif