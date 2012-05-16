#include "TestProcess.h"

#include "Commands\SceneRendererCommandManager.h"
#include "Scripting\ScriptManager.h"
#include "ActionToInput.h"
#include "Base.h"
#include "Core.h"

#include "Cameras\Camera.h"
#include "Cameras\FPSCamera.h"
#include "Cameras\ThPSCamera.h"

//#include "Player.h"

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
	//, m_pActivePlayer			( NULL )	
	, m_pThPSCamera				( NULL )
	, m_pFPSCamera				( NULL )
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
	CHECKED_DELETE ( m_pThPSCamera );
	CHECKED_DELETE ( m_pFPSCamera );
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

	return true;
}

void CargarProcesos ( void )
{
	// TODO: La idea es meter processos aquí o classes que generen tests	
	

}

void CTestProcess::Update( float _ElapsedTime )
{
	CORE->SetCamera( m_pCamera );
	
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
	/*CPlayer* pPlayer = new CPlayer ( _Name );
	if ( pPlayer->Init() )
	{	
		pPlayer->SetPosition		( _vPosition );
		pPlayer->SetMoveAlone		( _Automatic );
		m_pPlayersList.push_back	( pPlayer );
	}
	else
		CHECKED_DELETE ( pPlayer );*/
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
			m_pCamera = m_pThPSCamera;
		else
			// Asignamos cámera de tercera persona
			m_pCamera = m_pFPSCamera;
	}	
	
	//if ( l_pAction2Input->DoAction( ACTION_PLAYER_SWITCH ) )		// Conmutacio de jugador. Ara puc moure l'actual jugador i el vell es mou automàticament
	//{
	//	size_t l_Counter = m_pPlayersList.size() - 1;
	//	if ( m_uiIndicePlayerCamera < l_Counter ) 
	//		m_uiIndicePlayerCamera ++;
	//	else
	//		m_uiIndicePlayerCamera = 0;
	//	
	//	m_pCamera->SetObject3D ( m_pPlayersList[m_uiIndicePlayerCamera] );
	//	m_pActivePlayer = m_pPlayersList[m_uiIndicePlayerCamera];
	//}
}

