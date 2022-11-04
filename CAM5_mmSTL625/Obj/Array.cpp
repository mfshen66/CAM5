/*	1999/8/6 nt
 *	implementation file of class COA(object Array)
 */
#include "StdAfx.h"

///////////////////////////////////////////////////////////////
//	COA
/*	1999/8/6 nt
 *	Class COA
 */
COA::COA()
{
	int i ;

	m_grow = 8 ;
	m_nObj = 8 ;
	m_objs = new CObj*[m_nObj] ;
	if( !m_objs )
		stSetError(1) ;
	for( i = 0 ; i < m_nObj ; i++ )
		m_objs[i] = NULL ;
	m_size = 0 ;
}

COA::COA(int nGrow, int nSize)
{
	int i ;

	m_grow = nGrow < 1 ? 1 : nGrow ;
	m_nObj = nSize ;
	if( nSize > 0 )
	{
		m_objs = new CObj*[m_nObj] ;
		if( !m_objs )
			stSetError(1) ;
		for( i = 0 ; i < m_nObj ; i++ )
			m_objs[i] = NULL ;
	}
	else
		m_objs = NULL ;
	m_size = 0 ;
}

COA::~COA()
{
	if( m_objs )
		delete m_objs ;
}

/*	1999/8/6 nt
 *	basic operations of COA
 */
int	COA::GetSize()
{
	return m_size ;
}

int	COA::IsEmpty()
{
	return m_size == 0 ? 1 : 0 ;
}

void COA::SetSize(int nSize, int nGrow)
{
	int i ;
	CObj** objs ;

	m_grow = nGrow < 1 ? 1 : nGrow ;
	if( nSize != m_nObj ) // need reallocate memory
	{
		if( nSize < 1 ) // nSize < 1
		{
			if( m_objs )
				delete m_objs ;
			m_objs = NULL ;
			m_nObj = 0 ;
			m_size = 0 ;
		}
		else // nSize >= 1
		{
			objs = new CObj*[nSize] ;
			if( !objs )
				stSetError(1) ;
			else
			{
				if( m_size > nSize )
					m_size = nSize ;
				for( i = 0 ; i < m_size ; i++ )
					objs[i] = m_objs[i] ;
				for( i = m_size ; i < nSize ; i++ )
					objs[i] = NULL ;
				if( m_objs )
					delete m_objs ;
				m_objs = objs ;
				m_nObj = nSize ;
			}
		}
	}
}

void COA::RemoveAll()
{
	int i ;

	for( i = 0 ; i < m_size ; i++ )
		m_objs[i] = NULL ;
	m_size = 0 ;
}

void COA::FreeExtra()
{
	int i ;
	CObj** objs ;

	if( m_nObj > m_size )
	{
		if( m_size < 1 ) // free memory: m_objs
		{
			if( m_objs )
				delete m_objs ;
			m_objs = NULL ;
			m_nObj = 0 ;
		}
		else
		{
			objs = new CObj*[m_size] ;
			if( !objs )
				stSetError(1) ;
			else
			{
				for( i = 0 ; i < m_size ; i++ )
					objs[i] = m_objs[i] ;
				if( m_objs )
					delete m_objs ;
				m_objs = objs ;
				m_nObj = m_size ;
			}
		}
	}
}

CObj* COA::GetAt(int index)
{
	return m_objs[index] ;
}

void COA::SetAt(int index, CObj* pObj)
{
	m_objs[index] = pObj ;
}

int	COA::Add(CObj* pObj)
{
	if( m_size >= m_nObj )
		SetSize(m_nObj+m_grow, m_grow) ;
	m_objs[m_size++] = pObj ;

	return m_size-1 ;
}

int	COA::Append(COA* pOA)
{
	int i, n ;

	if( m_size + pOA->GetSize() > m_nObj )
		SetSize(m_size+pOA->GetSize(), m_grow) ;

	for( n = pOA->GetSize(), i = 0 ; i < n ; i++ )
		m_objs[m_size+i] = pOA->GetAt(i) ;
	m_size += n ;

	return m_size-n ;
}

void COA::Copy(COA* pOA)
{
	int i, n ;

	n = pOA->GetSize() ; // note : n may 0
	if( m_size != n )
	{
		if( m_objs )
		{
			delete m_objs ;
			m_objs = NULL ;
		}
		m_nObj = n ;
		if( n )
		{
			m_objs = new CObj*[n] ;
			if( !m_objs )
			{
				stSetError(1) ;
				return ;
			}
		}
	}
	for( i = 0 ; i < n ; i++ )
		m_objs[i] = pOA->GetAt(i) ;
	m_size = n ;
}

void COA::InsertAt(int index, CObj* pObj)
{
	int i ;

	if( m_size >= m_nObj )
		SetSize(m_nObj+m_grow, m_grow) ;
	for( i = m_size-1 ; i >= index ; i-- )
		m_objs[i+1] = m_objs[i] ;
	m_objs[index] = pObj ;
	m_size++ ;
}

void COA::InsertAt(int index, COA* pOA)
{
	int i, n ;

	if( (n = pOA->GetSize()) > 0 )
	{
		if( m_size+n > m_nObj )
			SetSize(m_size+n, m_grow) ;
		for( i = m_size-1 ; i >= index ; i-- )
			m_objs[i+n] = m_objs[i] ;
		for( i = 0 ; i < n ; i++ )
			m_objs[index+i] = pOA->GetAt(i) ;
		m_size += n ;
	}
}

void COA::RemoveAt(int index)
{
	int i ;

	for( i = index+1 ; i < m_size ; i++ )
		m_objs[i-1] = m_objs[i] ;
	m_size-- ;
}

// nt add 2010/4/18
// nt modify 2010/4/24
void COA::RemoveAt2(int index)
{
	if( m_size > 0 )
	{
		m_objs[index] = m_objs[m_size-1] ;
		m_size-- ;
	}
}

CObj* COA::operator[](int index)
{
	return m_objs[index] ;
}
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//	CIA
/*	1999/8/6 nt
 *	Class CIA
 */
CIA::CIA()
{
	int i ;

	m_grow = 8 ;
	m_nInt = 8 ;
	m_ints = new int[m_nInt] ;
	if( !m_ints )
		stSetError(1) ;
	for( i = 0 ; i < m_nInt ; i++ )
		m_ints[i] = NULL ;
	m_size = 0 ;
}

CIA::CIA(int nGrow, int nSize)
{
	int i ;

	m_grow = nGrow < 1 ? 1 : nGrow ;
	m_nInt = nSize ;
	if( nSize > 0 )
	{
		m_ints = new int[m_nInt] ;
		if( !m_ints )
			stSetError(1) ;
		for( i = 0 ; i < m_nInt ; i++ )
			m_ints[i] = NULL ;
	}
	else
		m_ints = NULL ;
	m_size = 0 ;
}

CIA::~CIA()
{
	if( m_ints )
		delete m_ints ;
}

/*	1999/8/6 nt
 *	basic operations of CIA
 */
int	CIA::GetSize()
{
	return m_size ;
}

int	CIA::IsEmpty()
{
	return m_size == 0 ? 1 : 0 ;
}

void CIA::SetSize(int nSize, int nGrow)
{
	int i ;
	int* ints ;

	m_grow = nGrow < 1 ? 1 : nGrow ;
	if( nSize != m_nInt ) // need reallocate memory
	{
		if( nSize < 1 ) // nSize < 1
		{
			if( m_ints )
				delete m_ints ;
			m_ints = NULL ;
			m_nInt = 0 ;
			m_size = 0 ;
		}
		else // nSize >= 1
		{
			ints = new int[nSize] ;
			if( !ints )
				stSetError(1) ;
			else
			{
				if( m_size > nSize )
					m_size = nSize ;
				for( i = 0 ; i < m_size ; i++ )
					ints[i] = m_ints[i] ;
				for( i = m_size ; i < nSize ; i++ )
					ints[i] = NULL ;
				if( m_ints )
					delete m_ints ;
				m_ints = ints ;
				m_nInt = nSize ;
			}
		}
	}
}

void CIA::RemoveAll()
{
	m_size = 0 ;
}

void CIA::FreeExtra()
{
	int i ;
	int* ints ;

	if( m_nInt > m_size )
	{
		if( m_size < 1 ) // free memory: m_ints
		{
			if( m_ints )
				delete m_ints ;
			m_ints = NULL ;
			m_nInt = 0 ;
		}
		else
		{
			ints = new int[m_size] ;
			if( !ints )
				stSetError(1) ;
			else
			{
				for( i = 0 ; i < m_size ; i++ )
					ints[i] = m_ints[i] ;
				if( m_ints )
					delete m_ints ;
				m_ints = ints ;
				m_nInt = m_size ;
			}
		}
	}
}

int CIA::GetAt(int index)
{
	return m_ints[index] ;
}

void CIA::SetAt(int index, int num)
{
	m_ints[index] = num ;
}

int	CIA::Add(int num)
{
	if( m_size >= m_nInt )
		SetSize(m_nInt+m_grow, m_grow) ;
	m_ints[m_size++] = num ;

	return m_size-1 ;
}

int	CIA::Append(CIA* pIA)
{
	int i, n ;

	if( m_size + pIA->GetSize() > m_nInt )
		SetSize(m_size+pIA->GetSize(), m_grow) ;

	for( n = pIA->GetSize(), i = 0 ; i < n ; i++ )
		m_ints[m_size+i] = pIA->GetAt(i) ;
	m_size += n ;

	return m_size-n ;
}

void CIA::Copy(CIA* pIA)
{
	int i, n ;

	n = pIA->GetSize() ; // note : n may 0
	if( m_size != n )
	{
		if( m_ints )
		{
			delete m_ints ;
			m_ints = NULL ;
		}
		m_nInt = n ;
		if( n )
		{
			m_ints = new int[n] ;
			if( !m_ints )
			{
				stSetError(1) ;
				return ;
			}
		}
	}
	for( i = 0 ; i < n ; i++ )
		m_ints[i] = pIA->GetAt(i) ;
	m_size = n ;
}

void CIA::InsertAt(int index, int num)
{
	int i ;

	if( m_size >= m_nInt )
		SetSize(m_nInt+m_grow, m_grow) ;
	for( i = m_size-1 ; i >= index ; i-- )
		m_ints[i+1] = m_ints[i] ;
	m_ints[index] = num ;
	m_size++ ;
}

void CIA::InsertAt(int index, CIA* pIA)
{
	int i, n ;

	if( (n = pIA->GetSize()) > 0 )
	{
		if( m_size+n > m_nInt )
			SetSize(m_size+n, m_grow) ;
		for( i = m_size-1 ; i >= index ; i-- )
			m_ints[i+n] = m_ints[i] ;
		for( i = 0 ; i < n ; i++ )
			m_ints[index+i] = pIA->GetAt(i) ;
		m_size += n ;
	}
}

void CIA::RemoveAt(int index)
{
	int i ;

	for( i = index+1 ; i < m_size ; i++ )
		m_ints[i-1] = m_ints[i] ;
	m_size-- ;
}

int CIA::operator[](int index)
{
	return m_ints[index] ;
}
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//	CDA
/*	1999/8/6 nt
 *	Class CDA
 */
CDA::CDA()
{
	int i ;

	m_grow = 8 ;
	m_nDbl = 8 ;
	m_dbls = new double[m_nDbl] ;
	if( !m_dbls )
		stSetError(1) ;
	for( i = 0 ; i < m_nDbl ; i++ )
		m_dbls[i] = NULL ;
	m_size = 0 ;
}

CDA::CDA(int nGrow, int nSize)
{
	int i ;

	m_grow = nGrow < 1 ? 1 : nGrow ;
	m_nDbl = nSize ;
	if( nSize > 0 )
	{
		m_dbls = new double[m_nDbl] ;
		if( !m_dbls )
			stSetError(1) ;
		for( i = 0 ; i < m_nDbl ; i++ )
			m_dbls[i] = NULL ;
	}
	else
		m_dbls = NULL ;
	m_size = 0 ;
}

CDA::~CDA()
{
	if( m_dbls )
		delete m_dbls ;
}

/*	1999/8/6 nt
 *	basic operations of CDA
 */
int	CDA::GetSize()
{
	return m_size ;
}

int	CDA::IsEmpty()
{
	return m_size == 0 ? 1 : 0 ;
}

void CDA::SetSize(int nSize, int nGrow)
{
	int i ;
	double* dbls ;

	m_grow = nGrow < 1 ? 1 : nGrow ;
	if( nSize != m_nDbl ) // need reallocate memory
	{
		if( nSize < 1 ) // nSize < 1
		{
			if( m_dbls )
				delete m_dbls ;
			m_dbls = NULL ;
			m_nDbl = 0 ;
			m_size = 0 ;
		}
		else // nSize >= 1
		{
			dbls = new double[nSize] ;
			if( !dbls )
				stSetError(1) ;
			else
			{
				if( m_size > nSize )
					m_size = nSize ;
				for( i = 0 ; i < m_size ; i++ )
					dbls[i] = m_dbls[i] ;
				for( i = m_size ; i < nSize ; i++ )
					dbls[i] = NULL ;
				if( m_dbls )
					delete m_dbls ;
				m_dbls = dbls ;
				m_nDbl = nSize ;
			}
		}
	}
}

void CDA::RemoveAll()
{
	m_size = 0 ;
}

void CDA::FreeExtra()
{
	int i ;
	double* dbls ;

	if( m_nDbl > m_size )
	{
		if( m_size < 1 ) // free memory: m_dbls
		{
			if( m_dbls )
				delete m_dbls ;
			m_dbls = NULL ;
			m_nDbl = 0 ;
		}
		else
		{
			dbls = new double[m_size] ;
			if( !dbls )
				stSetError(1) ;
			else
			{
				for( i = 0 ; i < m_size ; i++ )
					dbls[i] = m_dbls[i] ;
				if( m_dbls )
					delete m_dbls ;
				m_dbls = dbls ;
				m_nDbl = m_size ;
			}
		}
	}
}

double CDA::GetAt(int index)
{
	return m_dbls[index] ;
}

void CDA::SetAt(int index, double dbl)
{
	m_dbls[index] = dbl ;
}

int	CDA::Add(double dbl)
{
	if( m_size >= m_nDbl )
		SetSize(m_nDbl+m_grow, m_grow) ;
	m_dbls[m_size++] = dbl ;

	return m_size-1 ;
}

int	CDA::Append(CDA* pDA)
{
	int i, n ;

	if( m_size + pDA->GetSize() > m_nDbl )
		SetSize(m_size+pDA->GetSize(), m_grow) ;

	for( n = pDA->GetSize(), i = 0 ; i < n ; i++ )
		m_dbls[m_size+i] = pDA->GetAt(i) ;
	m_size += n ;

	return m_size-n ;
}

void CDA::Copy(CDA* pDA)
{
	int i, n ;

	n = pDA->GetSize() ; // note : n may 0
	if( m_size != n )
	{
		if( m_dbls )
		{
			delete m_dbls ;
			m_dbls = NULL ;
		}
		m_nDbl = n ;
		if( n )
		{
			m_dbls = new double[n] ;
			if( !m_dbls )
			{
				stSetError(1) ;
				return ;
			}
		}
	}
	for( i = 0 ; i < n ; i++ )
		m_dbls[i] = pDA->GetAt(i) ;
	m_size = n ;
}

void CDA::InsertAt(int index, double dbl)
{
	int i ;

	if( m_size >= m_nDbl )
		SetSize(m_nDbl+m_grow, m_grow) ;
	for( i = m_size-1 ; i >= index ; i-- )
		m_dbls[i+1] = m_dbls[i] ;
	m_dbls[index] = dbl ;
	m_size++ ;
}

void CDA::InsertAt(int index, CDA* pDA)
{
	int i, n ;

	if( (n = pDA->GetSize()) > 0 )
	{
		if( m_size+n > m_nDbl )
			SetSize(m_size+n, m_grow) ;
		for( i = m_size-1 ; i >= index ; i-- )
			m_dbls[i+n] = m_dbls[i] ;
		for( i = 0 ; i < n ; i++ )
			m_dbls[index+i] = pDA->GetAt(i) ;
		m_size += n ;
	}
}

void CDA::RemoveAt(int index)
{
	int i ;

	for( i = index+1 ; i < m_size ; i++ )
		m_dbls[i-1] = m_dbls[i] ;
	m_size-- ;
}

double CDA::operator[](int index)
{
	return m_dbls[index] ;
}
///////////////////////////////////////////////////////////////