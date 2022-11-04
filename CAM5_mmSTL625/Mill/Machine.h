#ifndef _Machine_h_
#define _Machine_h_

////////////////////////////////////////////////////////////////
// CMachine : 机床
/*  机床类型 from XRF 2010/9
 *  a）三轴机床
 *  b）四轴机床，共有四种类型，根据回转轴可分为为A、B、A’、B’
 *  c) 五轴机床，共有12种类型，
 *  刀具双摆动：A-B，B-A，C-A，C-B；
 *  工作台双转动：A’-B’，A’-C’，B’-A’，B’-C’；
 *	刀具与工作台分别回转：A’-B，B’-A，C’-A，C’-B；
*/
// 机床及各部件组成一个装配模型，各组件有自己的局部坐标系,
class CMachine : public CObj
{
private:
	CString m_type ; // for example : "AB", "BA", or "A'B"...
	CString m_name ; // 名称及型号
	int m_nAxis ; // 3, 4, 5
	RFRAME m_lf ; // 与CMill和CPart共享一个世界坐标系 
	// 机床部件几何模型...
	// 机床各轴的运动关系...
	BOX3D m_box ;
public:
	CMachine(int nAxis) ;
	~CMachine() ;

	//int GetBox(BOX3D* box) ;
	//int Draw(void* pVI, int drawMode) ;
	//int Pick(PINFO* pinfo) ;
	int Serialize(CArchive& ar) ;
} ;
////////////////////////////////////////////////////////////////

#endif