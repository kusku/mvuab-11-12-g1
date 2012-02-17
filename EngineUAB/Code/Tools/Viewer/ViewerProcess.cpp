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

	m_Player.SetPosition(Vect3f(0.f,1.f,0.f));
	m_Player.SetPitch(0.0f);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f);
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
	if( action2Input->DoAction("ReloadTTFs") )
	{
		CORE->ReloadTTFs();
	}

	if( action2Input->DoAction("ReloadLanguageXMLs") )
	{
		CORE->ReloadLanguages();
	}

	if( action2Input->DoAction("ReloadActions") )
	{
		CORE->ReloadInputs();
	}

	if( action2Input->DoAction("ReloadRenderableObjects") )
	{
		CORE->ReloadRenderableObjectsLayersManager();
	}
	
	if( action2Input->DoAction("ReloadMeshes") )
	{
		CORE->ReloadMeshes();
	}

	if( action2Input->DoAction("ReloadShaders") )
	{
		CORE->ReloadEffects();
	}

	if( action2Input->DoAction("ReloadPools") )
	{
		CORE->ReloadPools();
	}

	if( action2Input->DoAction("ReloadCommands") )
	{
		CORE->ReloadSceneRendererCommandManager();
	}

	if( action2Input->DoAction("ModifiersShow") )
	{
		bool visible = CORE->GetModifierManager()->GetVisible();
		CORE->GetModifierManager()->SetVisible( !visible );
	}

	if( action2Input->DoAction("Modifier_Previous") && !action2Input->DoAction("LogRender_PrevLine") )
	{
		CORE->GetModifierManager()->MoveToPreviousModifier();
	}

	if( action2Input->DoAction("Modifier_Next") && !action2Input->DoAction("LogRender_NextLine") )
	{
		CORE->GetModifierManager()->MoveToNextModifier();
	}

	if( action2Input->DoAction("GoToModifier") )
	{
		CORE->GetModifierManager()->GoToModifier();
	}

	if( action2Input->DoAction("GoToRootModifier") )
	{
		CORE->GetModifierManager()->GoToRoot();
	}

	if( action2Input->DoAction("AddValueToModifierByPass") || action2Input->DoAction("AddValueToModifier") )
	{
		CORE->GetModifierManager()->AddValueToModifier();
	}

	if( action2Input->DoAction("SubsValueToModifierByPass") || action2Input->DoAction("SubsValueToModifier") )
	{
		CORE->GetModifierManager()->SubsValueToModifier();
	}
}

void CViewerProcess::Render(CRenderManager *RM)
{
	CORE->GetSceneRendererCommandManager()->Execute(*RM);
}