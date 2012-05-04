#include "TestGUIProcess.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"
#include "Commands\SceneRendererCommandManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "ActionToInput.h"
#include "Scripting\ScriptManager.h"
#include "Console\Console.h"
#include "Modifiers\ModifierManager.h"
#include "DebugOptions\DebugOptions.h"
#include "DebugGUIManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTestGUIProcess::CTestGUIProcess( void )
	: m_pThPSCamera(NULL)
	, pos(0,0)
	, screen(800,600)
{
}

CTestGUIProcess::~CTestGUIProcess( void )
{
	CHECKED_DELETE( m_pThPSCamera );
	m_pCamera = NULL;
}

void CTestGUIProcess::Done ( void )
{
	
}

void CTestGUIProcess::Release ( void )
{
	
}

bool CTestGUIProcess::Init( void )
{
	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_Player.SetPosition(Vect3f(0.f,1.f,0.f));
	m_Player.SetPitch(0.0f);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f);
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);

	return true;
}

void CTestGUIProcess::Update( float _ElapsedTime )
{
	CORE->SetCamera( m_pCamera );
	m_Player.Update( _ElapsedTime, m_pCamera );
	//UpdateInputs( _ElapsedTime );

	CORE->GetRenderableObjectsLayersManager()->Update( _ElapsedTime );
}

void CTestGUIProcess::Render( CRenderManager &_RM )
{
}

//void CTestGUIProcess::UpdateInputs( float _ElapsedTime )
//{
//	CActionToInput *action2Input = CORE->GetActionToInput();
//	CScriptManager *SCRIPT = CORE->GetScriptManager();
//
//	if( action2Input->DoAction("Console") )
//	{
//		SCRIPT->RunCode("toggle_console()");
//	}
//
//	if( action2Input->DoAction("ReloadTTFs") )
//	{
//		SCRIPT->RunCode("reload_fonts()");
//	}
//
//	if( action2Input->DoAction("ReloadLanguageXMLs") )
//	{
//		SCRIPT->RunCode("reload_languages()");
//	}
//
//	if( action2Input->DoAction("ReloadActions") )
//	{
//		SCRIPT->RunCode("reload_inputs()");
//	}
//
//	if( action2Input->DoAction("ReloadRenderableObjects") )
//	{
//		SCRIPT->RunCode("reload_renderable_objects_layers()");
//	}
//	
//	if( action2Input->DoAction("ReloadMeshes") )
//	{
//		SCRIPT->RunCode("reload_meshes()");
//	}
//
//	if( action2Input->DoAction("ReloadShaders") )
//	{
//		SCRIPT->RunCode("reload_effects()");
//	}
//
//	if( action2Input->DoAction("ReloadPools") )
//	{
//		SCRIPT->RunCode("reload_pools()");
//	}
//
//	if( action2Input->DoAction("ReloadScripts") )
//	{
//		SCRIPT->RunCode("reload_scripts()");
//	}
//
//	if( action2Input->DoAction("ReloadCommands") )
//	{
//		SCRIPT->RunCode("reload_render_commands()");
//	}
//
//	UpdateDebugInputs( _ElapsedTime, *action2Input );
//}
//
//void CTestGUIProcess::UpdateDebugInputs(float elapsedTime, CActionToInput &action2Input)
//{
//	CModifierManager *l_pModifierManager = CORE->GetDebugGUIManager()->GetModifierManager();
//	CDebugOptions *l_DebugOptions = CORE->GetDebugGUIManager()->GetDebugOptions();
//
//	//Show & Unshow de debuggers
//	if( action2Input.DoAction("DebugOptions") )
//	{
//		l_DebugOptions->SetActive( !l_DebugOptions->GetActive() );
//	}
//
//	if( action2Input.DoAction("ModifiersShow") )
//	{
//		bool visible = l_pModifierManager->GetVisible();
//		l_pModifierManager->SetVisible( !visible );
//	}
//
//	//Modifiers actions
//	if( !CORE->GetDebugGUIManager()->GetConsole()->IsActive() && !l_DebugOptions->GetActive() )
//	{
//		if( action2Input.DoAction("Modifier_Previous") && !action2Input.DoAction("LogRender_PrevLine") )
//		{
//			l_pModifierManager->MoveToPreviousModifier();
//		}
//
//		if( action2Input.DoAction("Modifier_Next") && !action2Input.DoAction("LogRender_NextLine") )
//		{
//			l_pModifierManager->MoveToNextModifier();
//		}
//
//		if( action2Input.DoAction("GoToModifier") )
//		{
//			l_pModifierManager->GoToModifier();
//		}
//
//		if( action2Input.DoAction("GoToRootModifier") )
//		{
//			l_pModifierManager->GoToRoot();
//		}
//
//		if( action2Input.DoAction("AddValueToModifierByPass") || action2Input.DoAction("AddValueToModifier") )
//		{
//			l_pModifierManager->AddValueToModifier();
//		}
//
//		if( action2Input.DoAction("SubsValueToModifierByPass") || action2Input.DoAction("SubsValueToModifier") )
//		{
//			l_pModifierManager->SubsValueToModifier();
//		}
//	}
//
//	//Debug Options actions
//	if( l_DebugOptions->GetActive() )
//	{
//		if( action2Input.DoAction("NextPage") )
//		{
//			l_DebugOptions->MoveToNextPage();
//		}
//
//		if( action2Input.DoAction("PrevPage") )
//		{
//			l_DebugOptions->MoveToPrevPage();
//		}
//
//		if( action2Input.DoAction("NextLine") )
//		{
//			l_DebugOptions->MoveToNextLine();
//		}
//
//		if( action2Input.DoAction("PrevLine") )
//		{
//			l_DebugOptions->MoveToPrevLine();
//		}
//
//		if( action2Input.DoAction("DoAction") )
//		{
//			l_DebugOptions->DoAction();
//		}
//	}
//}