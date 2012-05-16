#include "Main.h"

#include "DebugScene\Scene.h"

#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"

#include "ActionToInput.h"
#include "StatesMachine\EntityManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// --- VARIABLES LOCALES ---

// -------------------------


CMain::CMain(void)
	: m_pThPSCamera(NULL)
{
	m_pScene	= new CScene ();
}


CMain::~CMain(void)
{
	CHECKED_DELETE ( m_pScene );
}


bool CMain::Init( void )
{
	CORE->GetScriptManager()->RunCode("load_basics()");
	CORE->GetScriptManager()->RunCode("load_data()");

	// Creamos la escena
	m_pScene->Init();

	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	m_Player.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_Player.SetPitch(-D3DX_PI/8);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);
	m_Player.Init();

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 12.0f, 4.f, "Wolf");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera( m_pCamera );

	m_FreeCamera.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_FreeCamera.SetPitch(-D3DX_PI/6);
	m_FreeCamera.SetYaw(0.0f);
	m_FreeCamera.SetRoll(0.0f);

	m_pThPSFreeCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_FreeCamera, 10.0f, 0.f, "Free");
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);

	m_Player.SetLockCamera( false );

	return true;
}

void CMain::Update( float _ElapsedTime )
{	
	CORE->SetCamera( m_pCamera );
	m_Player.Update(_ElapsedTime, m_pCamera );
	if( m_Player.GetLockCamera() )
	{
		m_FreeCamera.Update(_ElapsedTime ,m_pCamera);
	}

	CORE->GetRenderableObjectsLayersManager()->Update(_ElapsedTime);

	UpdateInputs( _ElapsedTime );
}

void CMain::Render( CRenderManager &RM )
{	
	m_Player.Render(&RM);
}

void CMain::UpdateInputs ( float _ElapsedTime )
{
	if( CORE->GetActionToInput()->DoAction("CommutacioCamera") )
	{
		if( m_pCamera == m_pThPSCamera )
		{
			m_pCamera = m_pThPSFreeCamera;
			CORE->SetCamera( m_pThPSFreeCamera );
			m_Player.SetLockCamera( true );
		}
		else
		{
			m_pCamera = m_pThPSCamera;
			CORE->SetCamera( m_pThPSCamera );
			m_Player.SetLockCamera( false );
		}
	}
}