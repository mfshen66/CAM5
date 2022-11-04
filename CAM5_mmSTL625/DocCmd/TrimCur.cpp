/*	1997/4/6 nt
 *	implementation file CTrimCur
 */
#include "StdAfx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "..\CAMView.h"
#include "..\Command.h"
#include "TrimCur.h"

///////////////////////////////////////////////////////////////
//	trim refcur
CTrimCur::CTrimCur(CCAMDoc* pDoc, CCAMView* pView)
{
	Init(pDoc, pView) ;
}

CTrimCur::~CTrimCur()
{
}
//--------------------------------------------------------------
int	CTrimCur::GetID()
{
	return dctTrimCur ;
}

int	CTrimCur::OnLButtonDown(UINT nFlags, double p[3], double v[3])
{
	if( Navigate(0, nFlags, p, v) )
	{
		PNT3D q ;
		GetNavP(q) ;
		CPart* pPart = m_pDoc->GetPart() ;
		if( !pPart )
			return 0 ;
		COA trimmed, created ;
		pPart->Trim(q, trimmed, created) ;

		int i, n ;
		CRefCur* pRefCur ;
		n = trimmed.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			pRefCur = (CRefCur*)trimmed[i] ;
			pRefCur->Draw(m_pVI, dmEraze) ;
			pPart->Delete(pRefCur) ;
			delete pRefCur ;
		}
		n = created.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			pRefCur = (CRefCur*)created[i] ;
			pRefCur->Draw(m_pVI, dmNormal) ;
			pPart->Add(pRefCur) ;
		}
		glFlush() ;
	}

	return 1 ;
}

int	CTrimCur::OnLButtonUp(UINT nFlags, double p[3], double v[3])
{
	return 1 ;
}

int	CTrimCur::OnMouseMove(UINT nFlags, double p[3], double v[3])
{
	Navigate(0, nFlags, p, v) ;
	return 1 ;
}

int	CTrimCur::OnRButtonDown(UINT nFlags, double p[3], double v[3])
{
	m_pDoc->ClearCmdAll() ;

	return 1 ;
}

// nt add 2009/10/21
int CTrimCur::OnReturn()
{
	return 1 ;
}

int	CTrimCur::Cancel()
{
	return 1 ;
}
///////////////////////////////////////////////////////////////
