#include "GameProcess.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"
#include "Scripting\ScriptManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "ActionToInput.h"
#include "RegisterToLua.h"
#include "Core.h"
#include "Base.h"

#include "VideogameDefs.h"
#include "Characters\CharacterManager.h"

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

CGameProcess::CGameProcess(HWND hWnd)
	: m_hWnd(hWnd)
	, m_pThPSCamera(NULL)
	, m_pCharacterManager(NULL)
	, m_IsOK(false)
{
}

CGameProcess::~CGameProcess(void)
{
	CHECKED_DELETE(m_pCharacterManager);
	CHECKED_DELETE( m_pThPSFreeCamera );
	CHECKED_DELETE( m_pThPSCamera );
}

bool CGameProcess::Init()
{
	//Registra los métodos
	RegisterMethods();

	//Crea los datos para el gameplay
	m_pCharacterManager = new CCharactersManager();

	//Carga los datos en el engine
	if( INIT_GUI )
	{
		CORE->GetScriptManager()->RunCode("load_data()");
	}
	else
	{
		CORE->GetScriptManager()->RunCode("load_all()");
	}

	//Crea una cámara libre de Debug
	m_FreeCamera.SetPosition(Vect3f( 0.f, 10.f, 0.f));
	m_FreeCamera.SetPitch(-D3DX_PI/6);
	m_FreeCamera.SetYaw(0.0f);
	m_FreeCamera.SetRoll(0.0f);

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSFreeCamera = new CThPSCamera( 1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_FreeCamera, 10.0f, 0.f, 0.f, "Free");
	m_pFreeCamera = static_cast<CCamera*>(m_pThPSFreeCamera);

	//Carga los objetos del juego
	LoadGameObjects();

	return true;
}

void CGameProcess::CreatePlayerCamera(float _near, float _far, float _zoom, float _heightEye, float _heightLookAt, const std::string &_name)
{
	CHECKED_DELETE( m_pThPSCamera );

	float aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(_near, _far, 45.f * D3DX_PI / 180.f, aspect, m_pCharacterManager->GetPlayer(), _zoom, _heightLookAt, _heightEye, _name);
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);
}

void CGameProcess::Update(float elapsedTime)
{
	//Vuelve a cargar los datos si hacemos el reload de LUA
	if( CORE->GetActionToInput()->DoAction("ReloadScripts") )
	{
		LoadGameObjects();
		m_pThPSCamera->SetObject3D( m_pCharacterManager->GetPlayer());
	}

	if( CORE->GetActionToInput()->DoAction("CommutationCamera") )
	{
		if( m_pCamera == m_pThPSCamera )
		{
			m_pCamera = m_pThPSFreeCamera;
			CORE->SetCamera( m_pThPSFreeCamera );
			m_pCharacterManager->GetPlayer()->SetLocked( true );
		}
		else
		{
			m_pCamera = m_pThPSCamera;
			CORE->SetCamera( m_pThPSCamera );
			m_pCharacterManager->GetPlayer()->SetLocked( false );
		}
	}

	if( m_pCharacterManager->GetPlayer()->GetLocked() )
	{
		m_FreeCamera.Update(elapsedTime ,m_pCamera);
	}

	m_pCharacterManager->Update(elapsedTime);
	CORE->GetRenderableObjectsLayersManager()->Update(elapsedTime);
}

void CGameProcess::Render(CRenderManager &RM)
{
}

bool CGameProcess::LoadMainScript( void )
{
	return SCRIPT->Load("./Data/XML/script_gameplay.xml");
}

void CGameProcess::LoadGameObjects()
{
	CHECKED_DELETE(m_pCharacterManager);

	//Crea los datos para el gameplay
	m_pCharacterManager = new CCharactersManager();

	//Crea escena debug 
	//m_pScene = new CScene();

	//Carga los scripts del juego
	m_IsOK = LoadMainScript();

	if ( !m_IsOK )
		return;

	// por si se desea hacer alguna mariconada...
	//SCRIPT->RunCode("init_game_data()");

	// Inicializa el gestor de player y enemigos. Carga propiedades y estados de todo.
	if ( !m_pCharacterManager->Initialize ( ) )
		return;

	CCharacter * l_Player = m_pCharacterManager->GetPlayer();

	//Crea la cámara
	/*float l_Aspect = CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera = new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, l_Aspect, m_pCharacterManager->GetPlayer(), 12.0f, 4.f, 0.f, "Caperucita");
	m_pCamera = static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);*/
}

CGameProcess* CGameProcess::GetGameProcess()
{
	CEngineProcess *proces = CORE->GetProcess();
	return static_cast<CGameProcess*>(proces);
}

//-------------------------------------
//--Registrador de métodos en LUA------
//-------------------------------------
void CGameProcess::RegisterMethods()
{
	lua_State *state = SCRIPT->GetLuaState();

	module(state) [
		def("get_game_process", GetGameProcess)
	];

	module(state) [
		class_<CGameProcess>("CGameProcess")
			.def("get_character_manager", &CGameProcess::GetCharacterManager)
			.def("create_player_camera", &CGameProcess::CreatePlayerCamera)
			.property("player_camera", &CGameProcess::GetPlayerCamera)
	];

	RegisterToLuaBaseGameEntity			( state );
	RegisterToLuaCharacter				( state );
	RegisterToLuaCharacterManager		( state );
	RegisterToLuaStateMachine			( state );
	RegisterToLuaState					( state );
	RegisterToLuaProperties				( state );
}