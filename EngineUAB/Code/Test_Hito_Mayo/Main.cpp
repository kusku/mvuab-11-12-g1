#include "Main.h"

#include "Scene.h"
#include "Player.h"
#include "Logic.h"

#include "Cameras\Camera.h"
#include "Cameras\FPSCamera.h"
#include "Cameras\ThPSCamera.h"

#include "StatesMachine\EntityManager.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"


// --- VARIABLES LOCALES ---

// -------------------------


CMain::CMain(void)
{
	m_pLogic = new CLogic();

	m_pPlayer	= new CPlayer( "Jolete" );
	m_pScene	= new CScene ();
}


CMain::~CMain(void)
{
	CHECKED_DELETE ( m_pLogic );
	CHECKED_DELETE ( m_pPlayer );
	CHECKED_DELETE ( m_pScene );
}


bool CMain::Init( void )
{
	CTestProcess::Init();

	/*AddPlayer ( "Player", Vect3f ( -40.f, 20.f, 0.f ), false );
	AddPlayer ( "Pepe",   Vect3f ( 1.f, 2.f, 1.f ),    false );*/

	// Creamos la escena
	m_pScene->Init();

	// Creamos un player
	m_pPlayer->Init				( );
	m_pPlayer->SetPosition		( Vect3f ( -40.f, 20.f, 0.f ) );
	m_pPlayer->SetMoveAlone		( false );
	m_pPlayersList.push_back	( m_pPlayer );

	// Inicializamos la parte de lógica. Registramos la entidad como entidad que se le permite enviar/recibir mensajes de cambios de estado
	assert ( ENTMGR != NULL );
	ENTMGR->RegisterEntity( m_pPlayer );
	
	float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pCameraTerceraPersona = new CThPSCamera (1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_pPlayersList[m_uiIndicePlayerCamera], 10.0f );
	m_pCameraPrimeraPersona = new CFPSCamera ( 0.f , 1.f ,  D3DX_PI / 4, static_cast <float> ( m_VectScreen.x/m_VectScreen.y), m_pPlayersList[m_uiIndicePlayerCamera] );
	m_pCamera = static_cast<CCamera*>( m_pCameraTerceraPersona );

	// Assignamos el player por defecto
	m_pActivePlayer = m_pPlayersList[m_uiIndicePlayerCamera];

	// Asignamos la camera activa al core
	CORE->SetCamera( m_pCamera );
	//
	// La primera Player-Camera
	m_uiIndicePlayerCamera = 0;

	
	return true;
}

void CMain::Update( float _ElapsedTime )
{
	CTestProcess::Update( _ElapsedTime );
	
	m_pPlayer->Update ( _ElapsedTime );

	UpdateInputs( _ElapsedTime );
}

void CMain::Render( CRenderManager *_RM )
{
	CTestProcess::Render ( _RM );
	
	m_pScene->Render  ( _RM );
	m_pPlayer->Render ( _RM );
}

void CMain::UpdateInputs ( float _ElapsedTime )
{
	CActionToInput *action2Input = CORE->GetActionToInput();

}



