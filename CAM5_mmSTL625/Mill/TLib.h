/*	2010/9/22 nt
 *	the head file of CTLib
 */
#ifndef _TLib_h_
#define _TLib_h_

////////////////////////////////////////////////////////////////
/*	2010/9/22 nt
 *	CTLib
 */
class CTLib
{
private:
	CStr m_pathName ;
	int m_modified ;
	COA m_groups ;
public:
	CTLib() ;
	~CTLib() ;

	int CreateBasic() ; // 生成一个基本的常用刀具库
	void SetModified() ;
	int IsModified() ;
	int GetNumOfGroup() ;
	CTGroup* GetGroup(int i) ;
	int GetNumOfTool() ;
	int Add(CTGroup* pTG) ;
	int Remove(CTGroup* pTG) ;
	int Add(CTool* pTool) ;
	int Remove(CTool* pTool) ;
	int Save(CStr& pathName) ;
	int Save() ; // nt add 2010/9/24
	int Load(CStr& pathName) ;
} ;
////////////////////////////////////////////////////////////////

#endif