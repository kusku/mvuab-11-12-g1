#include "ViewerProcess.h"
#include "RenderManager.h"
#include "ActionToInput.h"
#include "Cameras\ThPSCamera.h"
#include "Base.h"
#include "Core.h"
#include "Math\Color.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Math\Matrix44.h"
#include "Cameras\Camera.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Commands\SceneRendererCommandManager.h"
#include "Lights\LightManager.h"
#include "Scripting\ScriptManager.h"
#include "DebugOptions\DebugOptions.h"
#include "Console\Console.h"
#include "DebugGUIManager.h"

#include "ViewerDefs.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CViewerProcess::CViewerProcess()
	: pos(0,0)
	, screen(800,600)
	, yaw(0.0f)
	, m_pThPSCamera(NULL)	
{
}

CViewerProcess::~CViewerProcess()
{
	CHECKED_DELETE( m_pThPSCamera );
	m_Camera = NULL;
}

bool CViewerProcess::Init()
{
	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_Player.SetPosition(Vect3f( 220.f,150.f,-120.f));
	m_Player.SetPitch(-D3DX_PI/6);
	m_Player.SetYaw(D3DX_PI);
	m_Player.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f);
	m_Camera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_Camera);

	return true;
}

void CViewerProcess::Update(float elapsedTime)
{
	CORE->SetCamera(m_Camera);
	m_Player.Update(elapsedTime, m_Camera);

	CORE->GetRenderableObjectsLayersManager()->Update(elapsedTime);

#if defined(_DEBUG)
	UpdateInputs(elapsedTime);
#endif
}

void CViewerProcess::UpdateInputs(float elapsedTime)
{
	CActionToInput *action2Input = CORE->GetActionToInput();
	CScriptManager *SCRIPT = CORE->GetScriptManager();

	if( action2Input->DoAction( ACTION_CONSOLE ) )
	{
		SCRIPT->RunCode("toggle_console()");
	}

	if( action2Input->DoAction( ACTION_RELOAD_TTFS ) )
	{
		SCRIPT->RunCode("reload_fonts()");
	}

	if( action2Input->DoAction( ACTION_RELOAD_LANGUAGES ) )
	{
		SCRIPT->RunCode("reload_languages()");
	}

	if( action2Input->DoAction( ACTION_RELOAD_ACTIONS ) )
	{
		SCRIPT->RunCode("reload_inputs()");
	}
	
	if( action2Input->DoAction( ACTION_RELOAD_RENDERABLE_OBJECTS ) ) 
	{
		SCRIPT->RunCode("reload_renderable_objects_layers()");
	}
	
	if( action2Input->DoAction( ACTION_RELOAD_MESHES ) )
	{
		SCRIPT->RunCode("reload_meshes()");
	}

	if( action2Input->DoAction( ACTION_RELOAD_SHADERS ) )
	{
		SCRIPT->RunCode("reload_effects()");
	}

	if( action2Input->DoAction( ACTION_RELOAD_POOLS ) )
	{
		SCRIPT->RunCode("reload_pools()");
	}

	if( action2Input->DoAction( ACTION_RELOAD_SCRIPTS ) )
	{
		SCRIPT->RunCode("reload_scripts()");
	}

	if( action2Input->DoAction( ACTION_RELOAD_COMMANDS ) )
	{
		SCRIPT->RunCode("reload_render_commands()");
	}

	UpdateDebugInputs(elapsedTime, *action2Input);
}

void CViewerProcess::UpdateDebugInputs(float elapsedTime, CActionToInput &action2Input)
{
	CModifierManager *l_pModifierManager = CORE->GetDebugGUIManager()->GetModifierManager();
	CDebugOptions *l_DebugOptions = CORE->GetDebugGUIManager()->GetDebugOptions();

	//Show & Unshow de debuggers
	if( action2Input.DoAction("DebugOptions") )
	{
		l_DebugOptions->SetActive( !l_DebugOptions->GetActive() );
	}

	if( action2Input.DoAction("ModifiersShow") )
	{
		bool visible = l_pModifierManager->GetVisible();
		l_pModifierManager->SetVisible( !visible );
	}

	//Modifiers actions
	if( !CORE->GetDebugGUIManager()->GetConsole()->IsActive() && !l_DebugOptions->GetActive() )
	{
		if( action2Input.DoAction("Modifier_Previous") && !action2Input.DoAction("LogRender_PrevLine") )
		{
			l_pModifierManager->MoveToPreviousModifier();
		}

		if( action2Input.DoAction("Modifier_Next") && !action2Input.DoAction("LogRender_NextLine") )
		{
			l_pModifierManager->MoveToNextModifier();
		}

		if( action2Input.DoAction("GoToModifier") )
		{
			l_pModifierManager->GoToModifier();
		}

		if( action2Input.DoAction("GoToRootModifier") )
		{
			l_pModifierManager->GoToRoot();
		}

		if( action2Input.DoAction("AddValueToModifierByPass") || action2Input.DoAction("AddValueToModifier") )
		{
			l_pModifierManager->AddValueToModifier();
		}

		if( action2Input.DoAction("SubsValueToModifierByPass") || action2Input.DoAction("SubsValueToModifier") )
		{
			l_pModifierManager->SubsValueToModifier();
		}
	}

	//Debug Options actions
	if( l_DebugOptions->GetActive() )
	{
		
		if( action2Input.DoAction("NextPage") )
		{
			l_DebugOptions->MoveToNextPage();
		}

		if( action2Input.DoAction("PrevPage") )
		{
			l_DebugOptions->MoveToPrevPage();
		}

		if( action2Input.DoAction("NextLine") )
		{
			l_DebugOptions->MoveToNextLine();
		}

		if( action2Input.DoAction("PrevLine") )
		{
			l_DebugOptions->MoveToPrevLine();
		}

		if( action2Input.DoAction("DoAction") )
		{
			l_DebugOptions->DoAction();
		}
	}
}

void CViewerProcess::Render(CRenderManager &RM)
{
}