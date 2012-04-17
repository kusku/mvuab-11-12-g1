// MyPicture.cpp : implementation file
//

#include "stdafx.h"
#include "MyPicture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPicture
CMyPicture::CMyPicture()
	: m_R(15.f)
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
	////Posa el tipus de cursor corresponent
	//HCURSOR hCur; 
	//switch (m_Edicio)
	//{
	//case 0:
	//	{
	//		hCur  = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_POINTER)); 
	//		break;
	//	}
	//case 1:
	//	{
	//		hCur  = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_TRASLLADAR)); 
	//		break;
	//	}
	//case 2:
	//	{
	//		hCur  = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_ROTAR)); 
	//		break;
	//	}
	//case 3:
	//	{
	//		hCur  = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_ESCALAR)); 
	//		break;
	//	}
	//default:
	//	{
	//		hCur  = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_POINTER)); 
	//		break;
	//	}
	//}
	//SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)hCur); 

	CStatic::OnMouseMove(nFlags, point);
}