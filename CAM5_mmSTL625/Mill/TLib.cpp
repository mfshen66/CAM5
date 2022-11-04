/*	2010/9/23 nt
 *	implementation file of class CTLib
 */
#include "StdAfx.h"
#include "TRS.h"
#include "Tool.h"
#include "TSubGroup.h"
#include "TGroup.h"
#include "TLib.h"

///////////////////////////////////////////////////////////////
//	CTLib
/*	2010/9/23 nt
 *	CTlib
 */
CTLib::CTLib()
{
	m_modified = 0 ;
}

CTLib::~CTLib()
{
	int i, n = m_groups.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CTGroup* pTG = (CTGroup*)m_groups[i] ;
		if( pTG )
			delete pTG ;
	}
	m_groups.RemoveAll() ;
}
//--------------------------------------------------------------
// 生成一个基本的常用刀具库
int CTLib::CreateBasic()
{
	// group 0
	CTGroup* pTG = new CTGroup(toolTypeMill) ;
	pTG->SetName("铣刀") ;
	CTSubGroup* pTSG = new CTSubGroup(toolSubTypeFlat) ;
	pTSG->SetName("平底刀") ;
	CTool* pTool = new CTool() ;
	pTool->CreateFlat(5, 10) ;
	pTSG->Add(pTool) ;
	pTG->Add(pTSG) ;
	pTSG = new CTSubGroup(toolSubTypeBall) ;
	pTSG->SetName("球头刀") ;
	pTool = new CTool() ;
	pTool->CreateBall(5, 10) ;
	pTSG->Add(pTool) ;
	pTG->Add(pTSG) ;
	pTSG = new CTSubGroup(toolSubTypeTorus) ;
	pTSG->SetName("环形刀") ;
	pTool = new CTool() ;
	pTool->CreateTorus(10., 5., 5., 15., 50.) ;
	pTSG->Add(pTool) ;
	pTG->Add(pTSG) ;
	pTSG = new CTSubGroup(toolSubTypeBarrel) ;
	pTG->Add(pTSG) ;
	m_groups.Add(pTG) ;

	// group 1
	pTG = new CTGroup(toolTypeDrill) ;
	pTG->SetName("钻头") ;
	m_groups.Add(pTG) ;

	return 1 ;
}

void CTLib::SetModified()
{
	m_modified = 1 ;
}

int CTLib::IsModified()
{
	return m_modified ;
}

int CTLib::GetNumOfGroup()
{
	return m_groups.GetSize() ;
}

CTGroup* CTLib::GetGroup(int i)
{
	return (CTGroup*)m_groups[i] ;
}

int CTLib::GetNumOfTool()
{
	int i, n = GetNumOfGroup(), sum = 0 ;
	for( i = 0 ; i < n ; i++ )
	{
		CTGroup* pTG = GetGroup(i) ;
		if( pTG )
		{
			int j, m = pTG->GetNumOfSubGroup() ;
			for( j = 0 ; j < m ; j++ )
			{
				CTSubGroup* pTSG = pTG->GetSubGroup(j) ;
				if( pTSG )
				{
					sum += pTSG->GetNumOfTool() ;
				}
			}
		}
	}
	return sum ;
}

int CTLib::Add(CTGroup* pTG)
{
	if( pTG )
	{
		m_groups.Add(pTG) ;
		SetModified() ;
		return 1 ;
	}
	else
		return 0 ;
}

int CTLib::Remove(CTGroup* pTG)
{
	if( pTG )
	{
		int i, n = GetNumOfGroup() ;
		for( i = 0 ; i < n ; i++ )
		{
			CTGroup* pTG2 = GetGroup(i) ;
			if( pTG == pTG2 )
			{
				m_groups.RemoveAt(i) ;
				SetModified() ;
				return 1 ;
			}
		}
	}
	return 0 ;
}

int CTLib::Add(CTool* pTool)
{
	if( pTool )
	{
		int type = pTool->GetType() ;
		int subType = pTool->GetSubType() ;
		int i, n = GetNumOfGroup() ;
		for( i = 0 ; i < n ; i++ )
		{
			CTGroup* pTG = GetGroup(i) ;
			if( pTG &&
				pTG->GetType() == type )
			{
				int j, m = pTG->GetNumOfSubGroup() ;
				for( j = 0 ; j < m ; j++ )
				{
					CTSubGroup* pTSG = pTG->GetSubGroup(j) ;
					if( pTSG &&
						pTSG->GetSubType() == subType )
					{
						SetModified() ;
						return pTSG->Add(pTool) ;
					}
				}
			}
		}
	}

	return 0 ;
}

int CTLib::Remove(CTool* pTool)
{
	if( pTool )
	{
		int type = pTool->GetType() ;
		int subType = pTool->GetSubType() ;
		int i, n = GetNumOfGroup() ;
		for( i = 0 ; i < n ; i++ )
		{
			CTGroup* pTG = GetGroup(i) ;
			if( pTG &&
				pTG->GetType() == type )
			{
				int j, m = pTG->GetNumOfSubGroup() ;
				for( j = 0 ; j < m ; j++ )
				{
					CTSubGroup* pTSG = pTG->GetSubGroup(j) ;
					if( pTSG &&
						pTSG->GetSubType() == subType )
					{
						SetModified() ;
						return pTSG->Remove(pTool) ;
					}
				}
			}
		}
	}

	return 0 ;
}
//--------------------------------------------------------------
int CTLib::Save(CStr& pathName)
{
	FILE* fp = NULL ;
	fopen_s(&fp, pathName.GetBuffer(0), "w+t") ;
	if( fp )
	{
		int i, n = GetNumOfGroup() ;
		fprintf_s(fp, "%d\n\n", n) ;
		for( i = 0 ; i < n ; i++ )
		{
			CTGroup* pTG = GetGroup(i) ;
			if( pTG )
			{
				if( pTG->Save(fp) != 1 )
				{
					fclose(fp) ;
					return 0 ;
				}
				fprintf_s(fp, "\n") ;
			}
		}
		fclose(fp) ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/9/24
int CTLib::Save()
{
	return Save(m_pathName) ;
}

int CTLib::Load(CStr& pathName)
{
	m_pathName = pathName ;

	int i, n = m_groups.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CTGroup* pTG = (CTGroup*)m_groups[i] ;
		if( pTG )
			delete pTG ;
	}
	m_groups.RemoveAll() ;

	FILE* fp = NULL ;
	fopen_s(&fp, pathName.GetBuffer(0), "r+t") ;
	if( fp )
	{
		fscanf_s(fp, "%d", &n) ;
		for( i = 0 ; i < n ; i++ )
		{
			CTGroup* pTG = new CTGroup(0) ;
			if( pTG )
			{
				if( pTG->Load(fp) != 1 )
				{
					fclose(fp) ;
					return 0 ;
				}
			}
			m_groups.InsertAt(i, pTG) ; // 插入
		}
		fclose(fp) ;
		return 1 ;
	}
	else
		return 0 ;
}
///////////////////////////////////////////////////////////////