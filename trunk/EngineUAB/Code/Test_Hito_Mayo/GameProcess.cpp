#include "GameProcess.h"

#include "DebugScene\Scene.h"

#include "Cameras\Camera.h"
#include "Cameras\ThPSCamera.h"

#include "Characters\CharacterManager.h"

#include "ActionToInput.h"
#include "StatesMachine\EntityManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "Base.h"
#include "Core.h"

#include "Characters\PlayerDef.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// --- VARIABLES LOCALES ---

// -------------------------


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CGameProcess::CGameProcess(HWND hWnd)
	: m_pThPSCamera(NULL)
	, m_AudioIsFade(true)
	, m_uAudioID(0)
{
	m_pCharactersManager	= new CCharactersManager( );
	m_pScene				= new CScene ();
}


CGameProcess::~CGameProcess(void)
{
	CHECKED_DELETE( m_pScene );
	CHECKED_DELETE( m_pCharactersManager );
	CHECKED_DELETE( m_pThPSCamera );
	CHECKED_DELETE( m_pThPSFreeCamera );
}


// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CGameProcess::Init( void )
{
	CORE->GetSoundManager()->Stop();

	// Creamos la escena de debug. Esto solo si no se tiene escenario
	//m_pScene->Init();

	screen = CORE->GetRenderManager()->GetScreenSize();
	pos.x = screen.x / 2;
	pos.y = screen.y / 2;

	// Inicializa el gestor de player y enemigos. Carga propiedades y estados de todo.
	if ( !m_pCharactersManager->Initialize ( ) )
		return false;
	
	m_Player = m_pCharactersManager->GetPlayer();
	m_Player->SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_Player->SetPitch(-D3DX_PI/8);
	m_Player->SetYaw(0.0f);
	m_Player->SetRoll(0.0f);
	m_Player->Init();

	// Inicializamos la parte de lógica. Registramos la entidad como entidad que se le permite enviar/recibir mensajes de cambios de estado
	assert ( ENTMGR != NULL );
	//ENTMGR->RegisterEntity( m_pPlayer );
	
	float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_Player, 12.0f, 4.f, "Wolf");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	// Asignamos la camera activa al core
	CORE->SetCamera( m_pCamera );

	m_FreeCamera.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_FreeCamera.SetPitch(-D3DX_PI/6);
	m_FreeCamera.SetYaw(0.0f);
	m_FreeCamera.SetRoll(0.0f);

	m_pThPSFreeCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, &m_FreeCamera, 10.0f, 0.f, "Free");
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);

	m_Player->SetLockCamera( false );

	m_uAudioID = CORE->GetSoundManager()->CreateSource();
	CORE->GetSoundManager()->PlaySource3D(m_uAudioID, "background", true);
	CORE->GetSoundManager()->FadeOutSource(m_uAudioID, 0.001f);

	return true;
}

void CGameProcess::Update( float _ElapsedTime )
{	
	CORE->SetCamera( m_pCamera );
	
	m_pCharactersManager->Update ( _ElapsedTime );
		
	//m_Player->Update(_ElapsedTime, m_pCamera );
	if( m_Player->GetLockCamera() )
	{
		m_FreeCamera.Update(_ElapsedTime ,m_pCamera);
	}

	CORE->GetRenderableObjectsLayersManager()->Update(_ElapsedTime);

	UpdateInputs( _ElapsedTime );
}

void CGameProcess::Render( CRenderManager &RM )
{	
	//m_Player->Render(&RM);
}

void CGameProcess::UpdateInputs ( float _ElapsedTime )
{
	if( CORE->GetActionToInput()->DoAction("CommutacioCamera") )
	{
		if( m_pCamera == m_pThPSCamera )
		{
			m_pCamera = m_pThPSFreeCamera;
			CORE->SetCamera( m_pThPSFreeCamera );
			m_Player->SetLockCamera( true );
		}
		else
		{
			m_pCamera = m_pThPSCamera;
			CORE->SetCamera( m_pThPSCamera );
			m_Player->SetLockCamera( false );
		}
	}

	if( CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_FOWARD) || 
		CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_BACK) ||
		CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_LEFT) ||
		CORE->GetActionToInput()->DoAction(ACTION_MOVE_PLAYER_RIGHT))
	{
		if( m_AudioIsFade )
		{
			CORE->GetSoundManager()->FadeInSource(m_uAudioID, 1.f);
			m_AudioIsFade = false;
		}
	}
	else
	{
		if( !m_AudioIsFade )
		{
			CORE->GetSoundManager()->FadeOutSource(m_uAudioID, 1.f);
			m_AudioIsFade = true;
		}
	}
}