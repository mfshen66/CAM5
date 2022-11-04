/*	2010/9/22 nt
 *	implementation file of class CTGroup
 */
#include "StdAfx.h"
#include "TRS.h"
#include "Tool.h"
#include "TSubGroup.h"

///////////////////////////////////////////////////////////////
//	CTSubGroup
/*	2010/9/22 nt
 *	CTSubGroup
 */
CTSubGroup::CTSubGroup(int subType)
{
	m_subType = subType ;
	m_hItem = 0 ;
}

CTSubGroup::~CTSubGroup()
{
	int i = 0, n = m_tools.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CTool* pTool = (CTool*)m_tools[i] ;
		if( pTool )
			delete pTool ;
	}
	m_tools.RemoveAll() ;
}
//--------------------------------------------------------------
int CTSubGroup::GetSubType()
{
	return m_subType ;
}

void CTSubGroup::GetName(CStr& name)
{
	name = m_name ;
}

void CTSubGroup::SetName(char* name)
{
	m_name = name ;
}

void CTSubGroup::SetName(CStr& name)
{
	m_name = name ;
}

void CTSubGroup::GetName(CString& name)
{
	name = m_name;
}

void CTSubGroup::SetName(CString& name)
{
	m_name = name;
}

HTREEITEM CTSubGroup::GetItem()
{
	return m_hItem ;
}

void CTSubGroup::SetItem(HTREEITEM hItem)
{
	m_hItem = hItem ;
}

int CTSubGroup::GetNumOfTool()
{
	return m_tools.GetSize() ;
}

CTool* CTSubGroup::GetTool(int i)
{
	return (CTool*)m_tools[i] ;
}

int CTSubGroup::Add(CTool* pTool)
{
	if( pTool )
	{
		m_tools.Add(pTool) ;
		return 1 ;
	}
	else
		return 0 ;
}

int CTSubGroup::Remove(CTool* pTool)
{
	if( pTool )
	{
		int i, n = GetNumOfTool() ;
		for( i = 0 ; i < n ; i++ )
		{
			CTool* pTool2 = GetTool(i) ;
			if( pTool2 == pTool )
			{
				m_tools.RemoveAt(i) ;
				return 1 ;
			}
		}
	}
	return 0 ;
}
//--------------------------------------------------------------
int CTSubGroup::Save(FILE* fp)
{
	if( fp )
	{
		fprintf_s(fp, "%d\n", m_subType) ;
		fprintf_s(fp, "#%s\n", m_name.GetBuffer(0)) ;
		int i, n = GetNumOfTool() ;
		fprintf_s(fp, "%d\n", n) ;
		for( i = 0 ; i < n ; i++ )
		{
			CTool* pTool = GetTool(i) ;
			if( pTool )
			{
				if( pTool->Save(fp) != 1 )
					return 0 ;
				fprintf_s(fp, "\n") ;
			}
		}
		return 1 ;
	}
	else
		return 0 ;
}

int CTSubGroup::Load(FILE* fp)
{
	int i, n = m_tools.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CTool* pTool = (CTool*)m_tools[i] ;
		if( pTool )
			delete pTool ;
	}
	m_tools.RemoveAll() ;

	if( fp )
	{
		fscanf_s(fp, "%d", &m_subType) ;
		while(1)
		{
			char c = fgetc(fp) ;
			if( c == '#' ||
				c == EOF )
				break ;
		}
		char buf[64] ;
		fgets(buf, 64, fp) ;
		m_name = buf ;
		fscanf_s(fp, "%d", &n) ;
		for( i = 0 ; i < n ; i++ )
		{
			CTool* pTool = new CTool() ;
			if( pTool )
			{
				if( pTool->Load(fp) != 1 )
					return 0 ;
			}
			m_tools.InsertAt(i, pTool) ;
		}
		return 1 ;
	}
	else
		return 0 ;
}
////////////////////////////////////////////////////////////////
