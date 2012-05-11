#include "TestProcess.h"

#include "Commands\SceneRendererCommandManager.h"
#include "Scripting\ScriptManager.h"
#include "ActionToInput.h"
#include "Base.h"
#include "Core.h"

#include "Cameras\Camera.h"
#include "Cameras\FPSCamera.h"
#include "Cameras\ThPSCamera.h"

#include "Player.h"

#include "RenderManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "Textures\TextureManager.h"
#include "Textures\Texture.h"

#include "Console\Console.h"
#include "Modifiers\ModifierManager.h"
#include "DebugOptions\DebugOptions.h"
#include "DebugGUIManager.h"

#include "Main.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// --- VARIABLES LOCALES ---
// -------------------------

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CTestProcess::CTestProcess ( void )
	: m_VectRatoli				( 0, 0)
	, m_VectScreen				( 800, 600 )
	, m_fTempsTotal				( 0 )
	, m_uiIndicePlayerCamera	( 0 )
	, m_pActivePlayer			( NULL )	
	, m_pCameraTerceraPersona	( NULL )
	, m_pCameraPrimeraPersona	( NULL )
{}

CTestProcess::~CTestProcess ( void )
{
	Done ();
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
void CTestProcess::Done ( void )
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void CTestProcess::Release ( void )
{
	CHECKED_DELETE ( m_pCameraPrimeraPersona );
	CHECKED_DELETE ( m_pCameraTerceraPersona );
	m_pCamera = NULL;
}

bool CTestProcess::Init(void)
{
	m_VectScreen = CORE->GetRenderManager()->GetScreenSize( );
	m_VectRatoli.x = m_VectScreen.x / 2; 
	m_VectRatoli.y = m_VectScreen.y / 2;
	m_fAngleX = 0.f;
	m_fAngleY = 0.f;
	m_fAngleZ = 0.f; 
	

	CORE->GetScriptManager()->RunCode("load_basics()");
	CORE->GetScriptManager()->RunCode("load_data()");

	// TODO: Substituir tot per rails de càmeres o altres punts d'anclatge de les càmeres. En el procés que hereda d'aquest sí s'ha de fer l'anclatge la Player
	/*AddPlayer ( "Player", Vect3f ( -40.f, 20.f, 0.f ), false );
	AddPlayer ( "Pepe",   Vect3f ( 1.f, 2.f, 1.f ),    false );*/

	//float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	//m_pCameraTerceraPersona = new CThPSCamera (1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_pPlayersList[m_uiIndicePlayerCamera], 10.0f );
	//m_pCameraPrimeraPersona = new CFPSCamera ( 0.f , 1.f ,  D3DX_PI / 4, static_cast <float> ( m_VectScreen.x/m_VectScreen.y), m_pPlayersList[m_uiIndicePlayerCamera] );
	//m_pCamera = static_cast<CCamera*>( m_pCameraTerceraPersona );

	//// Assignamos el player por defecto
	//m_pActivePlayer = m_pPlayersList[m_uiIndicePlayerCamera];

	//// Asignamos la camera activa al core
	//CORE->SetCamera( m_pCamera );
	//
	//// La primera Player-Camera
	//m_uiIndicePlayerCamera = 0;

	return true;
}

void CargarProcesos ( void )
{
	// TODO: La idea es meter processos aquí o classes que generen tests	
	

}

void CTestProcess::Update( float _ElapsedTime )
{
	CORE->SetCamera( m_pCamera );
	m_pActivePlayer->Update( _ElapsedTime );
	
	UpdateInputs( _ElapsedTime );

	CORE->GetRenderableObjectsLayersManager()->Update( _ElapsedTime );
}

void CTestProcess::Render( CRenderManager *_RM )
{
}

// -----------------------------------------
//				METODES PRIVATS
// -----------------------------------------

void CTestProcess::AddPlayer ( std::string _Name,	Vect3f _vPosition, bool _Automatic )
{
	CPlayer* pPlayer = new CPlayer ( _Name );
	if ( pPlayer->Init() )
	{	
		pPlayer->SetPosition		( _vPosition );
		pPlayer->SetMoveAlone		( _Automatic );
		m_pPlayersList.push_back	( pPlayer );
	}
	else
		CHECKED_DELETE ( pPlayer );
}

void CTestProcess::UpdateInputs( float _ElapsedTime )
{
	CActionToInput *l_pAction2Input = CORE->GetActionToInput();
	
	if ( l_pAction2Input->DoAction ( ACTION_CAMERA_SWITCH ) )		// Conmutacio de camera i visualitzo el que veu el jugador
	{		
		//TC_FPS
		CCamera * l_Camera = GetCamera();
		if ( ( l_Camera->GetTypeCamera ( ) ) == CCamera::TC_FPS )
			// Asignamos cámera de primera persona
			m_pCamera = m_pCameraTerceraPersona;
		else
			// Asignamos cámera de tercera persona
			m_pCamera = m_pCameraPrimeraPersona;
	}	
	
	if ( l_pAction2Input->DoAction( ACTION_PLAYER_SWITCH ) )		// Conmutacio de jugador. Ara puc moure l'actual jugador i el vell es mou automàticament
	{
		size_t l_Counter = m_pPlayersList.size() - 1;
		if ( m_uiIndicePlayerCamera < l_Counter ) 
			m_uiIndicePlayerCamera ++;
		else
			m_uiIndicePlayerCamera = 0;
		
		m_pCamera->SetObject3D ( m_pPlayersList[m_uiIndicePlayerCamera] );
		m_pActivePlayer = m_pPlayersList[m_uiIndicePlayerCamera];
	}
}

