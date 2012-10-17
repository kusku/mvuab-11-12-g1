#include "GameProcess.h"
#include "_ScriptAPI\VideogameRegisterScript.h"
#include "Cameras\ThPSCharacterCamera.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "StatesMachine\EntityManager.h"
#include "ActionToInput.h"
#include "GUIManager.h"
#include "Core.h"
#include "Base.h"
#include "Particles\ParticleEmitterManager.h"
#include "Particles\ParticleEmitter.h"
#include "SoundManager.h"
#include "Listener.h"
#include "Triggers/TriggersManager.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"

#include "Rails\RailManager.h"
#include "Rails\Rail.h"
#include "Cameras\FPSCamera.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedModelManager.h"

#include "Utils\MapManager.h"
#include "Math\Matrix33.h"

#include "HUD\HUD.h"

#include "Core.h"
#include "Base.h"

#include "VideogameDefs.h"
#include "Characters\CharacterManager.h"

#include "Callbacks\State\ActionStateCallback.h"

#if defined (_DEBUG)
	#include "Memory/MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CGameProcess::CGameProcess( HWND hWnd )
	: m_hWnd						(hWnd)
	, m_pThPSCamera					(NULL)
	, m_pCharactersManager			(NULL)
	, m_pAnimationCallbackManager	(NULL)
	, m_pHUD						(NULL)
	, m_pFPSRailCamera				(NULL)
	, m_IsOK						(false)
	, m_bIntroFinished				(false)
	, m_uRailCounter				(0)
	, m_fTimeBetweenClicks			(0.f)
	, m_uElapsedIndex				(0)
	, m_bStablishFramerateState		(true)
	, m_bStartRails					(false)
	, m_bStartAnalise				(false)
	, m_bFirstLogicUpdate			(true)
	, m_pActionStateCallback		(NULL)
{
	for(uint8 i=0; i<30; ++i)
		m_fElapseds[i] = 0.f;
}

CGameProcess::~CGameProcess()
{
	CleanUp();
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CGameProcess::Init()
{
	//Registra los métodos
	//RegisterMethods();

	if( !INIT_GUI )
	{
		CORE->GetScriptManager()->RunCode("load_basics()");

		CGameProcess::RegisterMethods();
			
		CORE->GetScriptManager()->RunCode("load_data()");
	} 

	//Carga los objetos del juego
	LoadGameObjects();

	m_bFirstLogicUpdate = true;

	// Raíles
	CORE->SetCamera(m_pRailCamera);

	m_uRailCounter = 0;
	m_bIntroFinished = !INIT_RAILS;

	m_bStablishFramerateState = true;
	m_bStartRails = false;

	m_bStartAnalise = false;
	m_uElapsedIndex = 0;
	for(uint8 i=0; i<30; ++i)
	{
		m_fElapseds[i] = 0.f;
	}
	
	return true;
}

void CGameProcess::CleanUp()
{
	//m_FreeCamera = NULL;
	//m_StaticCamera = NULL;
	ENTMGR->RemoveEntities();

	
	m_pCamera = NULL;
	CHECKED_DELETE( m_pThPSFreeCamera );
	CHECKED_DELETE( m_pThPSCamera );
	//CHECKED_DELETE( m_pFreeCamera );
	m_pFreeCamera = NULL;

	CHECKED_DELETE(m_pFPSRailCamera);
	CHECKED_DELETE(m_pObjectRail);
	m_pRailCamera = NULL;

	CHECKED_DELETE(m_pActionStateCallback);

	CHECKED_DELETE( m_pCharactersManager );
	CHECKED_DELETE( m_pAnimationCallbackManager );
	CHECKED_DELETE( m_pHUD );
}

CThPSCharacterCamera* CGameProcess::CreatePlayerCamera(float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name)
{
	CHECKED_DELETE( m_pThPSCamera );

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCharacterCamera(_near, _far, 45.f * D3DX_PI / 180.f, aspect,  m_pCharactersManager->GetPlayer(), _zoom, _heightLookAt, _heightEye, _name);
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	m_pCamera->GetObject3D()->SetYaw(2.36f);
	CORE->SetCamera(m_pCamera);

	return m_pThPSCamera;
}

void CGameProcess::CreateRailCamera(float _near, float _far)
{
	m_pObjectRail = new CObject3D(v3fZERO, v3fUNIT, 0.f, 0.f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pFPSRailCamera = new CFPSCamera(_near, _far, 45.f * D3DX_PI / 180.f, aspect,  m_pObjectRail);
	m_pRailCamera = static_cast<CCamera*>(m_pFPSRailCamera);
}

void CGameProcess::CreateFreeCamera( float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name )
{
	//Crea una cámara libre de Debug
	m_FreeCamera.SetPosition(Vect3f( 64.f, 5.f, -19.f));
	m_FreeCamera.SetPitch(-D3DX_PI/4);
	m_FreeCamera.SetYaw(-1.0f);
	m_FreeCamera.SetRoll(0.0f);
	
	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSFreeCamera = new CThPSCamera( _near, _far, 45.f * D3DX_PI / 180.f, aspect, &m_FreeCamera, _zoom, _heightLookAt, _heightEye, _name);
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);
	CORE->SetCamera(m_pCamera);
}

void CGameProcess::Update(float elapsedTime)
{
	if (!m_IsOK) 
	{
		return;
	}
	
	if( INIT_GUI )
	{
		if( CORE->GetActionToInput()->DoAction("PauseGame") )
		{
			SCRIPT->RunCode("go_to_pause_game()");
		}
	}

	if( CORE->IsGameMode() )
	{
		// Controla la estabilización del frame rate al inicio
		if(m_bStablishFramerateState)
		{
			m_fElapseds[ m_uElapsedIndex ] = elapsedTime;

			if( m_uElapsedIndex >= 30 )
			{
				m_uElapsedIndex = 0;
				m_bStartAnalise = true;
			}
			else
				++m_uElapsedIndex;

			if( m_bStartAnalise )
			{
				float average = 0.f;
				for(uint8 i=0; i<30; ++i)
				{
					average += m_fElapseds[i];
				}
				average /= 30.f;

				if( mathUtils::Abs(average - elapsedTime) < 0.1f)
				{
					m_bStablishFramerateState = false;
					m_bStartRails = true;
				}
			}

			return;
		}

		// Lanza los raíles
		if( INIT_RAILS && m_bStartRails )
		{
			SCRIPT->RunCode("presentation_init()");
			m_bStartRails = false;
		}

#if defined (_DEBUG)
		//Vuelve a cargar los datos si hacemos el reload de LUA
		if( CORE->GetActionToInput()->DoAction("ReloadGamePlayScripts") )
		{
			ReloadGameObjects();
			m_pThPSCamera->SetObject3D( m_pCharactersManager->GetPlayer());

			m_pHUD->Reload();
			m_pHUD->Init( m_pCharactersManager->GetPlayerLife() );

			CORE->GetTriggersManager()->Reload();

			m_bFirstLogicUpdate = true;
		}

		if( CORE->GetActionToInput()->DoAction("CommutationCamera") )
		{
			if( m_pCamera == m_pThPSCamera )
			{
				m_pCamera = m_pThPSFreeCamera;
				CORE->SetCamera( m_pThPSFreeCamera );
				m_pCharactersManager->GetPlayer()->SetLocked( true );
			}
			else
			{
				m_pCamera = m_pThPSCamera;
				CORE->SetCamera( m_pThPSCamera );
				m_pCharactersManager->GetPlayer()->SetLocked( false );
			}
		}

		if( m_pCharactersManager->GetPlayer()->GetLocked() )
		{
			m_FreeCamera.Update(elapsedTime ,m_pCamera);
		}
#endif
		if( m_bIntroFinished )
		{
			if( m_bFirstLogicUpdate )
			{
				m_pHUD->SetActivePlayerBar(true);
				m_bFirstLogicUpdate = false;
			}

			CORE->SetCamera( m_pCamera );

			if( CORE->GetActionToInput()->DoAction("GoToMenu") )
			{
				SCRIPT->RunCode("change_to_end_gui_process()");
			}

			m_fTimeBetweenClicks += elapsedTime;
			if( CORE->GetActionToInput()->DoAction("AttackPlayer") )
			{
				m_fTimeBetweenClicks = 0.f;
				//CORE->GetParticleEmitterManager()->GetResource("Explosions")->EjectParticles();
			}

			m_pCharactersManager->Update(elapsedTime);
			CORE->GetRenderableObjectsLayersManager()->Update(elapsedTime);

			//Actualiza el HUD
			m_pHUD->Update(elapsedTime, m_pCharactersManager->GetPlayerLife() );
		}
		else
		{
			if( CORE->GetActionToInput()->DoAction(ACTION_FINISH_RAIL_ACTIONS) )
			{
				CORE->GetRailManager()->StopCurrentRail();
				m_pHUD->ActiveTexture("controls", false);
				m_bIntroFinished = true;
			}

			SCRIPT->RunCode("presentation_control()");
		}

		UpdateWindSounds(elapsedTime);
	}
	
}

void CGameProcess::ReloadGameObjects()
{	
	CleanUp();
	//SCRIPT->RegisterLUAMethods();
	LoadGameObjects();
}

void CGameProcess::Render(CRenderManager &RM)
{
	m_pHUD->Render(RM);
}

void CGameProcess::DebugRender( CRenderManager &RM )
{
#if defined (_DEBUG)
	m_pCharactersManager->Render(&RM, CORE->GetFontManager());
	//m_pThPSCamera->Render(&RM);
	
	Mat44f mat;
	mat.SetIdentity();
	mat.Translate(m_pRailCamera->GetEye());
	RM.SetTransform(mat);

	RM.DrawSphere(2.f, 10, colCYAN);

	mat.SetIdentity();
	RM.SetTransform(mat);

	RM.DrawLine(m_pRailCamera->GetEye(), m_pRailCamera->GetEye() + m_pRailCamera->GetDirection(), colYELLOW );
	
#endif
}

bool CGameProcess::LoadMainScript()
{
	return SCRIPT->Load("./Data/XML/script_gameplay.xml");
}

void CGameProcess::LoadGameObjects()
{
	//Crea el gestor de callbacks de animaciones
	m_pAnimationCallbackManager = new CAnimationCallbackManager();

	//Crea los datos para el gameplay
	m_pCharactersManager = new CCharactersManager();

	//Crea el HUD
	m_pHUD = new CHud();
	m_pHUD->Load("./Data/XML/hud.xml");

	//Crea escena debug 
	//m_pScene = new CScene();

	//Carga los scripts del juego
	m_IsOK = LoadMainScript();

	if ( !m_IsOK )
		return;

	CBaseGameEntity::Initialize();

	// por si se desea hacer alguna mariconada...
	SCRIPT->RunCode("init_game_data()");
	
	// Inicializa el gestor de player y enemigos. Carga propiedades y estados de todo.
	if ( !m_pCharactersManager->Initialize ( ) )
		return;

	CreateFreeCamera(  1.0f, 10000.f, 10.0f, 0.f, 0.f, "Free" );	
	CreateRailCamera( 1.f, 10000.f );

	//Asigna una cámara al micrófono de sonido
	CORE->GetSoundManager()->GetListener()->SetCamera( m_pThPSCamera );

	m_pHUD->Init( m_pCharactersManager->GetPlayerLife() );

	// Actualizo sonidos de viento cada ciertos segundos en todos los speakers
	m_pActionStateCallback = new CActionStateCallback(0, 0.f, 1.f);
	m_pActionStateCallback->InitAction(0, 0, 1);
	m_pActionStateCallback->StartAction();
	UpdateWindSounds(0.f);
}

void CGameProcess::UpdateWindSounds( float _ElapsedTime )
{
	m_pActionStateCallback->Update(_ElapsedTime);
	if ( m_pActionStateCallback->IsActionFinished() )
	{
		m_pActionStateCallback->InitAction(0, 21.f, 60.f);
		m_pActionStateCallback->StartAction();

		CORE->GetSoundManager()->PlayEvent("zona1_vientos001", "Play_EFX_wind1" );
		CORE->GetSoundManager()->PlayEvent("zona1_vientos002", "Play_EFX_wind1" );
	}
}

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGameProcess::ChangeToGUIProcess()
{
	PostMessage(m_hWnd, WM_GUI_PROCESS, 0, 0);
}

void CGameProcess::RegisterMethods()
{
	lua_State * l_State = SCRIPT->GetLuaState();

	module(l_State) [
		def("get_game_process", GetGameProcess)
	];

	Videogame::ScriptAPI::RegisterScript(l_State);
}

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------

CGameProcess* CGameProcess::GetGameProcess()
{
	CEngineProcess *proces = CORE->GetProcess();
	return static_cast<CGameProcess*>(proces);
}

void CGameProcess::PreReload( void )
{
	m_pCharactersManager->SetAllEnemiesSoundOff();
}

void CGameProcess::Reload( void )
{
	ReloadGameObjects();
}