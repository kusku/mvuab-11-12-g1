// MyPicture.cpp : implementation file
//

#include "stdafx.h"
#include "MyPicture.h"
#include "EngineManager.h"
#include "EngineProcess.h"
#include "Elements\ElementCreator.h"
#include "GUIEditorProcess.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "InputManager.h"
#include "Periphericals\Mouse.h"
#include "Base.h"
#include "Core.h"

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
	CElementManager *l_pElementManager = CElementManager::GetInstance();

	TElement element = l_pElementManager->GetElementToAdd();
	l_pElementManager->SetElementToAdd(NONE);

	AddElementToActiveWindow(element);

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
	CORE->GetInputManager()->GetMouse()->SetPosition( Vect2i(point.x, point.y) );

	CStatic::OnMouseMove(nFlags, point);
}

void CMyPicture::AddElementToActiveWindow(TElement element)
{
	CGUIManager *l_pGUIManager = ( (CGUIEditorProcess*)CEngineManager::GetInstance()->GetEngine()->GetProcess() )->GetGUIManager();

	std::string windowName = l_pGUIManager->GetCurrentWindow();
	CGUIWindow *l_pWindow = l_pGUIManager->GetWindow(windowName);

	switch( element )
	{
	case BUTTON:
		{
			CElementCreator::CreateButton(l_pWindow);
			break;
		}
	case IMAGE:
		{
			CElementCreator::CreateImage(l_pWindow);
			break;
		}
	}
}