#include "stdafx.h"
#include "GUIEditorProcess.h"
#include "RenderManager.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "GuiElement.h"
#include "defines.h"
#include "Base.h"
#include "Core.h"
#include "Fonts\FontManager.h"
#include "HWNDManager.h"
#include "Elements\ElementSaver.h"
#include "Elements\ElementProperties.h"
#include "Elements\ElementManager.h"
#include "InputManager.h"

CGUIEditorProcess::CGUIEditorProcess(void)
	: m_pGUIManager(NULL)
{	
}


CGUIEditorProcess::~CGUIEditorProcess(void)
{
	CHECKED_DELETE(m_pGUIManager);
}

void CGUIEditorProcess::Init()
{
	std::string window = CORE->GetGUIManager()->GetCurrentWindow();
	CElementProperties::WindowProperties( CORE->GetGUIManager()->GetWindow( window ) );
}

void CGUIEditorProcess::Update(float elapsedTime)
{
	bool l_bIsElementSelected = false;

	CORE->GetInputManager()->SetActiveMouse( false );

	HWND hWnd = CHWNDManager::GetInstance()->GetHWNDOutput();
	PostMessage( hWnd, WM_GETLOGGER, 0, 0);

	std::string path = CElementManager::GetInstance()->GetPathToInit();
	if( path != "")
	{
		std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
		CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);

		uint32 count = window->GetNumElements();
		for( uint32 i=0; i<count; ++i)
		{
			window->GetElementById(i)->SetIsSelected(false);
		}
		CElementProperties::WindowProperties(window);

		CORE->GetGUIManager()->Done();
		CORE->GetGUIManager()->Init( std::string(path) );
		CElementManager::GetInstance()->SetPathToInit("");

	}

	std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);

	CGuiElement *element = NULL;

	uint32 count = window->GetNumElements();
	for( uint32 i=0; i<count; ++i)
	{
		element = window->GetElementById(i);
		if( element != NULL )
		{
			if( element->GetIsSelected() )
			{
				CElementSaver::SaveProperties(element);	
				l_bIsElementSelected = true;
			}
		}
	}

	if( !l_bIsElementSelected )
	{
		CElementSaver::SaveWindowProperties( window );
	}

	//Calcula el mouse sobre los controles
	for( uint32 i = 0; i < count; ++i)
	{
		element = window->GetElementById(i);
		Vect2i mousePosition;
		CORE->GetInputManager()->GetPosition(IDV_MOUSE, mousePosition);
		element->CalculatePosMouse(mousePosition);
	}

}

void CGUIEditorProcess::Render(CRenderManager &RM)
{
}