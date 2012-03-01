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
#include "ScriptManager.h"

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

void CViewerProcess::Init()
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
}

void CViewerProcess::Update(float elapsedTime)
{
	CORE->SetCamera(m_Camera);
	m_Player.Update(elapsedTime, m_Camera);
	UpdateInputs(elapsedTime);

	CORE->GetRenderableObjectsLayersManager()->Update(elapsedTime);
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

	if( action2Input->DoAction( ACTION_SHOW_MODIFIERS ) )
	{
		bool visible = CORE->GetModifierManager()->GetVisible();
		CORE->GetModifierManager()->SetVisible( !visible );
	}

	if( action2Input->DoAction( ACTION_PREVIOUS_MODIFIERS ) && !action2Input->DoAction("LogRender_PrevLine") )
	{
		CORE->GetModifierManager()->MoveToPreviousModifier();
	}

	if( action2Input->DoAction( ACTION_NEXT_MODIFIERS ) && !action2Input->DoAction("LogRender_NextLine") )
	{
		CORE->GetModifierManager()->MoveToNextModifier();
	}

	if( action2Input->DoAction( ACTION_GOTO_MODIFIERS ) )
	{
		CORE->GetModifierManager()->GoToModifier();
	}

	if( action2Input->DoAction( ACTION_GOTO_ROOT_MODIFIERS ) )
	{
		CORE->GetModifierManager()->GoToRoot();
	}

	if( action2Input->DoAction( ACTION_ADD_VALUE_MODIFIERS ) || action2Input->DoAction("AddValueToModifier") )
	{
		CORE->GetModifierManager()->AddValueToModifier();
	}

	if( action2Input->DoAction( ACTION_SUBS_VALUE_MODIFIERS ) || action2Input->DoAction("SubsValueToModifier") )
	{
		CORE->GetModifierManager()->SubsValueToModifier();
	}
}

void CViewerProcess::Render(CRenderManager *RM)
{
	CORE->GetSceneRendererCommandManager()->Execute(*RM);
}