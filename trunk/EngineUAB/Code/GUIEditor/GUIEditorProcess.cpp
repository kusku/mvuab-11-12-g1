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
}

void CGUIEditorProcess::Update(float elapsedTime)
{
	HWND hWnd = CHWNDManager::GetInstance()->GetHWNDOutput();
	PostMessage( hWnd, WM_GETLOGGER, 0, 0);

	std::string name_window = CORE->GetGUIManager()->GetCurrentWindow();
	CGUIWindow *window = CORE->GetGUIManager()->GetWindow(name_window);

	CGuiElement *element = CORE->GetGUIManager()->GetWindow( name_window )->GetCurrentSelectedElement();
	if( element != NULL )
	{
		CElementSaver::SaveProperties(element);	
	}
}

void CGUIEditorProcess::Render(CRenderManager &RM)
{
}