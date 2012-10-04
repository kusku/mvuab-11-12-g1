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
#include "Weapons\WeaponManager.h"
#include "Callbacks\Animation\AnimationCallbackManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "HUD\HUD.h"

#include "Core.h"
#include "Base.h"

#include "VideogameDefs.h"
#include "Characters\CharacterManager.h"

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
	, m_pWeaponManager				(NULL)
	, m_pAnimationCallbackManager	(NULL)
	, m_pHUD						(NULL)
	, m_IsOK						(false)
	, m_fTimeBetweenClicks			(0.f)
{
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

	CHECKED_DELETE( m_pCharactersManager );
	CHECKED_DELETE( m_pWeaponManager );
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

void CGameProcess::CreateFreeCamera(float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name)
{
	//Crea una cámara libre de Debug
	m_FreeCamera.SetPosition(Vect3f( 50.f, 5.f, -100.f));
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
#if defined (_DEBUG)
		//Vuelve a cargar los datos si hacemos el reload de LUA
		if( CORE->GetActionToInput()->DoAction("ReloadGamePlayScripts") )
		{
			ReloadGameObjects();
			m_pThPSCamera->SetObject3D( m_pCharactersManager->GetPlayer());

			m_pHUD->Reload();
			m_pHUD->Init( m_pCharactersManager->GetPlayerLife() );

			CORE->GetTriggersManager()->Reload();
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

		//Actualiza la posición de las armas
		m_pWeaponManager->Update(elapsedTime);

		//Actualiza el HUD
		m_pHUD->Update(elapsedTime, m_pCharactersManager->GetPlayerLife() );

	}

	//SCRIPT->RunCode("collectgarbage('collect')");
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
	m_pThPSCamera->Render(&RM);
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

	//Asigna una cámara al micrófono de sonido
	CORE->GetSoundManager()->GetListener()->SetCamera( m_pThPSCamera );

	m_pWeaponManager = new CWeaponManager();
	m_pWeaponManager->Load("./Data/XML/weapons.xml");
	m_pWeaponManager->ChangeCurrentWeapon("hoces");

	m_pHUD->Init( m_pCharactersManager->GetPlayerLife() );
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