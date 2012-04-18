#include "RenderDebugInfoSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Core.h"
#include "Base.h"
#include "Fonts\FontManager.h"
#include "Utils\Timer.h"
#include "DebugGUIManager.h"
#include "DebugOptions\DebugOptions.h"
#include "Math\Matrix44.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderDebugInfoSceneRendererCommand::CRenderDebugInfoSceneRendererCommand(CXMLTreeNode &Node)
{
	m_Grid = Node.GetBoolProperty("grid", false);
	m_Axis = Node.GetBoolProperty("axis", false);
	m_bRenderConsole = Node.GetBoolProperty("render_console", false);
	m_bRenderDebugOptions = Node.GetBoolProperty("render_debug_options", false);
	m_bRenderDebugRender = Node.GetBoolProperty("render_debug_render", false);
	m_bRenderLogger = Node.GetBoolProperty("render_logger", false);
	m_bRenderModifiers = Node.GetBoolProperty("render_modifiers", false);
}

void CRenderDebugInfoSceneRendererCommand ::Execute(CRenderManager &RM)
{	
	CFontManager *fontManager = CORE->GetFontManager();
	CDebugGUIManager *l_pGUI = CORE->GetDebugGUIManager();

	l_pGUI->SetRenderConsole( m_bRenderConsole );
	l_pGUI->SetRenderDebugOptions( m_bRenderDebugOptions );
	l_pGUI->SetRenderDebugRender( m_bRenderDebugRender );
	l_pGUI->SetRenderLogger( m_bRenderLogger );
	l_pGUI->SetRenderModifiers( m_bRenderModifiers );

	l_pGUI->Render(RM, *fontManager, CORE->GetTimer() );

	Mat44f mat;
	mat.SetIdentity();
	RM.SetTransform(mat);

	if( m_Grid )
		RM.DrawGrid(100.f, 100.f, 30, colBLACK);

	if( m_Axis )
		RM.DrawAxis(1.0f);
}
