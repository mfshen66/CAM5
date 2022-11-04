/*	2000/7/24 nt
 *	implementation file of class CSelSet
 *	nt rewrite 2010/4/18
 */
#include "stdafx.h"
#include "Models.h"
#include "Mills.h"
#include "SelSet.h"
#include "ViewTree.h"
#include "CAMDoc.h"
#include "CAMView.h"

////////////////////////////////////////////////////////////////
//	CSelSet
CSelSet::CSelSet(void* pDoc)
{
	int i ;

	m_pDoc = pDoc ;
	m_pVI = NULL ;
	m_enabled = 1 ;
	// nt modify 2010/8/24 : old i is from 1
    for( i = 0 ; i < MAX_NUM_ET ; i++ )
		m_pickFlags[i] = 1 ;

	// nt add 2002/12/6
	m_p1[0] = 0.0 ;
	m_p1[1] = 0.0 ;
	m_p1[2] = 0.0 ;
	m_p2[0] = 0.0 ;
	m_p2[1] = 0.0 ;
	m_p2[2] = 0.0 ;
	m_draw = 0 ;

	// nt add 2009/11/12
	m_ppf = 1 ;

	// nt add 2010/4/18
	m_prompting = NULL ;
	m_actived = NULL ;

	// nt add 2010/4/15
	m_selecteds.SetSize(10000, 10000) ; //???
	m_es.SetSize(10000, 10000) ; //???
}

CSelSet::~CSelSet()
{
	m_selecteds.RemoveAll() ;
	m_es.RemoveAll() ; // nt add 2010/4/18
	return ;
}
//--------------------------------------------------------------
/*	1997/4/14 nt
 *	清除选择集中的所有实体
 */
void CSelSet::Clear()
{
	//m_pVI = NULL ;
	RemovePrompting() ;
	RemoveActived() ;
	RemoveSelected() ;

	// nt add 2002/12/6
	if( m_draw )
		DrawBox() ;
	m_p1[0] = -1.0e20 ;
	m_p1[1] = -1.0e20 ;
	m_p2[0] = -1.0e20 ;
	m_p2[1] = -1.0e20 ;
	m_draw = 0 ;

	// nt add 2010/4/15
	glFlush() ;

	return ;
}

// nt add 2009/9/17
void CSelSet::ClearDrawFlag()
{
	m_draw = 0 ;
	return ;
}

int CSelSet::IsEnable()
{
	return m_enabled ;
}

void CSelSet::Enable()
{
    m_enabled = 1 ;
}

void CSelSet::Disable()
{
    m_enabled = 0 ;
}

void CSelSet::EnableEntityPick(int type)
{
	m_pickFlags[type] = 1 ;
}

void CSelSet::DisableEntityPick(int type)
{
	m_pickFlags[type] = 0 ;
}

int CSelSet::IsEntityPickable(int type)
{
	return m_pickFlags[type] ;
}

// nt add 2010/4/18
int CSelSet::SetVI(void* pVI)
{
	m_pVI = pVI ;
	return 1 ;
}
//--------------------------------------------------------------
/*	1997/4/14 nt
 *	从选择集中取当前提示的元素
 */
CVO* CSelSet::GetPrompting()
{
	return m_prompting ;
}

// nt modify 2010/4/18
// nt modify 2011/7/4 : 当鼠标移动到曲面上时，不再进
// 行提示性显示
void CSelSet::RemovePrompting()
{
	// 原先已有提示实体,draw and delete
	if( m_prompting )
    {
		camPromptClear() ;
		// nt modify 2011/7/4
		/*if( m_prompting->IsPicked() == 1 )
			m_prompting->Draw2(m_pVI, dmSelect) ;
		else
			m_prompting->Draw2(m_pVI, dmNormal) ;*/
		int type = m_prompting->GetType() ;
		if( type < 80 )
		{
			CFeature* pF = (CFeature*)m_prompting ;
			if( pF->GetType() != ftRefSur )
				m_prompting->Draw2(m_pVI, dmNormal) ;
		}
		else
			m_prompting->Draw2(m_pVI, dmNormal) ;
		// end modify
	    m_prompting = NULL ;
    }
}

/*	1997/4/14 nt
 *	从选择集中取当前激活的元素
 */
CVO* CSelSet::GetActived()
{
	return m_actived ;
}

void CSelSet::RemoveActived()
{
	// 原先已有提示实体,draw and delete
	if( m_actived )
    {
		if( m_actived->IsPicked() == 1 )
			m_actived->Draw2(m_pVI, dmSelect) ;
		else
			m_actived->Draw2(m_pVI, dmNormal) ;
	    m_actived = NULL ;
    }
}

/*	1997/4/14 nt
 *	判断选择集是否为空
 */
int CSelSet::GetNumOfSelected()
{
	return m_selecteds.GetSize() ;
}

/*	2000/7/26 nt
 *	get a selected element
 */
CVO* CSelSet::GetSelected(int index)
{
	return (CVO*)m_selecteds[index] ;
}

/*	1997/4/14 nt
 *	从选择集中移出某一类的若干实例
 */
void CSelSet::RemoveSelected()
{
	int i, n, flag ;
	CVO* pVO ; // nt add 2010/4/18
	CCAMDoc* pDoc = (CCAMDoc*)m_pDoc ;
	CViewTree* pT = pDoc->GetTree() ;

	flag = ((CCAMDoc*)m_pDoc)->IsDestroying() ; // nt add 2002/5/21
	for( n = m_selecteds.GetSize(), i = 0 ; i < n ; i++ )
	{
		pVO = (CVO*)(m_selecteds[i]) ;
		if( flag == 0 )
			pVO->Draw2(m_pVI, dmNormal) ;
		pVO->SetPick(0) ;
		// nt add 2010/9/9-----------------------------------
		int type = pVO->GetType() ;
		if( type < 80 )
		{
			CFeature* pF = (CFeature*)pVO ;
			HTREEITEM hItem = pF->GetItem() ;
			pT->SetItemState(hItem, 0, TVIS_SELECTED) ;
		}
		else
		if( type == 80 ) // nt add 2010/11/29
		{
			CStep* pStep = (CStep*)pVO ;
			HTREEITEM hItem = pStep->GetItem() ;
			pT->SetItemState(hItem, 0, TVIS_SELECTED) ;
		}
		// end
	}
	m_selecteds.RemoveAll() ;
	pT->Invalidate(TRUE) ;
}

int CSelSet::GetSelected(int type, int nVO, COA& vos)
{
    int i, j, n, m ;
	CVO* pVO ;

	if( nVO < 1 )
		return 0 ;

	m = m_selecteds.GetSize() ;
	for( n = 0, i = 0 ; i < m ; i++ )
	{
		pVO = (CVO*)(m_selecteds[i]) ;
		// nt modify 2009/12/4 !!!100
		int type2 = pVO->GetType() ;
        if( (type == -1 &&
			type2 < 1000) || // get all entity
			type2 == type )
		{
			pVO->SetPick(0) ; // nt add 2010/4/18
			pVO->Draw2(m_pVI, dmNormal) ;
			vos.Add(m_selecteds[i]) ;
			m_selecteds.SetAt(i, NULL) ;
			n++ ;
			if( n >= nVO )
				break ;
		}
	}
	// nt add 2010/4/24 : 不改变m_selected中元素的顺序!!!
	for( i = 0 ; i < m ; i++ )
	{
		if( m_selecteds[i] == NULL )
		{
			for( j = i+1 ; j < m ; j++ )
			{
				if( m_selecteds[j] )
				{
					m_selecteds.SetAt(i, m_selecteds[j]) ;
					m_selecteds.SetAt(j, NULL) ;
					break ;
				}
			}
		}
	}
	for( i = m-1 ; i >= 0 ; i-- )
	{
		if( m_selecteds[i] == NULL )
			m_selecteds.RemoveAt(i) ;
	}

	// nt add 2010/4/15
	glFlush() ;
	
	return n ;
}

/*	1997/4/14 nt
 *	在选择集中增加一个实体
 */
int CSelSet::AddSelected(CVO* pVO) // 0:not add, 1:added
{
	if( pVO )
	{
		CCAMDoc* pDoc = (CCAMDoc*)m_pDoc ; // nt add 2010/9/9
		CViewTree* pT = pDoc->GetTree() ; // nt add 2010/9/9
		if( pVO->IsPicked() == 0 ) // 先在已选中元素中找,判pSE是否已经选中
		{
			pVO->SetPick(1) ;
			pVO->Draw2(m_pVI, dmSelect) ;
			// nt add 2010/9/9-------------------------------------------
			int type = pVO->GetType() ;
			if( type < 80 )
			{
				CFeature* pF = (CFeature*)pVO ;
				HTREEITEM hItem = pF->GetItem() ;
				pT->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED) ;
				pT->Invalidate(TRUE) ; // nt add 2010/9/9
			}
			else
			if( type == 80 ) // nt add 2010/11/29
			{
				CStep* pStep = (CStep*)pVO ;
				HTREEITEM hItem = pStep->GetItem() ;
				pT->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED) ;
				pT->Invalidate(TRUE) ;
			}
			// end
			m_selecteds.Add(pVO) ;
			return 1 ;
		}
		else
		{
			int i, n = m_selecteds.GetSize() ;
			for( i = 0 ; i < n ; i++ )
			{
				if( ((CVO*)m_selecteds[i]) == pVO )
				{
					pVO->SetPick(0) ;
					pVO->Draw2(m_pVI, dmNormal) ;
					// nt add 2010/9/9-----------------------------------
					int type = pVO->GetType() ;
					if( type < 80 )
					{
						CFeature* pF = (CFeature*)pVO ;
						HTREEITEM hItem = pF->GetItem() ;
						pT->SetItemState(hItem, 0, TVIS_SELECTED) ;
						pT->Invalidate(TRUE) ; // nt add 2010/9/9
					}
					else
					if( type == 80 )
					{
						CStep* pStep = (CStep*)pVO ;
						HTREEITEM hItem = pStep->GetItem() ;
						pT->SetItemState(hItem, 0, TVIS_SELECTED) ;
						pT->Invalidate(TRUE) ;
					}
					// end
					m_selecteds.RemoveAt2(i) ;
					return 0 ;
				}
			}
		}
	}

	return 0 ;
}

// nt add 2010/4/24
int CSelSet::AddSelected2(CVO* pVO)
{
	if( pVO )
	{
		if( pVO->IsPicked() == 0 ) // 先在已选中元素中找,判pSE是否已经选中
		{
			pVO->SetPick(1) ;
			pVO->Draw2(m_pVI, dmSelect) ;
			// nt add 2010/9/9-------------------------------------------
			int type = pVO->GetType() ;
			if( type < 80 )
			{
				CFeature* pF = (CFeature*)pVO ;
				HTREEITEM hItem = pF->GetItem() ;
				CCAMDoc* pDoc = (CCAMDoc*)m_pDoc ;
				CViewTree* pT = pDoc->GetTree() ;
				pT->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED) ;
				pT->Invalidate(TRUE) ;
			}
			else
			if( type == 80 ) // nt add 2010/11/29
			{
				CStep* pStep = (CStep*)pVO ;
				HTREEITEM hItem = pStep->GetItem() ;
				CCAMDoc* pDoc = (CCAMDoc*)m_pDoc ;
				CViewTree* pT = pDoc->GetTree() ;
				pT->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED) ;
				pT->Invalidate(TRUE) ;
			}
			m_selecteds.Add(pVO) ;
			return 1 ;
		}
	}

	return 0 ;
}

// nt add 2009/11/12
void CSelSet::SetPP(int flag)
{
	m_ppf = flag ;
}
//==============================================================
// nt add 2010/4/15 : 解决CCS和CPic可能遮挡CPLine等问题
// nt add 2009/12/17 : 所有需要拾取的ENTITY的类型信息
static int etTypes[MAX_NUM_SE] = {ftRefPt, 
								  ftRefCur, 
								  ftRefPlane,
								  ftRefSur,
								  ftPic,
								  ftTriangle, // nt add 2010/11/14
								  ftMark, // nt add 2011/4/20
								  STLTRI, // nt add 2022/6/15
								  MPOINT,
								  camStep} ;
/*	1997/4/14 nt
 *	当鼠标移动时,动态提示可被选择的实体
 *	nt modify 2010/4/19, 被提示的entity也可能在选择集里面!!!
 */
int CSelSet::OnMouseMove(int nFlags, 
						 double p[3], 
						 double v[3], 
						 void* pVI)
{
	int i /*etTypes[ENTITY_NUM],*/, rt_value ;
    PINFO pi ;
	CPart* pPart = NULL ;
	CMill* pMill = NULL ; // nt add 2010/6/2
    
	// nt move to here 2003/9/25
	if( m_draw )
		DrawBox() ;

    if( !m_pDoc )
		return 0 ;
	if( !m_enabled )
		return 1 ;
	if( (nFlags & MK_LBUTTON) && // only draw box dynamically
		m_p1[0] > -1.0e10 &&
		m_p1[1] > -1.0e10 &&
		m_p1[2] > -1.0e10 )
	{
		m_p2[0] = p[0] ;
		m_p2[1] = p[1] ;
		m_p2[2] = p[2] ;
		DrawBox() ;
		return 1 ;
	}
	pPart = ((CCAMDoc*)m_pDoc)->GetPart() ;
	pMill = ((CCAMDoc*)m_pDoc)->GetMill() ;

	// for each entity type, pick the entities in the m_pDoc model
	pi.pos[0] = p[0] ;
	pi.pos[1] = p[1] ;
	pi.pos[2] = p[2] ;
	pi.dir[0] = v[0] ;
	pi.dir[1] = v[1] ;
	pi.dir[2] = v[2] ;
	pi.obj = NULL ;
	pi.ot = 0 ;
	pi.u = pi.v = 0.0 ;
	pi.z = 1.0e20 ;
	pi.r = 2.5*viGetScale(pVI) ;
	for( i = 0 ; i < MAX_NUM_SE ; i++ )
	{
		if( m_pickFlags[etTypes[i]] )
		{
			pi.type = etTypes[i] ;
			if( etTypes[i] < 80 && // nt modify 2010/8/24, old=1000
				pPart )
			{
				if( (rt_value = pPart->Pick(&pi)) == 0 ) // 设计元素
					return 0 ;
				if( rt_value == 1 ) // nt add 2003/7/22
					break ;
			}
			else
			if( etTypes[i] >= 80 && // nt modify 2010/8/24, old=1000
				pMill )
			{
				if( (rt_value = pMill->Pick(&pi)) == 0 ) // 加工元素
					return 0 ;
				if( rt_value == 1 )
					break ;
			}
		}
    }

	// after picking, process the prompting entity
	CVO* pVO = (CVO*)pi.obj ;
	if( pVO )
	{
		if( pVO == m_prompting )
			return 1 ;
		RemovePrompting() ; // nt add 2010/4/18
		m_prompting = pVO ;
		m_prompting->Draw2(m_pVI, dmPrompt) ;
		// nt add 2010/12/24
		CStr str ;
		if( m_ppf && // nt add 2009/11/12
			pi.ot < 60 ) // nt modify 2022/6/15 old 80
			((CFeature*)pi.obj)->Prompt(str) ; // nt add 2002/12/5
		camPromptStr(str) ;
		// end
    }
	else
		RemovePrompting() ;

	// nt add 2010/4/15
	glFlush() ;

    return 1 ;
}

/*	1997/4/14 nt
 *	当鼠标左键按下时,调此函数确认实体选择
 *	nt modify 2010/4/18
 */
int CSelSet::OnLButtonDown(int nFlags, 
						   double p[3], 
						   double v[3], 
						   void* pVI)
{
	if( !m_enabled )
		return 1 ;

	// nt add 2002/12/5
	if( m_draw )
		DrawBox() ;
	m_p1[0] = p[0] ;
	m_p1[1] = p[1] ;
	m_p1[2] = p[2] ;

	// nt add 1998/4/8 按下CTRL键后,选择集中的元素只增不减
	if( !(nFlags & MK_CONTROL) )
		RemoveSelected() ;

	if( m_prompting ) // 若有实体被提示可选择
	{
		AddSelected(m_prompting) ;
		m_prompting = NULL ;
	}

	// nt add 2010/4/15
	glFlush() ;

	return 1 ;
}

// nt add 2002/12/5
// nt modify 2010/4/18
int CSelSet::OnLButtonUp(int nFlags, 
						 double p[3], 
						 double v[3], 
						 void* pVI)
{
	// nt add 2003/1/6
	if( m_p1[0] < -1.0e10 ||
		m_p1[1] < -1.0e10 ||
		m_p1[2] < -1.0e10 )
		return 1 ;

	if( !pVI ) // nt add 2002/12/6
		return 0 ;

	// nt add 2002/12/6
	if( m_draw )
		DrawBox() ;

	// record p
	m_p2[0] = p[0] ;
	m_p2[1] = p[1] ;
	m_p2[2] = p[2] ;

	// add codes of pick by box
	double r = 2.5*viGetScale(pVI) ;
	if( mathDist(m_p1, m_p2) > r )
	{
		RemovePrompting() ; // nt move to here, 2010/4/18
		CPart* pPart = ((CCAMDoc*)m_pDoc)->GetPart() ;
		if( !pPart )
			return 1 ;
		RFRAME lf ;
		viGetCenterLF(pVI, lf.X, lf.Y, lf.Z, lf.O) ;
		lf.scale = 1. ;
		PNT3D p1, p2 ;
		mathTransWorldPnt3D(&lf, m_p1, p1) ; // nt add 2010/4/24
		mathTransWorldPnt3D(&lf, m_p2, p2) ; // nt add 2010/4/24
		BOX2D box ;
		box.min[0] = mathMIN(p1[0], p2[0]) ; // define box
		box.min[1] = mathMIN(p1[1], p2[1]) ;
		box.max[0] = mathMAX(p1[0], p2[0]) ;
		box.max[1] = mathMAX(p1[1], p2[1]) ;
		pPart->PickByBox(&lf, &box, r, m_es) ;
		int i, n = m_es.GetSize() ;
		if( n > 0 )
		{	
			for( i = 0 ; i < n ; i++ )
			{
				CVO* pVO = (CVO*)m_es[i] ;
				if( pVO )
				{
					if( nFlags&MK_CONTROL )
						AddSelected2(pVO) ; // nt add 2010/4/24
					else
						AddSelected(pVO) ;
				}
			}
			m_es.RemoveAll() ;
		}
	}
	glFlush() ; // nt add 2010/4/15
	m_p1[0] = -1.0e20 ;
	m_p1[1] = -1.0e20 ;
	m_p1[2] = -1.0e20 ;
	m_p2[0] = -1.0e20 ;
	m_p2[1] = -1.0e20 ;
	m_p2[2] = -1.0e20 ;

	return 1 ;
}

/*	1998/4/16 nt
 *	当鼠标右键按下时,调此函数确认实体选择
 */
int CSelSet::OnRButtonDown(int nFlags, double p[3], double v[3], void* pVI)
{
	if( !pVI ) // nt add 2002/12/6
		return 0 ;

	return 1 ;
}

int CSelSet::OnLButtonDbClick(int nFlags, double p[3], double v[3], void* pVI)
{
	if( !pVI ) // nt add 2002/12/6
		return 0 ;

	return 1;
}
//==============================================================
void CSelSet::Draw()
{
	int i, n = m_selecteds.GetSize() ;
	CVO* pVO ;

    for( i = 0 ; i < n ; i++ )
	{
        pVO = (CVO*)m_selecteds[i] ;
		if( pVO )
			pVO->Draw2(m_pVI, dmSelect) ;
	}

	// nt add 2010/4/15
	glFlush() ;

	return ;
}

// nt modify 2010/2/2
void CSelSet::DrawBox()
{
	// use opengl
	glLineWidth(1.f) ; // nt add 2021/10/2
	glColor3ub(255, 0, 0) ;
	glEnable(GL_COLOR_LOGIC_OP) ;
	glLogicOp(GL_XOR) ;
	double X[3], Y[3], Z[3] ;
	double O[3], c1[3], c2[3] ;
	viGetCenterLF(m_pVI, X, Y, Z, O) ;
	viPrjPntLin(m_p2, m_p1, X, c1) ;
	viPrjPntLin(m_p2, m_p1, Y, c2) ;
	glBegin(GL_LINE_LOOP) ;
		glVertex3dv(m_p1) ;
		glVertex3dv(c1) ;
		glVertex3dv(m_p2) ;
		glVertex3dv(c2) ;
	glEnd() ;
	glDisable(GL_COLOR_LOGIC_OP) ;
	glFlush() ; // 必须加

	//
	if( m_draw )
		m_draw = 0 ;
	else
		m_draw = 1 ;

	return ;
}
////////////////////////////////////////////////////////////////