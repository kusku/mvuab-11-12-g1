
#include "stdafx.h"
#include "ViewTree.h"
#include "defines.h"
#include "Elements\ElementManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Controladores de mensajes de CViewTree

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnLButtonDown(UINT nFlags,CPoint point)
{
	UINT flags = 0;
	CPoint ptTree = point;
	HTREEITEM hTreeItem = HitTest(ptTree, &flags);
	LRESULT hr = PostMessage(WM_LBUTTON_DOWN,(WPARAM)hTreeItem,0);
}

void CViewTree::OnLButtonUp(UINT nFlags,CPoint point)
{
	CElementManager::GetInstance()->SetElementToAdd(NONE);
	CElementManager::GetInstance()->SetWindowToAdd("");
}