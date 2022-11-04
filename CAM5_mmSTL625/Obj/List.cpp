/*	1999/7/5 nt
 *	implementation file of class CLI(list term), COL(object list)
 */
#include "StdAfx.h"

///////////////////////////////////////////////////////////////
//	CLI COL
/*	1999/7/5 nt
 *	Class CLI
 */
CLI::CLI(CObj* pObj)
{
	m_prev = m_next = NULL ;
	m_pObj = pObj ;
}

CLI::~CLI()
{
}

CLI* CLI::GetPrev()
{
	return m_prev ;
}

void CLI::SetPrev(CLI* prev)
{
	m_prev = prev ;
}

CLI* CLI::GetNext()
{
	return m_next ;
}

void CLI::SetNext(CLI* next)
{
	m_next = next ;
}

CObj* CLI::GetObj()
{
	return m_pObj ;
}

void CLI::SetObj(CObj* pObj)
{
	m_pObj = pObj ;
}

/*	1999/7/5 nt
 *	Class COL
 */
COL::COL()
{
	m_size = 0 ;
	m_head = m_tail = NULL ;
}

COL::~COL()
{
	RemoveAll() ;
}

int	COL::GetSize()
{
	return m_size ;
}

int	COL::IsEmpty()
{
	return m_size < 1 ? 0 : 1 ;
}

void COL::RemoveAll()
{
	CLI* temp ;

	while(m_head)
	{
		temp = m_head->m_next ;
		delete m_head ;
		m_head = temp ;
	}
	m_size = 0 ;
}

CObj* COL::GetHead()
{
	if( m_head )
		return m_head->m_pObj ;
	else
		return NULL ;
}

POSI COL::GetHeadPosi()
{
	return m_head ;
}

POSI COL::AddHead(CObj* pObj)
{
	CLI* temp ;

	if( !pObj)
		return NULL ;

	temp = new CLI(pObj) ;
	if( !temp )
	{
		stSetError(1) ;
		return NULL ;
	}
	temp->m_next = m_head ;
	if( m_head )
		m_head->m_prev = temp ;
	else
		m_tail = temp ;
	m_head = temp ;
	m_size++ ;

	return m_head ;
}

CObj* COL::RemoveHead()
{
	CLI* temp ;
	CObj* pObj = NULL ;

	if( m_head )
	{
		temp = m_head->m_next ;
		if( temp )
			temp->m_prev = NULL ;
		else
		{
			m_tail = NULL ;
			temp = NULL ;
		}
		pObj = m_head->GetObj() ;
		delete m_head ;
		m_head = temp ;
		m_size-- ;
	}

	return pObj ;
}

CObj* COL::GetTail()
{
	if( m_tail )
		return m_tail->m_pObj ;
	else
		return NULL ;
}

POSI COL::GetTailPosi()
{
	return m_tail ;
}

POSI COL::AddTail(CObj* pObj)
{
	CLI* temp ;

	if( !pObj )
		return NULL ;

	temp = new CLI(pObj) ;
	if( !temp )
	{
		stSetError(1) ;
		return NULL ;
	}
	temp->m_prev = m_tail ;
	if( m_tail )
		m_tail->m_next = temp ;
	else
		m_head = temp ;
	m_tail = temp ;
	m_size++ ;

	return m_tail ;
}

CObj* COL::RemoveTail()
{
	CLI* temp ;
	CObj* pObj = NULL ;

	if( m_tail )
	{
		temp = m_tail->m_prev ;
		if( temp )
			temp->m_next = NULL ;
		else
		{
			m_head = NULL ;
			temp = NULL ;
		}
		pObj = m_tail->m_pObj ;
		delete m_tail ;
		m_tail = temp ;
		m_size-- ;
	}

	return pObj ;
}

CObj* COL::GetNext(POSI& pos)
{
	CLI* temp ;

	temp = (CLI*)pos ;
	if( temp )
	{
		pos = temp->m_next ;
		return temp->m_pObj ;
	}
	else
		return NULL ;
}

CObj* COL::GetPrev(POSI& pos)
{
	CLI* temp ;

	temp = (CLI*)pos ;
	if( temp )
	{
		pos = temp->m_prev ;
		return temp->m_pObj ;
	}
	else
		return NULL ;
}

CObj* COL::GetAt(POSI pos)
{
	if( pos )
		return ((CLI*)pos)->m_pObj ;
	else
		return NULL ;
}

void COL::SetAt(POSI pos, CObj* pObj)
{
	if( pos )
		((CLI*)pos)->m_pObj = pObj ;
}

void COL::RemoveAt(POSI pos)
{
	CLI* temp ;

	temp = (CLI*)pos ;
	if( temp ) // 分别处理表头、表尾和中间项
	{
		if( m_head == temp )
			RemoveHead() ;
		else
		if( m_tail == temp )
			RemoveTail() ;
		else
		{
			if( !temp->m_prev || !temp->m_next ) // 合法性
			{
				stSetError(1) ;
				return ;
			}
			temp->m_prev->m_next = temp->m_next ;
			temp->m_next->m_prev = temp->m_prev ;
			delete temp ;
		}
	}
}

POSI COL::InsertBefore(POSI pos, CObj* pObj)
{
	CLI* temp1, *temp2 = NULL ;

	temp1 = (CLI*)pos ;
	if( pObj && temp1 )
	{
		if( temp1 == m_head )
			return AddHead(pObj) ;
		else
		{
			temp2 = new CLI(pObj) ;
			if( !temp1->m_prev || !temp2 )
			{
				stSetError(1) ;
				return NULL ;
			}
			temp2->m_prev = temp1->m_prev ;
			temp2->m_next = temp1 ;
			temp1->m_prev->m_next = temp2 ;
			temp1->m_prev = temp2 ;
		}
	}

	return temp2 ;
}

POSI COL::InsertAfter(POSI pos, CObj* pObj)
{
	CLI* temp1, *temp2 = NULL ;

	temp1 = (CLI*)pos ;
	if( pObj && temp1 )
	{
		if( temp1 == m_tail )
			return AddTail(pObj) ;
		else
		{
			temp2 = new CLI(pObj) ;
			if( !temp1->m_next || !temp2 )
			{
				stSetError(1) ;
				return NULL ;
			}
			temp2->m_prev = temp1 ;
			temp2->m_next = temp1->m_next ;
			temp1->m_next = temp2 ;
			temp1->m_next->m_prev = temp2 ;
		}
	}

	return temp2 ;
}

POSI COL::Find(CObj* pObj, POSI startAfter)
{
	CLI* temp ;

	if( startAfter )
		temp = ((CLI*)startAfter)->m_next ;
	else
		temp = m_head ;
	while(temp)
	{
		if( temp->m_pObj == pObj )
			return temp ;
		temp = temp->m_next ;
	}

	return NULL ;
}

POSI COL::FindIndex(int index) 
{
	int i ;
	CLI* temp ;

	if( index < 0 || index >= m_size )
		return NULL ;

	for( i = 0, temp = m_head ; temp ; temp = temp->m_next, i++ )
	{
		if( i == index )
			return temp ;
	}

	return NULL ;
}
///////////////////////////////////////////////////////////////
