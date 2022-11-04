/*	2010/9/22 nt
 *	implementation file of class CTGroup
 */
#include "StdAfx.h"
#include "TRS.h"
#include "Tool.h"
#include "TSubGroup.h"
#include "TGroup.h"

///////////////////////////////////////////////////////////////
//	CTGroup
/*	2010/9/22 nt
 *	CTGroup
 */
CTGroup::CTGroup(int type)
{
	m_type = type ;
}

CTGroup::~CTGroup()
{
	int i = 0, n = m_subGroups.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CTSubGroup* pTSG = (CTSubGroup*)m_subGroups[i] ;
		if( pTSG )
			delete pTSG ;
	}
	m_subGroups.RemoveAll() ;
}
//--------------------------------------------------------------
int CTGroup::GetType()
{
	return m_type ;
}

void CTGroup::GetName(CStr& name)
{
	name = m_name ;
}

void CTGroup::SetName(char* name)
{
	m_name = name ;
}

void CTGroup::SetName(CStr& name)
{
	m_name = name ;
}

void CTGroup::GetName(CString& name)
{
	name = m_name;
}

void CTGroup::SetName(CString& name)
{
	m_name = name;
}


HTREEITEM CTGroup::GetItem()
{
	return m_hItem ;
}

void CTGroup::SetItem(HTREEITEM hItem)
{
	m_hItem = hItem ;
}

int CTGroup::GetNumOfSubGroup()
{
	return m_subGroups.GetSize() ;
}

CTSubGroup* CTGroup::GetSubGroup(int i)
{
	return (CTSubGroup*)m_subGroups[i] ;
}

int CTGroup::Add(CTSubGroup* pTSG)
{
	if( pTSG )
	{
		m_subGroups.Add(pTSG) ;
		return 1 ;
	}
	else
		return 0 ;
}

int CTGroup::Remove(CTSubGroup* pTSG)
{
	if( pTSG )
	{
		int i, n = GetNumOfSubGroup() ;
		for( i = 0 ; i < n ; i++ )
		{
			CTSubGroup* pTSG2 = GetSubGroup(i) ;
			if( pTSG2 == pTSG )
			{
				m_subGroups.RemoveAt(i) ;
				return 1 ;
			}
		}
	}
	return 0 ;
}
//--------------------------------------------------------------
int CTGroup::Save(FILE* fp)
{
	if( fp )
	{
		fprintf_s(fp, "%d\n", m_type) ;
		fprintf_s(fp, "$%s\n", m_name.GetBuffer(0)) ;
		int i, n = GetNumOfSubGroup() ;
		fprintf_s(fp, "%d\n\n", n) ;
		for( i = 0 ; i < n ; i++ )
		{
			CTSubGroup* pTSG = GetSubGroup(i) ;
			if( pTSG )
			{
				if( pTSG->Save(fp) != 1 )
					return 0 ;
				fprintf_s(fp, "\n") ;
			}
		}
		return 1 ;
	}
	else
		return 0 ;
}

int CTGroup::Load(FILE* fp)
{
	int i, n = m_subGroups.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CTSubGroup* pTSG = (CTSubGroup*)m_subGroups[i] ;
		if( pTSG )
			delete pTSG ;
	}
	m_subGroups.RemoveAll() ;

	if( fp )
	{
		fscanf_s(fp, "%d", &m_type) ;
		while(1)
		{
			char c = fgetc(fp) ;
			if (c == '$' ||
				c == EOF)
				break ;
		}
		char buf[64] ;
		fgets(buf, 64, fp) ;
		m_name = buf ;
		fscanf_s(fp, "%d", &n) ;
		for( i = 0 ; i < n ; i++ )
		{
			CTSubGroup* pTSG = new CTSubGroup(0) ;
			if( pTSG )
			{
				if( pTSG->Load(fp) != 1 )
					return 0 ;
			}
			m_subGroups.InsertAt(i, pTSG) ; // 加入数组中
		}
		return 1 ;
	}
	else
		return 0 ;
}
////////////////////////////////////////////////////////////////
