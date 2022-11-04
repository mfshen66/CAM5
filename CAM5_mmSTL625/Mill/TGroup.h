/*	2010/9/22 nt
 *	the head file of CTGroup
 *	µ¶¾ß×é
 */
#ifndef _TGroup_h_
#define _TGroup_h_

////////////////////////////////////////////////////////////////
class CTGroup : public CObj
{
public:
	int m_type ; // EToolType
	CStr m_name ; // nt add 2010/9/24
	HTREEITEM m_hItem ;
	COA m_subGroups ;
public:
	CTGroup(int m_type) ;
	~CTGroup() ;

	int GetType() ;
	void GetName(CStr& name) ;
	void SetName(char* name) ;
	void SetName(CStr& name) ;
	void GetName(CString& name);
	void SetName(CString& name);
	HTREEITEM GetItem() ;
	void SetItem(HTREEITEM hItem) ;
	int GetNumOfSubGroup() ;
	CTSubGroup* GetSubGroup(int i) ;
	int Add(CTSubGroup* pTSG) ;
	int Remove(CTSubGroup* pTSG) ;
	int Save(FILE* fp) ;
	int Load(FILE* fp) ;
} ;
////////////////////////////////////////////////////////////////

#endif