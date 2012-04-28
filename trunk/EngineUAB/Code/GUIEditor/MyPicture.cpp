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
#include "Elements\ElementProperties.h"
#include "Base.h"
#include "Core.h"

#ifndef SHARED_HANDLERS
#include "GUIEditor.h"
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
	ON_COMMAND(ID_TRAER_FRENTE, OnMoveToFront)
	ON_COMMAND(ID_TRAER_ADELANTE, OnMoveForward)
	ON_COMMAND(ID_ENVIAR_ATRAS, OnMoveBack)
	ON_COMMAND(ID_ENVIAR_FONDO, OnMoveToBack)
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
	m_bIsLMouseDown = true;
	m_LButtonPosition = Vect2i(point.x, point.y);
	CStatic::OnLButtonDown(nFlags, point);
}

void CMyPicture::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_bIsLMouseDown )
	{
		//Se selecciona un elemento
		std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
		CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);
		CGuiElement *element = NULL;

		//hace reset de todos los controles
		uint32 count = window->GetNumElements();
		for( uint32 i = 0; i < count; ++i)
		{
			element = window->GetElementById(i);
			element->SetIsSelected(false);
		}

		//Mira sobre qué elemento está el mouse
		for( uint32 i = count; i > 0; --i)
		{
			element = window->GetElementById(i-1);
			Vect2i mousePosition;
			CORE->GetInputManager()->GetPosition(IDV_MOUSE, mousePosition);
			element->CalculatePosMouse(mousePosition);
			if( element->IsInside() )
			{
				element->SetIsSelected(true);
				CElementProperties::ElementProperties(element);
				break;
			}
		}
	}
	else
	{
		//Se hace Drag & drop
		CElementManager *l_pElementManager = CElementManager::GetInstance();

		TElement element = l_pElementManager->GetElementToAdd();
		std::string window = l_pElementManager->GetWindowToAdd();
		if( element != NONE )
		{
			l_pElementManager->SetElementToAdd(NONE);

			AddElementToActiveWindow(element);
		}
		else if( window != "" )
		{
			std::string current_window = CORE->GetGUIManager()->GetCurrentWindow();
			current_window = current_window;
			CGUIWindow *windowcurrent = CORE->GetGUIManager()->GetWindow( current_window );
			windowcurrent->ClearSelectElements();

			window = window;
			CORE->GetGUIManager()->ActiveWindows( window );

			CORE->GetGUIManager()->GetWindow( window )->ClearSelectElements();
			CElementProperties::WindowProperties( CORE->GetGUIManager()->GetWindow( window ) );
		}
	}

	m_bIsLMouseDown = false;
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

	if( m_bIsLMouseDown )
	{
		float fact_pan = 0.1f;
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CMyPicture::AddElementToActiveWindow(TElement element)
{
	CGUIManager *l_pGUIManager = CORE->GetGUIManager();

	std::string windowName = l_pGUIManager->GetCurrentWindow();
	CGUIWindow *l_pWindow = l_pGUIManager->GetWindow(windowName);

	CElementCreator::CreateElement(element, l_pWindow);
}

void CMyPicture::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	m_ContextMenuPosition = Vect2i(point.x, point.y);
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

void CMyPicture::OnMoveToFront()
{
	//Se selecciona un elemento
	std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);
	CGuiElement *element = NULL;

	//Mira sobre qué elemento está el mouse
	uint16 count = window->GetNumElements();
	for( uint16 i = count; i > 0; --i)
	{
		element = window->GetElementById(i-1);
		Vect2i mousePosition;
		CORE->GetInputManager()->GetPosition(IDV_MOUSE, mousePosition);
		element->CalculatePosMouse(mousePosition);
		if( element->IsInside() )
		{
			//Está adentro
			window->MoveElementToFront(element);
			break;
		}
	}
}

void CMyPicture::OnMoveForward()
{
	//Se selecciona un elemento
	std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);
	CGuiElement *element = NULL;

	//Mira sobre qué elemento está el mouse
	uint16 count = window->GetNumElements();
	for( uint16 i = count; i > 0; --i)
	{
		element = window->GetElementById(i-1);
		Vect2i mousePosition;
		CORE->GetInputManager()->GetPosition(IDV_MOUSE, mousePosition);
		element->CalculatePosMouse(mousePosition);
		if( element->IsInside() )
		{
			//Está adentro
			window->MoveElementForward(element);
			break;
		}
	}
}

void CMyPicture::OnMoveBack()
{
	//Se selecciona un elemento
	std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);
	CGuiElement *element = NULL;

	//Mira sobre qué elemento está el mouse
	uint16 count = window->GetNumElements();
	for( uint16 i = count; i > 0; --i)
	{
		element = window->GetElementById(i-1);
		Vect2i mousePosition;
		CORE->GetInputManager()->GetPosition(IDV_MOUSE, mousePosition);
		element->CalculatePosMouse(mousePosition);
		if( element->IsInside() )
		{
			//Está adentro
			window->MoveElementBack(element);
			break;
		}
	}
}

void CMyPicture::OnMoveToBack()
{
	//Se selecciona un elemento
	std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);
	CGuiElement *element = NULL;

	//Mira sobre qué elemento está el mouse
	uint16 count = window->GetNumElements();
	for( uint16 i = count; i > 0; --i)
	{
		element = window->GetElementById(i-1);
		Vect2i mousePosition;
		CORE->GetInputManager()->GetPosition(IDV_MOUSE, mousePosition);
		element->CalculatePosMouse(mousePosition);
		if( element->IsInside() )
		{
			//Está adentro
			window->MoveElementToBack(element);
			break;
		}
	}
}