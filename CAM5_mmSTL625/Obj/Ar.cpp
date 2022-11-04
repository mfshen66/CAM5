/*	1999/8/16 nt
 *	implementation file of archive read and write
 */
#include "StdAfx.h"

///////////////////////////////////////////////////////////////
/*	1999/8/16 nt
 *	CAr
 *	nt modify 2010/5/3 add mode = 2 and 3
 */
// nt add 2010/5/3
CAr::CAr()
{
	m_mode = -1 ;
	m_fp = NULL ;
	m_name[0] = '\0' ;
}

CAr::CAr(int mode, TCHAR* name)
{
	if( Open(mode, name) == 0 )
		stSetError(1) ;
}

CAr::~CAr()
{
	if( m_fp )
		fclose(m_fp) ;
}
//--------------------------------------------------------------
int CAr::Open(int mode, TCHAR* name)
{
	int i ;

	for( i = 0 ; name[i] != '\0' && i < 511 ; i++ )
		m_name[i] = name[i] ;
	if( i >= 511 )
		return 0 ;
	m_name[i] = '\0' ;
	m_fp = NULL ;
	if( mode == 0 )
	{
		_tfopen_s(&m_fp, name, _T("rb")) ; // nt modify 2001/11/28 ???
		m_mode = 0 ;
	}
	else
	if( mode == 1 )
	{
		_tfopen_s(&m_fp, name, _T("wb")) ; // nt modify 2001/11/28 ???
		m_mode = 1 ;
	}
	else
	if( mode == 2 )
	{
		_tfopen_s(&m_fp, name, _T("rt")) ;
		m_mode = 0 ;
	}
	else
	{
		_tfopen_s(&m_fp, name, _T("wt")) ;
		m_mode = 1 ;
	}
	if( !m_fp )
		return 0 ;
	return 1 ;
}

int CAr::Close()
{
	if( m_fp )
	{
		fclose(m_fp) ;
		m_fp = NULL ;
	}
	return 1 ;
}

// see fseek(), nt add 2010/5/3
int CAr::Seek(long offset, int where)
{
	if( m_fp )
		return fseek(m_fp, offset, where) ;
	else
		return -1 ;
}

/*	1999/8/16 nt
 *	CAr operator
 */
int	CAr::IsStoring()
{
	return m_mode ;
}

TCHAR* CAr::GetName()
{
	return m_name ;
}

FILE* CAr::GetFP()
{
	return m_fp ;
}

CAr& CAr::operator >> (int& i)
{
	if( m_fp )
		fread(&i, sizeof(int), 1, m_fp) ; // ???

	return *this ;
}

CAr& CAr::operator >> (float& f)
{
	if( m_fp )
		fread(&f, sizeof(float), 1, m_fp) ;

	return *this ;
}

CAr& CAr::operator >> (double& d)
{
	if( m_fp )
		fread(&d, sizeof(double), 1, m_fp) ;

	return *this ;
}

int CAr::Read(void* buffer, int size)
{
	if( m_fp )
		return (int)fread(buffer, size, 1, m_fp) ;
	else
		return 0 ;
}

CAr& CAr::operator << (int i)
{
	if( m_fp )
		fwrite(&i, sizeof(i), 1, m_fp) ;

	return *this ;
}

CAr& CAr::operator << (float f)
{
	if( m_fp )
		fwrite(&f, sizeof(float), 1, m_fp) ;

	return *this ;
}

CAr& CAr::operator << (double d)
{
	if( m_fp )
		fwrite(&d, sizeof(double), 1, m_fp) ;

	return *this ;
}

int CAr::Write(void* buffer, int size)
{
	if( m_fp )
		return (int)fwrite(buffer, size, 1, m_fp) ;
	else
		return 0 ;
}
///////////////////////////////////////////////////////////////