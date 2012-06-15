#include "GameProcess.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "StatesMachine\EntityManager.h"
#include "ActionToInput.h"
#include "RegisterToLua.h"
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
	: m_hWnd				(hWnd)
	, m_pThPSCamera			(NULL)
	, m_pCharactersManager	(NULL)
	, m_IsOK				(false)
	, m_fTimeBetweenClicks	(0.f)
{
}

CGameProcess::~CGameProcess( void )
{
	CleanUp();
}

// -----------------------------------------
//			METODES PRINCIPALS
// -----------------------------------------
bool CGameProcess::Init( void )
{
	//Registra los métodos
	RegisterMethods();

	//Carga los datos en el engine
	if( INIT_GUI )
	{
		CORE->GetScriptManager()->RunCode("load_data()");
	}
	else
	{
		CORE->GetScriptManager()->RunCode("load_all()");
	}

	//Carga los objetos del juego
	LoadGameObjects();

	return true;
}

void CGameProcess::CleanUp( void )
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
}

void CGameProcess::CreatePlayerCamera(float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name)
{
	CHECKED_DELETE( m_pThPSCamera );

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(_near, _far, 45.f * D3DX_PI / 180.f, aspect,  m_pCharactersManager->GetPlayer(), _zoom, _heightLookAt, _heightEye, _name);
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);
}

void CGameProcess::CreateFreeCamera(float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name)
{
	//Crea una cámara libre de Debug
	m_FreeCamera.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_FreeCamera.SetPitch(-D3DX_PI/6);
	m_FreeCamera.SetYaw(0.0f);
	m_FreeCamera.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSFreeCamera = new CThPSCamera( _near, _far, 45.f * D3DX_PI / 180.f, aspect, &m_FreeCamera, _zoom, _heightLookAt, _heightEye, _name);
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);
}

void CGameProcess::Update(float elapsedTime)
{
	if (!m_IsOK) 
	{
		return;
	}
	
	//Vuelve a cargar los datos si hacemos el reload de LUA
	if( CORE->GetActionToInput()->DoAction("ReloadScripts") )
	{
		ReloadGameObjects();
		m_pThPSCamera->SetObject3D( m_pCharactersManager->GetPlayer());
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

	m_fTimeBetweenClicks += elapsedTime;
	if( CORE->GetActionToInput()->DoAction("AttackPlayer") )
	{
		m_fTimeBetweenClicks = 0.f;
	}

	m_pCharactersManager->Update(elapsedTime);
	CORE->GetRenderableObjectsLayersManager()->Update(elapsedTime);
}

void CGameProcess::ReloadGameObjects( void )
{
	CleanUp();
	//SCRIPT->RegisterLUAMethods();
	LoadGameObjects();
}

void CGameProcess::Render(CRenderManager &RM)
{
	 m_pCharactersManager->Render(&RM, CORE->GetFontManager());
}

bool CGameProcess::LoadMainScript( void )
{
	return SCRIPT->Load("./Data/XML/script_gameplay.xml");
}

void CGameProcess::LoadGameObjects()
{
	//Crea los datos para el gameplay
	m_pCharactersManager = new CCharactersManager();
	
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
}

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGameProcess::RegisterMethods()
{
	lua_State * l_State = SCRIPT->GetLuaState();

	module(l_State) [
		def("get_game_process", GetGameProcess)
	];

	module(l_State) [
		class_<CGameProcess>("CGameProcess")
			.def("get_character_manager", &CGameProcess::GetCharactersManager)
			.def("create_player_camera", &CGameProcess::CreatePlayerCamera)
			.def("get_time_between_clicks", &CGameProcess::GetTimeBetweenClicks)
			.property("player_camera", &CGameProcess::GetPlayerCamera)
	];

	RegisterToLuaTelegram				( l_State );
	RegisterToLuaBaseGameEntity			( l_State );
	RegisterToLuaCharacter				( l_State );
	RegisterToLuaCharacterManager		( l_State );
	RegisterToLuaStateMachine			( l_State );
	RegisterToLuaState					( l_State );
	RegisterToLuaProperties				( l_State );
}

// -----------------------------------------
//				PROPIEDADES
// -----------------------------------------

CGameProcess* CGameProcess::GetGameProcess()
{
	CEngineProcess *proces = CORE->GetProcess();
	return static_cast<CGameProcess*>(proces);
}