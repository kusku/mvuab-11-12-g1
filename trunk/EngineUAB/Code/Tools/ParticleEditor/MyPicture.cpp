// MyPicture.cpp : implementation file
//

#include "stdafx.h"
#include "MyPicture.h"

#ifndef SHARED_HANDLERS
#include "ParticleEditor.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPicture
CMyPicture::CMyPicture()
	: m_R(15.f)
	, m_bIsLMouseDown(false)
	, m_LButtonPosition(Vect2i(0,0))
	, m_ContextMenuPosition(Vect2i(0,0))
{
}

CMyPicture::~CMyPicture()
{
}


BEGIN_MESSAGE_MAP(CMyPicture, CStatic)
	//{{AFX_MSG_MAP(CMyPicture)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPicture message handlers

void CMyPicture::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CStatic::OnShowWindow(bShow, nStatus);
	
	//place my code here
	
}

void CMyPicture::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnLButtonDown(nFlags, point);
}

void CMyPicture::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonUp(nFlags, point);
}

void CMyPicture::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnRButtonDown(nFlags, point);
}

void CMyPicture::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CStatic::OnRButtonUp(nFlags, point);
}

BOOL CMyPicture::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_R=m_R+zDelta/2;
	if (m_R<1) m_R=1;

	return CStatic::OnMouseWheel(nFlags,zDelta, pt);
}

void CMyPicture::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);
}

void CMyPicture::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	m_ContextMenuPosition = Vect2i(point.x, point.y);
}