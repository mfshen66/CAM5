/*	2010/9/22 nt
 *	the head file of CTSubGroup
 *	µ¶¾ß×Ó×é
 */
#ifndef _TSubGroup_h_
#define _TSubGroup_h_

////////////////////////////////////////////////////////////////
class CTSubGroup : public CObj
{
public:
	int m_subType ; // EToolSubType
	CStr m_name ; // nt add 2010/9/24
	HTREEITEM m_hItem ;
	COA m_tools ;
public:
	CTSubGroup(int subType) ;
	~CTSubGroup() ;

	int GetSubType() ;
	void GetName(CStr& name) ;
	void SetName(char* name) ;
	void SetName(CStr& name) ;
	void GetName(CString& name);
	void SetName(CString& name);
	HTREEITEM GetItem() ;
	void SetItem(HTREEITEM hItem) ;
	int GetNumOfTool() ;
	CTool* GetTool(int i) ;
	int Add(CTool* pTool) ;
	int Remove(CTool* pTool) ;
	int Save(FILE* fp) ;
	int Load(FILE* fp) ;
} ;
////////////////////////////////////////////////////////////////

#endif