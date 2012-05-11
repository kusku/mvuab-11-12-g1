#include "GameProcess.h"
#include "RenderManager.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "ActionToInput.h"
#include "Core.h"
#include "Base.h"
#include "TestDefs.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CGameProcess::CGameProcess(HWND hWnd)
	: m_pThPSCamera(NULL)
	, m_hWnd(hWnd)
{
}

CGameProcess::~CGameProcess()
{
	CHECKED_DELETE( m_pThPSCamera );
	m_pCamera = NULL;
}

bool CGameProcess::Init()
{
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

void CGameProcess::Update(float _ElapsedTime)
{
	CORE->SetCamera( m_pCamera );

	m_Player.Update( _ElapsedTime, m_pCamera );
	CORE->GetRenderableObjectsLayersManager()->Update( _ElapsedTime );

	if( CORE->GetActionToInput()->DoAction("ChangeRenderCommands") )
	{
		CORE->SetGameMode( !CORE->IsGameMode() );
		PostMessage(m_hWnd, WM_GUI_PROCESS, 0, 0);
	}
}

void CGameProcess::Render(CRenderManager &_RM)
{
}