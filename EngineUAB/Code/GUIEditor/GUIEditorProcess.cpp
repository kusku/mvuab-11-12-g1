#include "stdafx.h"
#include "GUIEditorProcess.h"
#include "RenderManager.h"
#include "GUIManager.h"
#include "defines.h"
#include "Base.h"
#include "Core.h"
#include "Fonts\FontManager.h"

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
	m_pGUIManager = new CGUIManager(Vect2i(640,480));
	m_pGUIManager->Init("./Data/XML/gui.xml");
}

void CGUIEditorProcess::Update(float elapsedTime)
{
	PostMessage( m_hWndOutput, WM_GETLOGGER, 0, 0);
	m_pGUIManager->Update(elapsedTime);
}

void CGUIEditorProcess::Render(CRenderManager &RM)
{
	m_pGUIManager->Render(&RM, CORE->GetFontManager());
}