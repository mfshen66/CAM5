/*	2010/9/25 nt
 *	the head file of CP
 */
#ifndef _CP_h_
#define _Cp_h_

////////////////////////////////////////////////////////////////
// motion type from XRF 2010/9
#define MOTIONTYPE_MAXNUM 20 
enum EMotionType
{
	UF_PATH_MOTION_TYPE_UNDEFINED = 0, /* Motion type undefined. */
	UF_PATH_MOTION_TYPE_RAPID = 1, /* Motion type rapid. */ // 快速运动
	UF_PATH_MOTION_TYPE_ENGAGE = 2, /* Motion type engage. */ // 进刀
	UF_PATH_MOTION_TYPE_CUT = 3, /* Motion type cut. */ // 切削
	UF_PATH_MOTION_TYPE_RETRACT = 4, /* Motion type retract. */ // 退刀
	UF_PATH_MOTION_TYPE_FIRST_CUT = 5, /* Motion type first cut. */ //第一刀切削
	UF_PATH_MOTION_TYPE_APPROACH = 6, /* Motion type approach. */ // 接近
	UF_PATH_MOTION_TYPE_STEPOVER = 7, /* Motion type stepover. */ // 步进
	UF_PATH_MOTION_TYPE_DEPARTURE = 8, /* Motion type departure.  This is
										only valid for a lathe. */ // 离开
	UF_PATH_MOTION_TYPE_TRAVERSAL = 9, /* Motion type traversal. */ //横越
} ;
//--------------------------------------------------------------
// CP : cutting parameter = 切削参数
typedef struct _cp CP ;
struct _cp
{
	double speeds[MOTIONTYPE_MAXNUM] ; // unit = mm/min
	int spindle ; // r/min
	double depth ; // cutting depth, unit=mm
	//double m_tol ; // 加工精度, unit=mm
	//double m_dTol ; // 离散精度, unit=mm
	//double m_dAng ; // 离散角度精度，弧度
	//int m_motionDir ; // 走刀方向,0=顺铣,1=逆铣
	//double m_slotD ; // 开槽深度,unit=mm
	//double m_stepOver ; // 间距, unit=mm
	//int m_bm ; // 进刀方式
	//double m_bAng ; // 下刀角度
	//int m_em ; // 退刀方式
} ;

/*
 *	CP.cpp
 */
void cpInit(CP* pCP) ;
////////////////////////////////////////////////////////////////

#endif