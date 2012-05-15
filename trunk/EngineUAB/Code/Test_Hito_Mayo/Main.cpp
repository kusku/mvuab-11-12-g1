#include "Main.h"

#include "Debug Scene\Scene.h"

#include "Cameras\Camera.h"
#include "Cameras\FPSCamera.h"
#include "Cameras\ThPSCamera.h"

#include "Characters\CharacterManager.h"
#include "Characters\Properties.h"
#include "Characters\Player.h"

#include "StatesMachine\EntityManager.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"


// --- VARIABLES LOCALES ---

// -------------------------


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CMain::CMain(void)
{
	m_pCharactersManager	= new CCharactersManager( );
	m_pScene				= new CScene ();
}


CMain::~CMain(void)
{
	CHECKED_DELETE ( m_pScene );
	CHECKED_DELETE ( m_pCharactersManager );
}


// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CMain::Init( void )
{
	CTestProcess::Init();

	// Creamos la escena de debug. Esto solo si no se tiene escenario
	//m_pScene->Init();

	// Inicializa el gestor de player y enemigos. Carga propiedades y estados de todo.
	if ( !m_pCharactersManager->Initialize ( ) )
		return false;

	// Inicializamos el player, sus estados, mayas animadas...
	m_pCharactersManager->GetPlayer()->Init();

	// Inicializamos la parte de lógica. Registramos la entidad como entidad que se le permite enviar/recibir mensajes de cambios de estado
	assert ( ENTMGR != NULL );
	//ENTMGR->RegisterEntity( m_pPlayer );
	
	// La primera Player/Enemy-Camera. Esto podria ser útil si conseguimos cambiar la cámara de un player a un enemico en ciertos momentos. Lo usé y molaba!
	m_uiIndicePlayerCamera = 0;

	// Inicializamos las cámaras y se asocian al Player
	float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pCameraTerceraPersona = new CThPSCamera (1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_pCharactersManager->GetPlayer()->GetProperties(), 4.0f );
	m_pCameraPrimeraPersona = new CFPSCamera ( 0.f , 1.f ,  D3DX_PI / 4, static_cast <float> ( m_VectScreen.x/m_VectScreen.y), m_pCharactersManager->GetPlayer()->GetProperties() );
	m_pCamera = static_cast<CCamera*>( m_pCameraTerceraPersona );

	// Asignamos la camera activa al core
	CORE->SetCamera( m_pCamera );
	
	return true;
}

void CMain::Update( float _ElapsedTime )
{
	CTestProcess::Update( _ElapsedTime );
	
	m_pCharactersManager->Update ( _ElapsedTime );

	UpdateInputs( _ElapsedTime );
}

void CMain::Render( CRenderManager *_RM )
{
	CTestProcess::Render ( _RM );
	
	m_pScene->Render  ( _RM );
}

void CMain::UpdateInputs ( float _ElapsedTime )
{
	CTestProcess::UpdateInputs( _ElapsedTime );
}



